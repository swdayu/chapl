.global _start
.section .text

_start:

mov $0x3c,%eax   # 64 bit of exit is 60(0x3c)
mov $0x1,%ebx
syscall

.section .data
