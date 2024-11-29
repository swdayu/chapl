#ifndef CHAPL_CHCC_GELF_H
#define CHAPL_CHCC_GELF_H
#include "builtin/decl.h"

typedef uint32 Elf32Addr;   // 32位平台无符号程序地址
typedef uint32 Elf32Off;    // 32位平台无符号文件偏移
typedef uint64 Elf64Addr;   // 64位平台无符号程序地址
typedef uint64 Elf64Off;    // 64位平台无符号文件偏移
typedef Uint ElfAddr;       // 当前平台无符号程序地址
typedef Uint ElfOff;        // 当前平台无符号文件偏移
typedef uint16 ElfHalf;     // 无符号半整型
typedef uint32 ElfWord;     // 无符号整型
typedef int32 ElfSword;     // 有符号整型
typedef uint64 ElfXword;    // 无符号长整型
typedef int64 ElfSxword;    // 有符号长整型
typedef byte ElfByte;       // 无符号字节类型

// 文件头部16字节标识信息
// EI_MAG0~EI_MAG3 0~3 文件起始信息
// EI_CLASS        4   文件使用的数据类型
// EI_DATA         5   数据字节序
// EI_VERSION      6   文件版本
// EI_OSABI        7   使用的补充规范（Operating system/ABI identification）
// EI_ABIVERSION   8   补充规范的版本（ABI version），依赖于补充规范的定义，如果没有版本信息应该设置为 0
// EI_PAD          9   未使用字节的起始位置，这些是预留字节要设为全零
// EI_NIDENT       16  文件标识信息总大小

// 文件起始信息
#define ELF_IDENT_MAG0 0
#define ELF_IDENT_MAG1 1
#define ELF_IDENT_MAG2 2
#define ELF_IDENT_MAG3 3
#define ELF_HDR_MAG ELF_MAG0, ELF_MAG1, ELF_MAG2, ELF_MAG3
#define ELF_MAG0 0x7f
#define ELF_MAG1 'E'
#define ELF_MAG2 'L'
#define ELF_MAG3 'F'
// 使用的数据类型
#define ELF_IDENT_CLASS 4
#define ELF_CLASS_NONE 0    // 非法数据类型
#define ELF_CLASS_32 1      // 32位数据类型
#define ELF_CLASS_64 2      // 64位数据类型
// 数据的字节序，为了解析效率，字节序尽量与目标平台字节序保持一致
#define ELF_IDENT_DATA 5
#define ELF_DATA_NONE 0     // 非法字节序
#define ELF_DATA_2LSB 1     // 二进制补码低字节位于低地址（小端字节序）
#define ELF_DATA_2MSB 2     // 二进制补码高字节位于低地址（大端字节序）
// ELF 文件格式版本
#define ELF_IDENT_VERSION 6
#define ELF_VERSION_NONE 0  // 非法版本
#define ELF_VERSION_CURR 1  // 当前版本
// 指定 ELF 使用的特定平台扩展，处理器补充文档（psABI）可以定义 64~255
// 范围内的自己特定的值，如果 psABI 没有定义自己的值，应该使用下面这些预
// 定义的值，如果设置成 0 则表示没有使用任何扩展。如果设置的值在 64~255
// 范围内，它的含义依赖于 ELF 头部信息中的 machine 的解释。
#define ELF_IDENT_OSABI 7
#define ELF_OSABI_NONE 0        // 未指定或没有使用扩展
#define ELF_OSABI_HPUX 1        // Hewlett-Packard HP-UX
#define ELF_OSABI_NETBSD 2      // NetBSD
#define ELF_OSABI_GNU 3         // GNU
#define ELF_OSABI_LINUX 3       // 历史原因，它是 ELF_OSABI_GNU 的别名
#define ELF_OSABI_SOLARIS 6     // Sun Solaris
#define ELF_OSABI_AIX 7         // AIX
#define ELF_OSABI_IRIX 8        // IRIX
#define ELF_OSABI_FREEBSD 9     // FreeBSD
#define ELF_OSABI_TRU64 10      // Compaq TRU64 UNIX
#define ELF_OSABI_MODESTO 11    // Novell Modesto
#define ELF_OSABI_OPENBSD 12    // Open BSD
#define ELF_OSABI_OPENVMS 13    // Open VMS
#define ELF_OSABI_NSK 14        // Hewlett-Packard Non-Stop Kernel
#define ELF_OSABI_AROS 15       // Amiga Research OS
#define ELF_OSABI_FENIXOS 16    // The FenixOS highly scalable multi-core OS
#define ELF_OSABI_CLOUDABI 17   // Nuxi CloudABI
#define ELF_OSABI_OPENVOS 18    // Stratus Technologies OpenVOS
#define ELF_OSABI_ARCHSPEC 64   // 64~255 平台特定值
// 平台扩展的版本，依赖于 psABI 的定义，如果没有版本信息应该设置为0
#define ELF_IDENT_ABIVERSION 8
#define ELF_ABIVERSION_NONE 0
// 未使用字节的起始位置，这些是预留字节要设为全零
#define ELF_IDENT_PAD 9
#define ELF_HDR_PAD 0, 0, 0, 0, 0, 0, 0
// ELF 文件标识信息的大小
#define ELF_IDENT_SIZE 16

// 目标文件类型
#define ELF_TYPE_NONE 0         // 未知
#define ELF_TYPE_REL 1          // 可重定位文件
#define ELF_TYPE_EXEC 2         // 可执行文件
#define ELF_TYPE_DYN 3          // 共享目标文件
#define ELF_TYPE_CORE 4         // 核心文件，当前没有定义该类型的文件内容
#define ELF_TYPE_LOOS 0xfe00    // 特定操作系统
#define ELF_TYPE_HIOS 0xfeff    // 特定操作系统
#define ELF_TYPE_LOPROC 0xff00  // 特定处理器
#define ELF_TYPE_HIPROC 0xffff  // 特定处理器

// 处理器信息，处理器特定的 ELF 名字使用处理器名称来区分，例如处理器标记 ELF_FLAG_，
// 名为 WIDGET 的标记相对于处理器 ELF_MACHINE_XYZ 的名称为 ELF_FLAG_XYZ_WIDGET。
#define ELF_MACHINE_NONE 0      // 未知
#define ELF_MACHINE_M32 1       // AT&T WE 32100
#define ELF_MACHINE_SPARC 2     // SPARC
#define ELF_MACHINE_386 3       // Intel 80386
#define ELF_MACHINE_68K 4       // Motorola 68000
#define ELF_MACHINE_88K 5       // Motorola 88000
#define ELF_MACHINE_IAMCU 6     // Intel MCU
#define ELF_MACHINE_860 7       // Intel 80860
#define ELF_MACHINE_MIPS 8      // MIPS I Architecture
#define ELF_MACHINE_S370 9      // IBM System/370 Processor
#define ELF_MACHINE_MIPS_RS3_LE 10 // MIPS RS3000 Little-endian
#define ELF_MACHINE_RFU1 11     // 11-14 Reserved for future use
#define ELF_MACHINE_PARISC 15   // Hewlett-Packard PA-RISC
#define ELF_MACHINE_RFU2 16     // Reserved for future use
#define ELF_MACHINE_VPP500 17   // Fujitsu VPP500
#define ELF_MACHINE_SPARC32PLUS 18 // Enhanced instruction set SPARC
#define ELF_MACHINE_960 19      // Intel 80960
#define ELF_MACHINE_PPC 20      // PowerPC
#define ELF_MACHINE_PPC64 21    // 64-bit PowerPC
#define ELF_MACHINE_S390 22     // IBM System/390 Processor
#define ELF_MACHINE_SPU 23      // IBM SPU/SPC
#define ELF_MACHINE_RFU3 24     // 24-35 Reserved for future use
#define ELF_MACHINE_V800 36     // NEC V800
#define ELF_MACHINE_FR20 37     // Fujitsu FR20
#define ELF_MACHINE_RH32 38     // TRW RH-32
#define ELF_MACHINE_RCE 39      // Motorola RCE
#define ELF_MACHINE_ARM 40      // ARM 32-bit architecture (AARCH32)
#define ELF_MACHINE_ALPHA 41    // Digital Alpha
#define ELF_MACHINE_SH 42       // Hitachi SH
#define ELF_MACHINE_SPARCV9 43  // SPARC Version 9
#define ELF_MACHINE_TRICORE 44  // Siemens TriCore embedded processor
#define ELF_MACHINE_ARC 45      // Argonaut RISC Core, Argonaut Technologies Inc.
#define ELF_MACHINE_H8_300 46   // Hitachi H8/300
#define ELF_MACHINE_H8_300H 47  // Hitachi H8/300H
#define ELF_MACHINE_H8S 48      // Hitachi H8S
#define ELF_MACHINE_H8_500 49   // Hitachi H8/500
#define ELF_MACHINE_IA_64 50    // Intel IA-64 processor architecture
#define ELF_MACHINE_MIPS_X 51   // Stanford MIPS-X
#define ELF_MACHINE_COLDFIRE 52 // Motorola ColdFire
#define ELF_MACHINE_68HC12 53   // Motorola M68HC12
#define ELF_MACHINE_MMA 54      // Fujitsu MMA Multimedia Accelerator
#define ELF_MACHINE_PCP 55      // Siemens PCP
#define ELF_MACHINE_NCPU 56     // Sony nCPU embedded RISC processor
#define ELF_MACHINE_NDR1 57     // Denso NDR1 microprocessor
#define ELF_MACHINE_STARCORE 58 // Motorola Star*Core processor
#define ELF_MACHINE_ME16 59     // Toyota ME16 processor
#define ELF_MACHINE_ST100 60    // STMicroelectronics ST100 processor
#define ELF_MACHINE_TINYJ 61    // Advanced Logic Corp. TinyJ embedded processor family
#define ELF_MACHINE_X86_64 62   // AMD x86-64 architecture
#define ELF_MACHINE_PDSP 63     // Sony DSP Processor
#define ELF_MACHINE_PDP10 64    // Digital Equipment Corp. PDP-10
#define ELF_MACHINE_PDP11 65    // Digital Equipment Corp. PDP-11
#define ELF_MACHINE_FX66 66     // Siemens FX66 microcontroller
#define ELF_MACHINE_ST9PLUS 67  // STMicroelectronics ST9+ 8/16 bit microcontroller
#define ELF_MACHINE_ST7 68      // STMicroelectronics ST7 8-bit microcontroller
#define ELF_MACHINE_68HC16 69   // Motorola MC68HC16 Microcontroller
#define ELF_MACHINE_68HC11 70   // Motorola MC68HC11 Microcontroller
#define ELF_MACHINE_68HC08 71   // Motorola MC68HC08 Microcontroller
#define ELF_MACHINE_68HC05 72   // Motorola MC68HC05 Microcontroller
#define ELF_MACHINE_SVX 73      // Silicon Graphics SVx
#define ELF_MACHINE_ST19 74     // STMicroelectronics ST19 8-bit microcontroller
#define ELF_MACHINE_VAX 75      // Digital VAX
#define ELF_MACHINE_CRIS 76     // Axis Communications 32-bit embedded processor
#define ELF_MACHINE_JAVELIN 77  // Infineon Technologies 32-bit embedded processor
#define ELF_MACHINE_FIREPATH 78 // Element 14 64-bit DSP Processor
#define ELF_MACHINE_ZSP 79      // LSI Logic 16-bit DSP Processor
#define ELF_MACHINE_MMIX 80     // Donald Knuth's educational 64-bit processor
#define ELF_MACHINE_HUANY 81    // Harvard University machine-independent object files
#define ELF_MACHINE_PRISM 82    // SiTera Prism
#define ELF_MACHINE_AVR 83      // Atmel AVR 8-bit microcontroller
#define ELF_MACHINE_FR30 84     // Fujitsu FR30
#define ELF_MACHINE_D10V 85     // Mitsubishi D10V
#define ELF_MACHINE_D30V 86     // Mitsubishi D30V
#define ELF_MACHINE_V850 87     // NEC v850
#define ELF_MACHINE_M32R 88     // Mitsubishi M32R
#define ELF_MACHINE_MN10300 89  // Matsushita MN10300
#define ELF_MACHINE_MN10200 90  // Matsushita MN10200
#define ELF_MACHINE_PJ 91       // picoJava
#define ELF_MACHINE_OPENRISC 92 // OpenRISC 32-bit embedded processor
#define ELF_MACHINE_ARC_COMPACT 93 // ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5)
#define ELF_MACHINE_XTENSA 94   // Tensilica Xtensa Architecture
#define ELF_MACHINE_VIDEOCORE 95 // Alphamosaic VideoCore processor
#define ELF_MACHINE_TMM_GPP 96  // Thompson Multimedia General Purpose Processor
#define ELF_MACHINE_NS32K 97    // National Semiconductor 32000 series
#define ELF_MACHINE_TPC 98      // Tenor Network TPC processor
#define ELF_MACHINE_SNP1K 99    // Trebia SNP 1000 processor
#define ELF_MACHINE_ST200 100   // STMicroelectronics (www.st.com) ST200 microcontroller
#define ELF_MACHINE_IP2K 101    // Ubicom IP2xxx microcontroller family
#define ELF_MACHINE_MAX 102     // MAX Processor
#define ELF_MACHINE_CR 103      // National Semiconductor CompactRISC microprocessor
#define ELF_MACHINE_F2MC16 104  // Fujitsu F2MC16
#define ELF_MACHINE_MSP430 105  // Texas Instruments embedded microcontroller msp430
#define ELF_MACHINE_BLACKFIN 106 // Analog Devices Blackfin (DSP) processor
#define ELF_MACHINE_SE_C33 107  // S1C33 Family of Seiko Epson processors
#define ELF_MACHINE_SEP 108     // Sharp embedded microprocessor
#define ELF_MACHINE_ARCA 109    // Arca RISC Microprocessor
#define ELF_MACHINE_UNICORE 110 // Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University
#define ELF_MACHINE_EXCESS 111  // eXcess: 16/32/64-bit configurable embedded CPU
#define ELF_MACHINE_DXP 112     // Icera Semiconductor Inc. Deep Execution Processor
#define ELF_MACHINE_ALTERA_NIOS2 113 // Altera Nios II soft-core processor
#define ELF_MACHINE_CRX 114     // National Semiconductor CompactRISC CRX microprocessor
#define ELF_MACHINE_XGATE 115   // Motorola XGATE embedded processor
#define ELF_MACHINE_C166 116    // Infineon C16x/XC16x processor
#define ELF_MACHINE_M16C 117    // Renesas M16C series microprocessors
#define ELF_MACHINE_DSPIC30F 118 // Microchip Technology dsPIC30F Digital Signal Controller
#define ELF_MACHINE_CE 119      // Freescale Communication Engine RISC core
#define ELF_MACHINE_M32C 120    // Renesas M32C series microprocessors
#define ELF_MACHINE_RFU4 121    // 121-130 Reserved for future use
#define ELF_MACHINE_TSK3000 131 // Altium TSK3000 core
#define ELF_MACHINE_RS08 132    // Freescale RS08 embedded processor
#define ELF_MACHINE_SHARC 133   // Analog Devices SHARC family of 32-bit DSP processors
#define ELF_MACHINE_ECOG2 134   // Cyan Technology eCOG2 microprocessor
#define ELF_MACHINE_SCORE7 135  // Sunplus S+core7 RISC processor
#define ELF_MACHINE_DSP24 136   // New Japan Radio (NJR) 24-bit DSP Processor
#define ELF_MACHINE_VIDEOCORE3 137 // Broadcom VideoCore III processor
#define ELF_MACHINE_LATTICEMICO32 138 // RISC processor for Lattice FPGA architecture
#define ELF_MACHINE_SE_C17 139  // Seiko Epson C17 family
#define ELF_MACHINE_TI_C6000 140 // The Texas Instruments TMS320C6000 DSP family
#define ELF_MACHINE_TI_C2000 141 // The Texas Instruments TMS320C2000 DSP family
#define ELF_MACHINE_TI_C5500 142 // The Texas Instruments TMS320C55x DSP family
#define ELF_MACHINE_TI_ARP32 143 // Texas Instruments Application Specific RISC Processor, 32bit fetch
#define ELF_MACHINE_TI_PRU 144  // Texas Instruments Programmable Realtime Unit
#define ELF_MACHINE_RFU5 145    // 145-159 Reserved for future use
#define ELF_MACHINE_MMDSP_PLUS 160 // STMicroelectronics 64bit VLIW Data Signal Processor
#define ELF_MACHINE_CYPRESS_M8C 161 // Cypress M8C microprocessor
#define ELF_MACHINE_R32C 162    // Renesas R32C series microprocessors
#define ELF_MACHINE_TRIMEDIA 163 // NXP Semiconductors TriMedia architecture family
#define ELF_MACHINE_QDSP6 164   // QUALCOMM DSP6 Processor
#define ELF_MACHINE_8051 165    // Intel 8051 and variants
#define ELF_MACHINE_STXP7X 166  // STMicroelectronics STxP7x family of configurable and extensible RISC processors
#define ELF_MACHINE_NDS32 167   // Andes Technology compact code size embedded RISC processor family
#define ELF_MACHINE_ECOG1 168   // Cyan Technology eCOG1X family
#define ELF_MACHINE_ECOG1X 168  // Cyan Technology eCOG1X family
#define ELF_MACHINE_MAXQ30 169  // Dallas Semiconductor MAXQ30 Core Micro-controllers
#define ELF_MACHINE_XIMO16 170  // New Japan Radio (NJR) 16-bit DSP Processor
#define ELF_MACHINE_MANIK 171   // M2000 Reconfigurable RISC Microprocessor
#define ELF_MACHINE_CRAYNV2 172 // Cray Inc. NV2 vector architecture
#define ELF_MACHINE_RX 173      // Renesas RX family
#define ELF_MACHINE_METAG 174   // Imagination Technologies META processor architecture
#define ELF_MACHINE_MCST_ELBRUS 175 // MCST Elbrus general purpose hardware architecture
#define ELF_MACHINE_ECOG16 176  // Cyan Technology eCOG16 family
#define ELF_MACHINE_CR16 177    // National Semiconductor CompactRISC CR16 16-bit microprocessor
#define ELF_MACHINE_ETPU 178    // Freescale Extended Time Processing Unit
#define ELF_MACHINE_SLE9X 179   // Infineon Technologies SLE9X core
#define ELF_MACHINE_L10M 180    // Intel L10M
#define ELF_MACHINE_K10M 181    // Intel K10M
#define ELF_MACHINE_RFU6 182    // Reserved for future Intel use
#define ELF_MACHINE_AARCH64 183 // ARM 64-bit architecture (AARCH64)
#define ELF_MACHINE_RFU7 184    // Reserved for future ARM use
#define ELF_MACHINE_AVR32 185   // Atmel Corporation 32-bit microprocessor family
#define ELF_MACHINE_STM8 186    // STMicroeletronics STM8 8-bit microcontroller
#define ELF_MACHINE_TILE64 187  // Tilera TILE64 multicore architecture family
#define ELF_MACHINE_TILEPRO 188 // Tilera TILEPro multicore architecture family
#define ELF_MACHINE_MICROBLAZE 189 // Xilinx MicroBlaze 32-bit RISC soft processor core
#define ELF_MACHINE_CUDA 190    // NVIDIA CUDA architecture
#define ELF_MACHINE_TILEGX 191  // Tilera TILE-Gx multicore architecture family
#define ELF_MACHINE_CLOUDSHIELD 192 // CloudShield architecture family
#define ELF_MACHINE_COREA_1ST 193 // KIPO-KAIST Core-A 1st generation processor family
#define ELF_MACHINE_COREA_2ND 194 // KIPO-KAIST Core-A 2nd generation processor family
#define ELF_MACHINE_ARC_COMPACT2 195 // Synopsys ARCompact V2
#define ELF_MACHINE_OPEN8 196   // Open8 8-bit RISC soft processor core
#define ELF_MACHINE_RL78 197    // Renesas RL78 family
#define ELF_MACHINE_VIDEOCORE5 198 // Broadcom VideoCore V processor
#define ELF_MACHINE_78KOR 199   // Renesas 78KOR family
#define ELF_MACHINE_56800EX 200 // Freescale 56800EX Digital Signal Controller (DSC)
#define ELF_MACHINE_BA1 201     // Beyond BA1 CPU architecture
#define ELF_MACHINE_BA2 202     // Beyond BA2 CPU architecture
#define ELF_MACHINE_XCORE 203   // XMOS xCORE processor family
#define ELF_MACHINE_MCHP_PIC 204 // Microchip 8-bit PIC(r) family
#define ELF_MACHINE_INTEL205 205 // Reserved by Intel
#define ELF_MACHINE_INTEL206 206 // Reserved by Intel
#define ELF_MACHINE_INTEL207 207 // Reserved by Intel
#define ELF_MACHINE_INTEL208 208 // Reserved by Intel
#define ELF_MACHINE_INTEL209 209 // Reserved by Intel
#define ELF_MACHINE_KM32 210    // KM211 KM32 32-bit processor
#define ELF_MACHINE_KMX32 211   // KM211 KMX32 32-bit processor
#define ELF_MACHINE_KMX16 212   // KM211 KMX16 16-bit processor
#define ELF_MACHINE_KMX8 213    // KM211 KMX8 8-bit processor
#define ELF_MACHINE_KVARC 214   // KM211 KVARC processor
#define ELF_MACHINE_CDP 215     // Paneve CDP architecture family
#define ELF_MACHINE_COGE 216    // Cognitive Smart Memory Processor
#define ELF_MACHINE_COOL 217    // Bluechip Systems CoolEngine
#define ELF_MACHINE_NORC 218    // Nanoradio Optimized RISC
#define ELF_MACHINE_CSR_KALIMBA 219 // CSR Kalimba architecture family
#define ELF_MACHINE_Z80 220     // Zilog Z80
#define ELF_MACHINE_VISIUM 221  // Controls and Data Services VISIUMcore processor
#define ELF_MACHINE_FT32 222    // FTDI Chip FT32 high performance 32-bit RISC architecture
#define ELF_MACHINE_MOXIE 223   // Moxie processor family
#define ELF_MACHINE_AMDGPU 224  // AMD GPU architecture
#define ELF_MACHINE_RFU8 225    // 225-242 Reserved for future use
#define ELF_MACHINE_RISCV 243   // RISC-V

// ELF 文件头部结构体，目标文件的一些控制结构的大小可能会发生变化，因为
// 头部信息包含了这些结构的真实大小；如果程序遇到一个控制结构的大小比预
// 期的更大或更小，程序可能忽略额外的部分，而对于丢失的部分的处理，依赖
// 于当时的上下文以及扩展的设定（如果使用了扩展）。
typedef struct {
    // ELF 文件标识信息（Elf Identification）
    byte ident[ELF_IDENT_SIZE];
    // 目标文件类型
    uint16 type;
    // 处理器信息
    uint16 machine;
    // ELF 文件格式版本，当前只有一个版本 ELF_VERSION_CURR
    uint32 version;
    // 程序入口的虚拟地址，如果没有入口，该成员为0
    Uint entry;
    // 程序头部表的文件偏移，如果没有程序头部表，该成员为0
    Uint phoff;
    // 分区头部表的文件偏移，如果没有分区头部表，该成员为0
    Uint shoff;
    // 处理器标记
    uint32 flags;
    // ELF 头部大小
    uint16 ehsize;
    // 程序头部的大小，程序头部表由程序头部组成，每个条目的大小相同
    uint16 phsize;
    // 程序头部表条目的个数
    uint16 phnum;
    // 分区头部的大小，分区头部表由分区头部组成，每个条目的大小相同
    uint16 shsize;
    // 分区头部表中分区头部的个数，如果个数大于等于 0xff00，这个成员的
    // 值为0，具体的分区头部的个数位于第一个分区头部的 size 变量中，否
    // 则第一个分区头部的 size 值为 0
    uint16 shnum;
    // 分区名称字符串表位于哪个分区头部，如果没有分区名称字符串表，该成
    // 员的值为 ELF_SHNDX_UNDEF；如果该值大于等于 0xff00，该成员的值
    // 应该设为 0xffff，具体的分区索引值位于第一个分区头部的 link 变量
    // 中，否则第一个分区头部的 link 值为 0
    uint16 strsh;
} ElfEhdr;

typedef struct {
    byte ident[ELF_IDENT_SIZE];
    uint16 type;
    uint16 machine;
    uint32 version;
    uint32 entry;
    uint32 phoff;
    uint32 shoff;
    uint32 flags;
    uint16 ehsize;
    uint16 phsize;
    uint16 phnum;
    uint16 shsize;
    uint16 shnum;
    uint16 strsh;
} Elf32Ehdr;

typedef struct {
    byte ident[ELF_IDENT_SIZE];
    uint16 type;
    uint16 machine;
    uint32 version;
    uint64 entry;
    uint64 phoff;
    uint64 shoff;
    uint32 flags;
    uint16 ehsize;
    uint16 phsize;
    uint16 phnum;
    uint16 shsize;
    uint16 shnum;
    uint16 strsh;
} Elf64Ehdr;

#define ELF_HDR_X86_EXE(entry, phnum) {                                         \
    ELF_HDR_MAG,                            /* 0x7f 0x45 0x4C 0x46 */           \
    ELF_CLASS_32,                           /* 0x01 */                          \
    ELF_DATA_2LSB,                          /* 0x01 */                          \
    ELF_VERSION_CURR,                       /* 0x01 */                          \
    ELF_OSABI_NONE,                         /* 0x00 */                          \
    ELF_ABIVERSION_NONE,                    /* 0x00 */                          \
    ELF_HDR_PAD },                          /* 0x00 7-byte */                   \
    host_to_uint16_le(ELF_TYPE_EXEC),       /* 0x02 0x00            type */     \
    host_to_uint16_le(ELF_MACHINE_386),     /* 0x03 0x00            machine */  \
    host_to_uint32_le(ELF_VERSION_CURR),    /* 0x01 0x00 0x00 0x00  version */  \
    host_to_uint32_le(entry),               /* 0xXX 0xXX 0xXX 0xXX  entry */    \
    host_to_uint32_le(sizeof(Elf32Ehdr)),   /* 0x34 0x00 0x00 0x00  phoff */    \
    host_to_uint32_le(0x00),                /* 0x00 0x00 0x00 0x00  shoff */    \
    host_to_uint32_le(0x00),                /* 0x00 0x00 0x00 0x00  flags */    \
    host_to_uint16_le(sizeof(Elf32Ehdr)),   /* 0x34 0x00 (52)       ehsize */   \
    host_to_uint16_le(sizeof(Elf32Phdr)),   /* 0x20 0x00 (32)       phsize */   \
    host_to_uint16_le(phnum),               /* 0xXX 0xXX            phnum */    \
    host_to_uint16_le(0x00),                /* 0x00 0x00            shsize */   \
    host_to_uint16_le(0x00),                /* 0x00 0x00            shnum */    \
    host_to_uint16_le(0x00)                 /* 0x00 0x00            strsh */

#define ELF_HDR_X64_EXE(entry, phnum) {                                         \
    ELF_HDR_MAG,                            /* 0x7f 0x45 0x4C 0x46 */           \
    ELF_CLASS_64,                           /* 0x02 */                          \
    ELF_DATA_2LSB,                          /* 0x01 */                          \
    ELF_VERSION_CURR,                       /* 0x01 */                          \
    ELF_OSABI_NONE,                         /* 0x00 */                          \
    ELF_ABIVERSION_NONE,                    /* 0x00 */                          \
    ELF_HDR_PAD },                          /* 0x00 7-byte */                   \
    host_to_uint16_le(ELF_TYPE_EXEC),       /* 0x02 0x00            type*/      \
    host_to_uint16_le(ELF_MACHINE_X86_64),  /* 0x3E 0x00 (62)       machine */  \
    host_to_uint32_le(ELF_VERSION_CURR),    /* 0x01 0x00 0x00 0x00  version */  \
    host_to_uint32_le(entry),               /* 0xXX 0xXX 0xXX 0xXX  entry */    \
    host_to_uint32_le(sizeof(Elf64Ehdr)),   /* 0x40 0x00 0x00 0x00  phoff */    \
    host_to_uint32_le(0x00),                /* 0x00 0x00 0x00 0x00  shoff */    \
    host_to_uint32_le(0x00),                /* 0x00 0x00 0x00 0x00  flags */    \
    host_to_uint16_le(sizeof(Elf64Ehdr)),   /* 0x40 0x00 (64)       ehsize */   \
    host_to_uint16_le(sizeof(Elf64Phdr)),   /* 0x38 0x00 (56)       phsize */   \
    host_to_uint16_le(phnum),               /* 0xXX 0xXX            phnum */    \
    host_to_uint16_le(0x00),                /* 0x00 0x00            shsize */   \
    host_to_uint16_le(0x00),                /* 0x00 0x00            shnum */    \
    host_to_uint16_le(0x00)                 /* 0x00 0x00            strsh */

// 分区头部表中分区头部索引的特殊值，预留在 ELF_SHNDX_LORESERVE 和
// ELF_SHNDX_HIRESERVE 之间的索引值不是一个真实的头部索引，分区头部
// 表中不包含这种索引的分区。
#define ELF_SHNDX_UNDEF 0       // 第一个分区头部或未定义分区
#define ELF_SHNDX_LORESERVE 0xff00 // 低位预留
#define ELF_SHNDX_LOPROC 0xff00 // 处理器预留
#define ELF_SHNDX_HIPROC 0xff1f // 处理器预留
#define ELF_SHNDX_LOOS 0xff20   // 操作系统预留
#define ELF_SHNDX_HIOS 0xff3f   // 操作系统预留
#define ELF_SHNDX_ABS 0xfff1    // 指定该索引的符号的值是不受重定位影响的绝对值
#define ELF_SHNDX_COMMON 0xfff2 // 指定该索引的符号是一个通用符号
#define ELF_SHNDX_XINDEX 0xffff // 占位值
#define ELF_SHNDX_HIRESERVE 0xffff // 高位预留

// 第一个分区头部是一个特殊的头部，它包含以下信息：
// ElfShdr.name         0 没有名称
// ElfShdr.type         ELF_SEC_NULL 非活动分区头部
// ElfShdr.flags        0 没有分区属性
// ElfShdr.addr         0 没有分区地址
// ElfShdr.offset       0 没有对应的分区
// ElfShdr.size         如果不是0表示分区头部表中实际的分区头部个数
// ElfShdr.link         如果不是0表示分区头部字符串表的位置
// ElfShdr.info         0 没有额外信息
// ElfShdr.addralign    0 不需要对齐
// ElfShdr.entsize      0 分区不包含固定大小的条目

// 分区类型
#define ELF_SEC_NULL 0         // 非活动分区头部，没有关联的分区
#define ELF_SEC_PROGBITS 1     // 程序分区，包含程序定义的信息
#define ELF_SEC_SYMTAB 2       // 符号表分区，当前每个文件只能包含一个该分区
#define ELF_SEC_STRTAB 3       // 字符串分区，每个文件可以包含多个字符串分区
#define ELF_SEC_RELA 4         // 重定位分区，每个文件可以包含多个重定位分区，重定位分区由重定位条目组成
#define ELF_SEC_HASH 5         // 哈希表分区，包含符号哈希表，当前每个文件只能包含一个哈希表分区
#define ELF_SEC_DYNAMIC 6      // 动态链接分区，包含动态链接信息，只能包含一个动态链接分区
#define ELF_SEC_NOTE 7         // 说明分区，包含文件的说明注释
#define ELF_SEC_NOBITS 8       // 该分区不实际占用文件空间，但类似于程序分区；可以拥有非零的分区偏移值，表示概念上的文件偏移
#define ELF_SEC_REL 9          // 重定位分区，包含重定位条目，但是条目内容不包含显式的附加值，每个文件可以包含多个重定位分区
#define ELF_SEC_SHLIB 10       // 预留分区，当前没有指定具体含义
#define ELF_SEC_DYNSYM 11      // 符号表分区，当前每个文件只能包含一个该分区，仅包含最小动态链接符号集
#define ELF_SEC_INIT_ARRAY 14  // 该分区包含初始函数指针，每个函数不带参数也没有返回值
#define ELF_SEC_FINI_ARRAY 15  // 该分区包含终止函数指针，每个函数不带参数也没有返回值
#define ELF_SEC_PREINIT_ARRAY 16 // 该分区包含的函数指针，会在所有初始函数执行前执行，每个函数不带参数也没有返回值
#define ELF_SEC_GROUP 17       // 该分区定义一个分区组合，分区组合是一组关联的分区可以让链接器特别对待，只能存在于可重定位目标文件中，该分区头部必须位于所有包含的分区头部的前面
#define ELF_SEC_SYMTAB_SHNDX 18 // 该分区与一个符号表分区关联，符号表中如果包含值为 ELF_SHNDX_XINDEX 的分区头部索引，该分区包含对应符号引用地实际索引或0
#define ELF_SEC_LOOS 0x60000000 // 操作系统特殊语义预留
#define ELF_SEC_HIOS 0x6fffffff // 操作系统特殊语义预留
#define ELF_SEC_LOPROC 0x70000000 // 处理器特殊语义预留
#define ELF_SEC_HIPROC 0x7fffffff // 处理器特殊语义预留
#define ELF_SEC_LOUSER 0x80000000 // 应用程序预留，应用程序可以自由使用这些值而不用担心和系统定义值冲突
#define ELF_SEC_HIUSER 0xffffffff // 应用程序预留

// 分区属性标记
#define ELF_SF_WRITE 0x01         // 分区包含的数据是可写的
#define ELF_SF_ALLOC 0x02         // 分区在进程执行时会占用内存，如果分区不存在于目标文件的内存映像中，会关闭该属性
#define ELF_SF_EXECINSTR 0x04     // 分区包含可执行的机器指令
#define ELF_SF_MERGE 0x10         // 该分区中的数据可能进行合并来减少重复，除非设置了 ELF_SF_STRINGS 否则该分区包含的都是相同大小的条目
#define ELF_SF_STRINGS 0x20       // 分区包含NUL结尾的字符串，每个字符的大小由分区头部中的 entsize 指定
#define ELF_SF_INFO_LINK 0x40     // 分区头部中的 info 成员的值是一个分区头部索引
#define ELF_SF_LINK_ORDER 0x80    // 这个标记添加特别的链接顺序，典型的用法是以地址顺序构建引用代码或数据分区的表
#define ELF_SF_OS_NONCONFORMING 0x100 // 这种分区需要标准链接之外的特定操作系统处理来避免不正确行为
#define ELF_SF_GROUP 0x200        // 此分区是分区组合的一个成员分区，只有重定位目标文件中的分区才能设置为成员分区
#define ELF_SF_TLS 0x400          // 该分区包含线程本地存储
#define ELF_SF_COMPRESSED 0x800   // 该分区包含压缩的数据，不能和 ELF_SF_ALLOC 一起使用，也不能应用到 ELF_SEC_NOBITS 分区
#define ELF_SF_MASKOS 0x0ff00000  // 操作系统特殊语义预留
#define ELF_SF_MASKPROC 0xf0000000 // 处理器特殊语义预留

// 依赖于分区类型的值 link 和 info：
// 分区类型                  link                               info
// ELF_SEC_DYNAMIC         分区中的条目使用的字符串表分区的索引    0
// ELF_SEC_HASH            哈希表关联的符号表分区的索引            0
// ELF_SEC_REL/RELA        关联的符号表分区的索引                 应用重定位的分区的索引
// ELF_SEC_SYMTAB/DYNSYM   关联的字符串表分区的索引               第一个非本地符号的索引
// ELF_SEC_GROUP           关联的符号表分区的索引                 符号索引，该符号提供了组合分区的签名
// ELF_SEC_SYMTAB_SHNDX    关联的符号表分区的索引                 0

// 一些分区是相互关联的，这些分区必须作为一个单元整体包含或移除，一个分区
// 不同同时属于多个分区组合。分区类型 ELF_SEC_GOURP 用来定义这种分区组
// 和，该分区头部中的 link 和 info 用于指定该分区关联的符号，link 指定符
// 号所属符号表，info 指定符号在符号表中的索引。该分区的分区属性为0，分区
// 名称也是0未指定。关联的符号所在符号表分区不需要是该分区组合的成员。该分
// 区的数据是一个 Elf32Word 类型的数组，第一个 Elf32Word 是一个标记，剩下
// 的是成员分区的分区头部索引。成员分区必须设置 ELF_SF_GOURP 分区属性。
// 当移除分区组合时，为了避免留下悬置的引用，并对符号表进行最少的处理，需要
// 遵循以下的规则：TODO
#define ELF_GF_COMDAT 0x01 // 这个一个 COMDAT 分区组合，可能于其他目标文件中的 COMDAT 组合重复，是否重复由相同的组合签名定义
#define ELF_GF_MASKOS 0x0ff00000 // 在这种情况下，重复的分区组合只有一个会被链接器保留，剩余的分区组合的所有成员分区都会被移除
#define ELF_GF_MASKPROC 0xf0000000
typedef struct {
    uint32 flag;
    uint32 shndx[1];
} ElfGsec;

// 分区头部结构体，目标文件中的每个分区都对应有唯一一个分区头部，可以存在
// 一个分区头部而没有对应的分区；每个分区占据一块连续的字节空间（可能为空）；
// 目标文件中的分区不能相互覆盖，即一个字节数据不能同时属于多个分区；目标
// 文件中可以有非活动空间，非活动空间中的数据是未指定的。
typedef struct {
    // 分区名称，字符串分区的索引，名称是以NUL字符结束的字符串
    uint32 name;
    // 分区类型，不同的类型分区的内容和语义不同
    uint32 type;
    // 分区属性标记
    uint32 flags;
    // 如果分区会出现在进程的内存映像中，该地址指定分区第一个字节的地址，
    // 否则为0
    Uint addr;
    // 分区在文件中的偏移字节，如果分区的类型是 ELF_SEC_NOBITS，它不
    // 占用文件空间，该值是概念上的位置
    Uint offset;
    // 分区大小，如果是 ELF_SEC_NOBITS 分区，该值可以不是0，但是分区
    // 不占用文件空间
    uint32 size;
    // link 的解释依赖于分区的类型，通常是关联的分区头部索引
    uint32 link;
    // 分区的额外信息，含义依赖于分区的类型
    uint32 info;
    // 有些分区有地址对齐的要求，当前0和1表示没有对齐要求，2和2的幂表示
    // 有对齐要求
    uint32 addralign;
    // 有些分区包括多个固定大小的条目，例如符号表，该成员指定条目的大小，
    // 该值如果是0表示分区不包含固定大小的条目
    uint32 entsize;
} ElfShdr;

typedef struct {
    uint32 name;
    uint32 type;
    uint32 flags;
    uint32 addr;
    uint32 offset;
    uint32 size;
    uint32 link;
    uint32 info;
    uint32 addralign;
    uint32 entsize;
} Elf32Shdr;

typedef struct {
    uint32 name;
    uint32 type;
    uint32 flags;
    uint64 addr;
    uint64 offset;
    uint32 size;
    uint32 link;
    uint32 info;
    uint32 addralign;
    uint32 entsize;
} Elf64Shdr;

// 压缩算法类型
#define ELF_COMPRESS_ZLIB 1 // ZLIB 算法（http://zlib.net），压缩的数据紧随压缩头部结构体之后
#define ELF_COMPRESS_LOOS 0x60000000
#define ELF_COMPRESS_HIOS 0x6fffffff
#define ELF_COMPRESS_LOPROC 0x70000000
#define ELF_COMPRESS_HIPROC 0x7fffffff

// 一个压缩分区的重定位都要设定一个非压缩分区数据的偏移，因此在重定位之前，
// 必须先解压分区数据；每个压缩分区设定自己独立的压缩算法，一个目标文件中
// 的不同分区可以使用不同得压缩算法。每个压缩分区都以压缩头部结构体开始来
// 指定压缩算法。
typedef struct {
    // 压缩算法的类型
    uint32 compress;
    // 指定未压缩数据的字节数
    Uint size;
    // 未压缩数据的地址对齐要求
    Uint addralign;
} ElfChdr;

typedef struct {
    uint32 compress;
    uint32 size;
    uint32 addralign;
} Elf32Chdr;

typedef struct {
    uint32 compress;
    uint64 size;
    uint64 addralign;
} Elf64Chdr;

// 存在一些特殊的分区，有固定的分区类型和属性；以点号开始的分区名称是系统
// 预留的名称，一个目标文件可以有多个相同名称的分区；处理器架构相关的分区
// 以 machine 字段定义的处理器名称开头例如 .AARCH64.procsec。
// 分区名称         分区类型                 分区属性
// .bss             ELF_SEC_NOBITS         ELF_SF_ALLOC|WRITE       未初始化数据，程序开始运行时初始化为全零，该分区不占据文件空间
// .comment         ELF_SEC_PROGBITS       0                        该分区包含版本控制信息
// .data            ELF_SEC_PROGBITS       ELF_SF_ALLOC|WRITE       初始化数据
// .data1           ELF_SEC_PROGBITS       ELF_SF_ALLOC|WRITE       初始化数据
// .debug           ELF_SEC_PROGBITS       0                        符号调试信息，并且所有以.debug开始的名称都预留给以后使用
// .dynamic         ELF_SEC_DYNAMIC        ELF_SF_ALLOC|O(WRITE)    动态链接信息，是否可写由特定处理器决定
// .dynstr          ELF_SEC_STRTAB         ELF_SF_ALLOC             动态链接需要的字符串
// .dynsym          ELF_SEC_DYNSYM         ELF_SF_ALLOC             动态链接符号表
// .fini            ELF_SEC_PROGBITS       ELF_SF_ALLOC|EXECINSTR   进程终止时执行的指令
// .fini_array      ELF_SEC_FINI_ARRAY     ELF_SF_ALLOC|WRITE       函数指针数组，可执行或动态目标文件在进程终止时需要执行的函数
// .got             ELF_SEC_PROGBITS                                全局偏移表
// .hash            ELF_SEC_HASH           ELF_SF_ALLOC             符号哈希表
// .init            ELF_SEC_PROGBITS       ELF_SF_ALLOC|EXECINSTR   进程启动后调用主函数之前执行的指令
// .init_array      ELF_SEC_INIT_ARRAY     ELF_SF_ALLOC|WRITE       可执行或动态目标文件在初始化阶段需要执行的函数
// .interp          ELF_SEC_PROGBITS       O(ELF_SF_ALLOC)          包含程序解释器的路径名称
// .line            ELF_SEC_PROGBITS       0                        符号调试用的行号信息，描述源程序和机器码之间的行号关系
// .note            ELF_SEC_NOTE           0                        文件说明信息
// .plt             ELF_SEC_PROGBITS                                过程链接表
// .preinit_array   ELF_SEC_PREINIT_ARRAY  ELF_SF_ALLOC|WRITE       可执行或动态目标文件在初始化阶段之前需要执行的函数
// .relNAME         ELF_SEC_REL            O(ELF_SF_ALLOC)          重定位信息，如果文件包含一个包含重定位分区的可加载分段，分区需要包含ALLOC属性否则为0
// .relaNAME        ELF_SEC_RELA           O(ELF_SF_ALLOC)          按照惯例，NAME表示的是重定位需要应用的分区，例如 .text 对应的重定位分区为 .rel.text 或 .rela.text
// .rodata          ELF_SEC_PROGBITS       ELF_SF_ALLOC             只读数据
// .rodata1         ELF_SEC_PROGBITS       ELF_SF_ALLOC             只读数据
// .shstrtab        ELF_SEC_STRTAB         0                        分区名称字符串
// .strtab          ELF_SEC_STRTAB         O(ELF_SF_ALLOC)          字符串表，如果文件的一个可加载分段包含这个字符串表则需要指定ALLOC属性否则为0
// .symtab          ELF_SEC_SYMTAB         O(ELF_SF_ALLOC)          符号表，如果文件的一个可加载分段包含这个符号表则需要指定ALLOC属性否则为0
// .symtab_shndx    ELF_SEC_SYMTAB_SHNDX   O(ELF_SF_ALLOC)          符号表分区头部索引，如果对应的符号表有ALLOC属性那么这个分区也需要指定ALLOC否则为0
// .tbss            ELF_SEC_NOBITS         ELF_SF_ALLOC|WRITE|TLS   未初始化TLS数据
// .tdata           ELF_SEC_STRTAB         ELF_SF_ALLOC|WRITE|TLS   初始化TLS数据
// .tdata1          ELF_SEC_STRTAB         ELF_SF_ALLOC|WRITE|TLS   初始化TLS数据
// .text            ELF_SEC_STRTAB         ELF_SF_ALLOC|EXECINSTR   程序可执行指令
// .conflict .gptab .liblist .lit4 .lit8 .reginfo .sbss .sdata .tdesc 历史原因遗留下来的一些分区名称

// 符号索引
// 符号表分区第一个未定义符号的内容如下：
// ElfSym.name      0 没有名称
// ElfSym.value     0 零值
// ElfSym.size      0 没有大小
// ElfSym.info      0 没有类型，本地符号
// ElfSym.other     0 默认可见性
// ElfSym.shndx     0 无关联分区
#define ELF_SYMNDX_UNDEF 0
// 符号类型和绑定属性，符号的绑定属性定义了链接可见性和行为；
// 全局符号和弱符号有两个主要不同：
// 1. 全局符号不允许多个定义，但是弱符号可以有一个同名的全局符号定义，并使用
// 全局符号而忽略弱符号；如果符号关联的是一个通用符号 ELF_SHNDX_COMMON，链接
// 器也会选择该通用符号而忽略弱符号；
// 2. 当链接器搜索归档库文件时，它会解压其中包含未定义全局符号定义的成员文件，
// 该定义可以是一个全局符号或者弱符号；但是链接器不会解压归档成员文件来解决未
// 定义的弱符号，未解决的弱符号的值是0；
// 共享目标文件中的函数符号比较特殊，当一个目标文件引用共享目标文件中的函数时，
// 链接器会在该目标文件中自动为这个引用创建一个过程链接表条目；
// COMMON 类型的符号：TODO
#define ELF_SYM_TYPE(info) ((info)&0x0f)
#define ELF_SYM_BIND(info) ((info)>>4)
#define ELF_SYM_INFO(b,t) (((b)<<4)|((t)&0x0f))
#define ELF_SYM_BIND_LOCAL 0    // 本地符号，在目标文件外部不可见，在链接时本地符号优先于全局符号和弱符号
#define ELF_SYM_BIND_GLOBAL 1   // 全局符号，在所有目标文件中可见，一个目标中定义的全局符号可以解决另一个目标文件对相同符号的引用
#define ELF_SYM_BIND_WEAK 2     // 类似于全局符号，但是它们的定义优先级低
#define ELF_SYM_BIND_LOOS 10    // 操作系统语义预留
#define ELF_SYM_BIND_HIOS 12    // 操作系统语义预留
#define ELF_SYM_BIND_LOPROC 13  // 处理器语义预留
#define ELF_SYM_BIND_HIPROC 15  // 处理器语义预留
#define ELF_SYM_TYPE_NOTYPE 0   // 符号类型未指定
#define ELF_SYM_TYPE_OBJECT 1   // 符号是一个数据对象，例如变量、数组等等
#define ELF_SYM_TYPE_FUNC 2     // 符号是一个函数或其他可执行代码
#define ELF_SYM_TYPE_SECTION 3  // 符号关联的是一个分区，该类型符号主要用于重定位并拥有 LOCAL 绑定属性
#define ELF_SYM_TYPE_FILE 4     // 该符号名称是目标文件的源文件名称，绑定属性 LOCAL，关联分区 ELF_SHNDX_ABS
#define ELF_SYM_TYPE_COMMON 5   // 该符号表示一个未初始化的公共块
#define ELF_SYM_TYPE_TLS 6      // 该符号表示一个 TLS 条目，它给符号一个指定的偏移但不是实际的地址
#define ELF_SYM_TYPE_LOOS 10    // 操作系统语义预留
#define ELF_SYM_TYPE_HIOS 12    // 操作系统语义预留
#define ELF_SYM_TYPE_LOPROC 13  // 处理器语义预留
#define ELF_SYM_TYPE_HIPROC 15  // 处理器语义预留
// 符号可见性，重定位文件中的隐藏符号和内部使用符号包含到可执行或共享目标文件
// 中时要么移除要么转成成本地符号。文件内部引用的保护符号，一定会解析为该文件
// 中定义的这个保护符号，即使这个保护符号是弱符号；本地符号不能使用保护可见性，
// 如果共享目标文件中定义的保护符号被用来解决另一个可执行或共享目标文件的引用，
// 对应的未定义符号会被设成默认可见性。
#define ELF_SYM_VISIBILITY(other) ((other)&0x03)
#define ELF_SYM_DEFAULT 0       // 默认可见性，即绑定属性定义的链接可见性，本地符号是隐藏的，全局和弱符号是全局可见的且具抢占性
#define ELF_SYM_INTERNAL 1      // 内部可见，由处理器扩展定义来区分不同的隐藏可见性
#define ELF_SYM_HIDDEN 2        // 符号是隐藏的，表示符号对外部不可见也是保护的符号，注意由该符号命名的对象如果它的地址传递到了外部仍然可以被外部模块引用
#define ELF_SYM_PROTECTED 3     // 保护的符号对外部可见，但不会被外部定义的相同符号抢占，即使该符号的默认规则可以被外部符号覆盖

// 符号表分区由符号结构体组成，包含了程序符号定位和重定位信息。分区中的第
// 一个符号是未定义符号，对应的索引是 ELF_SYMNDX_UNDEF 即 0。
typedef struct {
    // 符号名称的字符串索引，索引0表示符号没有名称，字符串表由分区头部 link
    // 字段定义
    uint32 name;
    // 符号的值，根据上下文该值可能是一个绝对值、一个地址等等；如果符号的值
    // 是一个分区里的一个位置，那么 shndx 指定该分区头部索引，当这个分区因
    // 为重定位移动了，该符号的值也要随之改变；在重定位文件中，符号的值是定
    // 义该符号的分区的偏移（文件表示）；在可执行或共享目标文件中，符号的值
    // 是一个虚拟地址（内存表示），方便符号的动态链接
    uint32 value;
    // 符号关联对象的大小，例如符号表示一个 C 语言类型，该值是该类型的大小
    uint32 size;
    // 符号的类型和绑定属性，低 4-bit 是类型信息，高 4-bit 是绑定属性
    byte info;
    // 当前该字段表示符号的可见性，仅低 2-bit 有效
    byte other;
    // 当符号值是分区的位置时需要关联该分区，该字段表示这个关联分区的分区头
    // 部索引，如果这个索引值大于等于 0xffff（即 ELF_SHNDX_XINDEX），该字
    // 段设为 ELF_SHNDX_XINDEX，具体索引值由 ELF_SEC_SYMTAB_SHNDX 类型
    // 的分区定义，是哪个 ELF_SEC_SYMTAB_SHNDX 分区需要查找分区头部表，
    // 检查 ELF_SEC_SYMTAB_SHNDX 类型的分区，其 link 字段是否指向该符号
    // 分区的头部索引，并检查 ELF_SEC_SYMTAB_SHNDX 分区中对应符号位置的
    // 索引；如果该字段是 ELF_SHNDX_ABS 表示该符号的值是一个绝对值，不会被
    // 重定位影响；如果是 ELF_SHNDX_UNDEF 表示该符号是未定义符号，当链接器
    // 将当前文件与另一个定义了该符号的文件一起链接时，该符号会链接到这个具
    // 体定义；如果是 ELF_SHNDX_COMMON 表示该符号标记的是一个还未被分配的
    // 未初始化通用块，符号的值是地址对齐要求，链接器会在对应地址对齐的位置
    // 分配这个符号的存储空间，符号的 size 字段表示该通用块的大小，该符号只
    // 能出现在重定位文件中。
    uint16 shndx;
} Elf32Sym;

typedef struct {
    uint32 name;
    byte info;
    byte other;
    uint16 shndx;
    uint64 value;
    uint64 size;
} Elf64Sym;

#if __ARCH_64BIT__
typedef Elf64Sym ElfSym;
#else
typedef Elf32Sym ElfSym;
#endif

// 重定位符号和重定位类型
#define ELF32_REL_SYM(info) ((info)>>8)
#define ELF32_REL_TYPE(info) ((info)&0xff)
#define ELF32_REL_INFO(s,t) (((s)<<8)|((t)&0xff))
#define ELF64_REL_SYM(info) ((info)>>32)
#define ELF64_REL_TYPE(info) ((info)&0xffffffffL)
#define ELF64_REL_INFO(s,t) (((s)<<32)|((t)&0xffffffffL))
#if __ARCH_64BIT__
#define ELF_REL_SYM(info) ELF64_REL_SYM(info)
#define ELF_REL_TYPE(info) ELF64_REL_TYPE(info)
#define ELF_REL_INFO(s,t) ELF64_REL_INFO((s),(t))
#else
#define ELF_REL_SYM(info) ELF32_REL_SYM(info)
#define ELF_REL_TYPE(info) ELF32_REL_TYPE(info)
#define ELF_REL_INFO(s,t) ELF32_REL_INFO((s),(t))
#endif

// 重定位结构体，重定位用于处理符号引用和符号定义；例如当程序调用一个函数时，
// 对应的调用指令必须将代码控制权转移到合适的目标地址；重定位文件必须有重定
// 位条目来描述怎样修改它的分区内容，从而允许可执行或共享目标文件为程序的进
// 程映像创建正确的内容；重定位分区关联了两个其他分区，一个是符号表分区（由
// 重定位分区头部字段 link 指定），一个是修改的分区（由头部字段 info 指定）;
// 重定位的典型应用是：
// 1. 确定引用的符号值
// 2. 提取附加数
// 3. 将重定位类型所隐含的表达式应用于符号和加数
// 4. 提取表达式结果的所需部分
// 5. 将这部分结果保存到需要重定位的字段中
// 如果多个连续的重定位条目应用于同一个 offset 指定的位置，它们不单独应用，
// 而是组合应用，组合应用意味着上述标准应用方式需要修改成：
// 1. 除了组合中最后一个重定位操作，其他重定位操作的表达式结果以适用的 ABI
// 处理器补充的完整指针的精度保留，而不是提取部分结果并保存到重定位字段
// 2. 除了组合中的第一个重定位操作，其他重定位操作使用的附加数是前一次重定
// 位操作保留的表达式结果
// 3. ABI 处理器补充可能会指定单独的重定位类型，总是阻止组合应用，或总是开
// 启一个新的重定位应用
typedef struct {
    // 重定位应用的位置，重定位文件这个值是分区开始的偏移，可执行或共享目标
    // 文件是受重定位影响的存储单元的虚拟地址；具体应用到哪个分区，由重定位
    // 分区头部字段 info 指定
    Uint offset;
    // 指定需要重定位的符号索引（高24位或高32位）以及重定位类型（低8位或低
    // 32位），符号是哪个符号表中的符号由重定位分区头部字段 link 指定；例如，
    // 一个调用指令的重定位条目需要指定被调函数的符号索引，如果使用未定义的
    // 符号索引 ELF_SYMNDX_UNDEF，重定位使用0作为符号值；重定位的类型是处
    // 理器相关的，它的描述在 ABI 处理器补充文档中（psABI）
    Uint info;
    // 计算可重定位字段值需要显式附加的值，重定位条目类型 ElfRel 在需要修改
    // 的位置隐式保存了需要附加的值；根据具体处理器，这两种方式可能某一种会
    // 更方便，因此特定机器的实现可能根据具体上下文使用其中一种方式。
    Int addend;
} ElfRela;

typedef struct {
    Uint offset;
    Uint info;
} ElfRel;

typedef struct {
    uint32 offset;
    uint32 info;
    int32 addend;
} Elf32Rela;

typedef struct {
    uint64 offset;
    uint64 info;
    int64 addend;
} Elf64Rela;

typedef struct {
    uint32 offset;
    uint32 info;
} Elf32Rel;

typedef struct {
    uint64 offset;
    uint64 info;
} Elf64Rel;

// 程序头部表中的程序头部结构体，程序头部的虚拟地址可能不是程序内存映像真实的
// 虚拟地址；其中可执行文件通常包含绝对代码，为了使进程正确执行，分段必须位于
// 用来创建可执行文件的虚拟地址中；而共享目标文件通常包含位置无关代码，这让分
// 段虚拟地址随着不同进程而变化避免非法执行行为。在一些平台上，当系统为单个进
// 程选择虚拟地址时，它维护每个分段与其他共享对象分段的相对位置。由于这些平台
// 位置无关代码使用分段间的相对地址，内存中的虚拟地址的偏移必须与文件中虚拟地
// 址匹配，这个偏移对于一个给定的进程是一个常量值，称为基地址，基地址的一个用
// 途是在动态链接中重定位文件的内存映像。一个可执行文件或共享目标文件的基地址，
// 在支持这个概念的平台上，是在执行时根据三个值计算的：虚拟内存加载地址，最大
// 的内存页大小，程序可加载分段的最低虚拟地址。首先内存加载地址会关联到可加载
// 分段的最低虚拟地址并截断到最近的最大内存页的整数倍，而分段的虚拟地址也会截
// 断到最近的最大内存页的整数倍，最后基地址的值是两个截断地址间的差。
typedef struct {
    // 程序头部类型
    uint32 type;
    // 分段权限标记
    uint32 flags;
    // 分段第一个字节的文件偏移
    uint64 offset;
    // 分段第一个字节在内存的虚拟地址
    uint64 vaddr;
    // 在物理地址有意义的系统上该字段是分段的物理地址，但 System V 忽略应用
    // 程序的物理地址，因此该字段的内容未指定
    uint64 paddr;
    // 分段在文件中的大小，可能为 0，但必须大于等于 memsz
    uint64 filesz;
    // 分段在内存映像中的大小，可能为 0
    uint64 memsz;
    // 指定 vaddr 和 offset 的地址对齐要求，0和1表示没有要求，2和2的幂表示
    // 需要对齐到的该值的整数倍地址
    uint64 align;
} Elf64Phdr;

typedef struct {
    uint32 type;
    uint32 offset;
    uint32 vaddr;
    uint32 paddr;
    uint32 filesz;
    uint32 memsz;
    uint32 flags;
    uint32 align;
} Elf32Phdr;

#if __ARCH_64BIT__
typedef Elf64Phdr ElfPhdr;
#else
typedef Elf32Phdr ElfPhdr;
#endif

// 程序头部类型，每个程序头部描述一个程序分段，一个程序分段包含一个或多个分区。可加载分
// 段如果 memsz 大于 filesz，额外字节是0，可加载分段必须按虚拟地址 vaddr 从小到大排序。
// 描述程序头部表自己的头部只有当程序头部表是内存映像的一部分时才有意义，如果存在它必须
// 在所有可加载程序头部之前。描述解释器的程序头部如果存在也必须在所有可加载程序头部之前。
#define ELF_PT_NULL     0   // 未使用的程序头部，程序头部的其他字段的值未定义
#define ELF_PT_LOAD     1   // 描述一个可加载分段，文件中的分段会映射到内存分段
#define ELF_PT_DYNAMIC  2   // 描述一个 ELF_SEC_DYNAMIC 分区（.dynamic），包含动态链接信息，只能有一个
#define ELF_PT_INTERP   3   // 包含程序解释器的位置和路径字符串长度，只能有一个
#define ELF_PT_NOTE     4   // 说明分区，包含辅助信息的位置和长度
#define ELF_PT_SHLIB    5   // 预留但当前未指定语义
#define ELF_PT_PHDR     6   // 程序头部表自己的位置和长度，只能出现一次
#define ELF_PT_TLS      7   // 分段内容是 TLS 模板
#define ELF_PT_LOOS     0x60000000  // 操作系统特殊语义预留
#define ELF_PT_HIOS     0x6fffffff  // 操作系统特殊语义预留
#define ELF_PT_LOPROC   0x70000000  // 处理器殊语义预留
#define ELF_PT_HIPROC   0x7fffffff  // 处理器殊语义预留

// 程序分段权限标记，被系统加载的程序必须有至少一个可加载分段，当系统创建可加载分段的内
// 存映像时，根据程序头部的 flags 字段来确定分段的权限。实际的内存权限由内存管理单元决
// 定，系统可能授权比实际请求更多的权限。通常代码段是可读可执行的，但不可写；数据段是可
// 读、可写、并且可执行的。
// 请求权限          允许权限
// 0x00             无权限
// ELF_PF_X         可读、可执行
// ELF_PF_W         可读、可写、可执行
// ELF_PF_W|X       可读、可写、可执行
// ELF_PF_R         可读、可执行
// ELF_PF_R|X       可读、可执行
// ELF_PF_R|W       可读、可写、可执行
// ELF_PF_R|W|X     可读、可写、可执行
#define ELF_PF_X 0x01   // 可执行
#define ELF_PF_W 0x02   // 可写
#define ELF_PF_R 0x04   // 可读
#define ELF_PF_MASKOS 0x0ff00000    // 未指定
#define ELF_PF_MASKPROC 0xf0000000  // 未指定

// 动态链接段包含的动态链接结构体，如果一个目标文件参与到动态链接中，它的程序头部需要包
// 含一个动态链接分段（PT_DYNAMIC），该分段包含一个 .dynamic 分区。有一个特殊的符号
// _DYNAMIC 数组，数组元素是动态链接结构体，也即动态链接分段的内容是这个结构体数组。
// extern ElfDyn _DYNAMIC[];
typedef struct {
    // tag 控制着对联合字段的解释
    Int tag;
    union {
    // val 是一个机器字长的整数值
    Uint val;
    // ptr 表示程序的虚拟地址，文件的虚拟地址可能与执行时的内存虚拟地址不匹配，当解析这
    // 个地址时，动态链接器会计算实际的地址，基于文件地址和内存基址
    Uint ptr;
    };
} ElfDyn;

typedef struct {
    int32 tag;
    union {
    uint32 val;
    uint32 ptr;
    };
} Elf32Dyn;

typedef struct {
    int64 tag;
    union {
    uint64 val;
    uint64 ptr;
    };
} Elf64Dyn;

// 动态链接条目的类型标签，下表列出了可执行文件和共享目标文件对标签的需求，其中 M 表示动
// 态链接数组必须包含该类型的一个条目，而 O 表示包含这种标签的条目不是必须的：
//                               联合体 可执行文件 共享目标文件
#define ELF_DT_NULL 0         // 忽略     M       M       空元素表示数组的结束
#define ELF_DT_NEEDED 1       // val      O       O       字符串表的偏移表示所需库的名称，可用有多个依赖库，这些库出现的相对顺序是重要的
#define ELF_DT_PLTRELSZ 2     // val      O       O       与过程链接表关联的重定位条目的总大小，如果 JMPREL 存在该条目必须存在
#define ELF_DT_PLTGOT 3       // ptr      O       O       保存过程链接表或全局偏移表的地址
#define ELF_DT_HASH 4         // ptr      M       M       哈希表分区的地址，该哈希表被 SYMTAB 符号表使用
#define ELF_DT_STRTAB 5       // ptr      M       M       字符串表分区的地址
#define ELF_DT_SYMTAB 6       // ptr      M       M       符号表分区的地址
#define ELF_DT_RELA 7         // ptr      M       O       重定位表的地址，一个目标文件可能有多个重定位分区，但当构建可执行或共享目标文件的重定位表时，链接
#define ELF_DT_RELASZ 8       // val      M       O       编辑器会将所有重定位分区合并到一个表中，尽管重定位分区在目标文件中是独立的，但是动态链接器看到是
#define ELF_DT_RELAENT 9      // val      M       O       一个重定位表；RELASZ 表示重定位表的总大小；RELAENT 表示重定位结构体的大小；
#define ELF_DT_STRSZ 10       // val      M       M       字符串表的总大小
#define ELF_DT_SYMENT 11      // val      M       M       符号表中符号结构体的大小
#define ELF_DT_INIT 12        // ptr      O       O       初始函数的地址
#define ELF_DT_FINI 13        // ptr      O       O       终止函数的地址
#define ELF_DT_SONAME 14      // val      忽略    O       共享目标文件的名称
#define ELF_DT_RPATH 15       // val      O       忽略    库搜索路径字符串，如果存在 RUNPATH 会被 RUNPATH 覆盖
#define ELF_DT_SYMBOLIC 16    // 忽略     忽略     O       动态链接器首先从共享文件自己内部找未定义符号，失败后才正常查可执行和其他共享文件，会被 SYMBOLIC 标记覆盖
#define ELF_DT_REL 17         // ptr      M       O       重定位表的地址
#define ELF_DT_RELSZ 18       // val      M       O       重定位表的总大小
#define ELF_DT_RELENT 19      // val      M       O       重定位结构体的大小
#define ELF_DT_PLTREL 20      // val      O       O       过程链接表引用的重定位表的类型，ELF_DT_REL 或 ELF_DT_RELA，过程链接表的所有重定位必须是相同的类型
#define ELF_DT_DEBUG 21       // ptr      O       忽略    用于调试，其内容未指定
#define ELF_DT_TEXTREL 22     // 忽略      O       O      如果存在，重定位条目可以请求修改不可写分段，会被 TEXTREL 标记覆盖
#define ELF_DT_JMPREL 23      // ptr      O       O       过程链接表关联的重定位条目的地址，如果存在 PLTSZ 和 PLTREL 必须存在
#define ELF_DT_BIND_NOW 24    // 忽略      O       O      告诉动态链接器在把控制权交给程序之前处理所有的重定位，如果不存在则延时绑定或调用 dlopen(BA_LIB) 绑定，会被 BIND_NOW 标记覆盖
#define ELF_DT_INIT_ARRAY 25  // ptr      O       O       初始函数指针数组的地址
#define ELF_DT_FINI_ARRAY 26  // ptr      O       O       终结函数指针数组的地址
#define ELF_DT_INIT_ARRAYSZ 27 // val     O       O       初始函数指针数组的大小
#define ELF_DT_FINI_ARRAYSZ 28 // val     O       O       终结函数指针数组的大小
#define ELF_DT_RUNPATH 29     // val      O       O       库搜索路径字符串，会覆盖 RPATH
#define ELF_DT_FLAGS 30       // val      O       O       动态加载标记
#define ELF_DT_ENCODING 32    // 未指定   未指定  未指定    未指定
#define ELF_DT_PREINIT_ARRAY 32 // ptr    O       忽略     预初始化函数指针数组的地址
#define ELF_DT_PREINIT_ARRAYSZ 33 // val  O       忽略     预初始化函数指针数组的大小
#define ELF_DT_SYMTAB_SHNDX 34 // ptr     O       O       符号表 ELF_DT_SYMTAB 引用的 ELF_SEC_SYMTAB_SHNDX 分区的地址
#define ELF_DT_LOOS 0x6000000D // 未指定  未指定  未指定   未指定
#define ELF_DT_HIOS 0x6ffff000 // 未指定  未指定  未指定   未指定
#define ELF_DT_LOPROC 0x70000000 // 未指定 未指定 未指定   未指定
#define ELF_DT_HIPROC 0x7fffffff // 未指定 未指定 未指定   未指定
// 动态链接标记
#define ELF_DF_ORIGIN 0x01    // 加载的目标可以引用 $ORIGIN 替换字符串，动态链接器必须在目标加载时确定包含这个条目的目标的路径名称
#define ELF_DF_SYMBOLIC 0x02  // 用于共享目标文件，动态链接器会首先使用共享目标文件内部的符号来解析未定义符号
#define ELF_DF_TEXTREL 0x04   // 重定位条目可以请求修改不可写分段
#define ELF_DF_BIND_NOW 0x08  // 动态链接器在把控制权交给程序之前处理所有的重定位，否则延时绑定或调用 dlopen(BA_LIB) 绑定
#define ELF_DF_STATIC_TLS 0x10 // 让动态链接器拒绝动态加载 TLS，表示使用的是静态 TLS

// 说明分段
typedef struct {
    Uint namesz; // 包括 NUL 字符
    Uint descsz;
    Uint type;
    byte name[1];
    // byte desc[1]; // 对齐到 uint
} ElfNote;
typedef struct {
    uint32 namesz;
    uint32 descsz;
    uint32 type;
    byte name[1];
} Elf32Note;
typedef struct {
    uint64 namesz;
    uint64 descsz;
    uint64 type;
    byte name[1];
} Elf64Note;

// TLS 分段的程序头部信息，TLS 模板由所有标记是 ELF_SF_TLS 的分区组成，存有初始化数据
// 的 TLS 模板的位置是 TLS 初始映像，TLS 模板的剩余部分由一个 ELF_SEC_NOBITS 的 TLS
// 分区组成：
// ElfPhdr.type     ELF_SEG_TLS
// ElfPhdr.offset   TLS 初始映像的文件偏移位置
// ElfPhdr.vaddr    TLS 初始映像的内存虚拟地址
// ElfPhdr.paddr    保留
// ElfPhdr.filesz   TLS 初始映像的大小
// ElfPhdr.memsz    TLS 模板的总大小
// ElfPhdr.flags    ELF_PF_R 可读
// ElfPhdr.align    TLS 模板的地址对齐要求

// 哈希表是一个 ElfWord 整数数组，它的组织方式示意如下：
typedef struct {
    uint32 nbucket;     // 桶的个数，bucket[elfhash(symnm)%nbucket] 存储的是符号索引
    uint32 nchain;      // 链接的个数，必须等于符号表中符号的个数
    uint32 bucket[1];   // 通过 elfhash 函数计算符号对应的桶索引，对应的桶索引保存该符号在符号表中的符号索引
    // uint32 chain[1]; // 如果桶中保存的不是这个符号，继续查找 chain[符号索引] 中符号索引对应的符号
} ElfHash;              // 依次类推，直到找到对应符号，或者遇到 ELF_SYMNDX_UNDEF 表示没有这个符号

// 传入一个符号的名称，返回用于计算桶索引的值
uint32 elf_hash(const byte* symnm) {
    uint32 h = 0, g;
    while (*symnm) {
        h = (h << 4) + *symnm++;
        g = (h & 0xf0000000);
        if (g) {
            h ^= g >> 24;
        }
        h &= ~g;
    }
    return h;
}

#endif /* CHAPL_CHCC_GELF_H */
