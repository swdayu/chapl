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

coro_proc_func counter(coro_t *coro) {
    Counter *c = (Counter *)coro_get_data(coro);
    int n = c->n ? c->n : 10;
    for (int i = 0; i < n; i += 1) {
        printf("[coro %02d] %p i=%d recv %d\n", coro_self_id(coro), (void *)coro, i, c->main_to_coro);
        c->coro_to_main = i;
        coro_yield(coro);
    }
    c->coro_to_main += 1;
}

void test_yield_cycle(void) {
    coro_struct_t *s = coro_init(10, 3);
    Counter *c1 = (Counter *)coro_creatx(s, counter, CORO_STACK_SIZE, sizeof(Counter)); c1->n = 3;
    Counter *c2 = (Counter *)coro_creatx(s, counter, CORO_STACK_SIZE, sizeof(Counter)); c2->n = 5;
    coro_creatx(s, counter, CORO_STACK_SIZE, sizeof(Counter));
    while (coro_cycle_start(s)) ;
    coro_finish(&s);
}

void test_yield_manual(void) {
    Counter c = {0};
    coro_struct_t *s = coro_init(20, 5);
    coro_create(s, counter, CORO_STACK_SIZE, &c);
    coro_create(s, counter, CORO_STACK_SIZE, &c);
    coro_create(s, counter, CORO_STACK_SIZE, &c);

    c.n = 3; c.main_to_coro = 200;
    while (coro_start(s, 2)) {
        printf("[main %02d] recv i=%d\n", prh_coro_main_id(s), c.coro_to_main);
        c.main_to_coro += 1;
    }

    c.n = 7; c.main_to_coro = 100;
    while (coro_start(s, 1)) {
        printf("[main %02d] recv i=%d\n", prh_coro_main_id(s), c.coro_to_main);
        c.main_to_coro += 1;
    }

    c.n = 5; c.main_to_coro = 300;
    while (coro_start(s, 3)) {
        printf("[main %02d] recv i=%d\n", prh_coro_main_id(s), c.coro_to_main);
        c.main_to_coro += 1;
    }

    coro_finish(&s);
}

typedef struct {
    int count;
    int value;
} SoroTest;

soro_proc_func soro_count(soro_t *coro) {
    SoroTest *p = (SoroTest *)soro_get_data(coro);
    for (int i = 0; i < p->count; i += 1) {
        p->value = i;
        soro_yield(coro);
    }
    p->value += 1;
}

void test_yield_soro(void) {
    SoroTest st = {0};
    soro_struct_t soro;

    soro_init(&soro, 30);

    soro_create(&soro, soro_count, 384, &st); st.count = 5;
    while (soro_start(&soro)) {
        printf("[soro %02d] recv %d\n", soro.start_id, st.value);
    }

    soro_reload(&soro, soro_count); st.count = 7;
    while (soro_start(&soro)) {
        printf("[soro %02d] recv %d\n", soro.start_id, st.value);
    }

    soro_finish(&soro);
}

void test_lexer(const char *expr);

int main(int argc, char **argv) {
    test_yield_cycle();
    test_yield_manual();
    test_yield_soro();

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
