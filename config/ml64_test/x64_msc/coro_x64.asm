TITLE 64-bit MASM

; 程序退出
; kernel32.lib kernel32.dll windows.h (processthreadsapi.h)
; void WINAPI ExitProcess()
;       [in]            UINT exit_code
ExitProcess PROTO

; 调用线程最后一个错误的错误码
; kernel32.lib kernel32.dll windows.h (errhandlingapi.h)
; DWORD WINAPI GetLastError()
GetLastError PROTO

; 标准输入输出句柄
; kernel32.lib kernel32.dll windows.h (winbase.h <- processenv.h)
; HANDLE WINAPI GetStdHandle()
;       [in]            DWORD nStdHandle:
;                       STD_INPUT_HANDLE ((DWORD)-10)
;                       STD_OUTPUT_HANDLE ((DWORD)-11)
;                       STD_ERROR_HANDLE ((DWORD)-12)
;       [return NULL]   未找到关联的标准设备句柄
;       [return INVALID_HANDLE_VALUE] 函数失败，调用 GetLastError() 获取错误码
;       [return other]  标准设备句柄
GetStdHandle PROTO
HANDLE TEXTEQU <QWORD>
STDIN_HANDLE  EQU -10
STDOUT_HANDLE EQU -11
STDERR_HANDLE EQU -12

; 读控制台缓存
; kernel32.lib kernel32.dll windows.h (wincon.h <- consoleapi.h)
; BOOL WINAPI ReadConsole()
;       [in]            HANDLE handle_to_the_console_input_buffer
;       [out]           LPVOID out_buffer                                       ; 长度至少 number_of_chars_to_read * sizeof(TCHAR)
;       [in]            DWORD number_of_chars_to_read                           ; 读取的字符个数
;       [out]           LPDWORD number_of_chars_actually_read                   ; 返回实际读取的字符个数
;       [in, optional]  LPVOID input_control                                    ; 执行一个 CONSOLE_READCONSOLE_CONTROL 结构体
;       [return != 0]   成功
;       [return == 0]   失败，调用 GetLastError() 获取错误码
ReadConsoleA PROTO
readconsole TEXTEQU <ReadConsoleA>

; 写控制台缓存
; kernel32.lib kernel32.dll windows.h (wincon.h <- consoleapi.h)
; BOOL WINAPI WriteConsole()
;       [in]            HANDLE handle_to_the_console_output_buffer
;       [in]            const void *buffer                                      ; 需要写入的字符数据，是一个 char 数组或者 wchar_t 数组
;       [in]            DWORD number_of_chars_to_write                          ; 写入的字符个数
;       [out, optional] LPDWORD number_of_chars_written                         ; 返回实际写入的字符个数
;       [reserved]      LPVOID reserved                                         ; 必须为 NULL
;       [return != 0]   成功
;       [return == 0]   失败，调用 GetLastError() 获取错误码
WriteConsoleA PROTO
writeconsole TEXTEQU <WriteConsoleA>

; Windows 64 位程序前四个整型参数通过 RCX RDX R8 R9 传递，超过八字节的和额外的整型参数
; 通过栈传递。函数调用前必须至少留出32字节的栈空间，让系统函数保存 RCX RDX R8 R9 临时副
; 本，进入被调函数时 RSP 应该对齐到16字节地址边界，但 Win64 API 也不强制要求这个规则。
; 返回值使用 RAX 寄存器返回。
SHADOW_AND_ALIGN = 40   ; 4 * 8 + 8, reserve shadow space & align rsp

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

CO_MAX_NUM = 10
STACK_SIZE = 1024

.data
newline     BYTE 0dh,0ah
digtab      BYTE "0123456789ABCDEF"
coromsg     BYTE "coro print "
coromsg_len = $ - coromsg
overflow    BYTE "too many corutines."
overflow_len = $ - overflow
finished    BYTE "coroutine finished."
finished_len = $ - finished
rsp_not_match BYTE "coroutine rsp not match."
rsp_not_match_len = $ - rsp_not_match
hello       BYTE "hello world."
hello_len = $ - hello
co_id       QWORD 0
co_num      QWORD 0
stacks_end  QWORD stacks + CO_MAX_NUM * STACK_SIZE

.data?
stdout  HANDLE ?
rspval  QWORD ?
co_rsp  QWORD CO_MAX_NUM dup (?)
co_rbp  QWORD CO_MAX_NUM dup (?)
co_rip  QWORD CO_MAX_NUM dup (?)
stacks  QWORD CO_MAX_NUM * STACK_SIZE dup (?)

.code
init PROC
    sub rsp, SHADOW_AND_ALIGN
    mov rcx, STDOUT_HANDLE
    call GetStdHandle
    mov stdout, rax
    add rsp, SHADOW_AND_ALIGN
    ret
init ENDP

; 写出到标准输出
; [in]  rdx  字符串地址
; [in]  r8   字符串长度
print PROC
    sub rsp, (5 * 8)                    ; 预留四个影子栈空间和第五个参数的空间
    mov rcx, stdout                     ; 第一个参数，控制台句柄（rcx）；第二个参数，字符串地址（rdx）；第三个参数，字符串长度（r8）
    mov r9, 0                           ; 第四个参数，实际写入的字符个数（r9）
    mov QWORD PTR [rsp + 4 * 8], 0      ; 第五个参数，需要通过栈传递
    ; 地址由上到下从低到高
    ; [ 第一个参数    ] sub rsp, (5 * 8) 之后 RSP 指向这里
    ; [ 第二个参数    ]
    ; [ 第三个参数    ]
    ; [ 第四个参数    ]
    ; [ 第五个参数    ] rsp + 4 * 8
    ; [return address] 最开始 RSP 指向这里
    call writeconsole
    add rsp, (5 * 8)
    ret
print ENDP

crlf PROC
    mov rdx, OFFSET newline
    mov r8, 2
    call print
    ret
crlf ENDP

println PROC
    call print
    call crlf
println ENDP

; digchar: 将整数字节转换成数字字符
; [in]  al 传入整数字节
; [out] al 返回数字字符
digchar PROC PRIVATE
     push rbx
     mov rbx, OFFSET digtab
     xlat
     pop rbx
     ret
digchar ENDP

;------------------------------------------------------
; WriteHexB
; Writes an unsigned 64-bit hexadecimal number to
; the console window.
; Receives: RAX = the number to write. RBX = display size (1,2,4,8)
; Returns: nothing
;------------------------------------------------------
WriteHexB PROC 
    LOCAL displaySize:QWORD
    DOUBLEWORD_BUFSIZE = 8
    QUADWORD_BUFSIZE = 16
.data
bufferLHB BYTE QUADWORD_BUFSIZE DUP(?),0

.code
    push  rbx                        ; must be preserved
    push  rdi
    mov displaySize,rbx                ; save component size

; Clear unused bits from EAX to avoid a divide overflow.
; Also, verify that EBX contains either 1, 2, 4, or 8. If any
; other value is found, default to 8.

A1: cmp  rbx,1                    ; check the specified display size
     jne  A2
     and  rax,0FFh                ; mask lower byte, clear remaining bits
     jmp     A99

A2: cmp     rbx,2
     jne     A4
     and  rax,0FFFFh                ; mask lower word
     jmp     A99

A4: cmp   rbx,4
     jne   A8
     mov   r8,0FFFFFFFFh            ; clears upper 32 bits??
     and   rax,r8                    ; mask lower doubleword
     mov   displaySize,4
     jmp   A99

A8: mov displaySize,8            ; default size (quadword) == 8
A99:
    ; CheckInit

    mov   rdi,displaySize    ; let EDI point to the end of the buffer:
    shl   rdi,1                    ; multiply by 2 (2 digits per byte)
    mov   bufferLHB[rdi],0     ; store null string terminator
    dec   rdi                    ; back up one position

    mov   rcx,0               ; digit counter
    mov   rbx,16                ; hexadecimal base (divisor)

L1:
    mov   rdx,0              ; clear upper dividend
    div   rbx                ; divide EAX by the base

    xchg  rax,rdx            ; swap quotient, remainder
    call  digchar         ; convert AL to ASCII
    mov   bufferLHB[rdi],al       ; save the digit
    dec   rdi                 ; back up in buffer
    xchg  rax,rdx            ; swap quotient, remainder

    inc   rcx                 ; increment digit count
    or    rax,rax            ; quotient = 0?
    jnz   L1               ; no, divide again

     ; Insert leading zeros

    mov   rax,displaySize    ; set EAX to the
    shl   rax,1                    ; number of digits to print
    sub   rax,rcx                ; subtract the actual digit count
    jz    L3                   ; display now if no leading zeros required
    mov   rcx,rax             ; CX = number of leading zeros to insert

L2:
    mov   bufferLHB[rdi],'0'    ; insert a zero
    dec   rdi                      ; back up
    loop  L2                    ; continue the loop

    ; Display the digits. RCX contains the number of
    ; digits to display, and RDX points to the first digit.
L3:
    mov   rcx,displaySize        ; output format size
    shl   rcx,1                 ; multiply by 2
    inc   rdi
    mov   rdx,OFFSET bufferLHB
    
    add   rdx,rdi
    mov   r8,rcx
    call  print

    pop   rdi
    pop   rbx                ; must be preserved
    ret
WriteHexB ENDP

;------------------------------------------------------
; WriteHex64
; Writes an unsigned 64-bit hexadecimal number to
;         the console window
; Receives: RAX = the number to write
; Returns: nothing
;------------------------------------------------------
print64 proc
    push rbx
    mov  rbx,8
    call WriteHexB
    pop  rbx
    ret
print64 ENDP

;--------------------------------------------------
; WriteHex32
; Writes the lower 32 bits of an integer to the
; console window in hexadecimal format.
; Receives: RAX = the number to write.
; Returns: nothing
;--------------------------------------------------
print32 PROC
    push rbx
    mov  rbx,4        ; number of bytes
    call WriteHexB
    pop  rbx
    ret
print32 ENDP

; print_n: 打印单个数字字符
; [in]  rcx 要打印的整数
print_n PROC
    mov rax, rcx
    mov rcx, 0
    mov r8, 0
    mov r9, 10
again:
    mov rdx, 0
    div r9
    add rdx, 48
    shl rcx, 8
    or rcx, rdx
    inc r8
    cmp rax, 0
    jnz again
    push rcx
    mov rdx, rsp
    call println
    pop rcx
    ret
print_n ENDP


resume PROC
    mov rbx, co_id
    mov rsp, [OFFSET co_rsp + rbx * 8]      ;; 第一次执行会执行第一个 create 创建的协程
    mov rbp, [OFFSET co_rbp + rbx * 8]      ;; 将协程的 rsp、rbp 恢复到对应寄存器
    jmp QWORD PTR [OFFSET co_rip + rbx * 8] ;; 跳转到协程对应的过程处继续执行
resume ENDP

inc_co_id PROC
    mov rbx, co_id
    inc rbx                                 ;; 指向下一个协程
    mov rcx, 0                              ;; rcx 0
    cmp rbx, co_num                         ;;
    cmovge rbx, rcx                         ;; 如果超过当前 co_num 重新回到 0
    mov co_id, rbx                          ;; 更新到 co_id，也即对每个协程不断进行轮询
    ret
inc_co_id ENDP

yield PROC
    mov rbx, co_id
    pop rax                                 ;; rax 里保持返回地址
    mov [OFFSET co_rsp + rbx * 8], rsp      ;; 保存当前的协程环境
    mov [OFFSET co_rbp + rbx * 8], rbp
    mov [OFFSET co_rip + rbx * 8], rax

    call inc_co_id
    jmp resume
yield ENDP

co_end PROC
    mov rdx, OFFSET finished
    mov r8, finished_len
    call println
    call inc_co_id
    jmp resume
co_end ENDP


;; rcx - procedure to start in a new coroutine，相同过程可以启动多个协程，每个协程有单独的栈
create PROC
    cmp co_num, CO_MAX_NUM
    jge overflow_fail

    mov rbx, co_num                             ;; 当前创建的协程的索引
    inc co_num                                  ;; 协程计数加 1
    mov rax, stacks_end                         ;; 当前栈底地址
    sub stacks_end, STACK_SIZE                  ;; 下一个栈底地址，每个栈的大小为 STACK_SIZE

    sub rax, 8                                  ;; 将结束函数压入栈
    mov rdx, co_end                             ;; 协程结束函数 co_end
    mov QWORD PTR [rax], rdx                    ;; 作为协程的返回地址

    mov [OFFSET co_rsp + rbx * 8], rax          ;; 保持协程的栈地址
    mov QWORD PTR [OFFSET co_rbp + rbx * 8], 0  ;; 协程的基地址为0
    mov [OFFSET co_rip + rbx * 8], rcx          ;; 协程所要执行的过程地址

    call print32
    call crlf
    ret
overflow_fail:
    mov rdx, OFFSET overflow
    mov r8, overflow_len
    call println
    mov ecx, 1
    call ExitProcess
create ENDP


co_main PROC
    mov rbx, co_num                             ;; 当前创建协程的索引
    inc co_num                                  ;; 协程计数加1
    mov rax, rsp
    call print32
    call crlf
    pop rax                                     ;; 弹出函数返回地址到rax
    mov [OFFSET co_rsp + rbx * 8], rsp          ;; 主协程的栈地址
    mov [OFFSET co_rbp + rbx * 8], rbp          ;; 主协程的栈基址
    mov [OFFSET co_rip + rbx * 8], rax          ;; 主协程所要执行的过程地址
    jmp rax                                     ;; 跳转到返回地址
co_main ENDP


count PROC
    push rbp                    ;; 保护栈基地址
    mov rbp, rsp                ;; 将栈顶指针保持到rbp
    sub rsp, 8                  ;; 分配一个局部变量
    mov QWORD PTR [rbp - 8], 0  ;; 局部变量初始化为0
again:
    cmp QWORD PTR [rbp - 8], 3  ;; 与 3 进行比较
    jg  over                    ;; 大于 3 就结束循环
    mov rdx, OFFSET coromsg
    mov r8, coromsg_len
    call print
    mov rcx, [rbp - 8]          ;; 将局部变量的值移动到rcx
    call print_n                ;; 打印当前局部变量的值
    call yield                  ;; 暂停协程
    inc QWORD PTR [rbp - 8]     ;; 局部变量加1
    jmp again                   ;; 继续循环
over:
    add rsp, 8                  ;; 释放局部变量
    pop rbp                     ;; 恢复栈基地址
    ret                         ;; 函数返回
count ENDP


main PROC
    mov rspval, rsp
    call init

    call co_main

    mov rcx, count
    call create

    mov rcx, count
    call create

    mov rcx, co_num
    call print_n

    call yield ;; 协程0保存下一条指令的地址，执行协程1打印0，协程1保存下一条指令的地址，执行协程2打印0，协程2保存下一条指令的地址，执行协程0即跳到下一条指令
    call yield ;; 打印1
    call yield ;; 打印2
    call yield ;; 打印3
    call yield ;; 协程0保存下一条指令的地址，协程1执行co_end，协程2执行co_end，执行协程0即跳到下一条指令

    mov rdx, OFFSET hello
    mov r8, hello_len
    call println

    mov rcx, 1024
    call print_n

    mov rcx, stdout
    call print_n

    mov rax, rspval
    call print32
    call crlf

    mov ecx, 0
    call ExitProcess
main ENDP

; https://retroscience.net/x64-assembly.html
shadow_call PROC ; rax - func need shadow space
    and rsp, not 15 ; make sure current stack 16-byte aligned
    sub rsp, 32
    call rax
    add rsp, 32
    ret
shadow_call ENDP

END
