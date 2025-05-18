#include "fastcall.h"
#include <stdio.h>

void print_para(prh_uinp a, prh_uinp b, prh_uinp c) {
    printf("fastcall para MSC ecx=%08x %d, edx=%08x %d, argc=%08x %d\n",
        a, a, b, b, c, c);
}

void print_retp(prh_uinp a, prh_uinp b) {
    printf("fastcall retp MSC eax=%08x %d, edx=%08x %d\n", a, a, b, b);
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
    __asm {
        mov eax,DWORD PTR [esp+4]   // "movl 4(%esp),%eax\n\t"
        sub esp,12                  // "subl $12,%esp\n\t"
        mov DWORD PTR [esp],ecx     // "movl %ecx,(%esp)\n\t"
        mov DWORD PTR [esp+4],edx   // "movl %edx,4(%esp)\n\t"
        mov DWORD PTR [esp+8],eax   // "movl %eax,8(%esp)\n\t"
        call print_para             // "call print_para\n\t"
        add esp,12                  // "addl $12,%esp\n\t"
        ret 4                       // "ret $4\n\t"
    }
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
    __asm {
        sub esp,12                  // "subl $12,%esp\n\t"
        mov ecx,2                   // "movl $2,%ecx\n\t"
        mov edx,3                   // "movl $3,%edx\n\t"
        call fastcall_return_u32   // "call fastcall_return_int\n\t"
        mov DWORD PTR [esp],eax     // "movl %eax,(%esp)\n\t"
        mov DWORD PTR [esp+4],edx   // "movl $0,4(%esp)\n\t"
        call print_retp             // "call print_retp\n\t"
        add esp,12                  // "addl $12,%esp\n\t"
        ret                         // "ret\n\t"
    }
}

prh_naked_fastcall(void) fastcall_test_retp_big_n(void)
{
    __asm {
        sub esp,12                  // "subl $12,%esp\n\t"
        mov ecx,00ffffffh           // "movl $3,%ecx\n\t"
        mov edx,00eeeeeeh           // "movl $4,%edx\n\t"
        call fastcall_return_u64   // "call fastcall_return_i64\n\t"
        mov DWORD PTR [esp],eax     // "movl %eax,(%esp)\n\t"
        mov DWORD PTR [esp+4],edx   // "movl %edx,4(%esp)\n\t"
        call print_retp             // "call print_retp\n\t"
        add esp,12                  // "addl $12,%esp\n\t"
        ret                         // "ret\n\t"
    }
}

prh_naked_fastcall(data_t) fastcall_test_para_with_bigr(int a, int b)
{
    // retpar address 00 <-- 16字节对齐
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
    __asm {
        mov eax,DWORD PTR [esp+4]
        push eax
        push ecx
        push edx
        sub esp,16
        mov DWORD PTR [esp],ecx
        mov DWORD PTR [esp+4],edx
        mov DWORD PTR [esp+8],eax
        call print_para
        mov eax,DWORD PTR [esp+24]
        mov ecx,DWORD PTR [esp+20]
        add ecx,DWORD PTR [esp+16]
        mov DWORD PTR [eax],ecx
        add esp,16+12
        ret 4
    }
}
