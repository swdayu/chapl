#ifndef __magic_direct_thrd_h
#define __magic_direct_thrd_h
#ifdef __cplusplus
extern "C" {
#endif

struct thrd;

struct thrds {
    void **item;
    int maxthreads;
    int count;
};

typedef void (*thrdproc)(void *);

struct thrds *threads_init(int mainid, int maxthreads, int sizethrds);
struct thrd *threads_create(struct thrds *t, thrdproc func, int stacksize, int sizethrd);
struct thrd *threads_get(struct thrds *t, int index);
int thrd_get_id(struct thrd *thrd);
void threads_start(struct thrds *t);
void threads_join(struct thrds **t);

#ifdef __cplusplus
}
#endif
#endif /* __magic_direct_thrd_h */