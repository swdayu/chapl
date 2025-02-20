#ifndef CHAPL_LANG_CORO_H
#define CHAPL_LANG_CORO_H
#include "builtin/decl.h"
#ifdef __cplusplus
extern "C" {
#endif

#define __magic_coro_debug 1
#define __magic_coro_api(type) __magic_func_decl(fastcall, type)

struct Coro;
typedef uptr CoroCont;
__magic_func_typedef(fastcall, void, CoroFunc)(struct Coro *);

typedef struct Coro {
    uptr rsp;
    struct Coro *wait;
    CoroFunc func;
    uptr para;
    uptr retp;
    uptr id;
} Coro;

CoroCont coroutine_init(uptr cont_id, uptr count);
Coro *coroutine_at(CoroCont cont, uptr index);
Coro *coroutine_create(CoroCont cont, CoroFunc func, uptr stack_size, uptr para);
uptr coroutine_cont_id(CoroCont cont);
uptr coroutine_id(Coro *coro);
uptr coroutine_para(Coro *coro);
uptr coroutine_retp(Coro *coro);
void coroutine_set_retp(Coro *coro, uptr retp);
bool coroutine_yield_cycle(CoroCont cont);
bool coroutine_yield_manual(CoroCont cont, Coro *coro, uptr retp);
void coroutine_yield_with_retp(Coro *co, uptr retp);
void coroutine_yield(Coro *coro);
void coroutine_finish(CoroCont *cont);

#ifdef __cplusplus
}
#endif
#endif /* CHAPL_LANG_CORO_H */
