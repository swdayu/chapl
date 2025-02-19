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

.code
asm_coro_init PROC
    ; 调用该函数之前，协程地址已经保存
    ; pstack + alloc <-- 00                   <-- 16字节对齐
    ;             -8 <-- 08 co
    ;            -16 <-- 00 asm_coro_call     <-- 16字节对齐
    ;            -24 <-- 08           rcx, 08
    mov QWORD PTR [rcx - 8 * 1], 0  ; rdi, 00 <-- 16字节对齐
    mov QWORD PTR [rcx - 8 * 2], 0  ; rsi, 08
    mov QWORD PTR [rcx - 8 * 3], 0  ; rbx, 00 <-- 16字节对齐
    mov QWORD PTR [rcx - 8 * 4], 0  ; rbp, 08
    mov QWORD PTR [rcx - 8 * 5], 0  ; r12, 00 <-- 16字节对齐
    mov QWORD PTR [rcx - 8 * 6], 0  ; r13, 08
    mov QWORD PTR [rcx - 8 * 7], 0  ; r14, 00 <-- 16字节对齐
    mov QWORD PTR [rcx - 8 * 8], 0  ; r15, 08
    sub rcx, 8 * 8
    mov QWORD PTR [rcx - 8], rcx    ; rsp, 00 <-- 16字节对齐
    sub rcx, 8
    mov rax, rcx
    ret ; 返回当前 rsp 的值
asm_coro_init ENDP

; [in]  rcx 协程的栈指针
asm_call_stack_crash PROTO
asm_coro_resume PROC PRIVATE
    mov rsp, rcx
    pop rcx ; 弹出协程栈中保存的 rsp
    cmp rcx, rsp
    jne rsp_crash
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
    ;            rsp <-- 00
    ; coro stack r15 <-- 08 rsp
    ;         rsp-08 <-- 00
    ;         rsp-16 <-- 08
    ;         rsp-24 <-- 00
    ;         rsp-32 <-- 08
    ;         rsp-40 <-- 00 输入参数对齐
    ; return address <-- 08 rsp
    ; asm_call_stack_crash - 00
    sub rsp, 40     ; align rsp to 16 bytes
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
    push rsp
    mov [rcx], rsp
    ; 切换到需要处理的协程
    mov rcx, [rdx]
    jmp asm_coro_resume
asm_coro_yield ENDP

; [in]  rcx 当前协程
; [in]  rdx 需要处理的协程
asm_coro_yield_manual PROC
    mov [rdx + 8], rcx  ; 当协程处理完毕后恢复当前协程
    jmp asm_coro_yield
asm_coro_yield_manual ENDP

asm_call_coro_finish PROTO
asm_coro_return PROC PRIVATE
    pop rcx
    sub rsp, (40 + 8)   ; align rsp to 16 bytes
    call asm_call_coro_finish
    add rsp, (40 + 8)
    mov rcx, [rax]
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
;       co->func <-- 00
asm_coro_call PROC
    sub rsp, 40         ; align rsp to 16 bytes
    call QWORD PTR [rcx + 8 * 2]  ; call co->func
    add rsp, 40
    jmp asm_coro_return
asm_coro_call ENDP

END
