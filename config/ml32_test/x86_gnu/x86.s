# gcc -o a.out -nostdlib  a.s

.global _start
.section .text

_start:

mov $0x1,%eax   # 32 bit of exit is 1
mov $0x1,%ebx
int $0x80

.section .data

# gcc -o a.out -nostdlib  a.s
# ./a.out
# Hello World

.global _start
.section .text

_start:

# write(stdout, "Hello World", 13);

mov $0x4,%eax       # 32 bit write syscall number
mov $0x1,%ebx       # unsigned int fd (stdout)
lea (message),%ecx  # const char *buf
mov $13,%edx        # size_t count
int $0x80

# exit(0)

mov $0x1,%eax
mov $0x0,%ebx
int $0x80

.section .data
message:
.ascii "Hello World\n"
