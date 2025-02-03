; Windows 64 位程序前四个整型参数通过 RCX RDX R8 R9 传递，超过八字节的和额外的整型参数
; 通过栈传递。函数调用前必须至少留出32字节的栈空间，让系统函数保存 RCX RDX R8 R9 临时副
; 本，进入被调函数时 RSP 应该对齐到16字节地址边界，但 Win64 API 也不强制要求这个规则。
; 返回值使用 RAX 寄存器返回。调用者负责清除所有的参数和 shadow space 栈空间。
;
; 寄存器                   类型            用法
; RAX                     易变            返回值
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
coro_asm_init PROC
    ; 调用该函数之前，协程地址已经保存 ; rcx, +8
    mov QWORD PTR [rcx - 8 * 1], 0  ; rdi, +16 <-- 16 字节对齐
    mov QWORD PTR [rcx - 8 * 2], 0  ; rsi, +24
    mov QWORD PTR [rcx - 8 * 3], 0  ; rbx, +32 <-- 16 字节对齐
    mov QWORD PTR [rcx - 8 * 4], 0  ; rbp, +40
    mov QWORD PTR [rcx - 8 * 5], 0  ; r12, +48 <-- 16 字节对齐
    mov QWORD PTR [rcx - 8 * 6], 0  ; r13, +56
    mov QWORD PTR [rcx - 8 * 7], 0  ; r14, +64 <-- 16 字节对齐
    mov QWORD PTR [rcx - 8 * 8], 0  ; r15, +72
    sub rcx, 8 * 8
    mov QWORD PTR [rcx - 8], rcx    ; rsp, +80 <-- 16 字节对齐
    sub rcx, 8
    mov rax, rcx
    ret ; 返回当前 rsp 的值
coro_asm_init ENDP

; [in]  rcx 协程的栈指针
coro_stack_crash PROTO
coro_asm_resume PROC PRIVATE
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
    ; coro stack r15 <-- 72 rsp
    ;             -8 <-- 80 rsp 16 字节对齐
    ;            -16 <-- 08
    ;            -24 <-- 16 rsp 16 字节对齐
    ;            -32 <-- 24
    ;            -40 <-- 32 rsp 16 字节对齐
    ; return address <-- 40
    ;                <-- 48 rsp 16 字节对齐
    sub rsp, 40     ; align rsp to 16 bytes
    call coro_stack_crash
coro_asm_resume ENDP

; [in]  rcx 当前协程
; [in]  rdx 需要处理的协程
coro_asm_yield PROC
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
    jmp coro_asm_resume
coro_asm_yield ENDP

; [in]  rcx 当前协程
; [in]  rdx 需要处理的协程
coro_asm_yield_manual PROC
    mov [rdx + 8], rcx  ; 当协程处理完毕后恢复当前协程
    jmp coro_asm_yield
coro_asm_yield_manual ENDP

coro_finish PROTO
coro_asm_return PROC PRIVATE
    pop rcx
    sub rsp, (40 + 8)   ; align rsp to 16 bytes
    call coro_finish
    add rsp, (40 + 8)
    mov rcx, [rax]
    jmp coro_asm_resume
coro_asm_return ENDP

; return address <-- rsp
;             -8 <-- 00 rsp 16 字节对齐
;            -16 <-- 08
;            -24 <-- 16
;            -32 <-- 24
;            -40 <-- 32
; return address <-- 40
;                <-- 48 rsp 16 字节对齐
coro_asm_call PROC
    sub rsp, 40         ; align rsp to 16 bytes
    call QWORD PTR [rcx + 8 * 2]  ; call co->func
    add rsp, 40
    jmp coro_asm_return
coro_asm_call ENDP

END
