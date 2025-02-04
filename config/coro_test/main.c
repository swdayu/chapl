#include "coro.h"

#define CORO_STACK_SIZE 16*1024

CORO_API(void) counter(coroutine_t *co)
{
    upr i = 0;
    upr n = coroutine_para(co) ? coroutine_para(co) : 10;
    for (; i < n; i += 1) {
        printf("[%02d] %d retp %d switch to %02d\n", (int)coroutine_id(co), (int)i, (int)coroutine_retp(co), (int)coroutine_id(co->wait));
        coroutine_yield(co, (upr)i);
    }
    coroutine_set_retp(co, (upr)i);
}

void test(void)
{
    corotpool_t t = coroutine_init(0, 3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (upr)3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (upr)5);
    coroutine_create(t, counter, CORO_STACK_SIZE, null);
    while (coroutine_yield_cycle(t)) ;
    coroutine_finish(&t);
}

void test_manual(void)
{
    corotpool_t t2 = coroutine_init(10, 5);
    coroutine_t *c1 = coroutine_create(t2, counter, CORO_STACK_SIZE, (upr)3);
    coroutine_t *c2 = coroutine_create(t2, counter, CORO_STACK_SIZE, (upr)7);
    coroutine_t *c3 = coroutine_create(t2, counter, CORO_STACK_SIZE, (upr)5);
    while (coroutine_yield_manual(t2, c2, (upr)200)) {
        printf("[%02d] %d\n", (int)corotpool_id(t2), (int)(upr)coroutine_retp(c2));
    }
    while (coroutine_yield_manual(t2, c1, (upr)100)) {
        printf("[%02d] %d\n", (int)corotpool_id(t2), (int)(upr)coroutine_retp(c1));
    }
    while (coroutine_yield_manual(t2, c3, (upr)300)) {
        printf("[%02d] %d\n", (int)corotpool_id(t2), (int)(upr)coroutine_retp(c3));
    }
    coroutine_finish(&t2);
}

int main(int argc, char **argv)
{
    test();
    test_manual();
    return 0;
}
