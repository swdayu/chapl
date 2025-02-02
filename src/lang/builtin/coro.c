#define __CURR_FILE__ STRID_LANG_CORO
#include "internal/decl.h"
#include "builtin/coro.h"

void coro_asm_call(coroutine_t* co);
bool coro_asm_yield(coroutine_t *co, coroutine_t *wait);
bool coro_asm_yield_manual(coroutine_t *co, coroutine_t *wait);

typedef struct {
    coroutine_t init;
    coroutine_t **item;
    intx capacity;
    intx count;
} cocontext_t;

corotpool_t coroutine_init(intx pool_id, intx count)
{
    count = count > 0 ? count : 1;
    intx alloc = sizeof(cocontext_t) + count * sizeof(coroutine_t*);
    cocontext_t *ctx = (cocontext_t *)malloc(alloc);
    memset(ctx, 0, alloc);
    ctx->init.id = pool_id;
    ctx->capacity = count;
    ctx->item = (coroutine_t **)(ctx + 1);
    return (corotpool_t)ctx;
}

coroutine_t *coroutine_create(corotpool_t pool, coroutine_func_t f, intx stack_size, intx para)
{
    cocontext_t *ctx = (cocontext_t *)pool;
    if (ctx->count < ctx->capacity) {
        intx alloc = sizeof(coroutine_t) + stack_size + 32;
        void *p = malloc(alloc);
        void **rsp = (void **)((((intx)p + alloc) >> 5) << 5); // 对齐到 32 字节边界
        void *rsp_aligned_to_32 = (void *)rsp;
        void *align_to_32_bytes;
        coroutine_t *co = (coroutine_t *)p;
        co->func = f;
        co->para = para;
        co->wait = null;
        co->retp = null;
        *(--rsp) = 0;                       // 填补对齐
        *(--rsp) = 0;                       // 填补对齐
        *(--rsp) = co;                      // 提供给 coro_return 使用
        *(--rsp) = (void *)coro_asm_call;   // 创建时为 coro_call，然后会改为协程函数 f 中的返回地址
        align_to_32_bytes = (void *)rsp;    // 对齐到 32 字节边界
        *(--rsp) = co;                      // rcx
        *(--rsp) = 0;                       // rdi
        *(--rsp) = 0;                       // rsi
        *(--rsp) = 0;                       // rbx
        *(--rsp) = 0;                       // rbp
        *(--rsp) = 0;                       // r12
        *(--rsp) = 0;                       // r13
        *(--rsp) = 0;                       // r14
        *(--rsp) = 0;                       // r15
        *(--rsp) = (void *)(rsp + 1);       // rsp
        co->rsp = (intx)rsp;
        co->id = ctx->init.id + ctx->count + 1;
        if (ctx->count == 0) {
            ctx->init.wait = co;
        } else {
            ctx->item[ctx->count-1]->wait = co;
        }
        co->wait = &ctx->init;
        ctx->item[ctx->count++] = co;
        printf("[%02d] created %p %p rsp %p\n", (int)co->id, rsp_aligned_to_32, align_to_32_bytes, (void *)rsp);
        lang_assert((intp)rsp_aligned_to_32 % 16 == 0);
        lang_assert((intp)align_to_32_bytes % 32 == 0);
        return co;
    }
    return null;
}

intx corotpool_id(corotpool_t pool)
{
    return ((cocontext_t *)pool)->init.id;
}

intx coroutine_id(coroutine_t *co)
{
    return co->id;
}

intx coroutine_para(coroutine_t *co)
{
    return co->para;
}

intx coroutine_retp(coroutine_t *co)
{
    return co->retp;
}

void coroutine_set_retp(coroutine_t *co, intx retp)
{
    if (retp != coroutine_nullpara) {
        co->retp = retp;
    }
}

coroutine_t *coroutineyield_(coroutine_t *co)
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

void coroutine_yield(coroutine_t *co, intx retp)
{
    coroutine_t *wait;
    coroutine_set_retp(co, retp);
    if ((wait = coroutineyield_(co))) {
        coro_asm_yield(co, wait);
    }
}

bool coroutine_yield_cycle(corotpool_t pool)
{
    coroutine_t *wait;
    cocontext_t *ctx = (cocontext_t *)pool;
    if ((wait = coroutineyield_(&ctx->init))) {
        return coro_asm_yield(&ctx->init, wait);
    }
    return false;
}

bool coroutine_yield_manual(corotpool_t pool, coroutine_t *wait, intx retp)
{
    cocontext_t *ctx = (cocontext_t *)pool;
    coroutine_set_retp(wait, retp);
    return coro_asm_yield_manual(&ctx->init, wait);
}

void coroutine_finish(corotpool_t *pool)
{
    cocontext_t *ctx = (cocontext_t *)*pool;
    if (!ctx) {
        return;
    }
    for (intx i = 0; i < ctx->count; i += 1) {
        free(ctx->item[i]);
    }
    free(ctx);
    *pool = null;
}

coroutine_t *coro_finish(coroutine_t *co)
{
    coroutine_t *wait = co->wait;
    printf("[%02d] finish: rsp %p wait %p\n", (int)co->id, (void *)co->rsp, wait);
    co->rsp = 0;
    return wait;
}

void coro_rsp_crash(intx co_rsp, intx rsp)
{
    printf("coro rsp crash %p %p\n", (void *)co_rsp, (void *)rsp);
    exit(1);
}
