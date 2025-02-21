#include "builtin/decl.h"

typedef struct {
    uptr a;
    f64 b;
    i64 c;
} data_t;

__magic_func_decl(fastcall, void) fastcall_test_para(uptr a, uptr b, uptr c);
__magic_func_decl(fastcall, void) fastcall_test_retp_2_x_3(void);
__magic_func_decl(fastcall, void) fastcall_test_retp_big_n(void);
__magic_func_decl(fastcall, data_t) fastcall_test_para_with_bigr(i32 a, i32 b);
