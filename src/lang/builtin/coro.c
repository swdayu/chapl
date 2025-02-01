#include "builtin/coro.h"

void coro_init_stack(coro_t *co, coro_func_t f);
void coro_resume(coro_t *co, void *ret_param);

void print_dec(uint96 n)
{
    printf("%lld\n", n);
}

void print_hex(uint96 n)
{
    printf("%p\n", (void *)n);
}

coro_t *coro_create(coro_func_t f, uint96 stack_size)
{
    uint96 alloc = sizeof(coro_t) + stack_size + 16;
    byte *p = (byte *)malloc(alloc);
    uint96 rsp = ((((uint96)p) + alloc) >> 4) << 4; // 对齐到 16 字节边界
    coro_t *co = (coro_t *)p;
    co->rsp = rsp;
    co->wait = null;
    co->para = null;
    printf("coro %p rsp %p f %p\n", co, (void *)rsp, f);
    coro_init_stack(co, f);
    return co;
}

void coro_finish(coro_t *co)
{
    coro_t *wait = co->wait;
    printf("coro %p finish\n", co);
    free(co);
    coro_resume(wait, (void *)5);
}

void coro_rsp_not_match(uint96 co_rsp, uint96 rsp)
{
    printf("coro rsp not match %p %p\n", (void *)co_rsp, (void *)rsp);
    exit(1);
}
