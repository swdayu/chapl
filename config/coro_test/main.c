#include "builtin/coro.h"

void coro_func(coro_t *co)
{
    void *r = 0;
    for (int i = 0; i < 3; i += 1) {
        printf("coro %p print %d wait %p\n", co, i, co->wait);
        r = coro_yield(co, co->wait, (void *)(uint96)i);
        printf("coro %p resumed %d\n", co, (int)(uint96)r);
    }
}

#define CORO_STACK_SIZE 4*1024

int main(int argc, char **argv)
{
    void *r;
    coro_t main_co = {0};
    printf("coro %p rsp %p\n", &main_co, &main_co);

    coro_t *c1 = coro_create(coro_func, CORO_STACK_SIZE);
    coro_t *c2 = coro_create(coro_func, CORO_STACK_SIZE);

    r = coro_yield(&main_co, c1, (void *)100);
    printf("main resumed from c1 %d\n", (int)(uint96)r);
    r = coro_yield(&main_co, c2, (void *)200);
    printf("main resumed from c2 %d\n", (int)(uint96)r);

    r = coro_yield(&main_co, c1, (void *)101);
    printf("main resumed from c1 %d\n", (int)(uint96)r);
    r = coro_yield(&main_co, c2, (void *)201);
    printf("main resumed from c2 %d\n", (int)(uint96)r);

    return 0;
}
