#ifndef CHAPL_BUILTIN_CORO_H
#define CHAPL_BUILTIN_CORO_H
#include "builtin/decl.h"
#ifdef __cplusplus
extern "C" {
#endif

struct coroutine_t;
typedef intx corotpool_t;
typedef void (*coroutine_func_t)(struct coroutine_t* co);
#define coroutine_nullpara (-1)

typedef struct coroutine_t {
    intx rsp;
    struct coroutine_t *wait;
    coroutine_func_t func;
    intx para;
    intx retp;
    intx id;
} coroutine_t;

corotpool_t coroutine_init(intx pool_id, intx count);
coroutine_t *coroutine_create(corotpool_t pool, coroutine_func_t f, intx stack_size, intx para);
intx corotpool_id(corotpool_t pool);
intx coroutine_id(coroutine_t *co);
intx coroutine_para(coroutine_t *co);
intx coroutine_retp(coroutine_t *co);
void coroutine_set_retp(coroutine_t *co, intx retp);
bool coroutine_yield_cycle(corotpool_t pool);
bool coroutine_yield_manual(corotpool_t pool, coroutine_t *co, intx retp);
void coroutine_yield(coroutine_t *co, intx retp);
void coroutine_finish(corotpool_t *pool);

#ifdef __cplusplus
}
#endif
#endif /* CHAPL_BUILTIN_CORO_H */
