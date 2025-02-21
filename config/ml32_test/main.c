#include "fastcall.h"

void test_fastcall(void)
{
    printf("fastcall (0xaabbccdd, 0xbbccddee, 0xccddeeff)\n");
    fastcall_test_para(0xaabbccdd, 0xbbccddee, 0xccddeeff);
    printf("fastcall retp 32-bit 2 * 3\n");
    fastcall_test_retp_2_x_3();
    printf("fastcall retp 64-bit 0xffffff * 0xeeeeee = 0xeeee_ed111112\n");
    fastcall_test_retp_big_n();
    printf("fastcall retp big struct data(3 + 5)\n");
    data_t data = fastcall_test_para_with_bigr(3, 5);
    printf("fastcall data.a=%08x %d\n", data.a, data.a);
}

int main(int argc, char** argv)
{
    test_fastcall();
    return 0;
}
