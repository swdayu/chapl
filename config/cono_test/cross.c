#define PRH_CONO_INCLUDE
#define PRH_CONO_IMPLEMENTATION
#include "prh_include.h"

#define MAIN_CORO_STACK_SIZE 1000*1024
#define SUBR_CORO_STACK_SIZE 256

typedef struct {
    int i, result;
    int *start, *end;
} subroutine_t;

prh_cono_proc subroutine(prh_cono *cono) {
    subroutine_t *p = prh_cono_data(cono);
    int result = 0, *start = p->start;
    for (; start < p->end; start += 1) {
        result += *start;
    }
    p->result = result;
}

prh_cono_proc main_coro_proc(prh_cono *cono) {
    int a[100000];
    int sub_coro_cnt = 10;
    int calc_per_sub = 10000;
    int i = 0, result = 0;
    subroutine_t *p;
    for (; i < prh_array_size(a); i += 1) {
        a[i] = i + 1;
    }
    for (i = 0; i < sub_coro_cnt; i += 1) {
        p = prh_cono_spawn(subroutine, SUBR_CORO_STACK_SIZE, sizeof(subroutine_t));
        p->i = i;
        p->start = a + i * calc_per_sub;
        p->end = p->start + calc_per_sub;
        prh_cono_start(cono, (prh_spawn_data *)p);
    }
    for (i = 0; i < sub_coro_cnt; i += 1) {
        p = prh_cono_await(cono);
        result += p->result;
        printf("recv result [%d] %d\n", p->i, p->result);
    }
    printf("final result: %d\n", result);
}

int main(int argc, char **argv) {
    printf("\n\n");
    prh_cono_main(70, 3, main_coro_proc, MAIN_CORO_STACK_SIZE);
    printf("\n\nOnly use main thread:\n");
    prh_cono_main(90, 0, main_coro_proc, MAIN_CORO_STACK_SIZE);
    return 0;
}
