#define PRH_STRIP_PREFIX
#include "coroutine.h"
#include <stdio.h>
#include <string.h>

#define magic_coro_debug magic_debug
#define magic_lower_guard_word 0x5a5a5a5a
#define magic_upper_guard_word 0xa5a5a5a5

#if defined(magic_cl_msc)
#define aligned_memalloc(size) _aligned_malloc(size, 16)
#define aligned_memfree(p) _aligned_free(p)
#else
void *aligned_alloc(size_t alignment, size_t size);
#define aligned_memalloc(size) aligned_alloc(16, size)
#define aligned_memfree(p) free(p)
#endif

magic_fastcall(intp) asm_stack_init_depth(void);
magic_fastcall(void) asm_coro_call(void); // args on coro stack

magic_fastcall(void) asm_call_stack_crash(void *co_rsp, void *rsp)
{
    printf("coroutine stack corruption: rsp %p %p\n", co_rsp, rsp);
    exit(1);
}

void coroutine_stack_segmentation_fault(struct coro *coro)
{
    printf("stack segmentation fault: coro %p\n", (void *)coro);
    exit(2);
}

// coroutine stack layout:
//
//  lower memery address
//  [coroutine userdata area    ]
//  [struct coroguard           ]
//  [       ...                 ]
//  [       ...                 ]          ^
//  [       ...                 ]          ^
//  [stack memory data          ] <-- rsp  | <-- stack top
//  [stack memory data          ]
//  [stack memory data          ] <-- stack bottom
//  [struct coro: rspoffset     ] <-- coro
//  upper memory address
struct coro {
    unsigned rspoffset; // 1st field dont move
    unsigned loweraddr;
    unsigned maxudsize: 31, ptr_param: 1;
    int coro_id;
};

struct coro_main {
    unsigned rspoffset; // 1st field dont move
    unsigned yield_cycle;
};

struct coroguard {
    unsigned lower_guard_word;
    unsigned upper_guard_word;
};

magic_inline void *coroutinegetuserdata(struct coro *coro)
{
    return (char *)coro - coro->loweraddr;
}

magic_inline struct coroguard *coroutinegetguard(struct coro *coro)
{
    void *p = coroutinegetuserdata(coro);
    return (struct coroguard *)((char *)p + coro->maxudsize);
}

void *coroutine_userdata(struct coro *coro)
{
    void *p = coroutinegetuserdata(coro);
    if (coro->ptr_param) return *(void **)p;
    return ((int)coro->maxudsize > 0) ? p : NULL;
}

int coroutine_id(struct coro *coro)
{
    return coro->coro_id;
}

static_must(sizeof(struct coro) <= 16);

// coro_struct layout:
//  [struct corostruct]
//  [struct coro *]
//  [struct coro *]
//  ...
typedef struct coro_struct {
    coro_struct *chain; // 1st field dont move
    struct coro *curr_coro; // 2nd field dont move
    struct coro_main *main_coro; // 3rd field dont move
    unsigned maxcoros: 31, inplace: 1;
    int coro_cnt;
} coro_struct;

magic_inline int corostructallocsize(int maxcoros)
{
    return (int)(sizeof(coro_struct) + maxcoros * sizeof(void *));
}

int coroutine_alloc_size(int maxcoros)
{
    assert(maxcoros >= 0);
    return corostructallocsize(maxcoros);
}

coro_struct *coroutine_init_inplace(void *addr, int maxcoros)
{
    assert(maxcoros >= 0);
    coro_struct *s = (coro_struct *)addr;
    s->maxcoros = maxcoros;
    s->inplace = 1;
    return s;
}

coro_struct *coroutine_init(int maxcoros)
{
    int alloc_size = coroutine_alloc_size(maxcoros);
    coro_struct *s = (coro_struct *)calloc(1, alloc_size);
    coroutine_init_inplace(s, maxcoros);
    s->inplace = 0;
    return s;
}

void coroutinestackguard(struct coro *coro)
{
    struct coroguard *g = coroutinegetguard(coro);
    if (g->lower_guard_word != magic_lower_guard_word || g->upper_guard_word != magic_upper_guard_word) {
        coroutine_stack_segmentation_fault(coro);
    }
}

magic_inline int coroutineselfsize(int maxudsize)
{
    return 16 + magic_align(maxudsize) + sizeof(struct coroguard); // 16 for struct coro
}

struct coro *coro_alloc(int stack_size, int userdata_bytes)
{
    if (userdata_bytes < 0) userdata_bytes = 0;

    assert(stack_size > coroutineselfsize(userdata_bytes) + asm_stack_init_depth());

    int stackallocsize = magic_align_16_byte(stack_size);
    char *p = (char *)aligned_memalloc(stackallocsize);
    struct coro *coro = (struct coro *)(p + stackallocsize - 16);
    coro->rspoffset = 0;
    coro->loweraddr = (unsigned)((char *)coro - p);
    coro->maxudsize = magic_align(userdata_bytes);
    coro->ptr_param = 0;

    struct coroguard *guard = coroutinegetguard(coro);
    guard->lower_guard_word = magic_lower_guard_word;
    guard->upper_guard_word = magic_upper_guard_word;
    return coro;
}

struct coro **coroutinelist(coro_struct *main)
{
    return (struct coro **)(main + 1);
}

struct coro *coroutinealloc(coro_struct *main, int stack_size, int userdata_bytes)
{
    if ((unsigned)main->coro_cnt >= main->maxcoros) return NULL;
    struct coro *coro = coro_alloc(stack_size, userdata_bytes);
    coroutinelist(main)[main->coro_cnt] = coro;
    main->coro_cnt += 1;
    coro->coro_id = main->coro_cnt;
    return coro;
}

#define CORO_INIT_PTR_PARAM 1
#define CORO_ZERO_USER_DATA 2
#define CORO_UPDT_PTR_PARAM 3

static void coroutineresetuserdata(struct coro *coro, void *userdata, int cond)
{
    void *p = coroutinegetuserdata(coro);
    if ((int)coro->maxudsize <= 0) return;
    if (cond == CORO_INIT_PTR_PARAM) {
        coro->ptr_param = 1;
        *(void **)p = userdata;
    } else if (cond == CORO_ZERO_USER_DATA) {
        memset(p, 0, coro->maxudsize);
    } else if (cond == CORO_UPDT_PTR_PARAM) {
        if (coro->ptr_param) {
            *(void **)p = userdata;
        }
    }
}

void *coroutinelaunch(struct coro *coro, coroproc proc, void *userdata, int cond)
{
    if (coro == NULL) return NULL;

    coroutinestackguard(coro);

    coroutineresetuserdata(coro, userdata, cond);

    void **rsp = (void **)coro;
    void *rspaligned_to_16 = (void *)rsp;
    // [32-bit architecture]
    // pstack + alloc <-- 00 <-- 16字节对齐
    //           proc <-- 12
    //                <-- 08 asm_coro_call
    // [64-bit architecture]
    // pstack + alloc <-- 00 <-- 16字节对齐
    //           proc <-- 08
    //                <-- 00 asm_coro_call
    *(--rsp) = (void *)(intp)proc;
    *(--rsp) = (void *)(intp)asm_coro_call;
    coro->rspoffset = (unsigned)((char *)coro - (char *)rsp);

    assert((intp)rspaligned_to_16 % 16 == 0);

#if magic_coro_debug
    void *udata = coroutinegetuserdata(coro);
    rsp = (void **)((char *)rspaligned_to_16 - asm_stack_init_depth());
    printf("[coro] %p #%d created %08x left %08x rsp %p depth %08x high %p proc %p\n",
        (void *)coro, coro->coro_id, (int)(coro->maxudsize + sizeof(struct coroguard)), (int)((char *)rsp - (char *)udata), (void *)rsp,
        (int)((char *)rspaligned_to_16 + 16 - (char *)rsp), (char *)rspaligned_to_16 + 16, (void *)(intp)proc);
#endif

    return coroutine_userdata(coro);
}

struct coro *coro_create(int coro_id, coroproc proc, int stack_size, void *userdata)
{
    struct coro *coro = coro_alloc(stack_size, sizeof(void *));
    if (coro) coro->coro_id = coro_id;
    coroutinelaunch(coro, proc, userdata, CORO_INIT_PTR_PARAM);
    return coro;
}

struct coro *coro_ext_create(int coro_id, coroproc proc, int stack_size, int userdata_bytes)
{
    struct coro *coro = coro_alloc(stack_size, userdata_bytes);
    if (coro) coro->coro_id = coro_id;
    coroutinelaunch(coro, proc, NULL, CORO_ZERO_USER_DATA);
    return coro;
}

void coro_reload(struct coro *coro, coroproc proc)
{
    if (coro && coro->rspoffset == 0) {
        coroutinelaunch(coro, proc, NULL, 0);
    }
}

void coro_ext_reload(struct coro *coro, coroproc proc, void *userdata)
{
    if (coro && coro->rspoffset == 0) {
        coroutinelaunch(coro, proc, userdata, CORO_UPDT_PTR_PARAM);
    }
}

void coroutine_create(coro_struct *main, coroproc proc, int stack_size, void *userdata)
{
    struct coro *coro = coroutinealloc(main, stack_size, sizeof(void *));
    coroutinelaunch(coro, proc, userdata, CORO_INIT_PTR_PARAM);
}

void *coroutine_ext_create(coro_struct *main, coroproc proc, int stack_size, int userdata_bytes)
{
    struct coro *coro = coroutinealloc(main, stack_size, userdata_bytes);
    return coroutinelaunch(coro, proc, NULL, CORO_ZERO_USER_DATA);
}

struct coro *coroutine_get(coro_struct *main, int index)
{
    assert(index > 0 && index <= main->coro_cnt);
    return coroutinelist(main)[index - 1];
}

void coroutine_reload(coro_struct *main, int index, coroproc proc)
{
    struct coro *coro = coroutine_get(main, index);
    coro_reload(coro, proc);
}

void coroutine_ext_reload(coro_struct *main, int index, coroproc proc, void *userdata)
{
    struct coro *coro = coroutine_get(main, index);
    coro_ext_reload(coro, proc, userdata);
}

// 跨线程协程在每个线程中都有一个主协程，每个线程可以有多个主协程，不管是线程主动创建多
// 个主协程，还是一个主协程涉及的子协程创建了主协程，一个线程同一时间只会处理一个主协程
// 及其关联的子协程组合，而且只有当最深层的组合处理完毕才会处理浅层的组合。
magic_thread_local coro_struct *MAIN_CORO_CHAIN = NULL;

void push_main_coro(coro_struct *main)
{
    main->chain = MAIN_CORO_CHAIN;
    MAIN_CORO_CHAIN = main;
}

coro_struct *pop_main_coro(void)
{
    coro_struct *top = MAIN_CORO_CHAIN;
    MAIN_CORO_CHAIN = top->chain;
    return top;
}

struct coro *coro_next_resume(coro_struct *main, int start_index)
{
    struct coro_main *mainstack = main->main_coro;
    for (int i = start_index; i < main->coro_cnt; i += 1) {
        struct coro *next = coroutinelist(main)[i];
        if (next && next->rspoffset) {
            return next;
        }
    }
    return (struct coro *)mainstack;
}

struct coro *prh_impl_next_resume_coro(void)
{
    coro_struct *top = MAIN_CORO_CHAIN;
    struct coro_main *main = top->main_coro;
    struct coro *curr = top->curr_coro;
    if (main->yield_cycle == 0) return (struct coro *)main;
    struct coro *next = coro_next_resume(top, curr->coro_id);
    if (next != (struct coro *)main) {
        top->curr_coro = next; // dont move out, solo need curr_coro unchanged
    }
    return next;
}

magic_inline bool coroutinemainyield(coro_struct *main, struct coro *next)
{
    push_main_coro(main);
    main->curr_coro = next;
    bool result = prh_impl_asm_coro_yield((struct coro *)main->main_coro, next);
    coroutinestackguard(next);
    pop_main_coro();
    return result;
}

magic_fastcall(void *) asm_coro_finish(struct coro *coro)
{
    coro->rspoffset = 0;
    coroutinestackguard(coro);
    return prh_impl_next_resume_coro();
}

bool coroutine_start_cycle(coro_struct *main)
{
    struct coro_main mainstack;
    main->main_coro = &mainstack;
    mainstack.yield_cycle = 1;
    struct coro *next = coro_next_resume(main, 0);
    if (next == (struct coro *)&mainstack) {
        return false;
    }
    return coroutinemainyield(main, next);
}

bool coroutine_start(coro_struct *main, int index)
{
    struct coro_main mainstack;
    main->main_coro = &mainstack;
    mainstack.yield_cycle = 0;
    struct coro *next = coroutine_get(main, index);
    if (next == NULL || next->rspoffset == 0) {
        return false;
    }
    return coroutinemainyield(main, next);
}

magic_inline void coroutinefree(struct coro *coro)
{
    aligned_memfree(coroutinegetuserdata(coro));
}

void coroutine_finish(coro_struct **main)
{
    coro_struct *s = *main;
    if (s == NULL) return;
    struct coro **start = coroutinelist(s);
    for (int i = 0; i < (int)s->coro_cnt; i += 1) {
        struct coro *coro = start[i];
        if (coro) {
            coroutinefree(coro);
            start[i] = NULL;
        }
    }
    *main = NULL;
    if (!s->inplace) {
        free(s);
    }
}

void solo_create(solo_struct *main, coroproc proc, int stack_size, void *userdata)
{
    if (main->curr_coro) return; // already initialized
    struct coro *solo = coro_create(1, proc, stack_size, userdata);
    if (solo) {
        main->curr_coro = solo;
    }
}

void *solo_ext_create(solo_struct *main, coroproc proc, int stack_size, int maxudsize)
{
    if (main->curr_coro) return NULL; // already initialized
    struct coro *solo = coro_ext_create(1, proc, stack_size, maxudsize);
    if (solo) {
        main->curr_coro = solo;
        return coroutine_userdata(solo);
    }
    return NULL;
}

void solo_reload(solo_struct *main, coroproc proc)
{
    coro_reload(main->curr_coro, proc);
}

void solo_ext_reload(solo_struct *main, coroproc proc, void *userdata)
{
    coro_ext_reload(main->curr_coro, proc, userdata);
}

bool solo_start(solo_struct *main)
{
    struct coro_main mainstack;
    main->main_coro = &mainstack;
    mainstack.yield_cycle = 0;
    struct coro *next = main->curr_coro;
    if (next == NULL || next->rspoffset == 0) {
        return false;
    }
    return coroutinemainyield((coro_struct *)main, next);
}

void solo_finish(solo_struct *main)
{
    struct coro *solo = main->curr_coro;
    if (solo == NULL) return;
    coroutinefree(solo);
    main->curr_coro = NULL;
}
