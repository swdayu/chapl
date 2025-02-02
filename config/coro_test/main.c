#include "builtin/coro.h"

void print_dec(intx n)
{
    printf("%lld\n", n);
}

void print_hex(intx n)
{
    printf("%p\n", (void *)n);
}

void counter(coroutine_t *co)
{
    intx i = 0;
    intx n = coroutine_para(co) ? coroutine_para(co) : 10;
    for (; i < n; i += 1) {
        printf("[%02d] %d retp %d switch to %02d\n", (int)coroutine_id(co), (int)i, (int)coroutine_retp(co), (int)coroutine_id(co->wait));
        coroutine_yield(co, (intx)i);
    }
    coroutine_set_retp(co, (intx)i);
}

#define CORO_STACK_SIZE 16*1024

void test(void)
{
    corotpool_t t = coroutine_init(0, 3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (intx)3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (intx)5);
    coroutine_create(t, counter, CORO_STACK_SIZE, null);
    while (coroutine_yield_cycle(t)) ;
    coroutine_finish(&t);
}

void test_manual(void)
{
    corotpool_t t2 = coroutine_init(10, 5);
    coroutine_t *c1 = coroutine_create(t2, counter, CORO_STACK_SIZE, (intx)3);
    coroutine_t *c2 = coroutine_create(t2, counter, CORO_STACK_SIZE, (intx)7);
    coroutine_t *c3 = coroutine_create(t2, counter, CORO_STACK_SIZE, (intx)5);
    while (coroutine_yield_manual(t2, c2, (intx)200)) {
        printf("[%02d] %d\n", (int)corotpool_id(t2), (int)(intx)coroutine_retp(c2));
    }
    while (coroutine_yield_manual(t2, c1, (intx)100)) {
        printf("[%02d] %d\n", (int)corotpool_id(t2), (int)(intx)coroutine_retp(c1));
    }
    while (coroutine_yield_manual(t2, c3, (intx)300)) {
        printf("[%02d] %d\n", (int)corotpool_id(t2), (int)(intx)coroutine_retp(c3));
    }
    coroutine_finish(&t2);
}

int main(int argc, char **argv)
{
    test();
    test_manual();
    return 0;
}
