#define PRH_CONC_INCLUDE
#include "prh_include.h"

#define CORO_STACK_SIZE 8*1024

// 跨线程协程类型（一）
// 1. 主协程分发多个子任务实现并行执行
// 2. 产生1个主协程和n个子协程
// 3. 主协程分发完任务后挂起，等待子协程的结果
// 4. 所有线程会平等的争抢这(n+1)个协程的执行
// 5. 直到所有的子任务执行完毕，并且主协程得到所有的结果

typedef struct {
    int i, result;
    int *start, *end;
} routine_t;

prh_coro_proc routine(prh_coro_t *coro) {
    routine_t *r = prh_coroutine_userdata(coro);
    int result = 1, *start = r->start;
    for (; start < r->end; start += 1) {
        result *= *start;
    }
    r->result = result;
    prh_cono_finish(coro);
}

prh_coro_proc co_main(prh_coro_t *coro) {
    int a[100];
    int i = 0, result = 1;
    routine_t *p;
    for (; i < PRH_ARRAY_LEN(a); i += 1) {
        a[i] = i + 1;
    }
    for (i = 0; i < 10; i += 1) {
        p = prh_cono_1_create(coro, routine, CORO_STACK_SIZE, sizeof(routine_t));
        p->i = i;
        p->start = a + i * 10;
        p->end = p->start + 10;
        prh_cono_start(p);
    }
    for (i = 0; i < 10; i += 1) {
        p = prh_cono_await(coro);
        result *= p->result;
        printf("recv result [%d] %d\n", p->i, p->result);
    }
    printf("final result: %d\n", result);
}

// 跨线程协程类型（二）
// 1. 有一个公共的可以完成特定任务的协程
// 2. 任何其他协程都可以发送消息给该协程帮忙执行任务
// 3. 发送消息的协程挂起等待结果，其实这里不需要发送消息，直接将内容保存在请求协程的协程栈中即可
// 4. 执行特定任务的协程在递交执行结果时会挂起，然后直接执行发送消息的协程
// 5. 只执行到把结果传递过去，挂起协程，然后执行特定任务的协程继续执行

void cross_coro_test(void) {
    prh_conc_init(2, 100);
    prh_conc_run(co_main, CORO_STACK_SIZE);
}
