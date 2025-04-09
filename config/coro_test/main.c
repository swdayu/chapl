#define PRH_STRIP_PREFIX
#include "coroutine.h"
#include <stdio.h>

#define CORO_STACK_SIZE 16*1024

typedef struct {
    int n;
    int main_to_coro;
    int coro_to_main;
} Counter;

magic_coroproc counter(struct coro *coro)
{
    Counter *c = (Counter *)coroutine_userdata(coro);
    int n = c->n ? c->n : 10;
    for (int i = 0; i < n; i += 1) {
        printf("[coro] %p #%d %d para %d\n", (void *)coro, coroutine_id(coro), i, c->main_to_coro);
        c->coro_to_main = i;
        coroutine_yield(coro);
    }
    c->coro_to_main += 1;
}

void test_yield_cycle(void)
{
    coro_struct *main = coroutine_init(3);
    Counter *c1 = (Counter *)coroutine_ext_create(main, counter, CORO_STACK_SIZE, sizeof(Counter)); c1->n = 3;
    Counter *c2 = (Counter *)coroutine_ext_create(main, counter, CORO_STACK_SIZE, sizeof(Counter)); c2->n = 5;
    coroutine_ext_create(main, counter, CORO_STACK_SIZE, sizeof(Counter));
    while (coroutine_start_cycle(main)) ;
    coroutine_finish(&main);
}

void test_yield_manual(void)
{
    Counter c = {0};
    coro_struct *main = coroutine_init(5);
    coroutine_create(main, counter, CORO_STACK_SIZE, &c);
    coroutine_create(main, counter, CORO_STACK_SIZE, &c);
    coroutine_create(main, counter, CORO_STACK_SIZE, &c);

    c.n = 3; c.main_to_coro = 200;
    while (coroutine_start(main, 2)) {
        printf("[main] %p para %d\n", coroutine_main(main), c.coro_to_main);
        c.main_to_coro += 1;
    }

    c.n = 7; c.main_to_coro = 100;
    while (coroutine_start(main, 1)) {
        printf("[main] %p para %d\n", coroutine_main(main), c.coro_to_main);
        c.main_to_coro += 1;
    }

    c.n = 5; c.main_to_coro = 300;
    while (coroutine_start(main, 3)) {
        printf("[main] %p para %d\n", coroutine_main(main), c.coro_to_main);
        c.main_to_coro += 1;
    }

    coroutine_finish(&main);
}

typedef struct {
    int count;
    int value;
} SoloTest;

magic_coroproc solo_count(struct coro *coro)
{
    SoloTest *p = (SoloTest *)coroutine_userdata(coro);
    for (int i = 0; i < p->count; i += 1) {
        p->value = i;
        coroutine_yield(coro);
    }
    p->value += 1;
}

void test_yield_solo(void)
{
    SoloTest st = {0};
    solo_struct main = {NULL};

    solo_create(&main, solo_count, 384, &st); st.count = 5;
    while (solo_start(&main)) {
        printf("[solo #1] recv %d\n", st.value);
    }

    solo_reload(&main, solo_count); st.count = 7;
    while (solo_start(&main)) {
        printf("[solo #2] recv %d\n", st.value);
    }

    solo_finish(&main);
}

void test_lexer(const char *expr);

int main(int argc, char **argv)
{
    test_yield_cycle();
    test_yield_manual();
    test_yield_solo();

    printf("\n[lexter test #1]\n");
    test_lexer(NULL);

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
