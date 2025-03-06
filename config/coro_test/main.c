#include <stdint.h>
#include "coro.h"

#define CORO_STACK_SIZE 16*1024

__magic_coro_api(void) counter(Coro *co)
{
    int i = 0;
    int n = (int)(intptr_t)coroutine_para(co) ? (int)(intptr_t)coroutine_para(co) : 10;
    for (; i < n; i += 1) {
        printf("[%02d] %d retp %d\n", coroutine_id(co), i, (int)(intptr_t)coroutine_retp(co));
        coroutine_yield_with_retp(co, (void *)(intptr_t)i);
    }
    coroutine_set_retp(co, (void *)(intptr_t)i);
}

void test_yield_cycle(void)
{
    CoroCont t = coroutine_init(0, 3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (void *)(uintptr_t)3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (void *)(uintptr_t)5);
    coroutine_create(t, counter, CORO_STACK_SIZE, null);
    while (coroutine_yield_cycle(t)) ;
    coroutine_finish(&t);
}

void test_yield_manual(void)
{
    CoroCont t2 = coroutine_init(10, 5);
    Coro *c1 = coroutine_create(t2, counter, CORO_STACK_SIZE, (void *)(uintptr_t)3);
    Coro *c2 = coroutine_create(t2, counter, CORO_STACK_SIZE, (void *)(uintptr_t)7);
    Coro *c3 = coroutine_create(t2, counter, CORO_STACK_SIZE, (void *)(uintptr_t)5);
    while (coroutine_yield_manual(c2, (void *)(uptr)200)) {
        printf("[%02d] %d\n", (int)coroutine_cont_id(t2), (int)(uptr)coroutine_retp(c2));
    }
    while (coroutine_yield_manual(c1, (void *)(uptr)100)) {
        printf("[%02d] %d\n", (int)coroutine_cont_id(t2), (int)(uptr)coroutine_retp(c1));
    }
    while (coroutine_yield_manual(c3, (void *)(uptr)300)) {
        printf("[%02d] %d\n", (int)coroutine_cont_id(t2), (int)(uptr)coroutine_retp(c3));
    }
    coroutine_finish(&t2);
}

void test_lexer(const char *expr);

int main(int argc, char **argv)
{
    test_yield_cycle();
    test_yield_manual();

    printf("\n[lexter test #1]\n");
    test_lexer(null);

    printf("\n[lexter test #2]\n");
    test_lexer("1 + 2 * 3 - 4 / 5 + 6");

    printf("\n[lexter test #3]\n");
    test_lexer("10 + 20 * 30 - 40 / 5 + 6501");

    printf("\n[lexter test #4]\n");
    test_lexer("11 + 22 * 33 - 44 / 0 + 66");

    printf("\n[lexter test #5]\n");
    test_lexer("11 * 22 / 33 + 44 - 55 + 66");

    printf("\n[lexter test #6]\n");
    test_lexer("11 * 22 + 33 + 44 - 55 / 66");

    printf("\n[lexter test #7]\n");
    test_lexer("11 - 22 + 33 + 44 * 55 + 66");

    printf("\n[lexter test #8]\n");
    test_lexer("11 - 22 + 33 + 44 * 55 / 66");

    printf("\n[lexter with parentheses #1]\n");
    test_lexer("(10 + 20) * 30 - 40 / 5 + 60");

    printf("\n[lexter with parentheses #2]\n");
    test_lexer("10 + 20 * (30 - 4) / (5 + 60)");

    printf("\n[lexter with parentheses #3]\n");
    test_lexer("((10 - 20) + (30 + 40)) * 50 / 60");

    printf("\n[lexter with parentheses #4]\n");
    test_lexer("(10 + (20 * (30 - 40 / (5 + 60");

    printf("\n[lexter with parentheses #5]\n");
    test_lexer("10 - 20 * ((((30 + 40) * 50 / 60");

    return 0;
}
