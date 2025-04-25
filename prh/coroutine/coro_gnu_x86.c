// GNU X86 32位平台 fastcall 调用约定：被调函数清理栈参数，前两个整型参数通过寄存器
// ECX EDX 传递，超过四字节的和额外的整型参数从右到左通过栈传递。由于是被调函数自动清理
// 栈参数，fastcall 调用约定的函数必须提供函数原型，否则编译器不能确定参数的总长度。返
// 回值通过寄存器 EAX 或者 EDX:EAX 返回，更大的数据通过内存地址返回，该地址在被调函数返
// 回前要保存到 EAX 寄存器中。前两个参数通过 ECX EDX 传递时，不需要额外为被调函数分配
// 影子空间，如果被调函数需要保存寄存器参数的副本，它可以以任何自己的方式在函数内部处理。
// 因此被调函数返回前需要清理的栈参数的大小，需要减掉通过寄存器传递的参数大小。如果是可
// 变参数函数，所有参数都通过栈传递，回退去使用 cdecl 约定。 GNU X86 ABI 要求栈至少对
// 齐到16字节边界。
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
//
// 返回值通过寄存器 EAX EDX 返回，更大的返回值通过内存地址返回，返回值内存地址当作函数
// 的第一个参数传递，即 ECX，函数真正的函数依次后延。
#define PRH_CORO_INCLUDE
#include "prh_include.h"

prh_naked_fastcall(int) prh_impl_asm_stack_init_depth(void)
{
prh_asm_begin()
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
    "movl $(4*9),%eax\n\t"
    "ret\n\t"
prh_asm_end()
}

prh_naked_fastcall(void) prh_impl_asm_coro_resume(void)
{
prh_asm_begin()
    "fldcww (%esp)\n\t"     // fldcw m16, load m16 to fcw
    "addl $4,%esp\n\t"      // fstcw-16
    "popl %ebp\n\t"
    "popl %ebx\n\t"
    "popl %esi\n\t"
    "popl %edi\n\t"
    "movl $1,%eax\n\t"      // yield 函数的返回值
    "ret\n\t"               // 恢复协程运行
prh_asm_end()
}

prh_naked_fastcall(void) prh_impl_asm_coro_next(prh_coro_t *coro)
{
    // [in]  ecx 目标协程
prh_asm_begin()
    "movl %ecx,%esp\n\t"    // esp = coro
    "movl (%ecx),%eax\n\t"  // eax = rspoffset
    "subl %eax,%esp\n\t"    // esp = coro - rspoffset
    "popl %eax\n\t"         // 切换协程栈完成，弹出栈顶元素
    "jmp *%eax\n\t"         // 跳转到 asm_coro_call 或者 asm_coro_resume
prh_asm_end()
}

prh_naked_fastcall(bool) prh_impl_asm_coro_yield(prh_coro_t *coro, prh_coro_t *next)
{
    // [in]  ecx 当前协程
    // [in]  edx 目标协程
prh_asm_begin()
    // 保护当前协程，栈中已保存返回地址
    "pushl %edi\n\t"
    "pushl %esi\n\t"
    "pushl %ebx\n\t"
    "pushl %ebp\n\t"
    "subl $4,%esp\n\t"      // fstcw-16
    "fnstcww (%esp)\n\t"    // fstcw/fnstcw m16, save fcw to m16
    "pushl $(prh_impl_asm_coro_resume)\n\t"
    "movl %ecx,%eax\n\t"    // eax = coro
    "subl %esp,%eax\n\t"    // rspoffset = coro - esp
    "movl %eax,(%ecx)\n\t"
    // 切换到需要处理的目标协程
    "movl %edx,%ecx\n\t"
    "jmp prh_impl_asm_coro_next\n\t"
prh_asm_end()
}

prh_fastcall(void *) prh_impl_asm_coro_finish(prh_coro_t *coro);

prh_naked_fastcall(void) prh_impl_asm_coro_call(void) // args are on coro stack
{
    // 第一次进入协程函数，初始栈布局如下:
    //   stack bottom <-- 00 <-- 16字节对齐 <-- coro <-- esp 输入参数对齐
    //           proc <-- 12
    //                <-- 08 asm_coro_call
prh_asm_begin()
    "popl %edx\n\t"             // 协程函数 proc
    "movl %esp,%ecx\n\t"        // 协程函数的参数 coro
    "call *%edx\n\t"            // 执行协程函数 proc(coro)
    "movl %esp,%ecx\n\t"        // 协程函数执行完毕
    "call prh_impl_asm_coro_finish\n\t" // prh_impl_asm_coro_finish(coro)
    "movl %eax,%ecx\n\t"        // 执行下一个协程
    "jmp prh_impl_asm_coro_next\n\t"
prh_asm_end()
}
