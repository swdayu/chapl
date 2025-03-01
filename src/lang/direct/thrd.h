#ifndef __magic_direct_thrd_h
#define __magic_direct_thrd_h
#ifdef __cplusplus
extern "C" {
#endif

struct thrd;

struct thrdcont {
    void **item;
    int maxthreads;
    int count;
};

struct thrdindex {
    int n;
};

typedef void (*thrdproc)(void *);
typedef void (*thrdinit)(struct thrd *thrd, void *para);

struct thrdattr {
    int thrdsize;
    thrdinit init;
    void *para;
};

struct thrdcont *threads_init(int mainid, int maxthreads);
void threads_create(struct thrdcont *t, thrdproc proc, int stacksize, struct thrdattr *attr);
void threads_join(struct thrdcont **p);

struct thrd *thread_get_thrd(struct thrdcont *t, int index);
int thread_id_from_index(struct thrdcont *t, int index);
struct thrdindex thread_get_index(struct thrd *thrd);
int thread_get_id(struct thrd *thrd);

#ifdef __cplusplus
}
#endif
#endif /* __magic_direct_thrd_h */