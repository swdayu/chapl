#include <assert.h>
#include <stdlib.h>
#include "conc/atomic.h"

#define ispow2(x) (((x) & ((x) - 1)) == 0) // power of 2 or zero
#define nexpos(q, pos) ((pos + 1) & (q->u.size - 1))

struct atomic_que {
    union { unsigned size: 31, free: 1; void *align; } u;
    atomic_int head;
    atomic_int tail;
};

magic_inline int atomicqueallocsize(int size, int stride)
{
    assert(ispow2((unsigned)size));
    return magic_align(sizeof(struct atomic_que) + stride * size);
}

int atomic_que_alloc_size(int size)
{
    return atomicqueallocsize(size, sizeof(void *));
}

int atomic_int_que_alloc_size(int size)
{
    return atomicqueallocsize(size, sizeof(int));
}

struct atomic_que *atomic_que_init_inplace(void *addr, int size)
{
    struct atomic_que *q = (struct atomic_que *)addr;
    q->u.size = size;
    q->u.free = 0;
    atomic_int_init(&q->head, 0);
    atomic_int_init(&q->tail, 0);
    return q;
}

struct atomic_int_que *atomic_int_que_init_inplace(void *addr, int size)
{
    return (struct atomic_int_que *)atomic_que_init_inplace(addr, size);
}

struct atomic_que *atomic_que_init(int size)
{
    void *addr = malloc(atomic_que_alloc_size(size));
    struct atomic_que *q = atomic_que_init_inplace(addr, size);
    q->u.free = 1;
    return q;
}

struct atomic_int_que *atomic_int_que_init(int size)
{
    void *addr = malloc(atomic_int_que_alloc_size(size));
    struct atomic_int_que *q = atomic_int_que_init_inplace(addr, size);
    ((struct atomic_que *)q)->u.free = 1;
    return q;
}

void atomic_que_free(struct atomic_que **p)
{
    struct atomic_que *q = *p;
    if (q == NULL)
        return;
    if (q->u.free) {
        free(q);
    }
    *p = NULL;
}

void atomic_int_que_free(struct atomic_int_que **p)
{
    atomic_que_free((struct atomic_que **)p);
}

int atomic_que_size(struct atomic_que *q)
{
    int len = atomic_int_load(&q->tail) - atomic_int_load(&q->head);
    if (len < 0)
        len += q->u.size;
    return len;
}

int atomic_int_que_size(struct atomic_int_que *q)
{
    return atomic_que_size((struct atomic_que *)q);
}

bool queuefinishpush_(struct atomic_que *q, int tail)
{
    int next = nexpos(q, tail);
    if (next == atomic_int_load(&q->head))
        return false;
    assert(atomic_int_load(&q->tail) == tail); // only allow one writer
    atomic_int_store(&q->tail, next);
    return true;
}

void queuefinishpop_(struct atomic_que *q, int head)
{
    assert(atomic_int_load(&q->head) == head); // only allow one reader
    atomic_int_store(&q->head, nexpos(q, head));
}

#define get_que_ptr(q) ((void **)((struct atomic_que *)q + 1))
#define get_que_int(q) ((int *)((struct atomic_que *)q + 1))

bool atomic_que_push(struct atomic_que *q, void *v)
{
    assert(v != NULL);
    int tail = atomic_int_load(&q->tail);
    get_que_ptr(q)[tail] = v;
    return queuefinishpush_(q, tail);
}

void *atomic_que_pop(struct atomic_que *q)
{
    int head = atomic_int_load(&q->head);
    if (head == atomic_int_load(&q->tail)) {
        return NULL;
    }
    void *v = get_que_ptr(q)[head];
    queuefinishpop_(q, head);
    return v;
}

void *atomic_que_top(struct atomic_que *q)
{
    int head = atomic_int_load(&q->head);
    if (head == atomic_int_load(&q->tail)) {
        return NULL;
    }
    return get_que_ptr(q)[head];
}

bool atomic_int_que_push(struct atomic_int_que *q, int v)
{
    assert(v != 0);
    int tail = atomic_int_load(&((struct atomic_que *)q)->tail);
    get_que_int(q)[tail] = v;
    return queuefinishpush_((struct atomic_que *)q, tail);
}

int atomic_int_que_pop(struct atomic_int_que *q) // return 0 means empty
{
    int head = atomic_int_load(&((struct atomic_que *)q)->head);
    if (head == atomic_int_load(&((struct atomic_que *)q)->tail)) {
        return 0;
    }
    int v = get_que_int(q)[head];
    queuefinishpop_((struct atomic_que *)q, head);
    return v;
}

int atomic_int_que_top(struct atomic_int_que *q)
{
    int head = atomic_int_load(&((struct atomic_que *)q)->head);
    if (head == atomic_int_load(&((struct atomic_que *)q)->tail)) {
        return 0;
    }
    return get_que_int(q)[head];
}
