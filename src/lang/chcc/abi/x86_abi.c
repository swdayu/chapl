#include "chcc/abi/x86_abi.h"

// 000 001 010 011 100 101 110 111
// EAX ECX EDX EBX ESP EBP ESI EDI
//
// 寄存器                   类型            用法
// RAX                     易变            返回值
// RCX                     易变            第一个整型参数
// RDX                     易变            第二个整型参数
// R8                      易变            第三个整型参数
// R9                      易变            第四个整型参数
// R10:R11                 易变            调用者必须负责保护，用于 syscall/sysret 指令
// R12:R15                 非易变          被调函数必须负责保护
// RDI                     非易变          被调函数必须负责保护
// RSI                     非易变          被调函数必须负责保护
// RBX                     非易变          被调函数必须负责保护
// RBP                     非易变          被调函数必须负责保护，可以用于栈帧指针
// RSP                     非易变          栈指针
// XMM0 YMM0               易变            第一个FP参数，第一个向量类型参数
// XMM1 YMM1               易变            第二个FP参数，第二个向量类型参数
// XMM2 YMM2               易变            第三个FP参数，第三个向量类型参数
// XMM3 YMM3               易变            第四个FP参数，第四个向量类型参数
// XMM4 YMM4               易变            调用者必须负责保护，第五个向量类型参数
// XMM5 YMM5               易变            调用者必须负责保护，第六个向量类型参数
// XMM6:XMM15              非易变          被调函数必须负责保护
// YMM6:YMM15              高16字节易变    调用者根据需要必须负责保护YMM
// 当函数退出、函数进入 C 运行时库或 Windows 系统，标志寄存器中的方向标志必须先清位。

// 以下栈地址都从低到高
//
// 绝对地址调用：       9. 当被调函数返回时，执行 0xc9: leave 将 %ebp 位置栈中的值恢复到 esp 相当于 esp 重新执行函数返回地址，然后执行 0xc3 ret 抛出并跳转到返回地址
//      局部变量3       8. loc += 4; 第三个局部变量地址 (%ebp-12); *a = loc; 步骤5中 sub %xx,%esp 执行后，esp 指向这里 <--- esp
//      局部变量2       7. loc += 4; 第二个局部变量地址 (%ebp-8)
//      局部变量1       6. loc += 4; 地址 %ebp-loc，第一个局部变量的地址 (%ebp-4)
//      保存ebp         5. 进入被调函数，g(0x55): push %ebp; g(0xe589): mov %esp,%ebp; loc = 0; a = ga(0xec81, 0): sub $xx,%esp; 进入函数后，ebp 指向这里 <--- ebp，第一个参数的地址 (%ebp+8)
//      函数返回地址     4. ga(0x2494ff, l): call *disp32(%esp), 函数绝对地址保存在 (%esp + l) 的位置，这条指令执行后，esp指向这里被压入的函数返回地址; l += 4 相当于将被调函数地址也回退掉
//      参数1           1. ga(0x248489, l): mov %eax,disp32(%esp); l += 4;  <--- esp 步骤0执行后esp指向这里，函数返回后esp会重新指向这里，会执行 ga(0xc481, l): add $imm32,%esp 回退到被调函数地址之前
//      参数2           2. ga(0x248489, l): mov %eax,disp32(%esp); l += 4;
//      参数3           3. ga(0x248489, l): mov %eax,disp32(%esp); l += 4; *a = l;
//      被调函数地址     0. g(0x50): push %eax; a = ga(0xec81, 0): sub %xx,%esp; l = 0;
// 相对地址调用
//      函数返回地址     4. *useadr = ga(0xe8, useadr): call rel32, 最后函数地址确认后，在所有useadr地址中写入useadr相对函数地址的相对地址，这条指令执行后，esp指向这里被压入的函数返回地址;
//      参数1           1. ga(0x248489, l): mov %eax,disp32(%esp); l += 4;  <--- esp 步骤0执行后esp指向这里，函数返回后esp会重新指向这里，会执行 ga(0xc481, l): add $imm32,%esp 回退到栈顶
//      参数2           2. ga(0x248489, l): mov %eax,disp32(%esp); l += 4;
//      参数3           3. ga(0x248489, l): mov %eax,disp32(%esp); l += 4; *a = l;
//      栈顶            0. a = ga(0xec81, 0): sub %xx,%esp; l = 0;
//
// 协程栈函数的设计：
//
// 主调函数：----------------
//          ...                                                                 // 被调函数视角的地址
//          函数返回地址        <-- 主调函数edx                                   // (%ebp) %ebp保存主调函数的edx
//          返回值地址          +4                                               // (%ebp+4) 返回值的地址
//          参数1               +8                                              // (%ebp+8) 第一个参数位置
//          参数2               +12                                             // (%ebp+12)
// 被调函数：保存ebp             loc = pl+8; mov %ebp,(%edx+loc); mov %edx,%ebp  // (%ebp+p+0) p=16
//          预留edx保存空间      loc += 4;                                       // (%ebp+p+4) 用于保护本函数的edx
//          局部变量1           loc += 4;                                        // (%ebp+p+8) 第一个局部变量位置
//          局部变量2           loc += 4;                                        // (%ebp+p+12)
//          待写返回地址        add maxloc,%edx <-- 被调函数edx                   // <-- edx
//          被调函数返回        mov %ebp,%edx; mov (%edx+p),%ebp; jmp (%edx);
//
// 如果需要支持动态大小的栈局部变量，需要动态调整 %edx 寄存器的值，如果不支持那么 %edx
// 的值一直保持固定。函数设计都在协程中执行，协程拥有自己独立的程序栈，协程栈在32/64位机
// 器上分别以4/8字节为单位和对齐。变长参数传参总是保持edx到被调函数开始的大小是固定的，
// 变长的参数保存在主调函数返回地址之前，被调函数只负责回退到函数返回地址，其余的回退由
// 主调函数自己负责。
//
// 地址重定位涉及两个分区，一个分区是该地址所在分区，也即该分区中的这个地址内容需要重定
// 位为实际的地址，一般这个分区是代码分区，第二个分区是符号分区，也即该地址对应的符号的
// 分区，需要重定位的就是这个符号的实际地址。重定位需要知道：
// 1. 符号在符号分区的索引
// 2. 代码分区的文件偏移
// 3. 地址所在位置的分区偏移
// 4. 地址是一个绝对地址还是一个相对于程序计数器的相对地址，即重定位的类型
//
// 程序加载会将文件分段映射到内存分段，文件分段的地址使用的是文件偏移地址，内存分段的地址
// 使用的是内存虚拟地址，内存虚拟地址会以内存分页为单位最终映射到实际的物理地址。内存分段
// 会对齐到内存分页的整数倍位置，这会导致内存分段之前和之后可能有空白填补空间，例如代码
// 分段之后的数据分区，其开始部分可能是填补的空白空间。

void gi(chcc_t *cc, uint32 imm32) // 加载一个立即数
{
    ga(cc, 0xb8, (byte *)imm32); // mov $imm32, %eax
}

byte *gjmp(chcc_t *cc, byte *addr)
{
    // [e9] jmp rel32 相对下一条指令的近跳转
    return ga(cc, 0xe9, addr);
}

byte *gjcc(chcc_t *cc, bool jmp_when_true, byte *addr)
{
    // test %eax,%eax   [85 c0] 11 000 000
    // je rel32         [0F 84 rel32] 当ZF=1时跳转
    // jne rel32        [0F 85 rel32] 当ZF=0时跳转
    g(cc, 0x0fc085); // 跳转地址rel32是相对下一条指令的相对近跳转
    return ga(cc, 0x84 + jmp_when_true, addr);
}

void gcmp(byte cc) // 如果条件成立将%eax置1或置0
{
    g(0xc139);  // cmp %eax,%ecx    [39 c1] 11 000 001
    gi(0);      // mov $0,%eax
    g(0x0f);    // setcc %al        [0f 90 c0] 11 000 000
    g(0x90 + cc);
    g(0xc0);
}

void gmov(uint32 val, uint32 addr)
{
    var32_t *v = (var32_t *)addr;
    int32 n = v->v;
    // val = 0  [83]    add imm8 => r/m32 符号扩展
    // val = 6  [89]    mov r32 => r/m32
    // val = 8  [8B]    mov r/m32 => r32
    // val = 10 [8D]    lea EA => r32
    g(0x83 + val);
    if (n && n < LOCAL) { // 定义的局部变量，相对%ebp的地址
        // [83] 10 000 101 disp32   add $imm8,disp32(%ebp)
        // [89] 10 000 101 disp32   mov %eax,disp32(%ebp)
        // [8B] 10 000 101 disp32   mov disp32(%ebp),%eax
        // [8D] 10 000 101 disp32   lea disp32(%ebp),%eax
        ga(0x85, (byte *)n);
    } else { // 未定义的或定义的全局变量，使用绝对地址
        // [83] 00 000 101 disp32   add $imm8,(disp32)
        // [89] 00 000 101 disp32   mov %eax,(disp32)
        // [8B] 00 000 101 disp32   mov (disp32),%eax
        // [8D] 00 000 101 disp32   lea (disp32),%eax
        v->addr = (uint32)ga(cc, 0x05, v->addr);
    }
}


void grel(byte *cur, intv_t *usel)
{
    byte *a;
    while (usel) {
        a = (byte *)lp_ab_to_host(usel);
        if (*(a - 1) == 0x05) { // 绝对地址
            if (cur >= data && cur < glo) { // 数据区
                host_ab_to_lp(cur + vaddr_offset, a);
            } else { // 代码区
                host_ab_to_lp((cur - prog + text) + vaddr_offset, a);
            }
        } else { // 相对地址
            host_ab_to_lp(cur - (a + 4), a);
        }
        usel = (intv_t *)a;
    }
}

void gload(vsym_t *a, uint32 reg) // m32 => r32
{

}

void gstore(uint32 reg, vsym_t *a) // r32 => m32
{

}

uint32 genter(chcc_t *cc, fsym_t *f)
{
    // %edx:010
    // %ebp:101
    uint32 loc = f->plen;
    // 1. 保存ebp
    // mov %ebp,(%edx+loc)
    // r32 => m32 [89] 01 101 010 disp8     [89 6a xx]
    //            [89] 10 101 010 disp32    [89 aa xx xx xx xx]
    if (loc < 128) {
        g(cc, (loc<<16)|0x6a89);
    } else {
        ga(cc, 0xaa89, (byte *)loc);
    }
    // 2. 将 %edx 赋给 %ebp
    // mov %edx,%ebp
    // r32 => r32 [89] 11 010 101           [89 d5]
    g(cc, 0xd589);
    // 3. 预留edx保存空间，初始化loc和radr
    f->loc = loc + sizeof(uintv_t);
    f->radr = null;
    return f->loc;
}

void gret(chcc_t *cc, fsym_t *f)
{
    uint32 loc = f->plen;
    // 0. 重定位return语句的跳转地址
    grel(cc->text, f->radr);
    // 1. 将 %ebp 恢复到 %edx
    // mov %ebp,%edx
    // r32 => r32 [89] 11 101 010           [89 ea]
    g(cc, 0xea89);
    // 2. 恢复%ebp
    // mov (%edx+loc),%ebp
    // m32 => r32 [8b] 01 101 010 disp8     [8b 6a xx]
    //            [8b] 10 101 010 disp32    [8b aa xx xx xx xx]
    if (loc < 128) {
        g(cc, (loc<<16)|0x6a8b);
    } else {
        ga(cc, 0xaa8b, (byte *)loc);
    }
    // 3. 跳转到函数返回地址
    // jmp (%edx) FF /4 绝对近跳转，间接地址为 r/m32 零扩展
    // [ff] 00 100 010                      [ff 22]
    g(cc, 0x22ff);
}

typedef struct {
    byte *shnum;        // uint16 shnum 分区个数>=0xff00，该值0x0000
    byte *strsh;        // uint16 strsh 头部索引>=0xff00，该值0xffff
    byte *real_shnum;   // uint32 size
    byte *real_strsh;   // uint32 link
} objhdr_t;

byte *gelfobjhdr(byte *p, objhdr_t *out)
{
    // 文件头部
    // 0x7f 'E' 'L' 'F'
    p = host_32_to_lp(p, 0x464c457f);
    // 0x01 ELF_CLASS_32            32-bit
    // 0x01 ELF_DATA_2LSB           小端模式
    // 0x01 ELF_VERSION_CURR        当前版本
    // 0x00 ELF_OSABI_NONE          无操作系统ABI
    p = host_32_to_lp(p, 0x00010101);
    // 0x00 ELF_ABIVERSION_NONE     无ABI版本
    // 0x00 ELF_HDR_PAD 3-byte
    *(uint32 *)p = 0x00000000; p += 4;
    // 0x00 ELF_HDR_PAD 4-byte
    *(uint32 *)p = 0x00000000; p += 4;
    // 0x01 0x00 ELF_TYPE_REL       type    可重定位文件
    // 0x03 0x00 ELF_MACHINE_386    machine x86-32
    p = host_32_to_lp(p, 0x00030001);
    // 0x01 0x00 0x00 0x00          version ELF_VERSION_CURR
    p = host_32_to_lp(p, 0x00000001);
    // 0x00 0x00 0x00 0x00          entry
    *(uint32 *)p = 0x00000000; p += 4;
    // 0x00 0x00 0x00 0x00          phoff
    *(uint32 *)p = 0x00000000; p += 4;
    // 0x34 0x00 0x00 0x00          shoff   sizeof(Elf32Ehdr)
    p = host_32_to_lp(p, 0x00000034);
    // 0x00 0x00 0x00 0x00          flags
    *(uint32 *)p = 0x00000000; p += 4;
    // 0x34 0x00 (52)               ehsize  sizeof(Elf32Ehdr)
    // 0x00 0x00                    phsize
    p = host_32_to_lp(p, 0x00000034);
    // 0x00 0x00                    phnum
    // 0x28 0x00 (40)               shsize  sizeof(Elf32Shdr)
    p = host_32_to_lp(p, 0x00280000);
    // 0x00 0x00                    shnum   分区个数，如果个数>=0xff00，该值0x0000
    // 0x00 0x00                    strsh   分区名称使用的字符串表头部索引，如果索引>=0xff00，该值0xffff
    out->shnum = p;
    out->strsh = p + 2;
    // 特殊的第一个分区头部
    // 0x00 0x00 0x00 0x00          name    ELF_SHNDX_UNDEF 没有名称
    // 0x00 0x00 0x00 0x00          type    ELF_SEC_NULL 非活动分区头部
    // 0x00 0x00 0x00 0x00          flags   没有分区属性
    // 0x00 0x00 0x00 0x00          addr    没有内存映射地址
    // 0x00 0x00 0x00 0x00          offset  没有文件空间偏移
    // 0x00 0x00 0x00 0x00          size    非零表示实际的分区个数
    // 0x00 0x00 0x00 0x00          link    非零表示实际的字符串表头部索引
    // 0x00 0x00 0x00 0x00          info    没有额外信息
    // 0x00 0x00 0x00 0x00          addralign 无地址对齐要求
    // 0x00 0x00 0x00 0x00          entsize 不包含固定大小条目
    memset(p, 0, 4 + sizeof(Elf32Shdr));
    out->real_shnum = p + 4 + 20;   // size
    out->real_strsh = p + 4 + 24;   // link
    return p + 4 + sizeof(Elf32Shdr);
}

typedef struct {
    byte *name;     // uint32 name
    byte *offset;   // uint32 offset, offset-4 为 addr 位置
    byte *size;     // uint32 size
} sechdr_t;

byte *gelfsechdr(byte *p, sechdr_t *out, uint32 type, uint32 attr)
{
    out->name = p;
    *(uint32 *)p = 0x00000000; p += 4;  // name
    p = host_32_to_lp(p, type);         // type
    p = host_32_to_lp(p, attr);         // flags
    *(uint32 *)p = 0x00000000; p += 4;  // addr
    out->offset = p; p += 4;            // offset
    out->size = p; p += 4;              // size
    *(uint32 *)p = 0x00000000; p += 4;  // link
    *(uint32 *)p = 0x00000000; p += 4;  // info
    p = host_32_to_lp(p, 0x00000004);   // addralign
    *(uint32 *)p = 0x00000000;          // entsize
    return p + 4;
}

byte *gelfsnmstr(byte *p, sechdr_t *out) // 分区名称
{
    return gelfsechdr(p, out, ELF_SEC_STRTAB, 0);
}

byte *gelfsymstr(byte *p, sechdr_t *out) // 符号名称
{
    return gelfsechdr(p, out, ELF_SEC_STRTAB, 0);
}

byte *gelfsymtab(byte *p, sechdr_t *out) // 符号表分区，只能有一个
{
    p = gelfsechdr(p, out, ELF_SEC_SYMTAB, 0);
    host_32_to_lp(sizeof(Elf32Sym), p - 4); // entsize
    return p;
}

byte *gelfrel(byte *p, sechdr_t *out) // 代码分区重定位
{
    p = gelfsechdr(p, out, ELF_SEC_REL, 0);
    host_32_to_lp(sizeof(Elf32Rel), p - 4); // entsize
    return p;
}

byte *gelftext(byte *p, sechdr_t *out) // 程序代码
{
    return gelfsechdr(p, out, ELF_SEC_PROGBITS, ELF_SF_ALLOC|ELF_SF_EXECINSTR);
}

byte *gelfrodata(byte *p, sechdr_t *out) // 只读数据（非字符串）
{
    return gelfsechdr(p, out, ELF_SEC_PROGBITS, ELF_SF_ALLOC);
}

byte *gelfrodstr(byte *p, sechdr_t *out) // 只读字符串数据
{
    return gelfsechdr(p, out, ELF_SEC_PROGBITS, ELF_SF_ALLOC);
}

byte *gelfdata(byte *p, sechdr_t *out) // 全局初始化数据
{
    return gelfsechdr(p, out, ELF_SEC_PROGBITS, ELF_SF_ALLOC|ELF_SF_WRITE);
}

byte *gelfbss(byte *p, sechdr_t *out) // 全局未初始化数据，不占用文件空洞，但会分配内存
{
    return gelfsechdr(p, out, ELF_SEC_NOBITS, ELF_SF_ALLOC|ELF_SF_WRITE);
}

typedef struct {
    sechdr_t symstr;
    sechdr_t symtab;
    sechdr_t rel;
    sechdr_t text;
    sechdr_t rodata;
    sechdr_t rodstr;
    sechdr_t data;
    sechdr_t bss;
} objfile_t;

byte *gobjhdr(byte *p, objfile_t *obj)
{
    objhdr_t objhdr;
    p = gelfobjhdr(p, &objhdr);         // 文件头部
    p = gelfsymstr(p, &obj->symstr);    // 分区头部
    p = gelfsymtab(p, &obj->symtab);
    p = gelfrel(p, &obj->rel);
    p = gelftext(p, &obj->text);
    p = gelfrodata(p, &obj->rodata);
    p = gelfrodstr(p, &obj->rodstr);
    p = gelfdata(p, &obj->data);
    p = gelfbss(p, &obj->bss);
    host_16_to_lp(1 + sizeof(objfile_t)/sizeof(sechdr_t), (byte *)objhdr.shnum);
    return p;
}
