#ifndef magic_direct_thrd_h
#define magic_direct_thrd_h
#include "prelude.h"
#ifdef __cplusplus
extern "C" {
#endif

struct thrd;

struct thrdindex {
    int n;
};

typedef int (*thrdproc)(struct thrd* thrd);

int threads_get_thrid(struct thrd *thrd);
struct thrdindex threads_index(struct thrd *thrd);
magic_inline int threads_get_index(struct thrd *thrd) { return threads_index(thrd).n; }
struct thrd *threads_get_thrd(struct thrd *main, int index);
void *threads_get_userdata(struct thrd *thrd);
void threads_current_info(struct thrd *thrd);

int threads_alloc_size(int maxthreads, int *userdata_bytes); // userdata_bytes[i]<0 end =1 same, [0] for main [1..maxthreads+1] thrds
struct thrd *threads_init_inplace(void *addr, int start_id, int maxthreads, int *userdata_bytes);
struct thrd *threads_init(int start_id, int maxthreads, int *userdata_bytes);
void threads_create(struct thrd *main, thrdproc proc, int stacksize);
void threads_join(struct thrd *main);

#ifdef __cplusplus
}
#endif
#endif /* magic_direct_thrd_h */