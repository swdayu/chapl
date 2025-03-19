#define PRH_STRIP_PREFIX
#include "coroutine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define magic_coro_debug magic_debug
#define magic_udata_guard_word 0xa5a5a5a5
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

magic_fastcall(intp) asm_coro_stack_init_depth(void);
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

struct corostruct;

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
//  [           : basestruct    ]
//  upper memory address

struct coro {
    unsigned rspoffset; // 1st field dont move
    union { unsigned loweraddr; unsigned coro_run; } u;
    struct corostruct *basestruct;
};

struct coroudata {
    unsigned maxudsize: 31, just_ptr_value: 1;
    unsigned udata_guard_word;
};

struct coroguard {
    unsigned lower_guard_word;
    unsigned upper_guard_word;
};

magic_inline struct coroudata *coroutinegetuserdata(struct coro *coro)
{
    return (struct coroudata *)((char *)coro - coro->u.loweraddr);
}

magic_inline struct coroguard *coroutinegetguard(struct coroudata *udata)
{
    return (struct coroguard *)((char *)udata + sizeof(struct coroudata) + udata->maxudsize);
}

void *coroutine_userdata(struct coro *coro)
{
    struct coroudata *p = coroutinegetuserdata(coro);
    void *userdata = p + 1;
    if (p->just_ptr_value)
    {
        return *(void **)userdata;
    }
    return ((int)p->maxudsize > 0) ? userdata : NULL;
}

// corostruct layout:
//  [struct corostruct]
//  [struct coro *]
//  [struct coro *]
//  ...
struct corostruct {
    struct coro *main;
    unsigned maxcoros: 31, inplace: 1;
    unsigned coro_cnt: 31, yield_cycle: 1;
};

struct solostruct {
    struct corostruct base;
    struct coro *solo;
};

static_must(sizeof(struct coro) <= 16);
static_must(sizeof(solo_struct) >= sizeof(struct solostruct));

magic_inline int corostructallocsize(int maxcoros)
{
    return (int)(sizeof(struct corostruct) + maxcoros * sizeof(void *));
}

magic_inline struct coro **coroutineptrlist(struct corostruct *s)
{
    return (struct coro **)(s + 1);
}

struct coro *coroutine_get(coro_struct main, int index)
{
    struct corostruct *s = (struct corostruct *)main.address;
    assert(index > 0 && index <= (int)s->coro_cnt);
    return coroutineptrlist(s)[index - 1];
}

int coroutine_alloc_size(int maxcoros)
{
    assert(maxcoros >= 0);
    return corostructallocsize(maxcoros);
}

coro_struct coroutine_init_inplace(void *addr, int maxcoros)
{
    assert(maxcoros >= 0);
    struct corostruct *s = (struct corostruct *)addr;
    s->maxcoros = maxcoros;
    s->inplace = 1;
    return (coro_struct){s};
}

coro_struct coroutine_init(int maxcoros)
{
    int alloc_size = coroutine_alloc_size(maxcoros);
    struct corostruct *s = (struct corostruct *)malloc(alloc_size);
    memset(s, 0, alloc_size);
    coro_struct main = coroutine_init_inplace(s, maxcoros);
    s->inplace = 0;
    return main;
}

void coroutinestackguard(struct coro *coro)
{
    struct coroudata *p = coroutinegetuserdata(coro);
    if (p->udata_guard_word == magic_udata_guard_word)
    {
        struct coroguard *g = coroutinegetguard(p);
        if (g->lower_guard_word == magic_lower_guard_word && g->upper_guard_word == magic_upper_guard_word)
        {
            return;
        }
    }
    coroutine_stack_segmentation_fault(coro);
}

magic_inline int coroutineselfsize(int maxudsize)
{
    return 16 + sizeof(struct coroudata) + magic_align(maxudsize) + sizeof(struct coroguard); // 16 for struct coro
}

struct coro *coroutinealloc(struct corostruct *s, int stack_size, int userdata_bytes)
{
    if (s->coro_cnt >= s->maxcoros)
    {
        return NULL;
    }

    if (userdata_bytes < 0)
    {
        userdata_bytes = 0;
    }

    assert(stack_size > coroutineselfsize(userdata_bytes) + asm_coro_stack_init_depth());

    int stackallocsize = magic_align_16_byte(stack_size); // 对齐到 16 字节边界
    char *p = (char *)aligned_memalloc(stackallocsize);
    struct coro *coro = (struct coro *)(p + stackallocsize - 16);
    coro->rspoffset = 0;
    coro->u.loweraddr = (unsigned)((char *)coro - p);
    coro->basestruct = s;

    struct coroudata *udata = coroutinegetuserdata(coro);
    udata->maxudsize = magic_align(userdata_bytes);
    udata->just_ptr_value = 0;
    udata->udata_guard_word = magic_udata_guard_word;

    struct coroguard *guard = coroutinegetguard(udata);
    guard->lower_guard_word = magic_lower_guard_word;
    guard->upper_guard_word = magic_upper_guard_word;

    coroutineptrlist(s)[s->coro_cnt] = coro;
    s->coro_cnt += 1;
    return coro;
}

static void coroutineresetuserdata(struct coro *coro, void *userdata, int flags)
{
    struct coroudata *udata = coroutinegetuserdata(coro);
    void **data_start = (void **)(udata + 1);
    if ((int)udata->maxudsize <= 0) return;
    if (flags & 0x10) // init just_ptr_value
    {
        udata->just_ptr_value = 1;
        *data_start = userdata;
    }
    else
    {
        if (!udata->just_ptr_value)
        {
            memset(data_start, 0, udata->maxudsize);
        }
        if (flags & 0x01) // reset_ptr_value or reset first ptr field
        {
            *data_start = userdata;
        }
    }
}

void *coroutinelaunch(struct coro *coro, coroproc proc, void *userdata, int flags)
{
    if (coro == NULL)
    {
        return NULL;
    }

    coroutinestackguard(coro);

    coroutineresetuserdata(coro, userdata, flags);

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
    struct coroudata *udata = coroutinegetuserdata(coro);
    rsp = (void **)((char *)rspaligned_to_16 - asm_coro_stack_init_depth());
    printf("[coro] %p created %08x left %08x rsp %p depth %08x high %p proc %p\n",
        (void *)coro, (int)(sizeof(struct coroudata) + udata->maxudsize + sizeof(struct coroguard)), (int)((char *)rsp - (char *)udata), (void *)rsp,
        (int)((char *)rspaligned_to_16 + 16 - (char *)rsp), (char *)rspaligned_to_16 + 16, (void *)(intp)proc);
#endif

    return coroutine_userdata(coro);
}

void coroutine_create(coro_struct main, coroproc proc, int stack_size, void *userdata)
{
    struct coro *coro = coroutinealloc((struct corostruct *)main.address, stack_size, sizeof(void *));
    coroutinelaunch(coro, proc, userdata, 0x10);
}

void *coroutine_ext_create(coro_struct main, coroproc proc, int stack_size, int userdata_bytes)
{
    struct coro *coro = coroutinealloc((struct corostruct *)main.address, stack_size, userdata_bytes);
    return coroutinelaunch(coro, proc, NULL, 0x00);
}

void coroutine_reload(coro_struct main, int index, coroproc proc)
{
    struct coro *coro = coroutine_get(main, index);
    if (coro && coro->rspoffset == 0)
    {
        coroutinelaunch(coro, proc, NULL, 0x00);
    }
}

void coroutine_ext_reload(coro_struct main, int index, coroproc proc, void *userdata)
{
    struct coro *coro = coroutine_get(main, index);
    if (coro && coro->rspoffset == 0)
    {
        coroutinelaunch(coro, proc, userdata, 0x01);
    }
}

struct coro *prh_impl_coro_next_resume(struct coro *coro)
{
    struct corostruct *s = coro->basestruct;
    struct coro *main = s->main;
    if (coro != main && s->yield_cycle == 0)
    {
        goto labelnextismain;
    }
    int i = (coro == main) ? 0 : main->u.coro_run;
    for (; i < (int)s->coro_cnt; i += 1)
    {
        struct coro *next = coroutineptrlist(s)[i];
        if (next && next->rspoffset)
        {
            main->u.coro_run = i + 1;
            return next;
        }
    }
labelnextismain:
    main->u.coro_run = 0;
    return main;
}

magic_inline bool coroutinemainyield(struct corostruct *s, struct coro *next)
{
    bool result = prh_impl_asm_coro_yield(s->main, next);
    coroutinestackguard(next);
    return result;
}

magic_fastcall(void *) asm_coro_finish(struct coro *coro)
{
    coro->rspoffset = 0;
    coroutinestackguard(coro);
    return prh_impl_coro_next_resume(coro);
}

bool coroutine_start_cycle(coro_struct main)
{
    struct corostruct *s = (struct corostruct *)main.address;
    struct coro main_coro = {.basestruct = s};
    s->main = &main_coro;
    s->yield_cycle = 1;
    struct coro *next = prh_impl_coro_next_resume(s->main);
    if (next == s->main)
    {
        return false;
    }
    return coroutinemainyield(s, next);
}

bool coroutine_start(coro_struct main, int index)
{
    struct corostruct *s = (struct corostruct *)main.address;
    struct coro main_coro = {.basestruct = s};
    s->main = &main_coro;
    s->yield_cycle = 0;
    struct coro *next = coroutine_get(main, index);
    if (next == NULL || next->rspoffset == 0)
    {
        return false;
    }
    main_coro.u.coro_run = index;
    return coroutinemainyield(s, next);
}

int coroutine_self(struct coro *coro)
{
    struct coro *main = coro->basestruct->main;
    return main->u.coro_run;
}

magic_inline void coroutinefree(struct coro *coro)
{
    aligned_memfree(coroutinegetuserdata(coro));
}

void coroutine_finish(coro_struct *main)
{
    struct corostruct *s = (struct corostruct *)main->address;
    if (s == NULL)
    {
        return;
    }
    struct coro **start = coroutineptrlist(s);
    for (int i = 0; i < (int)s->coro_cnt; i += 1)
    {
        struct coro *coro = start[i];
        if (coro)
        {
            coroutinefree(coro);
            start[i] = NULL;
        }
    }
    main->address = NULL;
    if (!s->inplace)
    {
        free(s);
    }
}

bool solostructinit(struct solostruct *s)
{
    if (s->solo) return false; // already initialized
    coroutine_init_inplace(s, 1);
    return true;
}

void solo_create(solo_struct *main, coroproc proc, int stack_size, void *userdata)
{
    struct solostruct *s = (struct solostruct *)main;
    if (!solostructinit(s))
    {
        return;
    }
    coroutine_create((coro_struct){s}, proc, stack_size, userdata);
}

void *solo_ext_create(solo_struct *main, coroproc proc, int stack_size, int maxudsize)
{
    struct solostruct *s = (struct solostruct *)main;
    if (!solostructinit(s))
    {
        return NULL;
    }
    return coroutine_ext_create((coro_struct){s}, proc, stack_size, maxudsize);
}

void solo_reload(solo_struct *main, coroproc proc)
{
    coroutine_reload((coro_struct){main}, 1, proc);
}

void solo_ext_reload(solo_struct *main, coroproc proc, void *userdata)
{
    coroutine_ext_reload((coro_struct){main}, 1, proc, userdata);
}

bool solo_start(solo_struct *main)
{
    struct solostruct *s = (struct solostruct *)main;
    struct coro *next = s->solo;
    if (next == NULL || next->rspoffset == 0)
    {
        return false;
    }
    struct coro main_coro = {.basestruct = &s->base};
    main_coro.u.coro_run = 1;
    s->base.main = &main_coro;
    return coroutinemainyield(&s->base, next);
}

void solo_finish(solo_struct *main)
{
    struct solostruct *s = (struct solostruct *)main;
    if (s->solo == NULL)
    {
        return;
    }
    coroutinefree(s->solo);
    s->solo = NULL;
}
