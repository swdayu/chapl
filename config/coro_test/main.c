#include "coro.h"

#define CORO_STACK_SIZE 16*1024

__magic_coro_api(void) counter(Coro *co)
{
    uptr i = 0;
    uptr n = coroutine_para(co) ? coroutine_para(co) : 10;
    for (; i < n; i += 1) {
        printf("[%02d] %d retp %d switch to %02d\n", (int)coroutine_id(co), (int)i, (int)coroutine_retp(co), (int)coroutine_id(co->wait));
        coroutine_yield_with_retp(co, (uptr)i);
    }
    coroutine_set_retp(co, (uptr)i);
}

void test_yield_cycle(void)
{
    CoroCont t = coroutine_init(0, 3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (uptr)3);
    coroutine_create(t, counter, CORO_STACK_SIZE, (uptr)5);
    coroutine_create(t, counter, CORO_STACK_SIZE, null);
    while (coroutine_yield_cycle(t)) ;
    coroutine_finish(&t);
}

void test_yield_manual(void)
{
    CoroCont t2 = coroutine_init(10, 5);
    Coro *c1 = coroutine_create(t2, counter, CORO_STACK_SIZE, (uptr)3);
    Coro *c2 = coroutine_create(t2, counter, CORO_STACK_SIZE, (uptr)7);
    Coro *c3 = coroutine_create(t2, counter, CORO_STACK_SIZE, (uptr)5);
    while (coroutine_yield_manual(t2, c2, (uptr)200)) {
        printf("[%02d] %d\n", (int)coroutine_cont_id(t2), (int)(uptr)coroutine_retp(c2));
    }
    while (coroutine_yield_manual(t2, c1, (uptr)100)) {
        printf("[%02d] %d\n", (int)coroutine_cont_id(t2), (int)(uptr)coroutine_retp(c1));
    }
    while (coroutine_yield_manual(t2, c3, (uptr)300)) {
        printf("[%02d] %d\n", (int)coroutine_cont_id(t2), (int)(uptr)coroutine_retp(c3));
    }
    coroutine_finish(&t2);
}

void test_lexer(const char *expr);

int main(int argc, char **argv)
{
    test_yield_cycle();
    test_yield_manual();

    printf("\n[start lexter test #1]\n");
    test_lexer(null);

    printf("\n[start lexter test #2]\n");
    test_lexer("1 + 2 * 3 - 4 / 5 + 6");

    printf("\n[start lexter test #3]\n");
    test_lexer("10 + 20 * 30 - 40 / 5 + 6501");

    printf("\n[start lexter test #4]\n");
    test_lexer("11 + 22 * 33 - 44 / 0 + 66");

    printf("\n[start lexter test #5]\n");
    test_lexer("11 * 22 / 33 + 44 - 55 + 66");

    printf("\n[start lexter test #6]\n");
    test_lexer("11 * 22 + 33 + 44 - 55 / 66");

    printf("\n[start lexter test #7]\n");
    test_lexer("11 - 22 + 33 + 44 * 55 + 66");

    printf("\n[start lexter test #8]\n");
    test_lexer("11 - 22 + 33 + 44 * 55 / 66");

    return 0;
}
