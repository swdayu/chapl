#define __CURR_FILE__ STRID_LANG_CORO
#include "internal/decl.h"
#include "coro.h"

CORO_API(void) coro_asm_call(coroutine_t* co);
CORO_API(bool) coro_asm_yield(coroutine_t *co, coroutine_t *wait);
CORO_API(bool) coro_asm_yield_manual(coroutine_t *co, coroutine_t *wait);
CORO_API(upt) coro_asm_init(upt rsp);

CORO_API(coroutine_t*) coro_finish(coroutine_t *co)
{
    coroutine_t *wait = co->wait;
#if CORO_DEBUG
    printf("[%02d] finish: rsp %p wait %p\n", (int)co->id, (void *)co->rsp, wait);
#endif
    co->rsp = 0;
    return wait;
}

CORO_API(void) coro_stack_crash(upt co_rsp, upt rsp)
{
    printf("coro stack crash %p %p\n", (void *)co_rsp, (void *)rsp);
    exit(1);
}

typedef struct {
    coroutine_t init;
    coroutine_t **item;
    upt capacity;
    upt count;
} cocontext_t;

#define sys_malloc(size) _aligned_malloc(size, 16)
#define sys_free(p) _aligned_free(p)

CORO_API(corotpool_t) coroutine_init(upt pool_id, upt count)
{
    count = count > 0 ? count : 1;
    upt alloc = sizeof(cocontext_t) + count * sizeof(coroutine_t*);
    cocontext_t *ctx = (cocontext_t *)sys_malloc(alloc);
    memset(ctx, 0, alloc);
    ctx->init.id = pool_id;
    ctx->capacity = count;
    ctx->item = (coroutine_t **)(ctx + 1);
    return (corotpool_t)ctx;
}

CORO_API(coroutine_t*) coroutine_create(corotpool_t pool, coro_func_t f, upt stack_size, upt para)
{
    cocontext_t *ctx = (cocontext_t *)pool;
    if (ctx->count < ctx->capacity) {
        upt alloc = upt_times_of_16(sizeof(coroutine_t) + stack_size);
        void *p = sys_malloc(alloc); // 对齐到 16 字节边界
        void **rsp = (void **)((upt)p + alloc);
        void *rsp_aligned_to_16 = (void *)rsp;
        void *align_to_16_bytes;
        coroutine_t *co = (coroutine_t *)p;
        co->func = f;
        co->para = para;
        co->wait = null;
        co->retp = null;
        // p + alloc 00                 <-- esp 16 字节对齐
        //        -4 04 对齐填补
        //        -8 08 对齐填补
        //       -12 12 co
        //       -16 16 coro_asm_call   <-- esp 16 字节对齐
        if (sizeof(void *) == 4) {
            *(--rsp) = 0;
            *(--rsp) = 0;
        }
        // p + alloc 00                 <-- rsp 16 字节对齐
        //        -8 08 co
        //       -16 16 coro_asm_call   <-- rsp 16 字节对齐
        *(--rsp) = co;                      // 提供给 coro_return 使用
        *(--rsp) = (void *)coro_asm_call;   // 创建时为 coro_asm_call，之后会改为协程函数 f 中的返回地址
        align_to_16_bytes = (void *)rsp;    // 对齐到 16 字节边界
        *(--rsp) = co;                      // rcx
        co->rsp = coro_asm_init((upt)rsp);
        co->id = ctx->init.id + ctx->count + 1;
        if (ctx->count == 0) {
            ctx->init.wait = co;
        } else {
            ctx->item[ctx->count-1]->wait = co;
        }
        co->wait = &ctx->init;
        ctx->item[ctx->count++] = co;
#if CORO_DEBUG
        printf("[%02d] %p created %p %p rsp %p\n", (int)co->id, co, rsp_aligned_to_16, align_to_16_bytes, (void *)co->rsp);
#endif
        lang_assert((upt)rsp_aligned_to_16 % 16 == 0);
        lang_assert((upt)align_to_16_bytes % 16 == 0);
        return co;
    }
    return null;
}

CORO_API(upt) corotpool_id(corotpool_t pool)
{
    return ((cocontext_t *)pool)->init.id;
}

CORO_API(upt) coroutine_id(coroutine_t *co)
{
    return co->id;
}

CORO_API(upt) coroutine_para(coroutine_t *co)
{
    return co->para;
}

CORO_API(upt) coroutine_retp(coroutine_t *co)
{
    return co->retp;
}

CORO_API(void) coroutine_set_retp(coroutine_t *co, upt retp)
{
    if (retp != (upt)-1) {
        co->retp = retp;
    }
}

CORO_API(coroutine_t*) coroutineyield_(coroutine_t *co)
{
    coroutine_t *wait = co->wait;
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

CORO_API(void) coroutine_yield(coroutine_t *co, upt retp)
{
    coroutine_t *wait;
    coroutine_set_retp(co, retp);
    if ((wait = coroutineyield_(co))) {
        coro_asm_yield(co, wait);
    }
}

CORO_API(bool) coroutine_yield_cycle(corotpool_t pool)
{
    coroutine_t *wait;
    cocontext_t *ctx = (cocontext_t *)pool;
    if ((wait = coroutineyield_(&ctx->init))) {
        return coro_asm_yield(&ctx->init, wait);
    }
    return false;
}

CORO_API(bool) coroutine_yield_manual(corotpool_t pool, coroutine_t *wait, upt retp)
{
    cocontext_t *ctx = (cocontext_t *)pool;
    coroutine_set_retp(wait, retp);
    return coro_asm_yield_manual(&ctx->init, wait);
}

CORO_API(void) coroutine_finish(corotpool_t *pool)
{
    cocontext_t *ctx = (cocontext_t *)*pool;
    if (!ctx) {
        return;
    }
    for (upt i = 0; i < ctx->count; i += 1) {
        sys_free(ctx->item[i]);
    }
    sys_free(ctx);
    *pool = null;
}
