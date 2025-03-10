#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "coro.h"
#include "solo.h"

#define magic_coro_debug magic_code_debug
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

struct corostruct;

struct mainflags {
    unsigned yield_cycle: 1, inplace: 1;
};

struct coroflags {
    unsigned extra_flags;
};

struct coro {
    void *rsp;
    void *yield_para;
    struct corostruct *basestruct;
    union { struct mainflags main_flags; struct coroflags coro_flags; } u;
    union { int main_start_id; int coro_index; } uu;
};

struct coro_guard {
    struct coro_guard *userdata_guard;
    void *rsp_high_address;
    uint32_t lower_guard_word;
    uint32_t upper_guard_word;
};

// [coro_struct]
// [coro *][coro *]...
struct corostruct {
    struct coro main;
    int maxcoros;
    int coro_cnt;
};

typedef struct {
    struct corostruct base;
    struct coro *solo;
} solo_struct_impl;

magic_coro_api(void) asm_coro_call(struct coro *coro);
magic_coro_api(bool) asm_coro_yield(struct coro *coro, struct coro *next);
magic_coro_api(void *) asm_coro_init(void *rsp);

magic_coro_api(void) asm_call_stack_crash(void *co_rsp, void *rsp)
{
    printf("stack corruption: rsp %p %p\n", co_rsp, rsp);
    exit(1);
}

void coroutine_stack_segmentation_fault(struct coro *coro)
{
    printf("stack segmentation fault: coro %p\n", (void *)coro);
    exit(2);
}

static struct coro **get_struct_coro(struct corostruct *s)
{
    return (struct coro **)(s + 1);
}

struct coro *coroutine_get_coro(coro_struct main, int index)
{
    struct corostruct *s = (struct corostruct *)main.impl;
    assert(index > 0 && index <= s->coro_cnt);
    return get_struct_coro(s)[index - 1];
}

int coroutine_main_id(coro_struct main)
{
    struct corostruct *s = (struct corostruct *)main.impl;
    return s->main.uu.main_start_id;
}

void *coroutine_main_para(coro_struct main)
{
    struct corostruct *s = (struct corostruct *)main.impl;
    return s->main.yield_para;
}

void *coroutine_yield_para(struct coro *coro)
{
    return coro->yield_para;
}

int coroutine_id(struct coro *coro)
{
    return coro->basestruct->main.uu.main_start_id + coro->uu.coro_index;
}

struct coro_index coroutine_index(struct coro *coro)
{
    return (struct coro_index){coro->uu.coro_index};
}

#define coro_alloc_size_(maxcoros) (int)(sizeof(struct corostruct) + maxcoros * sizeof(void *))

int coroutine_alloc_size(int maxcoros)
{
    assert(maxcoros >= 0);
    return coro_alloc_size_(maxcoros);
}

coro_struct coroutine_init_inplace(void *addr, int start_id, int maxcoros)
{
    assert(maxcoros >= 0);
    struct corostruct *s = (struct corostruct *)addr;
    int size = coro_alloc_size_(maxcoros);
    memset(s, 0, size);
    s->maxcoros = maxcoros;
    s->main.u.main_flags.inplace = 1;
    s->main.uu.main_start_id = start_id;
    s->main.basestruct = s;
#if magic_coro_debug
    printf("[%02d] %p created\n", s->main.uu.main_start_id, (void *)&s->main);
#endif
    return (coro_struct){&s->main};
}

coro_struct coroutine_init(int start_id, int maxcoros)
{
    struct corostruct *s = (struct corostruct *)malloc(coroutine_alloc_size(maxcoros));
    coro_struct main = coroutine_init_inplace(s, start_id, maxcoros);
    s->main.u.main_flags.inplace = 0;
    return main;
}

bool coro_guard_verify(struct coro_guard *guard)
{
    return guard->lower_guard_word == magic_lower_guard_word &&
        guard->upper_guard_word == magic_upper_guard_word;
}

bool coroutine_stack_verify(struct coro *coro)
{
    if (coro == &coro->basestruct->main) {
        return true; // dont check main coroutine
    }
    struct coro_guard *guard = (struct coro_guard *)(coro + 1);
    if (!coro_guard_verify(guard)) {
        return false;
    }
    if (guard->userdata_guard && !coro_guard_verify(guard->userdata_guard)) {
        return false;
    }
    return true;
}

void *coroutinecreate_(coro_struct main, coroproc proc, int stack_size, void *userdata, int userdata_bytes, struct coro *inplace)
{
    void **rsp;
    struct coro *coro;
    struct coro_guard *guard;
    if (userdata_bytes < 0) userdata_bytes = 0;
    int coro_size = sizeof(struct coro ) + magic_align(userdata_bytes) + 2 * sizeof(struct coro_guard);

    if (inplace) {
        coro = inplace;
        guard = (struct coro_guard *)(coro + 1);
        if (!coro_guard_verify(guard)) {
            coroutine_stack_segmentation_fault(coro);
        }
        stack_size = (int)((char *)guard->rsp_high_address - (char *)coro);
        assert(stack_size > coro_size);
        rsp = (void **)guard->rsp_high_address;
        guard->userdata_guard = NULL;
    } else {
        struct corostruct *s = (struct corostruct *)main.impl;
        if (s->coro_cnt >= s->maxcoros) {
            return NULL;
        }
        assert(stack_size > coro_size);
        size_t alloc = align_16(stack_size);
        coro = (struct coro *)aligned_memalloc(alloc); // 对齐到 16 字节边界
        rsp = (void **)((char *)coro + alloc);
        memset(coro, 0, coro_size);
        get_struct_coro(s)[s->coro_cnt] = coro;
        s->coro_cnt += 1;
        coro->uu.coro_index = s->coro_cnt;
        coro->basestruct = s;
        guard = (struct coro_guard *)(coro + 1);
        guard->lower_guard_word = magic_lower_guard_word;
        guard->upper_guard_word = magic_upper_guard_word;
        guard->rsp_high_address = (void *)rsp;
    }

    if (userdata_bytes > 0) {
        userdata = (char *)coro + sizeof(struct coro) + sizeof(struct coro_guard);
        guard->userdata_guard = (struct coro_guard *)((char *)userdata + magic_align(userdata_bytes));
        guard = guard->userdata_guard;
        guard->lower_guard_word = magic_lower_guard_word;
        guard->upper_guard_word = magic_upper_guard_word;
        guard->rsp_high_address = (void *)rsp;
    }

    void *rspaligned_to_16 = (void *)rsp;
    // pstack + alloc <-- 00                 <-- 16字节对齐
    //             -4 <-- 12 对齐填补
    //             -8 <-- 08 对齐填补 userdata
    //            -12 <-- 04 proc/coro
    //            -16 <-- 00 asm_coro_call   <-- 16字节对齐
    //            -20 <-- 12 ecx
    *(--rsp) = 0;
    *(--rsp) = userdata;
    // pstack + alloc <-- 00                 <-- 16字节对齐
    //                <-- 08 对齐填补
    //       userdata <-- 00 对齐填补         <-- 16字节对齐
    //      proc/coro <-- 08 proc/coro
    //  asm_coro_call <-- 00 asm_coro_call   <-- 16字节对齐
    //           coro <-- 08 rcx/rdi
    *(--rsp) = (void *)(uintptr_t)proc;             // 初始为proc，后改为coro提供给 coro_return 使用
    *(--rsp) = (void *)(uintptr_t)asm_coro_call;    // 创建时为 coro_asm_call，之后会改为协程函数中的返回地址
    void *align_to_16_bytes = (void *)rsp;          // 对齐到 16 字节边界
    *(--rsp) = coro;                                // rcx/rdi
    coro->rsp = asm_coro_init((void *)rsp);

    assert((uintptr_t)rspaligned_to_16 % 16 == 0);
    assert((uintptr_t)align_to_16_bytes % 16 == 0);

#if magic_coro_debug
    printf("[%02d] %p created %08x left %08x rsp %p depth %08x high %p proc %p data %p\n",
        coroutine_id(coro), (void *)coro, coro_size, (int)((char *)coro->rsp - (char *)coro), coro->rsp,
        (int)((char *)rspaligned_to_16 - (char *)coro->rsp), rspaligned_to_16, (void *)(uintptr_t)proc, userdata);
#endif
    return userdata;
}

void coroutine_create(coro_struct main, coroproc proc, int stack_size, void *userdata)
{
    coroutinecreate_(main, proc, stack_size, userdata, 0, NULL);
}

void *coroutine_create_with_struct(coro_struct main, coroproc proc, int stack_size, int userdata_bytes)
{
    return coroutinecreate_(main, proc, stack_size, NULL, userdata_bytes, NULL);
}

struct coro *coro_next_resume_(struct coro *coro)
{
    struct corostruct *s = coro->basestruct;
    if (coro != &s->main && s->main.u.main_flags.yield_cycle == 0) {
        return &s->main;
    }
    int i = (coro == &s->main) ? 0 : coro->uu.coro_index;
    for (; i < s->coro_cnt; i += 1) {
        struct coro *next = get_struct_coro(s)[i];
        if (next && next->rsp) {
            return next;
        }
    }
    return &s->main;
}

static void coro_set_para_(struct coro *coro, void *yield_para)
{
    coro->yield_para = yield_para;
}

bool coroutine_yield_cycle(coro_struct main)
{
    struct corostruct *s = (struct corostruct *)main.impl;
    s->main.u.main_flags.yield_cycle = 1;
    struct coro *next = coro_next_resume_(&s->main);
    if (next == &s->main) {
        return false;
    }
    return asm_coro_yield(&s->main, next);
}

bool coroutine_yield_manual(coro_struct main, int index, void *yield_para)
{
    struct corostruct *s = (struct corostruct *)main.impl;
    s->main.u.main_flags.yield_cycle = 0;
    struct coro *next = coroutine_get_coro(main, index);
    coro_set_para_(next, yield_para);
    return asm_coro_yield(&s->main, next);
}

void coroutine_finish(coro_struct *main)
{
    struct corostruct *s = (struct corostruct *)main->impl;
    if (s == NULL) {
        return;
    }

    struct coro **start = get_struct_coro(s);
    for (int i = 0; i < s->coro_cnt; i += 1) {
        struct coro *coro = start[i];
        if (coro) {
            aligned_memfree(coro);
            start[i] = NULL;
        }
    }

    main->impl = NULL;
    if (!s->main.u.main_flags.inplace) {
        free(s);
    }
}

void coroutine_yield(struct coro *coro)
{
    asm_coro_yield(coro, coro_next_resume_(coro));
}

struct coro *coro_set_yield_para_(struct coro *coro, void *yield_para)
{
    struct corostruct *s = coro->basestruct;
    struct coro *next = coro_next_resume_(coro);
    if (!s->main.u.main_flags.yield_cycle) {
        coro_set_para_(next, yield_para);
    }
    return next;
}

void coroutine_yield_with_para(struct coro *coro, void *yield_para)
{
    asm_coro_yield(coro, coro_set_yield_para_(coro, yield_para));
}

void coroutine_set_yield_para(struct coro *coro, void *yield_para)
{
    coro_set_yield_para_(coro, yield_para);
}

magic_coro_api(struct coro*) asm_call_coro_finish(struct coro *coro)
{
    if (!coroutine_stack_verify(coro)) {
        coroutine_stack_segmentation_fault(coro);
    }
    struct coro *next = coro_next_resume_(coro);
    coro->rsp = 0;
    return next;
}

void solo_create(solo_struct *main, coroproc proc, int stack_size, void *userdata)
{
    solo_struct_impl *s = (solo_struct_impl *)main;
    if (s->solo) return;
    coroutine_init_inplace(s, 5010, 1);
    coroutine_create((coro_struct){main}, proc, stack_size, userdata);
}

void solo_recreate(solo_struct *main, coroproc proc, void *userdata)
{
    solo_struct_impl *s = (solo_struct_impl *)main;
    struct coro *solo = s->solo;
    if (solo == NULL || solo->rsp) {
        return;
    }
    coroutinecreate_((coro_struct){NULL}, proc, 0, userdata, 0, solo);
}

bool solo_call(solo_struct *main)
{
    solo_struct_impl *s = (solo_struct_impl *)main;
    struct coro *next = s->solo;
    if (next == NULL || next->rsp == NULL) {
        return false;
    }
    s->base.main.u.main_flags.yield_cycle = 0;
    return asm_coro_yield(&s->base.main, next);
}

void solo_finish(solo_struct *main)
{
    solo_struct_impl *s = (solo_struct_impl *)main;
    if (s->solo == NULL) {
        return;
    }
    aligned_memfree(s->solo);
    s->solo = NULL;
}
