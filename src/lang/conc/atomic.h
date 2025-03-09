#ifndef magic_conc_atomic_h
#define magic_conc_atomic_h
#include <stdatomic.h>
#include "prelude.h"
#ifdef __cplusplus
extern "C" {
#endif

magic_inline void atomic_int_init(atomic_int *a, int v)
{
    atomic_init(a, v);
}

magic_inline void atomic_int_store(atomic_int *a, int v)
{
    atomic_store(a, v);
}

magic_inline int atomic_int_load(atomic_int *a)
{
    return atomic_load(a);
}

magic_inline int atomic_int_inc(atomic_int *a)
{
    return atomic_fetch_add(a, 1) + 1;
}

magic_inline int atomic_int_dec(atomic_int *a)
{
    return atomic_fetch_sub(a, 1) - 1;
}

magic_inline bool atomic_int_cas(atomic_int *a, int oval, int nval)
{
    return atomic_compare_exchange_weak(a, &oval, nval); // compare and swap
}

magic_inline bool atomic_int_try_lock(atomic_int *a, int v)
{
    if (v == 0)
        return false;
    return atomic_int_cas(a, 0, v);
}

magic_inline void atomic_int_unlock(atomic_int *a)
{
    atomic_int_store(a, 0);
}

typedef atomic_uintptr_t atomic_ptr;

magic_inline void atomic_ptr_init(atomic_ptr *a, void *v)
{
    atomic_init(a, (uintptr_t)v);
}

magic_inline void atomic_ptr_store(atomic_ptr *a, void *v)
{
    atomic_store(a, (uintptr_t)v);
}

magic_inline void *atomic_ptr_load(atomic_ptr *a)
{
    return (void *)atomic_load(a);
}

magic_inline bool atomic_ptr_cas(atomic_ptr *a, void *oval, void *nval)
{
    uintptr_t temp = (uintptr_t)oval;
    return atomic_compare_exchange_weak(a, &temp, (uintptr_t)nval);
}

magic_inline bool atomic_ptr_try_lock(atomic_ptr *a, void *v)
{
    if (v == 0)
        return false;
    return atomic_ptr_cas(a, 0, v);
}

magic_inline void atomic_ptr_unlock(atomic_ptr *a)
{
    atomic_ptr_store(a, 0);
}

struct atomic_que;
int atomic_que_alloc_size(int size);
struct atomic_que *atomic_que_init_inplace(void *addr, int size);
struct atomic_que *atomic_que_init(int size); // size power of 2
void atomic_que_free(struct atomic_que **p);
int atomic_que_size(struct atomic_que *q);
bool atomic_que_push(struct atomic_que *q, void *v); // v shall not be NULL
void *atomic_que_pop(struct atomic_que *q); // return NULL means empty
void *atomic_que_top(struct atomic_que *q);

struct atomic_int_que;
int atomic_int_que_alloc_size(int size);
struct atomic_int_que *atomic_int_que_init_inplace(void *addr, int size);
struct atomic_int_que *atomic_int_que_init(int size); // size power of 2
void atomic_int_que_free(struct atomic_int_que **p);
int atomic_int_que_size(struct atomic_int_que *q);
bool atomic_int_que_push(struct atomic_int_que *q, int v); // v shall not be 0
int atomic_int_que_pop(struct atomic_int_que *q); // return 0 means empty
int atomic_int_que_top(struct atomic_int_que *q);

#ifdef __cplusplus
}
#endif
#endif /* magic_conc_atomic_h */