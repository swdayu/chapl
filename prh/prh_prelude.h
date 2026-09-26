// prh_prelude.h - v0.01 - public domain - swdayu <github.com/swdayu>
// No warranty implied, use at your own risk.

#ifndef prh_impl_include_prelude_h
#define prh_impl_include_prelude_h
#ifdef __cplusplus
extern "C" {
#endif

// Microsoft C/C++ 编译器（MSVC）会根据语言（C 或 C++）、编译目标和所选的编译器选项，预定义某些预处
// 理器宏。MSVC 支持 ANSI/ISO C99、C11 和 C17 标准，以及 ISO C++14、C++17 和 C++20 标准所要求的预定
// 义预处理器宏。该实现还支持若干 Microsoft 特有的预处理器宏。某些宏仅在特定构建环境或编译器选项下定
// 义。除特别注明外，这些宏在整个翻译单元内定义，如同以 /D 编译器选项参数指定的一样。定义后，预处理
// 器会在编译前将宏展开为其指定的值。预定义宏不接受参数，也不能被重新定义。
//
// https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros
//
//      __cplusplus
//      __STDC_VERSION__
//      __FILE__
//      __LINE__
//      __DATE__    Mon dd yyyy
//      __TIME__    hh:mm:ss
//
// ARM64EC（Emulation Compatible，仿真兼容）是微软在 Windows 11 上引入的一种 ABI。ARM64EC 代码本身是
// ARM64 机器码，在 ARM 芯片上原生执行，不走二进制翻译。当它调用 x64 函数时，系统插入 XTAC thunk
// （x64-to-ARM64 compatibility thunk），在两种 ABI 之间转换参数、寄存器状态和栈布局，然后进入 x64
// 代码，由 Prism 仿真器执行 x64 部分。反过来 x64 代码调用 EC 函数也同样经 thunk 转换。_M_ARM64EC
// 会同时定义 _M_ARM64。ARM64 是纯种原生 ABI，ARM64EC 是用 ARM64 指令实现、但 ABI 层面伪装成 x64 的
// 兼容层，核心价值是让 ARM 原生代码和 x64 仿真代码能在同一进程里互相调用。
//
//      _MSC_VER
//      _WIN32      target is x86, x64, 32-bit ARM, 64-bit ARM, or ARM64EC
//      _WIN64      target is x64, 64-bit ARM, or ARM64EC
//
//      _M_IX86     target x86 processors
//      _M_X64      target x64 processors or ARM64EC
//      _M_AMD64    target x64 processors or ARM64EC
//      _M_ARM      target ARM processors
//      _M_ARM64    target ARM64
//      _M_ARM64EC  target ARM64EC

// Visual Studio version                _MSC_VER
// Visual Studio 6.0                    1200
// Visual Studio .NET 2002 (7.0)        1300
// Visual Studio .NET 2003 (7.1)        1310
// Visual Studio 2005 (8.0)             1400
// Visual Studio 2008 (9.0)             1500
// Visual Studio 2010 (10.0)            1600
// Visual Studio 2012 (11.0)            1700
// Visual Studio 2013 (12.0)            1800
// Visual Studio 2015 (14.0)            1900
// Visual Studio 2017 RTW (15.0)        1910
// Visual Studio 2017 version 15.3      1911
// Visual Studio 2017 version 15.5      1912
// Visual Studio 2017 version 15.6      1913
// Visual Studio 2017 version 15.7      1914
// Visual Studio 2017 version 15.8      1915
// Visual Studio 2017 version 15.9      1916
// Visual Studio 2019 RTW 16.0          1920
// Visual Studio 2019 version 16.1      1921
// Visual Studio 2019 version 16.2      1922
// Visual Studio 2019 version 16.3      1923
// Visual Studio 2019 version 16.4      1924
// Visual Studio 2019 version 16.5      1925
// Visual Studio 2019 version 16.6      1926
// Visual Studio 2019 version 16.7      1927
// Visual Studio 2019 version 16.8      1928
// Visual Studio 2019 version 16.10     1929
// Visual Studio 2022 RTW 17.0          1930
// Visual Studio 2022 version 17.1      1931
// Visual Studio 2022 version 17.2      1932
// Visual Studio 2022 version 17.3      1933
// Visual Studio 2022 version 17.4      1934
// Visual Studio 2022 version 17.5      1935
// Visual Studio 2022 version 17.6      1936
// Visual Studio 2022 version 17.7      1937
// Visual Studio 2022 version 17.8      1938
// Visual Studio 2022 version 17.9      1939
// Visual Studio 2022 version 17.10     1940
// Visual Studio 2022 version 17.11     1941
// Visual Studio 2022 version 17.12     1942
// Visual Studio 2022 version 17.13     1943
// Visual Studio 2022 version 17.14     1944

// https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
// https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
// https://gcc.gnu.org/onlinedocs/cpp/System-specific-Predefined-Macros.html
//
// gcc -dM -E - < /dev/null         # 默认定义的宏
// gcc -m32 -dM -E - < /dev/null    # 32 位目标的宏（如 __i386__）
// gcc -m64 -dM -E - < /dev/null    # 64 为目标的宏
//
//      __GNUC__
//      __GNUC_MINOR__
//      __GNUC_PATCHLEVEL__
//
//      __BYTE_ORDER__
//      __ORDER_LITTLE_ENDIAN__
//      __ORDER_BIG_ENDIAN__
//      __ORDER_PDP_ENDIAN__
//
//                              long long always is 64-bit
//      __LP64__ _LP64          long/pointer 64-bit, target long int and pointer use 64-bit and int use 32-bit
//      __LLP64__               long long/pointer 64-bit, int/long 32-bit
//      __ILP32__               int/long/pointer 32-bit
//
// __GNUC__ __GNUC_MINOR__ __GNUC_PATCHLEVEL__
// /* Test for GCC > 3.2.0 */
// #if __GNUC__ > 3 || (__GNUC__ == 3 && (__GNUC_MINOR__ > 2 ||
//      (__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ > 0)))
// /* Test for GCC > 3.2.0 */
// #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 +
//      __GNUC_PATCHLEVEL__)
// #if GCC_VERSION > 30200

// https://clang.llvm.org/docs/LanguageExtensions.html
//
//      __clang__ defined when compiling with Clang
//      __clang_major__ e.g., the 2 in 2.0.1
//      __clang_minor__ e.g., the 0 in 2.0.1
//      __clang_patchlevel__ e.g., the 1 in 2.0.1
//      __clang_version__ version string, e.g., "1.5 (trunk 102332)"

#undef prh_msc_version
#undef prh_gcc_version
#undef prh_clang_version

#if defined(_MSC_VER)
    #define prh_msc_version _MSC_VER
#elif defined(__GNUC__)
    #define prh_gcc_version (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__clang__)
    #define prh_clang_version (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#else
    #error "unknown compiler"
#endif

#undef prh_arch_bits
#undef prh_arch_32
#undef prh_arch_64

#if defined(prh_using_arch_64_bit) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64) || defined(__LP64__) || defined(__LLP64__) || defined(__aarch64__)
    #define prh_arch_bits 64
    #define prh_arch_64 1
#elif defined(prh_using_arch_32_bit) || defined(_M_IX86) || defined(_M_ARM) || defined(__ILP32__) || defined(__i386__) || defined(__arm__)
    #define prh_arch_bits 32
    #define prh_arch_32 1
#else
    #error "unknown architecture"
#endif

#undef prh_arch_arm
#undef prh_arch_a64
#undef prh_arch_x86
#undef prh_arch_x64

#if defined(prh_arch_64)
    #if defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__)
        #define prh_arch_a64 1
    #elif defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || defined(__amd64__)
        #define prh_arch_x64 1
    #else
        #error "unknown 64-bit architecture"
    #endif
#elif defined(prh_arch_32)
    #if defined(_M_ARM) || defined(__arm__)
        #define prh_arch_arm 1
    #elif defined(_M_IX86) || defined(__i386__)
        #define prh_arch_x86 1
    #else
        #error "unknown 32-bit architecture"
    #endif
#endif

// 平台                                                                         现状
// AIX - IBM 的 Unix，跑在 Power 小型机上                                       仍然常用，银行、电信、大企业核心业务还在跑，IBM 持续更新（当前 AIX 7.3）
// Haiku OS - BeOS（1990 年代 Be 公司多媒体操作系统）的开源复刻版               活跃开发中，但属爱好者/小众 OS，装在真机上用的人很少
// RISC OS - Acorn 公司 1987 年为 ARM 设计的 OS（树莓派的前身 lineage）         小众存活，有社区维护版，跑在树莓派上，爱好者向
// QNX Neutrino - 黑莓的微内核实时操作系统（RTOS）                              仍然很常用，汽车车机、工业控制、医疗设备大量采用
// Microsoft GDK - 游戏开发套件（Game Development Kit），Xbox/Windows 游戏开发  现役工具，至今是 Xbox 游戏开发的标准 SDK
// Sony PlayStation 2 - 索尼 2000 年主机，史上销量最高游戏机（1.55 亿台）       2013 年停产，今天主要存在于模拟器和收藏圈
// Sony PSP - 索尼 2004 年掌机                                                  2014 年停产，模拟器/homebrew 活跃
// PS Vita - 索尼 2011 年掌机                                                   2019 年停产，同上，退役平台，homebrew 圈还活着
// Nintendo 3DS - 任天堂 2011 年掌机                                            2020 年停产、2023 年关闭 eShop，玩家存量很大，但已是退役平台，只剩自制软件（homebrew）生态活跃
// HP-UX (hpux) - 惠普的 Unix，跑在 PA-RISC / Itanium 服务器上                  遗留系统，还在一些大企业机房跑着，但 HP 已停止新硬件支持，整体走向消亡
// GNU/Hurd - GNU 计划的微内核 OS，30 多年"永远即将发布"                        实验性，没人实际生产使用，Debian GNU/Hurd 只是个移植版
// Nokia N-Gage - 诺基亚 2003 年的"游戏手机"（infamous 侧贴脸打电话造型）       早已淘汰（约 2006 年死亡）
// Tru64 (OSF/1) - DEC 为 Alpha 处理器开发的 Unix，后归惠普                     2012 年前后彻底终止，纯遗留
// OS/2 - IBM/微软合作的 PC 操作系统（Warp 最有名）                             IBM 2006 年停止支持，商业续命版 ArcaOS 仍卖给 ATM、自助终端等钉子户
// IRIX - SGI 工作站的 Unix（MIPS 架构，90 年代影视/图形工作站霸主）            2006 年随 SGI 衰落而终结，只剩硬件收藏爱好者
// BSDi - BSD/386 商业化公司（后改名 BSD/OS），与 FreeBSD 同源竞争              早已消亡（2000 年代初），血统部分并入 FreeBSD

#undef prh_plat_aix
#undef prh_plat_haiku
#undef prh_plat_qnxnto
#undef prh_plat_riscos
#undef prh_plat_ps2
#undef prh_plat_psp
#undef prh_plat_vita
#undef prh_plat_3ds

#ifdef _AIX
#define prh_plat_aix 1
#endif

#ifdef __HAIKU__
#define prh_plat_haiku 1
#endif

#if defined(__QNXNTO__) || defined(__QNX__)
#define prh_plat_qnxnto 1
#endif

#if defined(riscos) || defined(__riscos) || defined(__riscos__)
#define prh_plat_riscos 1
#endif

#if defined(__PS2__) || defined(PS2)
#define prh_plat_ps2 1
#endif

#if defined(__PSP__) || defined(__psp__)
#define prh_plat_psp 1
#endif

#if defined(__vita__) || defined(__psp2__)
#define prh_plat_vita 1
#endif

#ifdef __3DS__
#define prh_plat_3ds 1
#endif

#undef prh_plat_linux
#undef prh_plat_android
#undef prh_plat_unix
#undef prh_plat_emscripten
#undef prh_plat_freebsd
#undef prh_plat_netbsd
#undef prh_plat_openbsd

#if defined(linux) || defined(__linux) || defined(__linux__)
#define prh_plat_linux 1
#endif

#if defined(ANDROID) || defined(__ANDROID__)
#define prh_plat_android 1
#undef prh_plat_linux
#endif

#if defined(__unix__) || defined(__unix) || defined(unix)
#define prh_plat_unix 1 // unix-like system, other platforms like linux may addition define this
#endif

#ifdef __EMSCRIPTEN__
#define prh_plat_emscripten 1
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
#define prh_plat_freebsd 1
#endif

#ifdef __NetBSD__
#define prh_plat_netbsd 1
#endif

#ifdef __OpenBSD__
#define prh_plat_openbsd 1
#endif

#undef prh_plat_apple
#undef prh_plat_macos
#undef prh_plat_ios
#undef prh_plat_tvos
#undef prh_plat_versionos

#ifdef __APPLE__

// apple platforms, iOS macOS etc will additionally define a more specific platform
#define prh_plat_apple 1

// +---------------------------------------------------------------------+
// |                            TARGET_OS_MAC                            |
// | +---+ +-----------------------------------------------+ +---------+ |
// | |   | |               TARGET_OS_IPHONE                | |         | |
// | |   | | +---------------+ +----+ +-------+ +--------+ | |         | |
// | |   | | |      IOS      | |    | |       | |        | | |         | |
// | |OSX| | |+-------------+| | TV | | WATCH | | BRIDGE | | |DRIVERKIT| |
// | |   | | || MACCATALYST || |    | |       | |        | | |         | |
// | |   | | |+-------------+| |    | |       | |        | | |         | |
// | |   | | +---------------+ +----+ +-------+ +--------+ | |         | |
// | +---+ +-----------------------------------------------+ +---------+ |
// +---------------------------------------------------------------------+

// https://developer.apple.com/documentation/kernel/mach
// https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/
// https://github.com/phracker/MacOSX-SDKs/tree/master/MacOSX11.3.sdk
// https://www.manpagez.com/man/3/clock_gettime/
// https://epir.at/2019/10/30/api-availability-and-target-conditionals/
// https://github.com/phracker/MacOSX-SDKs/blob/master/MacOSX11.3.sdk/usr/include/AvailabilityMacros.h
// https://github.com/phracker/MacOSX-SDKs/blob/master/MacOSX11.3.sdk/usr/include/TargetConditionals.h

#include <AvailabilityMacros.h> // AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER, know what version of macOS compiling on
#ifndef __has_extension // older compilers don't support this
    #define __has_extension(x) 0
    #include <TargetConditionals.h>
    #undef __has_extension
#else
    #include <TargetConditionals.h>
#endif

// Fix building with older SDKs that don't define these. More information:
// https://stackoverflow.com/questions/12132933/preprocessor-macro-for-os-x-targets
// TARGET_OS_MAC - Generated code will run under Mac OS X variant
//      TARGET_OS_OSX               - Generated code will run under OS X devices
//      TARGET_OS_IPHONE            - Generated code for firmware, devices, or simulator
//          TARGET_OS_IOS           - Generated code will run under iOS
//          TARGET_OS_TV            - Generated code will run under Apple TV OS
//          TARGET_OS_WATCH         - Generated code will run under Apple Watch OS
//          TARGET_OS_BRIDGE        - Generated code will run under Bridge devices
//          TARGET_OS_MACCATALYST   - Generated code will run under macOS
//      TARGET_OS_SIMULATOR         - Generated code will run under a simulator
// TARGET_OS_EMBEDDED       - DEPRECATED: Use TARGET_OS_IPHONE and/or TARGET_OS_SIMULATOR instead
// TARGET_IPHONE_SIMULATOR  - DEPRECATED: Same as TARGET_OS_SIMULATOR
// TARGET_OS_NANO           - DEPRECATED: Same as TARGET_OS_WATCH

#ifndef TARGET_OS_MACCATALYST
    #define TARGET_OS_MACCATALYST 0
#endif
#ifndef TARGET_OS_IOS
    #define TARGET_OS_IOS 0
#endif
#ifndef TARGET_OS_IPHONE
    #define TARGET_OS_IPHONE 0
#endif
#ifndef TARGET_OS_TV
    #define TARGET_OS_TV 0
#endif
#ifndef TARGET_OS_SIMULATOR
    #define TARGET_OS_SIMULATOR 0
#endif
#ifndef TARGET_OS_VISION
    #define TARGET_OS_VISION 0
#endif

#if TARGET_OS_TV
    #define prh_plat_tvos 1
#endif

#if TARGET_OS_VISION
    #define prh_plat_versionos 1
#endif

#if TARGET_OS_IPHONE
    #define prh_plat_ios 1
#else
    #define prh_plat_macos 1 // 10.12 (101200)
    #if MAC_OS_X_VERSION_MIN_REQUIRED < 101200
        #error "only supports macOS 10.12 and above"
    #endif
#endif

#endif // __APPLE__

#undef prh_plat_ngage
#undef prh_plat_dos
#undef prh_plat_windows
#undef prh_plat_win32
#undef prh_plat_wingdk
#undef prh_plat_xboxone
#undef prh_plat_xboxseries
#undef prh_plat_msft_gdk

// CYGWIN 是一个在 Windows 操作系统上模拟 Unix/Linux 环境的大型工具集，它借助一个动态链接库 cygwin1.dll
// 来模拟许多类 Unix 系统调用和 POSIX API。当你在 CYGWIN 环境中运行程序时，程序会调用 cygwin1.dll，
// 该库再将这些调用转换为 Windows API 调用，从而实现类 Unix 环境的模拟。CYGWIN 相当于是 Windows 上的
// POSIX 兼容层，在 Windows 上提供完整的 POSIX API 模拟（fork、exec、/etc/passwd、……），程序链接 cygwin1.dll
// 运行。优点是能把大量 Unix 软件几乎原样移植到 Windows；代价是依赖这个 DLL，且 copyleft 许可对闭源
// 分发有影响。
//
// Cygwin 的 GCC 会定义 __CYGWIN__ 宏以及 __unix__ 等宏，可以原生调用 Win32 API，但程序运行时依赖
// cygwin1.dll。Cygwin 默认不定义 _WIN32 宏，所以网上常见 #ifdef _WIN32 的分支代码在 Cygwin 下不会
// 走 Windows 分支，要使用如下代码。Cygwin 的 Win32 API 包就是从 MinGW 继承来的同一套头文件/导入库，
// 所以 #include <windows.h> 直接可用：gcc main.c -o main.exe -luser32。但需要注意的是，程序必须链
// 接 cygwin1.dll，另外 Cygwin 程序本质是 "POSIX 程序"，混用 POSIX 调用（fork/管道）和 Win32 句柄
// （HANDLE）时要小心语义转换。
//
// #if defined(_WIN32) || defined(__CYGWIN__)
//     // Windows API 代码
// #endif
//
// MINGW（Minimalist GNU for Windows）则是将 GNU 工具集移植到 Windows 平台的项目，它直接生成原生的
// Windows 可执行文件，不依赖模拟层。MINGW 编译的程序使用 Windows API，而不是模拟 Unix 系统调用，因
// 此生成的程序可以直接在 Windows 上运行，无需额外的运行时环境。MinGW/MinGW-w64 是原生 Windows 工具
// 链，把 gcc、binutils、gdb 等 GNU 工具移植到 Windows，让 gcc 直接产出原生 PE 程序，链接 MSVCRT/UCRT
// 和 Win32 API，没有任何 POSIX 模拟层，编译出的 exe 拿到裸 Windows 上就能跑。MinGW-w64 是 MinGW 的
// fork，加入 64 位支持，如今是事实上的主流。
//
// MinGW/MinGW-w64 的 GCC（含 MSYS2 的 MINGW64、UCRT64、CLANG64 环境），不会定义 __CYGWIN__ 宏，但
// 会定义 __MINGW32__（32/64 位都有），以及 64 位另有 __MINGW64__，并且会定义 _WIN32 宏。MinGW 自带
// 一整套 Win32 API 头文件（windows.h、winuser.h 等）和系统 DLL 的导入库（libkernel32.a、libuser32.a、
// ……），不需要安装 Windows SDK。SDK 是给 MSVC 用的，MinGW 的实现是自包含的（从 SDK/WINE 元数据生成，
// ABI 兼容）。编出的 exe 只依赖 Windows 自带的 msvcrt.dll/ucrtbase.dll，拿到任何裸 Windows 上直接跑。
//
// MinGW：GNU 编译器套件（GCC）的原生 Windows 移植版，附带可自由分发的导入库和头文件，用于构建原生
// Windows 应用程序；它还包含对 MSVC 运行时的扩展，以支持 C99 功能。mingw-w64 项目则为 gcc 提供了一
// 套完整的运行时环境，用于支持原生运行于 Windows 64 位和 32 位操作系统的二进制程序。Mingw-w64 是原
// 始 mingw.org 项目的进一步发展，该项目最初是为在 Windows 系统上支持 GCC 编译器而创建的。该项目于
// 2007 年分叉（fork），以提供 64 位支持和新版 API。此后，它获得了广泛的使用和普及。
//
// A native Windows port of the GNU Compiler Collection (GCC), with freely distributable import
// libraries and header files for building native Windows applications; includes extensions to the
// MSVC runtime to support C99 functionality. The mingw-w64 project is a complete runtime environment
// for gcc to support binaries native to Windows 64-bit and 32-bit operating systems. Mingw-w64 is
// an advancement of the original mingw.org project, which was created to support the GCC compiler
// on Windows systems. It was forked in 2007 in order to provide 64-bit support and newer APIs. It
// has since then gained wide use and distribution.
//
// MSYS2（Minimal SYStem 2）是一个在 Windows 平台上提供类 Unix 环境和开发工具的软件。MSYS2 提供了类
// 似于 Unix/Linux 系统的 shell 环境，以及一系列 Unix 风格的命令行工具，这使得开发者可以在 Windows
// 上使用熟悉的 Unix 命令和操作方式进行开发和管理工作。它集成了 Pacman 包管理器，这是 Arch Linux 所
// 使用的包管理工具。借助 Pacman，你能够轻松地安装、更新和删除软件包，并且可以方便地管理软件包之间的
// 依赖关系。通过包管理器，你可以获取到大量的开源软件和开发工具，如 GCC 编译器、Python、Ruby 等。MSYS2
// 包含了一系列的开发工具，如 GCC、GDB、Make 等，这些工具是进行 C、C++ 等语言开发所必需的。同时，它
// 还支持多种编程语言的开发环境，如 Python、Ruby、Perl 等。与 MinGW 相比，MSYS2 不仅提供了编译工具，
// 还提供了完整的类 Unix 环境和丰富的开发工具。
//
// MSYS2 不只提供一套 GCC，它内部有多个"环境"：MINGW64、UCRT64、CLANG64 等环境用的是 mingw-w64 工具
// 链（产出原生 Windows 程序，不定义 __CYGWIN__）；而 MSYS 环境自带的 GCC 是 Cygwin 血统的，才定义
// __CYGWIN__（同时定义 __MSYS__）。而且 MSYS2 官方正在 "回归 Cygwin"：2025 年起 MSYS 环境的 C/C++
// 会同时定义 __MSYS__ 和 __CYGWIN__，默认三元组也从 x86_64-pc-msys 改成了 x86_64-pc-cygwin。
//
// MSYS2 是 MSYS 的现代版：基于 Cygwin 的运行时 + Arch Linux 的 pacman 包管理器，并且把 mingw-w64
// 的各环境（MINGW64/UCRT64/CLANG64/CLANGARM64）打包在一起，现在它已是获取 mingw-w64 的主要渠道。
// MSYS、MSYS2、Cygwin 的 POSIX 核心是同源的。MinGW 的工具本身（gcc、bash、make）是 Unix 程序，需要
// fork、管道等 POSIX 语义才能运行，而 Windows 原生没有。于是 MinGW 团队做了 MSYS：一个从 Cygwin 裁
// 剪出来的最小环境，核心 DLL（msys-2.0.dll，Cygwin 的近亲）让这些 GNU 构建工具能跑起来。它的使命只
// 是跑 configure 脚本和构建工具，不是用来给最终程序提供 POSIX 环境。
//
// MSYS2 GCC（MINGW64/UCRT64/CLANG64 等环境）同 MinGW 一样可以原生调用 Win32 API，这就是这些环境的
// 用途。工具链就是 mingw-w64，和 WinGW 的情况完全一样，这是 MSYS2 里写 Windows 应用的正确环境。而
// MSYS2 GCC（MSYS 环境）可以原生调用 Win32 API，类 Cygwin 方式，但产物依赖 msys-2.0.dll，不建议用
// 于最终应用。MSYS 环境（x86_64-pc-msys/x86_64-pc-cygwin 工具链）是 Cygwin 血统，技术上调 Win32 API
// 可行，但产物依赖 msys-2.0.dll，定位是跑 bash/configure/make 等构建工具，不是给你发布应用的。
//
// MSYS2 is a collection of tools and libraries providing you with an easy-to-use environment for
// building, installing and running native Windows software. It consists of a command line terminal
// called mintty, bash, version control systems like git and subversion, tools like tar and awk and
// even build systems like autotools, all based on a modified version of Cygwin. Despite some of
// these central parts being based on Cygwin, the main focus of MSYS2 is to provide a build environment
// for native Windows software and the Cygwin-using parts are kept at a minimum. MSYS2 provides
// up-to-date native builds for GCC, mingw-w64, CPython, CMake, Meson, OpenSSL, FFmpeg, Rust, Ruby,
// just to name a few. The unixy tools in MSYS2 are directly based on Cygwin, so there is some overlap
// there. While Cygwin focuses on building Unix software on Windows as is, MSYS2 focuses on building
// native software built against the Windows APIs.
//
// MSYS2 是一套工具和库的集合，为你提供一个易用的环境，用于构建、安装和运行原生 Windows 软件。它包含
// 一个名为 mintty 的命令行终端、bash、git 和 subversion 等版本控制系统、tar 和 awk 等工具，甚至 autotools
// 这样的构建系统，所有这些都基于一个修改版的 Cygwin。尽管这些核心部分有些基于 Cygwin，但 MSYS2 的主
// 要关注点是为原生 Windows 软件提供构建环境，因此基于 Cygwin 的部分被保持在最低限度。MSYS2 为 GCC、
// mingw-w64、CPython、CMake、Meson、OpenSSL、FFmpeg、Rust、Ruby 等提供最新的原生构建版本，这只是其
// 中几例。MSYS2 中的类 Unix 工具直接基于 Cygwin，因此二者存在一些重叠。Cygwin 专注于在 Windows 上按
// 原样构建 Unix 软件，而 MSYS2 专注于构建基于 Windows API 的原生软件。

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
#define prh_plat_windows 1
#endif

#ifdef __NGAGE__
#define prh_plat_ngage 1 // nokia n-gage
#undef prh_plat_windows
#endif

#ifdef __MSDOS__
#define prh_plat_dos 1
#undef prh_plat_windows
#endif

// https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html
// 4.2.12 __has_include、__has_include_next
//
// 特殊操作符 __has_include(operand) 和 __has_include_next(operand) 可用于 #if 和 #elif 表达式中，
// 分别测试其操作数所引用的头文件能否用 #include 和 #include_next 指令包含。在其他上下文中使用这些
// 操作符是无效的。操作数的形式分别与 #include 和 #include_next 指令中的文件形式相同；如果可以包含
// 该头文件，操作符求值为非零值，否则求值为零。注意，能够包含某个头文件，并不意味着该头文件不包含会
// 导致预处理失败的非法构造或 #error 指令。
//
// __has_include 和 __has_include_next 操作符自身，不带任何操作数或括号时，可作为预定义宏使用，从而
// 可以在可移植代码中测试对它们的支持。因此，推荐的使用方式如下：
//      #if defined(__has_include)
//      #if __has_include(<stdatomic.h>)
//      #include <stdatomic.h>
//      #endif
//      #endif
//
// 只有当所使用版本的 GCC（或其他编译器）支持该操作符时，第一个 #if 测试才会成功。只有在这个测试成
// 功之后，才把 __has_include 用作预处理操作符才是有效的。因此，像下面那样把两个测试合并成一个表达
// 式，只对支持该操作符的编译器有效，对不支持的编译器则无效。__has_include_next 同理。
//      #if defined __has_include && __has_include("header.h") // 不可移植
//      #endif

#if defined(prh_plat_windows)

#if defined(prh_msc_version) && defined(__has_include) // find out compiling for WinRT, GDK, or non-WinRT/GDK
    #if __has_include(<winapifamily.h>)
        #define prh_impl_have_winapifamily_h 1
    #else
        #define prh_impl_have_winapifamily_h 0
    #endif
#elif defined(prh_msc_version) && (prh_msc_version >= 1700 && !_USING_V110_SDK71_) // _USING_V110_SDK71_ means we are using the Windows XP toolset
    #define prh_impl_have_winapifamily_h 1
#else
    #define prh_impl_have_winapifamily_h 0
#endif

#if prh_impl_have_winapifamily_h
    #include <winapifamily.h> // platform winrt or uwp
    #define prh_impl_winapi_family_winrt (!WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP))
    #define prh_impl_winapi_family_phone (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#else
    #define prh_impl_winapi_family_winrt 0
    #define prh_impl_winapi_family_phone 0
#endif

// https://learn.microsoft.com/en-us/gaming/gdk/docs/gdk-dev/development-downloads/development-downloads-gdk
// https://learn.microsoft.com/en-us/gaming/gdk/docs/gdk-dev/get-started/get-started-home
// https://learn.microsoft.com/en-us/gaming/gdk/docs/gdk-dev/intro/welcome?view=gdk-2604
//
// Microsoft GDK（Game Development Kit）是微软的游戏开发套件，不是操作系统，而是一套面向"游戏"这个
// 特定场景的 SDK、工具链和运行时。它是老牌 XDK（Xbox Development Kit）的继任者，核心目标是一次开发，
// 同时覆盖 Windows PC 和 Xbox 主机。它包含的内容大致有：
//  1.  GameDK / Xbox Game Runtime API：游戏版系统 API（窗口/输入/存储/网络/成就等），设计上可跨 PC
//      与主机
//  2.  DirectX 12 Agility SDK、DirectStorage、GameInput 等游戏专用组件
//  3.  Xbox Live 服务接口（成就、多人、云存档）
//  4.  打包与部署工具（MSIXvc 等）、性能分析工具（PIX）
//  5.  授权形式：公开版 GDK（PC + 经 ID@Xbox 上 Xbox），GDKX 附带完整 Xbox 主机开发权限（需开发者
//      计划和专用开发机）
//
// 背景：Xbox 的系统本身就是基于 Windows 内核的（OneCore 家族），所以微软才能把两套平台统一成一套开
// 发套件。Microsoft GDK 能调用普通 Win32 API 吗？
//
//      目标                能否用 Win32 API    说明
//      GDK → Windows PC    完全可以            产物就是普通 Win32 程序，全套 Win32/第三方库随便用
//      GDK → Xbox 主机     只有白名单子集      Xbox 是锁定的应用沙箱，大量 Win32 API 被禁用
//
// Xbox 上的限制细节：游戏运行在一个受限的"游戏分区"里，许多常规 Win32 调用不可用：注册表访问、创建
// 进程/线程的部分能力、任意路径文件 I/O（被虚拟化到游戏存储区）、LoadLibrary 任意 DLL 等都会被拒。
// 微软提供一份允许使用的 API 清单（API whitelist），只有清单内的 Win32/C 运行时 API 能保证可用，未
// 列出的可能在运行时报错，或在认证（certification）时被拒。主机上缺失的能力由 GameDK API 补齐：比
// 如输入要走 GameInput（而不是 RawInput/DirectInput）、窗口管理由系统接管（游戏实际上没有自己的顶层
// 窗口句柄）、存储要走游戏存储 API。代码层面的体现就是条件编译：
//      #if defined(_GAMING_XBOX)
//          // Xbox：只用白名单 Win32 + GameDK API
//      #elif defined(_GAMING_DESKTOP) || defined(_WIN32)
//          // PC GDK：随便用 Win32
//      #endif
//
// GDK 是 "微软把 PC 和 Xbox 游戏开发统一起来的一套 SDK"。在 PC 上它编译出的就是普通 Win32 程序；在
// Xbox 主机上则跑在锁定的沙箱中，只能调用获准的 Win32 API 子集 + GameDK 专有 API。

// Windows RT 是微软 2012 年随 Windows 8 推出的 ARM 架构桌面系统（代表作：Surface RT）。界面和 Windows
// 8 几乎一样，但不能运行传统的 x86 Win32 桌面程序，只能运行"应用商店（Store）应用"和预装的 Office。
// 系统完全锁定：第三方程序不允许进入桌面环境，只能走 Store 的沙箱应用模型。可以把它理解为 "ARM 版
// Windows 8 + 强制沙箱"。由于生态太差（没有传统软件可跑），2015 年左右被微软放弃，没有真正的后继版
// 本（后续 ARM Windows 即现在的 Windows 10/11 ARM64，是完整桌面系统，可跑 Win32）。
//
// UWP（Universal Windows Platform，通用 Windows 平台）是微软 2015 年随 Windows 10 推出的应用平台，
// 目标是 "一次编写，处处运行"，同一套应用可以跑在 PC、平板、Xbox、HoloLens、IoT、Windows 10 Mobile
// 上。核心特征：
//  1.  沙箱化运行：应用有受限的权限模型，文件系统访问要走代理 API（如 StorageFile），不能直接读写
//      任意路径，注册表、进程创建、全局钩子等都受限制或禁止
//  2.  受限的 API 集：主要使用 WinRT/UWP API 层，而不是完整的 Win32 API，只允许调用一个白名单子集，
//      这一点和前面说的 Xbox GDK 沙箱是同一套思路，都源于 Windows 的 OneCore 核心
//  3.  Store 分发、应用容器：传统上必须经 Microsoft Store 安装（企业侧载除外），运行在应用容器中
//  4.  无传统窗口模型：UI 基于 CoreWindow/XAML，没有 HWND 那套桌面窗口机制
//
// UWP 与 Win32 的对比
//                  Win32 桌面应用          UWP 应用
//      分发        任意（exe 直接装）      主要走 Microsoft Store
//      权限        完全信任，什么都能干    沙箱 + 能力声明（capabilities）
//      API         全套 Win32              WinRT API + Win32 白名单子集
//      窗口        HWND/消息循环           CoreWindow/CoreDispatcher
//      运行平台    Windows 桌面            PC/Xbox/HoloLens/IoT/（已死的手机）
//
// 现状，UWP 的风头已经过去了，微软后来转向：
//  1.  Desktop Bridge / MSIX：把传统 Win32 应用打包上架 Store，沙箱限制放松
//  2.  Windows App SDK（原 Project Reunion）+ WinUI 3：让新应用用现代 UI/新 API，但仍是普通桌面应
//      用，不再是沙箱 UWP
//  3.  Xbox 上的 UWP 只剩"开发者模式"（Dev Mode）跑自制应用的场景，游戏开发早已转向 GDK
//
// Windows RT（2012）→ UWP（2015）→ GDK/Xbox 沙箱（2016 至今），都是微软 "统一 Windows 核心（OneCore）
// + 受控 API 沙箱" 的尝试：给不同设备提供同一套锁定、安全、可认证的应用环境。Win32 则始终作为 "完全
// 信任" 的传统模型与它们并存。

#if prh_impl_winapi_family_winrt
    #error "Windows RT/UWP not supported"
#elif defined(_GAMING_XBOX_XBOXONE)
    #define prh_plat_xboxone 1
#elif defined(_GAMING_XBOX_SCARLETT)
    #define prh_plat_xboxseries 1
#elif defined(_GAMING_DESKTOP)
    #define prh_plat_wingdk 1
#else
    #define prh_plat_win32 1 // desktop windows, win32 api (also cover 64-bit windows)
#endif

#if defined(prh_plat_wingdk) || defined(prh_plat_xboxone) || defined(prh_plat_xboxseries)
    #define prh_plat_msft_gdk // generic "any GDK" from a platform-specific GDK, Microsoft GDK on any platfrom
#endif

#endif // prh_plat_windows

#undef prh_lit_endian
#undef prh_big_endian

#if defined(prh_arch_x86) || defined(prh_arch_x64) || defined(prh_arch_arm) || defined(prh_arch_a64)
    #define prh_lit_endian 1
#endif

#if !defined(prh_lit_endian) && defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define prh_lit_endian 1
#endif

#if !defined(prh_lit_endian) && defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define prh_big_endian 1
#endif

#if !defined(prh_lit_endian) && !defined(prh_big_endian)
    #if defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MIPSEB__)) || defined(__ppc__) || defined(__POWERPC__) || defined(__powerpc__) || defined(__PPC__) || defined(__sparc__) || defined(__sparc)
        #define prh_big_endian 1
    #endif
#endif

#if !defined(prh_lit_endian) && !defined(prh_big_endian) && defined(prh_using_lit_endian)
    #define prh_lit_endian 1
#endif

#if !defined(prh_lit_endian) && !defined(prh_big_endian) && defined(prh_using_big_endian)
    #define prh_big_endian 1
#endif

#if !defined(prh_lit_endian) && !defined(prh_big_endian)
    #error "unknown byte order endian"
#endif

// __cplusplus
//  199711L(until C++11)
//  201103L(C++11)
//  201402L(C++14)
//  201703L(C++17)
//  202002L(C++20)
//  202302L(C++23)
//
// __STDC_VERSION__
//  199409L (C95)
//  199901L (C99)
//  201112L (C11)
//  201710L (C17)
//  202311L (C23)

#undef prh_inline
#undef prh_noline

// https://learn.microsoft.com/en-us/cpp/cpp/inline-functions-cpp
// https://gcc.gnu.org/onlinedocs/gcc/Inline.html
// https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html#Common-Function-Attributes
// https://gcc.gnu.org/onlinedocs/gcc/Alternate-Keywords.html
// https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html
// https://clang.llvm.org/docs/UsersManual.html#differences-between-various-standard-modes

#if defined(prh_msc_version) || defined(__BORLANDC__) || defined(__DMC__) || defined(__SC__) || defined(__WATCOMC__) || defined(__LCC__) || defined(__DECC) || defined(__CC_ARM)
    #if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L // C99 keyword
        #define prh_impl_inline_keyword inline
    #else
        #define prh_impl_inline_keyword __inline
    #endif
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    #define prh_impl_inline_keyword __inline__ // the gcc __inline__ exists even -ansi defined, the clang __inline__ is recognized in all modes
#else
    #define prh_impl_inline_keyword inline
#endif

#if defined(prh_msc_version)
    #define prh_inline static __forceinline
    #define prh_noline __declspec(noinline)
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    #define prh_inline static __inline__ __attribute__((always_inline))
    #define prh_noline __attribute__((noinline)) // always_inline 和 noinline 在 gcc 3.1.1+ 是存在的 (https://gcc.gnu.org/onlinedocs/gcc-3.1.1/gcc/Function-Attributes.html)
#else
    #define prh_inline static prh_impl_inline_keyword
    #define prh_noline
#endif

#undef prh_thread_local

#if defined(__cplusplus) && __cplusplus >= 201103L // C++11 keyword
    #define prh_thread_local thread_local
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    #define prh_thread_local thread_local
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L // C11 keyword
    #define prh_thread_local _Thread_local
#elif defined(prh_msc_version)
    #define prh_thread_local __declspec(thread)
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    #define prh_thread_local __thread
#else
    #error "thread local unsupported"
#endif

// https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html
//
// visibility ("visibility_type") 该属性影响其所附着声明的链接（linkage）方式。支持四种 visibility_type
// 值：default、hidden、protected 或 internal 可见性。
//
//      void __attribute__ ((visibility ("protected"))) f () { /* Do something. */; }
//      int i __attribute__ ((visibility ("hidden")));
//
// visibility_type 的可能取值对应 ELF gABI 中的可见性设置。
//
//  1.  default（默认）
//
// 默认可见性是对象文件格式的正常情况。此值可供 visibility 属性使用，以覆盖那些可能改变实体假定可见
// 性的其他选项。在 ELF 上，默认可见性意味着该声明对其他模块可见；在共享库中，意味着被声明的实体可以
// 被覆盖。在 Darwin 上，默认可见性意味着该声明对其他模块可见。默认可见性在语言层面（C/C++）对应 "外
// 部链接（external linkage）"。
//
//  2.  hidden（隐藏）
//
// 隐藏可见性表明被声明的实体将具有一种新的链接形式，我们称之为"隐藏链接（hidden linkage）"。具有隐
// 藏链接的对象的两处声明，只有在位于同一共享对象中时才会指向同一个对象。
//
//  3.  internal（内部）
//
// 内部可见性类似隐藏可见性，但带有额外的处理器特定语义。除非 psABI 另有规定，GCC 将内部可见性定义
// 为：该函数永远不会从另一个模块被调用。可将其与隐藏函数对比——隐藏函数虽然不能直接被其他模块引用，
// 但可以通过函数指针被间接引用。通过指示某函数不能从模块外被调用，GCC 例如可以省略 PIC 寄存器的加载
// 操作，因为已知调用函数已加载了正确的值。
//
//  4.  protected（受保护）
//
// 受保护可见性类似默认可见性，区别在于它表明：定义模块内部的引用将绑定到该模块中的定义。即被声明的
// 实体不能被其他模块覆盖。
//
// 所有可见性在许多（但不是全部）ELF 目标上受支持（取决于汇编器是否支持 .visibility 伪操作）。默认
// 可见性到处都支持。隐藏可见性在 Darwin 目标上受支持。visibility 属性应仅应用于原本就具有外部链接的
// 声明。该属性的应用应保持一致，即同一实体不应以该属性的不同设置进行声明。
//
// 在 C++ 中，visibility 属性不仅适用于函数和对象，也适用于类型，因为 C++ 中类型也有链接属性。类的
// 可见性不得高于其非静态数据成员类型和基类的可见性；类成员的可见性默认与其所属类一致。此外，没有显
// 式可见性的声明，其可见性受其类型的可见性限制。
//
// 在 C++ 中，你可以用 visibility 属性标记类的成员函数和静态成员变量。如果你知道某个特定方法或静态成
// 员变量只应从某一个共享对象中使用，这会很有用——你可以把它标记为 hidden，而类的其余部分保持默认可见
// 性。必须小心避免破坏一次定义规则（One Definition Rule）；例如，在不把整个类标记为 hidden 的情况下
// 把一个内联方法标记为 hidden，通常是没意义的。
//
// C++ 的命名空间声明也可以带 visibility 属性。该属性只应用于该特定命名空间体，不应用于同一命名空间
// 的其他定义；它等价于在该命名空间定义前后使用 #pragma GCC visibility（参见"可见性 pragma"一节）。
// 在 C++ 中，如果模板实参的可见性受限，该限制会隐式传播到模板实例化。否则，模板实例化和特化默认采用
// 其模板的可见性。如果模板和外围类都有显式可见性，则使用来自模板的可见性。

#undef prh_extern_api

// C++ 编译器会对函数名和变量名进行改编（mangle），这在链接的时候会导致严重的问题。举个例子，假设一
// 个 DLL 使用 C++ 编写的，而可执行文件使用 C 编写的。在构建 DLL 的时候，编译器会对函数名进行改编，
// 但在构建可执行文件的时候，编译器不会对函数名进行改编。当链接器试图链接可执行文件的时候，会发现可
// 执行文件引用了一个不存在的符号而报错。extern "C" 用来告诉编译器不要对变量名或函数名进行改编，这样
// 用 C 或 C++ 或任何编程语言编写的可执行模块都可以访问该变量或函数。

#ifdef __cplusplus
#define prh_impl_extern_keyword extern "C"
#else
#define prh_impl_extern_keyword extern
#endif

#if defined(prh_msc_version)
    #if defined(prh_export_dll_apis)
        #define prh_extern_api prh_impl_extern_keyword __declspec(dllexport)
    #elif defined(prh_import_dll_apis)
        #define prh_extern_api prh_impl_extern_keyword __declspec(dllimport)
    #else
        #define prh_extern_api prh_impl_extern_keyword
    #endif
#elif defined(prh_plat_windows) && defined(prh_gcc_version)
    #if defined(prh_export_dll_apis)
        #define prh_extern_api prh_impl_extern_keyword __attribute__((dllexport))
    #elif defined(prh_import_dll_apis)
        #define prh_extern_api prh_impl_extern_keyword __attribute__((dllimport))
    #else
        #define prh_extern_api prh_impl_extern_keyword
    #endif
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    #if defined(prh_export_dll_apis)
        #define prh_extern_api prh_impl_extern_keyword __attribute__((visibility("default")))
    #else
        #define prh_extern_api prh_impl_extern_keyword
    #endif
#else
    #define prh_extern_api prh_impl_extern_keyword
#endif

#undef prh_impl_extern_keyword

// alignas(size) alignas(type)
//
// When used in a declaration, the declared object will have its alignment requirement set to 1) the
// specified size, unless it is zero; 2) the alignment requirement of the type; except when this would
// weaken the alignment the type would have had naturally.
//
// When multiple alignas specifiers appear in the same declaration, the strictest one is used. In
// C++, the alignas specifier may also be applied to the declarations of class/struct/union types
// and enumerations. This is not supported in C, but the alignment of a struct type can be controlled
// by using alignas in a member declaration.

#undef prh_alignas

#if defined(__cplusplus) && __cplusplus >= 201103L // C++11 keyword
    #define prh_alignas(size) alignas(size)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    #define prh_alignas(size) alignas(size)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L // C11 keyword
    #define prh_alignas(size) _Alignas(size)
#elif defined(prh_msc_version)
    // Before Visual Studio 2015 you could use the Microsoft-specific keywords
    // __alignof and __declspec(align) to specify an alignment greater than
    // the default. Starting in Visual Studio 2015 you should use the C++11
    // standard keywords alignof and alignas for maximum code portability.
    #define prh_alignas(size) __declspec(align(size))
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    // The aligned attribute specifies a minimum alignment (in bytes) for
    // variables of the specified type. When specified, alignment must be a
    // power of 2. Specifying no alignment argument implies the maximum
    // alignment for the target, which is often, but by no means always,
    // 8 or 16 bytes.
    // Note that the alignment of any given struct or union type is required
    // by the ISO C standard to be at least a perfect multiple of the lowest
    // common multiple of the alignments of all of the members of the struct
    // or union in question. This means that you can effectively adjust the
    // alignment of a struct or union type by attaching an aligned attribute
    // to any one of the members of such a type. 请注意，ISO C 标准要求任何给定
    // 的结构体或联合体类型的对齐方式，至少要是该结构体或联合体中所有成员对齐方式的
    // 最小公倍数的整数倍。这意味着，你可以通过为结构体或联合体类型的任意一个成员附
    // 加 aligned 属性，来有效调整该结构体或联合体类型的对齐方式。
    #define prh_alignas(size) __attribute__ ((aligned (size)))
#else
    #error "alignas unsupported"
#endif

// alignof(type)
//
// Returns the alignment requirement of the type named by the type. If type is an array type, the
// result is the alignment requirement of the array element type. The type cannot be function type
// or an incomplete type. The result is an integer constant of type size_t.
//
// The operand is not evaluated (so external identifiers used in the operand do not have to be
// defined). The use of alignof with expressions is allowed by some C compilers as a non-standard
// extension.

#undef prh_alignof

#if defined(__cplusplus) && __cplusplus >= 201103L // C++11 keyword
    #define prh_alignof(type) alignof(type)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    #define prh_alignof(type) alignof(type)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L // C11 keyword
    #define prh_alignof(type) _Alignof(type)
#elif defined(prh_msc_version)
    // Before Visual Studio 2015 you could use the Microsoft-specific keywords
    // __alignof and __declspec(align) to specify an alignment greater than
    // the default. Starting in Visual Studio 2015 you should use the C++11
    // standard keywords alignof and alignas for maximum code portability.
    // The alignof(type) operator returns the alignment in bytes of the
    // specified type as a value of type size_t.
    #define prh_alignof(type) __alignof(type)
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    // The keyword __alignof__ determines the alignment requirement of a
    // function, object, or a type, or the minimum alignment usually required
    // by a type. Its syntax is just like sizeof and C11 _Alignof.
    #define prh_alignof(type) __alignof__(type)
#else
    #error "alignof unsupported"
#endif

#undef prh_packed_struct
#undef prh_packing_reset

#if defined(prh_msc_version)
    // Pragma directives specify machine-specific or operating system-specific
    // compiler features. A line that starts with #pragma specifies a pragma
    // directive. The Microsoft-specific __pragma keyword enables you to code
    // pragma directives within macro definitions. The standard _Pragma
    // preprocessor operator, introduced in C99 and adopted by C++11, is
    // similar.
    //      #pragma token-string
    //      __pragma(token-string)
    //      _Pragma(string-literal)
    // pack pragma takes effect at the first struct, union, or class
    // declaration after the pragma is seen.
    //      #pragma pack(2)
    //      struct T { int i; short j; double k; };
    // The sample shows how to use the push, pop, and show syntax.
    //      #pragma pack()               // n defaults to 8; equivalent to /Zp8
    //      #pragma pack(show)           // C4810
    //      #pragma pack(4)              // n = 4
    //      #pragma pack(show)           // C4810
    //      #pragma pack(push, r1, 16)   // n = 16 pushed to stack labeled r1
    //      #pragma pack(show)           // C4810
    //      pop until r1 is removed, and set current packing alignment to n = 2
    //      it equivalent to #pragma pack(pop, r1) followed by #pragma pack(2)
    //      #pragma pack(pop, r1, 2)
    //      #pragma pack(show)
    #define prh_packed_struct __pragma(pack(push, 1)) struct
    #define prh_packing_reset() __pragma(pack(pop))
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    // An attribute specifier list may appear as part of a struct, union or
    // enum specifier.
    // It may go either immediately after the struct, union or enum keyword,
    // or after the closing brace. The former syntax is preferred.
    // In the following example struct my_packed_struct’s members are packed
    // closely together, but the internal layout of its s member is not
    // packed — todo that, struct my_unpacked_struct needs to be packed too.
    // struct my_unpacked_struct { char c; int i; };
    // struct __attribute__ ((packed)) my_packed_struct {
    //      char c;
    //      int i;
    //      struct my_unpacked_struct s;
    // };
    // For compatibility with Microsoft Windows compilers, GCC supports a set
    // of #pragma pack directives that change the maximum alignment of members
    // of structures (other than zero-width bit-fields), unions, and classes
    // subsequently defined. The n value below specifies the new alignment in
    // bytes and may have the value 1, 2, 4, 8, and 16. A value of 0 is also
    // permitted and indicates the default alignment (as if no #pragma pack
    // were in effect) should be used.
    //      #pragma pack(n)
    //      #pragma pack()
    //      #pragma pack(push[,n])
    //      #pragma pack(pop)
    #define prh_packed_struct struct __attribute__ ((packed))
    #define prh_packing_reset()
#else
    // Implementation defined behavior is controlled by #pragma directive.
    //      #pragma pragma_params
    //      _Pragma(string-literal)
    // Non-standard pragmas #pragma pack, this family of pragmas control the
    // maximum alignment for subsequently defined structure and union members.
    //      #pragma pack(arg)
    //      #pragma pack()
    //      #pragma pack(push)
    //      #pragma pack(push, arg)
    //      #pragma pack(pop)
    #define prh_packed_struct _Pragma("pack(push, 1)") struct
    #define prh_packing_reset() _Pragma("pack(pop)")
#endif

#undef prh_typeof

#if defined(__cplusplus) && __cplusplus >= 201103L // C++11 keyword
    // Note that if the name of an object is parenthesized, it is treated as
    // an ordinary lvalue expression, thus decltype(x) and decltype((x)) are
    // often different types.
    // decltype is useful when declaring types that are difficult or impossible
    // to declare using standard notation, like lambda-related types or types
    // that depend on template parameters.
    // struct A { double x; };
    // const A* a;
    // decltype(a->x) y;       // type of y is double (declared type)
    // decltype((a->x)) z = y; // type of z is const double& (lvalue expr)
    #define prh_typeof(expr) decltype(expr)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    // typeof and typeof_unqual are collectively called the typeof operators.
    // The typeof operators cannot be applied to bit-field members. If the type
    // of the operand is a variably modified type, the operand is evaluated;
    // otherwise, the operand is not evaluated. The result of the typeof_unqual
    // operator is the non-atomic unqualified type that would result from the
    // typeof operator. The typeof operator preserves all qualifiers.
    // typeof_unqual 运算符的结果是 typeof 运算符所产生结果的非原子且无限定符的类
    // 型。typeof 运算符会保留所有限定符。
    #define prh_typeof(expr) typeof(expr)
#elif defined(prh_msc_version)
    // __typeof__ in msc requires Visual Studio 17.9 or later, or cl.exe
    // version 19.39.33428 or later. This can be meet after installing the
    // latest version of Visual Studio Community 2022.
    #define prh_typeof(expr) __typeof__(expr)
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    #define prh_typeof(expr) __typeof__(expr)
#else
    #error "typeof unsupported"
#endif

#undef prh_fastcall
#undef prh_naked_fastcall
#undef prh_fastcall_typedef
#undef prh_asm_begin
#undef prh_asm_end

#if defined(prh_msc_version)
    #if defined(prh_arch_x64)
        #define prh_naked_fastcall(ret) __declspec(naked) ret
        #define prh_fastcall(ret) ret
        #define prh_fastcall_typedef(ret, name) typedef ret (*name)
    #elif defined(prh_arch_x86)
        #define prh_naked_fastcall(ret) __declspec(naked) ret __fastcall
        #define prh_fastcall(ret) ret __fastcall
        #define prh_fastcall_typedef(ret, name) typedef ret (__fastcall *name)
    #endif
    #define prh_asm_begin() __asm {
    #define prh_asm_end() }
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    #if defined(prh_arch_x64)
        #define prh_naked_fastcall(ret) __attribute__((naked)) ret
        #define prh_fastcall(ret) ret
        #define prh_fastcall_typedef(ret, name) typedef ret (*name)
    #elif defined(prh_arch_x86)
        #define prh_naked_fastcall(ret) __attribute__((naked,fastcall)) ret
        #define prh_fastcall(ret) __attribute__((fastcall)) ret
        #define prh_fastcall_typedef(ret, name) typedef ret (__attribute__((fastcall)) *name)
    #endif
    #define prh_asm_begin() __asm__ (
    #define prh_asm_end() );
#endif

#undef prh_fallthrough

#if (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202000L)
    #define prh_fallthrough [[fallthrough]]
#else
    #if defined(__has_attribute) && !defined(__SUNPRO_C) && !defined(__SUNPRO_CC)
        #define prh_impl_has_fallthrough __has_attribute(__fallthrough__)
    #else
        #define prh_impl_has_fallthrough 0
    #endif
    #if prh_impl_has_fallthrough && ((defined(__GNUC__) && __GNUC__ >= 7) || (defined(__clang_major__) && __clang_major__ >= 10))
        #define prh_fallthrough __attribute__((__fallthrough__))
    #else
        #define prh_fallthrough do {} while (0)
    #endif
    #undef prh_impl_has_fallthrough
#endif

#undef prh_restrict

#if defined(restrict) || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
    #define prh_restrict restrict
#elif defined(prh_msc_version) || defined(prh_gcc_version) || defined(prh_clang_version)
    #define prh_restrict __restrict // restrict is from C99 and __restrict works both msc and gcc
#else
    #define prh_restrict
#endif

#undef prh_unused

#if defined(prh_gcc_version) || defined(prh_clang_version)
    #define __attribute__ ((unused))
#else
    #define prh_unused(a) ((void)(a))
#endif

#undef prh_struct_object

#if defined(__cplusplus)
#define prh_struct_object(type, ...) (type{__VA_ARGS__})
#else
#define prh_struct_object(type, ...) ((type){__VA_ARGS__})
#endif

#undef prh_null

#if defined(__cplusplus)
    // The macro NULL is an implementation-defined null pointer constant.
    // In C, the macro NULL may have the type void*, but that is not
    // allowed in C++ because null pointer constants cannot have that
    // type.
    #if __cplusplus >= 201103L // C++11 keyword
        #define prh_null nullptr
    #else
        #define prh_null 0
    #endif
#else
    // The keyword nullptr denotes a predefined null pointer constant. It
    // is a non-lvalue of type nullptr_t. nullptr can be converted to a
    // pointer types or bool, where the result is the null pointer value
    // of that type or false respectively.
    // The macro NULL is an implementation-defined null pointer constant,
    // which may be an integer constant expression with the value ​0​; an
    // integer constant expression with the value ​0​ cast to the type void*;
    // predefined constant nullptr (since C23).
    // POSIX requires NULL to be defined as an integer constant expression
    // with the value ​0​ cast to void*.
    #if __STDC_VERSION__ >= 202311L // C23 keyword
        #define prh_null nullptr
    #else
        #define prh_null ((void *)0)
    #endif
#endif

#if defined(__cplusplus)
    // bool true false are C++ keywords
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    // bool true false are C23 keywords
#elif !defined(bool)
    // Note that conversion to _Bool(until C23) bool(since C23) does not work
    // the same as conversion to other integer types: (bool)0.5 evaluates to
    // true, whereas (int)0.5 evaluates to ​0​.
    #define bool _Bool
    #define true 1
    #define false 0
#endif

// https://en.cppreference.com/w/cpp/error/assert
//
// The definition of the macro assert depends on another macro, NDEBUG, which
// is not defined by the standard library. If NDEBUG is defined as a macro name
// at the point in the source code where <cassert> or <assert.h> is included,
// the assertion is disabled: assert does nothing. Otherwise, the assertion is
// enabled.
//
// In one source file, you can define and undefine NDEBUG multiple times, each
// time followed by #include <cassert>, to enable or disable the assert macro
// multiple times in the same source file.
//
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/assert-macro-assert-wassert
//
// assert 宏通常用于在程序开发过程中识别逻辑错误。通过实现表达式参数仅在程序运行不正
// 确时评估为假，从而在出现意外条件时停止程序执行。可以通过定义宏 NDEBUG 在编译时关闭
// 断言检查。你可以通过使用 /DNDEBUG 命令行选项，在不修改源文件的情况下关闭 assert
// 宏。也可以通过在包含 <assert.h> 之前使用 #define NDEBUG 指令，在源代码中关闭
// assert 宏。
//
// 当表达式评估为假（0）时，assert 会打印一条诊断消息，并调用 abort 来停止程序执行。
// 如果表达式为真（非零），则不采取任何操作。诊断消息包括失败的表达式、源文件名以及断
// 言失败的行号。诊断消息以宽字符（wchar_t）形式打印。因此，即使表达式中包含 Unicode
// 字符，它也能按预期工作。诊断消息的目标取决于调用该例程的应用程序类型。控制台应用程
// 序通过 stderr 接收消息。在基于 Windows 的应用程序中，assert 调用 Windows 的
// MessageBox 函数来创建一个消息框以显示消息，该消息框包含三个按钮：中止（Abort）、
// 重试（Retry）和忽略（Ignore）。如果用户选择“中止”，程序将立即终止。如果用户选择
// “重试”，将调用调试器（如果启用了即时调试），用户可以调试程序。如果用户选择“忽略”，
// 程序将继续正常执行。在存在错误条件时点击“忽略”可能会导致未定义行为，因为调用代码的
// 前置条件未得到满足。要覆盖默认输出行为，无论应用程序类型如何，都可以调用
// _set_error_mode 来选择是将输出发送到 stderr 还是显示对话框。
//
// _assert 和 _wassert 函数是内部 CRT 函数。它们有助于减少对象文件中支持断言所需的
// 代码量。不建议直接调用这些函数。
//
// 当未定义 NDEBUG 时，assert 宏在 C 运行时库的发布版和调试版中都启用。当定义了
// NDEBUG 时，宏可用，但不会评估其参数，也没有任何效果。当启用时，assert 宏调用
// _wassert 来实现其功能。其他断言宏，如 _ASSERT、_ASSERTE 和 _ASSERT_EXPR 也
// 可用，但只有在定义了 _DEBUG 宏且代码链接了调试版的 C 运行时库时，才会评估传递
// 给它们的表达式。Other assertion macros, _ASSERT, _ASSERTE and _ASSERT_EXPR,
// are also available, but they only evaluate the expressions passed to them
// when the _DEBUG macro has been defined and when they are in code linked
// with the debug version of the C run-time libraries.
//
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/debug
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-debugging-techniques
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/assert-asserte-assert-expr-macros
//
// The compiler defines _DEBUG when you specify the /MTd or /MDd option. These
// options specify debug versions of the C run-time library.
//
// The C runtime (CRT) library provides extensive debugging support. To use one
// of the CRT debug libraries, you must link with /DEBUG and compile with /MDd,
// /MTd, or /LDd. The main definitions and macros for CRT debugging can be
// found in the<crtdbg.h> header file.
//
// _ASSERT_EXPR、_ASSERT 和 _ASSERTE 宏为应用程序提供了一种在调试过程中检查假设的简
// 洁机制。它们非常灵活，因为不需要用 #ifdef 语句将它们包围起来，以防止它们在应用程序
// 的零售版本中被调用。这种灵活性是通过使用 _DEBUG 宏实现的。_ASSERT_EXPR、_ASSERT
// 和 _ASSERTE 只有在编译时定义了 _DEBUG 宏时才可用。如果未定义 _DEBUG 宏，则在预处
// 理期间会移除对这些宏的调用。
//
// 如果结果为假（0），它们会打印一条诊断消息，并调用 _CrtDbgReportW 来生成调试报告。
// 除非你使用 _CrtSetReportMode 和 _CrtSetReportFile 函数指定了其他方式，否则消息
// 会出现在一个弹出式对话框中，这相当于设置了：
//      _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW); 将断言设置为弹窗
//
// 当目标是一个调试消息窗口且用户选择“重试”按钮时，_CrtDbgReportW 返回 1，这将导致
// _ASSERT_EXPR、_ASSERT 和 _ASSERTE 宏在启用了即时调试（JIT）的情况下启动调试器。
// _RPT 和 _RPTF 调试宏也可用于生成调试报告，但它们不评估表达式。_RPT 宏生成简单的报
// 告，而 _RPTF 宏在生成的报告中包含调用报告宏的源文件和行号。
//
// 虽然 _ASSERT_EXPR、_ASSERT 和 _ASSERTE 是宏，并且可以通过包含 <crtdbg.h> 来使
// 用，但当定义了 _DEBUG 宏时，应用程序必须链接到调试版本的 C 运行时库，因为这些宏调
// 用了其他运行时函数。

#if defined(_DEBUG)
    #undef NDEBUG // 启用标准断言
    #define PRH_DEBUG 1
#else
    #define NDEBUG 1 // 关闭标准断言
    #define PRH_DEBUG 0
#endif

#define prh_static_assert(const_expr) typedef int prh_impl_static_assert[(const_expr) ? 1 : -1]

#define PRH_I08_MIN ((prh_i08)-127-1)   // 0x80 128
#define PRH_I08_MAX ((prh_i08)127)      // 0x7f 127
#define PRH_I08_UMX ((prh_r08)255)      // 0xff 255
#define PRH_I16_MIN ((prh_i16)-32767-1) // 0x8000 32768
#define PRH_I16_MAX ((prh_i16)32767)    // 0x7fff 32767
#define PRH_I16_UMX ((prh_r16)65535)    // 0xffff 65535
#define PRH_I32_MIN ((prh_i32)-2147483647-1) // 0x8000_0000 2147483648
#define PRH_I32_MAX ((prh_i32)2147483647)    // 0x7fff_ffff 2147483647
#define PRH_I32_UMX ((prh_r32)4294967295)    // 0xffff_ffff 4294967295
#define PRH_I64_MIN ((prh_i64)-9223372036854775807LL-1) // 0x8000_0000_0000_0000  9223372036854775808
#define PRH_I64_MAX ((prh_i64)9223372036854775807)      // 0x7fff_ffff_ffff_ffff  9223372036854775807
#define PRH_I64_UMX ((prh_r64)18446744073709551615)     // 0xffff_ffff_ffff_ffff 18446744073709551615
typedef unsigned char prh_byte;
typedef unsigned char prh_r08;
typedef signed char prh_i08;
typedef unsigned short prh_r16;
typedef short prh_i16;
typedef unsigned int prh_r32;
typedef int prh_i32;
typedef unsigned long long prh_r64;
typedef long long prh_i64;

#if prh_arch_bits == 32
    #define PRH_IMT_MIN PRH_I32_MIN
    #define PRH_IMT_MAX PRH_I32_MAX
    #define PRH_IMT_UMX PRH_I32_UMX
    #define prh_imt_bits 32
    #define prh_imt_size 4
    typedef prh_i32 prh_imt;
    typedef prh_r32 prh_raw;
    typedef prh_i32 prh_int;
    typedef prh_r32 prh_reg;
    #define prh_int_bits 32
    #define prh_int_size 4
#elif prh_arch_bits == 64
    #define PRH_IMT_MIN PRH_I64_MIN
    #define PRH_IMT_MAX PRH_I64_MAX
    #define PRH_IMT_UMX PRH_I64_UMX
    #define prh_imt_bits 64
    #define prh_imt_size 8
    typedef prh_i64 prh_imt;
    typedef prh_r64 prh_raw;
    #ifdef prh_using_32_bit_memory_range
        typedef prh_i32 prh_int;
        typedef prh_r32 prh_reg;
        #define prh_int_bits 32
        #define prh_int_size 4
    #else
        typedef prh_i64 prh_int;
        typedef prh_r64 prh_reg;
        #define prh_int_bits 64
        #define prh_int_size 8
    #endif
#else
    #error "unsupported architecture"
#endif

#if prh_int_bits == 64
    #define PRH_INT_MIN PRH_I64_MIN
    #define PRH_INT_MAX PRH_I64_MAX
    #define PRH_INT_UMX PRH_I64_UMX
#elif prh_int_bits == 32
    #define PRH_INT_MIN PRH_I32_MIN
    #define PRH_INT_MAX PRH_I32_MAX
    #define PRH_INT_UMX PRH_I32_UMX
#else
    #error "unsupported int bits"
#endif

typedef prh_r32 prh_char;
typedef prh_raw prh_handle;
typedef float prh_f32;
typedef double prh_f64;
typedef prh_f32 prh_float;

prh_static_assert(sizeof(int) == 4);
prh_static_assert(sizeof(bool) == 1);
prh_static_assert(sizeof(prh_byte) == 1);
prh_static_assert(sizeof(prh_r08) == 1);
prh_static_assert(sizeof(prh_i08) == 1);
prh_static_assert(sizeof(prh_r16) == 2);
prh_static_assert(sizeof(prh_i16) == 2);
prh_static_assert(sizeof(prh_r32) == 4);
prh_static_assert(sizeof(prh_i32) == 4);
prh_static_assert(sizeof(prh_r64) == 8);
prh_static_assert(sizeof(prh_i64) == 8);
prh_static_assert(sizeof(prh_int) == sizeof(void *)); // signed pointer size type
prh_static_assert(sizeof(prh_reg) == sizeof(void *)); // unsigned pointer size type
prh_static_assert(sizeof(prh_char) == sizeof(prh_r32));
prh_static_assert(sizeof(prh_imt) == prh_arch_bits / 8); // architecture signed type with generic purpose regiter size
prh_static_assert(sizeof(prh_raw) == prh_arch_bits / 8); // architecture unsigned type with generic purpose register size
prh_static_assert(sizeof(prh_raw) == sizeof(prh_raw));
prh_static_assert(sizeof(prh_f32) == 4);
prh_static_assert(sizeof(prh_f64) == 8);
prh_static_assert(sizeof(prh_float) == 4);

typedef union {
    prh_f32 value; // <sign> 1.<mantissa> * 10 ^ <exponent>
#if defined(prh_lit_endian) // exponent = exponent + 127
    prh_r32 mantissa: 23, exponent: 8, sign: 1;
#else
    prh_r32 sign: 1, exponent: 8, mantissa: 23;
#endif
} prh_union_f32;

typedef union {
    prh_f64 value; // <sign> 1.<mantissa> * 10 ^ <Exponent>
#if defined(prh_lit_endian) // Exponent = exponent + 1023
    prh_r64 mantissa: 52, exponent: 11, sign: 1;
#else
    prh_r64 sign: 1, exponent: 11, mantissa: 52;
#endif
} prh_union_f64;

typedef struct {
    prh_byte *data;
    union {
        prh_reg size;
        prh_reg bytes;
        prh_reg count;
        prh_reg length;
    };
} prh_data;

typedef prh_data prh_view;

#define prh_offsetof(type, field) ((prh_reg)(&((type *)0)->field))

#define prh_macro_make_name(a, b) prh_impl_macro_make_name(a, b)
#define prh_macro_make_cstr(a) prh_impl_macro_make_cstr(a)
#define prh_impl_macro_make_name(a, b) a ## b
#define prh_impl_macro_make_cstr(a) #a

#define prh_arrlen(a) (sizeof(a)/sizeof((a)[0]))
#define prh_arrend(a) ((a) + prh_arrlen(a))
#define prh_arrelt(a, i) (a)[(prh_assert((i) >= 0 && (i) < prh_arrlen(a))), i]
#define prh_arrget(a) (a), prh_arrlen(a)

#ifdef __cplusplus
}
#endif
#endif // prh_impl_include_prelude_h

// FULL VERSION HISTORY
//
//   0.01 (2026-09-26) initial release for basic code
//

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2026 Godelder Brother (github.com/swdayu)
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/