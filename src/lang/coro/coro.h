#ifndef magic_lang_coro_h
#define magic_lang_coro_h
#include "prelude.h"
#ifdef __cplusplus
extern "C" {
#endif

struct coro;

struct coroindex {
    int n;
};

#define magic_coro_api(ret) magic_func_decl(fastcall, ret)
magic_func_type(fastcall, void, coroproc)(struct coro *);

struct coro *coroutine_get_coro(struct coro *main, int index);
void *coroutine_precreate_userdata(struct coro *main, int index);
void *coroutine_userdata(struct coro *coro);
void *coroutine_yield_para(struct coro *coro);
struct coroindex coroutine_index(struct coro *coro);
magic_inline int coroutine_get_index(struct coro *coro) { return coroutine_index(coro).n; }
int coroutine_id(struct coro *coro);

int coroutine_alloc_size(int maxcoros, int *userdata_bytes);
struct coro *coroutine_init_inplace(void *addr, int start_id, int maxcoros, int *userdata_bytes);
struct coro *coroutine_init(int start_id, int maxcoros, int *userdata_bytes);
void coroutine_create(struct coro *main, coroproc proc, int stack_size, void *para);
bool coroutine_yield_cycle(struct coro *main);
bool coroutine_yield_manual(struct coro *main, int index, void *yield_para);
void coroutine_finish(struct coro *main);

void coroutine_yield(struct coro *coro);
void coroutine_yield_with_para(struct coro *coro, void *yield_para);
void coroutine_set_yield_para(struct coro *coro, void *yield_para);

#ifdef __cplusplus
}
#endif
#endif /* magic_lang_coro_h */
