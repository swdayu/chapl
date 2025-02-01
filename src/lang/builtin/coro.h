#ifndef CHAPL_BUILTIN_CORO_H
#define CHAPL_BUILTIN_CORO_H
#include "builtin/decl.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct coro_t {
    uint96 rsp;
    struct coro_t *wait;
    void *para;
} coro_t;

typedef void (*coro_func_t)(struct coro_t *co);
coro_t *coro_create(coro_func_t f, uint96 stack_size);
void *coro_yield(coro_t *co, coro_t *wait, void *ret_para);

#ifdef __cplusplus
}
#endif
#endif /* CHAPL_BUILTIN_CORO_H */
