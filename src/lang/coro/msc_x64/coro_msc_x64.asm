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
asm_curr_rsp PROC
    mov rax, rsp
    ret
asm_curr_rsp ENDP

asm_coro_init PROC
    ; 调用该函数之前，协程地址已经保存
    ; pstack + alloc <-- 00                     <-- 16字节对齐
    ;             -8 <-- 08 co
    ;            -16 <-- 00 asm_coro_call       <-- 16字节对齐
    ;            -24 <-- 08           08 rcx
    mov QWORD PTR [rcx - 8 * 1], 0  ; 00 rdi    <-- 16字节对齐
    mov QWORD PTR [rcx - 8 * 2], 0  ; 08 rsi
    mov QWORD PTR [rcx - 8 * 3], 0  ; 00 rbx
    mov QWORD PTR [rcx - 8 * 4], 0  ; 08 rbp
    mov QWORD PTR [rcx - 8 * 5], 0  ; 00 r12
    mov QWORD PTR [rcx - 8 * 6], 0  ; 08 r13
    mov QWORD PTR [rcx - 8 * 7], 0  ; 00 r14
    mov QWORD PTR [rcx - 8 * 8], 0  ; 08 r15
    mov QWORD PTR [rcx - 8 * 9], 0  ; 00 fstcw-16:stmxcsr-32
    sub rcx, 8 * 9
    mov QWORD PTR [rcx - 8], rcx    ; 08 rsp
    sub rcx, 8
    mov rax, rcx
    ret ; 返回当前 rsp 的值
asm_coro_init ENDP

asm_call_stack_crash PROTO
asm_coro_resume PROC PRIVATE
    pop rcx ; 弹出协程栈中保存的 rsp
    cmp rcx, rsp
    jne rsp_crash
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
    pop rcx         ; 协程地址
    mov rax, 1      ; yield 函数的返回值
    ret             ; 恢复协程运行
rsp_crash:
    mov rdx, rsp    ; rcx 已经保存弹出的栈指针
    ;            rsp <-- 08
    ; coro stack fcw <-- 00 rsp
    ;         rsp-08 <-- 08
    ;         rsp-16 <-- 00
    ;         rsp-24 <-- 08
    ;         rsp-32 <-- 00 输入参数对齐
    ; return address <-- 08 rsp
    ; asm_call_stack_crash - 00
    sub rsp, 32     ; align rsp to 16 bytes
    call asm_call_stack_crash
asm_coro_resume ENDP

; [in]  rcx 当前协程
; [in]  rdx 需要处理的协程
asm_coro_yield PROC
    cmp QWORD PTR [rdx], 0
    jne save_context
    mov rax, 0  ; yield 函数的返回值
    ret ; 需要处理的协程已经处理完毕
save_context:
    ; 保护当前协程，栈中已保存返回地址
    push rcx ; 当前协程地址
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
    push rsp
    mov QWORD PTR [rcx], rsp
    ; 切换到需要处理的协程
    mov rsp, QWORD PTR [rdx]
    jmp asm_coro_resume
asm_coro_yield ENDP

asm_call_coro_finish PROTO
asm_coro_return PROC PRIVATE
    pop rcx
    sub rsp, (40 + 8)   ; align rsp to 16 bytes
    call asm_call_coro_finish
    add rsp, (40 + 8)
    mov rsp, QWORD PTR [rax]
    jmp asm_coro_resume
asm_coro_return ENDP

; pstack + alloc <-- 00 <-- 16 字节对齐
;             co <-- 08 <-- rsp
;  asm_coro_call <-- 00
;         rsp-16 <-- 08
;         rsp-24 <-- 00
;         rsp-32 <-- 08
;         rsp-40 <-- 00 <-- sub rsp,40 输入参数对齐
; return address <-- 08 <-- rsp
;     proc(coro) <-- 00
asm_coro_call PROC
    mov rax, rcx    ; mov co to rax
    xchg rax, [rsp] ; push co for asm_coro_return && coro proc -> rax
    sub rsp, 40     ; align rsp to 16 bytes
    call rax        ; call proc(coro)
    add rsp, 40
    jmp asm_coro_return
asm_coro_call ENDP

END
