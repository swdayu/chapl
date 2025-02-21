# GNU X64 ABI 使用 RDI RSI RDX RCX R8 R9 和 XMM0 ~ XMM7 可传递 6 个整型 8 个向量类
# 型的寄存器参数，调用者不需要像 Windows 64位程序那样需要为被调函数预留栈影子空间，但有
# 相同的栈对齐要求，根据栈参数中是否有向量类型参数，栈需要至少对齐到 16 字节、或 32 字
# 节、或 64 字节地址边界。同样，栈参数由调用者负责清理。
#
# 寄存器                   类型            用法
# RAX                     易变            返回值寄存器，变参时传递变参中向量参数的个数
# RCX                     易变            第四个整型参数
# RDX                     易变            第三个整型参数，返回值寄存器
# R8                      易变            第五个整型参数
# R9                      易变            第六个整型参数
# R10:R11                 易变            调用者必须负责保护，用于 syscall/sysret 指令
# R12:R15                 非易变          被调函数必须负责保护
# RDI                     易变            第一个整型参数，或当作第一个参数的返回值内存地址
# RSI                     易变            第二个整型参数
# RBX                     非易变          被调函数必须负责保护
# RBP                     非易变          被调函数必须负责保护，可以用于栈帧指针
# RSP                     非易变          栈指针
# XMM0 YMM0               易变            第一个向量类型参数，第一个向量类型返回值寄存器
# XMM1 YMM1               易变            第二个向量类型参数，第二个向量类型返回值寄存器
# XMM2 YMM2               易变            第三个向量类型参数
# XMM3 YMM3               易变            第四个向量类型参数
# XMM4 YMM4               易变            第五个向量类型参数
# XMM5 YMM5               易变            第六个向量类型参数
# XMM6 YMM6               易变            第七个向量类型参数
# XMM7 YMM7               易变            第八个向量类型参数
# XMM8:XMM15              易变            GNU X64 ABI 中向量寄存器都是易变的
#
# CPU 在进入函数之前必须是 x87 模式。因此每个用了 MMX 寄存器的函数，必须在使用完 MMX
# 寄存器之后，并在函数返回或调用另一个函数之前必须回到x87模式。所有x87寄存器都不能跨函
# 数，所有的 xmmN ymmN zmmN 都不能跨函数，但微软编译器要求XMM6~XMM15这10个16字节长的
# 向量寄存器可以跨越函数。浮点控制寄存器%fcw由被调函数保护，被调函数如果修改了它的值在
# 返回之前必须进行恢复。媒体控制和状态寄存器mxcsr的控制位，也由被调函数保护可以跨函数。
# 线程指针（thread pointer）%fs寄存器也需要保护。

.section .text

# [in]  rcx -> rdi 协程的栈指针
.global asm_coro_init
asm_coro_init:
    # 调用该函数之前，协程地址已经保存
    # pstack + alloc <-- 00                     <-- 16字节对齐
    #             -8 <-- 08 co
    #            -16 <-- 00 asm_coro_call       <-- 16字节对齐
    #            -24 <-- 08    rcx -> 08 rdi
    movq $0,-1*8(%rdi)              # 00 rbx    <-- 16字节对齐
    movq $0,-2*8(%rdi)              # 08 rbp
    movq $0,-3*8(%rdi)              # 00 r12
    movq $0,-4*8(%rdi)              # 08 r13
    movq $0,-5*8(%rdi)              # 00 r14
    movq $0,-6*8(%rdi)              # 08 r15
    movq $0,-7*8(%rdi)              # 00 fstcw-16:stmxcsr-32
    subq $(8*7),%rdi
    movq %rdi,-8(%rdi)              # 08 rsp
    subq $8,%rdi
    movq %rdi,%rax
    ret # 返回当前 rsp 的值

# as treats all undefined symbols as external
# .extern asm_call_stack_crash
# [in]  rcx -> rdi 协程的栈指针
asm_coro_resume:
    movq %rdi,%rsp
    popq %rdi       # 弹出协程栈中保存的 rsp
    cmpq %rsp,%rdi
    jne rsp_crash
    ldmxcsr (%rsp)  # ldmxcsr m32, load m32 to mxcsr
    fldcww 4(%rsp)  # fldcw m16, load m16 to fcw
    addq $8,%rsp    # fstcw-16:stmxcsr-32
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    popq %rdi       # 协程地址
    movq $1,%rax    # yield 函数的返回值
    ret             # 恢复协程运行
rsp_crash:          # rcx -> rdi 已经保存弹出的栈指针
    movq %rsp,%rsi  # rdx -> rsi
    #            rsp <-- 08
    # coro stack fcw <-- 00 rsp
    #         rsp-08 <-- 08
    #         rsp-16 <-- 00
    #         rsp-24 <-- 08
    #         rsp-32 <-- 00 输入参数对齐
    # return address <-- 08 rsp
    # asm_call_stack_crash - 00
    subq $32,%rsp    # align rsp to 16 bytes
    call asm_call_stack_crash

# [in]  rcx -> rdi 当前协程
# [in]  rdx -> rsi 需要处理的协程
.global asm_coro_yield
asm_coro_yield:
    cmpq $0,(%rsi)
    jne save_context
    movq $0,%rax # yield 函数的返回值
    ret # 需要处理的协程已经处理完毕
save_context:
    # 保护当前协程，栈中已保存返回地址
    pushq %rdi  # 当前协程地址
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    subq $8,%rsp    # fstcw-16:stmxcsr-32
    stmxcsr (%rsp)  # stmxcsr m32, save mxcsr to m32
    fnstcww 4(%rsp) # fstcw/fnstcw m16, save fcw to m16
    pushq %rsp
    movq %rsp,(%rdi)
    # 切换到需要处理的协程
    movq (%rsi),%rdi
    jmp asm_coro_resume

# [in]  rcx -> rdi 当前协程
# [in]  rdx -> rsi 需要处理的协程
.global asm_coro_yield_manual
asm_coro_yield_manual:
    movq %rdi,8(%rsi)  # 当协程处理完毕后恢复当前协程
    jmp asm_coro_yield

# as treats all undefined symbols as external
# .extern asm_call_coro_finish
asm_coro_return:
    popq %rdi               # rcx -> rdi
    subq $(40 + 8),%rsp     # align rsp to 16 bytes
    call asm_call_coro_finish
    addq $(40 + 8),%rsp
    movq (%rax),%rdi        # rcx -> rdi
    jmp asm_coro_resume

# pstack + alloc <-- 00 <-- 16 字节对齐
#             co <-- 08 <-- rsp
#  asm_coro_call <-- 00
#         rsp-16 <-- 08
#         rsp-24 <-- 00
#         rsp-32 <-- 08
#         rsp-40 <-- 00 <-- sub rsp,40 输入参数对齐
# return address <-- 08 <-- rsp
#       co->func <-- 00
.global asm_coro_call
asm_coro_call:
    subq $40,%rsp       # align rsp to 16 bytes
    call *2*8(%rdi)     # rcx -> rdi, call co->func
    addq $40,%rsp
    jmp asm_coro_return
