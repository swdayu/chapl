#define PRH_CORO_INCLUDE
#define PRH_CORO_STRIP_PREFIX
#define PRH_CORO_IMPLEMENTATION
#include "prh_include.h"
#include <stdio.h>

#define CORO_STACK_SIZE 16*1024

typedef struct {
    int n;
    int main_to_coro;
    int coro_to_main;
} Counter;

coro_proc counter(coro_t *coro) {
    Counter *c = (Counter *)coro_data(coro);
    int n = c->n ? c->n : 10;
    for (int i = 0; i < n; i += 1) {
        printf("[coro %02d] %p i=%d recv %d\n", coro_self_id(), (void *)coro, i, c->main_to_coro);
        c->coro_to_main = i;
        coro_yield(coro);
    }
    c->coro_to_main += 1;
}

void test_yield_cycle(void) {
    coro_struct *s = coro_init(3, 10);
    Counter *c1 = (Counter *)coro_creatx(s, counter, CORO_STACK_SIZE, sizeof(Counter)); c1->n = 3;
    Counter *c2 = (Counter *)coro_creatx(s, counter, CORO_STACK_SIZE, sizeof(Counter)); c2->n = 5;
    coro_creatx(s, counter, CORO_STACK_SIZE, sizeof(Counter));
    while (coro_cycle_await(s)) ;
    coro_finish(&s);
}

void test_yield_manual(void) {
    Counter c = {0};
    coro_struct *s = coro_init(5, 20);
    coro_create(s, counter, CORO_STACK_SIZE, &c);
    coro_create(s, counter, CORO_STACK_SIZE, &c);
    coro_create(s, counter, CORO_STACK_SIZE, &c);

    c.n = 3; c.main_to_coro = 200;
    while (coro_await(s, 2)) {
        printf("[main %02d] recv i=%d\n", prh_coro_main_id(s), c.coro_to_main);
        c.main_to_coro += 1;
    }

    c.n = 7; c.main_to_coro = 100;
    while (coro_await(s, 1)) {
        printf("[main %02d] recv i=%d\n", prh_coro_main_id(s), c.coro_to_main);
        c.main_to_coro += 1;
    }

    c.n = 5; c.main_to_coro = 300;
    while (coro_await(s, 3)) {
        printf("[main %02d] recv i=%d\n", prh_coro_main_id(s), c.coro_to_main);
        c.main_to_coro += 1;
    }

    coro_finish(&s);
}

typedef struct {
    int count;
    int value;
} SoloTest;

coro_proc solo_count(coro_t *coro) {
    SoloTest *p = (SoloTest *)coro_data(coro);
    for (int i = 0; i < p->count; i += 1) {
        p->value = i;
        coro_yield(coro);
    }
    p->value += 1;
}

void test_yield_solo(void) {
    SoloTest st = {0};
    solo_struct solo;

    solo_init(&solo, 30);

    solo_create(&solo, solo_count, 384, &st); st.count = 5;
    while (solo_await(&solo)) {
        printf("[solo %02d] recv %d\n", solo.start_id, st.value);
    }

    solo_reload(&solo, solo_count); st.count = 7;
    while (solo_await(&solo)) {
        printf("[solo %02d] recv %d\n", solo.start_id, st.value);
    }

    solo_finish(&solo);
}

void test_lexer(const char *expr);

int main(int argc, char **argv) {
    test_yield_cycle();
    test_yield_manual();
    test_yield_solo();

    printf("\n[lexter test #1]\n");
    test_lexer(prh_null);

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
