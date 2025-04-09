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
#define PRH_STRIP_PREFIX
#include "coroutine.h"

magic_naked_fastcall(intp) asm_stack_init_depth(void)
{
magic_asm_begin()
    // 进入协程函数内部第一次yield，其协程栈的最小深度
    //   stack bottom <-- 00 <-- coro <-- esp
    // return address <-- 12 调用协程函数
    //      coro proc <-- 08
    // return address <-- 04 调用 prh_impl_asm_coro_yield
    //                    00 edi
    //                    12 esi
    //                    08 ebx
    //                    04 ebp
    //                    00 fstcw-16
    //                    12 asm_coro_resume
    mov eax, 4 * 9
    ret
magic_asm_end()
}

magic_naked_fastcall(void) asm_coro_resume(void)
{
magic_asm_begin()
    fldcw WORD PTR [esp]    // fldcw m16, load m16 to fcw
    add esp, 4              // fstcw-16
    pop ebp
    pop ebx
    pop esi
    pop edi
    mov eax, 1      // yield 函数的返回值
    ret             // 恢复协程运行
magic_asm_end()
}

magic_naked_fastcall(void) asm_coro_next(struct coro *coro)
{
    // [in]  ecx 目标协程
magic_asm_begin()
    mov esp, ecx    // esp = coro
    mov eax, [ecx]  // eax = rspoffset
    sub esp, eax    // esp = coro - rspoffset
    pop eax         // 切换协程栈完成，弹出栈顶元素
    jmp eax         // 跳转到 asm_coro_call 或者 asm_coro_resume
magic_asm_end()
}

magic_naked_fastcall(bool) prh_impl_asm_coro_yield(struct coro *coro, struct coro *next)
{
    // [in]  ecx 当前协程
    // [in]  edx 目标协程
magic_asm_begin()
    // 保护当前协程，栈中已保存返回地址
    push edi
    push esi
    push ebx
    push ebp
    sub esp, 4              // fstcw-16
    fnstcw WORD PTR [esp]   // fstcw/fnstcw m16, save fcw to m16
    push OFFSET asm_coro_resume
    mov eax, ecx            // eax = coro
    sub eax, esp            // rspoffset = coro - esp
    mov [ecx], eax
    // 切换到需要处理的目标协程
    mov ecx, edx
    jmp asm_coro_next
magic_asm_end()
}

magic_fastcall(void *) asm_coro_finish(struct coro *coro);

magic_naked_fastcall(void) asm_coro_call(void) // args on coro stack
{
    // 第一次进入协程函数，初始栈布局如下:
    //   stack bottom <-- 00 <-- 16字节对齐 <-- coro <-- esp 输入参数对齐
    //           proc <-- 12
    //                <-- 08 asm_coro_call
magic_asm_begin()
    pop edx                 // 协程函数 proc
    mov ecx, esp            // 协程函数的参数 coro
    call edx                // 执行协程函数 proc(coro)
    mov ecx, esp            // 协程函数执行完毕
    call asm_coro_finish    // 调用 asm_coro_finish(coro)
    mov ecx, eax            // 执行下一个协程
    jmp asm_coro_next
magic_asm_end()
}
