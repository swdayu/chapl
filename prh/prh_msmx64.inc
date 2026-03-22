; 寄存器保护   <-- rsp
; 寄存器保护
; 寄存器保护
; 局部变量一   <-- localbase@proc rbp-局部变量空间大小
; 局部变量二
; 局部变量三
; 寄存器rbp    <-- rbp
; ------------- 进入被调函数
; 函数返回地址 <-- rsp
; RCX 影子空间 <-- parmbase rbp+16
; RDX 影子空间
; R8 影子空间
; R9 影子空间
; 栈函数参数一
; 栈函数参数二
; 栈函数参数三
; 主调函数，此处为高地址（栈底）

; [in]  rcx 当前协程，协程返回地址已在栈顶
prh_asm_spco_yield:
    mov [rcx], rsp      ; 保护协程栈指针
    mov rsp, [rcx-8]    ; 切换到主协程栈
    ret                 ; 返回主协程

dptr equ dq

macro prh_asm_rsp_enter { ; 仅使用寄存器传递参数，因此适用于函数参数不大于4个的场景
    sub rsp,40 ; 预留4个寄存器影子栈空间，填补1个空间用于rsp对齐到16字节边界
}

macro prh_asm_rsp_leave {
    add rsp,40
}

macro prh_asm_rbp_enter {
    push rbp
    mov rbp,rsp
}

macro prh_asm_rbp_leave {
    mov rsp,rbp
    pop rbp
}
