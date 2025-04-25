; Windows 64 位程序前四个整型参数通过 RCX RDX R8 R9 传递，超过八字节的和额外的整型参数
; 通过栈传递。函数调用前必须至少留出32字节的栈空间，让系统函数保存 RCX RDX R8 R9 临时副
; 本，进入被调函数时 RSP 应该对齐到16字节地址边界，但 Win64 API 也不强制要求这个规则。
; 只使用 RAX 作为整型返回值寄存器。调用者负责清除所有参数和 shadow space 栈空间。
;
; 寄存器                   类型            用法
; RAX                     易变            返回值寄存器
; RCX                     易变            第一个整型参数
; RDX                     易变            第二个整型参数
; R8                      易变            第三个整型参数
; R9                      易变            第四个整型参数
; R10:R11                 易变            调用者必须负责保护，用于 syscall/sysret 指令
; R12:R15                 非易变          被调函数必须负责保护
; RDI                     非易变          被调函数必须负责保护
; RSI                     非易变          被调函数必须负责保护
; RBX                     非易变          被调函数必须负责保护
; RBP                     非易变          被调函数必须负责保护，可以用于栈帧指针
; RSP                     非易变          栈指针
; XMM0 YMM0               易变            第一个FP参数，第一个向量类型参数
; XMM1 YMM1               易变            第二个FP参数，第二个向量类型参数
; XMM2 YMM2               易变            第三个FP参数，第三个向量类型参数
; XMM3 YMM3               易变            第四个FP参数，第四个向量类型参数
; XMM4 YMM4               易变            调用者必须负责保护，第五个向量类型参数
; XMM5 YMM5               易变            调用者必须负责保护，第六个向量类型参数
; XMM6:XMM15              非易变          被调函数必须负责保护
; YMM6:YMM15              高16字节易变    调用者根据需要必须负责保护YMM
;
; 当函数退出、函数进入 C 运行时库或 Windows 系统，标志寄存器中的方向标志必须先清位。
; CPU 在进入函数之前必须是 x87 模式。因此每个用了 MMX 寄存器的函数，必须在使用完 MMX
; 寄存器之后，并在函数返回或调用另一个函数之前必须回到x87模式。所有x87寄存器都不能跨函
; 数，所有的 xmmN ymmN zmmN 都不能跨函数，但微软编译器要求XMM6~XMM15这10个16字节长的
; 向量寄存器可以跨越函数。浮点控制寄存器%fcw由被调函数保护，被调函数如果修改了它的值在
; 返回之前必须进行恢复。媒体控制和状态寄存器mxcsr的控制位，也由被调函数保护可以跨函数。
; 线程指针（thread pointer）%fs寄存器也需要保护。

.code

prh_impl_asm_stack_init_depth PROC
    ; 进入协程函数内部第一次yield，其协程栈的最小深度
    ;   stack bottom <-- 00 <-- coro <-- rsp
    ;         rsp-08 <-- 08 调用协程函数
    ;         rsp-16 <-- 00
    ;         rsp-24 <-- 08
    ;         rsp-32 <-- 00 输入参数对齐
    ; return address <-- 08
    ;      coro proc <-- 00
    ; return address <-- 08 调用 prh_impl_asm_coro_yield
    ;                    00 rdi
    ;                    08 rsi
    ;                    00 rbx
    ;                    08 rbp
    ;                    00 r12
    ;                    08 r13
    ;                    00 r14
    ;                    08 r15
    ;                    00 fstcw-16:stmxcsr-32
    ;                    08 asm_coro_resume
    mov rax, 8 * 17
    ret
prh_impl_asm_stack_init_depth ENDP

prh_impl_asm_coro_resume PROC PRIVATE
    ldmxcsr DWORD PTR [rsp] ; ldmxcsr m32, load m32 to mxcsr
    fldcw WORD PTR [rsp+4]  ; fldcw m16, load m16 to fcw
    add rsp, 8              ; fstcw-16:stmxcsr-32
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    pop rsi
    pop rdi
    mov rax, 1      ; yield 函数的返回值
    pop rcx         ; 函数返回地址
    sub rsp, 32     ; 恢复影子空间
    jmp rcx         ; 恢复协程运行
prh_impl_asm_coro_resume ENDP

; [in]  rcx 目标协程
prh_impl_asm_coro_next PROC PRIVATE
    mov rsp, rcx                ; rsp = coro
    movsxd rax, DWORD PTR [rcx] ; rax = rspoffset
    sub rsp, rax                ; rsp = coro - rspoffset
    pop rax         ; 切换协程栈完成，弹出栈顶元素
    jmp rax         ; 跳转到 asm_coro_call 或者 asm_coro_resume
prh_impl_asm_coro_next ENDP

; [in]  rcx 当前协程
; [in]  rdx 目标协程
prh_impl_asm_coro_yield PROC
    pop rax         ; 函数返回地址
    add rsp, 32     ; 无需影子空间
    ; 保护当前协程
    push rax
    push rdi
    push rsi
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8              ; fstcw-16:stmxcsr-32
    stmxcsr DWORD PTR [rsp] ; stmxcsr m32, save mxcsr to m32
    fnstcw WORD PTR [rsp+4] ; fstcw/fnstcw m16, save fcw to m16
    mov rax,OFFSET prh_impl_asm_coro_resume
    push rax
    mov rax, rcx            ; rax = coro
    sub rax, rsp            ; rspoffset = coro - rsp
    mov DWORD PTR [rcx], eax
    ; 切换到需要处理的目标协程
    mov rcx, rdx
    jmp prh_impl_asm_coro_next
prh_impl_asm_coro_yield ENDP

prh_impl_asm_coro_finish PROTO
prh_impl_asm_coro_call PROC
    ; 第一次进入协程函数，初始栈布局如下:
    ;   stack bottom <-- 00 <-- 16字节对齐 <-- coro <-- rsp
    ;         rsp-08 <-- 08 proc
    ;         rsp-16 <-- 00 prh_impl_asm_coro_call
    ;         rsp-24 <-- 08
    ;         rsp-32 <-- 00 ; sub rsp,32 输入参数对齐
    pop rdx                 ; 协程函数 proc
    mov rcx, rsp            ; 协程函数的参数 coro
    sub rsp, 32             ; 影子空间并且16字节对齐
    call rdx                ; 执行协程函数 proc(coro)
    mov rcx, rsp            ; 协程函数执行完毕
    add rcx, 32             ; rcx 保存 coro
    call prh_impl_asm_coro_finish ; prh_impl_asm_coro_finish(coro)
    mov rcx, rax            ; 执行下一个协程
    jmp prh_impl_asm_coro_next
prh_impl_asm_coro_call ENDP

END
