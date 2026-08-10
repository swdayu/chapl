#include "prh_pretest.h"

int main(int argc, char **argv) {
    prh_set_local_alloc(prh_default_alloc());
    prh_console_setup();
    prh_bit_operation_test();
    prh_bsearch_test();
    return 0;
}
