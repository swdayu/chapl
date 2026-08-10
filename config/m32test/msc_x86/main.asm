; .386
; .MODEL FLAT
; .code

TITLE Adds Two 32-bit Intergers (AddTwo.asm)
.386
.MODEL FLAT,STDCALL
.STACK 4096
ExitProcess PROTO,exitcode:DWORD

.DATA
sum DWORD 0

.CODE
main PROC
    MOV EAX,5
    ADD EAX,6
    MOV sum,EAX

    INVOKE ExitProcess,0
main ENDP
END main
