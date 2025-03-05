#include "conc/conc.h"

int threadproc(struct thrd *t)
{
    threads_current_thrd_info(t);
    printf("[thread %d] index %d\n", threads_get_thrid(t), t->index);
    return 0;
}

int main(int argc, char **argv)
{
    direct_test();

    struct thrd *t = threads_init(30, 3, NULL);
    printf("\n\n\n[GNU][threads][main %d]\n", threads_get_thrid(t));
    threads_create(t, threadproc, 1);
    threads_create(t, threadproc, 0);
    threads_create(t, threadproc, 0);
    threads_join(t);

    return 0;
}
