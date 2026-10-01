// prh_prelude.h - v0.01 - public domain - swdayu <github.com/swdayu>
// No warranty implied, use at your own risk.

#ifndef prh_include_prelude_h
#define prh_include_prelude_h
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
//      __FLOAT_WORD_ORDER__
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
#undef prh_plat_openharmony
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

#if defined(__OHOS__)
#define prh_plat_openharmony 1
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

#if !defined(prh_lit_endian) && defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define prh_lit_endian 1
#endif

#if !defined(prh_lit_endian) && defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define prh_big_endian 1
#endif

#if !defined(prh_lit_endian) && !defined(prh_big_endian)
    #if defined(prh_using_lit_endian) || defined(prh_arch_x86) || defined(prh_arch_x64) || defined(prh_arch_arm) || defined(prh_arch_a64)
        #define prh_lit_endian 1
    #endif
#endif

#if !defined(prh_lit_endian) && !defined(prh_big_endian)
    #if defined(prh_using_big_endian) || defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MIPSEB__)) || defined(__ppc__) || defined(__POWERPC__) || defined(__powerpc__) || defined(__PPC__) || defined(__sparc__) || defined(__sparc)
        #define prh_big_endian 1
    #endif
#endif

#if !defined(prh_lit_endian) && !defined(prh_big_endian)
    #error "unknown byte order endian"
#endif

#undef prh_float_lit_endian
#undef prh_float_big_endian

#if !defined(prh_float_lit_endian) && defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define prh_float_lit_endian 1
#endif

#if !defined(prh_float_lit_endian) && defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define prh_float_big_endian 1
#endif

#if !defined(prh_float_lit_endian) && !defined(prh_float_big_endian) && defined(prh_using_float_lit_endian)
    #define prh_float_lit_endian 1
#endif

#if !defined(prh_float_lit_endian) && !defined(prh_float_big_endian) && defined(prh_using_float_big_endian)
    #define prh_float_big_endian 1
#endif

#if !defined(prh_float_lit_endian) && !defined(prh_float_big_endian)
    #if defined(prh_lit_endian)
        #define prh_float_lit_endian 1
    #else
        #define prh_float_big_endian 1
    #endif
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

#undef prh_export
#undef prh_import

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
        #define prh_export prh_impl_extern_keyword __declspec(dllexport)
    #elif defined(prh_import_dll_apis)
        #define prh_export prh_impl_extern_keyword __declspec(dllimport)
    #else
        #define prh_export prh_impl_extern_keyword
    #endif
#elif defined(prh_plat_windows) && defined(prh_gcc_version)
    #if defined(prh_export_dll_apis)
        #define prh_export prh_impl_extern_keyword __attribute__((dllexport))
    #elif defined(prh_import_dll_apis)
        #define prh_export prh_impl_extern_keyword __attribute__((dllimport))
    #else
        #define prh_export prh_impl_extern_keyword
    #endif
#elif defined(prh_gcc_version) || defined(prh_clang_version)
    #if defined(prh_export_dll_apis)
        #define prh_export prh_impl_extern_keyword __attribute__((visibility("default")))
    #else
        #define prh_export prh_impl_extern_keyword
    #endif
#else
    #define prh_export prh_impl_extern_keyword
#endif

#if defined(prh_msc_version)
    #define prh_import prh_impl_extern_keyword __declspec(dllimport)
#elif defined(prh_plat_windows) && defined(prh_gcc_version)
    #define prh_import prh_impl_extern_keyword __attribute__((dllimport))
#else
    #define prh_import prh_impl_extern_keyword
#endif

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
typedef struct prh_data prh_view;

typedef struct prh_data {
    prh_byte *data;
    union {
        prh_reg size;
        prh_reg bytes;
        prh_reg count;
        prh_reg length;
    };
} prh_data;

typedef union {
    prh_f32 value; // <sign> 1.<mantissa> * 10 ^ <exponent>
#if defined(prh_float_lit_endian) // exponent = exponent + 127
    prh_r32 mantissa: 23, exponent: 8, sign: 1;
#else
    prh_r32 sign: 1, exponent: 8, mantissa: 23;
#endif
} prh_union_f32;

typedef union {
    prh_f64 value; // <sign> 1.<mantissa> * 10 ^ <Exponent>
#if defined(prh_float_lit_endian) // Exponent = exponent + 1023
    prh_r64 mantissa: 52, exponent: 11, sign: 1;
#else
    prh_r64 sign: 1, exponent: 11, mantissa: 52;
#endif
} prh_union_f64;

#define prh_static_assert(const_expr) typedef int prh_impl_static_assert[(const_expr) ? 1 : -1]
#define prh_offsetof(type, field) ((prh_reg)(&((type *)0)->field))

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

#undef prh_return_address

#if defined(prh_msc_version)
// https://learn.microsoft.com/en-us/cpp/intrinsics/intrinsics-available-on-all-architectures
#include <intrin.h>
#define prh_return_address ((prh_raw)_ReturnAddress())
#elif defined(prh_gcc_version) || defined(prh_clang_version)
// https://gcc.gnu.org/onlinedocs/gcc/Return-Address.html
// 7.6 获取函数的返回地址或帧地址，这些函数可用于获取函数调用者的信息
//
// void * __builtin_return_address(unsigned int level)
//
// 此函数返回当前函数的返回地址，或其某个调用者的返回地址。level 参数是要沿调用栈向上扫描的帧数。值
// 为 0 时得到当前函数的返回地址，值为 1 时得到当前函数调用者的返回地址，以此类推。内联（inlining）
// 时的预期行为是：该函数返回"将要返回到的那个函数"的地址。要规避此行为，可使用 noinline 函数属性。
// level 参数必须是常量整数。
//
// 在某些机器上，可能无法确定除当前函数之外的任何函数的返回地址；在这种情况下，或当已到达栈顶时，此
// 函数返回一个未指定的值。此外，可使用 __builtin_frame_address 来确定是否已到达栈顶。返回的值可能需
// 要额外的后处理，参见 __builtin_extract_return_addr。
//
// 返回地址在内存中的存储表示可能与此函数返回的地址不同。例如，在 AArch64 上，存储的地址可能经过返回
// 地址签名（return address signing）处理，而 __builtin_return_address 返回的地址则没有。
//
// 以非零参数调用此函数可能产生不可预知的影响，包括使调用程序崩溃。因此，当启用 -Wframe-address 选项
// 时，被认为不安全的调用会被诊断。此类调用只应在调试场景中进行。
//
// 在代码地址可用 void * 表示的目标平台上，下面的代码可以得到当前函数将要返回到的代码地址。例如，这
// 样的地址可以与 dladdr 或其他使用代码地址的接口配合使用。
//      void *addr = __builtin_extract_return_addr(__builtin_return_address(0));
//
// void * __builtin_extract_return_addr(void *addr)
//
// 函数 __builtin_return_address 返回的地址可能需要经过它处理，才能得到实际编码的地址。例如，在 31
// 位 S/390 平台上必须掩掉最高位；在 SPARC 平台上必须加上一个偏移量，才能得到真正要执行的下一条指令。
// 如果无需修正，此函数只是原样透传 addr。
//
// void * __builtin_frob_return_addr(void *addr)
//
// 此函数执行与 __builtin_extract_return_addr 相反的操作。
//
// void * __builtin_frame_address(unsigned int level)
//
// 此函数与 __builtin_return_address 类似，但它返回的是函数帧的地址，而不是函数的返回地址。以值 0 调
// 用 __builtin_frame_address 得到当前函数的帧地址，值 1 得到当前函数调用者的帧地址，以此类推。帧是
// 栈上存放局部变量和已保存寄存器的区域。帧地址通常是函数压入栈的第一个字的地址。不过，精确定义取决
// 于处理器和调用约定。如果处理器有专用的帧指针寄存器，且函数有帧，则 __builtin_frame_address 返回帧
// 指针寄存器的值。
//
// 在某些机器上，可能无法确定除当前函数之外的任何函数的帧地址；在这种情况下，或当已到达栈顶时，如果
// 第一个帧指针已由启动代码正确初始化，此函数返回 0。
//
// 以非零参数调用此函数可能产生不可预知的影响，包括使调用程序崩溃。因此，当启用 -Wframe-address 选项
// 时，被认为不安全的调用会被诊断。此类调用只应在调试场景中进行。
//
// void * __builtin_stack_address()
//
// 此函数返回栈指针寄存器的值，如果定义了 STACK_ADDRESS_OFFSET，则加上该偏移量。从概念上讲，此内建函
// 数返回的地址，是其调用者可用的栈区与"可能被函数调用修改的区域"之间的边界；调用者可以（在调用序列
// 之前或之后，但不能在调用期间）安全地将该区域清零。
//
// 被调用者的参数可能作为调用者栈帧的一部分预先分配，也可能按每次调用分配，这取决于目标平台，因此它
// 们可能位于此边界的任一侧。
//
// 即使栈指针是有偏置（biased）的，返回结果也是没有偏置的。SPARC 上的寄存器保存区被视为可被调用修改，
// 而不是分配给调用者函数使用，因为在调用者函数自身运行期间它从未被使用。
//
// 只有叶函数（leaf function）才能使用的红区（red zone）也被视为可被调用修改，而不是分配给调用者使
// 用。这只是理论上的说法，因为叶函数不会发起调用；但使用常量偏移使此内建函数更具可预测性。
#define prh_return_address ((prh_raw)__builtin_extract_return_addr(__builtin_return_address(0)))
#endif

#if defined(prh_msc_version)
#include <intrin.h> // https://learn.microsoft.com/en-us/cpp/intrinsics/x64-amd64-intrinsics-list
#include <stdlib.h> // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/byteswap-uint64-byteswap-ulong-byteswap-ushort

prh_static_assert(sizeof(short) == sizeof(prh_r16));
prh_static_assert(sizeof(long) == sizeof(prh_r32));
prh_static_assert(sizeof(__int64) == sizeof(prh_r64));

prh_inline prh_r16 prh_r16_byte_swap(prh_r16 value)
{
    return _byteswap_ushort(value);
}

prh_inline prh_r32 prh_r32_byte_swap(prh_r32 value)
{
    return _byteswap_ulong(value);
}

prh_inline prh_r64 prh_r64_byte_swap(prh_r64 value)
{
    return _byteswap_uint64(value);
}

prh_inline prh_r32 prh_r32_unchecked_lower_most_bit_position(prh_r32 n)
{
    unsigned long zeros;
    _BitScanForward(&zeros, n);
    return (prh_r32)zeros;
}

prh_inline prh_r32 prh_r32_unchecked_higher_most_bit_position(prh_r32 n)
{
    unsigned long zeros;
    _BitScanReverse(&zeros, n);
    return (prh_r32)zeros;
}

prh_inline prh_r32 prh_r32_lower_most_bit_position(prh_r32 n)
{
    return n ? prh_r32_unchecked_lower_most_bit_position(n) : 0;
}

prh_inline prh_r32 prh_r32_higher_most_bit_position(prh_r32 n)
{
    return n ? prh_r32_unchecked_higher_most_bit_position(n) : 0;
}

prh_inline prh_r32 prh_r32_trailing_zeros(prh_r32 n)
{
    unsigned long zeros; // 从最低位开始找1比特位，返回第一个1比特位前面的0的个数
    prh_byte has_set_bit = _BitScanForward(&zeros, n);
    return has_set_bit ? zeros : 32;
}

prh_inline prh_r32 prh_r32_leading_zeros(prh_r32 n)
{
    unsigned long zeros; // 从最高位开始找1比特位，返回第一个1比特位前面的0的个数
    prh_byte has_set_bit = _BitScanReverse(&zeros, n);
    return has_set_bit ? 31 - zeros : 32;
}

#if defined(prh_arch_x64) || defined(prh_arch_a64)

prh_inline prh_r32 prh_r64_unchecked_lower_most_bit_position(prh_r64 n)
{
    unsigned long zeros;
    _BitScanForward64(&zeros, n);
    return (prh_r32)zeros;
}

prh_inline prh_r32 prh_r64_unchecked_higher_most_bit_position(prh_r64 n)
{
    unsigned long zeros;
    _BitScanReverse64(&zeros, n);
    return (prh_r32)zeros;
}

prh_inline prh_r32 prh_r64_lower_most_bit_position(prh_r64 n)
{
    return n ? prh_r64_unchecked_lower_most_bit_position(n) : 0;
}

prh_inline prh_r32 prh_r64_higher_most_bit_position(prh_r64 n)
{
    return n ? prh_r64_unchecked_higher_most_bit_position(n) : 0;
}

prh_inline prh_r32 prh_r64_trailing_zeros(prh_r64 n)
{
    unsigned long zeros; // 从最低位开始找1比特位，返回第一个1比特位前面的0的个数
    prh_byte has_set_bit = _BitScanForward64(&zeros, n);
    return has_set_bit ? zeros : 64;
}

prh_inline prh_r32 prh_r64_leading_zeros(prh_r64 n)
{
    unsigned long zeros; // 从最高位开始找1比特位，返回第一个1比特位前面的0的个数
    prh_byte has_set_bit = _BitScanReverse64(&zeros, n);
    return has_set_bit ? 63 - zeros : 64;
}

#endif // prh_arch_x64 prh_arch_a64

#if defined(prh_arch_x86) || defined(prh_arch_x64)

prh_inline prh_r32 prh_r32_set_bit_count(prh_r32 n)
{
    return _mm_popcnt_u32(n);
}

#if defined(prh_arch_x64)
prh_inline prh_r32 prh_r64_set_bit_count(prh_r64 n)
{
    return (prh_r32)_mm_popcnt_u64(n);
}
#endif

#elif defined(prh_arch_arm) || defined(prh_arch_a64)

prh_inline prh_r32 prh_r32_set_bit_count(prh_r32 n)
{
    return _CountOneBits(n);
}

#if defined(prh_arch_a64)
prh_inline prh_r32 prh_r64_set_bit_count(prh_r64 n)
{
    return _CountOneBits64(n);
}
#endif

#endif // prh_arch_x86 prh_arch_x64
#endif // prh_msc_version

#if defined(prh_gcc_version) || defined(prh_clang_version)
// https://gcc.gnu.org/onlinedocs/gcc/Bit-Operation-Builtins.html

prh_inline prh_r16 prh_r16_byte_swap(prh_r16 value)
{
    return __builtin_bswap16(value);
}

prh_inline prh_r32 prh_r32_byte_swap(prh_r32 value)
{
    return __builtin_bswap32(value);
}

prh_inline prh_r64 prh_r64_byte_swap(prh_r64 value)
{
    return __builtin_bswap64(value);
}

prh_inline prh_r32 prh_r32_trailing_zeros(prh_r32 n)
{
    return n ? __builtin_ctz(n) : 32;
}

prh_inline prh_r32 prh_r32_leading_zeros(prh_r32 n)
{
    return n ? __builtin_clz(n) : 32;
}

prh_inline prh_r32 prh_r64_trailing_zeros(prh_r64 n)
{
    return n ? __builtin_ctzll(n) : 64;
}

prh_inline prh_r32 prh_r64_leading_zeros(prh_r64 n)
{
    return n ? __builtin_clzll(n) : 64;
}

prh_inline prh_r32 prh_r32_unchecked_trailing_zeros(prh_r32 n)
{
    return __builtin_ctz(n); /* if n is 0, the result is undefined */
}

prh_inline prh_r32 prh_r32_unchecked_leading_zeros(prh_r32 n)
{
    return __builtin_clz(n);
}

prh_inline prh_r32 prh_r64_unchecked_trailing_zeros(prh_r64 n)
{
    return __builtin_ctzll(n);
}

prh_inline prh_r32 prh_r64_unchecked_leading_zeros(prh_r64 n)
{
    return __builtin_clzll(n);
}

prh_inline prh_r32 prh_r32_unchecked_lower_most_bit_position(prh_r32 n)
{
    return prh_r32_unchecked_trailing_zeros(n);
}

prh_inline prh_r32 prh_r32_unchecked_higher_most_bit_position(prh_r32 n)
{
    return 31 - prh_r32_unchecked_leading_zeros(n);
}

prh_inline prh_r32 prh_r64_unchecked_lower_most_bit_position(prh_r64 n)
{
    return prh_r64_unchecked_trailing_zeros(n);
}

prh_inline prh_r32 prh_r64_unchecked_higher_most_bit_position(prh_r64 n)
{
    return 63 - prh_r64_unchecked_leading_zeros(n);
}

prh_inline prh_r32 prh_r32_lower_most_bit_position(prh_r32 n)
{
    return (n) ? prh_r32_unchecked_trailing_zeros(n) : 0;
}

prh_inline prh_r32 prh_r32_higher_most_bit_position(prh_r32 n)
{
    return (n) ? 31 - prh_r32_unchecked_leading_zeros(n) : 0;
}

prh_inline prh_r32 prh_r64_lower_most_bit_position(prh_r64 n)
{
    return (n) ? prh_r64_unchecked_trailing_zeros(n) : 0;
}

prh_inline prh_r32 prh_r64_higher_most_bit_position(prh_r64 n)
{
    return (n) ? 63 - prh_r64_unchecked_leading_zeros(n) : 0;
}

prh_inline prh_r32 prh_r32_set_bit_count(prh_r32 n)
{
    return __builtin_popcount(n);
}

prh_inline prh_r32 prh_r64_set_bit_count(prh_r64 n)
{
    return __builtin_popcountll(n);
}
#endif // prh_gcc_version

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

#undef PRH_DEBUG

#if defined(_DEBUG)
    #undef NDEBUG // 启用标准断言
    #define PRH_DEBUG 1
#else
    #define NDEBUG 1 // 关闭标准断言
    #define PRH_DEBUG 0
#endif

#undef prh_line_file_debug
#undef prh_line_caller_debug

#if defined(prh_using_line_caller_debug)
    #define prh_line_caller_debug 1
#elif defined(prh_using_line_file_debug)
    #define prh_line_file_debug 1
#else
    #define prh_line_file_debug 1
#endif

#undef prh_caller

#if defined(prh_line_caller_debug)
    #define prh_caller prh_return_address
#else
    #define prh_caller ((prh_raw)__FILE__)
#endif

// 当 ## 记号粘贴（token paste）操作符位于逗号（,）和可变参数（__VA_ARGS__）之间时，具有特殊含义。如
// 果在使用宏时省略了可变参数，那么 ## 之前的逗号会被删除。如果传入的是空参数，则不会发生这种删除；如
// 果 ## 之前的记号不是逗号，也不会发生。ISO C99 要求可变参数宏中的 "..." 至少有一个参数。
//
// The ‘##’ token paste operator has a special meaning when placed between a comma (,) and a variable
// argument (__VA_ARGS__). If the variable argument is left out when the macro is used, then the comma
// before the ‘##’ will be deleted. This does not happen if you pass an empty argument, nor does it
// happen if the token preceding ‘##’ is anything other than a comma. ISO C99 requires at least one
// argument for the "..." in a variadic macro.

#define prh_macro_make_name(a, b) prh_impl_macro_make_name(a, b)
#define prh_macro_make_cstr(a) prh_impl_macro_make_cstr(a)
#define prh_impl_macro_make_name(a, b) a ## b
#define prh_impl_macro_make_cstr(a) #a

// 0 1 2 3  4  5  6   7   8   9  10  11  12  13   14   15   16    17    18
// 1 2 4 8 16 32 64 128 256 512 1KB 2KB 4KB 8KB 16KB 32KB 64KB 128KB 256KB
#define prh_impl_align_4_byte 2
#define prh_impl_align_8_byte 3
#define prh_impl_align_16_byte 4
#define prh_impl_align_32_byte 5
#define prh_impl_align_64_byte 6
#define prh_impl_align_128_byte 7
#define prh_impl_align_256_byte 8
#define prh_impl_align_512_byte 9
#define prh_impl_align_1k_byte 10
#define prh_impl_align_2k_byte 11
#define prh_impl_align_4k_byte 12
#define prh_impl_align_8k_byte 13
#define prh_impl_align_16k_byte 14
#define prh_impl_align_32k_byte 15
#define prh_impl_align_64k_byte 16
#define prh_impl_align_128k_byte 17
#define prh_impl_align_256k_byte 18
#define prh_impl_align_512k_byte 19
#define prh_impl_align_1m_byte 20
#define prh_impl_align_2m_byte 21
#define prh_impl_align_4m_byte 22
#define prh_impl_align_8m_byte 23
#define prh_impl_align_16m_byte 24
#define prh_impl_align_32m_byte 25
#define prh_impl_align_64m_byte 26
#define prh_impl_align_128m_byte 27
#define prh_impl_align_256m_byte 28
#define prh_impl_align_512m_byte 29
#define prh_impl_align_1g_byte 30

#ifndef prh_cache_line_size
#define prh_cache_line_size 64
#define prh_impl_align_line prh_impl_align_64_byte
#endif

#ifndef prh_memory_page_size
#define prh_memory_page_size 4096
#define prh_impl_align_page prh_impl_align_4k_byte
#endif

#ifndef prh_vmem_unit_size
#define prh_vmem_unit_size (64*1024)
#define prh_impl_align_vmem prh_impl_align_64k_byte
#endif

#if defined(prh_plat_windows)
prh_static_assert(prh_vmem_unit_size == 64*1024);
#endif

prh_static_assert(prh_cache_line_size == (1 << prh_impl_align_line));
prh_static_assert(prh_memory_page_size == (1 << prh_impl_align_page));
prh_static_assert(prh_vmem_unit_size == (1 << prh_impl_align_vmem));

#define prh_align_line_size prh_cache_line_size
#define prh_align_page_size prh_memory_page_size
#define prh_align_vmem_unit prh_vmem_unit_size
#define prh_align_int_size prh_int_size
#define prh_align_imt_size prh_imt_size
#define prh_align_ptr_size prh_imt_size
#define prh_align_4_byte 4
#define prh_align_8_byte 8
#define prh_align_16_byte 16
#define prh_align_32_byte 32
#define prh_align_64_byte 64
#define prh_align_128_byte 128
#define prh_align_256_byte 256
#define prh_align_512_byte 512
#define prh_align_1k_byte 1024
#define prh_align_2k_byte (prh_align_1k_byte * 2)
#define prh_align_4k_byte (prh_align_1k_byte * 4)
#define prh_align_8k_byte (prh_align_1k_byte * 8)
#define prh_align_16k_byte (prh_align_1k_byte * 16)
#define prh_align_32k_byte (prh_align_1k_byte * 32)
#define prh_align_64k_byte (prh_align_1k_byte * 64)
#define prh_align_128k_byte (prh_align_1k_byte * 128)
#define prh_align_256k_byte (prh_align_1k_byte * 256)
#define prh_align_512k_byte (prh_align_1k_byte * 512)
#define prh_align_1m_byte 1048576
#define prh_align_2m_byte (prh_align_1m_byte * 2)
#define prh_align_4m_byte (prh_align_1m_byte * 4)
#define prh_align_8m_byte (prh_align_1m_byte * 8)
#define prh_align_16m_byte (prh_align_1m_byte * 16)
#define prh_align_32m_byte (prh_align_1m_byte * 32)
#define prh_align_64m_byte (prh_align_1m_byte * 64)
#define prh_align_128m_byte (prh_align_1m_byte * 128)
#define prh_align_256m_byte (prh_align_1m_byte * 256)
#define prh_align_512m_byte (prh_align_1m_byte * 512)
#define prh_align_1g_byte 1073741824

prh_static_assert(prh_align_4_byte == (1 << prh_impl_align_4_byte));
prh_static_assert(prh_align_8_byte == (1 << prh_impl_align_8_byte));
prh_static_assert(prh_align_16_byte == (1 << prh_impl_align_16_byte));
prh_static_assert(prh_align_32_byte == (1 << prh_impl_align_32_byte));
prh_static_assert(prh_align_64_byte == (1 << prh_impl_align_64_byte));
prh_static_assert(prh_align_128_byte == (1 << prh_impl_align_128_byte));
prh_static_assert(prh_align_256_byte == (1 << prh_impl_align_256_byte));
prh_static_assert(prh_align_512_byte == (1 << prh_impl_align_512_byte));
prh_static_assert(prh_align_1k_byte == (1 << prh_impl_align_1k_byte));
prh_static_assert(prh_align_2k_byte == (1 << prh_impl_align_2k_byte));
prh_static_assert(prh_align_4k_byte == (1 << prh_impl_align_4k_byte));
prh_static_assert(prh_align_8k_byte == (1 << prh_impl_align_8k_byte));
prh_static_assert(prh_align_16k_byte == (1 << prh_impl_align_16k_byte));
prh_static_assert(prh_align_32k_byte == (1 << prh_impl_align_32k_byte));
prh_static_assert(prh_align_64k_byte == (1 << prh_impl_align_64k_byte));
prh_static_assert(prh_align_128k_byte == (1 << prh_impl_align_128k_byte));
prh_static_assert(prh_align_256k_byte == (1 << prh_impl_align_256k_byte));
prh_static_assert(prh_align_512k_byte == (1 << prh_impl_align_512k_byte));
prh_static_assert(prh_align_1m_byte == (1 << prh_impl_align_1m_byte));
prh_static_assert(prh_align_2m_byte == (1 << prh_impl_align_2m_byte));
prh_static_assert(prh_align_4m_byte == (1 << prh_impl_align_4m_byte));
prh_static_assert(prh_align_8m_byte == (1 << prh_impl_align_8m_byte));
prh_static_assert(prh_align_16m_byte == (1 << prh_impl_align_16m_byte));
prh_static_assert(prh_align_32m_byte == (1 << prh_impl_align_32m_byte));
prh_static_assert(prh_align_64m_byte == (1 << prh_impl_align_64m_byte));
prh_static_assert(prh_align_128m_byte == (1 << prh_impl_align_128m_byte));
prh_static_assert(prh_align_256m_byte == (1 << prh_impl_align_256m_byte));
prh_static_assert(prh_align_512m_byte == (1 << prh_impl_align_512m_byte));
prh_static_assert(prh_align_1g_byte == (1 << prh_impl_align_1g_byte));

prh_inline prh_reg prh_set_value_if_true(prh_reg value, prh_reg orelse, bool b)
{
    return (b == true) * value + (b == false) * orelse;
}

prh_inline prh_r32 prh_set_value_32_if_true(prh_r32 value, prh_r32 orelse, bool b)
{
    return (b == true) * value + (b == false) * orelse;
}

prh_inline prh_reg prh_set_value_if_zero(prh_reg value, prh_reg a)
{
    return prh_set_value_if_true(value, a, a == 0);
}

prh_inline prh_r32 prh_set_value_32_if_zero(prh_r32 value, prh_r32 a)
{
    return prh_set_value_32_if_true(value, a, a == 0);
}

prh_inline prh_r32 prh_r32_clear_set(prh_r32 value, prh_r32 mask, prh_r32 set)
{
    return (value & (~mask)) | set;
}

prh_inline prh_r32 prh_r32_clear_bits(prh_r32 value, prh_r32 mask)
{
    return value & (~mask);
}

prh_inline prh_r64 prh_r64_clear_set(prh_r64 value, prh_r64 mask, prh_r64 set)
{
    return (value & (~mask)) | set;
}

prh_inline prh_r64 prh_r64_clear_bits(prh_r64 value, prh_r64 mask)
{
    return value & (~mask);
}

#if prh_int_bits == 64
prh_inline prh_reg prh_reg_clear_set(prh_reg value, prh_reg mask, prh_reg set) { return prh_r64_clear_set(value, mask, set); }
prh_inline prh_reg prh_reg_clear_bits(prh_reg value, prh_reg mask) { return prh_r64_clear_bits(value, mask); }
#else
prh_inline prh_reg prh_reg_clear_set(prh_reg value, prh_reg mask, prh_reg set) { return prh_r32_clear_set(value, mask, set); }
prh_inline prh_reg prh_reg_clear_bits(prh_reg value, prh_reg mask) { return prh_r32_clear_bits(value, mask); }
#endif

#if prh_imt_bits == 64
prh_inline prh_raw prh_raw_clear_set(prh_raw value, prh_raw mask, prh_raw set) { return prh_r64_clear_set(value, mask, set); }
prh_inline prh_raw prh_raw_clear_bits(prh_raw value, prh_raw mask) { return prh_r64_clear_bits(value, mask); }
#else
prh_inline prh_raw prh_raw_clear_set(prh_raw value, prh_raw mask, prh_raw set) { return prh_r32_clear_set(value, mask, set); }
prh_inline prh_raw prh_raw_clear_bits(prh_raw value, prh_raw mask) { return prh_r32_clear_bits(value, mask); }
#endif

#if prh_int_bits == 64
prh_inline prh_r32 prh_reg_higher_most_bit_position(prh_reg n) { return prh_r64_higher_most_bit_position(n); }
prh_inline prh_r32 prh_reg_lower_most_bit_position(prh_reg n) { return prh_r64_lower_most_bit_position(n); }
prh_inline prh_r32 prh_reg_unchecked_higher_most_bit_position(prh_reg n) { return prh_r64_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_reg_unchecked_lower_most_bit_position(prh_reg n) { return prh_r64_unchecked_lower_most_bit_position(n); }
#else
prh_inline prh_r32 prh_reg_higher_most_bit_position(prh_reg n) { return prh_r32_higher_most_bit_position(n); }
prh_inline prh_r32 prh_reg_lower_most_bit_position(prh_reg n) { return prh_r32_lower_most_bit_position(n); }
prh_inline prh_r32 prh_reg_unchecked_higher_most_bit_position(prh_reg n) { return prh_r32_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_reg_unchecked_lower_most_bit_position(prh_reg n) { return prh_r32_unchecked_lower_most_bit_position(n); }
#endif

#if prh_imt_bits == 64
prh_inline prh_r32 prh_raw_higher_most_bit_position(prh_raw n) { return prh_r64_higher_most_bit_position(n); }
prh_inline prh_r32 prh_raw_lower_most_bit_position(prh_raw n) { return prh_r64_lower_most_bit_position(n); }
prh_inline prh_r32 prh_raw_unchecked_higher_most_bit_position(prh_raw n) { return prh_r64_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_raw_unchecked_lower_most_bit_position(prh_raw n) { return prh_r64_unchecked_lower_most_bit_position(n); }
#else
prh_inline prh_r32 prh_raw_higher_most_bit_position(prh_raw n) { return prh_r32_higher_most_bit_position(n); }
prh_inline prh_r32 prh_raw_lower_most_bit_position(prh_raw n) { return prh_r32_lower_most_bit_position(n); }
prh_inline prh_r32 prh_raw_unchecked_higher_most_bit_position(prh_raw n) { return prh_r32_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_raw_unchecked_lower_most_bit_position(prh_raw n) { return prh_r32_unchecked_lower_most_bit_position(n); }
#endif

#define prh_byte_1(n) ((prh_byte)((n)&0xFF))
#define prh_byte_2(n) ((prh_byte)(((n)>>8)&0xFF))
#define prh_byte_3(n) ((prh_byte)(((n)>>16)&0xFF))
#define prh_byte_4(n) ((prh_byte)(((n)>>24)&0xFF))
#define prh_byte_5(n) ((prh_byte)(((n)>>32)&0xFF))
#define prh_byte_6(n) ((prh_byte)(((n)>>40)&0xFF))
#define prh_byte_7(n) ((prh_byte)(((n)>>48)&0xFF))
#define prh_byte_8(n) ((prh_byte)(((n)>>56)&0xFF))

#define prh_le_2_bytes(n) prh_byte_1(n), prh_byte_2(n)
#define prh_le_3_bytes(n) prh_byte_1(n), prh_byte_2(n), prh_byte_3(n)
#define prh_le_4_bytes(n) prh_byte_1(n), prh_byte_2(n), prh_byte_3(n), prh_byte_4(n)
#define prh_le_5_bytes(n) prh_byte_1(n), prh_byte_2(n), prh_byte_3(n), prh_byte_4(n), prh_byte_5(n)
#define prh_le_6_bytes(n) prh_byte_1(n), prh_byte_2(n), prh_byte_3(n), prh_byte_4(n), prh_byte_5(n), prh_byte_6(n)
#define prh_le_7_bytes(n) prh_byte_1(n), prh_byte_2(n), prh_byte_3(n), prh_byte_4(n), prh_byte_5(n), prh_byte_6(n), prh_byte_7(n)
#define prh_le_8_bytes(n) prh_byte_1(n), prh_byte_2(n), prh_byte_3(n), prh_byte_4(n), prh_byte_5(n), prh_byte_6(n), prh_byte_7(n), prh_byte_8(n)

#define prh_be_2_bytes(n) prh_byte_2(n), prh_byte_1(n)
#define prh_be_3_bytes(n) prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)
#define prh_be_4_bytes(n) prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)
#define prh_be_5_bytes(n) prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)
#define prh_be_6_bytes(n) prh_byte_6(n), prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)
#define prh_be_7_bytes(n) prh_byte_7(n), prh_byte_6(n), prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)
#define prh_be_8_bytes(n) prh_byte_8(n), prh_byte_7(n), prh_byte_6(n), prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)

prh_inline prh_r16 prh_impl_r16_from(prh_r16 a, prh_r16 b) { return (b << 8) | a; }
prh_inline prh_r32 prh_impl_r24_from(prh_r32 a, prh_r32 b, prh_r32 c) { return (c << 16) | (b << 8) | a; }
prh_inline prh_r32 prh_impl_r32_from(prh_r32 a, prh_r32 b, prh_r32 c, prh_r32 d) { return (d << 24) | (c << 16) | (b << 8) | a; }
prh_inline prh_r64 prh_impl_r40_from(prh_r64 a, prh_r64 b, prh_r64 c, prh_r64 d, prh_r64 e) { return (e << 32) | (d << 24) | (c << 16) | (b << 8) | a; }
prh_inline prh_r64 prh_impl_r48_from(prh_r64 a, prh_r64 b, prh_r64 c, prh_r64 d, prh_r64 e, prh_r64 f) { return (f << 40) | (e << 32) | (d << 24) | (c << 16) | (b << 8) | a; }
prh_inline prh_r64 prh_impl_r56_from(prh_r64 a, prh_r64 b, prh_r64 c, prh_r64 d, prh_r64 e, prh_r64 f, prh_r64 g) { return (g << 48) | (f << 40) | (e << 32) | (d << 24) | (c << 16) | (b << 8) | a; }
prh_inline prh_r64 prh_impl_r64_from(prh_r64 a, prh_r64 b, prh_r64 c, prh_r64 d, prh_r64 e, prh_r64 f, prh_r64 g, prh_r64 h) { return (h << 56) | (g << 48) | (f << 40) | (e << 32) | (d << 24) | (c << 16) | (b << 8) | a; }

prh_inline prh_r16 prh_le_2b_to_host(prh_byte a, prh_byte b) { return prh_impl_r16_from(a, b); }
prh_inline prh_r32 prh_le_3b_to_host(prh_byte a, prh_byte b, prh_byte c) { return prh_impl_r24_from(a, b, c); }
prh_inline prh_r32 prh_le_4b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d) { return prh_impl_r32_from(a, b, c, d); }
prh_inline prh_r64 prh_le_5b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e) { return prh_impl_r40_from(a, b, c, d, e); }
prh_inline prh_r64 prh_le_6b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e, prh_byte f) { return prh_impl_r48_from(a, b, c, d, e, f); }
prh_inline prh_r64 prh_le_7b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e, prh_byte f, prh_byte g) { return prh_impl_r56_from(a, b, c, d, e, f, g); }
prh_inline prh_r64 prh_le_8b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e, prh_byte f, prh_byte g, prh_byte h) { return prh_impl_r64_from(a, b, c, d, e, f, g, h); }

prh_inline prh_r16 prh_be_2b_to_host(prh_byte a, prh_byte b) { return prh_impl_r16_from(b, a); }
prh_inline prh_r32 prh_be_3b_to_host(prh_byte a, prh_byte b, prh_byte c) { return prh_impl_r24_from(c, b, a); }
prh_inline prh_r32 prh_be_4b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d) { return prh_impl_r32_from(d, c, b, a); }
prh_inline prh_r64 prh_be_5b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e) { return prh_impl_r40_from(e, d, c, b, a); }
prh_inline prh_r64 prh_be_6b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e, prh_byte f) { return prh_impl_r48_from(f, e, d, c, b, a); }
prh_inline prh_r64 prh_be_7b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e, prh_byte f, prh_byte g) { return prh_impl_r56_from(g, f, e, d, c, b, a); }
prh_inline prh_r64 prh_be_8b_to_host(prh_byte a, prh_byte b, prh_byte c, prh_byte d, prh_byte e, prh_byte f, prh_byte g, prh_byte h) { return prh_impl_r64_from(h, g, f, e, d, c, b, a); }

prh_inline prh_r16 prh_lp_2b_to_host(const prh_byte *p) { return prh_le_2b_to_host(p[0], p[1]); }
prh_inline prh_r32 prh_lp_3b_to_host(const prh_byte *p) { return prh_le_3b_to_host(p[0], p[1], p[2]); }
prh_inline prh_r32 prh_lp_4b_to_host(const prh_byte *p) { return prh_le_4b_to_host(p[0], p[1], p[2], p[3]); }
prh_inline prh_r64 prh_lp_5b_to_host(const prh_byte *p) { return prh_le_5b_to_host(p[0], p[1], p[2], p[3], p[4]); }
prh_inline prh_r64 prh_lp_6b_to_host(const prh_byte *p) { return prh_le_6b_to_host(p[0], p[1], p[2], p[3], p[4], p[5]); }
prh_inline prh_r64 prh_lp_7b_to_host(const prh_byte *p) { return prh_le_7b_to_host(p[0], p[1], p[2], p[3], p[4], p[5], p[6]); }
prh_inline prh_r64 prh_lp_8b_to_host(const prh_byte *p) { return prh_le_8b_to_host(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]); }

prh_inline prh_r16 prh_bp_2b_to_host(const prh_byte *p) { return prh_be_2b_to_host(p[0], p[1]); }
prh_inline prh_r32 prh_bp_3b_to_host(const prh_byte *p) { return prh_be_3b_to_host(p[0], p[1], p[2]); }
prh_inline prh_r32 prh_bp_4b_to_host(const prh_byte *p) { return prh_be_4b_to_host(p[0], p[1], p[2], p[3]); }
prh_inline prh_r64 prh_bp_5b_to_host(const prh_byte *p) { return prh_be_5b_to_host(p[0], p[1], p[2], p[3], p[4]); }
prh_inline prh_r64 prh_bp_6b_to_host(const prh_byte *p) { return prh_be_6b_to_host(p[0], p[1], p[2], p[3], p[4], p[5]); }
prh_inline prh_r64 prh_bp_7b_to_host(const prh_byte *p) { return prh_be_7b_to_host(p[0], p[1], p[2], p[3], p[4], p[5], p[6]); }
prh_inline prh_r64 prh_bp_8b_to_host(const prh_byte *p) { return prh_be_8b_to_host(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]); }

prh_inline void prh_host_to_lp_2b(prh_r16 n, prh_byte *p) { p[0] = prh_byte_1(n); p[1] = prh_byte_2(n); }
prh_inline void prh_host_to_lp_3b(prh_r32 n, prh_byte *p) { p[0] = prh_byte_1(n); p[1] = prh_byte_2(n); p[2] = prh_byte_3(n); }
prh_inline void prh_host_to_lp_4b(prh_r32 n, prh_byte *p) { p[0] = prh_byte_1(n); p[1] = prh_byte_2(n); p[2] = prh_byte_3(n); p[3] = prh_byte_4(n); }
prh_inline void prh_host_to_lp_5b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_1(n); p[1] = prh_byte_2(n); p[2] = prh_byte_3(n); p[3] = prh_byte_4(n); p[4] = prh_byte_5(n); }
prh_inline void prh_host_to_lp_6b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_1(n); p[1] = prh_byte_2(n); p[2] = prh_byte_3(n); p[3] = prh_byte_4(n); p[4] = prh_byte_5(n); p[5] = prh_byte_6(n); }
prh_inline void prh_host_to_lp_7b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_1(n); p[1] = prh_byte_2(n); p[2] = prh_byte_3(n); p[3] = prh_byte_4(n); p[4] = prh_byte_5(n); p[5] = prh_byte_6(n); p[6] = prh_byte_7(n); }
prh_inline void prh_host_to_lp_8b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_1(n); p[1] = prh_byte_2(n); p[2] = prh_byte_3(n); p[3] = prh_byte_4(n); p[4] = prh_byte_5(n); p[5] = prh_byte_6(n); p[6] = prh_byte_7(n); p[7] = prh_byte_8(n); }

prh_inline void prh_host_to_bp_2b(prh_r16 n, prh_byte *p) { p[0] = prh_byte_2(n); p[1] = prh_byte_1(n); }
prh_inline void prh_host_to_bp_3b(prh_r32 n, prh_byte *p) { p[0] = prh_byte_3(n); p[1] = prh_byte_2(n); p[2] = prh_byte_1(n); }
prh_inline void prh_host_to_bp_4b(prh_r32 n, prh_byte *p) { p[0] = prh_byte_4(n); p[1] = prh_byte_3(n); p[2] = prh_byte_2(n); p[3] = prh_byte_1(n); }
prh_inline void prh_host_to_bp_5b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_5(n); p[1] = prh_byte_4(n); p[2] = prh_byte_3(n); p[3] = prh_byte_2(n); p[4] = prh_byte_1(n); }
prh_inline void prh_host_to_bp_6b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_6(n); p[1] = prh_byte_5(n); p[2] = prh_byte_4(n); p[3] = prh_byte_3(n); p[4] = prh_byte_2(n); p[5] = prh_byte_1(n); }
prh_inline void prh_host_to_bp_7b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_7(n); p[1] = prh_byte_6(n); p[2] = prh_byte_5(n); p[3] = prh_byte_4(n); p[4] = prh_byte_3(n); p[5] = prh_byte_2(n); p[6] = prh_byte_1(n); }
prh_inline void prh_host_to_bp_8b(prh_r64 n, prh_byte *p) { p[0] = prh_byte_8(n); p[1] = prh_byte_7(n); p[2] = prh_byte_6(n); p[3] = prh_byte_5(n); p[4] = prh_byte_4(n); p[5] = prh_byte_3(n); p[6] = prh_byte_2(n); p[7] = prh_byte_1(n); }

#if 1
prh_inline prh_r16 prh_impl_r16_invert_order(prh_r16 n) { return prh_r16_byte_swap(n); }
prh_inline prh_r32 prh_impl_r24_invert_order(prh_r32 n) { return prh_r32_byte_swap(n); }
prh_inline prh_r32 prh_impl_r32_invert_order(prh_r32 n) { return prh_r32_byte_swap(n); }
prh_inline prh_r64 prh_impl_r40_invert_order(prh_r64 n) { return prh_r64_byte_swap(n); }
prh_inline prh_r64 prh_impl_r48_invert_order(prh_r64 n) { return prh_r64_byte_swap(n); }
prh_inline prh_r64 prh_impl_r56_invert_order(prh_r64 n) { return prh_r64_byte_swap(n); }
prh_inline prh_r64 prh_impl_r64_invert_order(prh_r64 n) { return prh_r64_byte_swap(n); }
#else
prh_inline prh_r16 prh_impl_r16_invert_order(prh_r16 n) { return prh_impl_r16_from(prh_byte_2(n), prh_byte_1(n)); }
prh_inline prh_r32 prh_impl_r24_invert_order(prh_r32 n) { return prh_impl_r24_from(prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)); }
prh_inline prh_r32 prh_impl_r32_invert_order(prh_r32 n) { return prh_impl_r32_from(prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)); }
prh_inline prh_r64 prh_impl_r40_invert_order(prh_r64 n) { return prh_impl_r40_from(prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)); }
prh_inline prh_r64 prh_impl_r48_invert_order(prh_r64 n) { return prh_impl_r48_from(prh_byte_6(n), prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)); }
prh_inline prh_r64 prh_impl_r56_invert_order(prh_r64 n) { return prh_impl_r56_from(prh_byte_7(n), prh_byte_6(n), prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)); }
prh_inline prh_r64 prh_impl_r64_invert_order(prh_r64 n) { return prh_impl_r64_from(prh_byte_8(n), prh_byte_7(n), prh_byte_6(n), prh_byte_5(n), prh_byte_4(n), prh_byte_3(n), prh_byte_2(n), prh_byte_1(n)); }
#endif

#if prh_int_bits == 64
prh_inline prh_reg prh_impl_reg_invert_order(prh_reg n) { return prh_impl_r64_invert_order(n); }
#else
prh_inline prh_reg prh_impl_reg_invert_order(prh_reg n) { return prh_impl_r32_invert_order(n); }
#endif

#if prh_imt_bits == 64
prh_inline prh_raw prh_impl_raw_invert_order(prh_raw n) { return prh_impl_r64_invert_order(n); }
#else
prh_inline prh_raw prh_impl_raw_invert_order(prh_raw n) { return prh_impl_r32_invert_order(n); }
#endif

#define prh_set_r16_host_to_le(a) (a) = prh_r16_host_to_le(a)
#define prh_set_r24_host_to_le(a) (a) = prh_r24_host_to_le(a)
#define prh_set_r32_host_to_le(a) (a) = prh_r32_host_to_le(a)
#define prh_set_r40_host_to_le(a) (a) = prh_r40_host_to_le(a)
#define prh_set_r48_host_to_le(a) (a) = prh_r48_host_to_le(a)
#define prh_set_r56_host_to_le(a) (a) = prh_r56_host_to_le(a)
#define prh_set_r64_host_to_le(a) (a) = prh_r64_host_to_le(a)
#define prh_set_reg_host_to_le(a) (a) = prh_reg_host_to_le(a)
#define prh_set_raw_host_to_le(a) (a) = prh_raw_host_to_le(a)
#define prh_set_r16_le_to_host(a) (a) = prh_r16_le_to_host(a)
#define prh_set_r24_le_to_host(a) (a) = prh_r24_le_to_host(a)
#define prh_set_r32_le_to_host(a) (a) = prh_r32_le_to_host(a)
#define prh_set_r40_le_to_host(a) (a) = prh_r40_le_to_host(a)
#define prh_set_r48_le_to_host(a) (a) = prh_r48_le_to_host(a)
#define prh_set_r56_le_to_host(a) (a) = prh_r56_le_to_host(a)
#define prh_set_r64_le_to_host(a) (a) = prh_r64_le_to_host(a)
#define prh_set_reg_le_to_host(a) (a) = prh_reg_le_to_host(a)
#define prh_set_raw_le_to_host(a) (a) = prh_raw_le_to_host(a)
#define prh_set_r16_host_to_be(a) (a) = prh_r16_host_to_be(a)
#define prh_set_r24_host_to_be(a) (a) = prh_r24_host_to_be(a)
#define prh_set_r32_host_to_be(a) (a) = prh_r32_host_to_be(a)
#define prh_set_r40_host_to_be(a) (a) = prh_r40_host_to_be(a)
#define prh_set_r48_host_to_be(a) (a) = prh_r48_host_to_be(a)
#define prh_set_r56_host_to_be(a) (a) = prh_r56_host_to_be(a)
#define prh_set_r64_host_to_be(a) (a) = prh_r64_host_to_be(a)
#define prh_set_reg_host_to_be(a) (a) = prh_reg_host_to_be(a)
#define prh_set_raw_host_to_be(a) (a) = prh_raw_host_to_be(a)
#define prh_set_r16_be_to_host(a) (a) = prh_r16_be_to_host(a)
#define prh_set_r24_be_to_host(a) (a) = prh_r24_be_to_host(a)
#define prh_set_r32_be_to_host(a) (a) = prh_r32_be_to_host(a)
#define prh_set_r40_be_to_host(a) (a) = prh_r40_be_to_host(a)
#define prh_set_r48_be_to_host(a) (a) = prh_r48_be_to_host(a)
#define prh_set_r56_be_to_host(a) (a) = prh_r56_be_to_host(a)
#define prh_set_r64_be_to_host(a) (a) = prh_r64_be_to_host(a)
#define prh_set_reg_be_to_host(a) (a) = prh_reg_be_to_host(a)
#define prh_set_raw_be_to_host(a) (a) = prh_raw_be_to_host(a)

#define prh_i16_host_to_le(n) ((prh_i16)prh_r16_host_to_le(n))
#define prh_i24_host_to_le(n) ((prh_i32)prh_r24_host_to_le(n))
#define prh_i32_host_to_le(n) ((prh_i32)prh_r32_host_to_le(n))
#define prh_i40_host_to_le(n) ((prh_i64)prh_r40_host_to_le(n))
#define prh_i48_host_to_le(n) ((prh_i64)prh_r48_host_to_le(n))
#define prh_i56_host_to_le(n) ((prh_i64)prh_r56_host_to_le(n))
#define prh_i64_host_to_le(n) ((prh_i64)prh_r64_host_to_le(n))
#define prh_int_host_to_le(n) ((prh_int)prh_reg_host_to_le(n))
#define prh_imt_host_to_le(n) ((prh_imt)prh_raw_host_to_le(n))
#define prh_i16_le_to_host(n) ((prh_i16)prh_r16_le_to_host(n))
#define prh_i24_le_to_host(n) ((prh_i32)prh_r24_le_to_host(n))
#define prh_i32_le_to_host(n) ((prh_i32)prh_r32_le_to_host(n))
#define prh_i40_le_to_host(n) ((prh_i64)prh_r40_le_to_host(n))
#define prh_i48_le_to_host(n) ((prh_i64)prh_r48_le_to_host(n))
#define prh_i56_le_to_host(n) ((prh_i64)prh_r56_le_to_host(n))
#define prh_i64_le_to_host(n) ((prh_i64)prh_r64_le_to_host(n))
#define prh_int_le_to_host(n) ((prh_int)prh_reg_le_to_host(n))
#define prh_imt_le_to_host(n) ((prh_imt)prh_raw_le_to_host(n))
#define prh_i16_host_to_be(n) ((prh_i16)prh_r16_host_to_be(n))
#define prh_i24_host_to_be(n) ((prh_i32)prh_r24_host_to_be(n))
#define prh_i32_host_to_be(n) ((prh_i32)prh_r32_host_to_be(n))
#define prh_i40_host_to_be(n) ((prh_i64)prh_r40_host_to_be(n))
#define prh_i48_host_to_be(n) ((prh_i64)prh_r48_host_to_be(n))
#define prh_i56_host_to_be(n) ((prh_i64)prh_r56_host_to_be(n))
#define prh_i64_host_to_be(n) ((prh_i64)prh_r64_host_to_be(n))
#define prh_int_host_to_be(n) ((prh_int)prh_reg_host_to_be(n))
#define prh_imt_host_to_be(n) ((prh_imt)prh_raw_host_to_be(n))
#define prh_i16_be_to_host(n) ((prh_i16)prh_r16_be_to_host(n))
#define prh_i24_be_to_host(n) ((prh_i32)prh_r24_be_to_host(n))
#define prh_i32_be_to_host(n) ((prh_i32)prh_r32_be_to_host(n))
#define prh_i40_be_to_host(n) ((prh_i64)prh_r40_be_to_host(n))
#define prh_i48_be_to_host(n) ((prh_i64)prh_r48_be_to_host(n))
#define prh_i56_be_to_host(n) ((prh_i64)prh_r56_be_to_host(n))
#define prh_i64_be_to_host(n) ((prh_i64)prh_r64_be_to_host(n))
#define prh_int_be_to_host(n) ((prh_int)prh_reg_be_to_host(n))
#define prh_imt_be_to_host(n) ((prh_imt)prh_raw_be_to_host(n))

#if prh_lit_endian
prh_inline prh_r16 prh_r16_host_to_le(prh_r16 n) { return n; }
prh_inline prh_r32 prh_r24_host_to_le(prh_r32 n) { return n; }
prh_inline prh_r32 prh_r32_host_to_le(prh_r32 n) { return n; }
prh_inline prh_r64 prh_r40_host_to_le(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r48_host_to_le(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r56_host_to_le(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r64_host_to_le(prh_r64 n) { return n; }
prh_inline prh_reg prh_reg_host_to_le(prh_reg n) { return n; }
prh_inline prh_raw prh_raw_host_to_le(prh_raw n) { return n; }

prh_inline prh_r16 prh_r16_le_to_host(prh_r16 n) { return n; }
prh_inline prh_r32 prh_r24_le_to_host(prh_r32 n) { return n; }
prh_inline prh_r32 prh_r32_le_to_host(prh_r32 n) { return n; }
prh_inline prh_r64 prh_r40_le_to_host(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r48_le_to_host(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r56_le_to_host(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r64_le_to_host(prh_r64 n) { return n; }
prh_inline prh_reg prh_reg_le_to_host(prh_reg n) { return n; }
prh_inline prh_raw prh_raw_le_to_host(prh_raw n) { return n; }

prh_inline prh_r16 prh_r16_host_to_be(prh_r16 n) { return prh_impl_r16_invert_order(n); }
prh_inline prh_r32 prh_r24_host_to_be(prh_r32 n) { return prh_impl_r24_invert_order(n); }
prh_inline prh_r32 prh_r32_host_to_be(prh_r32 n) { return prh_impl_r32_invert_order(n); }
prh_inline prh_r64 prh_r40_host_to_be(prh_r64 n) { return prh_impl_r40_invert_order(n); }
prh_inline prh_r64 prh_r48_host_to_be(prh_r64 n) { return prh_impl_r48_invert_order(n); }
prh_inline prh_r64 prh_r56_host_to_be(prh_r64 n) { return prh_impl_r56_invert_order(n); }
prh_inline prh_r64 prh_r64_host_to_be(prh_r64 n) { return prh_impl_r64_invert_order(n); }
prh_inline prh_reg prh_reg_host_to_be(prh_reg n) { return prh_impl_reg_invert_order(n); }
prh_inline prh_raw prh_raw_host_to_be(prh_raw n) { return prh_impl_raw_invert_order(n); }

prh_inline prh_r16 prh_r16_be_to_host(prh_r16 n) { return prh_impl_r16_invert_order(n); }
prh_inline prh_r32 prh_r24_be_to_host(prh_r32 n) { return prh_impl_r24_invert_order(n); }
prh_inline prh_r32 prh_r32_be_to_host(prh_r32 n) { return prh_impl_r32_invert_order(n); }
prh_inline prh_r64 prh_r40_be_to_host(prh_r64 n) { return prh_impl_r40_invert_order(n); }
prh_inline prh_r64 prh_r48_be_to_host(prh_r64 n) { return prh_impl_r48_invert_order(n); }
prh_inline prh_r64 prh_r56_be_to_host(prh_r64 n) { return prh_impl_r56_invert_order(n); }
prh_inline prh_r64 prh_r64_be_to_host(prh_r64 n) { return prh_impl_r64_invert_order(n); }
prh_inline prh_reg prh_reg_be_to_host(prh_reg n) { return prh_impl_reg_invert_order(n); }
prh_inline prh_raw prh_raw_be_to_host(prh_raw n) { return prh_impl_raw_invert_order(n); }
#else
prh_inline prh_r16 prh_r16_host_to_le(prh_r16 n) { return prh_impl_r16_invert_order(n); }
prh_inline prh_r32 prh_r24_host_to_le(prh_r32 n) { return prh_impl_r24_invert_order(n); }
prh_inline prh_r32 prh_r32_host_to_le(prh_r32 n) { return prh_impl_r32_invert_order(n); }
prh_inline prh_r64 prh_r40_host_to_le(prh_r64 n) { return prh_impl_r40_invert_order(n); }
prh_inline prh_r64 prh_r48_host_to_le(prh_r64 n) { return prh_impl_r48_invert_order(n); }
prh_inline prh_r64 prh_r56_host_to_le(prh_r64 n) { return prh_impl_r56_invert_order(n); }
prh_inline prh_r64 prh_r64_host_to_le(prh_r64 n) { return prh_impl_r64_invert_order(n); }
prh_inline prh_reg prh_reg_host_to_le(prh_reg n) { return prh_impl_reg_invert_order(n); }
prh_inline prh_raw prh_raw_host_to_le(prh_raw n) { return prh_impl_raw_invert_order(n); }

prh_inline prh_r16 prh_r16_le_to_host(prh_r16 n) { return prh_impl_r16_invert_order(n); }
prh_inline prh_r32 prh_r24_le_to_host(prh_r32 n) { return prh_impl_r24_invert_order(n); }
prh_inline prh_r32 prh_r32_le_to_host(prh_r32 n) { return prh_impl_r32_invert_order(n); }
prh_inline prh_r64 prh_r40_le_to_host(prh_r64 n) { return prh_impl_r40_invert_order(n); }
prh_inline prh_r64 prh_r48_le_to_host(prh_r64 n) { return prh_impl_r48_invert_order(n); }
prh_inline prh_r64 prh_r56_le_to_host(prh_r64 n) { return prh_impl_r56_invert_order(n); }
prh_inline prh_r64 prh_r64_le_to_host(prh_r64 n) { return prh_impl_r64_invert_order(n); }
prh_inline prh_reg prh_reg_le_to_host(prh_reg n) { return prh_impl_reg_invert_order(n); }
prh_inline prh_raw prh_raw_le_to_host(prh_raw n) { return prh_impl_raw_invert_order(n); }

prh_inline prh_r16 prh_r16_host_to_be(prh_r16 n) { return n; }
prh_inline prh_r32 prh_r24_host_to_be(prh_r32 n) { return n; }
prh_inline prh_r32 prh_r32_host_to_be(prh_r32 n) { return n; }
prh_inline prh_r64 prh_r40_host_to_be(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r48_host_to_be(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r56_host_to_be(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r64_host_to_be(prh_r64 n) { return n; }
prh_inline prh_reg prh_reg_host_to_be(prh_reg n) { return n; }
prh_inline prh_raw prh_raw_host_to_be(prh_raw n) { return n; }

prh_inline prh_r16 prh_r16_be_to_host(prh_r16 n) { return n; }
prh_inline prh_r32 prh_r24_be_to_host(prh_r32 n) { return n; }
prh_inline prh_r32 prh_r32_be_to_host(prh_r32 n) { return n; }
prh_inline prh_r64 prh_r40_be_to_host(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r48_be_to_host(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r56_be_to_host(prh_r64 n) { return n; }
prh_inline prh_r64 prh_r64_be_to_host(prh_r64 n) { return n; }
prh_inline prh_reg prh_reg_be_to_host(prh_reg n) { return n; }
prh_inline prh_raw prh_raw_be_to_host(prh_raw n) { return n; }
#endif

#ifdef __cplusplus
}
#endif
#endif // prh_include_prelude_h

#undef prh_impl_stdc_assert
#undef prh_impl_lang_assert
#undef prh_impl_sdl3_assert

#if defined(prh_using_lang_assert)
    #define prh_impl_lang_assert 1
#elif defined(prh_using_sdl3_assert)
    #define prh_impl_sdl3_assert 1
#elif defined(prh_using_stdc_assert)
    #define prh_impl_stdc_assert 1
#else
    #define prh_impl_stdc_assert 1
#endif

#if defined(prh_impl_stdc_assert)
#ifndef prh_include_stdc_assert_h
#define prh_include_stdc_assert_h
#ifdef __cplusplus
extern "C" {
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

#include <assert.h> // assert 如果在这之前定义了 NDEBUG 断言为空
#include <stdlib.h> // malloc calloc realloc free abort exit size_t
#include <stdio.h> // printf fprintf

#undef prh_impl_real_assert
#undef prh_impl_plus_assert
#undef prh_real_assert
#undef prh_real_plus_assert
#undef prh_assert
#undef prh_plus_assert

#define prh_impl_real_assert(a, line, caller) ((void)((a) || (prh_impl_assert_line_caller((line), (caller)), false)))
#define prh_impl_plus_assert(a, line, caller, ...) ((void)((a) || ((__VA_ARGS__), prh_impl_assert_line_caller((line), (caller)), false)))
#define prh_real_plus_assert(a, ...) prh_impl_plus_assert((a), __LINE__, prh_caller, __VA_ARGS__)

#if PRH_DEBUG
    #if defined(prh_line_caller_debug)
        #define prh_real_assert(a) prh_impl_real_assert(a, __LINE__, prh_caller)
    #else
        #define prh_real_assert(a) assert(a)
    #endif
    #define prh_plus_assert(a, ...) prh_real_plus_assert((a), __VA_ARGS__)
    #define prh_assert(a) prh_real_assert(a)
#else
    #define prh_real_assert(a) prh_impl_real_assert(a, __LINE__, prh_caller)
    #define prh_plus_assert(a, ...)
    #define prh_assert(a)
#endif

prh_inline void prh_impl_assert_line_caller(int line, prh_raw caller)
{
#if defined(prh_line_caller_debug)
    fprintf(stderr, "assert line %d caller %p\n", line, (void *)caller);
#else
    fprintf(stderr, "assert line %d file %s\n", line, (const char *)caller);
#endif
    abort();
}

#ifdef __cplusplus
}
#endif
#endif // prh_include_stdc_assert_h
#endif // prh_impl_stdc_assert

#ifndef prh_impl_prelude_extern_h
#define prh_impl_prelude_extern_h
#ifdef __cplusplus
extern "C" {
#endif

#define prh_arrlen(a) (sizeof(a)/sizeof((a)[0]))
#define prh_arrend(a) ((a) + prh_arrlen(a))
#define prh_arrelt(a, i) (a)[(prh_assert((i) >= 0 && (i) < prh_arrlen(a))), i]
#define prh_arrget(a) (a), prh_arrlen(a)

prh_inline prh_reg prh_impl_times_power_of_2(prh_reg n, prh_reg power_minus_one)
{
    prh_reg result = (n + power_minus_one) & (~power_minus_one);
    prh_assert(result >= n);
    return result;
}

prh_inline bool prh_impl_is_times_power_of_2(prh_reg n, prh_reg power_minus_one)
{
    return (n & power_minus_one) == 0;
}

prh_inline prh_reg prh_lower_most_bit(prh_reg n)
{
    return n & (-(prh_int)n); // 0000 & 0000 -> 0000, 0001 & 1111 -> 0001, 1010 & 0110 -> 0010
}

prh_inline prh_reg prh_remove_lower_most_bit(prh_reg n)
{
    return n & (n - 1);
}

prh_inline bool prh_is_times_4_byte(prh_reg n)
{
    return (n & 0x03) == 0;
}

prh_inline bool prh_is_times_8_byte(prh_reg n)
{
    return (n & 0x07) == 0;
}

prh_inline bool prh_is_times_16_byte(prh_reg n)
{
    return (n & 0xF) == 0;
}

prh_inline bool prh_is_times_ptr_size(prh_reg n)
{
    return (n & (sizeof(void *) - 1)) == 0;
}

prh_inline bool prh_is_times_line_size(prh_reg n)
{
    return (n & (prh_cache_line_size - 1)) == 0;
}

prh_inline bool prh_is_times_page_size(prh_reg n)
{
    return (n & (prh_memory_page_size - 1)) == 0;
}

prh_inline bool prh_is_times_vmem_size(prh_reg n)
{
    return (n & (prh_vmem_unit_size - 1)) == 0;
}

prh_inline bool prh_is_power_of_2(prh_reg n)
{
    return prh_remove_lower_most_bit(n) == 0; // power of 2 or zero
}

prh_inline bool prh_is_times_power_of_2(prh_reg n, prh_reg power_of_2)
{
    prh_assert(prh_is_power_of_2(power_of_2));
    return prh_impl_is_times_power_of_2(n, power_of_2 - 1);
}

prh_inline prh_reg prh_to_power_of_2(prh_reg n)
{
    if (prh_is_power_of_2(n)) return n;
    // TODO: 字节序交换然后计算lower most bit
    prh_reg m = prh_lower_most_bit(n);
    while (m < n) m <<= 1;
    return m;
}

prh_inline prh_reg prh_times_power_of_2(prh_reg n, prh_reg power_of_2)
{
    prh_assert(prh_is_power_of_2(power_of_2));
    return prh_impl_times_power_of_2(n, power_of_2 - 1);
}

prh_inline prh_reg prh_times_align_size(prh_reg n, prh_reg alignment)
{
    return prh_times_power_of_2(n, alignment);
}

#define prh_times_ptr_size(n) (((prh_reg)(n)+(prh_reg)(sizeof(void*)-1)) & (~(prh_reg)(sizeof(void*)-1)))
#define prh_times_line_size(n) (((prh_reg)(n)+prh_cache_line_size-1) & (~(prh_reg)(prh_cache_line_size-1)))
#define prh_times_page_size(n) (((prh_reg)(n)+prh_memory_page_size-1) & (~(prh_reg)(prh_memory_page_size-1)))
#define prh_times_4_byte(n) (((prh_reg)(n)+3) & (~(prh_reg)3))
#define prh_times_8_byte(n) (((prh_reg)(n)+7) & (~(prh_reg)7))
#define prh_times_16_byte(n) (((prh_reg)(n)+15) & (~(prh_reg)15))

#define prh_r32_times_ptr_size(n) (((prh_r32)(n)+(prh_r32)(sizeof(void*)-1)) & (~(prh_r32)(sizeof(void*)-1)))
#define prh_r32_times_line_size(n) (((prh_r32)(n)+prh_cache_line_size-1) & (~(prh_r32)(prh_cache_line_size-1)))
#define prh_r32_times_page_size(n) (((prh_r32)(n)+prh_memory_page_size-1) & (~(prh_r32)(prh_memory_page_size-1)))
#define prh_r32_times_4_byte(n) (((prh_r32)(n)+3) & (~(prh_r32)3))
#define prh_r32_times_8_byte(n) (((prh_r32)(n)+7) & (~(prh_r32)7))
#define prh_r32_times_16_byte(n) (((prh_r32)(n)+15) & (~(prh_r32)15))

#define prh_generic_bsearch(succ, out_i, value, p, size, eval) do {             \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) > eval(e + mid - 1)) { e += mid; end -= mid; }              \
        else end = mid; /* 大于中间值大于左半所有值，小于中间值小于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)); (succ) = ((value) == eval(e));              \
} while (0)

#define prh_generic_bsearch_first_less_equal(out_i, value, p, size, eval) do {  \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) > eval(e + mid - 1)) { e += mid; end -= mid; }              \
        else end = mid; /* 大于中间值大于左半所有值，小于中间值小于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)) + ((value) > eval(e));                       \
} while (0) /* 返回 size 表示失败 */

#define prh_generic_bsearch_last_greater_equal(out_i, value, p, size, eval) do {\
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) > eval(e + mid - 1)) { e += mid; end -= mid; }              \
        else end = mid; /* 大于中间值大于左半所有值，小于中间值小于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)) - ((value) < eval(e));                       \
} while (0) /* 返回 (prh_reg)-1 表示失败 */

#define prh_bsearch(succ, out_i, value, p, size) do {                           \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) > e[mid - 1]) { e += mid; end -= mid; }                     \
        else end = mid; /* 大于中间值大于左半所有值，小于中间值小于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)); (succ) = ((value) == *e);                   \
} while (0)

#define prh_bsearch_value_belong_range_by_comparing_the_range_end prh_bsearch_first_less_equal
#define prh_bsearch_value_belong_range_by_comparing_the_range_start prh_bsearch_last_greater_equal

#define prh_bsearch_first_less_equal(out_i, value, p, size) do {                \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) > e[mid - 1]) { e += mid; end -= mid; }                     \
        else end = mid; /* 大于中间值大于左半所有值，小于中间值小于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)) + ((value) > *e);                            \
} while (0) /* 返回 size 表示失败 */

#define prh_bsearch_last_greater_equal(out_i, value, p, size) do {              \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) > e[mid - 1]) { e += mid; end -= mid; }                     \
        else end = mid; /* 大于中间值大于左半所有值，小于中间值小于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)) - ((value) < *e);                            \
} while (0) /* 返回 (prh_reg)-1 表示失败 */

#define prh_descending_bsearch(succ, out_i, value, p, size) do {                \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) < e[mid - 1]) { e += mid; end -= mid; }                     \
        else end = mid; /* 小于中间值小于左半所有值，大于中间值大于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)); (succ) = ((value) == *e);                   \
} while (0)

#define prh_descending_bsearch_first_greater_equal(out_i, value, p, size) do {  \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) < e[mid - 1]) { e += mid; end -= mid; }                     \
        else end = mid; /* 小于中间值小于左半所有值，大于中间值大于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)) + ((value) < *e);                            \
} while (0) /* 返回 size 表示失败 */

#define prh_descending_bsearch_last_less_equal(out_i, value, p, size) do {      \
    prh_typeof(&(p)[0]) e = (p); prh_reg end = (size), mid;                     \
    while ((mid = end >> 1)) { /* 二分无限逼近值 value */                       \
        if ((value) < e[mid - 1]) { e += mid; end -= mid; }                     \
        else end = mid; /* 小于中间值小于左半所有值，大于中间值大于右半所有值 */\
    } (out_i) = (prh_reg)(e - (p)) - ((value) > *e);                            \
} while (0) /* 返回 (prh_reg)-1 表示失败 */

#define prh_fast_bsearch_first_less_equal(out_i, value, p, size) do {           \
    prh_typeof(&(p)[0]) e = (p); prh_reg n = (size); prh_assert(n != 0);        \
    prh_r32 shifts = prh_reg_unchecked_higher_most_bit_position(n);             \
    /* [0 1 2 3]            1.  当长度就是2的幂，elem_ptr 不会移动      */      \
    /*  ^                                                               */      \
    /* [0 1 2 | 3 4 5 6]    2.  不是2的幂，要查找的值在后一2的幂部分    */      \
    /*          ^                                                       */      \
    /* [0 1 2 3 | 4 5 6]    3.  不是2的幂，要查找的值在前一2的幂部分    */      \
    /*  ^                                                               */      \
    if ((value) >= e[(n -= ((prh_reg)1 << shifts))]) e += n;                    \
    while (shifts--) {                                                          \
        if ((value) > e[((prh_reg)1<<shifts)-1]) e += ((prh_reg)1 << shifts);   \
    } /* 循环 shifts 次，如果 shifts 为 2 即长度 4，循环 2 次检查 [0 1 | 2 3] 和 [0 | 1 | 2 | 3] */ \
    (out_i) = (prh_reg)(e - (p)) + ((value) > *e);                              \
} while (0) /* 返回 size 表示失败 */

#define prh_fast_bsearch_last_greater_equal(out_i, v, p, size) do {             \
    prh_typeof(&(p)[0]) e = (p); prh_reg n = (size); prh_assert(n != 0);        \
    prh_r32 s = prh_reg_unchecked_higher_most_bit_position(n);                  \
    if ((v) >= e[(n -= ((prh_reg)1<<s))]) e += n;                               \
    while (s--) if ((v) > e[((prh_reg)1 << s) - 1]) e += ((prh_reg)1 << s);     \
    (out_i) = (prh_reg)(e - (p)) - ((v) < *e);                                  \
} while (0) /* 返回 (prh_reg)-1 表示失败 */

#define prh_fast_descending_bsearch_first_greater_equal(out_i, v, p, size) do { \
    prh_typeof(&(p)[0]) e = (p); prh_reg n = (size); prh_assert(n != 0);        \
    prh_r32 s = prh_reg_unchecked_higher_most_bit_position(n);                  \
    if ((v) <= e[(n -= ((prh_reg)1<<s))]) e += n;                               \
    while (s--) if ((v) < e[((prh_reg)1 << s) - 1]) e += ((prh_reg)1 << s);     \
    (out_i) = (prh_reg)(e - (p)) + ((v) < *e);                                  \
} while (0) /* 返回 size 表示失败 */

#define prh_fast_descending_bsearch_last_less_equal(out_i, v, p, size) do {     \
    prh_typeof(&(p)[0]) e = (p); prh_reg n = (size); prh_assert(n != 0);        \
    prh_r32 s = prh_reg_unchecked_higher_most_bit_position(n);                  \
    if ((v) <= e[(n -= ((prh_reg)1<<s))]) e += n;                               \
    while (s--) if ((v) < e[((prh_reg)1 << s) - 1]) e += ((prh_reg)1 << s);     \
    (out_i) = (prh_reg)(e - (p)) - ((v) > *e);                                  \
} while (0) /* 返回 (prh_reg)-1 表示失败 */

#ifdef __cplusplus
}
#endif
#endif // prh_impl_prelude_extern_h

#undef prh_multiple_include_different_config
#define prh_multiple_include_different_config 1

#undef prh_impl_stdc_alloc
#undef prh_impl_lang_alloc
#undef prh_impl_sdl3_alloc

#if defined(prh_using_lang_alloc)
    #define prh_impl_lang_alloc 1
#elif defined(prh_using_sdl3_alloc)
    #define prh_impl_sdl3_alloc 1
#elif defined(prh_using_stdc_alloc)
    #define prh_impl_stdc_alloc 1
#else
    #define prh_impl_stdc_alloc 1
#endif

#if defined(prh_impl_stdc_alloc)
#ifndef prh_include_stdc_alloc_h
#define prh_include_stdc_alloc_h
#ifdef __cplusplus
extern "C" {
#endif

// https://learn.microsoft.com/en-us/cpp/c-runtime-library/find-memory-leaks-using-the-crt-library
//
// 检测内存泄漏的主要工具是 C/C++ 调试器和 CRT 调试堆函数。要启用所有调试堆函数，
// 需要在你的 C++ 程序中按以下顺序包含下面的语句。其中 #define _CRTDBG_MAP_ALLOC
// 将 CRT 堆函数的基本版本映射到相应的调试版本。如果省略了 #define 语句，内存泄漏
// 信息将不够详细。包含 crtdbg.h 会将 malloc 和 free 函数映射到它们的调试版本 _malloc_dbg
// 和 _free_dbg，这些版本会跟踪内存分配和释放。这种映射仅在具有 _DEBUG 的调试生成
// 中发生。发布生成使用普通的 malloc 和 free 函数。
// 通过使用前面的语句启用了调试堆函数后，在应用程序退出点之前调用 _CrtDumpMemoryLeaks()，
// 以在应用程序退出时显示内存泄漏报告。如果你的应用程序有多个退出点，你无需手动在每
// 个退出点放置 _CrtDumpMemoryLeaks()。为了在每个退出点自动调用 _CrtDumpMemoryLeaks()，
// 在应用程序开头放置一个对 _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
// 的调用即可。
// 默认情况下， _CrtDumpMemoryLeaks 将内存泄漏报告输出到“输出”窗口的“调试”窗格。
// 如果你使用了库，库可能会将输出重置到另一个位置。你可以使用 _CrtSetReportMode
// 将报告重定向到另一个位置，或者像下面这样重新定向回“输出”窗口：
//      _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
//
// 内存块类型有 “normal”、“client” 和 “CRT”。普通块是由你的程序分配的普通内存。
// “client”块是 MFC 程序用于需要析构函数的对象的特殊类型的内存块。MFC 的 new 运
// 算符会根据被创建的对象类型创建普通块或客户端块。“CRT” 块是由 CRT 库为其自身用
// 途分配的。CRT 库会处理这些块的释放，因此除非 CRT 库出现严重问题，否则 CRT 块
// 不会出现在内存泄漏报告中。还有两种内存块类型永远不会出现在内存泄漏报告中。“free”
// 块是已经释放的内存，按定义不会泄漏。“ignore” 块是你明确标记为从内存泄漏报告中
// 排除的内存。
//
// 前面的技术可以识别使用标准 CRT malloc 函数分配的内存的内存泄漏。然而，如果你的
// 程序使用 C++ 的 new 运算符分配内存，你可能只能在内存泄漏报告中看到 operator
// new 调用 _malloc_dbg 的文件名和行号。为了创建更有用的内存泄漏报告，你可以编写
// 一个像下面这样的宏来报告 new 分配的行，然后你可以使用 DBG_NEW 宏在代码中替换
// new 运算符。
// #ifdef _DEBUG
//     #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//     // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//     // allocations to be of _CLIENT_BLOCK type
// #else
//     #define DBG_NEW new
// #endif
//
// 内存分配编号可以告诉你泄漏的内存块是在何时分配的。例如，一个内存分配编号为 18
// 的块是在应用程序运行期间分配的第 18 个内存块。CRT 报告会统计运行期间的所有内存
// 块分配，包括 CRT 库和其他库（如 MFC）的分配。因此，内存分配块编号 18 可能不是
// 你的代码分配的第 18 个内存块。你可以使用分配编号在内存分配上设置断点。

#if PRH_DEBUG && defined(prh_msc_version)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <stdlib.h> // malloc calloc realloc free abort exit size_t
#include <string.h> // memcpy memmove memset
// void *memcpy(void *dest, const void *src, size_t count);
// void *memmove(void *dest, const void *src, size_t count);
// void *memset(void *ptr, int value, size_t count);
// if either dest or src is an invalid or null pointer { undefined behavior }
// if dest and src memory overlap for memcpy { undefined behavior }
// the count parameter can be set to zero value.

// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/crt-alphabetical-function-reference
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/aligned-malloc
// https://en.cppreference.com/w/c/memory/aligned_alloc
// https://www.man7.org/linux/man-pages/man3/posix_memalign.3.html
// https://jemalloc.net/
//
// Maximum heap request the heap manager will attempt
// #ifdef _WIN64
// #define _HEAP_MAXREQ 0xFFFFFFFFFFFFFFE0
// #else
// #define _HEAP_MAXREQ 0xFFFFFFE0
// #endif
//
// void *_aligned_offset_malloc(size_t size, size_t alignment, size_t offset);
//      offset - [0, size)
//      (ptr + offset) % alignment == 0
//
// 在指定的对齐边界上分配内存。参数 size 请求的内存分配大小。alignment 对齐值，必须是 2 的整数次幂。
// offset 内存分配中的偏移量，用于在该位置强制对齐。返回指向已分配内存块的指针，操作失败返回 NULL。
//
// _aligned_offset_malloc 适用于需要在嵌套元素上对齐的场景。例如，需要在嵌套类（nested class）上对齐
// 的情况。_aligned_offset_malloc 基于 malloc 实现，_aligned_offset_malloc 标记有 __declspec(noalias)
// 和 __declspec(restrict)，意味着保证该函数不修改全局变量，且返回的指针没有别名。更多信息参见 noalias
// 和 restrict。
// https://learn.microsoft.com/en-us/cpp/cpp/noalias
// https://learn.microsoft.com/en-us/cpp/cpp/restrict
//
// 如果内存分配失败，或请求的大小大于 _HEAP_MAXREQ，此函数将 errno 设置为 ENOMEM。关于 errno 的更多
// 信息，参见 errno、_doserrno、_sys_errlist 和 _sys_nerr。此外，_aligned_offset_malloc 会校验其参数，
// 如果 alignment 不是 2 的幂，或者 offset 非零且大于等于 size，此函数将调用无效参数处理程序（invalid
// parameter handler）。如果允许继续执行，此函数返回 NULL 并将 errno 设置为 EINVAL。默认情况下，此函
// 数的全局状态作用域限定于应用程序。要更改此行为，参见"CRT 中的全局状态"。
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/parameter-validation
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/global-state

#undef prh_impl_plat_aligned_realloc

#if defined(prh_msc_version)
    #include <malloc.h> // _aligned_malloc _aligned_realloc _aligned_free
    // _aligned_malloc is based on malloc. required C header malloc.h.
    // if alignment isn't a power of 2 or size is zero, this function invokes
    // the invalid parameter handler. if execution is allowed to continue, this
    // function returns NULL and sets errno to EINVAL.
    #define prh_impl_plat_aligned_malloc(size, alignment) _aligned_malloc((size), (alignment))
    #define prh_impl_plat_aligned_realloc(p, size, alignment) _aligned_realloc((p), (size), (alignment))
    #define prh_impl_plat_aligned_dealloc(p) _aligned_free(p) // _aligned_free(p) if p is a null, this function simply performs no actions
#else
    // behavior is undefined if size is not an integral multiple of alignment
    void *aligned_alloc(size_t alignment, size_t size); // glibc 2.16. C11
    #define prh_impl_plat_aligned_malloc(size, alignment) aligned_alloc((alignment), (size))
    #define prh_impl_plat_aligned_dealloc(p) prh_impl_rawc_free(p) // free(p) if p is null do nothing
#endif

prh_inline void *prh_impl_rawc_malloc(prh_reg capacity)
{
    return malloc(capacity);
}

prh_inline void *prh_impl_rawc_calloc(prh_reg capacity)
{
    return calloc(1, capacity);
}

prh_inline void *prh_impl_rawc_realloc(void *buffer, prh_reg new_capacity)
{
    return realloc(buffer, new_capacity);
}

prh_inline void prh_impl_rawc_free(void *buffer)
{
    free(buffer);
}

#if defined(malloc)
#undef malloc
#endif

#if defined(calloc)
#undef calloc
#endif

#if defined(realloc)
#undef realloc
#endif

#if defined(free)
#undef free
#endif

#define prh_stdc_malloc(capacity) prh_impl_stdc_malloc((capacity), __LINE__, prh_caller)
#define prh_stdc_calloc(capacity) prh_impl_stdc_calloc((capacity), __LINE__, prh_caller)
#define prh_stdc_realloc(buffer, new_capacity) prh_impl_stdc_realloc((buffer), (new_capacity), __LINE__, prh_caller)
#define prh_stdc_dealloc(buffer) prh_impl_stdc_dealloc(buffer) // 规范 prh_stdc_realloc 只做分配操作，不释放内存，释放内存必须调用 prh_stdc_dealloc

#define prh_stdc_aligned_malloc(capacity, alignment) prh_impl_stdc_aligned_malloc((capacity), (alignment), __LINE__, prh_caller)
#define prh_stdc_aligned_calloc(capacity, alignment) prh_impl_stdc_aligned_calloc((capacity), (alignment), __LINE__, prh_caller)
#define prh_stdc_aligned_realloc(buffer, new_capacity, alignment) prh_impl_stdc_aligned_realloc((buffer), (new_capacity), (alignment), __LINE__, prh_caller)
#define prh_stdc_aligned_dealloc(buffer) prh_impl_stdc_aligned_dealloc((buffer))

#define prh_stdc_line_aligned_malloc(capacity) prh_stdc_aligned_malloc((capacity), PRH_ALIGN_LINE)
#define prh_stdc_page_aligned_malloc(capacity) prh_stdc_aligned_malloc((capacity), PRH_ALIGN_PAGE)
#define prh_stdc_vmem_aligned_malloc(capacity) prh_stdc_aligned_malloc((capacity), PRH_ALIGN_VMEM)
#define prh_stdc_line_aligned_calloc(capacity) prh_stdc_aligned_calloc((capacity), PRH_ALIGN_LINE)
#define prh_stdc_page_aligned_calloc(capacity) prh_stdc_aligned_calloc((capacity), PRH_ALIGN_PAGE)
#define prh_stdc_vmem_aligned_calloc(capacity) prh_stdc_aligned_calloc((capacity), PRH_ALIGN_VMEM)

// void *malloc(size_t size);
// the newly allocated block of memory is not initialized, remaining with indeterminate values.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.

prh_inline void *prh_impl_stdc_malloc(prh_reg capacity, int line, prh_reg caller)
{
    void *buffer = prh_impl_rawc_malloc(capacity); // 如果 capacity 为零，可能返回 null 也可能返回正常地址，但该地址不能访问
    prh_impl_real_assert(buffer != prh_null, line, caller); // 对于 aligned_alloc，如果 capacity 大小不是对齐的整数倍或者为零，或者对齐字节数不是2的幂，都将触发非法处理
    return buffer;
}

prh_inline void *prh_impl_stdc_aligned_malloc(prh_reg capacity, prh_reg alignment, int line, prh_reg caller)
{
    // 对于 aligned_malloc，如果 capacity 大小不是对齐的整数倍或者为零，或者对齐字节数不是2的幂，都将触发非法处理
    // 分配零字节长度正常返回或返回空，总之返回的地址位置不能访问，可以统一标准都返回非空，并强制要求不能将空传给dealloc
    capacity = prh_times_align_size(prh_set_value_if_zero(1, capacity), alignment);
    void *buffer = prh_impl_plat_aligned_malloc(capacity, alignment + 1);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

// void *calloc(size_t num, size_t size);
// allocates a block of memory for an array of num elements, each of them size bytes long, and
// initializes all its bits to zero. the effective result is the allocation of a zero-initialized
// memory block of (num*size) bytes.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.

prh_inline void *prh_impl_stdc_calloc(prh_reg capacity, int line, prh_reg caller)
{
    void *buffer = prh_impl_rawc_calloc(capacity);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void *prh_impl_stdc_aligned_calloc(prh_reg capacity, prh_reg alignment, int line, prh_reg caller)
{
    void *buffer = prh_impl_stdc_aligned_malloc(capacity, alignment, line, caller);
    memset(buffer, 0, capacity);
    return buffer;
}

// void free(void *ptr) if ptr is null do nothing

prh_inline void prh_impl_stdc_dealloc(void *buffer)
{
    prh_impl_rawc_free(buffer);
}

prh_inline void prh_impl_stdc_aligned_dealloc(void *buffer)
{
    prh_impl_plat_aligned_dealloc(buffer); // 如果 buffer 为空，prh_impl_plat_aligned_dealloc 不做任何事
}

prh_export void *prh_impl_stdc_realloc(void *buffer, prh_reg new_capacity, int line, prh_reg caller);
prh_export void *prh_impl_stdc_aligned_realloc(void *buffer, prh_reg new_capacity, prh_reg alignment, int line, prh_reg caller);

#ifdef __cplusplus
}
#endif
#endif // prh_include_stdc_alloc_h

#if defined(prh_source_implement)
#ifdef __cplusplus
extern "C" {
#endif

// void *realloc(void *ptr, size_t size);
// if ptr == prh_null { return malloc(size) }
// if size == 0 { may be free(ptr) or depends on library implementation }
// if size > ptr old size { may return the new location and the newer portion is indeterminate }
// the content is preserved up to min(old and new size), even if moved to a new location.
// if fails to allocate the requested block of memory, null is returned and ptr remain unchanged.
// if ptr != NULL
//      if size != 0
//          return realloc(ptr, size)
//      else
//          **MAYBE** free(ptr) return NULL *** 规范 prh_stdc_realloc 只做分配操作，不释放内存
// else
//      // if size = 0 may or may not return null, but the returned pointer shall not be dereferenced
//      return malloc(size)

void *prh_impl_stdc_realloc(void *buffer, prh_reg new_capacity, int line, prh_reg caller)
{
    // 规范 prh_stdc_realloc 只做分配操作，不释放内存，释放内存必须调用 prh_stdc_dealloc
    // 当 buffer 为空时直接分配 new_capacity 大小内存，当 buffer 不为空时，重新分配到 new_capacity 大小
    new_capacity = prh_set_value_if_zero(sizeof(void *), new_capacity);
    buffer = prh_impl_rawc_realloc(buffer, new_capacity);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

void *prh_impl_stdc_aligned_realloc(void *buffer, prh_reg capacity, prh_reg alignment, int line, prh_reg caller)
{
    // 规范 prh_stdc_realloc 只做分配操作，不释放内存，释放内存必须调用 prh_stdc_dealloc
    // 当 buffer 为空时直接分配 capacity 大小内存，当 buffer 不为空时，重新分配到 capacity 大小
    capacity = prh_times_align_size(prh_set_value_if_zero(1, capacity), alignment);
#if defined(prh_impl_plat_aligned_realloc)
    buffer = prh_impl_plat_aligned_realloc(buffer, capacity, alignment + 1);
    prh_impl_real_assert(buffer != prh_null, line, caller);
#else
    if (buffer == prh_null)
    {
        buffer = prh_impl_plat_aligned_malloc(capacity, alignment + 1);
        prh_impl_real_assert(buffer != prh_null, line, caller);
    }
    else
    {
        void *old_buffer = buffer;
        buffer = prh_impl_plat_aligned_malloc(capacity, alignment + 1);
        prh_impl_real_assert(buffer != prh_null, line, caller);
        memmove(buffer, old_buffer, capacity); // capacity 可能扩大或缩小
        prh_impl_plat_aligned_dealloc(old_buffer);
    }
#endif
    return buffer;
}

#ifdef __cplusplus
}
#endif
#endif // prh_source_implement
#endif // prh_impl_stdc_alloc

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