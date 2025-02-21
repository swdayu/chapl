#include "fastcall.h"

void print_para(uptr a, uptr b, uptr c) {
    printf("fastcall para GNU ecx=%08x %d, edx=%08x %d, argc=%08x %d\n",
        a, a, b, b, c, c);
}

void print_retp(uptr a, uptr b) {
    printf("fastcall retp GNU eax=%08x %d, edx=%08x %d\n", a, a, b, b);
}

__magic_nakedcall(fastcall, void) fastcall_test_para(uptr a, uptr b, uptr c)
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

__magic_func_decl(fastcall, u32) fastcall_return_u32(u32 a, u32 b)
{
    return a * b;
}

__magic_func_decl(fastcall, u64) fastcall_return_u64(u32 a, u32 b)
{
    return ((u64)a) * b;
}

__magic_nakedcall(fastcall, void) fastcall_test_retp_2_x_3(void)
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

__magic_nakedcall(fastcall, void) fastcall_test_retp_big_n(void)
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

__magic_nakedcall(fastcall, data_t) fastcall_test_para_with_bigr(int a, int b)
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
