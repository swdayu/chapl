#include "fastcall.h"

void print_para(uptr a, uptr b, uptr c) {
    printf("fastcall para MSC ecx=%08x %d, edx=%08x %d, argc=%08x %d\n",
        a, a, b, b, c, c);
}

void print_retp(uptr a, uptr b) {
    printf("fastcall retp MSC eax=%08x %d, edx=%08x %d\n", a, a, b, b);
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

__magic_nakedcall(fastcall, void) fastcall_test_retp_big_n(void)
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

__magic_nakedcall(fastcall, data_t) fastcall_test_para_with_bigr(int a, int b)
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
