#define __CURR_FILE__ STRID_LANG_CORO
#include "internal/decl.h"
#include "coro.h"

#if defined(__magic_cl_gnu)
void *aligned_alloc(size_t alignment, size_t size);
#define aligned_memalloc(size) aligned_alloc(16, size)
#define aligned_memfree(p) free(p)
#else
#define aligned_memalloc(size) _aligned_malloc(size, 16)
#define aligned_memfree(p) _aligned_free(p)
#endif

__magic_coro_api(void) asm_coro_call(Coro* co);
__magic_coro_api(bool) asm_coro_yield(Coro *co, Coro *wait);
__magic_coro_api(bool) asm_coro_yield_manual(Coro *co, Coro *wait);
__magic_coro_api(uptr) asm_coro_init(uptr rsp);

__magic_coro_api(Coro*) asm_call_coro_finish(Coro *co)
{
    Coro *wait = co->wait;
#if __magic_coro_debug
    printf("[%02d] %p finish: rsp %p wait %p left %d\n", (int)co->id, (void *)co, (void *)co->rsp, (void *)wait, (int)(((byte *)co->rsp) - (byte *)co - sizeof(Coro)));
#endif
    co->rsp = 0;
    return wait;
}

__magic_coro_api(void) asm_call_stack_crash(uptr co_rsp, uptr rsp)
{
    printf("coro stack crash %p %p\n", (void *)co_rsp, (void *)rsp);
    exit(1);
}

typedef struct {
    Coro init;
    Coro **item;
    uptr capacity;
    uptr count;
} CoroImpl;

CoroCont coroutine_init(uptr cont_id, uptr count)
{
    count = count > 0 ? count : 1;
    uptr alloc = sizeof(CoroImpl) + count * sizeof(Coro*);
    CoroImpl *ctx = (CoroImpl *)aligned_memalloc(alloc);
    memset(ctx, 0, alloc);
    ctx->init.id = cont_id;
    ctx->capacity = count;
    ctx->item = (Coro **)(ctx + 1);
    return (CoroCont)ctx;
}

Coro *coroutine_at(CoroCont cont, uptr index)
{
    CoroImpl *ctx = (CoroImpl *)cont;
    if (index >= ctx->count) {
        return null;
    }
    return ctx->item[index];
}

Coro *coroutine_create(CoroCont cont, CoroFunc f, uptr stack_size, uptr para)
{
    CoroImpl *ctx = (CoroImpl *)cont;
    if (ctx->count < ctx->capacity) {
        uptr alloc = uptr_times_of_16(sizeof(Coro) + stack_size);
        void *p = aligned_memalloc(alloc); // 对齐到 16 字节边界
        void **rsp = (void **)((uptr)p + alloc);
        void *rsp_aligned_to_16 = (void *)rsp;
        void *align_to_16_bytes;
        Coro *co = (Coro *)p;
        co->func = f;
        co->para = para;
        co->wait = null;
        co->retp = null;
        // pstack + alloc <-- 00                 <-- 16字节对齐
        //             -4 <-- 12 对齐填补
        //             -8 <-- 08 对齐填补
        //            -12 <-- 04 co
        //            -16 <-- 00 asm_coro_call   <-- 16字节对齐
        //            -20 <-- 12 ecx
        if (sizeof(void *) == 4) {
            *(--rsp) = 0;
            *(--rsp) = 0;
        }
        // pstack + alloc <-- 00                 <-- 16字节对齐
        //             -8 <-- 08 co
        //            -16 <-- 00 asm_coro_call   <-- 16字节对齐
        //            -24 <-- 08 rcx/rdi
        *(--rsp) = co;                      // 提供给 coro_return 使用
        *(--rsp) = (void *)(uptr)asm_coro_call;   // 创建时为 coro_asm_call，之后会改为协程函数 f 中的返回地址
        align_to_16_bytes = (void *)rsp;    // 对齐到 16 字节边界
        *(--rsp) = co;                      // rcx/rdi
        co->rsp = asm_coro_init((uptr)rsp);
        co->id = ctx->init.id + ctx->count + 1;
        if (ctx->count == 0) {
            ctx->init.wait = co;
        } else {
            ctx->item[ctx->count-1]->wait = co;
        }
        co->wait = &ctx->init;
        ctx->item[ctx->count++] = co;
#if __magic_coro_debug
        printf("[%02d] %p created rsp %p depth %d f %p para %p\n", (int)co->id, (void *)co, (void *)co->rsp,
            (int)((byte*)rsp_aligned_to_16 - (byte*)co->rsp), (void *)(uptr)f, (void *)para);
#endif
        lang_assert((uptr)rsp_aligned_to_16 % 16 == 0);
        lang_assert((uptr)align_to_16_bytes % 16 == 0);
        return co;
    }
    return null;
}

uptr coroutine_cont_id(CoroCont cont)
{
    return ((CoroImpl *)cont)->init.id;
}

uptr coroutine_id(Coro *co)
{
    return co->id;
}

uptr coroutine_para(Coro *co)
{
    return co->para;
}

uptr coroutine_retp(Coro *co)
{
    return co->retp;
}

void coroutine_set_retp(Coro *co, uptr retp)
{
    if (retp != (uptr)-1) {
        co->retp = retp;
    }
}

Coro *coroutineyield_(Coro *co)
{
    Coro *wait = co->wait;
    for (; ;) {
        if (!wait || wait == co) {
            return null;
        }
        if (wait->rsp) {
            return wait;
        }
        wait = wait->wait;
    }
}

void coroutine_yield_with_retp(Coro *co, uptr retp)
{
    Coro *wait;
    coroutine_set_retp(co, retp);
    if ((wait = coroutineyield_(co))) {
        asm_coro_yield(co, wait);
    }
}

void coroutine_yield(Coro *co)
{
    Coro *wait;
    if ((wait = coroutineyield_(co))) {
        asm_coro_yield(co, wait);
    }
}

bool coroutine_yield_cycle(CoroCont cont)
{
    Coro *wait;
    CoroImpl *ctx = (CoroImpl *)cont;
    if ((wait = coroutineyield_(&ctx->init))) {
        return asm_coro_yield(&ctx->init, wait);
    }
    return false;
}

bool coroutine_yield_manual(CoroCont cont, Coro *wait, uptr retp)
{
    CoroImpl *ctx = (CoroImpl *)cont;
    coroutine_set_retp(wait, retp);
    return asm_coro_yield_manual(&ctx->init, wait);
}

void coroutine_finish(CoroCont *cont)
{
    CoroImpl *ctx = (CoroImpl *)*cont;
    if (!ctx) {
        return;
    }
    for (uptr i = 0; i < ctx->count; i += 1) {
        aligned_memfree(ctx->item[i]);
    }
    aligned_memfree(ctx);
    *cont = null;
}
