#include "prh_include.h"

typedef struct {
    prh_uinp a;
    prh_f64 b;
    prh_i64 c;
} data_t;

prh_fastcall(void) fastcall_test_para(prh_uinp a, prh_uinp b, prh_uinp c);
prh_fastcall(void) fastcall_test_retp_2_x_3(void);
prh_fastcall(void) fastcall_test_retp_big_n(void);
prh_fastcall(data_t) fastcall_test_para_with_bigr(int a, int b);
