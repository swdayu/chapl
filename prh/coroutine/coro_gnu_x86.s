# GNU X86 32位平台 fastcall 调用约定：被调函数清理栈参数，前两个整型参数通过寄存器
# ECX EDX 传递，超过四字节的和额外的整型参数从右到左通过栈传递。由于是被调函数自动清理
# 栈参数，fastcall 调用约定的函数必须提供函数原型，否则编译器不能确定参数的总长度。返
# 回值通过寄存器 EAX 或者 EDX:EAX 返回，更大的数据通过内存地址返回，该地址在被调函数返
# 回前要保存到 EAX 寄存器中。前两个参数通过 ECX EDX 传递时，不需要额外为被调函数分配
# 影子空间，如果被调函数需要保存寄存器参数的副本，它可以以任何自己的方式在函数内部处理。
# 因此被调函数返回前需要清理的栈参数的大小，需要减掉通过寄存器传递的参数大小。如果是可
# 变参数函数，所有参数都通过栈传递，回退去使用 cdecl 约定。 GNU X86 ABI 要求栈至少对
# 齐到16字节边界。
#
# 寄存器                   类型            用法
# EAX                     易变            返回值
# ECX                     易变            第一个整型参数
# EDX                     易变            第二个整型参数
# EDI                     非易变          被调函数必须负责保护
# ESI                     非易变          被调函数必须负责保护
# EBX                     非易变          被调函数必须负责保护
# EBP                     非易变          被调函数必须负责保护，可以用于栈帧指针
# ESP                     非易变          栈指针
#
# 返回值通过寄存器 EAX EDX 返回，更大的返回值通过内存地址返回，返回值内存地址当作函数
# 的第一个参数传递，即 ECX，函数真正的函数依次后延。在 Linux GCC（32 位）上，不会对
# C 语言函数名称进行装饰，例如声明为 __attribute__((__fastcall__)) 不会像 Windows
# 那样给函数名加前缀 "@" 或后缀 "@字节数"；符号无装饰保持原样，你可以直接用原始名字进
# 行调用或链接。

.section .text

.global prh_impl_asm_stack_init_depth
prh_impl_asm_stack_init_depth:
    # 进入协程函数内部第一次yield，其协程栈的最小深度
    #   stack bottom <-- 00 <-- coro <-- esp
    # return address <-- 12 调用协程函数
    #      coro proc <-- 08
    # return address <-- 04 调用 prh_impl_asm_coro_yield
    #                    00 edi
    #                    12 esi
    #                    08 ebx
    #                    04 ebp
    #                    00 fstcw-16
    #                    12 asm_coro_resume
    movl $(4*9),%eax
    ret

prh_impl_asm_coro_resume:
    fldcww (%esp)     # fldcw m16, load m16 to fcw
    addl $4,%esp      # fstcw-16
    popl %ebp
    popl %ebx
    popl %esi
    popl %edi
    ret               # 恢复协程运行

# [in]  ecx 目标协程
prh_impl_asm_coro_next:
    movl %ecx,%esp    # esp = coro
    movl (%ecx),%eax  # eax = rspoffset
    subl %eax,%esp    # esp = coro - rspoffset
    popl %eax         # 切换协程栈完成，弹出栈顶元素
    jmp *%eax         # 跳转到 asm_coro_call 或者 asm_coro_resume

# [in]  ecx 当前协程
# [in]  edx 目标协程
.global prh_impl_asm_coro_yield
prh_impl_asm_coro_yield:
    # 保护当前协程，栈中已保存返回地址
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %ebp
    subl $4,%esp      # fstcw-16
    fnstcww (%esp)    # fstcw/fnstcw m16, save fcw to m16
    pushl $(prh_impl_asm_coro_resume)
    movl %ecx,%eax    # eax = coro
    subl %esp,%eax    # rspoffset = coro - esp
    movl %eax,(%ecx)
    # 切换到需要处理的目标协程
    movl %edx,%ecx
    jmp prh_impl_asm_coro_next

.global prh_impl_asm_coro_call
prh_impl_asm_coro_call:
    # 第一次进入协程函数，初始栈布局如下:
    #   stack bottom <-- 00 <-- 16字节对齐 <-- coro <-- esp 输入参数对齐
    #           proc <-- 12
    #                <-- 08 prh_impl_asm_coro_call
    popl %edx             # 协程函数 proc
    movl %esp,%ecx        # 协程函数的参数 coro
    call *%edx            # 执行协程函数 proc(coro)
    movl %esp,%ecx        # 协程函数执行完毕
    call prh_impl_asm_coro_finish # prh_impl_asm_coro_finish(coro)
    movl %eax,%ecx        # 执行下一个协程
    jmp prh_impl_asm_coro_next

.global prh_impl_asm_soro_call
prh_impl_asm_soro_call:
    # 第一次进入协程函数，初始栈布局如下:
    #   stack bottom <-- 00 <-- 16字节对齐 <-- coro <-- esp 输入参数对齐
    #           proc <-- 12
    #                <-- 08 prh_impl_asm_soro_call
    popl %edx             # 协程函数 proc
    movl %esp,%ecx        # 协程函数的参数 coro
    call *%edx            # 执行协程函数 proc(coro)
    movl %esp,%ecx        # 协程函数执行完毕
    call prh_impl_asm_soro_finish # prh_impl_asm_soro_finish(coro)
    movl %eax,%ecx        # 执行下一个协程
    jmp prh_impl_asm_coro_next

.global prh_impl_asm_cono_call
prh_impl_asm_cono_call:
    # 第一次进入协程函数，初始栈布局如下:
    #   stack bottom <-- 00 <-- 16字节对齐 <-- coro <-- esp 输入参数对齐
    #           proc <-- 12
    #                <-- 08 prh_impl_asm_cono_call
    popl %edx             # 协程函数 proc
    movl %esp,%ecx        # 协程函数的参数 coro
    call *%edx            # 执行协程函数 proc(coro)
    movl %esp,%ecx        # 协程函数执行完毕
    call prh_impl_asm_cono_finish # prh_impl_asm_cono_finish(coro)
    movl %eax,%ecx        # 执行下一个协程
    jmp prh_impl_asm_coro_next
