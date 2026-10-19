// prh_prelude.h - v0.01 - public domain - swdayu <github.com/swdayu>
// No warranty implied, use at your own risk.

#ifndef prh_include_nothing_h
#ifdef __cplusplus
extern "C" {
#endif

#define prh_multiple_time_include 1

#ifndef prh_include_prelude_h
#define prh_include_prelude_h

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
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__) // C11 keyword
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

#define prh_i08_min ((prh_i08)-127-1)   // 0x80 128
#define prh_i08_max ((prh_i08)127)      // 0x7f 127
#define prh_i08_umx ((prh_r08)255)      // 0xff 255
#define prh_i16_min ((prh_i16)-32767-1) // 0x8000 32768
#define prh_i16_max ((prh_i16)32767)    // 0x7fff 32767
#define prh_i16_umx ((prh_r16)65535)    // 0xffff 65535
#define prh_i32_min ((prh_i32)-2147483647-1) // 0x8000_0000 2147483648
#define prh_i32_max ((prh_i32)2147483647)    // 0x7fff_ffff 2147483647
#define prh_i32_umx ((prh_r32)4294967295)    // 0xffff_ffff 4294967295
#define prh_i64_min ((prh_i64)-9223372036854775807LL-1) // 0x8000_0000_0000_0000  9223372036854775808
#define prh_i64_max ((prh_i64)9223372036854775807)      // 0x7fff_ffff_ffff_ffff  9223372036854775807
#define prh_i64_umx ((prh_r64)18446744073709551615)     // 0xffff_ffff_ffff_ffff 18446744073709551615
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
    #define prh_imt_min prh_i32_min
    #define prh_imt_max prh_i32_max
    #define prh_imt_umx prh_i32_umx
    #define prh_imt_bits 32
    #define prh_imt_size 4
    typedef prh_i32 prh_imt;
    typedef prh_r32 prh_raw;
    typedef prh_i32 prh_int;
    typedef prh_r32 prh_reg;
    #define prh_int_bits 32
    #define prh_int_size 4
#elif prh_arch_bits == 64
    #define prh_imt_min prh_i64_min
    #define prh_imt_max prh_i64_max
    #define prh_imt_umx prh_i64_umx
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
    #define prh_int_min prh_i64_min
    #define prh_int_max prh_i64_max
    #define prh_int_umx prh_i64_umx
#elif prh_int_bits == 32
    #define prh_int_min prh_i32_min
    #define prh_int_max prh_i32_max
    #define prh_int_umx prh_i32_umx
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

prh_inline prh_r08 prh_r08_set_value_if_true(prh_r08 value, prh_r08 orelse, bool b)
{
    return (b == true) * value + (b == false) * orelse;
}

prh_inline prh_r16 prh_r16_set_value_if_true(prh_r16 value, prh_r16 orelse, bool b)
{
    return (b == true) * value + (b == false) * orelse;
}

prh_inline prh_r32 prh_r32_set_value_if_true(prh_r32 value, prh_r32 orelse, bool b)
{
    return (b == true) * value + (b == false) * orelse;
}

prh_inline prh_r32 prh_r32_set_value_if_zero(prh_r32 value, prh_r32 a)
{
    return prh_r32_set_value_if_true(value, a, a == 0);
}

prh_inline prh_r64 prh_r64_set_value_if_true(prh_r64 value, prh_r64 orelse, bool b)
{
    return (b == true) * value + (b == false) * orelse;
}

prh_inline prh_r64 prh_r64_set_value_if_zero(prh_r64 value, prh_r64 a)
{
    return prh_r64_set_value_if_true(value, a, a == 0);
}

#define prh_set_value_if_true prh_reg_set_value_if_true
#define prh_set_value_if_zero prh_reg_set_value_if_zero

#if prh_int_bits == 64
    #define prh_reg_set_value_if_true prh_r64_set_value_if_true
    #define prh_reg_set_value_if_zero prh_r64_set_value_if_zero
#else
    #define prh_reg_set_value_if_true prh_r32_set_value_if_true
    #define prh_reg_set_value_if_zero prh_r32_set_value_if_zero
#endif

#if prh_imt_bits == 64
    #define prh_raw_set_value_if_true prh_r64_set_value_if_true
    #define prh_raw_set_value_if_zero prh_r64_set_value_if_zero
#else
    #define prh_raw_set_value_if_true prh_r32_set_value_if_true
    #define prh_raw_set_value_if_zero prh_r32_set_value_if_zero
#endif

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
    unsigned long lower_most_bit_position; // 当 n 为 0 时，最低置位的位置没有定义
    _BitScanForward(&lower_most_bit_position, n);
    return (prh_r32)lower_most_bit_position;
}

prh_inline prh_r32 prh_r32_unchecked_higher_most_bit_position(prh_r32 n)
{
    unsigned long higher_most_bit_position; // 当 n 为 0 时，最高置位的位置没有定义
    _BitScanReverse(&higher_most_bit_position, n);
    return (prh_r32)higher_most_bit_position;
}

prh_inline prh_r32 prh_r32_unchecked_trailing_zeros(prh_r32 n)
{
    unsigned long lower_most_bit_position; // 低位零的个数
    prh_byte has_set_bit = _BitScanForward(&lower_most_bit_position, n);
    return (prh_r32)lower_most_bit_position;
}

prh_inline prh_r32 prh_r32_unchecked_leading_zeros(prh_r32 n)
{
    unsigned long higher_most_bit_position; // 高位零的个数
    prh_byte has_set_bit = _BitScanReverse(&higher_most_bit_position, n);
    return (prh_r32)(31 - higher_most_bit_position);
}

prh_inline prh_r32 prh_r32_trailing_zeros(prh_r32 n)
{
    unsigned long lower_most_bit_position; // 低位零的个数
    prh_byte has_set_bit = _BitScanForward(&lower_most_bit_position, n);
    return prh_r32_set_value_if_true(lower_most_bit_position, 32, has_set_bit);
}

prh_inline prh_r32 prh_r32_leading_zeros(prh_r32 n)
{
    unsigned long higher_most_bit_position; // 高位零的个数
    prh_byte has_set_bit = _BitScanReverse(&higher_most_bit_position, n);
    return prh_r32_set_value_if_true(31 - higher_most_bit_position, 32, has_set_bit);
}

#if defined(prh_arch_x64) || defined(prh_arch_a64)

prh_inline prh_r32 prh_r64_unchecked_lower_most_bit_position(prh_r64 n)
{
    unsigned long lower_most_bit_position; // 当 n 为 0 时，最低置位的位置没有定义
    _BitScanForward64(&lower_most_bit_position, n);
    return (prh_r32)lower_most_bit_position;
}

prh_inline prh_r32 prh_r64_unchecked_higher_most_bit_position(prh_r64 n)
{
    unsigned long higher_most_bit_position; // 当 n 为 0 时，最高置位的位置没有定义
    _BitScanReverse64(&higher_most_bit_position, n);
    return (prh_r32)higher_most_bit_position;
}

prh_inline prh_r32 prh_r64_unchecked_trailing_zeros(prh_r64 n)
{
    unsigned long lower_most_bit_position; // 低位零的个数
    prh_byte has_set_bit = _BitScanForward64(&lower_most_bit_position, n);
    return (prh_r32)lower_most_bit_position;
}

prh_inline prh_r32 prh_r64_unchecked_leading_zeros(prh_r64 n)
{
    unsigned long higher_most_bit_position; // 高位零的个数
    prh_byte has_set_bit = _BitScanReverse64(&higher_most_bit_position, n);
    return (prh_r32)(63 - higher_most_bit_position);
}

prh_inline prh_r32 prh_r64_trailing_zeros(prh_r64 n)
{
    unsigned long lower_most_bit_position; // 低位零的个数
    prh_byte has_set_bit = _BitScanForward64(&lower_most_bit_position, n);
    return prh_r32_set_value_if_true(lower_most_bit_position, 64, has_set_bit);
}

prh_inline prh_r32 prh_r64_leading_zeros(prh_r64 n)
{
    unsigned long higher_most_bit_position; // 高位零的个数
    prh_byte has_set_bit = _BitScanReverse64(&higher_most_bit_position, n);
    return prh_r32_set_value_if_true(63 - higher_most_bit_position, 64, has_set_bit);
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

prh_inline prh_r32 prh_r32_unchecked_lower_most_bit_position(prh_r32 n)
{
    return __builtin_ctz(n); /* if n is 0, the result is undefined */
}

prh_inline prh_r32 prh_r32_unchecked_higher_most_bit_position(prh_r32 n)
{
    return 31 - __builtin_clz(n);
}

prh_inline prh_r32 prh_r64_unchecked_lower_most_bit_position(prh_r64 n)
{
    return __builtin_ctzll(n);
}

prh_inline prh_r32 prh_r64_unchecked_higher_most_bit_position(prh_r64 n)
{
    return 63 - __builtin_clzll(n);
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

prh_inline prh_r32 prh_r32_trailing_zeros(prh_r32 n)
{
    return prh_r32_set_value_if_true(__builtin_ctz(n), 32, n);
}

prh_inline prh_r32 prh_r32_leading_zeros(prh_r32 n)
{
    return prh_r32_set_value_if_true(__builtin_clz(n), 32, n);
}

prh_inline prh_r32 prh_r64_trailing_zeros(prh_r64 n)
{
    return prh_r32_set_value_if_true(__builtin_ctzll(n), 64, n);
}

prh_inline prh_r32 prh_r64_leading_zeros(prh_r64 n)
{
    return prh_r32_set_value_if_true(__builtin_clzll(n), 64, n);
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

#if prh_int_bits == 64
prh_inline prh_r32 prh_reg_unchecked_higher_most_bit_position(prh_reg n) { return prh_r64_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_reg_unchecked_lower_most_bit_position(prh_reg n) { return prh_r64_unchecked_lower_most_bit_position(n); }

prh_inline prh_r32 prh_reg_unchecked_trailing_zeros(prh_reg n) { return prh_r64_unchecked_trailing_zeros(n); }
prh_inline prh_r32 prh_reg_unchecked_leading_zeros(prh_reg n) { return prh_r64_unchecked_leading_zeros(n); }

prh_inline prh_r32 prh_reg_trailing_zeros(prh_reg n) { return prh_r64_trailing_zeros(n); }
prh_inline prh_r32 prh_reg_leading_zeros(prh_reg n) { return prh_r64_leading_zeros(n); }

prh_inline prh_r32 prh_reg_set_bit_count(prh_reg n) { return prh_r64_set_bit_count(n); }
#else
prh_inline prh_r32 prh_reg_unchecked_higher_most_bit_position(prh_reg n) { return prh_r32_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_reg_unchecked_lower_most_bit_position(prh_reg n) { return prh_r32_unchecked_lower_most_bit_position(n); }

prh_inline prh_r32 prh_reg_unchecked_trailing_zeros(prh_reg n) { return prh_r32_unchecked_trailing_zeros(n); }
prh_inline prh_r32 prh_reg_unchecked_leading_zeros(prh_reg n) { return prh_r32_unchecked_leading_zeros(n); }

prh_inline prh_r32 prh_reg_trailing_zeros(prh_reg n) { return prh_r32_trailing_zeros(n); }
prh_inline prh_r32 prh_reg_leading_zeros(prh_reg n) { return prh_r32_leading_zeros(n); }

prh_inline prh_r32 prh_reg_set_bit_count(prh_reg n) { return prh_r32_set_bit_count(n); }
#endif

#if prh_imt_bits == 64
prh_inline prh_r32 prh_raw_unchecked_higher_most_bit_position(prh_raw n) { return prh_r64_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_raw_unchecked_lower_most_bit_position(prh_raw n) { return prh_r64_unchecked_lower_most_bit_position(n); }

prh_inline prh_r32 prh_raw_unchecked_trailing_zeros(prh_raw n) { return prh_r64_unchecked_trailing_zeros(n); }
prh_inline prh_r32 prh_raw_unchecked_leading_zeros(prh_raw n) { return prh_r64_unchecked_leading_zeros(n); }

prh_inline prh_r32 prh_raw_trailing_zeros(prh_raw n) { return prh_r64_trailing_zeros(n); }
prh_inline prh_r32 prh_raw_leading_zeros(prh_raw n) { return prh_r64_leading_zeros(n); }

prh_inline prh_r32 prh_raw_set_bit_count(prh_raw n) { return prh_r64_set_bit_count(n); }
#else
prh_inline prh_r32 prh_raw_unchecked_higher_most_bit_position(prh_raw n) { return prh_r32_unchecked_higher_most_bit_position(n); }
prh_inline prh_r32 prh_raw_unchecked_lower_most_bit_position(prh_raw n) { return prh_r32_unchecked_lower_most_bit_position(n); }

prh_inline prh_r32 prh_raw_unchecked_trailing_zeros(prh_raw n) { return prh_r32_unchecked_trailing_zeros(n); }
prh_inline prh_r32 prh_raw_unchecked_leading_zeros(prh_raw n) { return prh_r32_unchecked_leading_zeros(n); }

prh_inline prh_r32 prh_raw_trailing_zeros(prh_raw n) { return prh_r32_trailing_zeros(n); }
prh_inline prh_r32 prh_raw_leading_zeros(prh_raw n) { return prh_r32_leading_zeros(n); }

prh_inline prh_r32 prh_raw_set_bit_count(prh_raw n) { return prh_r32_set_bit_count(n); }
#endif

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

#define prh_macro_stmt_block(...) do { __VA_ARGS__ } while (0)

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

#if prh_int_bits == 64
    #define prh_impl_align_int_size prh_impl_align_8_byte
#else
    #define prh_impl_align_int_size prh_impl_align_4_byte
#endif

#if prh_imt_bits == 64
    #define prh_impl_align_imt_size prh_impl_align_8_byte
#else
    #define prh_impl_align_imt_size prh_impl_align_4_byte
#endif

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

#define prh_macro_r32_clear_bits(value, mask) value &= ~(prh_r32)(mask)
#define prh_macro_r64_clear_bits(value, mask) value &= ~(prh_r64)(mask)

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

#if defined(prh_lit_endian)
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
#endif // prh_include_prelude_h

#if defined(prh_file_include)
#ifndef prh_impl_file_prelude_h
#define prh_impl_file_prelude_h
#include <stdarg.h>

typedef prh_reg (*prh_write_func)(void *writer, const prh_byte *data, prh_reg bytes);
prh_export prh_reg prh_write_vargs(void *writer, prh_write_func func, const char *format, ...);
prh_export prh_reg prh_write_vlist(void *writer, prh_write_func func, const char *format, va_list vl);

// debug print warns mains error fatal quiet
// prh_debug_print
// prh_debug_error
// prh_debug_fatal
// prh_print
// prh_mains_print
// prh_warns_print
// prh_error_print
// prh_fatal_print

prh_export void prh_impl_stdio_init(void);
prh_export void prh_impl_stdio_exit(void);

#endif // prh_impl_file_prelude_h
#endif // prh_file_include

#if defined(prh_alloc_include)
#define prh_thread_include
#endif

#if defined(prh_thread_include)
#ifndef prh_impl_thread_prelude_h
#define prh_impl_thread_prelude_h

#ifndef prh_alloc_stack_size
#define prh_alloc_stack_size 16
#endif

typedef struct {
    prh_alignas(prh_cache_line_size) // 初始化后只读
    void *thread_impl;
    void *thread_data;
    void *static_alloc; // 静态分配器，一旦分配永不释放直到线程退出，该分配器一旦初始化不可重新配置
    void *cyclic_alloc; // 周期分配器，分配后不需要释放，所有内存自动会在周期开始时重置
    void *intant_alloc; // 即时分配器，分配后必须在同一函数中释放，该分配器一旦初始化不可重新配置
    void *ehub_service;
    void *coro_service;
    prh_r32 thread_id;
    prh_alignas(prh_cache_line_size) // 仅由当前线程访问和修改
    prh_r32 error_code;
    prh_byte alloc_top_index;
    void *allocators[prh_alloc_stack_size];
    void *active_window;
    void *limited_data;
} prh_impl_thread;

typedef prh_impl_thread prh_runtime_context;
prh_export prh_thread_local prh_impl_thread *prh_self_thread;

prh_inline prh_runtime_context *prh_thread_runtime(void) { return prh_self_thread; }
prh_inline prh_r32 prh_runtime_error(void) { return prh_thread_runtime()->error_code; }

#define prh_set_runtime_error(error_code) prh_impl_set_runtime_error((error_code), __LINE__, prh_caller)
prh_export void prh_impl_set_runtime_error(prh_r32 error_code, int line, prh_caller caller);

#endif // prh_impl_thread_prelude_h
#endif // prh_thread_include

#ifndef prh_impl_main_init_include_h
#define prh_impl_main_init_include_h

typedef struct {
    prh_byte *thread_base;
    prh_r32 thread_bytes;
    prh_r32 total_threads;
#if defined(prh_file_include)
    prh_handle stdinp_handle;
    prh_handle stdout_handle;
    prh_handle stderr_handle;
#endif
} prh_impl_global;

prh_export prh_impl_global prh_global;

#if defined(prh_source_implement)
#ifndef prh_impl_main_init_source_h
#define prh_impl_main_init_source_h

prh_impl_global prh_global;

void prh_main_exit(void)
{
}

void prh_main_init(prh_r32 total_threads)
{
}

#endif // prh_impl_main_init_source_h
#endif // prh_source_implement
#endif // prh_impl_main_init_include_h

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

#undef prh_impl_real_assign_and_not_null_assert
#undef prh_real_assign_and_not_null_assert
#undef prh_assign_and_not_null_assert

#define prh_impl_real_assert(a, line, caller) ((void)((a) || (prh_impl_assert_line_caller((line), (caller)), false)))
#define prh_impl_plus_assert(a, line, caller, ...) ((void)((a) || ((__VA_ARGS__), prh_impl_assert_line_caller((line), (caller)), false)))
#define prh_real_plus_assert(a, ...) prh_impl_plus_assert((a), __LINE__, prh_caller, __VA_ARGS__)

#define prh_impl_real_assign_and_not_null_assert(value, expr, line, caller) (((value) = (expr)) || (prh_impl_assert_line_caller(__LINE__, prh_caller), prh_null))
#define prh_real_assign_and_not_null_assert(value, expr) prh_impl_real_assign_and_not_null_assert((value), (expr), __LINE__, prh_caller)

#if PRH_DEBUG
    #if defined(prh_line_caller_debug)
        #define prh_real_assert(a) prh_impl_real_assert(a, __LINE__, prh_caller)
    #else
        #define prh_real_assert(a) assert(a)
    #endif
    #define prh_plus_assert(a, ...) prh_real_plus_assert((a), __VA_ARGS__)
    #define prh_assign_and_not_null_assert(value, expr) prh_real_assign_and_not_null_assert((value), (expr))
    #define prh_assert(a) prh_real_assert(a)
#else
    #define prh_real_assert(a) prh_impl_real_assert(a, __LINE__, prh_caller)
    #define prh_plus_assert(a, ...)
    #define prh_assign_and_not_null_assert(value, expr) ((value) = (expr))
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

#endif // prh_include_stdc_assert_h
#endif // prh_impl_stdc_assert

#ifndef prh_impl_prelude_extern_h
#define prh_impl_prelude_extern_h

#define prh_arrlen(a) (sizeof(a)/sizeof((a)[0]))
#define prh_arrend(a) ((a) + prh_arrlen(a))
#define prh_arrelt(a, i) (a)[(prh_assert((i) >= 0 && (i) < prh_arrlen(a))), i]
#define prh_arrget(a) (a), prh_arrlen(a)

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
    return (n & (power_of_2 - 1)) == 0;
}

prh_inline bool prh_raw_is_times_power_of_2(prh_raw n, prh_reg power_of_2)
{
    prh_assert(prh_is_power_of_2(power_of_2));
    return (n & (power_of_2 - 1)) == 0;
}

prh_inline prh_byte prh_unchecked_log2_power_of_2(prh_reg alignment)
{
    prh_assert(alignment != 0 && prh_is_power_of_2(alignment));
    return (prh_byte)prh_reg_unchecked_trailing_zeros(alignment);
}

prh_inline prh_reg prh_ceil_power_of_2(prh_reg n)
{
    prh_reg higher_most_bit = 1 << prh_reg_unchecked_higher_most_bit_position(prh_set_value_if_true(1, n, n == 0));
    prh_reg result_power_of_2 = higher_most_bit << ((n & (higher_most_bit - 1)) != 0);
    prh_assert(result_power_of_2 >= higher_most_bit);
    return result_power_of_2;
}

prh_inline prh_reg prh_floor_power_of_2(prh_reg n)
{
    return (prh_reg)1 << prh_reg_unchecked_higher_most_bit_position(prh_set_value_if_true(1, n, n == 0));
}

prh_inline prh_reg prh_times_power_of_2(prh_reg n, prh_reg power_of_2)
{
    prh_assert(prh_is_power_of_2(power_of_2));
    prh_reg result = (n + power_of_2 - 1) & (~(power_of_2 - 1));
    prh_assert(result >= n);
    return result;
}

prh_inline prh_raw prh_raw_times_power_of_2(prh_raw n, prh_reg power_of_2)
{
    prh_assert(prh_is_power_of_2(power_of_2));
    prh_raw result = (n + power_of_2 - 1) & (~(power_of_2 - 1));
    prh_assert(result >= n);
    return result;
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

#endif // prh_impl_prelude_extern_h

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   STDC & PLAT ALLOC
////
////

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
// void *_aligned_malloc(size_t size, size_t alignment);
// void *_aligned_offset_malloc(size_t size, size_t alignment, size_t offset);
//
// void *_aligned_realloc(void *memblock, size_t size, size_t alignment);
// void *_aligned_offset_realloc(void *memblock, size_t size, size_t alignment, size_t offset);
//
// void *_aligned_recalloc(void *memblock, size_t num, size_t size, size_t alignment);
// void *_aligned_offset_recalloc(void *memblock, size_t num, size_t size, size_t alignment, size_t offset);
//
// size_t _aligned_msize(void *memblock, size_t alignment, size_t offset);
// void _aligned_free(void *memblock);
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

// https://cppreference.com/c/header/stdlib
// https://cppreference.com/c/memory/aligned_alloc (C11)
//
// void* malloc(size_t size);
// void* calloc(size_t nmemb, size_t size);
// void* realloc(void* ptr, size_t size);
// void free(void* ptr);
// void free_sized(void* ptr, size_t size);
//
// void* aligned_alloc(size_t alignment, size_t size);
// void free_aligned_sized(void* ptr, size_t alignment, size_t size);

// https://pubs.opengroup.org/onlinepubs/9699919799.2008edition/basedefs/contents.html
// https://pubs.opengroup.org/onlinepubs/9699919799.2008edition/basedefs/stdlib.h.html
// https://pubs.opengroup.org/onlinepubs/9699919799.2008edition/idx/headers.html
//
// int posix_memalign(void **memptr, size_t alignment, size_t size);

#undef prh_impl_plat_aligned_realloc
#undef prh_impl_plat_aligned_offset_realloc
#undef prh_impl_plat_aligned_offset_malloc
#undef prh_impl_plat_aligned_memory_size

#if defined(prh_msc_version)
    // https://learn.microsoft.com/en-us/cpp/c-runtime-library/heap-maxreq
    // _HEAP_MAXREQ - maximum size of a user request for memory that can be granted
    // Maximum heap request the heap manager will attempt
    // #ifdef _WIN64
    // #define _HEAP_MAXREQ 0xFFFFFFFFFFFFFFE0
    // #else
    // #define _HEAP_MAXREQ 0xFFFFFFE0
    // #endif
    #include <malloc.h> // _aligned_malloc _aligned_realloc _aligned_free
    // _aligned_malloc is based on malloc. required C header malloc.h.
    // if alignment isn't a power of 2 or size is zero, this function invokes
    // the invalid parameter handler. if execution is allowed to continue, this
    // function returns NULL and sets errno to EINVAL.
    #define prh_impl_plat_aligned_malloc(size, alignment) _aligned_malloc((size), (alignment))
    #define prh_impl_plat_aligned_realloc(p, size, alignment) _aligned_realloc((p), (size), (alignment))
    #define prh_impl_plat_aligned_offset_realloc(p, size, alignment, offset) _aligned_offset_realloc((p), (size), (alignment), (offset))
    #define prh_impl_plat_aligned_offset_malloc(size, alignment, offset) _aligned_offset_malloc((size), (alignment), (offset))
    #define prh_impl_plat_aligned_memory_size(p, alignment, offset) _aligned_msize((p), (alignment), (offset))
    #define prh_impl_plat_aligned_delete(p) _aligned_free(p) // _aligned_free(p) if p is a null, this function simply performs no actions
#else
    // behavior is undefined if size is not an integral multiple of alignment
    void *aligned_alloc(size_t alignment, size_t size); // glibc 2.16. C11
    #define prh_impl_plat_aligned_malloc(size, alignment) aligned_alloc((alignment), (size))
    #define prh_impl_plat_aligned_delete(p) prh_impl_rawc_delete(p) // free(p) if p is null do nothing
#endif

prh_export void *prh_impl_rawc_aligned_realloc(void *buffer, prh_reg new_capacity, prh_reg alignment); // 标准 C 兼容，buffer 为空时相当于调用 aligned_malloc
prh_export void *peh_impl_rawc_aligned_malter(void *buffer, prh_reg new_capacity, prh_reg alignment); // 规范接口，buffer 不能为空，只做重新分配

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
    return realloc(buffer, new_capacity); // 标准 C 兼容，buffer 为空时相当于调用 aligned_malloc
}

prh_inline void *prh_impl_rawc_malter(void *buffer, prh_reg new_capacity)
{
    prh_assert(buffer != prh_null); // 规范接口， buffer 不能为空，只做重新分配
    return realloc(buffer, new_capacity);
}

prh_inline void prh_impl_rawc_delete(void *buffer)
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

prh_inline void prh_memcpy_old_content_to_new_buffer(prh_byte *new_buffer, prh_reg new_capacity, const prh_byte *old_buffer, prh_reg old_capacity)
{
    prh_assert(old_buffer || old_capacity == 0);
    memcpy(new_buffer, old_buffer, prh_set_value_if_true(old_capacity, new_capacity, old_capacity <= new_capacity)); // new_capacity 可能扩大或缩小
}

prh_inline void prh_memset_expanded_content_to_zero(prh_byte *new_buffer, prh_reg new_capacity, prh_reg old_capacity)
{
    memset(new_buffer + old_capacity, 0, prh_set_value_if_true(new_capacity - old_capacity, 0, new_capacity > old_capacity));
}

prh_inline void prh_memcpy_old_content_memset_expanded_content(prh_byte *new_buffer, prh_reg new_capacity, const prh_byte *old_buffer, prh_reg old_capacity)
{
    prh_assert(old_buffer || old_capacity == 0);
    memcpy(new_buffer, old_buffer, prh_set_value_if_true(old_capacity, new_capacity, old_capacity <= new_capacity));
    memset(new_buffer + old_capacity, 0, prh_set_value_if_true(new_capacity - old_capacity, 0, new_capacity > old_capacity));
}

prh_inline prh_reg prh_alloc_capacity(prh_reg capacity, prh_reg alignment)
{
    // 1. 避免 realloc 当 capacity 为 0 时的不确定行为（may be free(ptr) or depends on library implementation）
    // 2. 避免 malloc 当 capacity 为 0 时的不确定行为（may or may not return null, but the returned pointer shall not be dereferenced）
    // 避免 realloc 遇到 0 释放内存，至少分配 alignment 大小的内存，规范 realloc 总是分配而不会释放内存，释放内存必须调用 delete
    // 使得 malloc 至少分配 alignment 大小的内存，并检查 alignment 的合法性
    prh_assert(alignment >= sizeof(void *));
    return prh_times_align_size(prh_set_value_if_zero(1, capacity), alignment);
}

prh_inline prh_byte *prh_aligned_address(prh_byte *address, prh_reg alignment, prh_reg offset)
{
    prh_byte *buffer = (prh_byte *)prh_raw_times_power_of_2((prh_raw)address, alignment);
    return buffer + prh_set_value_if_true(prh_times_align_size(offset - (buffer - address), alignment), 0, offset > buffer - address);
}

// void *malloc(size_t size);
// the newly allocated block of memory is not initialized, remaining with indeterminate values.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.
//
// 如果 capacity 为零，可能返回 null 也可能返回正常地址，但该地址不能访问
// 对于 aligned_malloc，如果 capacity 大小不是对齐的整数倍或者为零，或者对齐字节数不是2的幂，都将触发非法处理
// 分配零字节长度正常返回或返回空，总之返回的地址位置不能访问，free 函数可以处理空指针不会报错
// 对齐 alignment 必须是 sizeof(void *) 的 2 的幂的倍数（The address of the allocated memory will be
// a multiple of alignment, which must be a power of two and a multiple of sizeof(void *)）

prh_inline void *prh_stdc_malloc_with_trace(prh_reg capacity, int line, prh_raw caller)
{
    void *buffer = prh_impl_rawc_malloc(prh_alloc_capacity(capacity, sizeof(void *)));
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void *prh_stdc_aligned_malloc_with_trace(prh_reg capacity, prh_reg alignment, int line, prh_raw caller)
{
    void *buffer = prh_impl_plat_aligned_malloc(prh_alloc_capacity(capacity, alignment), alignment);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

// void *calloc(size_t num, size_t size);
// allocates a block of memory for an array of num elements, each of them size bytes long, and
// initializes all its bits to zero. the effective result is the allocation of a zero-initialized
// memory block of (num*size) bytes.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.

prh_inline void *prh_stdc_calloc_with_trace(prh_reg capacity, int line, prh_raw caller)
{
    void *buffer = prh_impl_rawc_calloc(capacity);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void *prh_stdc_aligned_calloc_with_trace(prh_reg capacity, prh_reg alignment, int line, prh_raw caller)
{
    void *buffer = prh_stdc_aligned_malloc_with_trace(capacity, alignment, line, caller);
    memset(buffer, 0, capacity);
    return buffer;
}

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
//          **MAYBE** free(ptr) return NULL *** 规范 realloc 只做分配操作，不释放内存
// else
//      // if size = 0 may or may not return null, but the returned pointer shall not be dereferenced
//      return malloc(size)
//
// 规范 realloc 只做分配操作，不释放内存，释放内存必须调用 delete
// 当 buffer 为空时直接分配 new_capacity 大小内存，当 buffer 不为空时，重新分配到 new_capacity 大小

prh_inline void *prh_stdc_realloc_with_trace(void *buffer, prh_reg new_capacity, int line, prh_raw caller)
{
    buffer = prh_impl_rawc_realloc(buffer, prh_alloc_capacity(new_capacity, sizeof(void *)));
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void *prh_stdc_malter_with_trace(void *buffer, prh_reg new_capacity, int line, prh_raw caller)
{
    buffer = prh_impl_rawc_malter(buffer, prh_alloc_capacity(new_capacity, sizeof(void *)));
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void *prh_stdc_aligned_realloc_with_trace(void *buffer, prh_reg new_capacity, prh_reg alignment, int line, prh_raw caller)
{
    void *buffer = prh_impl_rawc_aligned_realloc(buffer, new_capacity, alignment);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void *prh_stdc_aligned_malter_with_trace(void *buffer, prh_reg new_capacity, prh_reg alignment, int line, prh_raw caller)
{
    void *buffer = prh_impl_rawc_aligned_malter(buffer, new_capacity, alignment);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

// void free(void *ptr) if ptr is null do nothing

prh_inline void prh_stdc_delete_with_trace(void *buffer, int line, prh_raw caller)
{
    prh_impl_rawc_delete(buffer);
}

prh_inline void prh_stdc_aligned_delete_with_trace(void *buffer, int line, prh_raw caller)
{
    prh_impl_plat_aligned_delete(buffer); // 如果 buffer 为空，prh_impl_plat_aligned_delete 不做任何事
}

// 由于无法知道 old_buffer 的 old_capacity，无法使用 stdc 标准函数实现 recalloc、aligned_recalloc、
// calter 等函数，要将增长的内存内容清零，必须每次调用 realloc 和 aligned_realloc 或 malter 之后手动
// 清零。或调用扩展的 prh_extend_stdc_recalloc、prh_extend_stdc_calter 等函数，另外如果标准 C 函数
// aligned_realloc 存在兼容性问题，也可以调用扩展的 prh_extend_stdc_realloc 和 prh_extend_stdc_malter
// 等函数。

#define prh_stdc_malloc(capacity) prh_stdc_malloc_with_trace((capacity), __LINE__, prh_caller)
#define prh_stdc_calloc(capacity) prh_stdc_calloc_with_trace((capacity), __LINE__, prh_caller)
#define prh_stdc_realloc(buffer, new_capacity) prh_stdc_realloc_with_trace((buffer), (new_capacity), __LINE__, prh_caller)
#define prh_stdc_malter(buffer, new_capacity) prh_stdc_malter_with_trace((buffer), (new_capacity), __LINE__, prh_caller)
#define prh_stdc_delete(buffer) prh_stdc_delete_with_trace(buffer, __LINE__, prh_caller) // 规范 realloc 只做分配操作，不释放内存，释放内存必须调用 delete

#define prh_stdc_aligned_malloc(capacity, alignment) prh_stdc_aligned_malloc_with_trace((capacity), (alignment), __LINE__, prh_caller)
#define prh_stdc_aligned_calloc(capacity, alignment) prh_stdc_aligned_calloc_with_trace((capacity), (alignment), __LINE__, prh_caller)
#define prh_stdc_aligned_realloc(buffer, new_capacity, alignment) prh_stdc_aligned_realloc_with_trace((buffer), (new_capacity), (alignment), __LINE__, prh_caller)
#define prh_stdc_aligned_malter(buffer, new_capacity, alignment) prh_stdc_aligned_malter_with_trace((buffer), (new_capacity), (alignment), __LINE__, prh_caller)
#define prh_stdc_aligned_delete(buffer) prh_stdc_aligned_delete_with_trace((buffer), __LINE__, prh_caller) // 规范 aligned_realloc 只做分配操作，不释放内存，释放内存必须调用 aligned_delete

#endif // prh_include_stdc_alloc_h

#if defined(prh_source_implement)

void *prh_impl_rawc_aligned_realloc(void *buffer, prh_reg new_capacity, prh_reg alignment)
{
    // 规范 realloc 只做分配操作，不释放内存，释放内存必须调用 delete
    // 当 buffer 为空时直接分配 capacity 大小内存，当 buffer 不为空时，重新分配到 capacity 大小
    new_capacity = prh_alloc_capacity(new_capacity, alignment);
#if defined(prh_impl_plat_aligned_realloc)
    return prh_impl_plat_aligned_realloc(buffer, new_capacity, alignment);
#else
    if (buffer == prh_null)
    {
        return prh_impl_plat_aligned_malloc(new_capacity, alignment);
    }
    else
    {
    #if defined(prh_impl_plat_aligned_memory_size) || defined(prh_impl_thread_local_realloc_old_capacity)
        #if defined(prh_impl_plat_aligned_memory_size)
        prh_reg old_capacity = prh_impl_plat_aligned_memory_size(buffer, alignment, 0);
        #else
        prh_reg old_capacity = prh_impl_thread_local_realloc_old_capacity();
        #endif
        if (new_capacity <= old_capacity)
        {
            // 注意这里什么也没做实际上是忽略了缩减容量的请求，原来分配内存保持原样不变
        }
        else
        {
            void *old_buffer = buffer;
            buffer = prh_impl_plat_aligned_malloc(new_capacity, alignment);
            if (buffer) memcpy(buffer, old_buffer, old_capacity);
            prh_impl_plat_aligned_delete(old_buffer);
        }
    #else
        // 1. buffer 总是新分配，因此 buffer 一定与 old_buffer 地址不同
        // 2. 如果 new_capacity <= old_capacity，拷贝区间不会发生重叠
        // 3. 如果 new_capacity > old_capacity，拷贝区间可能发生重叠
        //      1) [old_buffer, new_buffer) 的长度超过 new_capacity，不会发生重叠
        //          [buffer data | unknown data] => [buffer data | unknown data]
        //          ^ old_buffer ^                  ^ new_buffer ------------- ^
        //      2) [old_buffer, new_buffer) 的长度小于 new_capacity，会重叠拷贝一部分 new_buffer 内容到 new_buffer + old_capacity 之后
        //          [buffer data ] => [buffer data | buffer d...]
        //          ^ old_buffer ^    ^ new_buffer ------------ ^
        //      3) 如果分配的新内存位于 old_buffer 之前，不会发生重叠，但有访问 old_buffer 之后非法内存的风险
        //         规避方法一：不使用 aligned_realloc，只是用 realloc 标准库函数
        //         规避方法二：使用平台提供的 aligned_realloc 并定义为 prh_impl_plat_aligned_realloc
        //         规避方法三：使用 thread_local 变量在调用该函数之前传递 old_capacity 的值，走上面 #if 分支
        //         规避方法四：确定 old_buffer + new_capacity 位于合法内存范围之内之后才调用
        //          [new_buffer ...] <= [old_buffer | unknown memory area]
        void *old_buffer = buffer;
        buffer = prh_impl_plat_aligned_malloc(new_capacity, alignment);
        if (buffer) memmove(buffer, old_buffer, new_capacity);
        prh_impl_plat_aligned_delete(old_buffer);
    #endif
        return buffer;
    }
#endif
}

void *prh_impl_rawc_aligned_malter(void *buffer, prh_reg new_capacity, prh_reg alignment)
{
    // 规范 malter 只做分配操作，不释放内存，释放内存必须调用 delete，而且 buffer 不能为空，该函数只能作为重新分配使用
    prh_assert(buffer != prh_null);
    new_capacity = prh_alloc_capacity(new_capacity, alignment);
#if defined(prh_impl_plat_aligned_realloc)
    return prh_impl_plat_aligned_realloc(buffer, new_capacity, alignment);
#elif defined(prh_impl_plat_aligned_memory_size) || defined(prh_impl_thread_local_realloc_old_capacity)
    #if defined(prh_impl_plat_aligned_memory_size)
    prh_reg old_capacity = prh_impl_plat_aligned_memory_size(buffer, alignment, 0);
    #else
    prh_reg old_capacity = prh_impl_thread_local_realloc_old_capacity();
    #endif
    if (new_capacity <= old_capacity)
    {
        // 注意这里什么也没做实际上是忽略了缩减容量的请求，原来分配内存保持原样不变
    }
    else
    {
        void *old_buffer = buffer;
        buffer = prh_impl_plat_aligned_malloc(new_capacity, alignment);
        if (buffer) memcpy(buffer, old_buffer, old_capacity);
        prh_impl_plat_aligned_delete(old_buffer);
    }
    return buffer;
#else
    // 1. buffer 总是新分配，因此 buffer 一定与 old_buffer 地址不同
    // 2. 如果 new_capacity <= old_capacity，拷贝区间不会发生重叠
    // 3. 如果 new_capacity > old_capacity，拷贝区间可能发生重叠
    //      1) [old_buffer, new_buffer) 的长度超过 new_capacity，不会发生重叠
    //          [buffer data | unknown data] => [buffer data | unknown data]
    //          ^ old_buffer ^                  ^ new_buffer ------------- ^
    //      2) [old_buffer, new_buffer) 的长度小于 new_capacity，会重叠拷贝一部分 new_buffer 内容到 new_buffer + old_capacity 之后
    //          [buffer data ] => [buffer data | buffer d...]
    //          ^ old_buffer ^    ^ new_buffer ------------ ^
    //      3) 如果分配的新内存位于 old_buffer 之前，不会发生重叠，但有访问 old_buffer 之后非法内存的风险
    //         规避方法一：不使用 aligned_realloc，只是用 realloc 标准库函数
    //         规避方法二：使用平台提供的 aligned_realloc 并定义为 prh_impl_plat_aligned_realloc
    //         规避方法三：使用 thread_local 变量在调用该函数之前传递 old_capacity 的值，走上面 #if 分支
    //         规避方法四：确定 old_buffer + new_capacity 位于合法内存范围之内之后才调用
    //          [new_buffer ...] <= [old_buffer | unknown memory area]
    void *old_buffer = buffer;
    buffer = prh_impl_plat_aligned_malloc(new_capacity, alignment);
    if (buffer) memmove(buffer, old_buffer, new_capacity);
    prh_impl_plat_aligned_delete(old_buffer);
    return buffer;
#endif
}

#endif // prh_source_implement
#endif // prh_impl_stdc_alloc

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   ALLOC INTERFACE
////
////

#if defined(prh_char_include)
#define prh_alloc_include
#endif

#if defined(prh_array_include)
#define prh_alloc_include
#endif

#if defined(prh_thread_include)
#define prh_alloc_include
#endif

#if defined(prh_alloc_include)
#ifndef prh_impl_alloc_include_h
#define prh_impl_alloc_include_h

#undef prh_impl_64_bit_alloc
#undef prh_impl_32_bit_alloc
#undef prh_impl_16_bit_alloc

#if prh_int_bits == 64
    #define prh_impl_64_bit_alloc
#elif prh_int_bits == 32
    #if defined(prh_using_low_cost_alloc)
        #define prh_impl_16_bit_alloc
    #else
        #define prh_impl_32_bit_alloc
    #endif
#endif

#ifndef prh_single_allocator_program
#define prh_single_allocator_program 0
#endif

typedef enum {
    prh_alloc_align_int_size = prh_impl_align_int_size,
    prh_alloc_align_imt_size = prh_impl_align_imt_size,
    prh_alloc_align_line_size = prh_cache_line_size,
    prh_alloc_align_page_size = prh_memory_page_size,
    prh_alloc_align_vmem_unit = prh_vmem_unit_size,
    prh_alloc_align_4_byte = prh_impl_align_4_byte,
    prh_alloc_align_8_byte = prh_impl_align_8_byte,
    prh_alloc_align_16_byte = prh_impl_align_16_byte,
    prh_alloc_align_32_byte = prh_impl_align_32_byte,
    prh_alloc_align_64_byte = prh_impl_align_64_byte,
    prh_alloc_align_128_byte = prh_impl_align_128_byte,
    prh_alloc_align_256_byte = prh_impl_align_256_byte,
    prh_alloc_align_512_byte = prh_impl_align_512_byte,
    prh_alloc_align_1k_byte = prh_impl_align_1k_byte,
    prh_alloc_align_2k_byte = prh_impl_align_2k_byte,
    prh_alloc_align_4k_byte = prh_impl_align_4k_byte,
    prh_alloc_align_8k_byte = prh_impl_align_8k_byte,
    prh_alloc_align_16k_byte = prh_impl_align_16k_byte,
    prh_alloc_align_32k_byte = prh_impl_align_32k_byte,
    prh_alloc_align_64k_byte = prh_impl_align_64k_byte,
    prh_alloc_align_128k_byte = prh_impl_align_128k_byte,
    prh_alloc_align_max_byte = prh_alloc_align_128k_byte,
} prh_alloc_align_enum;

typedef struct prh_alloc_face prh_alloc_face; // 至少分配 alignment 大小的内存
typedef void (*prh_alloc_func)(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset);
typedef void (*prh_alloc_free)(prh_alloc_face *alloc, prh_memory *buffer, prh_reg offset);

typedef struct prh_alloc_face {
    prh_alloc_func alloc_func; // malloc calloc
    prh_alloc_free alloc_free; // delete
} prh_alloc_face;

prh_export prh_alloc_face *prh_default_alloc(void); // 全局默认分配器，可在初始化时重新配置，一旦初始化完成不可再更改
prh_export void prh_empty_alloc_free(prh_alloc_face *alloc, prh_memory *buffer, prh_reg offset);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// 64 BIT ALLOC DEFINES
///
///

#if defined(prh_impl_64_bit_alloc)

#undef prh_minimal_alloc_alignment
#define prh_minimal_alloc_alignment prh_alloc_align_8_byte

#ifndef prh_default_alloc_alignment
#define prh_default_alloc_alignment prh_alloc_align_16_byte
#endif

prh_static_assert(prh_default_alloc_alignment >= prh_minimal_alloc_alignment);
prh_static_assert(prh_default_alloc_alignment <= prh_alloc_align_max_byte);
prh_static_assert(sizeof(prh_reg) == sizeof(prh_r64));

// 0 1 2 3  4  5  6   7   8   9  10  11  12  13   14   15   16    17    18    19
// 1 2 4 8 16 32 64 128 256 512 1KB 2KB 4KB 8KB 16KB 32KB 64KB 128KB 256KB 512KB
//       0  1  2  3   4   5   6   7   8   9  10   11   12   13    14    15

typedef struct {
    prh_reg impl_capacity_60: 60, impl_alignment: 4;
    prh_reg buffer_address;
} prh_memory;

typedef struct {
    prh_reg impl_capacity_60: 60, impl_alignment: 4;
} prh_inplace_memory;

prh_inline prh_byte *prh_memory_buffer(prh_memory *self)
{
    return (prh_byte *)(prh_raw)self->buffer_address;
}

prh_inline void prh_memory_set_buffer(prh_memory *self, prh_byte *buffer)
{
    self->buffer_address = (prh_reg)(prh_raw)buffer; // buffer 可能因为 offset_malloc 而不对齐
}

prh_inline prh_reg prh_memory_capacity(const prh_memory *self)
{
    return self->impl_capacity_60;
}

prh_inline void prh_memory_set_capacity(prh_memory *self, prh_reg capacity)
{
    prh_assert(prh_is_times_power_of_2(capacity, 1 << prh_minimal_alloc_alignment));
    prh_assert(capacity <= 0x0FFFFFFFFFFFFFFFULL);
    self->impl_capacity_60 = capacity;
}

prh_inline prh_alloc_align_enum prh_memory_alignment(const prh_memory *self)
{
    return (prh_alloc_align_enum)((prh_byte)prh_minimal_alloc_alignment + (prh_byte)self->impl_alignment); // alignment = log2(real_alignment)
}

prh_inline prh_reg prh_memory_real_alignment(const prh_memory *self)
{
    return (prh_reg)1 << prh_memory_alignment(self);
}

prh_inline void prh_memory_set_alignment(prh_memory *self, prh_alloc_align_enum alignment)
{
    prh_assert(alignment <= prh_alloc_align_max_byte);
    self->impl_alignment = prh_r08_set_value_if_true(alignment - prh_minimal_alloc_alignment, 0, alignment > prh_minimal_alloc_alignment);
}

prh_inline void prh_memory_set_real_alignment(prh_memory *self, prh_reg real_alignment)
{
    prh_memory_set_alignment(self, (prh_alloc_align_enum)prh_unchecked_log2_power_of_2(real_alignment));
}

prh_inline prh_memory prh_empty_memory(void)
{
    prh_memory memory = {0, prh_default_alloc_alignment - prh_minimal_alloc_alignment};
    return memory;
}

prh_inline prh_memory prh_aligned_memory(prh_alloc_align_enum alignment)
{
    prh_assert(alignment <= prh_alloc_align_max_byte);
    prh_memory memory = {0, prh_r08_set_value_if_true(alignment - prh_minimal_alloc_alignment, 0, alignment > prh_minimal_alloc_alignment)};
    return memory;
}

prh_inline prh_memory prh_real_aligned_memory(prh_reg real_alignment)
{
    return prh_aligned_memory((prh_alloc_align_enum)prh_unchecked_log2_power_of_2(real_alignment));
}

typedef struct {
    prh_memory memory;
#if prh_single_allocator_program == 0
    prh_alloc_face *impl_alloc_face;
#endif
} prh_buffer;

typedef struct {
#if prh_single_allocator_program == 0
    prh_alloc_face *impl_alloc_face;
#endif
    prh_inplace_memory memory;
} prh_inplace_buffer;

prh_inline prh_alloc_face *prh_buffer_alloc(const prh_buffer *self)
{
#if prh_single_allocator_program
    return prh_default_alloc();
#else
    prh_assert(self->impl_alloc_face != prh_null);
    return self->impl_alloc_face;
#endif
}

prh_inline void prh_buffer_set_alloc(prh_buffer *self, prh_alloc_face *alloc)
{
#if prh_single_allocator_program
#else
    prh_assert(alloc != prh_null);
    self->impl_alloc_face = alloc;
#endif
}

prh_inline prh_alloc_face *prh_inplace_buffer_alloc(const prh_inplace_buffer *self)
{
#if prh_single_allocator_program
    return prh_default_alloc();
#else
    prh_assert(self->impl_alloc_face != prh_null);
    return self->impl_alloc_face;
#endif
}

prh_inline void prh_inplace_buffer_set_alloc(prh_inplace_buffer *self, prh_alloc_face *alloc)
{
#if prh_single_allocator_program
#else
    prh_assert(alloc != prh_null);
    self->impl_alloc_face = alloc;
#endif
}

prh_inline prh_buffer prh_empty_buffer(prh_alloc_face *alloc)
{
#if prh_single_allocator_program
    prh_buffer buffer = {{0, prh_default_alloc_alignment - prh_minimal_alloc_alignment}};
#else
    prh_assert(alloc != prh_null);
    prh_buffer buffer = {{0, prh_default_alloc_alignment - prh_minimal_alloc_alignment}, alloc};
#endif
    return buffer;
}

prh_inline prh_buffer prh_aligned_buffer(prh_alloc_face *alloc, prh_alloc_align_enum alignment)
{
    prh_assert(alignment <= prh_alloc_align_max_byte);
#if prh_single_allocator_program
    prh_buffer buffer = {{0, prh_r08_set_value_if_true(alignment - prh_minimal_alloc_alignment, 0, alignment > prh_minimal_alloc_alignment)}};
#else
    prh_assert(alloc != prh_null);
    prh_buffer buffer = {{0, prh_r08_set_value_if_true(alignment - prh_minimal_alloc_alignment, 0, alignment > prh_minimal_alloc_alignment)}, alloc};
#endif
    return buffer;
}

prh_inline prh_buffer prh_real_aligned_buffer(prh_alloc_face *alloc, prh_reg real_alignment)
{
    return prh_aligned_buffer(alloc, (prh_alloc_align_enum)prh_unchecked_log2_power_of_2(real_alignment));
}

#endif // prh_impl_64_bit_alloc

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// 32 BIT & 64_32 BIT ALLOC DEFINES
///
///

#if defined(prh_impl_32_bit_alloc)

#undef prh_minimal_alloc_alignment
#define prh_minimal_alloc_alignment prh_alloc_align_4_byte

#ifndef prh_default_alloc_alignment
#define prh_default_alloc_alignment prh_alloc_align_8_byte
#endif

prh_static_assert(prh_default_alloc_alignment >= prh_minimal_alloc_alignment);
prh_static_assert(prh_default_alloc_alignment <= prh_alloc_align_max_byte);
prh_static_assert(sizeof(prh_reg) == sizeof(prh_r32));

// 0 1 2 3  4  5  6   7   8   9  10  11  12  13   14   15   16    17    18    19
// 1 2 4 8 16 32 64 128 256 512 1KB 2KB 4KB 8KB 16KB 32KB 64KB 128KB 256KB 512KB
//     0 1  2  3  4   5   6   7   8   9  10  11   12   13   14    15
//
// real_capacity = capacity (28-bit) * real_alignment
//  1.  对齐到4字节一次最多只能分配 1073741820 字节，比 1GB 少 4 字节，使用更大的对齐分配更大的内存，比起 GB 大小的分配浪费几个字节微不足道
//  2.  对齐到8字节一次最多只能分配 2147483640 字节，比 2GB 少 8 字节，使用更大的对齐分配更大的内存，比起 GB 大小的分配浪费几个字节微不足道
//  3.  对齐到16字节一次最多能分配  4294967280 字节，比 4GB 少 16 字节

#define prh_impl_32_bit_alloc_max_size_4 1073741820
#define prh_impl_32_bit_alloc_max_size_8 2147483640

typedef struct {
    prh_r32 impl_capacity_alignment_4;
    prh_r32 buffer_address;
} prh_memory;

typedef struct {
    prh_r32 impl_capacity_alignment_4;
} prh_inplace_memory;

prh_inline prh_byte *prh_memory_buffer(prh_memory *self)
{
    return (prh_byte *)(prh_raw)self->buffer_address;
}

prh_inline void prh_memory_set_buffer(prh_memory *self, prh_byte *buffer)
{
    self->buffer_address = (prh_r32)(prh_raw)buffer; // buffer 可能因为 offset_malloc 而不对齐
}

prh_inline prh_byte prh_memory_alignment(const prh_memory *self)
{
    return (prh_alloc_align_enum)((prh_byte)prh_minimal_alloc_alignment + (prh_byte)(self->impl_capacity_alignment_4 & 0x0F);
}

prh_inline prh_reg prh_memory_real_alignment(const prh_memory *self)
{
    return (prh_reg)1 << prh_memory_alignment(self);
}

prh_inline void prh_memory_set_alignment(prh_memory *self, prh_alloc_align_enum alignment)
{
    prh_assert(alignment <= prh_alloc_align_max_byte);
    self->impl_capacity_alignment_4 = (self->impl_capacity_alignment_4 & 0xFFFFFFF0) | (prh_r08_set_value_if_true(alignment - prh_minimal_alloc_alignment, 0, alignment > prh_minimal_alloc_alignment) & 0x0F);
}

prh_inline void prh_memory_set_real_alignment(prh_memory *self, prh_reg real_alignment)
{
    prh_memory_set_alignment(self, (prh_alloc_align_enum)prh_unchecked_log2_power_of_2(real_alignment));
}

prh_inline prh_reg prh_memory_capacity(prh_memory *self)
{
    return (self->impl_capacity_alignment_4 >> 4) << prh_memory_alignment(self);
}

prh_inline void prh_memory_set_capacity(prh_memory *self, prh_reg capacity)
{
#if PRH_DEBUG
    prh_assert(prh_is_times_power_of_2(capacity, 1 << prh_minimal_alloc_alignment));
    prh_reg alignment = prh_memory_real_alignment(self);
    if (alignment == 8) prh_assert(capacity <= prh_impl_32_bit_alloc_max_size_8);
    else if (alignment == 4) prh_assert(capacity <= prh_impl_32_bit_alloc_max_size_4);
#endif
    self->impl_capacity_alignment_4 = ((capacity >> prh_memory_alignment(self)) << 4) | (self->impl_capacity_alignment_4 & 0x0F);
}

prh_inline prh_memory prh_empty_memory(void)
{
    prh_memory memory = {prh_default_alloc_alignment - prh_minimal_alloc_alignment};
    return memory;
}

prh_inline prh_memory prh_aligned_memory(prh_alloc_align_enum alignment)
{
    prh_assert(alignment >= prh_minimal_alloc_alignment && alignment <= prh_alloc_align_max_byte);
    prh_memory memory = {alignment - prh_minimal_alloc_alignment};
    return memory;
}

prh_inline prh_memory prh_real_aligned_memory(prh_reg real_alignment)
{
    return prh_aligned_memory((prh_alloc_align_enum)prh_unchecked_log2_power_of_2(real_alignment));
}

typedef struct {
    prh_memory memory;
#if prh_single_allocator_program == 0
    prh_alloc_face *impl_alloc_face;
#endif
} prh_buffer;

typedef struct {
#if prh_single_allocator_program == 0
    prh_alloc_face *impl_alloc_face;
#endif
    prh_inplace_memory memory;
} prh_inplace_buffer;

prh_inline prh_alloc_face *prh_buffer_alloc(const prh_buffer *self)
{
#if prh_single_allocator_program
    return prh_default_alloc();
#else
    prh_assert(self->impl_alloc_face != prh_null);
    return self->impl_alloc_face;
#endif
}

prh_inline void prh_buffer_set_alloc(prh_buffer *self, prh_alloc_face *alloc)
{
#if prh_single_allocator_program
#else
    prh_assert(alloc != prh_null);
    self->impl_alloc_face = alloc;
#endif
}

prh_inline prh_alloc_face *prh_inplace_buffer_alloc(const prh_inplace_buffer *self)
{
#if prh_single_allocator_program
    return prh_default_alloc();
#else
    prh_assert(self->impl_alloc_face != prh_null);
    return self->impl_alloc_face;
#endif
}

prh_inline void prh_inplace_buffer_set_alloc(prh_inplace_buffer *self, prh_alloc_face *alloc)
{
#if prh_single_allocator_program
#else
    prh_assert(alloc != prh_null);
    self->impl_alloc_face = alloc;
#endif
}

prh_inline prh_buffer prh_empty_buffer(prh_alloc_face *alloc)
{
#if prh_single_allocator_program
    prh_buffer buffer = {{prh_default_alloc_alignment - prh_minimal_alloc_alignment}};
#else
    prh_assert(alloc != prh_null);
    prh_buffer buffer = {{prh_default_alloc_alignment - prh_minimal_alloc_alignment}, alloc};
#endif
    return buffer;
}

prh_inline prh_buffer prh_aligned_buffer(prh_alloc_face *alloc, prh_alloc_align_enum alignment)
{
    prh_assert(alignment <= prh_alloc_align_max_byte);
#if prh_single_allocator_program
    prh_buffer buffer = {{prh_r08_set_value_if_true(alignment - prh_minimal_alloc_alignment, 0, alignment > prh_minimal_alloc_alignment)}};
#else
    prh_assert(alloc != prh_null);
    prh_buffer buffer = {{prh_r08_set_value_if_true(alignment - prh_minimal_alloc_alignment, 0, alignment > prh_minimal_alloc_alignment)}, alloc};
#endif
    return buffer;
}

prh_inline prh_buffer prh_real_aligned_buffer(prh_alloc_face *alloc, prh_reg real_alignment)
{
    return prh_aligned_buffer(alloc, (prh_alloc_align_enum)prh_unchecked_log2_power_of_2(real_alignment));
}

#endif // prh_impl_32_bit_alloc

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// 32 BIT LOW COST ALLOC DEFINES
///
///

#if defined(prh_impl_16_bit_alloc)

#if prh_single_allocator_program == 0
#error "must be single allocator program"
#endif

#undef prh_minimal_alloc_alignment
#define prh_minimal_alloc_alignment prh_alloc_align_4_byte

#ifndef prh_default_alloc_alignment
#define prh_default_alloc_alignment prh_alloc_align_8_byte
#endif

prh_static_assert(prh_default_alloc_alignment >= prh_minimal_alloc_alignment);
prh_static_assert(prh_default_alloc_alignment <= prh_alloc_align_max_byte);
prh_static_assert(sizeof(prh_reg) == sizeof(prh_r32));

// 0 1 2 3  4  5  6   7   8   9  10  11  12  13   14   15   16    17    18    19
// 1 2 4 8 16 32 64 128 256 512 1KB 2KB 4KB 8KB 16KB 32KB 64KB 128KB 256KB 512KB
//     0 1  2  3  4   5   6   7   8   9  10  11   12   13   14    15

typedef struct {
    prh_r32 impl_capacity: 16, impl_extra: 16;
    prh_r32 buffer_address;
} prh_memory;

typedef struct {
    prh_r32 impl_capacity: 16, impl_extra: 16;
} prh_inplace_memory;

prh_inline prh_byte *prh_memory_buffer(prh_memory *self)
{
    return (prh_byte *)(prh_raw)self->buffer_address;
}

prh_inline void prh_memory_set_buffer(prh_memory *self, prh_byte *buffer)
{
    self->buffer_address = (prh_r32)(prh_raw)buffer; // buffer 可能因为 offset_malloc 而不对齐
}

prh_inline prh_reg prh_memory_capacity(const prh_memory *self)
{
    return self->impl_capacity;
}

prh_inline void prh_memory_set_capacity(prh_memory *self, prh_reg capacity)
{
    prh_assert(prh_is_times_power_of_2(capacity, 1 << prh_default_alloc_alignment));
    prh_assert(capacity <= 0xFFFF);
    self->impl_capacity = (prh_r16)capacity;
}

prh_inline prh_alloc_align_enum prh_memory_alignment(const prh_memory *self)
{
    return (prh_byte)prh_default_alloc_alignment;
}

prh_inline prh_reg prh_memory_real_alignment(const prh_memory *self)
{
    return (prh_reg)1 << prh_memory_alignment(self);
}

prh_inline void prh_memory_set_alignment(prh_memory *self, prh_alloc_align_enum alignment)
{
    prh_unused(self);
    prh_unused(alignment);
}

prh_inline void prh_memory_set_real_alignment(prh_memory *self, prh_reg real_alignment)
{
    prh_unused(self);
    prh_unused(real_alignment);
}

prh_inline prh_memory prh_empty_memory(void)
{
    prh_memory memory = {0};
    return memory;
}

prh_inline prh_memory prh_aligned_memory(prh_alloc_align_enum alignment)
{
    prh_memory memory = {0};
    return memory;
}

prh_inline prh_memory prh_real_aligned_memory(prh_reg real_alignment)
{
    prh_memory memory = {0};
    return memory;
}

typedef struct {
    prh_memory memory;
} prh_buffer;

typedef struct {
    prh_inplace_memory memory;
} prh_inplace_buffer;

prh_inline prh_alloc_face *prh_buffer_alloc(const prh_buffer *self)
{
    return prh_default_alloc();
}

prh_inline void prh_buffer_set_alloc(prh_buffer *self, prh_alloc_face *alloc)
{
    prh_unused(self);
    prh_unused(alloc);
}

prh_inline prh_alloc_face *prh_inplace_buffer_alloc(const prh_inplace_buffer *self)
{
    return prh_default_alloc();
}

prh_inline void prh_inplace_buffer_set_alloc(prh_inplace_buffer *self, prh_alloc_face *alloc)
{
    prh_unused(self);
    prh_unused(alloc);
}

prh_inline prh_buffer prh_empty_buffer(prh_alloc_face *alloc)
{
    prh_buffer buffer = {0};
    return buffer;
}

prh_inline prh_buffer prh_aligned_buffer(prh_alloc_face *alloc, prh_alloc_align_enum alignment)
{
    prh_buffer buffer = {0};
    return buffer;
}

prh_inline prh_buffer prh_real_aligned_buffer(prh_alloc_face *alloc, prh_reg real_alignment)
{
    prh_buffer buffer = {0};
    return buffer;
}

#endif // prh_impl_16_bit_alloc

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// COMMON ALLOC BUFFER DEFINES
///
///

#undef prh_default_alloc_real_alignment
#define prh_default_alloc_real_alignment (1 << prh_default_alloc_alignment)

typedef struct {
    prh_reg offset_alignment;
} prh_type_alignment;

prh_inline prh_type_alignment prh_default_alignment(prh_reg offset)
{
    return struct_object(prh_type_alignment, (offset << 4) | (prh_default_alloc_alignment - prh_minimal_alloc_alignment));
}

prh_inline prh_type_alignment prh_special_alignment(prh_reg offset, prh_alloc_align_enum alignment)
{
    prh_assert(alignment >= prh_minimal_alloc_alignment && alignment <= prh_alloc_align_max_byte);
    return struct_object(prh_type_alignment, (offset << 4) | ((alignment - prh_minimal_alloc_alignment) & 0x0F));
}

prh_inline prh_type_alignment prh_special_real_alignment(prh_reg offset, prh_reg real_alignment)
{
    return prh_special_alignment(offset, (prh_alloc_align_enum)prh_unchecked_log2_power_of_2(real_alignment));
}

prh_inline prh_alloc_align_enum prh_alloc_alignment(prh_type_alignment a)
{
    return (prh_alloc_align_enum)((prh_byte)prh_minimal_alloc_alignment + (prh_byte)(a.offset_alignment & 0x0F));
}

prh_inline prh_reg prh_alloc_real_alignment(prh_type_alignment a)
{
    return 1 << prh_alloc_alignment(a);
}

prh_inline prh_reg prh_alloc_offset(prh_type_alignment a)
{
    return (a.offset_alignment >> 4);
}

prh_inline prh_inplace_memory *prh_inplace_memory_from_buffer(prh_byte *buffer)
{
    prh_assert(buffer != prh_null);
    return ((prh_inplace_memory *)buffer) - 1);
}

prh_inline prh_reg prh_inplace_memory_capacity(prh_byte *buffer)
{
    return prh_memory_capacity((prh_memory *)prh_inplace_memory_from_buffer(buffer));
}

prh_inline void prh_inplace_memory_set_capacity(prh_byte *buffer, prh_reg capacity)
{
    prh_memory_set_capacity((prh_memory *)prh_inplace_memory_from_buffer(buffer), capacity);
}

prh_inline prh_alloc_align_enum prh_inplace_memory_alignment(prh_byte *buffer)
{
    return prh_memory_alignment((prh_memory *)prh_inplace_memory_from_buffer(buffer));
}

prh_inline prh_reg prh_inplace_memory_real_alignment(prh_byte *buffer)
{
    return prh_memory_real_alignment((prh_memory *)prh_inplace_memory_from_buffer(buffer));
}

prh_inline void prh_inplace_memory_set_alignment(prh_byte *buffer, prh_alloc_align_enum alignment)
{
    prh_memory_set_alignment((prh_memory *)prh_inplace_memory_from_buffer(buffer), alignment);
}

prh_inline void prh_inplace_memory_set_real_alignment(prh_byte *buffer, prh_reg real_alignment)
{
    prh_memory_set_real_alignment((prh_memory *)prh_inplace_memory_from_buffer(buffer), real_alignment);
}

prh_inline void prh_inplace_memory_init_from(prh_memory *buffer)
{
    prh_assert(buffer != prh_null && buffer->buffer_address != 0);
    *prh_inplace_memory_from_buffer(prh_memory_buffer(buffer)) = *(prh_inplace_memory *)buffer;
}

prh_inline void prh_inplace_buffer_init_from(prh_alloc_face *alloc, prh_memory *buffer)
{
    prh_assert(alloc != prh_null && buffer != prh_null && buffer->buffer_address != 0);
    *prh_inplace_memory_from_buffer(prh_memory_buffer(buffer)) = *(prh_inplace_memory *)buffer;
    prh_inplace_buffer_set_alloc(((prh_inplace_buffer *)prh_memory_buffer(buffer)) - 1, alloc);
}

prh_inline prh_memory prh_memory_from(prh_byte *buffer)
{
    prh_memory memory;
    *(prh_inplace_memory *)&memory = prh_inplace_memory_from_buffer(buffer);
    prh_memory_set_buffer(&memory, buffer);
    return memory;
}

prh_inline prh_memory *prh_buffer_memory(prh_buffer *buffer)
{
    return &buffer->memory;
}

prh_inline prh_byte *prh_buffer_data(prh_buffer *self)
{
    return prh_memory_bufffer(prh_buffer_memory(self));
}

prh_inline void prh_buffer_set_data(prh_buffer *self, prh_byte *data)
{
    prh_memory_set_buffer(prh_buffer_memory(self), data);
}

prh_inline prh_reg prh_buffer_capacity(const prh_buffer *self)
{
    return prh_memory_capacity(prh_buffer_memory(self));
}

prh_inline void prh_buffer_set_capacity(prh_buffer *self, prh_reg capacity)
{
    prh_memory_set_capacity(prh_buffer_memory(self), capacity);
}

prh_inline prh_alloc_align_enum prh_buffer_alignment(const prh_buffer *self)
{
    return prh_memory_alignment(prh_buffer_memory(self));
}

prh_inline prh_reg prh_buffer_real_alignment(const prh_buffer *self)
{
    return prh_memory_real_alignment(prh_buffer_memory(self));
}

prh_inline void prh_buffer_set_alignment(prh_buffer *self, prh_alloc_align_enum alignment)
{
    return prh_memory_set_alignment(prh_buffer_memory(self), alignment);
}

prh_inline void prh_buffer_set_real_alignment(prh_buffer *self, prh_reg real_alignment)
{
    return prh_memory_set_real_alignment(prh_buffer_memory(self), real_alignment);
}

prh_inline prh_reg prh_inplace_buffer_capacity(prh_byte *buffer)
{
    return prh_inplace_memory_capacity(buffer);
}

prh_inline void prh_inplace_buffer_set_capacity(prh_byte *buffer, prh_reg capacity)
{
    prh_inplace_memory_set_capacity(buffer, capacity);
}

prh_inline prh_alloc_align_enum prh_inplace_buffer_alignment(prh_byte *buffer)
{
    return prh_inplace_memory_alignment(buffer);
}

prh_inline prh_reg prh_inplace_buffer_real_alignment(prh_byte *buffer)
{
    return prh_inplace_memory_real_alignment(buffer);
}

prh_inline void prh_inplace_buffer_set_alignment(prh_byte *buffer, prh_alloc_align_enum alignment)
{
    prh_inplace_memory_set_alignment(buffer, alignment);
}

prh_inline void prh_inplace_buffer_set_real_alignment(prh_byte *buffer, prh_reg real_alignment)
{
    prh_inplace_memory_set_real_alignment(buffer, real_alignment);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// GENERAL ALLOC INTERFACE
///
///

prh_export void prh_impl_malloc_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset);
prh_export void prh_impl_calloc_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset);
prh_export void prh_impl_malter_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset); // buffer->buffer_address 必须不为空，只做重新分配单一任务
prh_export void prh_impl_calter_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset); // buffer 必须是通过 alloc 分配的
prh_export void prh_impl_delete_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg offset); // buffer 必须是通过 alloc 分配的

prh_export void prh_impl_malloc_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset);
prh_export void prh_impl_calloc_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset);
prh_export void prh_impl_malter_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset); // buffer->buffer_address 必须不为空，只做重新分配单一任务
prh_export void prh_impl_calter_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset); // buffer 必须使用自带的分配器进行重新分配
prh_export void prh_impl_delete_buffer(prh_buffer *buffer, prh_reg offset); // buffer 必须使用自带的分配器进行释放

#define prh_malloc_memory(alloc, buffer, capacity) prh_malloc_memory_with_trace((alloc), (buffer), (capacity), 0, __LINE__, prh_caller) // 需要自行保存 prh_memory 一般两个指针大小
#define prh_calloc_memory(alloc, buffer, capacity) prh_calloc_memory_with_trace((alloc), (buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_malter_memory(alloc, buffer, capacity) prh_malter_memory_with_trace((alloc), (buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_calter_memory(alloc, buffer, capacity) prh_calter_memory_with_trace((alloc), (buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_delete_memory(alloc, buffer) prh_delete_memory_with_trace((alloc), (buffer), 0, __LINE__, prh_caller)

#define prh_realloc_memory(alloc, buffer, capacity) prh_realloc_memory_with_trace((alloc), (buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_recalloc_memory(alloc, buffer, capacity) prh_recalloc_memory_with_trace((alloc), (buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_realloc_buffer(buffer, capacity) prh_realloc_buffer_with_trace((buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_recalloc_buffer(buffer, capacity) prh_recalloc_buffer_with_trace((buffer), (capacity), 0, __LINE__, prh_caller)

#define prh_malloc_buffer(buffer, capacity) prh_malloc_buffer_with_trace((buffer), (capacity), 0, __LINE__, prh_caller) // 需要自行保存 prh_buffer 一般三个指针大小（保存了分配器）
#define prh_calloc_buffer(buffer, capacity) prh_calloc_buffer_with_trace((buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_malter_buffer(buffer, capacity) prh_malter_buffer_with_trace((buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_calter_buffer(buffer, capacity) prh_calter_buffer_with_trace((buffer), (capacity), 0, __LINE__, prh_caller)
#define prh_delete_buffer(buffer) prh_delete_buffer_with_trace((buffer), 0, __LINE__, prh_caller)

#define prh_malloc_offset_memory(alloc, buffer, capacity, offset) prh_malloc_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller) // 需要自行保存 prh_memory 一般两个指针大小
#define prh_calloc_offset_memory(alloc, buffer, capacity, offset) prh_calloc_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_malter_offset_memory(alloc, buffer, capacity, offset) prh_malter_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_calter_offset_memory(alloc, buffer, capacity, offset) prh_calter_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_delete_offset_memory(alloc, buffer, offset) prh_delete_memory_with_trace((alloc), (buffer), (offset), __LINE__, prh_caller)

#define prh_realloc_offset_memory(alloc, buffer, capacity, offset) prh_realloc_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_recalloc_offset_memory(alloc, buffer, capacity, offset) prh_recalloc_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_realloc_offset_buffer(buffer, capacity, offset) prh_realloc_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_recalloc_offset_buffer(buffer, capacity, offset) prh_recalloc_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller)

#define prh_malloc_offset_buffer(buffer, capacity, offset) prh_malloc_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller) // 需要自行保存 prh_buffer 一般三个指针大小（保存了分配器）
#define prh_calloc_offset_buffer(buffer, capacity, offset) prh_calloc_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_malter_offset_buffer(buffer, capacity, offset) prh_malter_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_calter_offset_buffer(buffer, capacity, offset) prh_calter_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_delete_offset_buffer(buffer, offset) prh_delete_buffer_with_trace((buffer), (offset), __LINE__, prh_caller)

prh_export prh_byte *prh_impl_malloc_inplace_memory(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt);
prh_export prh_byte *prh_impl_calloc_inplace_memory(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt);
prh_export prh_byte *prh_impl_malter_inplace_memory(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_reg offset); // buffer 必须不为空，只做重新分配单一任务
prh_export prh_byte *prh_impl_calter_inplace_memory(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_reg offset); // buffer 必须是通过 alloc 分配的
prh_export void prh_impl_delete_inplace_memory(prh_alloc_face *alloc, prh_byte *buffer, prh_reg offset); // buffer 必须是通过 alloc 分配的

prh_export prh_byte *prh_impl_malloc_inplace_buffer(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt);
prh_export prh_byte *prh_impl_calloc_inplace_buffer(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt);
prh_export prh_byte *prh_impl_malter_inplace_buffer(prh_byte *buffer, prh_reg capacity, prh_reg offset); // buffer 必须使用自带的分配器进行重新分配
prh_export prh_byte *prh_impl_calter_inplace_buffer(prh_byte *buffer, prh_reg capacity, prh_reg offset); // buffer 必须使用自带的分配器进行重新分配
prh_export void prh_impl_delete_inplace_buffer(prh_byte *buffer, prh_reg offset); // buffer 必须使用自带的分配器进行释放

#define prh_malloc_inplace_memory(alloc, capacity, alignment) prh_malloc_inplace_memory_with_trace((alloc), (capacity), (alignemnt), __LINE__, prh_caller) // 只需保存一个指针，offset >= sizeof(prh_inplace_memory)
#define prh_calloc_inplace_memory(alloc, capacity, alignment) prh_calloc_inplace_memory_with_trace((alloc), (capacity), (alignemnt), __LINE__, prh_caller)
#define prh_malter_inplace_memory(alloc, buffer, capacity, offset) prh_malter_inplace_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_calter_inplace_memory(alloc, buffer, capacity, offset) prh_calter_inplace_memory_with_trace((alloc), (buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_delete_inplace_memory(alloc, buffer, offset) prh_delete_inplace_memory_with_trace((alloc), (buffer), (offset), __LINE__, prh_caller)

#define prh_realloc_inplace_memory(alloc, buffer, capacity, alignment) prh_realloc_inplace_memory_with_trace((alloc), (buffer), (capacity), (alignment), __LINE__, prh_caller)
#define prh_recalloc_inplace_memory(alloc, buffer, capacity, alignment) prh_recalloc_inplace_memory_with_trace((alloc), (buffer), (capacity), (alignment), __LINE__, prh_caller)
#define prh_realloc_inplace_buffer(alloc, buffer, capacity, alignment) prh_realloc_inplace_buffer_with_trace((alloc), (buffer), (capacity), (alignment), __LINE__, prh_caller)
#define prh_recalloc_inplace_buffer(alloc, buffer, capacity, alignment) prh_recalloc_inplace_buffer_with_trace((alloc), (buffer), (capacity), (alignment), __LINE__, prh_caller)

#define prh_malloc_inplace_buffer(alloc, capacity, alignment) prh_malloc_inplace_buffer_with_trace((alloc), (capacity), (alignemnt), __LINE__, prh_caller) // 只需保存一个指针，offset >= sizeof(prh_inplace_buffer)
#define prh_calloc_inplace_buffer(alloc, capacity, alignment) prh_calloc_inplace_buffer_with_trace((alloc), (capacity), (alignemnt), __LINE__, prh_caller)
#define prh_malter_inplace_buffer(buffer, capacity, offset) prh_malter_inplace_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_calter_inplace_buffer(buffer, capacity, offset) prh_calter_inplace_buffer_with_trace((buffer), (capacity), (offset), __LINE__, prh_caller)
#define prh_delete_inplace_buffer(buffer, offset) prh_delete_inplace_buffer_with_trace((buffer), (offset), __LINE__, prh_caller)

//////////////////////////////////////////////////////////////////////////////
///
/// ALLOC MEMORY INTERFACE
///

prh_inline void prh_malloc_memory_with_trace(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_malloc_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_calloc_memory_with_trace(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_calloc_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_malter_memory_with_trace(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_malter_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_calter_memory_with_trace(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_calter_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_delete_memory_with_trace(prh_alloc_face *alloc, prh_memory *buffer, prh_reg offset, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_delete_memory(alloc, buffer, offset);
    prh_memory_set_buffer(buffer, prh_null);
}

prh_inline void prh_realloc_memory_with_trace(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_assert(buffer != prh_null); // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    if (buffer->buffer_address == prh_null) prh_impl_malloc_memory(alloc, buffer, capacity, offset);
    else prh_impl_malter_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_recalloc_memory_with_trace(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_assert(buffer != prh_null); // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    if (buffer->buffer_address == prh_null) prh_impl_calloc_memory(alloc, buffer, capacity, offset);
    else prh_impl_calter_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

//////////////////////////////////////////////////////////////////////////////
///
/// ALLOC BUFFER INTERFACE
///

prh_inline void prh_malloc_buffer_with_trace(prh_buffer *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_malloc_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_calloc_buffer_with_trace(prh_buffer *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_calloc_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_malter_buffer_with_trace(prh_buffer *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_malter_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_calter_buffer_with_trace(prh_buffer *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_calter_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_delete_buffer_with_trace(prh_buffer *buffer, prh_reg offset, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_delete_buffer(buffer, offset);
    prh_buffer_set_data(buffer, prh_null);
}

prh_inline void prh_realloc_buffer_with_trace(prh_buffer *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_assert(buffer != prh_null); // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    if (buffer->buffer_address == prh_null) prh_impl_malloc_buffer(buffer, capacity, offset);
    else prh_impl_malter_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

prh_inline void prh_recalloc_buffer_with_trace(prh_buffer *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    prh_assert(buffer != prh_null); // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    if (buffer->buffer_address == prh_null) prh_impl_calloc_buffer(buffer, capacity, offset);
    else prh_impl_calter_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer->buffer_address != 0, line, caller);
}

//////////////////////////////////////////////////////////////////////////////
///
/// ALLOC INPLACE MEMORY INTERFACE
///

prh_inline prh_byte *prh_malloc_inplace_memory_with_trace(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    prh_byte *buffer = prh_impl_malloc_inplace_memory(alloc, capacity, alignment);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_calloc_inplace_memory_with_trace(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    prh_byte *buffer = prh_impl_calloc_inplace_memory(alloc, capacity, alignment);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_malter_inplace_memory_with_trace(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    buffer = prh_impl_malter_inplace_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_calter_inplace_memory_with_trace(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    buffer = prh_impl_calter_inplace_memory(alloc, buffer, capacity, offset);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void prh_delete_inplace_memory_with_trace(prh_alloc_face *alloc, prh_byte *buffer, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_delete_inplace_memory(alloc, buffer, offset);
}

prh_inline prh_byte *prh_realloc_inplace_memory_with_trace(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    if (buffer == prh_null) // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    {
        buffer = prh_impl_malloc_inplace_memory(alloc, capacity, alignemnt);
    }
    else
    {
        prh_assert(prh_alloc_alignment(alignment) == prh_inplace_memory_alignment(buffer));
        buffer = prh_impl_malter_inplace_memory(alloc, buffer, capacity, prh_alloc_offset(alignment));
    }
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_recalloc_inplace_memory_with_trace(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    if (buffer == prh_null) // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    {
        buffer = prh_impl_calloc_inplace_memory(alloc, capacity, alignemnt);
    }
    else
    {
        prh_assert(prh_alloc_alignment(alignment) == prh_inplace_memory_alignment(buffer));
        buffer = prh_impl_calter_inplace_memory(alloc, buffer, capacity, prh_alloc_offset(alignment));
    }
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

//////////////////////////////////////////////////////////////////////////////
///
/// ALLOC INPLACE BUFFER INTERFACE
///

prh_inline prh_byte *prh_malloc_inplace_buffer_with_trace(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    prh_byte *buffer = prh_impl_malloc_inplace_buffer(alloc, capacity, alignment);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_calloc_inplace_buffer_with_trace(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    prh_byte *buffer = prh_impl_calloc_inplace_buffer(alloc, capacity, alignment);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_malter_inplace_buffer_with_trace(prh_byte *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    buffer = prh_impl_malter_inplace_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_calter_inplace_buffer_with_trace(prh_byte *buffer, prh_reg capacity, prh_reg offset, int line, prh_raw caller)
{
    buffer = prh_impl_calter_inplace_buffer(buffer, capacity, offset);
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline void prh_delete_inplace_buffer_with_trace(prh_byte *buffer, prh_reg offset, int line, prh_raw caller)
{
    prh_impl_delete_inplace_buffer(buffer, offset);
}

prh_inline prh_byte *prh_realloc_inplace_buffer_with_trace(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    if (buffer == prh_null) // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    {
        buffer = prh_impl_malloc_inplace_buffer(alloc, capacity, alignemnt);
    }
    else
    {
        prh_assert(alloc == prh_inplace_buffer_alloc(buffer) && prh_alloc_alignment(alignment) == prh_inplace_buffer_alignment(buffer));
        buffer = prh_impl_malter_inplace_buffer(buffer, capacity, prh_alloc_offset(alignment));
    }
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

prh_inline prh_byte *prh_recalloc_inplace_buffer_with_trace(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_type_alignment alignemnt, int line, prh_raw caller)
{
    if (buffer == prh_null) // 大多数情况仅在容器初始化时走一次 if 分支，如果使用显式初始化则可以一次都不会走
    {
        buffer = prh_impl_calloc_inplace_buffer(alloc, capacity, alignemnt);
    }
    else
    {
        prh_assert(alloc == prh_inplace_buffer_alloc(buffer) && prh_alloc_alignment(alignment) == prh_inplace_buffer_alignment(buffer));
        buffer = prh_impl_calter_inplace_buffer(buffer, capacity, prh_alloc_offset(alignment));
    }
    prh_impl_real_assert(buffer != prh_null, line, caller);
    return buffer;
}

//////////////////////////////////////////////////////////////////////////////
///
/// EXTEND STDC ALLOC INTERFACE
///

#define prh_extend_stdc_malloc(alloc, capacity) (void *)prh_malloc_inplace_memory((alloc), (capacity), prh_default_alignment(sizeof(prh_inplace_memory)))
#define prh_extend_stdc_calloc(alloc, capacity) (void *)prh_calloc_inplace_memory((alloc), (capacity), prh_default_alignment(sizeof(prh_inplace_memory)))
#define prh_extend_stdc_malter(alloc, buffer, capacity) (void *)prh_malter_inplace_memory((alloc), (buffer), (capacity), sizeof(prh_inplace_memory))
#define prh_extend_stdc_calter(alloc, buffer, capacity) (void *)prh_calter_inplace_memory((alloc), (buffer), (capacity), sizeof(prh_inplace_memory))
#define prh_extend_stdc_delete(alloc, buffer) prh_delete_inplace_memory((alloc), (buffer), sizeof(prh_inplace_memory))

#define prh_extend_stdc_realloc(alloc, buffer, capacity) (void *)prh_realloc_inplace_memory((alloc), (buffer), (capacity), prh_default_alignment(sizeof(prh_inplace_memory)))
#define prh_extend_stdc_recalloc(alloc, buffer, capacity) (void *)prh_recalloc_inplace_memory((alloc), (buffer), (capacity), prh_default_alignment(sizeof(prh_inplace_memory)))
#define prh_extend_stdc_aligned_realloc(alloc, buffer, capacity, alignment) (void *)prh_realloc_inplace_memory((alloc), (buffer), (capacity), prh_special_real_alignment(sizeof(prh_inplace_memory), (alignment)))
#define prh_extend_stdc_aligned_recalloc(alloc, buffer, capacity, alignment) (void *)prh_recalloc_inplace_memory((alloc), (buffer), (capacity), prh_special_real_alignment(sizeof(prh_inplace_memory), (alignment)))

#define prh_extend_stdc_aligned_malloc(alloc, capacity, alignment) (void *)prh_malloc_inplace_memory((alloc), (capacity), prh_special_real_alignment(sizeof(prh_inplace_memory), (alignment)))
#define prh_extend_stdc_aligned_calloc(alloc, capacity, alignment) (void *)prh_calloc_inplace_memory((alloc), (capacity), prh_special_real_alignment(sizeof(prh_inplace_memory), (alignment)))
#define prh_extend_stdc_aligned_malter(alloc, buffer, capacity) (void *)prh_malter_inplace_memory((alloc), (buffer), (capacity), sizeof(prh_inplace_memory))
#define prh_extend_stdc_aligned_calter(alloc, buffer, capacity) (void *)prh_calter_inplace_memory((alloc), (buffer), (capacity), sizeof(prh_inplace_memory))
#define prh_extend_stdc_aligned_delete(alloc, buffer) prh_delete_inplace_memory((alloc), (buffer), sizeof(prh_inplace_memory))

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// GLOBAL DEFAULT ALLOC
///
///

#define prh_default_malloc(buffer, capacity) prh_malloc_memory(prh_default_alloc(), (buffer), (capacity)) // 需要自行保存 prh_memory 一般两个指针大小，默认分配器初始化后永不改变，不需要记录分配器
#define prh_default_calloc(buffer, capacity) prh_calloc_memory(prh_default_alloc(), (buffer), (capacity))
#define prh_default_malter(buffer, capacity) prh_malter_memory(prh_default_alloc(), (buffer), (capacity))
#define prh_default_calter(buffer, capacity) prh_calter_memory(prh_default_alloc(), (buffer), (capacity))
#define prh_default_delete(buffer) prh_delete_memory(prh_default_alloc(), (buffer))

#define prh_default_offset_malloc(buffer, capacity, offset) prh_malloc_offset_memory(prh_default_alloc(), (buffer), (capacity), (offset))
#define prh_default_offset_calloc(buffer, capacity, offset) prh_calloc_offset_memory(prh_default_alloc(), (buffer), (capacity), (offset))
#define prh_default_offset_malter(buffer, capacity, offset) prh_malter_offset_memory(prh_default_alloc(), (buffer), (capacity), (offset))
#define prh_default_offset_calter(buffer, capacity, offset) prh_calter_offset_memory(prh_default_alloc(), (buffer), (capacity), (offset))
#define prh_default_offset_delete(buffer, offset) prh_delete_offset_memory(prh_default_alloc(), (buffer), (offset))

#define prh_default_inplace_malloc(capacity, alignment) prh_malloc_inplace_memory(prh_default_alloc(), (buffer), (capacity), (alignemnt)) // 只需保存一个指针，offset >= sizeof(prh_inplace_memory)
#define prh_default_inplace_calloc(capacity, alignment) prh_calloc_inplace_memory(prh_default_alloc(), (buffer), (capacity), (alignemnt))
#define prh_default_inplace_malter(buffer, capacity, offset) prh_malter_inplace_memory(prh_default_alloc(), (buffer), (capacity), (offset))
#define prh_default_inplace_calter(buffer, capacity, offset) prh_calter_inplace_memory(prh_default_alloc(), (buffer), (capacity), (offset))
#define prh_default_inplace_delete(buffer, offset) prh_delete_inplace_memory(prh_default_alloc(), (buffer), (offset))

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// THREAD SPECIFIC STATIC & INTANT ALLOC
///
///

// TODO: 两种实现方式
//  1.  一次性分配足够大的内存区间（例如只有使用时才真正分配的虚拟内存页），不足之后使用默认分配
//  2.  以固定大小的内存块开始，使用后备分配器分配更多的内存块实现自动增长

typedef struct { // 仅由所属线程访问和修改
    prh_alignas(prh_cache_line_size)
    prh_alloc_face alloc_base;
    prh_byte *base_address;
    prh_byte *buffer_end;
    prh_byte *walk_pointer;
    prh_reg overflow_bytes;
    prh_reg peak_bytes;
} prh_type_static_alloc;

typedef struct { // 仅由所属线程访问和修改
    prh_alignas(prh_cache_line_size)
    prh_alloc_face alloc_base;
    prh_byte *base_address;
    prh_byte *buffer_end;
    prh_byte *walk_pointer;
    prh_reg overflow_bytes;
    prh_reg peak_bytes;
} prh_type_intant_alloc;

prh_static_assert(sizeof(prh_type_static_alloc) == sizeof(prh_type_intant_alloc));

prh_export void prh_create_static_alloc(prh_type_static_alloc *self, prh_byte *base_address, prh_reg capacity);
prh_export void prh_create_intant_alloc(prh_type_intant_alloc *self, prh_byte *base_address, prh_reg capacity);
prh_export void prh_restore_intant_alloc(prh_byte *buffer);
prh_export void prh_reset_intant_alloc(void);

prh_inline prh_alloc_face *prh_static_alloc(void)
{
    prh_alloc_face *alloc = (prh_alloc_face *)prh_thread_runtime()->static_alloc;
    prh_assert(alloc != prh_null);
    return alloc;
}

// 静态分配的内存永远不释放，直到对应的线程退出为止

#define prh_static_malloc(buffer, capacity) prh_malloc_memory(prh_static_alloc(), (buffer), (capacity)) // 需要自行保存 prh_memory 一般两个指针大小，静态分配器初始化后永不改变，不需要记录分配器
#define prh_static_calloc(buffer, capacity) prh_calloc_memory(prh_static_alloc(), (buffer), (capacity))
#define prh_static_malter(buffer, capacity) prh_malter_memory(prh_static_alloc(), (buffer), (capacity))
#define prh_static_calter(buffer, capacity) prh_calter_memory(prh_static_alloc(), (buffer), (capacity))

#define prh_static_offset_malloc(buffer, capacity, offset) prh_malloc_offset_memory(prh_static_alloc(), (buffer), (capacity), (offset))
#define prh_static_offset_calloc(buffer, capacity, offset) prh_calloc_offset_memory(prh_static_alloc(), (buffer), (capacity), (offset))
#define prh_static_offset_malter(buffer, capacity, offset) prh_malter_offset_memory(prh_static_alloc(), (buffer), (capacity), (offset))
#define prh_static_offset_calter(buffer, capacity, offset) prh_calter_offset_memory(prh_static_alloc(), (buffer), (capacity), (offset))

#define prh_static_inplace_malloc(capacity, alignment) prh_malloc_inplace_memory(prh_static_alloc(), (buffer), (capacity), (alignemnt)) // 只需保存一个指针，offset >= sizeof(prh_inplace_memory)
#define prh_static_inplace_calloc(capacity, alignment) prh_calloc_inplace_memory(prh_static_alloc(), (buffer), (capacity), (alignemnt))
#define prh_static_inplace_malter(buffer, capacity, offset) prh_malter_inplace_memory(prh_static_alloc(), (buffer), (capacity), (offset))
#define prh_static_inplace_calter(buffer, capacity, offset) prh_calter_inplace_memory(prh_static_alloc(), (buffer), (capacity), (offset))

prh_inline prh_alloc_face *prh_intant_alloc(void)
{
    prh_alloc_face *alloc = (prh_alloc_face *)prh_thread_runtime()->intant_alloc;
    prh_assert(alloc != prh_null);
    return alloc;
}

prh_inline prh_byte *prh_protect_intant_alloc(void)
{
    return ((prh_type_intant_alloc *)prh_intant_alloc())->walk_pointer;
}

// 不使用该策略：即时分配的内存必须在同一个函数中释放，并且必须按先分配的内存后释放的原则释放
// 即时分配的内存由即时分配器自身决定记不记录，并且由上层调用者决定怎么释放或者只需简单重置

#define prh_intant_malloc(buffer, capacity) prh_malloc_memory(prh_intant_alloc(), (buffer), (capacity)) // 需要自行保存 prh_memory 一般两个指针大小，即时分配器初始化后永不改变，不需要记录分配器
#define prh_intant_calloc(buffer, capacity) prh_calloc_memory(prh_intant_alloc(), (buffer), (capacity))
#define prh_intant_malter(buffer, capacity) prh_malter_memory(prh_intant_alloc(), (buffer), (capacity))
#define prh_intant_calter(buffer, capacity) prh_calter_memory(prh_intant_alloc(), (buffer), (capacity))
#define prh_intant_delete(buffer) prh_delete_memory(prh_intant_alloc(), (buffer))

#define prh_intant_offset_malloc(buffer, capacity) prh_malloc_offset_memory(prh_intant_alloc(), (buffer), (capacity), (offset))
#define prh_intant_offset_calloc(buffer, capacity) prh_calloc_offset_memory(prh_intant_alloc(), (buffer), (capacity), (offset))
#define prh_intant_offset_malter(buffer, capacity) prh_malter_offset_memory(prh_intant_alloc(), (buffer), (capacity), (offset))
#define prh_intant_offset_calter(buffer, capacity) prh_calter_offset_memory(prh_intant_alloc(), (buffer), (capacity), (offset))
#define prh_intant_offset_delete(buffer) prh_delete_offset_memory(prh_intant_alloc(), (buffer), (offset))

#define prh_intant_inplace_malloc(capacity, alignment) prh_malloc_inplace_memory(prh_intant_alloc(), (buffer), (capacity), (alignemnt)) // 只需保存一个指针，offset >= sizeof(prh_inplace_memory)
#define prh_intant_inplace_calloc(capacity, alignment) prh_calloc_inplace_memory(prh_intant_alloc(), (buffer), (capacity), (alignemnt))
#define prh_intant_inplace_malter(buffer, capacity, offset) prh_malter_inplace_memory(prh_intant_alloc(), (buffer), (capacity), (offset))
#define prh_intant_inplace_calter(buffer, capacity, offset) prh_calter_inplace_memory(prh_intant_alloc(), (buffer), (capacity), (offset))
#define prh_intant_inplace_delete(buffer, offset) prh_delete_inplace_memory(prh_intant_alloc(), (buffer), (offset))

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// THREAD SPECIFIC CURRENT ALLOC
///
///

prh_inline void prh_push_allocator(prh_alloc_face *alloc)
{
    prh_runtime_context *runtime = prh_thread_runtime();
    prh_assert(alloc != prh_null && runtime->alloc_top_index + 1 < prh_alloc_stack_size);
    runtime->allocators[++runtime->alloc_top_index] = alloc;
}

prh_inline void prh_push_intant_alloc(void)
{
    prh_runtime_context *runtime = prh_thread_runtime();
    prh_assert(runtime->intant_alloc != prh_null && runtime->alloc_top_index + 1 < prh_alloc_stack_size);
    runtime->allocators[++runtime->alloc_top_index] = runtime->intant_alloc;
}

prh_inline void prh_pop_allocator(void)
{
    prh_assert(prh_thread_runtime()->alloc_top_index > 0);
    prh_thread_runtime()->alloc_top_index -= 1;
}

prh_inline prh_alloc_face *prh_current_alloc(void)
{
    prh_runtime_context *runtime = prh_thread_runtime();
    return (prh_alloc_face *)runtime->allocators[runtime->alloc_top_index];
}

prh_buffer prh_current_buffer(void) // 需要自行保存 prh_buffer 一般三个指针大小，当前分配器随时发生改变，需要记录分配器
{
    return prh_empty_buffer(prh_current_alloc());
}

prh_buffer prh_current_aligned_buffer(prh_alloc_align_enum alignment)
{
    return prh_aligned_buffer(prh_current_alloc(), alignment);
}

prh_buffer prh_current_real_aligned_buffer(prh_reg real_alignment)
{
    return prh_real_aligned_buffer(prh_current_alloc(), real_alignment);
}

prh_export prh_byte *prh_offset_malloc(prh_reg capacity, prh_reg offset); // 只需保存一个指针，offset >= sizeof(prh_inplace_buffer)

prh_export prh_byte *prh_offset_calloc(prh_reg capacity, prh_reg offset);

prh_export prh_byte *prh_offset_aligned_malloc(prh_reg capacity, prh_type_alignment alignment);

prh_export prh_byte *prh_offset_aligned_calloc(prh_reg capacity, prh_type_alignment alignment);

prh_inline prh_byte *prh_offset_malter(prh_byte *buffer, prh_reg capacity, prh_reg offset)
{
    return prh_impl_malter_inplace_buffer(buffer, capacity, offset);
}

prh_inline prh_byte *prh_offset_calter(prh_byte *buffer, prh_reg capacity, prh_reg offset)
{
    return prh_impl_calter_inplace_buffer(buffer, capacity, offset);
}

prh_inline void prh_offset_delete(prh_byte *buffer, prh_reg offset)
{
    prh_impl_delete_inplace_buffer(buffer, offset);
}

#endif // prh_impl_alloc_include_h

#if defined(prh_source_implement)

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// GENERAL ALLOC IMPLEMENTATION
///
///

// 内存普通分配和原地分配的内存布局（memory allocation layout）
//
//                                  .----------- offset -----------.-------------- <capacity> --------------.
//                                  [header with extra_offset_bytes|aligned memory block with capacity bytes]
// prh_memory [<capacity>    ]                                     ^
//            [buffer_address] ------------------------------------'
//
// prh_inplace_memory * buffer ------------------------------------.
//                                                                 v
//                                  .----------- offset -----------.-------------- <capacity> --------------.
//                                  [ ................. |<capacity>|aligned memory block with capacity bytes]
//

//////////////////////////////////////////////////////////////////////////////
///
/// ALLOC MEMORY & BUFFER
///

void prh_impl_malloc_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && buffer->buffer_address == 0);
    alloc->alloc_func(alloc, buffer, capacity, offset);
}

void prh_impl_calloc_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && buffer->buffer_address == 0);
    alloc->alloc_func(alloc, buffer, capacity, offset);
    if (buffer->buffer_address) memset(prh_memory_buffer(buffer) - offset, 0, offset + prh_memory_capacity(buffer));
}

void prh_impl_malter_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && buffer->buffer_address != 0);
    prh_memory memory = prh_aligned_memory(prh_memory_alignment(buffer)); // 要求 buffer 必须是通过同一个 alloc 分配的，两次分配的 offset 必须也一样
    alloc->alloc_func(alloc, &memory, capacity, offset); // 总是分配一个新的内存，并将旧内存拷贝到新内存
    if (memory.buffer_address) prh_memcpy_old_content_to_new_buffer(prh_memory_buffer(&memory) - offset, offset + prh_memory_capacity(&memory), prh_memory_buffer(buffer) - offset, offset + prh_memory_capacity(buffer));
    alloc->alloc_free(alloc, buffer, offset);
    *buffer = memory;
}

void prh_impl_calter_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && buffer->buffer_address != 0);
    prh_memory memory = prh_aligned_memory(prh_memory_alignment(buffer)); // 要求 buffer 必须是通过同一个 alloc 分配的，两次分配的 offset 必须也一样
    alloc->alloc_func(alloc, &memory, capacity, offset); // 总是分配一个新的内存，并将旧内存拷贝到新内存
    if (memory.buffer_address) prh_memcpy_old_content_memset_expanded_content(prh_memory_buffer(&memory) - offset, offset + prh_memory_capacity(&memory), prh_memory_buffer(buffer) - offset, offset + prh_memory_capacity(buffer));
    alloc->alloc_free(alloc, buffer, offset);
    *buffer = memory;
}

void prh_impl_delete_memory(prh_alloc_face *alloc, prh_memory *buffer, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && buffer->buffer_address != 0); // 分配器永远不会返回空指针，因为一旦分配失败程序直接崩溃退出
    alloc->alloc_free(alloc, buffer, offset); // 要求 buffer 必须是通过 alloc 分配的，并且分配时使用的 offset 必须也相同
}

void prh_impl_malloc_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_buffer_alloc(buffer) != prh_null && prh_buffer_data(buffer) == prh_null);
    alloc->alloc_func((prh_buffer_alloc(buffer), prh_buffer_memory(buffer), capacity, offset);
}

void prh_impl_calloc_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_buffer_alloc(buffer) != prh_null && prh_buffer_data(buffer) == prh_null);
    prh_impl_calloc_memory(prh_buffer_alloc(buffer), prh_buffer_memory(buffer), capacity, offset);
}

void prh_impl_malter_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_buffer_alloc(buffer) != prh_null && prh_buffer_data(buffer) != prh_null);
    prh_impl_malter_memory(prh_buffer_alloc(buffer), prh_buffer_memory(buffer), capacity, offset); // offset 必须是 buffer 分配时使用的 offset
}

void prh_impl_calter_buffer(prh_buffer *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_buffer_alloc(buffer) != prh_null && prh_buffer_data(buffer) != prh_null);
    prh_impl_calter_memory(prh_buffer_alloc(buffer), prh_buffer_memory(buffer), capacity, offset); // offset 必须是 buffer 分配时使用的 offset
}

void prh_impl_delete_buffer(prh_buffer *buffer, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_buffer_alloc(buffer) != prh_null && prh_buffer_data(buffer) != prh_null);
    alloc->alloc_free(prh_buffer_alloc(buffer), prh_buffer_memory(buffer), offset); // offset 必须是 buffer 分配时使用的 offset
}

//////////////////////////////////////////////////////////////////////////////
///
/// ALLOC INPLACE MEMORY & BUFFER
///

prh_byte *prh_impl_malloc_inplace_memory(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt)
{
    prh_assert(alloc != prh_null && prh_alloc_offset(alignment) >= sizeof(prh_inplace_memory));
    prh_memory memory = prh_aligned_memory(prh_alloc_alignment(alignment));
    alloc->alloc_func(alloc, &memory, capacity, prh_alloc_offset(alignment));
    if (memory.buffer_address) prh_inplace_memory_init_from(&memory);
    return prh_memory_buffer(&memory);
}

prh_byte *prh_impl_calloc_inplace_memory(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt)
{
    prh_assert(alloc != prh_null && prh_alloc_offset(alignment) >= sizeof(prh_inplace_memory));
    prh_memory memory = prh_aligned_memory(prh_alloc_alignment(alignment));
    alloc->alloc_func(alloc, &memory, capacity, prh_alloc_offset(alignment));
    if (memory.buffer_address)
    {
        memset(prh_memory_buffer(&memory) - prh_alloc_offset(alignment), 0, prh_alloc_offset(alignment) + prh_memory_capacity(&memory));
        prh_inplace_memory_init_from(&memory);
    }
    return prh_memory_buffer(&memory);
}

prh_byte *prh_impl_malter_inplace_memory(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && offset >= sizeof(prh_inplace_memory));
    prh_memory memory = prh_aligned_memory(prh_inplace_memory_alignment(buffer)); // 要求 buffer 必须是通过同一个 alloc 分配的，两次分配的 offset 必须也一样
    alloc->alloc_func(alloc, &memory, capacity, offset); // 总是分配一个新的内存，并将旧内存拷贝到新内存
    if (memory.buffer_address)
    {
        prh_memcpy_old_content_to_new_buffer(prh_memory_buffer(&memory) - offset, offset + prh_memory_capacity(&memory), buffer - offset, offset + prh_inplace_memory_capacity(buffer));
        prh_inplace_memory_init_from(&memory);
    }
    prh_impl_delete_inplace_memory(alloc, buffer, offset);
    return prh_memory_buffer(&memory);
}

prh_byte *prh_impl_calter_inplace_memory(prh_alloc_face *alloc, prh_byte *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && offset >= sizeof(prh_inplace_memory));
    prh_memory memory = prh_aligned_memory(prh_inplace_memory_alignment(buffer)); // 要求 buffer 必须是通过同一个 alloc 分配的，两次分配的 offset 必须也一样
    alloc->alloc_func(alloc, &memory, capacity, offset); // 总是分配一个新的内存，并将旧内存拷贝到新内存
    if (memory.buffer_address)
    {
        prh_memcpy_old_content_memset_expanded_content(prh_memory_buffer(&memory) - offset, offset + prh_memory_capacity(&memory), buffer - offset, offset + prh_inplace_memory_capacity(buffer));
        prh_inplace_memory_init_from(&memory);
    }
    prh_impl_delete_inplace_memory(alloc, buffer, offset);
    return prh_memory_buffer(&memory);
}

void prh_impl_delete_inplace_memory(prh_alloc_face *alloc, prh_byte *buffer, prh_reg offset)
{
    prh_assert(alloc != prh_null && buffer != prh_null && offset >= sizeof(prh_inplace_memory)); // 分配器永远不会返回空指针，因为一旦分配失败程序直接崩溃退出
    prh_memory memory = prh_memory_from(buffer); // 要求 buffer 必须是通过 alloc 分配的，并且分配时使用的 offset 必须也相同
    alloc->alloc_free(alloc, &memory, offset);
}

prh_byte *prh_impl_malloc_inplace_buffer(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt)
{
    prh_assert(alloc != prh_null && prh_alloc_offset(alignment) >= sizeof(prh_inplace_buffer));
    prh_memory memory = prh_aligned_memory(prh_alloc_alignment(alignment));
    alloc->alloc_func(alloc, &memory, capacity, prh_alloc_offset(alignment));
    if (memory.buffer_address) prh_inplace_buffer_init_from(alloc, &memory);
    return prh_memory_buffer(&memory);
}

prh_byte *prh_impl_calloc_inplace_buffer(prh_alloc_face *alloc, prh_reg capacity, prh_type_alignment alignemnt)
{
    prh_assert(alloc != prh_null && prh_alloc_offset(alignment) >= sizeof(prh_inplace_buffer));
    prh_memory memory = prh_aligned_memory(prh_alloc_alignment(alignment));
    alloc->alloc_func(alloc, &memory, capacity, prh_alloc_offset(alignment));
    if (memory.buffer_address)
    {
        memset(prh_memory_buffer(&memory) - prh_alloc_offset(alignment), 0, prh_alloc_offset(alignment) + prh_memory_capacity(&memory));
        prh_inplace_buffer_init_from(alloc, &memory);
    }
    return prh_memory_buffer(&memory);
}

prh_byte *prh_impl_malter_inplace_buffer(prh_byte *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_inplace_buffer_alloc(buffer) != prh_null && offset >= sizeof(prh_inplace_buffer));
    prh_alloc_face *alloc = prh_inplace_buffer_alloc(buffer);
    prh_memory memory = prh_aligned_memory(prh_inplace_buffer_alignment(buffer)); // offset 必须是 buffer 分配时使用的 offset
    alloc->alloc_func(alloc, &memory, capacity, offset); // 总是分配一个新的内存，并将旧内存拷贝到新内存
    if (memory.buffer_address)
    {
        prh_memcpy_old_content_to_new_buffer(prh_memory_buffer(&memory) - offset, offset + prh_memory_capacity(&memory), buffer - offset, offset + prh_inplace_buffer_capacity(buffer));
        prh_inplace_memory_init_from(&memory);
    }
    prh_impl_delete_inplace_buffer(buffer, offset);
    return prh_memory_buffer(&memory);
}

prh_byte *prh_impl_calter_inplace_buffer(prh_byte *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_inplace_buffer_alloc(buffer) != prh_null && offset >= sizeof(prh_inplace_buffer));
    prh_alloc_face *alloc = prh_inplace_buffer_alloc(buffer);
    prh_memory memory = prh_aligned_memory(prh_inplace_buffer_alignment(buffer)); // offset 必须是 buffer 分配时使用的 offset
    alloc->alloc_func(alloc, &memory, capacity, offset); // 总是分配一个新的内存，并将旧内存拷贝到新内存
    if (memory.buffer_address)
    {
        prh_memcpy_old_content_memset_expanded_content(prh_memory_buffer(&memory) - offset, offset + prh_memory_capacity(&memory), buffer - offset, offset + prh_inplace_buffer_capacity(buffer));
        prh_inplace_memory_init_from(&memory);
    }
    prh_impl_delete_inplace_buffer(buffer, offset);
    return prh_memory_buffer(&memory);
}

void prh_impl_delete_inplace_buffer(prh_byte *buffer, prh_reg offset)
{
    prh_assert(buffer != prh_null && prh_inplace_buffer_alloc(buffer) != prh_null && offset >= sizeof(prh_inplace_buffer));
    prh_alloc_face *alloc = prh_inplace_buffer_alloc(buffer); // 分配器永远不会返回空指针，因为一旦分配失败程序直接崩溃退出
    prh_memory memory = prh_memory_from(buffer); // offset 必须是 buffer 分配时使用的 offset
    alloc->alloc_free(alloc, &memory, offset);
}

prh_byte *prh_offset_malloc(prh_reg capacity, prh_reg offset)
{
    return prh_impl_malloc_inplace_buffer(prh_current_alloc(), capacity, prh_default_alignment(offset));
}

prh_byte *prh_offset_calloc(prh_reg capacity, prh_reg offset)
{
    return prh_impl_calloc_inplace_buffer(prh_current_alloc(), capacity, prh_default_alignment(offset));
}

prh_byte *prh_offset_aligned_malloc(prh_reg capacity, prh_type_alignment alignment)
{
    return prh_impl_malloc_inplace_buffer(prh_current_alloc(), capacity, alignment);
}

prh_byte *prh_offset_aligned_calloc(prh_reg capacity, prh_type_alignment alignment)
{
    return prh_impl_calloc_inplace_buffer(prh_current_alloc(), capacity, alignment);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// DEFAULT ALLOC IMPLEMENTATION
///
///

void prh_impl_default_stdc_alloc_func(prh_alloc_face *alloc, prh_memory *buffer, prh_reg capacity, prh_reg offset)
{
    prh_assert(buffer != prh_null);
    prh_reg alignment = prh_memory_real_alignment(buffer);
    capacity = prh_alloc_capacity(capacity, alignment);
    prh_memory_set_capacity(buffer, capacity);

#if defined(prh_impl_plat_aligned_offset_malloc)
    prh_assert(capacity + offset >= capacity);
    prh_byte *data = (prh_byte *)prh_impl_plat_aligned_offset_malloc(offset + capacity, alignment, offset);
    prh_memory_set_buffer(buffer, (prh_byte *)prh_raw_set_value_if_true((prh_raw)(data + offset), 0, data != prh_null));
#else
    prh_reg new_offset = prh_times_align_size(offset, alignment); // offset 为 0 对齐后还是 0
    prh_assert(capacity + new_offset >= capacity);
    prh_byte *data = (prh_byte *)prh_impl_plat_aligned_malloc(new_offset + capacity, alignment);
    prh_memory_set_buffer(buffer, (prh_byte *)prh_raw_set_value_if_true((prh_raw)(data + new_offset), 0, data != prh_null));
#endif
}

void prh_impl_default_stdc_alloc_free(prh_alloc_face *alloc, prh_memory *buffer, prh_reg offset)
{
    prh_assert(buffer != prh_null);
    if (buffer->buffer_address == 0)
    {
        return;
    }
#if defined(prh_impl_plat_aligned_offset_malloc)
    prh_impl_plat_aligned_delete(prh_memory_buffer(buffer) - offset);
#else
    prh_impl_plat_aligned_delete(prh_memory_buffer(buffer) - prh_times_align_size(offset, prh_memory_real_alignment(buffer)));
#endif
}

void prh_empty_alloc_free(prh_alloc_face *alloc, prh_memory *buffer, prh_reg offset)
{
    prh_unused(alloc);
    prh_unused(buffer);
}

static prh_alloc_face prh_impl_default_stdc_alloc = {prh_impl_default_stdc_alloc_func, prh_impl_default_stdc_alloc_free};
static prh_alloc_face *prh_impl_default_alloc = &prh_impl_default_stdc_alloc;

prh_alloc_face *prh_default_alloc(void)
{
    return &prh_impl_default_alloc;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// INTANT & STATIC ALLOC IMPLEMENTATION
///
///

#define prh_impl_intant ((prh_type_intant_alloc *)alloc)

void prh_impl_intant_overflow_alloc_func(prh_alloc_face *alloc, prh_memory *memory, prh_reg capacity, prh_reg offset)
{
    prh_default_malloc(memory, capacity, offset);
#if PRH_DEBUG
    prh_impl_intant->overflow_bytes += offset + prh_memory_capacity(memory);
    if (prh_impl_intant->overflow_bytes > prh_impl_intant->peak_bytes)
    {
        prh_impl_intant->peak_bytes = prh_impl_intant->overflow_bytes;
    }
#endif
}

void prh_impl_intant_overflow_alloc_free(prh_alloc_face *alloc, prh_memory *memory, prh_reg offset)
{
    prh_byte *buffer = prh_memory_buffer(memory);

    // 1. intant malloc P1 in range [base_address, buffer_end)
    // 2. intant malloc P2 overflow
    // 3. intant delete P2 shall global_offset_delete it
    // 4. intant delete P1 shall NOT global_offset_delete it

    if (buffer != prh_null && (buffer < prh_impl_intant->base_address || buffer >= prh_impl_intant->buffer_end))
    {
#if PRH_DEBUG
        prh_assert(prh_impl_intant->overflow_bytes >= prh_memory_capacity(memory));
        prh_impl_intant->overflow_bytes -= prh_memory_capacity(memory);
#endif
        prh_default_delete(memory, offset);
    }
}

void prh_impl_intant_alloc_func(prh_alloc_face *alloc, prh_memory *memory, prh_reg capacity, prh_reg offset)
{
    prh_assert(alloc != prh_null && memory != prh_null);
    prh_buffer *buffer = prh_aligned_address(prh_impl_intant->walk_pointer, prh_memory_real_alignment(memory), offset);
    prh_impl_intant->walk_pointer = buffer + (capacity = prh_alloc_capacity(capacity, prh_memory_real_alignment(memory)));

    prh_memory_set_buffer(memory, buffer);
    prh_memory_set_capacity(memory, capacity);

    if (prh_impl_intant->walk_pointer > prh_impl_intant->buffer_end)
    {
        prh_impl_intant->alloc_base.alloc_func = prh_impl_intant_overflow_alloc_func;
        prh_impl_intant->alloc_base.alloc_free = prh_impl_intant_overflow_alloc_free;
        prh_impl_intant->overflow_bytes = prh_impl_intant->buffer_end - prh_impl_intant->base_address;
        prh_memory overflow_memory = prh_aligned_memory(prh_memory_alignment(memory));
        prh_impl_intant_overflow_alloc_func(alloc, &overflow_memory, capacity, offset);
        prh_memory_set_buffer(memory, prh_memory_buffer(&overflow_memory));
    }
#if PRH_DEBUG
    else if (prh_impl_intant->walk_pointer - prh_impl_intant->base_address > prh_impl_intant->peak_bytes)
    {
        prh_impl_intant->peak_bytes = prh_impl_intant->walk_pointer - prh_impl_intant->base_address;
    }
#endif
}

void prh_impl_intant_alloc_free(prh_alloc_face *alloc, prh_memory *memory, prh_reg offset)
{
    prh_assert(memory != prh_null && prh_memory_buffer(memory) >= prh_impl_intant->base_address && prh_memory_buffer(memory) < prh_impl_intant->buffer_end);
    prh_assert((prh_reg)prh_memory_buffer(memory) & (prh_memory_real_alignment(memory) - 1) == 0);
    prh_impl_intant->walk_pointer = prh_memory_buffer(memory) - offset;
}

void prh_restore_intant_alloc(prh_byte *saved_buffer_tail)
{
    prh_type_intant_alloc *self = (prh_type_intant_alloc *)prh_intant_alloc();

    // 在恢复之前 baseline 之后分配的内存必须已经完全释放完毕
    //  1.  restore_baseline = prh_protect_intant_alloc()
    //  2.  intant malloc P1, P2, ...
    //  3.  intant delete ..., P2, P1
    //  4.  prh_restore_intant_alloc(restore_baseline)

    if (self->overflow_bytes == 0)
    {
        prh_assert(saved_buffer_tail != prh_null && saved_buffer_tail >= self->base_address && saved_buffer_tail <= self->buffer_end);
        prh_assert((prh_raw)saved_buffer_tail & (sizeof(prh_reg) - 1) == 0); // 至少对齐到 prh_reg 大小
        self->walk_pointer = saved_buffer_tail;
    }
    else
    {
        // overflow 之后 walk_pointer 不会变化，恢复不会有什么问题，但有问题的是要恢复的基线位于正常
        // 分配区间，但在恢复之前发生了 overflow 的情况：
        //  1.  restore_baseline = prh_protect_intant_alloc()
        //  2.  intant malloc P1, P2
        //  3.  intant malloc P3 overflow
        //  4.  intant delete P3 global delete
        //  5.  intant delete P2 do nothing
        //  6.  intant delete P1 do nothing
        //  7.  prh_restore_intant_alloc(restore_baseline)
        //  8.  必须将 overflow 状态恢复到正常 intant 分配状态
        //  9.  或者什么也不做，只有调用 prh_reset_intant_alloc 才能恢复正常分配状态
    }
}

void prh_reset_intant_alloc(void)
{
    prh_type_intant_alloc *self = (prh_type_intant_alloc *)prh_intant_alloc();;
    prh_assert(self != prh_null && ((prh_raw)self->base_address & (sizeof(prh_reg) - 1)) == 0);
    self->alloc_base.alloc_func = prh_impl_intant_alloc_func;
    self->alloc_base.alloc_free = prh_impl_intant_alloc_free;
    self->walk_pointer = self->base_address;
    self->overflow_bytes = 0;
    self->peak_bytes = 0;
}

void prh_impl_create_intant_alloc(prh_type_intant_alloc *self, prh_byte *base_address, prh_reg capacity)
{
    prh_assert(self != prh_null && ((prh_raw)base_address & (sizeof(prh_reg) - 1)) == 0); // base_address 可以为零表示不使用即时分配
    self->alloc_base.alloc_func = prh_impl_intant_alloc_func;
    self->base_address = base_address;
    self->bufffer_end = base_address + capacity;
    self->walk_pointer = base_address;
    self->overflow_bytes = 0;
    self->peak_bytes = 0;
}

void prh_create_intant_alloc(prh_type_intant_alloc *self, prh_byte *base_address, prh_reg capacity)
{
    prh_impl_create_intant_alloc(self, base_address, capacity);
    self->alloc_base.alloc_free = prh_impl_intant_alloc_free;
}

void prh_create_static_alloc(prh_type_static_alloc *self, prh_byte *base_address, prh_reg capacity)
{
    prh_impl_create_intant_alloc((prh_type_intant_alloc *)self, base_address, capacity);
    self->alloc_base.alloc_free = prh_empty_alloc_free;
}

#endif // prh_source_implement
#endif // prh_alloc_include

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   ATOMIC INTREFACE
////
////

#if defined(prh_atomic_include)
#ifndef prh_impl_atomic_include_h
#define prh_impl_atomic_include_h // 仅包含一次

// 这里的原子类型不需要声明成 volatile，因为它们总是通过变量的地址进行访问，而不是直接访问变量的值。
// 如果传一个变量的地址给函数，那么函数必须从内存中读取它的值，变量值不会被优化到寄存器中，因此编译
// 器优化不会对此产生影响。

typedef prh_i32 prh_atom_i32;
typedef prh_r32 prh_atom_r32;
typedef prh_imt prh_atom_imt;
typedef prh_raw prh_atom_raw;
typedef prh_raw prh_atom_ptr;

#endif // prh_impl_atomic_include_h

#ifdef prh_multiple_time_include // 可包含多次不同配置

#undef prh_impl_stdc_atomic
#undef prh_impl_sdl3_atomic

#if defined(prh_using_stdc_atomic)
    #define prh_impl_stdc_atomic 1
#elif defined(prh_using_sdl3_atomic)
    #define prh_impl_sdl3_atomic 1
#else
    #define prh_impl_stdc_atomic 1
#endif

#undef prh_atom_r32_read
#undef prh_atom_r32_write
#undef prh_atom_r32_add
#undef prh_atom_r32_compare_exchange
#undef prh_atom_raw_read
#undef prh_atom_raw_write
#undef prh_atom_raw_add
#undef prh_atom_raw_compare_exchange

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// STDC ATOMIC INTERFACE
///
///

#if defined(prh_impl_stdc_atomic)
#include <stdatomic.h>

// 当多个线程访问一个原子对象时，所有的原子操作都会针对该原子对象产生明确的行为：在任
// 何其他原子操作能够访问该对象之前，每个原子操作都会在该对象上完整地执行完毕。这就保
// 证了在这些对象上不会出现数据竞争，而这也正是定义原子性的关键特征。
//
// https://devblogs.microsoft.com/cppblog/c11-atomics-in-visual-studio-2022-version-17-5-preview-2/
//
// Once you’ve installed Visual Studio 2022 17.5 Preview 2 you can try out C11
// atomics by adding /experimental:c11atomics and /std:c11 or /std:c17 to your
// compile options. Note that we still define __STDC_NO_ATOMICS__ so if your
// build system is testing for that you will need to add a special case or
// change to checking if compiling a translation unit including <stdatomic.h>
// succeeds.
//
// At the moment only lock-free atomics are supported, but in an upcoming
// release we will extend this support to include locking atomics as well.
// Atomics of all the usual built-in C types are lock-free, including long
// long on 32-bit x86. We will continue to define the __STDC_NO_ATOMICS__ macro
// even under /experimental:c11atomics until locking atomics are implemented.

prh_static_assert(prh_alignof(prh_atom_i32) == prh_alignof(atomic_int));
prh_static_assert(prh_alignof(prh_atom_r32) == prh_alignof(atomic_uint));
prh_static_assert(prh_alignof(prh_atom_imt) == prh_alignof(atomic_intptr_t));
prh_static_assert(prh_alignof(prh_atom_raw) == prh_alignof(atomic_uintptr_t));
prh_static_assert(prh_alignof(prh_atom_ptr) == prh_alignof(atomic_uintptr_t));

prh_static_assert(sizeof(prh_atom_i32) == sizeof(atomic_int));
prh_static_assert(sizeof(prh_atom_r32) == sizeof(atomic_uint));
prh_static_assert(sizeof(prh_atom_imt) == sizeof(atomic_intptr_t));
prh_static_assert(sizeof(prh_atom_raw) == sizeof(atomic_uintptr_t));
prh_static_assert(sizeof(prh_atom_ptr) == sizeof(atomic_uintptr_t));

#define prh_atom_r32_read prh_stdc_atom_r32_read
#define prh_atom_r32_write prh_stdc_atom_r32_write
#define prh_atom_r32_add prh_stdc_atom_r32_add
#define prh_atom_r32_compare_exchange prh_stdc_atom_r32_compare_exchange
#define prh_atom_raw_read prh_stdc_atom_raw_read
#define prh_atom_raw_write prh_stdc_atom_raw_write
#define prh_atom_raw_add prh_stdc_atom_raw_add
#define prh_atom_raw_compare_exchange prh_stdc_atom_raw_compare_exchange

prh_inline prh_r32 prh_stdc_atom_r32_read(const prh_atom_r32 *a)
{
    return atomic_load((atomic_uint *)a);
}

prh_inline prh_r32 prh_stdc_atom_r32_write(prh_atom_r32 *a, prh_r32 b)
{
    return atomic_exchange((atomic_uint *)a, b);
}

prh_inline prh_r32 prh_stdc_atom_r32_add(prh_atom_r32 *a, prh_r32 b)
{
    return atomic_fetch_add((atomic_uint *)a, b);
}

prh_inline bool prh_stdc_atom_r32_compare_exchange(prh_atom_r32 *a, prh_r32 old_value, prh_r32 new_value)
{
    return atomic_compare_exchange_strong((atomic_uint *)a, (unsigned int *)&old_value, new_value);
}

prh_inline prh_raw prh_stdc_atom_raw_read(const prh_atom_raw *a)
{
    return (prh_raw)atomic_load((atomic_uintptr_t *)a);
}

prh_inline prh_raw prh_stdc_atom_raw_write(prh_atom_raw *a, prh_raw b)
{
    return (prh_raw)atomic_exchange((atomic_uintptr_t *)a, b);
}

prh_inline prh_raw prh_stdc_atom_raw_add(prh_atom_raw *a, prh_raw b)
{
    return atomic_fetch_add((atomic_uintptr_t *)a, b);
}

prh_inline bool prh_stdc_atom_raw_compare_exchange(prh_atom_raw *a, prh_raw old_value, prh_raw new_value)
{
    return atomic_compare_exchange_strong((atomic_uintptr_t *)a, (uintptr_t *)&old_value, new_value);
}

#endif // prh_impl_stdc_atomic

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///
/// SDL3 ATOMIC INTERFACE
///
///

#if defined(prh_impl_sdl3_atomic)
#include <SDL3/SDL_atomic.h>

// https://wiki.libsdl.org/SDL3/CategoryAtomic
// https://wiki.libsdl.org/SDL3/SDL_MemoryBarrierRelease
// https://preshing.com/20120913/acquire-and-release-semantics/
// https://learn.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming

prh_static_assert(prh_alignof(prh_atom_i32) == prh_alignof(SDL_AtomicInt));
prh_static_assert(prh_alignof(prh_atom_r32) == prh_alignof(SDL_AtomicU32));
prh_static_assert(prh_alignof(prh_atom_imt) == prh_alignof(void *));
prh_static_assert(prh_alignof(prh_atom_raw) == prh_alignof(void *));
prh_static_assert(prh_alignof(prh_atom_ptr) == prh_alignof(void *));

prh_static_assert(sizeof(prh_atom_i32) == sizeof(SDL_AtomicInt));
prh_static_assert(sizeof(prh_atom_r32) == sizeof(SDL_AtomicU32));
prh_static_assert(sizeof(prh_atom_imt) == sizeof(void *));
prh_static_assert(sizeof(prh_atom_raw) == sizeof(void *));
prh_static_assert(sizeof(prh_atom_ptr) == sizeof(void *));

#define prh_memory_barrier_write_release() SDL_MemoryBarrierRelease()
#define prh_memory_barrier_read_acquire() SDL_MemoryBarrierAcquire()

#define prh_atom_r32_read prh_sdl3_atom_r32_read
#define prh_atom_r32_write prh_sdl3_atom_r32_write
#define prh_atom_r32_add prh_sdl3_atom_r32_add
#define prh_atom_r32_compare_exchange prh_sdl3_atom_r32_compare_exchange
#define prh_atom_raw_read prh_sdl3_atom_raw_read
#define prh_atom_raw_write prh_sdl3_atom_raw_write
#define prh_atom_raw_add prh_sdl3_atom_raw_add
#define prh_atom_raw_compare_exchange prh_sdl3_atom_raw_compare_exchange

prh_inline prh_r32 prh_sdl3_atom_r32_read(const prh_atom_r32 *a)
{
    return SDL_GetAtomicU32((SDL_AtomicU32 *)a);
}

prh_inline prh_r32 prh_sdl3_atom_r32_write(prh_atom_r32 *a, prh_r32 b)
{
    return SDL_SetAtomicU32((SDL_AtomicU32 *)a, b);
}

prh_inline prh_r32 prh_sdl3_atom_r32_add(prh_atom_r32 *a, prh_r32 b)
{
    return SDL_AddAtomicU32((SDL_AtomicU32 *)a, b);
}

prh_inline bool prh_sdl3_atom_r32_compare_exchange(prh_atom_r32 *a, prh_r32 old_value, prh_r32 new_value)
{
    return SDL_CompareAndSwapAtomicU32((SDL_AtomicU32 *)a, old_value, new_value);
}

prh_inline prh_raw prh_sdl3_atom_raw_read(const prh_atom_raw *a)
{
    return (prh_raw)SDL_GetAtomicPointer((void **)a);
}

prh_inline prh_raw prh_sdl3_atom_raw_write(prh_atom_raw *a, prh_raw b)
{
    return (prh_raw)SDL_SetAtomicPointer((void **)a, (void *)b);
}

prh_inline bool prh_sdl3_atom_raw_compare_exchange(prh_atom_raw *a, prh_raw old_value, prh_raw new_value)
{
    return SDL_CompareAndSwapAtomicPointer((void **)a, (void *)old_value, (void *)new_value);
}

#if prh_imt_bits == 64
prh_inline prh_raw prh_sdl3_atom_raw_add(prh_atom_raw *a, prh_raw b)
{
    prh_raw old_value = prh_sdl3_atom_raw_read(a);
    while (!prh_sdl3_atom_raw_compare_exchange(a, old_value, old_value + b))
    {
        old_value = prh_sdl3_atom_raw_read(a);
    }
    return old_value;
}
#else
prh_inline prh_raw prh_sdl3_atom_raw_add(prh_atom_raw *a, prh_raw b)
{
    return SDL_AddAtomicU32((SDL_AtomicU32 *)a, b);
}
#endif

#endif // prh_impl_sdl3_atomic

#endif // prh_multiple_time_include
#endif // prh_atomic_include

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   THREAD INTREFACE
////
////

// 线程局部存储区，动态 TLS 和静态 TLS。DLL 也可以使用线程局部存储，一般来说，这两项技术在创建 DLL
// 的时候更加有用，因为 DLL 通常并不知道它们被链接到的应用程序的结构是什么样的。
//
// 动态 TLS，应用程序通过调用一组4个函数来使用动态 TLS，这些函数实际上最经常为 DLL 所使用。每个线程
// 都有多个 TLS 槽位，每个槽位保存一个 PVOID 的值，Microsoft 保证至少有 TLS_MINIMUM_AVAILABLE 个槽位
// 可供使用，该值在 winnt.h 中被定义为 64，系统会在需要时分配更多的 TLS 元素，最多可达 1000 多个，这
// 对任何应用程序来说应该足够。系统中的每个进程都有一组是否使用的标志（in-use flag），每个标志表示
// FREE 或 INUSE，表示该 TLS 元素是否正在使用。
//
// DWORD TlsAlloc(); // 分配一个 TLS 元素，返回 TLS 元素的索引值
// BOOL TlsFree(DWORD index); // 当不再需要预订的 TLS 元素时，调用该函数释放
//
// 如果 TlsAlloc 无法在列表中找到一个 FREE 标志的 TLS 元素，那么它会返回 TLS_OUT_OF_INDEXES。当系统
// 创建一个线程的时候，会分配 TLS_MINIMUM_AVAILABLE 个 PVOID 值，将它们都初始化为 0，并与线程关联起
// 来。每个线程都有自己的 PVOID 数组，数组中的每个 PVOID 可以保存任意值。
//
// 将信息保存到 PVOID 数组之前，我们必须知道数组中的哪个索引可供使用，这正是 TlsAlloc 的目的。本质上
// 讲，TlsAlloc 为我们预定了一个索引。
//
// BOOL TlsSetValue(DWORD index, PVOID value); // 将值保存到 TLS 元素中
// PVOID TlsGetValue(DWORD index); // 从 TLS 中取回对应值
//
// 当一个线程调用 TlsSetValue 的时候，会修改自己的数组，但它无法修改另一个线程的 TLS 值。调用时应该
// 总是传入 TlsAlloc 返回的索引，Microsoft 为了效率实现时牺牲了错误检查，即使传入的不是由 TlsAlloc
// 分配的索引值，系统仍然会将值保存到线程的数组中去。
//
// 通常，如果 DLL 要使用 TLS，那它会在 DllMain 函数处理 DLL_PROCESS_ATTACH 时调用 TlsAlloc，在处理
// DLL_PROCESS_DETACH 时调用 TlsFree。而 TlsSetValue 和 TlsGetValue 的调用则最有可能发生在 DLL 所提
// 供的其他函数中。另一种方法时直到需要时才添加，避免浪费仅有的 TLS 资源。
//
// 前面介绍 TlsAlloc 的时候，已经介绍了它 99% 的工作内容，为了帮助读者理解剩下的 1%，看下面的代码：
//      PVOID some_value;
//      DWORD tls_index = TlsAlloc();
//      TlsSetValue(tls_index, (PVOID)12345);
//      TlsFree(tls_index);
//      tls_index = TlsAlloc(); // 假设分配的是同一个 TLS 元素
//      some_value = TlsGetValue(tls_index);
//
// 这段代码执行后，some_value 包含什么值？答案是 0，TlsAlloc 在返回之前，会遍历进程种的每个线程，并
// 根据新分配的索引，在每个线程的数组种把对应的元素设为 0。这是一件好事，因为程序可能会调用 LoadLibrary
// 来载入一个 DLL，而该 DLL 可能会调用 TlsAlloc 分配索引，然后可能调用 FreeLibrary 卸载 DLL，这时候
// DLL 调用 TlsFree 释放索引，但谁知道 DLL 的代码在线程的 TLS 元素种存放了什么值呢？接着相同的操作
// 加载和释放另一个 DLL，假设分配了相同的 TLS 元素，那么就可能取得老的值，代码执行可能会不正确，因为
// 程序可能根据 TLS 元素的值是否为空来判断是否设置了内容。
//
// 静态 TLS。与动态 TLS 相似，静态 TLS 也将数据与线程关联起来。但是，由于使用的时候不必在代码种调用
// 任何函数，因此静态 TLS 更容易使用。__declspec(thread) DWORD tls_value = 0;
//
// __declspec(thread) 是 Microsoft 为 Visual C++ 编译器添加的一个修饰符，它告诉编译器应该在可执行文
// 件或 DLL 文件种，把对应的变量放到它自己的段中。该变量必须是全局变量或者是静态变量，既可以在函数内，
// 也可以在函数外。当编译器对程序进行编译的时候，会将所有 TLS 变量放到它们自己的段中，这个段名为 .tls，
// 链接器会将所有对象模块中的 .tls 段合并为一个大的 .tls 段，并将它保存到生成的可执行文件或 DLL 文件
// 中。
//
// 为了让 TLS 能够正常工作，操作系统也必须参与进来，当系统将应用程序载入到内存时，会查看可执行文件中
// 的 .tls 段，并分配一块足够大的内存来保存所有的静态 TLS 变量。每当应用程序中的代码引用这些变量时，
// 相应的引用会被解析到刚分配的这块内存中的一个位置。因此，编译器必须生成额外的代码来引用静态 TLS 变
// 量，这使得应用程序不仅变得更大，而且执行也更慢。在 x86 CPU 上，每次引用一个静态 TLS 变量会生成三
// 条额外的机器指令。
//
// 如果进程创建了另一个线程，那么系统会获知这一情况并自动分配另一块内存来保存新线程的静态 TLS 变量。
// 新线程只能访问自己的静态 TLS 变量，它无法访问属于任何其他线程的 TLS 变量。
//
// 这基本上就是静态 TLS 的工作方式，现在我们考虑有 DLL 的情况。我们的应用程序可能会用到一些静态 TLS
// 变量，而且我们链接的 DLL 也想要使用静态 TLS 变量。当系统载入应用程序的时候，会首先确定应用程序的
// .tls 段的大小，并将它与应用程序链接的所有 DLL 的 .tls 段的大小相加。当系统在创建线程的时候，会自
// 动分配一块足够大的内存来保存应用程序和所有隐式链接的 DLL 需要的 TLS 变量。
//
// 但是，如果应用程序调用 LoadLibrary 来链接一个 DLL，而且该 DLL 包含了静态 TLS 变量，这时会发生什么。
// 为了给新 DLL 提供它需要的额外 TLS 内存，系统必须查看进程中所有已有的线程，并扩大它们的 TLS 内存块。
// 另外，如果应用程序调用 FreeLibrary 来释放一个 DLL，而且该 DLL 包含了静态 TLS 变量，那么与进程中的
// 每个线程相关联的内存块应该相应的缩减。好消息是，Windows Vista 对此提供了完全的支持。

// 应用程序动态加载 DLL 并访问其中定义的静态 TLS 变量。应用程序拿到 DLL 静态 TLS 变量地址的方式是：
// 从 TEB 的 ThreadLocalStoragePointer取出每线程的模块指针数组 → 用 DLL 自己的 __tls_index 做下标 →
// 得到本模块本线程的数据块 → 加上编译期确定的变量偏移。索引和每线程数据块都由加载器在模块加载/线程
// 创建时填好，用户代码永远只负责最后那次间接寻址。整条链路分编译期、加载期、运行期三部分。
//
// 编译期：
//  1.  DLL 定义的静态 TLS 变量被放进 DLL 的 .tls 段
//  2.  链接生成 DLL 可执行文件时，创建 PE 的 TLS 目录（IMAGE_TLS_DIRECTORY），关键字段：
//          * AddressOfIndex 指向该 DLL 模块的 __tls_index，一个 32 位索引变量，索引 ThreadLocalStoragePointer 数组元素
//          * StartAddressOfRawData / EndAddressOfRawData 表示 .tls 段原始数据的范围
//          * AddressOfZeroFill 需要清零补足的尾部大小
//  3.  应用程序访问 DLL 静态 TLS 变量的代码，x64 完全类似，只是偏移和指针宽度不同
//          mov eax, dword ptr fs:[0x2c] // TEB.ThreadLocalStoragePointer
//          mov ecx, dword ptr [__tls_index] // 读取 __tls_index 变量
//          mov edx, dword ptr [eax + ecx * 4] // 找到静态 TLS 变量区域
//          lea eax, [edx + tls_var_offset] // 在该区域中根据变量偏移找到特定变量
//
// 加载期：
//  1.  进程主 EXE 的 __tls_index 固定为 0，之后每加载一个带 .tls 的 DLL，ntdll（LdrpAllocateTls，
//      LdrpHandleTlsData）分配一个递增索引并写入该 DLL 的 __tls_index 变量
//  2.  对进程启动时就加载的模块，加载器为每个线程做三件事
//      * 按对齐要求分配一块每线程内存
//      * 把 .tls 原始数据复制进去，各线程各一份
//      * 把块地址填进该线程 TEB 的 TLS 指针数组 __tls_index 槽位
//  3.  之后新线程创建时，线程初始化代码同样照此办理
//
// 上面的机制成立的前提是 "线程创建时已存在该模块"。如果 DLL 是运行期通过 LoadLibrary 动态加载的，
// 只有发起 LoadLibrary 的那个线程的槽位会被初始化，其他线程访问该 DLL 的静态 TLS 变量时，拿到的
// 槽位是空导致访问违规。这就是著名的限制，可动态加载的 DLL 必须使用动态 TLS，不能用定义静态 TLS，
// 静态 TLS 基本只适合进程启动时加载的模块。
//
// 因此，一个应用程序如果想要访问 DLL 定义的静态 TLS 变量，最好的方式是静态链接该 DLL，然后访问其中
// 的静态 TLS 变量。另外不能静态链接的 DLL 最好不要定义静态 TLS 变量，但可以访问应用程序或静态链接的
// DLL 中定义的静态 TLS 变量。

// 在 32 位和 64 位模式下，不允许修改任何段寄存器，即使是临时修改也不行。CS、DS、ES 和 SS 全都指向平
// 坦段组（flat segment group）。FS 在 Windows 中用于线程环境块（TEB），在 Linux 中用于线程局部数据。
// GS 在 64 位 Windows 中用于处理器控制区（KPCR）。在 32 位 Windows 中它未被使用但被保留。在 32 位
// Linux 中它大概也未被使用。
//
//          平台        32 位程序                               64 位程序
//          Windows     FS → TEB（线程环境块）                  FS → TEB
//          Linux       FS → TCB（线程控制块/TLS，glibc）       FS → TCB，GS 不用
//
//      Windows
//          x86（32 位）：FS 指向当前线程的 TEB：
//                  FS:[0x00] — SEH 异常处理链头指针
//                  FS:[0x18] — TEB 自身指针
//                  FS:[0x2C] — TLS 数组
//                  FS:[0x30] — PEB（进程环境块）
//          x64（64 位）：
//                  FS:[0x30] - TEB
//                  FS:[0x60] - PEB
//
//      WinDbg - !teb，还可以使用 dg @fs 查看寄存器值，r @fs, @rax
//          TEB at 0000005662453000
//              [0x00] ExceptionList:        0000000000000000
//              [0x08] StackBase:            00000056623a0000
//              [0x10] StackLimit:           000000566239c000
//              [0x18] SubSystemTib:         0000000000000000
//              [0x20] FiberData:            0000000000001e00
//              [0x28] ArbitraryUserPointer: 0000000000000000
//              [0x30] Self:                 0000005662453000
//              [0x38] EnvironmentPointer:   0000000000000000
//              [0x40] ClientId:             0000000000007374 . 000000000000413c
//              [0x50] RpcHandle:            0000000000000000
//              [0x58] Tls Storage:          0000017ef3608360
//              [0x60] PEB Address:          0000005662452000
//              [0x68] LastErrorValue:       0
//              [0x70] LastStatusValue:      c0000034
//              [0x78] Count Owned Locks:    0
//              [0x80] HardErrorMode:        0
//
//      Linux
//          i386（32 位）：
//              glibc 用 FS 指向 TLS/TCB（struct pthread，FS:[0] 是自指指针）
//              裸程序/静态无 TLS 时 FS 可以是 0。
//          x86-64（64 位）：同样是 FS 指向 TCB，两个经典位置：
//              FS:[0x00] — TCB 自指
//              FS:[0x28] — stack canary（栈保护金丝雀，__stack_chk_fail 就靠它）
//
// Windows：用户态没有任何合法途径改变 FS/GS 基址；TLS 只能用 TlsSetValue 等在 TEB 槽位里存值，段基
// 址由内核/加载器掌控。Linux：可以通过 arch_prctl(ARCH_SET_FS)（或 FSGSBASE 开启后的 wrfsbase）把
// FS 指到任意地址，技术上"可以改"，但改了就等着崩：TLS 自指失效、栈金丝雀读不到、errno 全乱，马上
// 触发段错误或 stack smashing detected。

// 16 位段寄存器 FS GS 本身只是选择子（selector），真正的段基址（一个32/64位指针）藏在 CPU 内部的隐
// 藏段描述符缓存（descriptor cache）里，程序代码看不见也改不了。
//
// 全局描述符表（Global Descriptor Table, GDT）和局部描述符表（Local Descriptor Table, LDT）是两张存
// 放段描述符的表。每个描述符表项 8 字节（64 位），描述一个段的：
//      1.  基址（Base，32 位，分散在字节 2–4 和字节 7 中）
//      2.  界限（Limit，20 位）
//      3.  属性：类型（代码/数据/系统）、存在位 P、特权级 DPL、粒度 G、32/64 位标志等
//
// GDT 全局一张，由 CPU 的 GDTR 寄存器（存表的基址+界限）指向，操作系统启动时建好。LDT 每个进程可以
// 有一张（现代 OS 基本不用），它的位置也由 GDT 中一个"系统描述符"描述，由 LDTR 寄存器指向。
//
// Windows 内核为每个线程在 GDT 中建好一个段描述符（基址 = 该线程 TEB 的地址），线程切换时 mov fs,
// 0x3B（或类似选择子）这种指令会让 CPU 自动从 GDT 读出基址填进隐藏部分。之后每条 FS:[x] 访问都用隐
// 藏基址计算地址，16 位可见部分自始至终只当描述符表项的"索引"用。
//
// 所以 mov eax, fs:[0x18] 的意思是：从"TEB 起始地址 + 0x18"处读出一个 32 位指针，而这个指针的值恰好
// 指回 TEB 自己。设计这个自指字段的用途是：代码里随时可以用一条指令拿到 TEB 的地址，而不必知道它具
// 体在哪。
//
// 16 位段寄存器如何索引表项。段寄存器可见的 16 位叫选择子（Selector），结构如下：
//              [15                              3 |  2 | 1 0 ]
//              [         Index（13 位）           │ TI │ RPL ]
//
//      * Index（13 位） 表内索引 0 ~ 8191，选择 GDT 或 LDT 中 8192 个描述符中的一个
//      * TI（1 位） 选哪张表 0=GDT 1=LDT
//      * RPL（2 位）请求特权级 0~3 (3=用户态）
//
// 64 位模式下，FS 基址根本不走 GDT 描述符的 32 位 base 字段。这正是 x86-64 设计里的一个特殊安排，
// Intel SDM 3.4.4 节里有一句话点明了答案："FS.base 和 GS.base 的隐藏描述符寄存器字段在物理上映射到
// MSR，以便装入 64 位实现所支持的所有地址位。"
//
//  1.  隐藏的 FS 基址是 64 位的，来自 MSR，长模式下有三个专用 MSR。这些 MSR 是完整的 64 位，装得下
//      任何规范形式的 TEB 地址。Windows 在每次线程切换时，由内核把新线程的 TEB 地址 WRMSR 进 IA32_FS_BASE，
//      这才是 "FS 指向 TEB" 在 64 位下的真实实现。
//
//      MSR                     地址            用途
//      IA32_FS_BASE            0xC0000100      用户态 FS 基址（TEB 就在这）
//      IA32_GS_BASE            0xC0000101      用户态 GS 基址（Linux 内核态用它）
//      IA32_KERNEL_GS_BASE     0xC0000102      内核 GS 基址（Windows KPCR），经 SWAPGS 切换
//
//  2.  GDT 描述符的 32 位 base 字段呢？基本被架空了。SDM 同节还有一句："普通段加载（MOV to Sreg 和
//      POP Sreg）装入 FS 和 GS 时，在隐藏部分装入标准的 32 位基址值，高于 32 位的位被清零"。
//
//      也就是说：
//      * mov fs, ax —— CPU 从描述符取 32 位 base 装入 FS 基址的低 32 位，高位清零，它只能表达 4GB 以内的地址
//      * WRMSR IA32_FS_BASE（CPL=0）—— 写入完整 64 位基址，并校验规范形式（非规范地址触发 #GP）
//
//      所以用户态没有任何手段把 FS 指到 4GB 以上的地方，而内核通过 MSR 不受这个限制。GDT 描述符在这
//      条链路上只剩"选择子必须合法、不能是 null"这类格式检查作用。
//
//  3.  GS 的对称设计：SWAPGS
//
//      进内核时（中断/系统调用）执行 SWAPGS，把当前 GS 基址与 IA32_KERNEL_GS_BASE 互换：用户态 GS
//      换出、内核 KPCR 换入。这就是为什么 x64 Windows 用户态用 GS 指向 TEB（GS:[0x30]），而内核态
//      GS 瞬间变成 KPCR。
//
// 和 32 位对比
//
//                          32 位保护模式                       64 位长模式
//      FS 基址来源         GDT 描述符的 32 位 Base 字段        MSR（IA32_FS_BASE），64 位
//      谁设置的            内核建好 GDT 项 + mov fs, sel       内核 WRMSR（或启用 FSGSBASE 后的 WRFSBASE）
//      用户态能否改基址    不能（只能装合法选择子）            不能（Windows 未开启 FSGSBASE，WRFSBASE 直接 #UD）
//
//      Linux 侧同理：arch_prctl(ARCH_SET_FS, addr) 本质就是内核帮你执行 WRMSR IA32_FS_BASE

// 3.4.1 IA-32e 模式下的逻辑地址转换
//
// 在 IA-32e 模式下，Intel 64 处理器使用上述步骤将逻辑地址转换为线性地址。在 64 位模式下，段的偏移量
// 和基地址是 64 位而不是 32 位。线性地址格式也是 64 位宽，并受线性地址预处理（linear-address
// pre-processing）约束（见第 4 章）。每个代码段描述符提供一个 L 位。该位允许代码段执行 64 位代码或
// 传统的 32 位代码。
//
// 3.4.2 段选择子
//
// 段选择子（segment selector）是一个 16 位的段标识符。它不直接指向段，而是指向定义该段的段描述符。
// 段选择子包含以下各项：
//
//  1.  索引（Index，第 3 到 15 位）—— 在 GDT 或 LDT 中选择 8192 个描述符之一。处理器将索引值乘以 8
//      （段描述符的字节数），并将结果加到 GDT 或 LDT 的基地址上（分别来自 GDTR 或 LDTR 寄存器）。
//  2.  TI（表指示符，table indicator）标志（第 2 位）—— 指定要使用的描述符表，清除此标志选择 GDT，
//      设置此标志选择当前 LDT。
//  3.  请求特权级（Requested Privilege Level RPL，第 0 和 1 位）—— 指定选择子的特权级。特权级范围
//      从 0 到 3，其中 0 为最高特权级。RPL 与执行程序（或任务）的 CPL、以及段选择子所指描述符的描
//      述符特权级（DPL）之间的关系，见 6.5 节"特权级"。
//
// GDT 的第一项不被处理器使用。指向 GDT 这一项的段选择子（即索引为 0 且 TI 标志为 0 的段选择子）被
// 用作"空段选择子（null segment selector）"。当段寄存器（CS 或 SS 寄存器除外）被装入空选择子时，处
// 理器不产生异常。然而，当用持有空选择子的段寄存器访问内存时，处理器会产生异常。空选择子可用于初始
// 化未使用的段寄存器。向 CS 或 SS 寄存器装入空段选择子会导致产生一般保护异常（#GP）。
//
// 段选择子作为指针变量的一部分对应用程序可见，但选择子的值通常由链接编辑器（link editor）或链接加载
// 器（linking loader）分配或修改，而不是应用程序。
//
// 3.4.3 段寄存器
//
// 为减少地址转换时间和编程复杂性，处理器提供最多可保存 6 个段选择子的寄存器。每个段寄存器支持一种特
// 定类型的内存引用（代码、栈或数据）。几乎任何程序执行要发生，至少代码段（CS）、数据段（DS）和栈段
// （SS）寄存器必须装入有效的段选择子。处理器还额外提供三个数据段寄存器（ES、FS 和 GS），可用于让当
// 前执行的程序（或任务）使用额外的数据段。
//
// 程序要访问一个段，该段的段选择子必须已装入某个段寄存器。因此，虽然一个系统可以定义数千个段，但只
// 有 6 个可立即使用。其他段可通过在程序执行期间将其段选择子装入这些寄存器而变得可用。
//
//       可见部分           隐藏部分
//      [Segment Selector | Base Address, Limit, Access Information] CS
//      [                 |                                        ] SS
//      [                 |                                        ] DS
//      [                 |                                        ] ES
//      [                 |                                        ] FS
//      [                 |                                        ] GS
//                  图 3-7. 各段寄存器
//
// 每个段寄存器都有"可见"部分（16位的选择子）和"隐藏"部分（基地址、段界限、访问信息）。隐藏部分有时
// 称为"描述符缓存"或"影子寄存器"。当段选择子装入段寄存器的可见部分时，处理器同时把该段选择子所指段
// 描述符中的基地址、段界限和访问控制信息装入段寄存器的隐藏部分。缓存在段寄存器中（可见和隐藏）的信
// 息使处理器无需额外总线周期去读取段描述符中的基地址和界限即可转换地址。在多个处理器可访问同一描述
// 符表的系统中，软件有责任在描述符表被修改时重新加载段寄存器。如果不这样做，段寄存器中缓存的旧段描
// 述符可能会在内存中的版本被修改之后仍被使用。
//
// 提供了两类加载指令用于加载段寄存器：
//  1.  直接加载指令，如 MOV、POP、LDS、LES、LSS、LGS 和 LFS 指令。这些指令显式引用段寄存器。
//  2.  隐式加载指令，如 CALL、JMP 和 RET 指令的远指针（far pointer）版本，SYSENTER 和 SYSEXIT 指令，
//      以及 IRET、INT n、INTO、INT3 和 INT1 指令。这些指令在运行过程中附带地改变 CS 寄存器（有时还
//      有其他段寄存器）的内容。
//
// MOV 指令还可用于把段寄存器的可见部分存储到通用寄存器中。
//
// 3.4.4 IA-32e 模式下的段加载指令
//
// 由于 ES、DS 和 SS 段寄存器在 64 位模式下不使用，段描述符寄存器中的字段（基址、界限和属性）被忽略。
// 某些形式的段加载指令也无效（例如 LDS、POP ES）。引用 ES、DS 或 SS 段的地址计算被当作段基址为零处
// 理。处理器执行线性地址预处理（第 4 章），而不执行界限检查。模式切换不改变段寄存器或相关描述符寄存
// 器的内容。在 64 位模式执行期间，这些寄存器也不会改变，除非执行显式的段加载。
//
// 为了为应用程序设置兼容模式，段加载指令（MOV to Sreg、POP Sreg）在 64 位模式下正常工作。从系统描述
// 符表（GDT 或 LDT）中读取一个表项，装入段寄存器的隐藏部分。描述符寄存器的基址、界限和属性字段全部
// 被加载。然而，数据段和栈段的选择子及描述符寄存器的内容被忽略。
//
// 在 64 位模式下使用 FS 和 GS 段超越（segment override）时，它们各自的基地址用于线性地址计算：(FS
// 或 GS).base + index + displacement。FS.base 和 GS.base 随后被扩展到实现所支持的完整线性地址大小。
// 由此产生的有效地址计算可能跨越正地址和负地址回绕；所得地址受线性地址预处理约束。
//
// 在 64 位模式下，使用 FS 段和 GS 段超越的内存访问不检查运行时界限，也不进行属性检查。装入 FS 和 GS
// 的普通段加载（MOV to Sreg 和 POP Sreg）在段寄存器隐藏部分装入标准的 32 位基址值。基地址高于标准
// 32 位的各位被清零，以便为使用少于 64 位的实现保持一致性。
//
// FS.base 和 GS.base 的隐藏描述符寄存器字段在物理上映射到 MSR，以便装入 64 位实现所支持的所有地址位。     *** 64 位模式下，段寄存器隐藏部分基地址只会装入低 32 位，高位被清零
// CPL = 0 的软件（特权软件）可以使用 WRMSR 把所有支持的线性地址位装入 FS.base 或 GS.base。写入 64 位          对应的 64 位 FS.base 和 GS.base 映射在 MSR 中
// FS.base 和 GS.base 寄存器的地址必须是规范形式（canonical form）。试图向这些寄存器写入非规范地址的
// WRMSR 指令会导致 #GP 故障。见 4.5.2 节。
//
// 在兼容模式下，无论装入隐藏描述符寄存器基址字段高 32 位线性地址的值是什么，FS 和 GS 超越都按 32 位
// 模式定义的行为运作。兼容模式在计算有效地址时忽略高 32 位。
//
// 新的 64 位模式指令 SWAPGS 可用于加载 GS 基址。SWAPGS 将来自 IA32_KERNEL_GS_BASE MSR 的内核数据结
// 构指针与 GS 基址寄存器交换。随后内核可以在普通内存引用上使用 GS 前缀来访问内核数据结构。试图（用
// WRMSR）向 IA32_KERNEL_GS_BASE MSR 写入非规范值会导致 #GP 故障；见 4.5.2 节。
//
// 3.4.5 段描述符
//
// 段描述符是 GDT 或 LDT 中的一种数据结构，向处理器提供段的大小和位置，以及访问控制和状态信息。段描
// 述符通常由编译器、链接器、加载器或操作系统或执行体（executive）创建，而不是应用程序。图 3-8 说明
// 了所有类型段描述符的通用描述符格式。
//
//        [31        24| 23| 22  | 21| 20  |19                 16| 15|14 13| 12|11   8|7          0]
//        [ Base 31:24 | G | D/B | L | AVL | Setment Limit 19:16 | P | DPL | S | Type | Base 23:16 ]
//
//        [31                16|15                                                                0]
//        [ Base Address 15:00 | Segment Limit 15:00                                               ]
//
//                  L       - 64-bit code segment (IA-32e mode only)
//                  AVL     - available for use by system software
//                  BASE    - segment base address
//                  D/B     - default operation size (0 - 16-bit segment, 1 - 32-bit segment)
//                  DPL     - descriptor privilege level
//                  G       - granularity
//                  LIMIT   - segment limit
//                  P       - segment present
//                  S       - descriptor type (0 system, 1 code or data)
//                  TYPE    - segment type
//
//                  图 3-8. 段描述符格式
//
//          [31       16 | 15|14 13| 12|11   8|7         0]
//          [ Available  | 0 | DPL | S | Type | Available ]
//
//          [31                                          0]
//          [ Available                                   ]
//
//          图 3-9. 当 Segment-Present 清位时的段描述符格式
//
// 段描述符中的标志和字段如下：
//  1.  段界限字段（Segment limit field）
//      指定段的大小。处理器把两个段界限字段拼成一个 20 位的值。根据 G（粒度）标志的设置，处理器以两
//      种方式之一解释段界限。处理器以两种不同的方式使用段界限，取决于段是向上扩展段还是向下扩展段。
//      关于段类型的更多信息见 3.4.5.1 节"代码段和数据段描述符类型"。对于向上扩展段，逻辑地址中的偏移
//      量范围可从 0 到段界限。大于段界限的偏移量产生一般保护异常（#GP，对 SS 以外的所有段）或栈故障
//      异常（#SS，对 SS 段）。对于向下扩展段，段界限功能相反：偏移量范围可从段界限加 1 到 FFFFFFFFH
//      或 FFFFH，取决于 B 标志的设置。小于或等于段界限的偏移量产生一般保护异常或栈故障异常。减小向
//      下扩展段的段界限字段值会在段地址空间的底部而不是顶部分配新内存。IA-32 架构的栈总是向下增长，
//      这使得这种机制便于实现可扩展栈。
//          * 如果粒度标志清零，段大小范围可为 1 字节到 1 MByte，以字节递增
//          * 如果粒度标志置位，段大小范围可为 4 KByte 到 4 GByte，以 4 KByte 递增
//  2.  基地址字段（Base address fields）
//      定义段中字节 0 在 4 GByte 线性地址空间中的位置。处理器把三个基地址字段拼成一个 32 位的值。段
//      基地址应按 16 字节边界对齐。虽然不要求 16 字节对齐，但该对齐使程序能通过对齐代码和数据到 16
//      字节边界来获得最佳性能。
//  3.  类型字段（Type field）
//      指示段或门类型，并指定可对段进行的访问类型以及增长方向。该字段的解释取决于描述符类型标志指定
//      的是应用程序（代码或数据）描述符还是系统描述符。类型字段对代码、数据和系统描述符的编码不同
//      （见图 6-1）。
//  4.  S（描述符类型）标志
//      指定段描述符用于系统段（S 标志清零）还是代码或数据段（S 标志置位）。
//  5.  DPL（描述符特权级）字段
//      指定段的特权级。特权级范围从 0 到 3，其中 0 为最高特权级。DPL 用于控制对段的访问。DPL 与执行
//      代码段的 CPL 以及段选择子的 RPL 之间的关系见 6.5 节"特权级"。
//  6.  P（段存在 present）标志
//      指示段是否在内存中（置位）或不在内存中（清零）。如果此标志清零，当指向该段描述符的段选择子装
//      入段寄存器时，处理器产生段不存在异常（#NP）。内存管理软件可以使用此标志来控制在给定时间哪些
//      段实际加载到物理内存中。它提供了除分页之外管理虚拟内存的一种手段。
//      图 3-9 显示了段存在标志清零时段描述符的格式。当此标志清零时，操作系统或执行体可以自由使用标
//      有"可用（Available）"的位置来存储自己的数据，例如关于缺失段位置的信息。
//  7.  D/B（默认操作大小/默认栈指针大小和/或上界）标志
//      根据段描述符是可执行代码段、向下扩展数据段还是栈段，执行不同的功能。对于 32 位代码和数据段，
//      此标志应始终置 1；对于 16 位代码和数据段，应清零。
//      可执行代码段。此标志称为 D 标志，指示段中指令引用的有效地址和操作数的默认长度。如果标志置位，
//      假定为 32 位地址和 32 位或 8 位操作数；如果清零，假定为 16 位地址和 16 位或 8 位操作数。指令
//      前缀 66H 可用于选择非默认的操作数大小，前缀 67H 可用于选择非默认的地址大小。
//      栈段（SS 寄存器指向的数据段）。此标志称为 B（big）标志，它指定隐式栈操作（如压栈、弹栈和调用）
//      所使用的栈指针大小。如果标志置位，使用 32 位栈指针，保存在 32 位 ESP 寄存器中；如果清零，使用
//      16 位栈指针，保存在 16 位 SP 寄存器中。如果栈段被设置为向下扩展数据段（下一段描述），B 标志
//      还指定栈段的上界。
//      向下扩展数据段。此标志称为 B 标志，它指定段的上界。如果标志置位，上界为 FFFFFFFFH（4 GByte）；
//      如果清零，上界为 FFFFH（64 KByte）。
//  8.  G（粒度）标志
//      决定段界限字段的缩放方式。当粒度标志清零时，段界限以字节为单位解释；当标志置位时，段界限以 4
//      KByte 为单位解释。此标志不影响基地址的粒度，基地址始终是字节粒度。当粒度标志置位时，在将偏移
//      量与段界限比较时不检查偏移量的低 12 位。例如，当粒度标志置位时，界限为 0 的结果是偏移量 0 到
//      4095 均有效。
//  9.  L（64 位代码段）标志
//      在 IA-32e 模式下，段描述符第二个双字的第 21 位指示代码段是否包含原生 64 位代码。值为 1 表示
//      该代码段中的指令在 64 位模式下执行；值为 0 表示该代码段中的指令在兼容模式下执行。如果 L 位置
//      位，则 D 位必须清零。第 21 位在 IA-32e 模式之外（或用于数据段）不使用。因为如果当前 CS 的 L
//      位置位，激活 IA-32e 模式将导致故障（见 11.8.5 节），所以在 IA-32e 模式之外运行的软件应避免从
//      设置了 L 位的描述符加载 CS。
//  10. 可用位和保留位
//      段描述符第二个双字的第 20 位可供系统软件使用。
//
// 3.5.1 段描述符表
//
// 段描述符表是段描述符的数组，描述符表长度可变，最多可包含 8192（2^13）个 8 字节描述符。描述符表有
// 两种：
//  1.  全局描述符表（GDT）
//  2.  局部描述符表（LDT）
//
// 每个系统必须定义一个 GDT，可供系统中所有程序和任务使用。可以选择定义一个或多个 LDT。例如，可以为
// 每个单独运行的任务定义一个 LDT，或者某些或所有任务共享同一个 LDT。
//
// GDT 本身不是一个段，它是线性地址空间中的一种数据结构。GDT 的基线性地址和界限必须装入 GDTR 寄存器
// （见 2.4 节"内存管理寄存器"）。GDT 的基地址应按八字节边界对齐以获得最佳处理器性能。GDT 的界限值以
// 字节表示。与段一样，界限值加到基地址上得到最后一个有效字节的地址。界限值为 0 恰好产生一个有效字节。
// 由于段描述符总是 8 字节长，GDT 的界限应始终是 8 的整数倍减 1（即 8N – 1）。
//
// GDT 中的第一个描述符不被处理器使用。指向这个"空描述符"的段选择子在装入数据段寄存器（DS、ES、FS 或
// GS）时不产生异常，但在试图使用该描述符访问内存时总是产生一般保护异常（#GP）。通过用这个段选择子初
// 始化段寄存器，可以保证对未使用段寄存器的意外引用必然产生异常。
//
// LDT 位于 LDT 类型的系统段中。GDT 必须包含该 LDT 段的段描述符。如果系统支持多个 LDT，每个都必须有
// 单独的段选择子和 GDT 中的段描述符。LDT 的段描述符可位于 GDT 的任何位置。关于 LDT 段描述符类型的信
// 息见 3.5 节"系统描述符类型"。
//
// 通过其段选择子访问 LDT。为消除访问 LDT 时的地址转换，LDT 的段选择子、基线性地址、界限和访问权保存
// 在 LDTR 寄存器中（见 2.4 节"内存管理寄存器"）。
//
// 当 GDTR 寄存器被存储（使用 SGDT 指令）时，一个 48 位的"伪描述符（pseudo-descriptor）"被存入内存。
// 为避免用户模式（特权级 3）中的对齐检查故障，伪描述符应位于奇字地址（即地址 MOD 4 等于 2）。这会使
// 处理器先存储一个对齐的字，再存储一个对齐的双字。用户模式程序通常不存储伪描述符，但用这种方式对齐
// 伪描述符可以避免产生对齐检查故障的可能性。使用 SIDT 指令存储 IDTR 寄存器时应使用相同的对齐。当存
// 储 LDTR 或任务寄存器（分别使用 SLDT 或 STR 指令）时，伪描述符应位于双字地址（即地址 MOD 4 等于0）。
//
//          [47                 16| 15   0]
//          [ 32-bit Base Address | Limit ]
//
//          [79                 16| 15   0]
//          [ 64-bit Base Address | Limit ]
//
//             图 3-11. 各伪描述符格式
//
// 3.5.2 IA-32e 模式下的段描述符表
//
// 在 IA-32e 模式下，段描述符表最多可包含 8192（2^13）个 8 字节描述符。段描述符表中的表项可以是 8 字
// 节。系统描述符扩展为 16 字节（占据两个表项的空间）。GDTR 和 LDTR 寄存器被扩展为保存 64 位基地址。
// 相应的伪描述符为 80 位。以下系统描述符扩展为 16 字节：
//  1.  调用门描述符（call gate descriptors），见 6.8.3.1 节"IA-32e 模式调用门"
//  2.  IDT 门描述符（idt gate descriptors），见 7.14.1 节"64 位模式 IDT"，Interrupt Descriptor Table (IDT)
//  3.  LDT 和 TSS 描述符（ldt and task state segment descriptors），见 9.2.3 节"64 位模式下的 TSS 描述符"

#ifdef __cplusplus
}
#endif
#endif // prh_include_nothing_h

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