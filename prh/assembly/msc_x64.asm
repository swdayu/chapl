; Windows 32 位程序 __fastcall 调用约定：被调函数清理栈参数，前两个整型参数通过寄存器
; ECX EDX 传递，超过四字节的和额外的整型参数从右到左通过栈传递，对于类、结构体、或联合
; 体，不管其大小都通过栈传递。由于是被调函数自动清理栈参数，__fastcall 调用约定的函数必
; 须提供函数原型，否则编译器不能确定参数的总长度。__fastcall 的函数名称装饰规则：添加一
; 个前置 @ 字符，并且添加一个 @ 字符后缀以及一个十进制数表示所有参数的字节大小。返回值
; 会通过寄存器 EAX 或者 EDX:EAX 返回，可以传递 8 个字节，更大的结构体数据通过指针返回，
; 该指针在被调函数返回前保存在 EAX 寄存器中。另外非 POD 的结构体数据也通过指针返回。前
; 两个参数通过 ECX EDX 传递时，不需要额外为被调函数分配影子空间，如果被调函数需要保存
; 寄存器参数的副本，它可以以任何自己的方式在函数内部处理。因此被调函数返回前需要清理的
; 栈参数的大小，需要减掉通过寄存器传递的参数大小。如果是可变参数函数，所有参数都通过栈
; 传递，回退去使用 cdecl 约定。
;
; 寄存器                   类型            用法
; EAX                     易变            返回值
; ECX                     易变            第一个整型参数
; EDX                     易变            第二个整型参数
; EDI                     非易变          被调函数必须负责保护
; ESI                     非易变          被调函数必须负责保护
; EBX                     非易变          被调函数必须负责保护
; EBP                     非易变          被调函数必须负责保护，可以用于栈帧指针
; ESP                     非易变          栈指针
; 当函数退出、函数进入 C 运行时库或 Windows 系统，标志寄存器中的方向标志必须先清位。
;
; 返回值通过寄存器 EAX EDX 返回，更大的返回值通过内存地址返回，返回值内存地址当作函数
; 第一个栈参数传递，函数真正的参数还依然依次使用 ECX EDX 传递。
;
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
