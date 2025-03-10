#ifndef magic_lang_coro_h
#define magic_lang_coro_h
#include "prelude.h"
#ifdef __cplusplus
extern "C" {
#endif

struct coro;

typedef struct {
    void *impl;
} coro_struct;

struct coro_index {
    int n;
};

#define magic_coro_api(ret) magic_func_decl(fastcall, ret)
magic_func_type(fastcall, void, coroproc)(struct coro *, void *userdata);

int coroutine_alloc_size(int maxcoros);
coro_struct coroutine_init_inplace(void *addr, int start_id, int maxcoros);
coro_struct coroutine_init(int start_id, int maxcoros);

void coroutine_create(coro_struct main, coroproc proc, int stack_size, void *userdata);
void *coroutine_create_with_struct(coro_struct main, coroproc proc, int stack_size, int userdata_bytes);

struct coro *coroutine_get_coro(coro_struct main, int index);
int coroutine_main_id(coro_struct main);
void *coroutine_main_para(coro_struct main);

bool coroutine_yield_cycle(coro_struct main);
bool coroutine_yield_manual(coro_struct main, int index, void *yield_para);
void coroutine_finish(coro_struct *main);

void coroutine_yield(struct coro *coro);
void coroutine_yield_with_para(struct coro *coro, void *yield_para);
void coroutine_set_yield_para(struct coro *coro, void *yield_para);

void *coroutine_yield_para(struct coro *coro);
struct coro_index coroutine_index(struct coro *coro);
magic_inline int coroutine_get_index(struct coro *coro) { return coroutine_index(coro).n; }
int coroutine_id(struct coro *coro);

#endif /* magic_lang_coro_h */
