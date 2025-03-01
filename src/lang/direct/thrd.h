#ifndef __magic_direct_thrd_h
#define __magic_direct_thrd_h
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int maxthreads;
    int thread_cnt;
} thrdstruct;

struct thrd {
    thrdstruct *basestruct;
    union { void *tid_impl; int start_id; } u;
    void *userdata;
    unsigned inplace: 1, created: 1, joined: 1;
    int index;
};

typedef int (*thrdproc)(struct thrd* thrd);

static inline struct thrd *threads_main_thrd(struct thrd *thrd) {
    return (struct thrd *)(thrd->basestruct + 1);
}

static inline int threads_get_thrd_id(struct thrd *thrd) {
    return thrd->index + threads_main_thrd(thrd)->u.start_id;
}

int threads_alloc_size(int maxthreads, int *userdata_bytes);
struct thrd *threads_init_inplace(void *addr, int start_id, int maxthreads, int *userdata_bytes);
struct thrd *threads_init(int start_id, int maxthreads, int *userdata_bytes);
struct thrd *threads_get_thrd(struct thrd *main, int index);
void threads_current_thrd_info(struct thrd *thrd);
void threads_create(struct thrd *main, thrdproc proc, int stacksize);
void threads_join(struct thrd *main);

#ifdef __cplusplus
}
#endif
#endif /* __magic_direct_thrd_h */