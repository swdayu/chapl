#define PRH_THRD_INCLUDE
#define PRH_THRD_IMPLEMENTATION
#define PRH_TEST_IMPLEMENTATION
#include "prh_include.h"

prh_void_ptr thrd_proc(prh_thrd *thrd) {
    return 0;
}

int main(int argc, char **argv) {
    prh_thrd_struct *s = prh_thrd_init(30, 3, 0);
    prh_thrd_start(s, thrd_proc, 1, prh_null);
    prh_thrd_start(s, thrd_proc, 0, prh_null);
    prh_thrd_start(s, thrd_proc, 0, prh_null);
    prh_thrd_jall(&s, prh_null);

    prh_test_code();
    return 0;
}
