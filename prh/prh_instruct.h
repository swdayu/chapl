#include "prh_include.h"
// Windows 64-bit                   Linux 64-bit
//  易变寄存器      非易变寄存器        易变寄存器      非易变寄存器
//     <RAX>        RBX                <RAX>            RBX
//     [RCX]        RDI                [RDI]            RBP
//     [RDX]        RSI                [RSI]            RSP
//     [R8 ]        RBP               <[RDX]>           R12
//     [R9 ]        RSP                [RCX]            R13
//      R10         R12                [R8 ]            R14
//      R11         R13                [R9 ]            R15
//    <[XMM0]>      R14                 R10
//     [XMM1]       R15                 R11
//     [XMM2]       XMM6             <[XMM0]> <[YMM0]>
//     [XMM3]       XMM7             <[XMM1]> <[YMM1]>
//     [XMM4]       XMM8              [XMM2]   [YMM2]
//     [XMM5]       XMM9               ...      ...
//    <[YMM0]>      XMM10             [XMM7]   [YMM7]
//     [YMM1]       XMM11              XMM8     YMM8
//     [YMM2]       XMM12              ...      ...
//     [YMM3]       XMM13              XMM15    YMM15
//     [YMM4]       XMM14
//     [YMM5]       XMM15
//
// Linux System Call
//  SYSCALL 64-bit                              32-bit INT80 中断
//      RAX     系统调用编号                        EAX     系统调用编号
//      RCX     保存用户空间的返回地址（RIP）        EBX     第一个参数
//      RDI     第一个参数                          ECX     第二个参数
//      RSI     第二个参数                          EDX     第三个参数
//      RDX     第三个参数                          ESI     第四个参数
//      R10     第四个参数                          EDI     第五个参数
//      R8      第五个参数                          EBP     第六个参数，因为是非易变寄存器，如果要使用必须先保护该寄存器
//      R9      第六个参数
//
// INT80 中断会自动记录用户空间的返回地址，遗留或老式系统调用，可以通过 int 指令触发软
// 件中断来使内核执行。Linux 内核预留了一个特定的软件中断编号，供用户空间程序用来进入内
// 核并执行系统调用。Linux 内核为中断编号 128（0x80）注册了一个名为 ia32_syscall 的中
// 断处理程序。使用 iret 指令可以从中断返回到用户空间地址，中断时返回地址和编码特权级别
// 以及其他内容的 CPU 标志等都被保存在程序栈中，然后 ia32_syscall 执行。为了恢复执行，
// 内核只需要从程序栈中复制回它们所属的寄存器值，然后执行就会在用户空间恢复。
//
//  movl $SYS_CODE, %eax
//  movl arg1, %ebx
//  movl arg2, %ecx
//  movl arg3, %edx
//  movl arg4, %esi
//  movl arg5, %edi
//  pushl %ebp
//  movl arg6, %ebp
//  int $0x80
//
// 通过 syscall 指令发起系统调用，内核会破坏寄存器 %rcx 和 %r11，但会保护除 %rax 之外
// 的其他寄存器。系统调用限制为六个参数，没有参数直接放在栈上。从系统调用返回后，寄存器
// %rax 包含系统调用的结果。范围在 -4095 到 -1 之间的值表示错误（-errno）。只有 INTEGER
// 类或 MEMORY 类的值能传递给内核。
//
//  movq $SYS_CODE, %rax
//  movq arg1, %rdi
//  movq arg2, %rsi
//  movq arg3, %rdx
//  movq arg4, %r10
//  movq arg5, %r8
//  movq arg6, %r9
//  syscall
//
// https://david-grs.github.io/tls_performance_overhead_cost_linux/
// https://www.akkadia.org/drepper/tls.pdf
// The glibc is using the segment register FS for accessing the TLS variable:
//      0x0000000000404f40 <+0>:     inc    DWORD PTR [rip+0x202382]
//      0x0000000000404f46 <+6>:     ret
//      0x0000000000404f50 <+0>:     inc    DWORD PTR fs:0xfffffffffffffffc
//      0x0000000000404f58 <+8>:     ret
// But if the thread variable is in a dynamic module, the linker cannot compute
// a direct reference to it and we get the much vaunted lookup.
//      0x0000000000000880 <+0>:     push   rax
//      0x0000000000000881 <+1>:     data32 lea rdi,[rip+0x20074f]
//      0x0000000000000889 <+9>:     data32 data32 call 0x770 <__tls_get_addr@plt>
//      0x0000000000000891 <+17>:    inc    DWORD PTR [rax]
//      0x0000000000000893 <+19>:    pop    rax
//      0x0000000000000894 <+20>:    ret
// 如果你的代码与TLS变量所在的代码是静态链接在一起的，那么访问TLS变量和普通变量几乎没有
// 差别。但是如果TLS变量所在的代码是一个动态链接库，那么访问TLS变量则变得昂贵，因为需要
// 进行运行时查询。
// As global and static variables are stored in .data and .bss sections, thread
// local variables get stored in equivalent .tdata and .tbss sections. Each
// thread has its on TCB — Thread Control Block — that contains some metadata
// about the thread itself. One metadata is the address of the DTV — Dynamic
// Thread Vector — which is a vector that contains pointers to thread local
// variables and other TLS blocks.
//
// 新代码只使用易变寄存器，可以兼容直接调C代码，如果调用层次中只允许新代码调用C代码，那
// 么新代码也可以自由使用非易变寄存器，因为这些寄存器C代码要使用就需要保护它，当C函数返
// 回时，这些寄存器总是没有被破坏。因此，其实新代码可以自由使用任何寄存器。
//  1.  新代码调用C代码，按照对应的调用约定直接调用
//  2.  C代码调用新代码，保护新代码用到的所有非易变寄存器，调用新代码，恢复非易变寄存器
//  3.  编译器为新代码函数记录，该函数（包括函数调用的函数）使用的所有非易变寄存器
//  4.  生成可以被C代码调用的新代码时，可以优化只使用易变寄存器
//  5.  为了兼容 MSVC 和 GCC，易变寄存器只使用 RAX RCX RDX R8 R9 R10 R11
//  6.  协程挂起需要保护非易变寄存器的原因是，新执行的协程可能执行一半就返回了
//  7.  此时，新协程可能就破坏了非易变寄存器，因为这些寄存器在函数返回时才会恢复
//
// stackful specially made coroutine stack layout:
//  Lower memery address
//  [coro_guard ]
//  [    ...    ]
//  [    ...    ]         ^
//  [    ...    ]         ^
//  [stack data ] <-- rsp | <-- stack top
//  [stack data ]         |
//  [stack data ]         |
//  [born_proc  ]         |
//  [spco_proc  ]         |
//  [guardaddr  ]         |
//  [mainstack  ]         |
//  [stacktop   ] <-- spco
//  [userdata   ]
//  -------------
//  Upper memory address

// 特制的简单协程不会修改非易变寄存器，而协程调用的C函数会立即返回，因此不需要保护寄存器
typedef struct {
    prh_arch_reg stacktop;
    prh_arch_reg userdata;
} prh_spco;

typedef struct {
    prh_arch_reg born_proc;
    prh_arch_reg spco_proc;
    prh_arch_reg guardaddr;
    prh_arch_reg mainstack;
} prh_impl_spco_base;

typedef struct {
    prh_arch_reg lower_guard_word;
    prh_arch_reg upper_guard_word;
} prh_impl_guard;

#define prh_spco_proc prh_fastcall(void) // prh_spco_proc procedure(prh_spco *co)
prh_fastcall_typedef(void, prh_spcoproc_t)(prh_spco *);
prh_fastcall(void) prh_impl_asm_spco_start(prh_spco *);

void prh_impl_spco_load(prh_spco *coro, prh_spcoproc_t proc) {
    assert((prh_reg)coro % 16 == 0); // initial rsp shall 16-byte aligned
    prh_impl_spco_base *base = (prh_impl_spco_base *)coro - 1;
    base->born_proc = (prh_arch_reg)prh_impl_asm_spco_start;
    base->spco_proc = (prh_arch_reg)proc;
    coro->stacktop = (prh_arch_reg)base;
}
