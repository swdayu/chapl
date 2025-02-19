// Windows 32 位程序 __fastcall 调用约定：被调函数清理栈参数，前两个整型参数通过寄存器
// ECX EDX 传递，超过四字节的和额外的整型参数从右到左通过栈传递，对于类、结构体、或联合
// 体，不管其大小都通过栈传递。由于是被调函数自动清理栈参数，__fastcall 调用约定的函数必
// 须提供函数原型，否则编译器不能确定参数的总长度。__fastcall 的函数名称装饰规则：添加一
// 个前置 @ 字符，并且添加一个 @ 字符后缀以及一个十进制数表示所有参数的字节大小。返回值
// 会通过寄存器 EAX 或者 EDX:EAX 返回，可以传递 8 个字节，更大的结构体数据通过指针返回，
// 该指针在被调函数返回前保存在 EAX 寄存器中。另外非 POD 的结构体数据也通过指针返回。前
// 两个参数通过 ECX EDX 传递时，不需要额外为被调函数分配影子空间，如果被调函数需要保存
// 寄存器参数的副本，它可以以任何自己的方式在函数内部处理。因此被调函数返回前需要清理的
// 栈参数的大小，需要减掉通过寄存器传递的参数大小。如果是可变参数函数，所有参数都通过栈
// 传递，回退去使用 cdecl 约定。
//
// 寄存器                   类型            用法
// EAX                     易变            返回值
// ECX                     易变            第一个整型参数
// EDX                     易变            第二个整型参数
// EDI                     非易变          被调函数必须负责保护
// ESI                     非易变          被调函数必须负责保护
// EBX                     非易变          被调函数必须负责保护
// EBP                     非易变          被调函数必须负责保护，可以用于栈帧指针
// ESP                     非易变          栈指针
// 当函数退出、函数进入 C 运行时库或 Windows 系统，标志寄存器中的方向标志必须先清位。
//
// 返回值通过寄存器 EAX EDX 返回，更大的返回值通过内存地址返回，返回值内存地址当作函数
// 第一个栈参数传递，函数真正的参数还依然依次使用 ECX EDX 传递。
#include "coro.h"

__magic_func_decl(fastcall, Coro*) asm_call_coro_finish(Coro *co);
__magic_func_decl(fastcall, void) asm_call_stack_crash(uptr co_rsp, uptr rsp);

__magic_nakedcall(fastcall, uptr) asm_coro_init(uptr rsp)
{
__magic_asm_begin()
    // 调用该函数之前协程地址已经保存
    // pstack + alloc <-- 00                   <-- 16字节对齐
    //             -4 <-- 12 对齐填补
    //             -8 <-- 08 对齐填补
    //            -12 <-- 04 co
    //            -16 <-- 00 asm_coro_call     <-- 16字节对齐
    //            -20 <-- 12           ecx, 12
    mov DWORD PTR [ecx - 4 * 1], 0  // edi, 08
    mov DWORD PTR [ecx - 4 * 2], 0  // esi, 04
    mov DWORD PTR [ecx - 4 * 3], 0  // ebx, 00 <-- 16字节对齐
    mov DWORD PTR [ecx - 4 * 4], 0  // ebp, 12
    sub ecx, 4 * 4
    mov DWORD PTR [ecx - 4], ecx    // esp, 08
    sub ecx, 4
    mov eax, ecx
    ret // 返回当前 esp 的值
__magic_asm_end()
}

__magic_nakedcall(fastcall, void) asm_coro_resume(uptr coro_esp)
{
    // [in]  ecx 协程的栈指针
__magic_asm_begin()
    mov esp, ecx
    pop ecx // 弹出协程栈中保存的 esp
    cmp ecx, esp
    jne esp_crash
    pop ebp
    pop ebx
    pop esi
    pop edi
    pop ecx         // 协程地址
    mov eax, 1      // yield 函数的返回值
    ret             // 恢复协程运行
esp_crash:
    mov edx, esp    // ecx 已经保存弹出的栈指针
    //            esp <-- xx
    // coro stack ebp <-- 12 esp
    //         esp-04 <-- 08
    //         esp-08 <-- 04
    //         esp-12 <-- 00 输入参数对齐
    // return address <-- 12 rsp
    // asm_call_stack_crash - 08
    sub esp, 12     // align esp to 16 bytes
    call asm_call_stack_crash
__magic_asm_end()
}

__magic_nakedcall(fastcall, void) asm_coro_yield(Coro *co, Coro *wait)
{
    // [in]  ecx 当前协程
    // [in]  edx 需要处理的协程
__magic_asm_begin()
    cmp DWORD PTR [edx], 0
    jne save_context
    mov eax, 0  // yield 函数的返回值
    ret // 需要处理的协程已经处理完毕
save_context:
    // 保护当前协程，栈中已保存返回地址
    push ecx // 当前协程地址
    push edi
    push esi
    push ebx
    push ebp
    push esp
    mov [ecx], esp
    // 切换到需要处理的协程
    mov ecx, [edx]
    jmp asm_coro_resume
__magic_asm_end()
}

__magic_nakedcall(fastcall, void) asm_coro_yield_manual(Coro *co, Coro *wait)
{
    // [in]  ecx 当前协程
    // [in]  edx 需要处理的协程
__magic_asm_begin()
    mov [edx + 4], ecx   // 当协程处理完毕后恢复当前协程
    jmp asm_coro_yield
__magic_asm_end()
}

__magic_nakedcall(fastcall, void) asm_coro_return(void)
{
__magic_asm_begin()
    pop ecx
    sub esp, (20 + 4)   // align esp to 16 bytes
    call asm_call_coro_finish
    add esp, (20 + 4)
    mov ecx, [eax]
    jmp asm_coro_resume
__magic_asm_end()
}

__magic_nakedcall(fastcall, void) asm_coro_call(Coro *co)
{
    // pstack + alloc <-- 00 地址对齐
    //             -4 <-- 12 对齐填补
    //             -8 <-- 08 对齐填补
    //             co <-- 04 <-- esp
    //  asm_coro_call <-- 00
    //         esp-08 <-- 12
    //         esp-12 <-- 08
    //         esp-16 <-- 04
    //         esp-20 <-- 00 <-- sub esp,20 输入参数对齐
    // return address <-- 12 <-- esp
    //       co->func <-- 08
__magic_asm_begin()
    sub esp, 20                     // align esp to 16 bytes
    call DWORD PTR [ecx + 4 * 2]    // call co->func
    add esp, 20
    jmp asm_coro_return
__magic_asm_end()
}
