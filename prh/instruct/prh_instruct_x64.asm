.code

; [in]  rcx 需要执行的协程，主协程返回地址已在栈顶
; 特制的简单协程不会修改非易变寄存器，而协程调用的C函数会立即返回，因此不需要保护寄存器
prh_impl_asm_spco_main_yield PROC
    ;   协程初态
    ;   born_proc  <-- rsp      进入协程函数
    ;   spco_proc               返回地址  <-- rsp
    ;   guardaddr               guardaddr
    ;   mainstack               mainstack
    ;   stacktop   <-- coro --> stacktop
    ;   userdata   ____________ userdata
    ; ------------- 进入协程函数
    ;   函数返回地址 <-- mainstack
    ;   RCX 影子空间
    ;   RDX 影子空间
    ;   R8 影子空间
    ;   R9 影子空间
    ;   栈函数参数一
    ;   栈函数参数二
    ;   栈函数参数三
    ; 主协程栈布局，此处为高地址（栈底）
    mov rax, rsp        ; 主协程栈指针
    mov rsp, [rcx]      ; 加载协程栈
    mov [rcx-8], rax    ; 保护主协程栈指针
    ret                 ; 协程栈顶为start函数或协程继续执行地址或finish函数
prh_impl_asm_spco_main_yield ENDP

; [in]  rcx 当前协程，协程返回地址已在栈顶
prh_impl_asm_spco_yield PROC
    mov [rcx], rsp      ; 保护协程栈指针
    mov rsp, [rcx-8]    ; 切换到主协程栈
    mov rax, 1          ; 协程成功返回
    ret                 ; 返回主协程
prh_impl_asm_spco_yield ENDP

; [in]  rcx 当前协程
prh_impl_asm_spco_finish PROC PRIVATE
    mov [rcx], 0        ; 协程执行完毕，将栈指针清零
    mov rsp, [rcx-8]    ; 切换到主协程栈
    mov rax, 0          ; 协程执行完毕
    ret                 ; 返回主协程
prh_impl_asm_spco_finish ENDP

; [in]  rcx 需要执行的协程
; 由 prh_impl_asm_spco_main_yield 触发调用
prh_impl_asm_spco_start PROC
    ;   born_proc   <-- 00 对齐需求（输入参数必须对齐，即返回地址下面的地址是16字节对齐的）
    ;   spco_proc   <-- 08 <-- rsp 返回地址   <-- 08 <-- rsp
    ;   guardaddr   <-- 00     ==> guardaddr <-- 00
    ;   mainstack   <-- 08     ==> mainstack <-- 08
    ;   stacktop    <-- 00 <-- coro
    ;   userdata    <-- 08
    ; 协程初态布局，此处为高地址（栈底）
    pop rdx             ; 协程函数 spco_proc
    call rdx            ; 调用协程函数，特制协程不需要影子空间
    mov rax,OFFSET prh_impl_asm_spco_finish
    push rax            ; 栈顶压入finish函数
    mov rcx, [rsp+24]   ; rsp指向spco_proc位置，往后移动三个指针指向协程
    jmp prh_impl_asm_spco_yield
prh_impl_asm_spco_start ENDP

END
