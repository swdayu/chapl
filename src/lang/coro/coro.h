#ifndef CHAPL_LANG_CORO_H
#define CHAPL_LANG_CORO_H
#include "builtin/decl.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CORO_DEBUG 1

#ifdef __MSC__
#define CORO_API(ret_type) ret_type __fastcall
#define CORO_FUNC_TYPE(ret_type, func_type_name) ret_type (__fastcall *func_type_name)
#else
#define CORO_API(ret_type) ret_type
#define CORO_FUNC_TYPE(ret_type, func_type_name) ret_type (*func_type_name)
#endif

struct coroutine_t;
typedef upr corotpool_t;
typedef CORO_FUNC_TYPE(void, coro_func_t)(struct coroutine_t* co);

typedef struct coroutine_t {
    upr rsp;
    struct coroutine_t *wait;
    coro_func_t func;
    upr para;
    upr retp;
    upr id;
} coroutine_t;

CORO_API(corotpool_t) coroutine_init(upr pool_id, upr count);
CORO_API(coroutine_t*) coroutine_create(corotpool_t pool, coro_func_t f, upr stack_size, upr para);
CORO_API(upr) corotpool_id(corotpool_t pool);
CORO_API(upr) coroutine_id(coroutine_t *co);
CORO_API(upr) coroutine_para(coroutine_t *co);
CORO_API(upr) coroutine_retp(coroutine_t *co);
CORO_API(void) coroutine_set_retp(coroutine_t *co, upr retp);
CORO_API(bool) coroutine_yield_cycle(corotpool_t pool);
CORO_API(bool) coroutine_yield_manual(corotpool_t pool, coroutine_t *co, upr retp);
CORO_API(void) coroutine_yield(coroutine_t *co, upr retp);
CORO_API(void) coroutine_finish(corotpool_t *pool);

#ifdef __cplusplus
}
#endif
#endif /* CHAPL_LANG_CORO_H */
