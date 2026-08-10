#define PRH_THRD_INCLUDE
#define PRH_THRD_IMPLEMENTATION
#define PRH_TEST_IMPLEMENTATION
#include "prh_include.h"

int thrd_proc(prh_thrd *thrd) {
    return 0;
}

int main(int argc, char **argv) {
    prh_main_init();
    prh_thrds *s = prh_thrd_init(30, 5, 0);
    prh_sys_info info;
    prh_system_info(&info);
    prh_thrd_start(s, thrd_proc, 0, prh_null);
    prh_thrd_start(s, thrd_proc, 1, prh_null);
    prh_thrd_start(s, thrd_proc, info.page_size, prh_null);
    prh_thrd_start(s, thrd_proc, info.vmem_unit, prh_null);
    prh_thrd_start(s, thrd_proc, info.vmem_unit+1, prh_null);
    prh_thrd_jall(&s, prh_null);
    return 0;
}
