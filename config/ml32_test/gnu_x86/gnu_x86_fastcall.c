#include "fastcall.h"
#include <stdio.h>

void print_para(prh_uinp a, prh_uinp b, prh_uinp c) {
    printf("fastcall para GNU ecx=%08x %d, edx=%08x %d, argc=%08x %d\n",
        a, a, b, b, c, c);
}

void print_retp(prh_uinp a, prh_uinp b) {
    printf("fastcall retp GNU eax=%08x %d, edx=%08x %d\n", a, a, b, b);
}

prh_naked_fastcall(void) fastcall_test_para(prh_uinp a, prh_uinp b, prh_uinp c)
{
    //              c 00 <-- 16字节对齐
    // return address 12 <-- esp
    //             -4 08 arg c
    //             -8 04 arg b
    //            -12 00 arg a
    // return address 12
    //     print_para 08
    __asm__ (
        "movl 4(%esp),%eax\n\t"
        "subl $12,%esp\n\t"
        "movl %ecx,(%esp)\n\t"
        "movl %edx,4(%esp)\n\t"
        "movl %eax,8(%esp)\n\t"
        "call print_para\n\t"
        "addl $12,%esp\n\t"
        "ret $4\n\t"
    );
}

prh_fastcall(prh_u32) fastcall_return_u32(prh_u32 a, prh_u32 b)
{
    return a * b;
}

prh_fastcall(prh_u64) fastcall_return_u64(prh_u32 a, prh_u32 b)
{
    return ((prh_u64)a) * b;
}

prh_naked_fastcall(void) fastcall_test_retp_2_x_3(void)
{
    __asm__ (
        "subl $12,%esp\n\t"
        "movl $2,%ecx\n\t"
        "movl $3,%edx\n\t"
        "call fastcall_return_u32\n\t"
        "movl %eax,(%esp)\n\t"
        "movl %edx,4(%esp)\n\t"
        "call print_retp\n\t"
        "addl $12,%esp\n\t"
        "ret\n\t"
    );
}

prh_naked_fastcall(void) fastcall_test_retp_big_n(void)
{
    __asm__ (
        "subl $12,%esp\n\t"
        "movl $0x00ffffff,%ecx\n\t"
        "movl $0x00eeeeee,%edx\n\t"
        "call fastcall_return_u64\n\t"
        "movl %eax,(%esp)\n\t"
        "movl %edx,4(%esp)\n\t"
        "call print_retp\n\t"
        "addl $12,%esp\n\t"
        "ret\n\t"
    );
}

prh_naked_fastcall(data_t) fastcall_test_para_with_bigr(int a, int b)
{
    //              b 00 <-- 16字节对齐
    // return address 12 esp
    //                08 eax   esp+24
    //                04 ecx   esp+20
    //            esp 00 edx   esp+16
    //             -4 12       esp+12
    //             -8 08 arg c esp+8
    //            -12 04 arg b esp+4
    //            -16 00 arg a <-- esp
    // return address 12
    //     print_para 08
    __asm__ (
        "movl 4(%esp),%eax\n\t"
        "pushl %ecx\n\t"        // 返回值内存地址 -> %eax
        "pushl %edx\n\t"        // 参数 a        -> %ecx
        "pushl %eax\n\t"        // 参数 b        -> %edx
        "subl $16,%esp\n\t"
        "movl %ecx,(%esp)\n\t"
        "movl %edx,4(%esp)\n\t"
        "movl %eax,8(%esp)\n\t"
        "call print_para\n\t"
        "movl 24(%esp),%eax\n\t"
        "movl 20(%esp),%ecx\n\t"
        "addl 16(%esp),%ecx\n\t"
        "movl %ecx,(%eax)\n\t"
        "addl $(16 + 12),%esp\n\t"
        "ret $4\n\t"
    );
}
