#define PRH_IMPLEMENTATION
#define PRH_CONC_INCLUDE
#define PRH_CONC_STRIP_PREFIX
#define PRH_CONC_IMPLEMENTATION
#define PRH_CONC_TEST
#include "prh_include.h"

int threadproc(thrd_t *thrd)
{
    return 0;
}

int main(int argc, char **argv)
{
    thrd_struct *s = thread_init(30, 3, 0);
    thread_create(s, threadproc, 1, 0);
    thread_create(s, threadproc, 0, 0);
    thread_create(s, threadproc, 0, 0);
    thread_join(&s, NULL);

    prh_conc_test();
    return 0;
}
