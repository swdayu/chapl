#include <stdio.h>
#include "solo.h"

#define CORO_STACK_SIZE 16*1024

magic_coro_api(void) counter(struct coro *coro, void *userdata)
{
    int i = 0, n = (int)(intptr_t)userdata;
    n = n ? n : 10;
    for (; i < n; i += 1) {
        printf("[%02d] %d para %d\n", coroutine_id(coro), i, (int)(intptr_t)coroutine_yield_para(coro));
        coroutine_yield_with_para(coro, (void *)(intptr_t)i);
    }
    coroutine_set_yield_para(coro, (void *)(intptr_t)i);
}

void test_yield_cycle(void)
{
    coro_struct main = coroutine_init(0, 3);
    coroutine_create(main, counter, CORO_STACK_SIZE, (void *)(uintptr_t)3);
    coroutine_create(main, counter, CORO_STACK_SIZE, (void *)(uintptr_t)5);
    coroutine_create(main, counter, CORO_STACK_SIZE, NULL);
    while (coroutine_yield_cycle(main)) ;
    coroutine_finish(&main);
}

void test_yield_manual(void)
{
    coro_struct main = coroutine_init(10, 5);
    coroutine_create(main, counter, CORO_STACK_SIZE, (void *)(uintptr_t)3);
    coroutine_create(main, counter, CORO_STACK_SIZE, (void *)(uintptr_t)7);
    coroutine_create(main, counter, CORO_STACK_SIZE, (void *)(uintptr_t)5);

    intptr_t yield_para = 200;
    while (coroutine_yield_manual(main, 2, (void *)yield_para)) {
        printf("[%02d] %d\n", (int)coroutine_main_id(main), (int)(uintptr_t)coroutine_main_para(main));
        yield_para += 1;
    }

    yield_para = 100;
    while (coroutine_yield_manual(main, 1, (void *)yield_para)) {
        printf("[%02d] %d\n", (int)coroutine_main_id(main), (int)(uintptr_t)coroutine_main_para(main));
        yield_para += 1;
    }

    yield_para = 300;
    while (coroutine_yield_manual(main, 3, (void *)yield_para)) {
        printf("[%02d] %d\n", (int)coroutine_main_id(main), (int)(uintptr_t)coroutine_main_para(main));
        yield_para += 1;
    }

    coroutine_finish(&main);
}

struct soloresult {
    int count;
    int value;
};

magic_coro_api(void) solo_count(struct coro *coro, void *userdata)
{
    struct soloresult *p = (struct soloresult *)userdata;
    for (int i = 0; i < p->count; i += 1) {
        p->value = i;
        coroutine_yield(coro);
    }
    p->value += 1;
}

void test_yield_solo(void)
{
    struct soloresult solo;
    solo_struct main = {0};
    solo_create(&main, solo_count, 384, &solo);

    solo.count = 5;
    while (solo_call(&main)) {
        printf("[solo #1] recv %d\n", solo.value);
    }

    solo.count = 7;
    solo_recreate(&main, solo_count, &solo);
    while (solo_call(&main)) {
        printf("[solo #2] recv %d\n", solo.value);
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
