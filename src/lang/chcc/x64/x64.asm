; Windows 64 位程序前四个整型参数通过 RCX RDX R8 R9 传递，超过八字节的和额外的整型参数
; 通过栈传递。函数调用前必须至少留出32字节的栈空间，让系统函数保存 RCX RDX R8 R9 临时副
; 本，进入被调函数时 RSP 应该对齐到16字节地址边界，但 Win64 API 也不强制要求这个规则。
; 返回值使用 RAX 寄存器返回。
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

coro_rsp_not_match PROTO
coro_finish PROTO
print_dec PROTO
print_hex PROTO

.code

; [in]  rcx 协程的地址
; [in]  rdx 协程继续处理时的返回参数
coro_resume PROC
    pop rax ; 不需要返回到被调函数中
    mov rsp, [rcx]
    pop rax ; 弹出协程栈中保存的 rsp
    cmp rax, rsp
    jne rsp_err
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    pop rsi
    pop rdi
    mov rax, rdx    ; 协程函数的参数在 rcx 中
    push rcx

    sub rsp, 40
    call print_hex  ; 打印协程地址
    add rsp, 40

    mov rcx, rsp
    sub rsp, 40
    call print_hex  ; 打印协程 rsp
    add rsp, 40

    pop rcx
    pop rdx         ; 协程函数地址
    push rdx
    push rcx

    mov rcx, rdx
    sub rsp, 40
    call print_hex  ; 打印协程函数地址
    add rsp, 40

    pop rcx
    ret ; 返回到栈中已保存的协程返回地址
rsp_err:
    mov rcx, rax
    mov rdx, rsp
    sub rsp, 40
    call coro_rsp_not_match
    add rsp, 40
coro_resume ENDP


; [in]  rcx 当前协程
; [in]  rdx 需要等待处理的协程
; [in]  r8  等待处理的协程继续处理时的返回参数
coro_yield PROC
    ; 栈中已保存该协程的返回地址
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
    mov [rdx + 8], rcx  ; 当等待的协程处理完毕需要恢复当前协程
    mov rcx, rdx
    mov rdx, r8
    call coro_resume
coro_yield ENDP


coro_end PROC PRIVATE
    pop rcx ; 弹出协程地址
    jmp coro_finish
coro_end ENDP


; [in]  rcx 协程的地址
; [in]  rdx 协程函数的地址
coro_init_stack PROC
    push rdx
    mov rax, [rcx]
    mov [rax - 8 * 1], rcx              ; 压入协程地址
    mov rdx, coro_end
    mov [rax - 8 * 2], rdx              ; 压入协程结束处理函数
    pop rdx
    mov [rax - 8 * 3], rdx              ; 压入协程函数
    mov QWORD PTR [rax - 8 * 4], 0      ; rdi
    mov QWORD PTR [rax - 8 * 5], 0      ; rsi
    mov QWORD PTR [rax - 8 * 6], 0      ; rbx
    mov QWORD PTR [rax - 8 * 7], 0      ; rbp
    mov QWORD PTR [rax - 8 * 8], 0      ; r12
    mov QWORD PTR [rax - 8 * 9], 0      ; r13
    mov QWORD PTR [rax - 8 *10], 0      ; r14
    mov QWORD PTR [rax - 8 *11], 0      ; r15
    sub rax, 8 * 11
    mov [rax - 8], rax                  ; rsp
    sub rax, 8
    mov [rcx], rax  ; 更新协程的栈地址
    ret
coro_init_stack ENDP

END
