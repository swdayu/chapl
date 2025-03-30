#ifndef magic_direct_thrd_h
#define magic_direct_thrd_h
#include "prelude.h"
#ifdef __cplusplus
extern "C" {
#endif

struct thrd;

struct thrd_index {
    int n;
};

typedef int (*thrdproc)(struct thrd* thrd);

int threads_get_thrid(struct thrd *thrd);
struct thrd_index threads_index(struct thrd *thrd);
magic_inline int threads_get_index(struct thrd *thrd) { return threads_index(thrd).n; }
struct thrd *threads_get_thrd(struct thrd *main, int index);
void *threads_get_userdata(struct thrd *thrd);
void threads_current_info(struct thrd *thrd);

int threads_alloc_size(int maxthreads, int *userdata_bytes);
struct thrd *threads_init_inplace(void *addr, int start_id, int maxthreads, int *userdata_bytes);
struct thrd *threads_init(int start_id, int maxthreads, int *userdata_bytes);
void threads_create(struct thrd *main, thrdproc proc, int stacksize);
void threads_join(struct thrd *main);

#if defined(_WIN32)

#else
#include <pthread.h>
struct thrd_mutex {
    pthread_mutex_t mutex;
};
struct thrd_cond {
    struct thrd_mutex mutex;
    pthread_cond_t cond;
};
#endif

void thrd_mutex_init(struct thrd_mutex *p);
void thrd_mutex_lock(struct thrd_mutex *p);
void thrd_mutex_unlock(struct thrd_mutex *p);
void thrd_mutex_free(struct thrd_mutex *p);

void thrd_cond_init(struct thrd_cond *p);
void thrd_cond_wait(struct thrd_cond *p);
bool thrd_cond_timedwait(struct thrd_cond *p, unsigned msec);
void thrd_cond_signal(struct thrd_cond *p);
void thrd_cond_broadcast(struct thrd_cond *p);
void thrd_cond_free(struct thrd_cond *p);

#ifdef __cplusplus
}
#endif
#endif /* magic_direct_thrd_h */