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

.data
newline BYTE 0dh,0ah
digtab BYTE "0123456789ABCDEF"
hello BYTE "hello world."
hello_len = $ - hello

.data?
stdout  HANDLE ?
rspval  QWORD ?

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

println PROC
    call print
    mov rdx, OFFSET newline
    mov r8, 2
    call print
    ret
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

; printdigit: 打印单个数字字符
; [in]  rcx 要打印的整数
print_int_ln PROC
    add rdi, 48
    mov [ch], rdi
    mov rax, SYS_write
    mov rdi, stdout
    mov rsi, ch
    mov rdx, ch_len
    syscall
    ret
printdigit ENDP

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
    call  prints

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

main PROC
    mov rspval,rsp
    call init

    mov eax,5
    add eax,6
    call print32
    call crlf

    mov rdx,offset hello
    mov r8,COUNT
    call prints
    call crlf

    mov rax,rspval
    call print32
    call crlf

    mov ecx,0
    call ExitProcess
main ENDP

test_add PROC
    add al,bl
    add ax,bx
    add eax,ebx
    add rax,rbx
    add [eax],bl
    add [eax],bx
    add [eax],ebx
    add [rax],bl
    add [rax],bx
    add [rax],ebx
    add [rax],r8b
    add [rax],r8w
    add [rax],r8d
    add [rax],r8
    add bl,al
    add bx,ax
    add ebx,eax
    add rbx,rax
    add bl,[eax]
    add bx,[eax]
    add ebx,[eax]
    add bl,[rax]
    add bx,[rax]
    add ebx,[rax]
    add r8b,[rax]
    add r8w,[rax]
    add r8d,[rax]
    add r8,[rax]
test_add ENDP

END
