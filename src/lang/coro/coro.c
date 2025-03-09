#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "coro.h"

#define magic_coro_debug magic_code_debug

#if defined(magic_cl_msc)
#define aligned_memalloc(size) _aligned_malloc(size, 16)
#define aligned_memfree(p) _aligned_free(p)
#else
void *aligned_alloc(size_t alignment, size_t size);
#define aligned_memalloc(size) aligned_alloc(16, size)
#define aligned_memfree(p) free(p)
#endif

struct corostruct;

struct coro {
    void *rsp;
    struct corostruct *basestruct;
    void *userdata;
    void *yield_para;
    int index;
};

// [corostruct]
// [coro *][coro *]...
// [userdata][userdata]...
struct corostruct {
    int maxcoros;
    int coro_cnt;
    int start_id;
    unsigned yield_cycle: 1, inplace: 1;
    struct coro main;
};

magic_coro_api(void) asm_coro_call(struct coro *coro);
magic_coro_api(bool) asm_coro_yield(struct coro *coro, struct coro *next);
magic_coro_api(void *) asm_coro_init(void *rsp);

static struct coro **corostruct_get_coro_(struct corostruct *s)
{
    return (struct coro **)(s + 1);
}

void *coroutine_precreate_userdata(struct coro *main, int index)
{
    assert(index > 0 && index <= main->basestruct->maxcoros);
    void **userdata = (void **)corostruct_get_coro_(main->basestruct);
    return userdata[index - 1];
}

struct coro *coroutine_get_coro(struct coro *main, int index)
{
    assert(index > 0 && index <= main->basestruct->coro_cnt);
    return corostruct_get_coro_(main->basestruct)[index - 1];
}

void *coroutine_userdata(struct coro *coro)
{
    return coro->userdata;
}

void *coroutine_yield_para(struct coro *coro)
{
    return coro->yield_para;
}

int coroutine_id(struct coro *coro)
{
    return coro->basestruct->start_id + coro->index;
}

struct coroindex coroutine_index(struct coro *coro)
{
    return (struct coroindex){coro->index};
}

int coroutine_alloc_size(int maxcoros, int *userdata_bytes)
{
    assert(maxcoros >= 0);
    int size = sizeof(struct corostruct) + maxcoros * sizeof (void *);
    struct userdatadesc d = {userdata_bytes};
    for (int i = 0; i < maxcoros; i += 1) {
        if (!get_userdata_bytes(&d)) {
            break;
        }
    }
    return size + d.sum;
}

struct coro *coroutine_init_inplace(void *addr, int start_id, int maxcoros, int *userdata_bytes)
{
    assert(maxcoros >= 0);
    struct corostruct *s = (struct corostruct *)addr;
    int header_size = sizeof(struct corostruct) + maxcoros * sizeof (void *);
    memset(s, 0, header_size);
    s->maxcoros = maxcoros;
    s->inplace = 1;
    s->start_id = start_id;
    s->main.basestruct = s;

    void **userdata = (void **)corostruct_get_coro_(s);
    struct userdatadesc d = {userdata_bytes};
    for (int i = 0; i < maxcoros; i += 1) {
        if (!get_userdata_bytes(&d)) {
            break;
        }
        if (d.bytes) {
            userdata[i] = (char *)s + header_size + d.offset;
        }
    }

    if (d.sum) { // zero all userdata
        memset((char *)s + header_size, 0, d.sum);
    }

#if magic_coro_debug
    printf("[%02d] %p created\n", coroutine_id(&s->main), (void *)&s->main);
#endif

    return &s->main;
}

struct coro *coroutine_init(int start_id, int maxcoros, int *userdata_bytes)
{
    struct corostruct *s = (struct corostruct *)malloc(coroutine_alloc_size(maxcoros, userdata_bytes));
    struct coro *main = coroutine_init_inplace(s, start_id, maxcoros, userdata_bytes);
    s->inplace = 0;
    return main;
}

void coroutine_create(struct coro *main, coroproc proc, int stack_size, void *para)
{
    struct corostruct *s = main->basestruct;
    if (s->coro_cnt >= s->maxcoros) {
        return;
    }

    assert(stack_size > sizeof(struct coro));
    int alloc = align_16(stack_size);
    struct coro *coro = (struct coro *)aligned_memalloc(alloc); // 对齐到 16 字节边界
    void **rsp = (void **)((char *)coro + alloc);
    void *rspaligned_to_16 = (void *)rsp;
    memset(coro, 0, sizeof(struct coro));
    // pstack + alloc <-- 00                 <-- 16字节对齐
    //             -4 <-- 12 对齐填补
    //             -8 <-- 08 对齐填补
    //            -12 <-- 04 coro
    //            -16 <-- 00 asm_coro_call   <-- 16字节对齐
    //            -20 <-- 12 ecx
#if magic_arch_bits == 32
    *(--rsp) = 0;
    *(--rsp) = 0;
#elif magic_arch_bits != 64
    #error error: unsupported architecture.
#endif
    // pstack + alloc <-- 00                 <-- 16字节对齐
    //             -8 <-- 08 coro
    //            -16 <-- 00 asm_coro_call   <-- 16字节对齐
    //            -24 <-- 08 rcx/rdi
    *(--rsp) = (void *)(uintptr_t)proc;             // 初始为proc，后改为coro提供给 coro_return 使用
    *(--rsp) = (void *)(uintptr_t)asm_coro_call;    // 创建时为 coro_asm_call，之后会改为协程函数中的返回地址
    void *align_to_16_bytes = (void *)rsp;          // 对齐到 16 字节边界
    *(--rsp) = coro;                                // rcx/rdi
    coro->rsp = asm_coro_init((void *)rsp);

    void **ud = (void **)corostruct_get_coro_(s);
    if (ud[s->coro_cnt]) {
        coro->userdata = ud[s->coro_cnt];
    } else {
        coro->userdata = para;
    }
    ud[s->coro_cnt] = coro;
    coro->basestruct = s;
    coro->index = s->coro_cnt + 1;
    s->coro_cnt += 1;

#if magic_coro_debug
    printf("[%02d] %p created left %08x rsp %p depth %08x high %p proc %p data %p\n", coroutine_id(coro), (void *)coro, (int)((char *)coro->rsp - (char *)coro),
        coro->rsp, (int)((char *)rspaligned_to_16 - (char *)coro->rsp), rspaligned_to_16, (void *)(uintptr_t)proc, coro->userdata);
#endif

    assert((uintptr_t)rspaligned_to_16 % 16 == 0);
    assert((uintptr_t)align_to_16_bytes % 16 == 0);
}

struct coro *coroutine_next_resume_(struct coro *coro)
{
    struct corostruct *s = coro->basestruct;
    int i = 0;
    if (coro != &s->main) {
        if (!s->yield_cycle) {
            return &s->main;
        }
        i = coro->index;
    }
    for (; i < s->coro_cnt; i += 1) {
        struct coro *next = corostruct_get_coro_(s)[i];
        if (next && next->rsp) {
            return next;
        }
    }
    return &s->main;
}

static void coroutine_set_para_(struct coro *coro, void *yield_para)
{
    coro->yield_para = yield_para;
}

bool coroutine_yield_cycle(struct coro *main)
{
    struct corostruct *s = main->basestruct;
    assert(main == &s->main);

    s->yield_cycle = 1;

    struct coro *next = coroutine_next_resume_(main);
    if (next == main) {
        return false;
    }

    return asm_coro_yield(main, next);
}

bool coroutine_yield_manual(struct coro *main, int index, void *yield_para)
{
    struct corostruct *s = main->basestruct;
    assert(main == &s->main);

    s->yield_cycle = 0;

    struct coro *coro = coroutine_get_coro(main, index);
    coroutine_set_para_(coro, yield_para);
    return asm_coro_yield(main, coro);
}

void coroutine_finish(struct coro *main)
{
    struct corostruct *s = main->basestruct;
    if (s == NULL) {
        return;
    }

    assert(main == &s->main);

    for (int i = 0; i < s->coro_cnt; i += 1) {
        struct coro *coro = corostruct_get_coro_(s)[i];
        if (coro) {
            coro->basestruct = NULL;
            aligned_memfree(coro);
        }
    }

    main->basestruct = NULL;

    if (!s->inplace) {
        free(s);
    }
}

void coroutine_yield(struct coro *coro)
{
    struct corostruct *s = coro->basestruct;
    assert(coro != &s->main);
    asm_coro_yield(coro, coroutine_next_resume_(coro));
}

struct coro *coroutine_set_yield_para_(struct coro *coro, void *yield_para)
{
    struct corostruct *s = coro->basestruct;
    assert(coro != &s->main);
    struct coro *next = coroutine_next_resume_(coro);
    if (!s->yield_cycle) {
        coroutine_set_para_(next, yield_para);
    }
    return next;
}

void coroutine_yield_with_para(struct coro *coro, void *yield_para)
{
    asm_coro_yield(coro, coroutine_set_yield_para_(coro, yield_para));
}

void coroutine_set_yield_para(struct coro *coro, void *yield_para)
{
    coroutine_set_yield_para_(coro, yield_para);
}

magic_coro_api(struct coro*) asm_call_coro_finish(struct coro *coro)
{
    struct coro *next = coroutine_next_resume_(coro);
#if 0 // printf consume large coroutine stack size
    printf("[%02d] %p finish: left %08x rsp %p next %p\n", coroutine_id(coro), (void *)coro,
        (int)((char *)coro->rsp - (char *)coro), coro->rsp, (void *)next);
#endif
    coro->rsp = 0;
    return next;
}

magic_coro_api(void) asm_call_stack_crash(void *co_rsp, void *rsp)
{
    printf("Fatal: coro stack crash %p %p\n", co_rsp, rsp);
    exit(1);
}

#if 0
typedef struct {
    struct coro *coro;
    struct coro *permalink;
    bool ready;
    bool across;
} struct coroSlot;

typedef struct {
    struct coro init;
    struct coroSlot *slot;
    struct coroSlot *recv;
    int capacity;
    int count;
} struct coroImpl;

#define is_yield_cycle(main) (main->init.flags & 0x01)
#define set_yield_cycle(main) main->init.flags |= 0x01
#define clr_yield_cycle(main) main->init.flags &= (~0x01)
#define is_received_task(coro) (coro->flags & 0x02)
#define set_received_task(coro) coro->flags |= 0x02
#define clr_received_task(coro) coro->flags &= (~0x02)

bool coroutine_recv_task(struct coro * cont, struct coro *coro)
{
    struct coroImpl *main = (struct coroImpl *)cont.a;
    struct coroSlot *recv = main->recv;
    if (recv->coro) {
        return false;
    }
    recv->coro = coro;
    recv->permalink = (struct coro *)coro->main;
    recv->ready = true;
    coro->main = main;
    set_received_task(coro);
    return true;
}

struct coro *coroutine_next_resume_(struct coro *coro)
{
    struct corostruct *s = coro->basestruct;
    int i = 0;
    if (coro != &s->main) {
        if (!is_yield_cycle(main) || is_received_task(coro)) {
            return &main->init;
        }
        i = coro->id - main->init.id;
    }
    for (; i < main->count; i += 1) {
        struct coro *next = slot[i].coro;
        if (slot[i].ready && next && next->rsp) {
            return next;
        }
    }
    struct coro *recv = coroutinereceivedtask_(main);
    return recv ? recv : &main->init;
}

void coroutine_set_ready(struct coro *coro, bool ready)
{
    struct coroImpl *main = (struct coroImpl *)coro->main;
    struct coroSlot *slot = main->slot;
    if (coro == &main->init) {
        return;
    }
    if (is_received_task(coro)) {
        slot = main->recv;
    } else {
        slot += coro->id - main->init.id - 1;
    }
    slot->ready = ready;
}

struct coro *coroutinereceivedtask_(struct coroImpl *main)
{
    struct coroSlot *recv = main->recv;
    if (recv->coro && recv->ready) {
        return recv->coro;
    }
    return null;
}

void coroutine_across_world(struct coro *coro)
{
    struct coroImpl *main = (struct coroImpl *)coro->main;
    struct coroSlot *slot = main->slot;
    struct coro *next = coroutine_next_resume_(coro);
    if (coro == &main->init) {
        return;
    }
    if (is_received_task(coro)) {
        clr_received_task(coro);
        slot = main->recv;
        coro->main = slot->permalink;
        slot->permalink = null;
    } else {
        slot += coro->id - main->init.id - 1;
    }
    slot->ready = false;
    slot->across = true;
    asm_coro_yield(coro, next);
}
#endif
