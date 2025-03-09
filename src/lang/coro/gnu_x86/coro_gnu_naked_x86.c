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
#include "coro.h"

magic_func_decl(fastcall, struct coro*) asm_call_coro_finish(struct coro *co);
magic_func_decl(fastcall, void) asm_call_stack_crash(void *co_esp, void *esp);

magic_nakedcall(fastcall, void*) asm_curr_rsp(void)
{
magic_asm_begin()
    "movl %esp,%eax\n\t"
    "ret\n\t"
magic_asm_end()
}

magic_nakedcall(fastcall, void*) asm_coro_init(void *esp)
{
magic_asm_begin()
    // 调用该函数之前协程地址已经保存
    // pstack + alloc <-- 00                   <-- 16字节对齐
    //             -4 <-- 12 对齐填补
    //             -8 <-- 08 对齐填补
    //            -12 <-- 04 co
    //            -16 <-- 00 asm_coro_call     <-- 16字节对齐
    //            -20 <-- 12           ecx, 12
    "movl $0,-1*4(%ecx)\n\t"        // edi, 08
    "movl $0,-2*4(%ecx)\n\t"        // esi, 04
    "movl $0,-3*4(%ecx)\n\t"        // ebx, 00 <-- 16字节对齐
    "movl $0,-4*4(%ecx)\n\t"        // ebp, 12
    "subl $(4*4),%ecx\n\t"
    "movl %ecx,-4(%ecx)\n\t"        // esp, 08
    "subl $4,%ecx\n\t"
    "movl %ecx,%eax\n\t"
    "ret\n\t" // 返回当前 esp 的值
magic_asm_end()
}

magic_nakedcall(fastcall, void) asm_coro_resume(void)
{
magic_asm_begin()
    "popl %ecx\n\t" // 弹出协程栈中保存的 esp
    "cmpl %esp,%ecx\n\t"
    "jne esp_crash\n\t"
    "popl %ebp\n\t"
    "popl %ebx\n\t"
    "popl %esi\n\t"
    "popl %edi\n\t"
    "popl %ecx\n\t"         // 协程地址
    "movl $1,%eax\n\t"      // yield 函数的返回值
    "ret\n\t"               // 恢复协程运行
"esp_crash:\n\t"
    "movl %esp,%edx\n\t"    // ecx 已经保存弹出的栈指针
    //            esp <-- xx
    // coro stack ebp <-- 12 esp
    //         esp-04 <-- 08
    //         esp-08 <-- 04
    //         esp-12 <-- 00 输入参数对齐
    // return address <-- 12 rsp
    // asm_call_stack_crash - 08
    "subl $12,%esp\n\t"     // align esp to 16 bytes
    "call asm_call_stack_crash\n\t"
magic_asm_end()
}

magic_nakedcall(fastcall, void) asm_coro_yield(struct coro *co, struct coro *next)
{
    // [in]  ecx 当前协程
    // [in]  edx 需要处理的协程
magic_asm_begin()
    "cmpl $0,(%edx)\n\t"
    "jne save_context\n\t"
    "movl $0,%eax\n\t"  // yield 函数的返回值
    "ret\n\t" // 需要处理的协程已经处理完毕
"save_context:\n\t"
    // 保护当前协程，栈中已保存返回地址
    "pushl %ecx\n\t" // 当前协程地址
    "pushl %edi\n\t"
    "pushl %esi\n\t"
    "pushl %ebx\n\t"
    "pushl %ebp\n\t"
    "pushl %esp\n\t"
    "movl %esp,(%ecx)\n\t"
    // 切换到需要处理的协程
    "movl (%edx),%esp\n\t"
    "jmp asm_coro_resume\n\t"
magic_asm_end()
}

magic_nakedcall(fastcall, void) asm_coro_return(void)
{
magic_asm_begin()
    "popl %ecx\n\t"
    "subl $(20 + 4),%esp\n\t"   // align esp to 16 bytes
    "call asm_call_coro_finish\n\t"
    "addl $(20 + 4),%esp\n\t"
    "movl (%eax),%esp\n\t"
    "jmp asm_coro_resume\n\t"
magic_asm_end()
}

magic_nakedcall(fastcall, void) asm_coro_call(struct coro *co)
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
    //     proc(coro) <-- 08
magic_asm_begin()
    "movl %ecx,%eax\n\t"    // mov co to eax
    "xchgl %eax,(%esp)\n\t" // push co for asm_coro_return && coro proc -> eax
    "subl $20,%esp\n\t"     // align esp to 16 bytes
    "call *%eax\n\t"        // call proc(coro)，绝对地址调用
    "addl $20,%esp\n\t"
    "jmp asm_coro_return\n\t"
magic_asm_end()
}
