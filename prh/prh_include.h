// prh_include.h - public domain - swdayu <github.com/swdayu>
// No warranty implied, use at your own risk.
//
// REVISION HISTORY
//
// VERSION CONVENTIONS
//
// We are following https://semver.org/ as format MAJOR.MINOR.PATCH:
// - Modifying comments does not update the version.
// - PATCH is incremented in case of a bug fix or refactoring without
//   touching the API.
// - MINOR is incremented when new functions and/or types are added in a way
//   that does not break any existing user code. We want to do this in the
//   majority of the situation.
//   If we want to delete a certain function or type in favor of another one
//   we should just add the new function/type and deprecate the old one in a
//   backward compatible way and let them co-exist for a while.
// - MAJOR update should be just a periodic cleanup of the deprecated
//   functions and types without really modifying any existing functionality.
//
// LICENSE
//
// See end of the file for license information.
//
// NAMING CONVENTIONS
//
// Since Pure C does not have any namespaces, we prefix each name of the API
// with the `prh_` or `PRH_` to avoid any potential conflicts with any other
// names in your code.
//
// For the same reason, there is no way to hide implementation file scope
// names due to single-file-style library. They inevitably pollute the global
// namespace. So we choose `prh_impl_` or `PRH_IMPL_` prefix for these names.
//
// Above choices base on the fact that following identifiers are language
// reserved in C/C++:
// 1. all external identifiers that begin with an underscore.
// 2. all identifiers that begin with an underscore followed by a capital
//    letter or by another underscore (these reserved identifiers allow the
//    library to use numerous behind-the-scenes non-external macros and
//    functions).
// 3. in C++, identifiers with a double underscore anywhere are reserved
//    everywhere; in C, only the ones that begin with a double underscore
//    are reserved.
//
// In general, all exposed names are prefixed with `prh_` or `PRH_` depending
// on the case. However, the names prefixed with `prh_impl_` or `PRH_IMPL_`
// are implementation details of the library, the user should not directly
// use them.
//
// PREFIX STRIPPING
//
// The prefixed names sometimes are very annoying and make the code noisy. If
// you know that part of prh_include.h names are not conflict with your code,
// you can define PRH_XXX_STRIP_PREFIX macro to drop related name prefixes.
// For example, define PRH_ARRAY_STRIP_PREFIX can strip off `prh_` prefix
// from `prh_array_*` interfaces.
//
// Not all the names have strippable prefixes. All the redefinable names like
// `prh_realloc` and `PRH_ARRAY_INIT_ELEMS` for instance will retain their
// prefix. All basic names like `prh_i64` and `prh_inline` can not strip off
// the prefix due to they are short names anyway. But you can still #define
// your own names as you need before or after include prh_include.h. All
// implementation names `prh_impl_` and `PRH_IMPL_` are keep their original
// name untouched.
//
// The prefixes are stripped off only on the level of preprocessor. The names
// of the functions in the compiled object file will still retain the `prh_`
// prefix.
//
// If only few specific names cause conflicts for you, you can just #undef
// those names after include prh_include.h since they are macros anyway.
#ifndef PRH_IMPL_INCLUDE_H
#define PRH_IMPL_INCLUDE_H
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
#if defined(NDEBUG) || defined(PRH_RELEASE) // cc no debug, NDEBUG or PRH_RELEASE defined
#undef PRH_DEBUG
#define PRH_DEBUG 0
#elif defined(_DEBUG) || defined(PRH_DEBUG) // cc with debug option
#undef PRH_DEBUG
#define PRH_DEBUG 1
#else // default in debug developing mode
#undef PRH_DEBUG
#define PRH_DEBUG 1
#endif

#undef _DEBUG
#if (PRH_DEBUG == 1)
#define _DEBUG 1
#endif

// architecture
#ifndef prh_arch_bits
#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(_M_ARM64) || defined(__powerpc64__)
    #if defined(__x86_64__) || defined(_M_X64)
        #define prh_arch_x64 1
    #elif defined(__aarch64__) || defined(_M_ARM64)
        #define prh_arch_a64 1
    #endif
    #define prh_arch_bits 64
    #define prh_arch_32 0
    #define prh_arch_64 64
#else
    #if defined(__i386) || defined(_M_IX86)
        #define prh_arch_x86 1
    #elif defined(__arm__) || defined(_M_ARM)
        #define prh_arch_arm 1
    #endif
    #define prh_arch_bits 32
    #define prh_arch_32 32
    #define prh_arch_64 0
#endif
#endif // prh_arch_bits

// byte endian
#ifndef prh_lit_endian
#if defined(prh_arch_x86) || defined(prh_arch_x64) || defined(prh_arch_arm) || defined(prh_arch_a64)
    #define prh_lit_endian 1
    #define prh_big_endian 0
#endif
#ifndef prh_lit_endian
    #define prh_lit_endian 1
    #define prh_big_endian 0
#endif
#endif // prh_lit_endian

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
//
// __GNUC__ __GNUC_MINOR__ __GNUC_PATCHLEVEL__
// /* Test for GCC > 3.2.0 */
// #if __GNUC__ > 3 || (__GNUC__ == 3 && (__GNUC_MINOR__ > 2 ||
//      (__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ > 0)))
// /* Test for GCC > 3.2.0 */
// #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 +
//      __GNUC_PATCHLEVEL__)
// #if GCC_VERSION > 30200

#ifndef PRH_GCC_VERSION
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
#define PRH_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif
#endif

// https://clang.llvm.org/docs/LanguageExtensions.html#builtinmacros
// __clang__ defined when compiling with Clang
// __clang_major__ e.g., the 2 in 2.0.1
// __clang_minor__ e.g., the 0 in 2.0.1
// __clang_patchlevel__ e.g., the 1 in 2.0.1
// __clang_version__ version string, e.g., "1.5 (trunk 102332)"

#ifndef PRH_CLANG_VERSION
#if defined(__clang__) && defined(__clang_major__) && defined(__clang_minor__) && defined(__clang_patchlevel__)
#define PRH_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#endif
#endif

// inline noinline
#ifndef prh_inline
#if defined(_MSC_VER)
    #define prh_inline static __forceinline
    #if _MSC_VER >= 1400 // noinline was introduced in Visual Studio 2005
        #define prh_noinline __declspec(noinline)
    #else
        #define prh_noinline
    #endif
#elif defined(__GNUC__)
    // I've had a bug report where GCC is emitting warnings about functions
    // possibly not being inlineable. This warning happens when the
    // __attribute__((always_inline)) attribute is defined without an "inline"
    // statement. I think therefore there must be some case where "__inline__"
    // is not always defined, thus the compiler emitting these warnings. When
    // using -std=c89 or -ansi on the command line, we cannot use the "inline"
    // keyword and instead need to use "__inline__". In an attempt to work
    // around this issue I am using "__inline__" only when we're compiling in
    // strict ANSI mode.
    #if defined(__STRICT_ANSI__)
        #define prh_impl_gnuc_inline __inline__
    #else
        #define prh_impl_gnuc_inline inline
    #endif
    #if PRH_GCC_VERSION >= 30200 || defined(__clang__)
        #define prh_inline static prh_impl_gnuc_inline __attribute__((always_inline))
        #define prh_noinline __attribute__((noinline))
    #else
        #define prh_inline static prh_impl_gnuc_inline
        #define prh_noinline __attribute__((noinline))
    #endif
#elif defined(__WATCOMC__)
    #define prh_inline static __inline
    #define prh_noinline
#else
    #define prh_inline static inline
    #define prh_noinline
#endif
#endif // prh_inline

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

// thread_local
#ifndef prh_thread_local
#if defined(__cplusplus) && __cplusplus >= 201103L // C++11 keyword
    #define prh_thread_local thread_local
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    #define prh_thread_local thread_local
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L // C11 keyword
    #define prh_thread_local _Thread_local
#elif defined(_MSC_VER)
    #define prh_thread_local __declspec(thread)
#elif defined(__GNUC__)
    #define prh_thread_local __thread
#endif
#endif

// alignas(size) alignas(type)
// When used in a declaration, the declared object will have its alignment
// requirement set to 1) the specified size, unless it is zero; 2) the
// alignment requirement of the type; except when this would weaken the
// alignment the type would have had naturally.
// When multiple alignas specifiers appear in the same declaration, the
// strictest one is used.
// In C++, the alignas specifier may also be applied to the declarations of
// class/struct/union types and enumerations. This is not supported in C, but
// the alignment of a struct type can be controlled by using alignas in a
// member declaration.
#ifndef prh_alignas
#if defined(__cplusplus) && __cplusplus >= 201103L // C++11 keyword
    #define prh_alignas(size) alignas(size)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    #define prh_alignas(size) alignas(size)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L // C11 keyword
    #define prh_alignas(size) _Alignas(size)
#elif defined(_MSC_VER)
    // Before Visual Studio 2015 you could use the Microsoft-specific keywords
    // __alignof and __declspec(align) to specify an alignment greater than
    // the default. Starting in Visual Studio 2015 you should use the C++11
    // standard keywords alignof and alignas for maximum code portability.
    #define prh_alignas(size) __declspec(align(size))
#elif defined(__GNUC__)
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
#endif
#endif

// alignof(type)
// Returns the alignment requirement of the type named by the type. If type
// is an array type, the result is the alignment requirement of the array
// element type. The type cannot be function type or an incomplete type.
// The result is an integer constant of type size_t.
// The operand is not evaluated (so external identifiers used in the operand
// do not have to be defined).
// The use of alignof with expressions is allowed by some C compilers as a
// non-standard extension.
#ifndef prh_alignof
#if defined(__cplusplus) && __cplusplus >= 201103L // C++11 keyword
    #define prh_alignof(type) alignof(type)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 202311L // C23 keyword
    #define prh_alignof(type) alignof(type)
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L // C11 keyword
    #define prh_alignof(type) _Alignof(type)
#elif defined(_MSC_VER)
    // Before Visual Studio 2015 you could use the Microsoft-specific keywords
    // __alignof and __declspec(align) to specify an alignment greater than
    // the default. Starting in Visual Studio 2015 you should use the C++11
    // standard keywords alignof and alignas for maximum code portability.
    // The alignof(type) operator returns the alignment in bytes of the
    // specified type as a value of type size_t.
    #define prh_alignof(type) __alignof(type)
#elif defined(__GNUC__)
    // The keyword __alignof__ determines the alignment requirement of a
    // function, object, or a type, or the minimum alignment usually required
    // by a type. Its syntax is just like sizeof and C11 _Alignof.
    #define prh_alignof(type) __alignof__(type)
#endif
#endif

// packed struct
#ifndef prh_packed_struct
#if defined(_MSC_VER)
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
    #define prh_packing_reset __pragma(pack(pop))
#elif defined(__GNUC__)
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
    #define prh_packing_reset
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
    #define prh_packing_reset _Pragma("pack(pop)")
#endif
#endif

// typeof(expr)
#ifndef prh_typeof
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
#elif defined(_MSC_VER)
    // __typeof__ in msc requires Visual Studio 17.9 or later, or cl.exe
    // version 19.39.33428 or later. This can be meet after installing the
    // latest version of Visual Studio Community 2022.
    #define prh_typeof(expr) __typeof__(expr)
#elif defined(__GNUC__)
    #define prh_typeof(expr) __typeof__(expr)
#endif
#endif

// compiler
#ifndef prh_impl_compiler
#ifdef _MSC_VER
    #define prh_cl_msc 1
#elif defined(__GNUC__)
    #define prh_cl_gnu 1
#endif
#define prh_impl_compiler
#endif

// fastcall convension
#ifndef prh_impl_fastcall
#if defined(prh_cl_msc)
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
#elif defined(prh_cl_gnu)
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
#define prh_impl_fastcall
#endif

#ifndef prh_static_assert
    #define prh_static_assert(const_expr) typedef int prh_impl_static_assert[(const_expr) ? 1 : -1]
#endif

#ifndef prh_array_size
    #define prh_array_size(a) (sizeof(a)/sizeof((a)[0]))
    #define prh_array_elem(a, i) (prh_assert((i) >= 0 && (i) < prh_array_size(a))), (a)[i]
#endif

#ifndef prh_offsetof
    #define prh_offsetof(type, field) ((prh_unt)(&((type *)0)->field))
#endif

#ifndef prh_possibly_nouse
    #define prh_possibly_nouse(a) ((void)(a))
#endif

#ifndef prh_macro_concat_name
    #define prh_macro_concat_name(a, b) prh_impl_macro_concat_name(a, b)
    #define prh_impl_macro_concat_name(a, b) a ## b
#endif

#ifndef prh_release_assert
    #define prh_release_assert(a) ((void)((a) || prh_impl_assert(__LINE__)))
    #define prh_release_unreachable() prh_abort_error(0xea)
    void prh_impl_assert(int line);
#endif

#ifndef prh_assert
#if PRH_DEBUG
    #define prh_assert(a) prh_release_assert(a)
    #define prh_unreachable() prh_release_unreachable()
#else
    #define prh_assert(a) ((void)0)
    #define prh_unreachable() ((void)0)
#endif
#endif

#ifndef prh_zeroret
#if PRH_DEBUG // macro arg 'a' can only expand once
    #define prh_condret(c, a) if (!((a) c)) { exit(__LINE__); }
    #define prh_numbret(n, a) if ((a) != (n)) { exit(__LINE__); }
    #define prh_zeroret(a) if ((a) != 0) { exit(__LINE__); }
    #define prh_nnegret(a) if ((a) < 0) { exit(__LINE__); }
    #define prh_boolret(a) if (!(a)) { exit(__LINE__); }
#else
    #define prh_condret(c, a) a
    #define prh_numbret(n, a) a
    #define prh_zeroret(a) a
    #define prh_nnegret(a) a
    #define prh_boolret(a) a
#endif
#endif

// The ‘##’ token paste operator has a special meaning when placed between a
// comma (,) and a variable argument (__VA_ARGS__). If the variable argument
// is left out when the macro is used, then the comma before the ‘##’ will be
// deleted. This does not happen if you pass an empty argument, nor does it
// happen if the token preceding ‘##’ is anything other than a comma.
// ISO C99 requires at least one argument for the "..." in a variadic macro.

#ifndef prh_defer_if
    #define prh_defer_if(cond, ...) if (cond) { __VA_ARGS__; goto label_defer; }
#endif

#ifndef prh_prerr_if
    #define prh_preno_if(a) if (a) { prh_prerr(errno); }
    #define prh_prerr_if(err, ...) if (err) { prh_prerr(err); __VA_ARGS__; }
    #define prh_abort_if(err) if (err) { prh_abort_error(err); }
    #define prh_prerr(err) prh_impl_prerr((err), __LINE__)
    #define prh_abort_error(err) prh_impl_abort_error((err), __LINE__)
    void prh_impl_prerr(int err, int line);
    void prh_impl_abort_error(int err, int line);
#endif

#ifndef prh_null
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
#endif

// bool true false
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

#ifndef prh_impl_basic_types
    #define prh_impl_basic_types
    typedef unsigned char prh_byte;
    typedef unsigned char prh_u08;
    typedef signed char prh_i08;
    typedef unsigned short prh_u16;
    typedef short prh_i16;
    typedef unsigned int prh_u32;
    typedef int prh_i32;
    typedef unsigned long long prh_u64;
    typedef long long prh_i64;
#if prh_arch_bits == 32
    typedef prh_i32 prh_int;
    typedef prh_u32 prh_unt;
    typedef prh_i32 prh_mach_int;
    typedef prh_u32 prh_mach_unt;
#elif prh_arch_bits == 64
    #ifdef prh_32_bit_memory_range
        typedef prh_i32 prh_int;
        typedef prh_u32 prh_unt;
    #else
        typedef prh_i64 prh_int;
        typedef prh_u64 prh_unt;
    #endif
    typedef prh_i64 prh_mach_int;
    typedef prh_u64 prh_mach_unt;
#else
    #error unsupported architecture
#endif
    typedef prh_unt prh_void_ptr;
    typedef prh_mach_unt prh_mach_ptr;
    typedef float prh_f32;
    typedef double prh_f64;
    typedef prh_f32 prh_float;
    prh_static_assert(sizeof(int) == 4);
    prh_static_assert(sizeof(bool) == 1);
    prh_static_assert(sizeof(prh_byte) == 1);
    prh_static_assert(sizeof(prh_u08) == 1);
    prh_static_assert(sizeof(prh_i08) == 1);
    prh_static_assert(sizeof(prh_u16) == 2);
    prh_static_assert(sizeof(prh_i16) == 2);
    prh_static_assert(sizeof(prh_u32) == 4);
    prh_static_assert(sizeof(prh_i32) == 4);
    prh_static_assert(sizeof(prh_u64) == 8);
    prh_static_assert(sizeof(prh_i64) == 8);
    prh_static_assert(sizeof(prh_int) == sizeof(void *)); // signed pointer size type
    prh_static_assert(sizeof(prh_unt) == sizeof(void *)); // unsigned pointer size type
    prh_static_assert(sizeof(prh_mach_int) == prh_arch_bits / 8); // signed machine generic purpose regiter size type
    prh_static_assert(sizeof(prh_mach_unt) == prh_arch_bits / 8); // unsigned machine generic purpose register size type
    prh_static_assert(sizeof(prh_void_ptr) == sizeof(prh_unt));
    prh_static_assert(sizeof(prh_mach_ptr) == sizeof(prh_mach_unt));
    prh_static_assert(sizeof(prh_f32) == 4);
    prh_static_assert(sizeof(prh_f64) == 8);
    prh_static_assert(sizeof(prh_float) == 4);
#endif

#ifdef PRH_CONO_INCLUDE
#define PRH_THRD_INCLUDE
#define PRH_CORO_INCLUDE
#ifdef PRH_CONO_IMPLEMENTATION
#define PRH_THRD_IMPLEMENTATION
#define PRH_CORO_IMPLEMENTATION
#endif
#endif

#ifdef PRH_THRD_INCLUDE
#define PRH_ATOMIC_INCLUDE
#define PRH_TIME_INCLUDE
#ifdef PRH_THRD_IMPLEMENTATION
#define PRH_ATOMIC_IMPLEMENTATION
#define PRH_TIME_IMPLEMENTATION
#endif
#endif

#ifdef PRH_TIME_INCLUDE
#ifdef PRH_TIME_IMPLEMENTATION
#define PRH_BASE_IMPLEMENTATION
#endif
#endif

#ifdef PRH_ATOMIC_INCLUDE
#define PRH_QUEUE_INCLUDE
#ifdef PRH_ATOMIC_IMPLEMENTATION
#define PRH_QUEUE_IMPLEMENTATION
#endif
#endif

#ifdef PRH_STACK_INCLUDE
#define PRH_LIST_INCLUDE
#ifdef PRH_STACK_IMPLEMENTATION
#define PRH_LIST_IMPLEMENTATION
#endif
#endif

#ifdef PRH_QUEUE_INCLUDE
#define PRH_LIST_INCLUDE
#ifdef PRH_QUEUE_IMPLEMENTATION
#define PRH_LIST_IMPLEMENTATION
#endif
#endif

#ifdef RPH_STRING_INCLUDE
#define PRH_ARRAY_INCLUDE
#ifdef PRH_STRING_IMPLEMENTATION
#define PRH_ARRAY_IMPLEMENTATION
#endif
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
// The macro that is only defined if compiling for FreeBSD.
#define prh_plat_freebsd 1
#endif

#if (defined(linux) || defined(__linux) || defined(__linux__))
// The macro that is only defined if compiling for Linux.
// Note that Android, although ostensibly a Linux-based system, will not
// define this. It defines prh_plat_android instead.
#define prh_plat_linux 1
#endif

#if defined(ANDROID) || defined(__ANDROID__)
// The macro that is only defined if compiling for Android.
#define prh_plat_android 1
#undef prh_plat_linux
#endif

#if defined(__unix__) || defined(__unix) || defined(unix)
// The macro that is only defined if compiling for a Unix-like system.
// Other platforms, like Linux, might define this in addition to their
// primary define.
#define prh_plat_unix 1
#endif

#if defined(__EMSCRIPTEN__)
// The macro that is only defined if compiling for Emscripten.
#define prh_plat_emscripten 1
#endif

#if defined(__NetBSD__)
// The macro that is only defined if compiling for NetBSD.
#define prh_plat_netbsd 1
#endif

#if defined(__OpenBSD__)
// The macro that is only defined if compiling for OpenBSD.
#define prh_plat_openbsd 1
#endif

#if defined(__APPLE__)
// The macro that is only defined if compiling for Apple platforms.
// iOS, macOS, etc will additionally define a more specific platform macro.
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
#define prh_plat_apple 1
#endif

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#define prh_plat_windows 1
#endif

// CYGWIN 是一个在 Windows 操作系统上模拟 Unix/Linux 环境的大型工具集，它借助一个
// 动态链接库cygwin1.dll来模拟许多类 Unix 系统调用和 POSIX API。当你在 CYGWIN 环
// 境中运行程序时，程序会调用cygwin1.dll，该库再将这些调用转换为 Windows API 调用，
// 从而实现类 Unix 环境的模拟。
//
// MINGW（Minimalist GNU for Windows）则是将 GNU 工具集移植到 Windows 平台的项目，
// 它直接生成原生的 Windows 可执行文件，不依赖模拟层。MINGW 编译的程序使用 Windows
// API，而不是模拟 Unix 系统调用，因此生成的程序可以直接在 Windows 上运行，无需额外
// 的运行时环境。A native Windows port of the GNU Compiler Collection (GCC),
// with freely distributable import libraries and header files for building
// native Windows applications; includes extensions to the MSVC runtime to
// support C99 functionality. The mingw-w64 project is a complete runtime
// environment for gcc to support binaries native to Windows 64-bit and 32-bit
// operating systems. Mingw-w64 is an advancement of the original mingw.org
// project, which was created to support the GCC compiler on Windows systems.
// It was forked in 2007 in order to provide 64-bit support and newer APIs.
// It has since then gained wide use and distribution.
//
// MSYS2（Minimal SYStem 2）是一个在 Windows 平台上提供类 Unix 环境和开发工具的软
// 件。MSYS2 提供了类似于 Unix/Linux 系统的 shell 环境，以及一系列 Unix 风格的命令
// 行工具，这使得开发者可以在 Windows 上使用熟悉的 Unix 命令和操作方式进行开发和管理
// 工作。它集成了 Pacman 包管理器，这是 Arch Linux 所使用的包管理工具。借助 Pacman，
// 你能够轻松地安装、更新和删除软件包，并且可以方便地管理软件包之间的依赖关系。通过包
// 管理器，你可以获取到大量的开源软件和开发工具，如 GCC 编译器、Python、Ruby 等。
// MSYS2 包含了一系列的开发工具，如 GCC、GDB、Make 等，这些工具是进行 C、C++ 等语言
// 开发所必需的。同时，它还支持多种编程语言的开发环境，如 Python、Ruby、Perl 等。
// 与 MinGW 相比，MSYS2 不仅提供了编译工具，还提供了完整的类 Unix 环境和丰富的开
// 发工具。MSYS2 is a collection of tools and libraries providing you with an
// easy-to-use environment for building, installing and running native Windows
// software. It consists of a command line terminal called mintty, bash,
// version control systems like git and subversion, tools like tar and awk and
// even build systems like autotools, all based on a modified version of
// Cygwin. Despite some of these central parts being based on Cygwin, the main
// focus of MSYS2 is to provide a build environment for native Windows software
// and the Cygwin-using parts are kept at a minimum. MSYS2 provides up-to-date
// native builds for GCC, mingw-w64, CPython, CMake, Meson, OpenSSL, FFmpeg,
// Rust, Ruby, just to name a few. The unixy tools in MSYS2 are directly based
// on Cygwin, so there is some overlap there. While Cygwin focuses on building
// Unix software on Windows as is, MSYS2 focuses on building native software
// built against the Windows APIs.

#if defined(PRH_CONO_IMPLEMENTATION) || defined(PRH_THRD_IMPLEMENTATION) || \
    defined(PRH_ATOMIC_IMPLEMENTATION) || defined(PRH_TIME_IMPLEMENTATION)
#if defined(prh_plat_windows)
    // Predefined macros:
    //      _WIN16      A 16-bit platform
    //      _WIN32      A 32-bit platform. This value is also defined by the
    //                  64-bit compiler for backward compatibility.
    //      _WIN64      A 64-bit platform. This includes both x64 and ARM64.
    //      _M_IA64     Intel Itanium platform
    //      _M_IX86     x86 platform
    //      _M_X64      x64 platform
    //      _M_ARM64    ARM64 platform
    //
    // When you use the Windows SDK, you can specify which versions of Windows
    // your code can run on. The preprocessor macros WINVER and _WIN32_WINNT
    // specify the minimum operating system version your code supports.
    //
    // The possible values are listed in the Windows header file sdkddkver.h,
    // which defines macros for each major Windows version. To build your
    // application to support a previous Windows platform, include WinSDKVer.h.
    // Then, set the WINVER and _WIN32_WINNT macros to the oldest supported
    // platform before including sdkddkver.h. Here are the lines from the
    // Windows 10 SDK version of sdkddkver.h that encode the values for each
    // major version of Windows.
    //
    //      #define _WIN32_WINNT_NT4            0x0400 // Windows NT 4.0
    //      #define _WIN32_WINNT_WIN2K          0x0500 // Windows 2000
    //      #define _WIN32_WINNT_WINXP          0x0501 // Windows XP
    //      #define _WIN32_WINNT_WS03           0x0502 // Windows Server 2003
    //      #define _WIN32_WINNT_WIN6           0x0600 // Windows Vista
    //      #define _WIN32_WINNT_VISTA          0x0600 // Windows Vista
    //      #define _WIN32_WINNT_WS08           0x0600 // Windows Server 2008
    //      #define _WIN32_WINNT_LONGHORN       0x0600 // Windows Vista
    //      #define _WIN32_WINNT_WIN7           0x0601 // Windows 7
    //      #define _WIN32_WINNT_WIN8           0x0602 // Windows 8
    //      #define _WIN32_WINNT_WINBLUE        0x0603 // Windows 8.1
    //      #define _WIN32_WINNT_WINTHRESHOLD   0x0A00 // Windows 10
    //      #define _WIN32_WINNT_WIN10          0x0A00 // Windows 10
    //
    // For a more fine-grained approach to versioning, you can use the NTDDI
    // version constants in sdkddkver.h. Here are some of the macros defined
    // by sdkddkver.h in Windows 10 SDK version 10.0.18362.0.
    //
    //      #define NTDDI_WIN4                  0x04000000
    //      #define NTDDI_WIN2K                 0x05000000
    //      #define NTDDI_WIN2KSP1              0x05000100
    //      #define NTDDI_WIN2KSP2              0x05000200
    //      #define NTDDI_WIN2KSP3              0x05000300
    //      #define NTDDI_WIN2KSP4              0x05000400
    //      #define NTDDI_WINXP                 0x05010000
    //      #define NTDDI_WINXPSP1              0x05010100
    //      #define NTDDI_WINXPSP2              0x05010200
    //      #define NTDDI_WINXPSP3              0x05010300
    //      #define NTDDI_WINXPSP4              0x05010400
    //      #define NTDDI_WS03                  0x05020000
    //      #define NTDDI_WS03SP1               0x05020100
    //      #define NTDDI_WS03SP2               0x05020200
    //      #define NTDDI_WS03SP3               0x05020300
    //      #define NTDDI_WS03SP4               0x05020400
    //      #define NTDDI_WIN6                  0x06000000
    //      #define NTDDI_WIN6SP1               0x06000100
    //      #define NTDDI_WIN6SP2               0x06000200
    //      #define NTDDI_WIN6SP3               0x06000300
    //      #define NTDDI_WIN6SP4               0x06000400
    //      #define NTDDI_VISTA                 NTDDI_WIN6
    //      #define NTDDI_VISTASP1              NTDDI_WIN6SP1
    //      #define NTDDI_VISTASP2              NTDDI_WIN6SP2
    //      #define NTDDI_VISTASP3              NTDDI_WIN6SP3
    //      #define NTDDI_VISTASP4              NTDDI_WIN6SP4
    //      #define NTDDI_LONGHORN              NTDDI_VISTA
    //      #define NTDDI_WS08                  NTDDI_WIN6SP1
    //      #define NTDDI_WS08SP2               NTDDI_WIN6SP2
    //      #define NTDDI_WS08SP3               NTDDI_WIN6SP3
    //      #define NTDDI_WS08SP4               NTDDI_WIN6SP4
    //      #define NTDDI_WIN7                  0x06010000
    //      #define NTDDI_WIN8                  0x06020000
    //      #define NTDDI_WINBLUE               0x06030000
    //      #define NTDDI_WINTHRESHOLD          0x0A000000
    //      #define NTDDI_WIN10                 0x0A000000
    //      #define NTDDI_WIN10_TH2             0x0A000001
    //      #define NTDDI_WIN10_RS1             0x0A000002
    //      #define NTDDI_WIN10_RS2             0x0A000003
    //      #define NTDDI_WIN10_RS3             0x0A000004
    //      #define NTDDI_WIN10_RS4             0x0A000005
    //      #define NTDDI_WIN10_RS5             0x0A000006
    //      #define NTDDI_WIN10_19H1            0x0A000007
    //      #define NTDDI_WIN10_VB              0x0A000008
    //      #define NTDDI_WIN10_MN              0x0A000009
    //      #define NTDDI_WIN10_FE              0x0A00000A
    //      #define NTDDI_WIN10_CO              0x0A00000B
    //      #define NTDDI_WIN10_NI              0x0A00000C
    //      #define WDK_NTDDI_VERSION           NTDDI_WIN10_NI
    //
    // The OSVER, SPVER, and SUBVER macros can be used in your code to control
    //  conditional compilation for different levels of API support.
    //
    //      #define OSVERSION_MASK      0xFFFF0000
    //      #define SPVERSION_MASK      0x0000FF00
    //      #define SUBVERSION_MASK     0x000000FF
    //      #define OSVER(Version)  ((Version) & OSVERSION_MASK)
    //      #define SPVER(Version)  (((Version) & SPVERSION_MASK) >> 8)
    //      #define SUBVER(Version) (((Version) & SUBVERSION_MASK))
    //
    // By default, new Windows projects in Visual Studio use the latest
    // Windows SDK that ships with Visual Studio. To use a newer SDK you've
    // installed separately, you'll have to set the Windows SDK explicitly
    // in your project properties. Values are not guaranteed to work if you
    // include internal MFC headers in your application. You can also define
    // this macro by using the /D compiler option.
    //
    // https://learn.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt
    // https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
    // https://learn.microsoft.com/en-us/windows/win32/winprog/header-annotations
    //
    // The header files for the Windows API enable you to create 32- and 64-bit
    // applications. They include declarations for both Unicode and ANSI
    // versions of the API. For more information, see Unicode in the Windows
    // API. They use data types that enable you to build both 32- and 64-bit
    // versions of your application from a single source code base. For more
    // information, see Getting Ready for 64-bit Windows. Additional features
    // include Header Annotations and STRICT Type Checking.
    //
    // Microsoft Visual C++ 包含了在 Visual C++ 发布时当前版本的 Windows 头文件。
    // 因此，如果你从 SDK 安装了更新的头文件，你的计算机上可能会有多个版本的 Windows
    // 头文件。如果你没有确保使用的是最新版本的 SDK 头文件，当你编译使用了在 Visual
    // C++ 发布后引入的功能的代码时，你会收到以下错误代码：error C2065: undeclared
    // identifier。
    //
    // 某些依赖于特定版本 Windows 的函数使用条件代码声明。这使得你可以利用编译器检测
    // 你的应用程序是否使用了其目标 Windows 版本不支持的函数。要编译使用这些函数的应
    // 用程序，你必须定义适当的宏。否则，你会收到 C2065 错误消息。
    //
    // The Windows header files use macros to indicate which versions of
    // Windows support many programming elements. Therefore, you must define
    // these macros to use new functionality introduced in each major
    // operating system release. (Individual header files may use different
    // macros; therefore, if compilation problems occur, check the header file
    // that contains the definition for conditional definitions.) For more
    // information, see SdkDdkVer.h.
    //
    // The table show above describes the preferred macros used in the Windows
    // header files. If you define NTDDI_VERSION, you must also define
    // _WIN32_WINNT. You can define these symbols by using the #define
    // statement in each source file, or by specifying the /D compiler option
    // supported by Visual C++. For example, to set _WIN32_WINNT in your source
    // file, use the following statement:
    //      #define _WIN32_WINNT 0x0502
    // To set _WIN32_WINNT using the /D compiler option, use the following
    //  command:
    //      cl -c /D_WIN32_WINNT=0x0502 source.cpp
    //
    // 注意，最新版本的 Windows 中引入的一些功能可能会添加到早期版本 Windows 的服务
    // 包（Service Pack, SP）中。因此，要为目标服务包定义 _WIN32_WINNT，你可能需要
    // 使用下一个主要操作系统版本的值。例如，GetDllDirectory 函数是在 Windows
    // Server 2003 中引入的，并且如果 _WIN32_WINNT 是 0x0502（Windows Server
    // 2003）或更高版本，则会定义该函数。此函数还被添加到了带有 SP1 的 Windows XP
    // 中。因此，如果你将 _WIN32_WINNT 定义为 0x0501（Windows XP），你将错过在带
    // 有 SP1 的 Windows XP 中定义的功能。
    //
    // https://devblogs.microsoft.com/oldnewthing/20070411-00/?p=27283
    // 关于 WINVER 和其他版本控制符号的起源与演变
    //
    // WINVER 符号是最古老的。它是 16 位 Windows 用于控制其头文件版本的符号，后来被
    // 延续到了 32 位头文件中，这可能是因为最初将头文件从 16 位转换为 32 位的那批人，
    // 他们习惯于使用 WINVER 符号。这个符号在那些可以追溯到 16 位 Windows 的头文件中
    // 仍然被广泛使用，例如 winuser.h、wingdi.h 和 mmsystem.h。
    //
    // 接下来出现的是 _WIN32_WINNT 符号。我不确定它从何而来，但从它的名字来看，很可
    // 能是 Windows NT 团队发明的，以便让他们能够将头文件中仅在 Windows NT 的 Win32
    // 实现中可用的部分隔离出来。别忘了在早期，还有 Win32s，这是可以在 16 位
    // Windows 3.1 上运行的 Win32 子集。单靠 WINVER 符号是不足以精确指定你想要兼容
    // 的版本的。例如，仅在 Windows NT 3.1 中可用的函数会被用 #if _WIN32_WINNT >=
    // 0x030A 保护起来，这样那些希望在 Win32s 上运行的程序就可以将 _WIN32_WINNT 设
    // 置为零，从而将该函数排除在外。同样，Windows 95 和 Windows NT 4 都标识为
    // Windows 主版本 4，所以 WINVER 符号也不足以区分它们。因此，那些存在于 Windows
    // NT 4 但不存在于 Windows 95 中的函数被用 _WIN32_WINNT 保护起来。另一方面，也
    // 有一些函数是在 Windows 95 中首次引入的，而最初的 Windows NT 4 中并不存在。
    // _WIN32_WINDOWS 符号让你可以指定你想要访问那些在 Windows 95 中新增的、并且会
    // 被移植到 Windows NT 4 和后续版本 Windows NT 的功能。
    //
    // 历史总是惊人地相似：我们在试图弄清楚为什么有些函数返回 NULL，而另一些函数返回
    // INVALID_HANDLE_VALUE 时就见过这种情况。每次有人向 Windows 添加新功能并需要
    // 添加 #ifdef 保护时，他们几乎都会随机选择使用 WINVER、_WIN32_WINDOWS 还是
    // _WIN32_WINNT。为了试图让这一切混乱变得有些条理，SDK 和 DDK 团队为 Windows
    // Vista 的头文件制定了一个新计划：sdkddkver.h。现在只有一个符号需要你定义来指
    // 定你的最低目标操作系统：NTDDI_VERSION。一旦你设置了它，所有其他符号都会自动
    // 设置为适合你的目标操作系统的值。（至于 NTDDI 这几个字母代表什么，我也不知道，
    // 尽管有一个显而易见的候选词。）但愿所有人都能统一使用 NTDDI_VERSION，这样这篇
    // 文章就会成为那些“古色古香的历史小玩意儿”，就像所有关于 16 位 Windows 的文章
    // 一样。就像“一个关于 21 世纪初疯狂日子里人们不得不做的事情的小故事。谢天谢地，
    // 我们再也不用担心这些了！”
    //
    // GetTickCount64 Windows Vista Windows Server 2008         0x0600
    // QueryInterruptTime Windows 7 Windows Server 2008 S2      0x0601
    #include <WinSDKVer.h>
    #include <sdkddkver.h>
    #if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0600)
    #undef _WIN32_WINNT
    #undef NTDDI_VERSION
    #define _WIN32_WINNT 0x0600
    #include <sdkddkver.h>
    #endif
    // When you define the STRICT symbol, you enable features that require more
    // care in declaring and using types. This helps you write more portable
    // code. This extra care will also reduce your debugging time. Enabling
    // STRICT redefines certain data types so that the compiler does not permit
    // assignment from one type to another without an explicit cast. This is
    // especially helpful with Windows code. Errors in passing data types are
    // reported at compile time instead of causing fatal errors at run time.
    // With Visual C++, STRICT type checking is defined by default.
    #define STRICT // #define NO_STRICT
    // https://learn.microsoft.com/en-us/windows/win32/intl/unicode-in-the-windows-api
    // https://learn.microsoft.com/en-us/windows/win32/intl/unicode
    // https://learn.microsoft.com/en-us/cpp/text/unicode-programming-summary
    // To take advantage of the MFC and C run-time support for Unicode, you
    // need to:
    //      Define the symbol _UNICODE before you build your program.
    //      Set the Entry Point symbol to wWinMainCRTStartup.
    //      Use portable run-time functions and types.
    // The run-time library provides a wide-character version of main. Use
    // wmain to make your application Unicode-aware.
    #define UNICODE
    #define _UNICODE
    // Define WIN32_LEAN_AND_MEAN to exclude APIs such as Cryptography, DDE,
    // RPC, Shell, and Windows Sockets.
    #define WIN32_LEAN_AND_MEAN
    // Suppress deprecation warnings for the older less secure functions.
    #define _CRT_SECURE_NO_DEPRECATE 1
    #define _CRT_SECURE_NO_WARNINGS 1
    // Define one or more of the NOapi symbols to exclude the API. For example,
    // NOCOMM excludes the serial communication API. For a list of support
    // NOapi symbols, see Windows.h. such as #define NOCOMM
    #include <windows.h>
    #if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0600)
    #error unsupported windows minimum version
    #endif
    #define PRH_WINOS_BOOLRET(a) if (!(a)) { prh_abort_error(GetLastError()); }
#else
    // POSIX allows an application to test at compile or run time whether
    // certain options are supported, or what the value is of certain
    // configurable constants or limits.
    // At compile time this is done by including <unistd.h> and/or <limits.h>
    // and testing the value of certain macros.
    // At run time, one can ask for numerical values using the present
    // function sysconf(). One can ask for numerical values that may depend
    // on the filesystem in which a file resides using fpathconf(3) and
    // pathconf(3). One can ask for string values using confstr(3).
    // The values obtained from these functions are system configuration
    // constants. They do not change during the lifetime of a process.
    //
    // For options, typically, there is a constant _POSIX_FOO that may be
    // defined in <unistd.h>. If it is undefined, one should ask at run time.
    // If it is defined to -1, then the option is not supported. If it is
    // defined to 0, then relevant functions and headers exist, but one has
    // to ask at run time what degree of support is available. If it is
    // defined to a value other than -1 or 0, then the option is supported.
    // Usually the value (such as 200112L) indicates the year and month of
    // the POSIX revision describing the option. glibc uses the value 1 to
    // indicate support as long as the POSIX revision has not been published
    // yet. The sysconf() argument will be _SC_FOO. For a list of options,
    // see posixoptions(7).
    // https://www.man7.org/linux/man-pages/man7/posixoptions.7.html
    //
    // For variables or limits, typically, there is a constant _FOO, maybe
    // defined in <limits.h>, or _POSIX_FOO, maybe defined in <unistd.h>.
    // The constant will not be defined if the limit is unspecified. If the
    // constant is defined, it gives a guaranteed value, and a greater value
    // might actually be supported. If an application wants to take advantage
    // of values which may change between systems, a call to sysconf() can
    // be made. The sysconf() argument will be _SC_FOO.
    #define prh_plat_posix 1
    #define _POSIX_C_SOURCE 200809L
    // glibc https://www.gnu.org/software/libc/
    // getconf GNU_LIBC_VERSION, ldd --version, ldd `which ls` | grep libc
    #ifndef _GNU_SOURCE // Many features require _GNU_SOURCE on various platforms
    #define _GNU_SOURCE // pthread_getattr_np glibc 2.2.3, __GLIBC__ __GLIBC_MINOR__
    #endif
    // Need this so Linux systems define fseek64o, ftell64o and off64_t
    #ifndef _LARGEFILE64_SOURCE
    #define _LARGEFILE64_SOURCE 1
    #endif
    #if defined(prh_plat_apple)
    #ifndef _DARWIN_C_SOURCE // Restore Darwin APIs removed by _POSIX_C_SOURCE
        #define _DARWIN_C_SOURCE 1
    #endif
    // https://epir.at/2019/10/30/api-availability-and-target-conditionals/
    #include <Availability.h>
    // https://github.com/phracker/MacOSX-SDKs/blob/master/MacOSX11.3.sdk/usr/include/AvailabilityMacros.h
    // https://github.com/phracker/MacOSX-SDKs/blob/master/MacOSX11.3.sdk/usr/include/TargetConditionals.h
    #include <AvailabilityMacros.h> // AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER
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
    #if TARGET_OS_IPHONE
        // The macro that is only defined if compiling for iOS.
        #define prh_plat_ios 1
    #else
        // The macro that is only defined if compiling for macOS.
        #define prh_plat_macos 1
        #if MAC_OS_X_VERSION_MIN_REQUIRED < 1070 // 10.12 101200
        #error Only support deploying on MAC OS 10.7 and later.
        #endif
    #endif
    #endif // prh_plat_apple
    // cc -dM -E -</dev/null
    // https://jdebp.uk/FGA/predefined-macros-compiler.html
    // https://jdebp.uk/FGA/predefined-macros-processor.html
    // https://jdebp.uk/FGA/predefined-macros-language.html
    // https://jdebp.uk/FGA/predefined-macros-platform.html
    // https://docwiki.embarcadero.com/RADStudio/Alexandria/en/Predefined_Macros
    // https://docs.freebsd.org/en/books/porters-handbook/porting-dads/#porting-versions
    // https://docs-archive.freebsd.org/doc/7.3-RELEASE/usr/share/doc/zh_CN/books/porters-handbook/porting-versions.html
    // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/
    // https://man.freebsd.org/cgi/man.cgi/help.html
    // https://developer.apple.com/library/archive/documentation/Porting/Conceptual/PortingUnix/compiling/compiling.html
    //
    // __MACH__
    //      This macro is defined if Mach system calls are supported.
    // __APPLE__
    //      This macro is defined in any Apple computer. Note: To define a section
    //      of code to be compiled on OSX system, you should define a section using
    //      __APPLE__ with __MACH__ macros. The macro __UNIX__ is not defined in
    //      OSX.
    // __APPLE_CC__
    //      This macro is set to an integer that represents the version number of
    //      the compiler. This lets you distinguish, for example, between compilers
    //      based on the same version of GCC, but with different bug fixes or
    //      features. Larger values denote later compilers.
    // __BIG_ENDIAN__ and __LITTLE_ENDIAN__
    //      These macros tell whether the current architecture uses little endian
    //      or big endian byte ordering. For more information, see Compiling for
    //      Multiple CPU Architectures.
    //
    // 通俗的来讲，Apple现在的主要操作系统，无论是macOS、iOS还是iPadOS，甚至是HomePod和
    // Apple TV（TvOS）都是建立在Darwin的基础上。Darwin 是苹果公司开发的操作系统内核，是
    // macOS 和 iOS 的基础。它基于 Mach 微内核和 FreeBSD 的某些部分。
    //
    // __APPLE__ 宏有一个且只有一个有效的用途：在检查 BSD 系统时识别 Darwin。它仅由苹果
    // 提供的编译器和苹果分叉的编译器定义，例如 lvm-gcc、苹果旧版的 GCC 4 分叉版本以及
    // Clang。此外，一些寻求与这些编译器兼容的编译器（如 IBM 的 XLC++）也会定义这个宏。
    // 即使在这种情况下，__APPLE__ 也仅在目标平台是 Darwin 时被定义。当你需要在代码中区分
    // 不同的 Unix 系统（如 Linux、FreeBSD、OpenBSD 等）时，__APPLE__ 宏可以帮助你识别
    // Darwin 系统。如果你在其他情况下使用 __APPLE__ 宏，你可能正在做一件非常错误的事情。
    // __APPLE__ 宏并不意味着目标设备是 macOS 或 iOS。它仅表示目标平台是 Darwin。
    //
    // __APPLE__ the target platform api is "Apple-ish"                         llvm-gcc Clang
    // __MACH__ the target platform api is Mach-based (including NextSTEP and MacOS 10)     GCC Clang
    // __OpenBSD__ the target platform api is OpenBSD                           GCC Clang
    // __NetBSD__ the target platform api is NetBSD                             GCC Clang
    // __FreeBSD__ the target platform api is FreeBSD                           GCC Clang
    // __DragonFly__ the target platform api is DragonFly BSD                   GCC Clang
    // __linux__ the target platform api is Linux                               GCC
    // __unix__ the target platform api is Unix-alike (i.e. Cygwin or Linux)    GCC Clang
    // _WIN32 the target platform api is Win32                                  DigitalMars GCC Clang MSVC
    // __CYGWIN__ __CYGWIN32__ the target plaform api is Cygwin                 GCC Clang
    //
    // GCC and Clang targetting Apple's Darwin is one of the few cases where a
    // "Unix-alike" target platform doesn't have the __unix__ or __UNIX__ macros
    // defined, hence the check for __APPLE__ and __MACH__. Strictly speaking,
    // those two macros don't, either apart or in combination, mean "Unix-alike".
    // It just so happens that no Mach-based "Apple-ish" platform API will fail
    // to have a <sys/param.h> header. 严格来说，这两个宏（无论是单独还是组合）并不意
    // 味着“类 Unix”。只是碰巧没有基于 Mach 的“类苹果”平台 API 会缺少 <sys/param.h> 
    // 头文件。
    //
    // https://docs-archive.freebsd.org/doc/7.3-RELEASE/usr/share/doc/zh_CN/books/porters-handbook/porting-versions.html
    #if defined(__unix__) || defined(__UNIX__) || (defined(__APPLE__) && defined(__MACH__))
    #include <sys/param.h>
    #endif
    #include <pthread.h> // pthread_create POSIX.1-2008
    #include <unistd.h> // sysconf confstr POSIX.1-2008
    #include <errno.h> // ETIMEDOUT ...
    #define PRH_POSIX_ZERORET(a) if (a) { prh_abort_error(errno); }
#endif
#include <stdatomic.h>
#include <assert.h> // assert
#include <stdlib.h> // malloc calloc realloc free exit
#include <string.h> // memcpy memove memset
// void *memcpy(void *dest, const void *src, size_t count);
// void *memmove(void *dest, const void *src, size_t count);
// void *memset(void *ptr, int value, size_t count);
// if either dest or src is an invalid or null pointer { undefined behavior }
// if dest and src memory overlap for memcpy { undefined behavior }
// the count parameter can be set to zero value.
#include <stdio.h> // printf fprintf
#endif

// 高速缓存（Cache） 是一种位于 CPU 和主存之间的高速存储器，用于存储 CPU 频繁访问的数据
// 和指令。它的速度远高于主存，能够显著提升 CPU 的数据处理效率。高速缓存技术利用程序的局
// 部性原理，将程序中正在使用的部分存放在一个高速的、容量较小的缓存中，使 CPU 的访存操作
// 大多数针对缓存进行，从而提高程序的执行速度。时间局部性：如果一个存储单元被访问，那么该
// 单元很可能很快被再次访问。空间局部性：如果一个存储单元被访问，那么该单元邻近的单元也可
// 能很快被访问。
//
// 高速缓存通常被组织成一个有多个缓存组的数组，每个缓存组（Cache Set）包含若干个缓存行
// （Cache Line）。缓存行是高速缓存中的最小访问单元，用于缓存内存块数据。当 CPU 需要访
// 问主存中的数据时，高速缓存会拦截所有对内存的访问，并判断所需数据是否已经存在于高速缓存
// 中。如果缓存命中（即找到所需数据），则直接从缓存中读取数据。如果缓存未命中（即未找到所
// 需数据），则需要从主存中加载数据到缓存中。组选择（Set Selection）：根据地址中的索引位
// 找到对应的缓存组。行匹配（Line Matching）：在缓存组中，通过标记位判断是否存在匹配的缓
// 存行。字抽取（Word Extraction）：如果找到匹配的缓存行，根据偏移量提取所需的数据。
//
// 现代 CPU 通常具有多级缓存结构，包括 L1、L2 和 L3 缓存。L1 缓存：位于 CPU 芯片内部，
// 速度最快，容量最小，通常分为指令缓存和数据缓存。L2 缓存：位于 CPU 芯片内部或外部，速
// 度稍慢，容量较大。L3 缓存：位于 CPU 芯片外部，速度较慢，容量最大，通常被多个核心共享。
#ifndef PRH_CACHE_LINE_SIZE
#define PRH_CACHE_LINE_SIZE 64
#endif

#ifndef prh_round_ptrsize
#define prh_round_ptrsize(n) (((prh_unt)(n)+(prh_unt)(sizeof(void*)-1)) & (~(prh_unt)(sizeof(void*)-1)))
#define prh_round_cache_line_size(n) (((prh_unt)(n)+PRH_CACHE_LINE_SIZE-1) & (~(prh_unt)(PRH_CACHE_LINE_SIZE-1)))
#define prh_round_08_byte(n) (((prh_unt)(n)+7) & (~(prh_unt)7))
#define prh_round_16_byte(n) (((prh_unt)(n)+15) & (~(prh_unt)15))
prh_inline prh_unt prh_lower_most_bit(prh_unt n) {
    return n & (-(prh_int)n); // 0000 & 0000 -> 0000, 0001 & 1111 -> 0001, 1010 & 0110 -> 0010
}
prh_inline prh_unt prh_remove_lower_most_bit(prh_unt n) {
    return n & (n - 1);
}
prh_inline bool prh_is_power_of_2(prh_unt n) {
    return prh_remove_lower_most_bit(n) == 0; // power of 2 or zero
}
prh_inline prh_unt prh_to_power_of_2(prh_unt n) {
    if (prh_is_power_of_2(n)) return n;
    // TODO: 字节序交换然后计算lower most bit
    prh_unt m = prh_lower_most_bit(n);
    while (m < n) m <<= 1;
    return m;
}
#endif

// https://en.cppreference.com/w/c/memory/aligned_alloc
// https://www.man7.org/linux/man-pages/man3/posix_memalign.3.html
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/aligned-malloc
// https://jemalloc.net/
#ifndef prh_aligned_malloc
#if defined(prh_cl_msc)
    #include <malloc.h>
    // _aligned_malloc is based on malloc. required C header malloc.h.
    // if alignment isn't a power of 2 or size is zero, this function invokes
    // the invalid parameter handler. if execution is allowed to continue, this
    // function returns NULL and sets errno to EINVAL.
    #define prh_aligned_malloc(alignment, size) _aligned_malloc((size), (alignment))
    #define prh_cache_line_aligned_malloc(size) _aligned_malloc((size), PRH_CACHE_LINE_SIZE)
    #define prh_16_byte_aligned_malloc(size) _aligned_malloc((size), 16)
    #define prh_aligned_free(p) _aligned_free(p)
#else
    #include <stdlib.h>
    // behavior is undefined if size is not an integral multiple of alignment
    void *aligned_alloc(size_t alignment, size_t size); // glibc 2.16. C11
    #define prh_aligned_malloc(alignment, size) aligned_alloc((alignment), (size))
    #define prh_cache_line_aligned_malloc(size) aligned_alloc(PRH_CACHE_LINE_SIZE, (size))
    #define prh_16_byte_aligned_malloc(size) aligned_alloc(16, (size))
    #define prh_aligned_free(p) free(p)
#endif
#endif

#ifndef prh_malloc
#include <stdlib.h>
// void *malloc(size_t size);
// the newly allocated block of memory is not initialized, remaining with indeterminate values.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.
#define prh_malloc(size) malloc(size)
// void *calloc(size_t num, size_t size);
// allocates a block of memory for an array of num elements, each of them size bytes long, and
// initializes all its bits to zero. the effective result is the allocation of a zero-initialized
// memory block of (num*size) bytes.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.
#define prh_calloc(size) calloc(1, (size))
// void *realloc(void *ptr, size_t size);
// if ptr == prh_null { return malloc(size) }
// if size == 0 { may be free(ptr) or depends on library implementation }
// if size > ptr old size { may return the new location and the newer portion is indeterminate }
// the content is preserved up to min(old and new size), even if moved to a new location.
// if fails to allocate the requested block of memory, null is returned and ptr remain unchanged.
#define prh_realloc realloc
// void free(void *ptr);
// if ptr == prh_null { the function does nothing }
#define prh_free free
#endif

#ifdef PRH_ARRAY_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#endif

#ifdef PRH_QUEUE_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#endif

#ifdef PRH_CORO_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#endif

#ifdef PRH_BASE_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
void prh_impl_assert(int line) {
    fprintf(stderr, "assert line %d\n", line);
    abort();
}
void prh_impl_prerr(int err, int line) {
    fprintf(stderr, "error %d line %d\n", err, line);
}
void prh_impl_abort_error(int err, int line) {
    prh_impl_prerr(err, line);
    abort();
}
#endif // PRH_BASE_IMPLEMENTATION

#ifndef PRH_GLIBC_VERSION
#if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
    #define PRH_GLIBC_VERSION (__GLIBC__ * 100 + __GLIBC_MINOR__)
#endif
#endif

#ifdef prh_plat_posix
// The preferred way to tell FreeBSD versions apart are the __FreeBSD_version
// and __FreeBSD__ macros defined in sys/param.h. __FreeBSD__ is defined in all
// versions of FreeBSD as their major version number. For example, in FreeBSD
// 9.x, __FreeBSD__ is defined to be 9. __FreeBSD_version can be found in page
// https://people.freebsd.org/~olivierd/porters-handbook/versions.html. See
// https://docs.freebsd.org/en/books/porters-handbook/porting-dads/#porting-versions
//
//      #if __FreeBSD__ >= 9
//      #if __FreeBSD_version >= 901000
//      ... 9.1+ release specific code here ...
//      #endif
//      #endif
//
// __NetBSD_Version__ is defined in sys/param.h, formatted as VVRR00PP00 from
// NetBSD 2.99.9. VV is version, RR is revision, PP is patch. For example
// 3.99.8, VV is 3, RR is 99, PP is 8, __NetBSD_Version__ is 399000800.
// To distinguish between specific NetBSD versions, you should use the
// following code. See https://www.netbsd.org/docs/pkgsrc/fixes.html. Also see
// https://sourceforge.net/p/predef/wiki/OperatingSystems/
#if defined(PRH_GLIBC_VERSION) && (PRH_GLIBC_VERSION >= 206)
    // prctl - operations on a process or thread
    // int prctl(int op, ...);
    #include <linux/prctl.h> // definition of PR_* constants
    #include <sys/prctl.h>
#endif
#ifndef prh_impl_pthread_getattr
    #if defined(PRH_GLIBC_VERSION) && (PRH_GLIBC_VERSION >= 203)
        #define prh_impl_pthread_getattr_np pthread_getattr_np
    #endif
    #ifdef __NetBSD__ // __NetBSD_Version__ is defined in sys/param.h
        #include <sys/param.h>
        #if defined __NetBSD_Version__ && __NetBSD_Version__ >= 600000000 // NetBSD 6.0
        #define prh_impl_pthread_attr_get_np pthread_attr_get_np
        #endif
    #endif
    #if defined(__FreeBSD__) || defined(__OpenBSD__)
        #include <pthread_np.h> // pthread_attr_get_np pthread_set_name_np
        #define prh_impl_pthread_attr_get_np pthread_attr_get_np
    #endif
    #if defined(prh_impl_pthread_getattr_np)
        #define prh_impl_pthread_getattr prh_impl_pthread_getattr_np
    #elif defined(prh_impl_pthread_attr_get_np)
        #define prh_impl_pthread_getattr prh_impl_pthread_attr_get_np
    #endif
#endif // prh_impl_pthread_getattr
#endif // prh_plat_posix

#ifdef PRH_TEST_IMPLEMENTATION
#if defined(PRH_ATOMIC_INCLUDE) && defined(PRH_ATOMIC_IMPLEMENTATION)
void prh_impl_atomic_test(void);
#endif
#if defined(PRH_TIME_INCLUDE) && defined(PRH_TIME_IMPLEMENTATION)
void prh_impl_time_test(void);
#endif
#if defined(PRH_THRD_INCLUDE) && defined(PRH_THRD_IMPLEMENTATION)
void prh_impl_thrd_test(void);
#endif
#if defined(PRH_CONO_INCLUDE) && defined(PRH_CONO_IMPLEMENTATION)
void prh_impl_cono_test(void);
#endif
typedef struct {
    char a;
    int b;
    short c;
    void *d;
} prh_impl_test_struct;
void prh_impl_basic_test(void) {
    int a[6] = {0}, b = 1, count = 0;
    int len, *len_ptr = a;
    len = (*len_ptr)++; prh_release_assert(len == 0 && a[0] == 1 && len_ptr == a);
    len = ++*len_ptr; prh_release_assert(len == 2 && a[0] == 2 && len_ptr == a);
    len = *len_ptr++; prh_release_assert(len == 2 && a[0] == 2 && len_ptr == a + 1);
    len = *++len_ptr; prh_release_assert(len == 0 && a[0] == 2 && len_ptr == a + 2);
    memcpy(&a, &b, count); // 长度参数可以传递数值零
    memmove(&a, &b, count);
    memset(&a, 2, count);
    prh_release_assert(prh_offsetof(prh_impl_test_struct, a) == 0);
    prh_release_assert(prh_offsetof(prh_impl_test_struct, b) == 4);
    prh_release_assert(prh_offsetof(prh_impl_test_struct, c) == 8);
#if prh_arch_32
    prh_release_assert(prh_offsetof(prh_impl_test_struct, d) == 12);
#elif prh_arch_64
    prh_release_assert(prh_offsetof(prh_impl_test_struct, d) == 16);
#endif
}
void prh_test_code(void) {
#if defined(__linux__)
    printf("__linux__ %d defined\n", __linux__);
#endif
#if defined(__LINUX__)
    printf("__LINUX__ %d defined\n", __LINUX__);
#endif
#if defined(__linux)
    printf("__linux %d defined\n", __linux);
#endif
#if prh_plat_linux
#if defined(HZ)
    printf("Linux kernel HZ %lld\n", (long long)HZ);
#endif
#endif
#if defined(linux)
    printf("linux %d defined\n", linux);
#endif
#if defined(__unix__)
    printf("__unix__ %d defined\n", __unix__);
#endif
#if defined(__UNIX__)
    printf("__UNIX__ %d defined\n", __UNIX__);
#endif
#if defined(__unix)
    printf("__unix %d defined\n", __unix);
#endif
#if defined(unix)
    printf("unix %d defined\n", unix);
#endif
#if defined(BSD)
    printf("BSD %d defined\n", BSD);
#endif
    printf("PRH_DEBUG %d\n", PRH_DEBUG);
#if defined(MAC_OS_X_VERSION_MAX_ALLOWED)
    printf("MAC_OS_X_VERSION_MAX_ALLOWED %d\n", MAC_OS_X_VERSION_MAX_ALLOWED);
#endif
#if defined(MAC_OS_X_VERSION_MIN_REQUIRED)
    printf("MAC_OS_X_VERSION_MIN_REQUIRED %d\n", MAC_OS_X_VERSION_MIN_REQUIRED);
#endif
#if defined(WINVER_MAXVER)
    printf("WINVER_MAXVER %x\n", WINVER_MAXVER);
#endif
#if defined(WDK_NTDDI_VERSION)
    printf("WDK_NTDDI_VERSION %x\n", WDK_NTDDI_VERSION);
#endif
#if defined(WINVER)
    printf("WINVER %x\n", WINVER);
#endif
#if defined(_WIN32_WINNT)
    printf("_WIN32_WINNT %x\n", _WIN32_WINNT);
#endif
#if defined(NTDDI_VERSION)
    printf("NTDDI_VERSION %x\n", NTDDI_VERSION);
#endif
#if defined(_MSC_VER)
    printf("msc version %d\n", _MSC_VER);
#endif
#if defined(PRH_GCC_VERSION)
    printf("gcc version %d\n", PRH_GCC_VERSION);
#endif
#if defined(PRH_CLANG_VERSION)
    printf("clang version %d\n", PRH_CLANG_VERSION);
#endif
#if defined(PRH_GLIBC_VERSION)
    printf("glibc version %d\n", PRH_GLIBC_VERSION);
#endif
#if defined(__STDC_VERSION__)
    printf("stdc version %ld\n", (long)__STDC_VERSION__);
#endif
#if defined(__cplusplus)
    printf("c++ version %ld\n", (long)__cplusplus);
#endif
    prh_impl_basic_test();
#if defined(PRH_ATOMIC_INCLUDE) && defined(PRH_ATOMIC_IMPLEMENTATION)
    prh_impl_atomic_test();
#endif
#if defined(PRH_TIME_INCLUDE) && defined(PRH_TIME_IMPLEMENTATION)
    prh_impl_time_test();
#endif
#if defined(PRH_THRD_INCLUDE) && defined(PRH_THRD_IMPLEMENTATION)
    prh_impl_thrd_test();
#endif
#if defined(PRH_CONO_INCLUDE) && defined(PRH_CONO_IMPLEMENTATION)
    prh_impl_cono_test();
#endif
}
#endif // PRH_TEST_IMPLEMENTATION

// ARRAYS - Very simple single file style library for array data structures
//
//  * fixed runtime length array
//          [cap][end][elem][elem]...
//                    ^
//    arrfix elt ptr  |
//
//  * dynamic length array
//          [cap|len][elem][elem]...
//                    ^
//    array elt ptr   |
//
//  * double ended dynamic array
//    [cap|len|start][elem][elem]...
//                    ^
//    dearr elt ptr   |
//
// USAGE
//
//   struct fix_ints {
//      int *PREFIX_;
//   };
//   struct dyn_ints {
//      int *PREFIX_array;
//   };
//   struct dea_ints {
//      int *PREFIX_dearr;
//   };
//
//   struct fix_ints fix_ints = {0};
//   struct dea_ints dea_ints = {0};
//   TODO
//
//   struct dyn_ints dyn_ints = {0};
//   array_push(&dyn_ints, 1);
//
//   int elt = 2;
//   array_push_elt(&dyn_ints, &elt);
//
//   int elts[] = {3, 4, 5};
//   array_push_elts(&dyn_ints, elts, prh_array_size(elts));
//
//   array_pop(&dyn_ints);
//   array_pop_elts(&dyn_ints, 2);
//
//   array_len(&dyn_ints);
//   array_clear(&dyn_ints);
//   array_free(&dyn_ints);
//
// struct SliceStruct {
//      T *ptr;
//      prh_unt len;
//      prh_unt cap;
// };
#ifdef PRH_ARRAY_INCLUDE
#define prh_impl_arrfix_elt(s, a) (s->prh_macro_concat_name(a, _arrfix_type))
#define prh_impl_arrfit_elt(s, a) (s->prh_macro_concat_name(a, _arrfit_type))
#define prh_impl_arrdyn_elt(s, a) (s->prh_macro_concat_name(a, _arrdyn_type))
#define prh_impl_arrlax_elt(s, a) (s->prh_macro_concat_name(a, _arrlax_type))
#define prh_impl_arrfix_size(s, a) sizeof(*prh_impl_arrfix_elt(s, a))
#define prh_impl_arrfit_size(s, a) sizeof(*prh_impl_arrfit_elt(s, a))
#define prh_impl_arrdyn_size(s, a) sizeof(*prh_impl_arrdyn_elt(s, a))
#define prh_impl_arrlax_size(s, a) sizeof(*prh_impl_arrlax_elt(s, a))
#define prh_impl_arrfix_type(s, a) prh_typeof(prh_impl_arrfix_elt(s, a))
#define prh_impl_arrfit_type(s, a) prh_typeof(prh_impl_arrfit_elt(s, a))
#define prh_impl_arrdyn_type(s, a) prh_typeof(prh_impl_arrdyn_elt(s, a))
#define prh_impl_arrlax_type(s, a) prh_typeof(prh_impl_arrlax_elt(s, a))
#define prh_impl_arrfix_data(s, a) (prh_byte *)prh_impl_arrfix_elt(s, a)
#define prh_impl_arrfit_data(s, a) (prh_byte *)prh_impl_arrfit_elt(s, a)
#define prh_impl_arrdyn_data(s, a) (prh_byte *)prh_impl_arrdyn_elt(s, a)
#define prh_impl_arrlax_data(s, a) (prh_byte *)prh_impl_arrlax_elt(s, a)
#define prh_impl_arrfix_addr(s, a) (prh_impl_array *)&prh_impl_arrfix_elt(s, a)
#define prh_impl_arrfit_addr(s, a) (prh_impl_array *)&prh_impl_arrfit_elt(s, a)
#define prh_impl_arrdyn_addr(s, a) (prh_impl_array *)&prh_impl_arrdyn_elt(s, a)
#define prh_impl_arrlax_addr(s, a) (prh_impl_array *)&prh_impl_arrlax_elt(s, a)

#define prh_impl_arrfix_dvar(a) prh_macro_concat_name(a, _arrfix_type)
#define prh_impl_arrfit_dvar(a) prh_macro_concat_name(a, _arrfit_type)
#define prh_impl_arrdyn_dvar(a) prh_macro_concat_name(a, _arrdyn_type)
#define prh_impl_arrlax_dvar(a) prh_macro_concat_name(a, _arrlax_type)
#define prh_impl_arrfix_dvar_size(a) sizeof(*prh_impl_arrfix_dvar(a))
#define prh_impl_arrfit_dvar_size(a) sizeof(*prh_impl_arrfit_dvar(a))
#define prh_impl_arrdyn_dvar_size(a) sizeof(*prh_impl_arrdyn_dvar(a))
#define prh_impl_arrlax_dvar_size(a) sizeof(*prh_impl_arrlax_dvar(a))
#define prh_impl_arrfix_dvar_type(a) prh_typeof(prh_impl_arrfix_dvar(a))
#define prh_impl_arrfit_dvar_type(a) prh_typeof(prh_impl_arrfit_dvar(a))
#define prh_impl_arrdyn_dvar_type(a) prh_typeof(prh_impl_arrdyn_dvar(a))
#define prh_impl_arrlax_dvar_type(a) prh_typeof(prh_impl_arrlax_dvar(a))
#define prh_impl_arrfix_dvar_data(a) (prh_byte *)prh_impl_arrfix_dvar(a)
#define prh_impl_arrfit_dvar_data(a) (prh_byte *)prh_impl_arrfit_dvar(a)
#define prh_impl_arrdyn_dvar_data(a) (prh_byte *)prh_impl_arrdyn_dvar(a)
#define prh_impl_arrlax_dvar_data(a) (prh_byte *)prh_impl_arrlax_dvar(a)

#define prh_impl_arrfix_avar(a) prh_macro_concat_name(a, _arrfix_addr_type)
#define prh_impl_arrfit_avar(a) prh_macro_concat_name(a, _arrfit_addr_type)
#define prh_impl_arrdyn_avar(a) prh_macro_concat_name(a, _arrdyn_addr_type)
#define prh_impl_arrlax_avar(a) prh_macro_concat_name(a, _arrlax_addr_type)
#define prh_impl_arrfix_avar_size(a) sizeof(**prh_impl_arrfix_avar(a))
#define prh_impl_arrfit_avar_size(a) sizeof(**prh_impl_arrfit_avar(a))
#define prh_impl_arrdyn_avar_size(a) sizeof(**prh_impl_arrdyn_avar(a))
#define prh_impl_arrlax_avar_size(a) sizeof(**prh_impl_arrlax_avar(a))
#define prh_impl_arrfix_avar_type(a) prh_typeof(*prh_impl_arrfix_avar(a))
#define prh_impl_arrfit_avar_type(a) prh_typeof(*prh_impl_arrfit_avar(a))
#define prh_impl_arrdyn_avar_type(a) prh_typeof(*prh_impl_arrdyn_avar(a))
#define prh_impl_arrlax_avar_type(a) prh_typeof(*prh_impl_arrlax_avar(a))
#define prh_impl_arrfix_avar_data(a) (prh_byte *)*prh_impl_arrfix_avar(a)
#define prh_impl_arrfit_avar_data(a) (prh_byte *)*prh_impl_arrfit_avar(a)
#define prh_impl_arrdyn_avar_data(a) (prh_byte *)*prh_impl_arrdyn_avar(a)
#define prh_impl_arrlax_avar_data(a) (prh_byte *)*prh_impl_arrlax_avar(a)
#define prh_impl_arrfix_avar_addr(a) (prh_impl_array *)prh_impl_arrfix_avar(a)
#define prh_impl_arrfit_avar_addr(a) (prh_impl_array *)prh_impl_arrfit_avar(a)
#define prh_impl_arrdyn_avar_addr(a) (prh_impl_array *)prh_impl_arrdyn_avar(a)
#define prh_impl_arrlax_avar_addr(a) (prh_impl_array *)prh_impl_arrlax_avar(a)

#define prh_define_arrvew_struct(struct_name, T)                                \
    typedef struct {                                                            \
        prh_int count;                                                          \
        T* data;                                                                \
    } struct_name

typedef struct { // 只读不拥有内存仅对实际数组进行引用
    prh_int count; // count number of array elements
    prh_byte *data;
} prh_impl_arrvew; // read-only array view

#define prh_arrfit_view(s, a, view) {                                           \
    prh_byte *prh_impl_p = prh_impl_arrfit_data((s), a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p;                                                    \
}
#define prh_arrdyn_view(s, a, view) {                                           \
    prh_byte *prh_impl_p = prh_impl_arrdyn_data((s), a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p;                                                    \
}
#define prh_arrlax_view(s, a, view) {                                           \
    prh_byte *prh_impl_p = prh_impl_arrlax_data((s), a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p + *prh_impl_arr_start(prh_impl_p);                  \
}

#define prh_arrfit_v_view(a, view) {                                            \
    prh_byte *prh_impl_p = prh_impl_arrfit_dvar_data(a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p;                                                    \
}
#define prh_arrdyn_v_view(a, view) {                                            \
    prh_byte *prh_impl_p = prh_impl_arrdyn_dvar_data(a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p;                                                    \
}
#define prh_arrlax_v_view(a, view) {                                            \
    prh_byte *prh_impl_p = prh_impl_arrlax_dvar_data(a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p + *prh_impl_arr_start(prh_impl_p);                  \
}

#define prh_arrfit_a_view(a, view) {                                            \
    prh_byte *prh_impl_p = prh_impl_arrfit_avar_data(a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p;                                                    \
}
#define prh_arrdyn_a_view(a, view) {                                            \
    prh_byte *prh_impl_p = prh_impl_arrdyn_avar_data(a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p;                                                    \
}
#define prh_arrlax_a_view(a, view) {                                            \
    prh_byte *prh_impl_p = prh_impl_arrlax_avar_data(a);                        \
    prh_assert(prh_impl_p != prh_null);                                         \
    view->count = *prh_impl_arr_len(prh_impl_p);                                \
    view->data = prh_impl_p + *prh_impl_arr_start(prh_impl_p);                  \
}

#define prh_arrdyn_addr(s, a) &prh_impl_arrdyn_elt((s), a)
#define prh_arrlax_addr(s, a) &prh_impl_arrlax_elt((s), a)

typedef struct { // 数组分配后大小固定
    prh_int count;
} prh_impl_arrfix_hdr; // array with fixed size

typedef struct { // 数组分配后容量固定，因此可以有限的调整大小，由于不会重新分配内存，该数组迭代器永远不会失效
    prh_int capacity;
    prh_int count;
} prh_impl_arrfit_hdr; // the allocated array is fixed in the place, never realloc

typedef struct { // 动态容量动态大小的数组，数组容量的调整会使数组迭代器指针失效
    prh_int capacity;
    prh_int count;
} prh_impl_arrdyn_hdr; // array can dynamic re-allocated and re-sized

typedef struct { // 动态数组头部空间可伸展
    prh_int start;
    prh_unt extra;
    prh_int capacity;
    prh_int count;
} prh_impl_arrlax_hdr; // relaxed array - header space can stretch around

typedef struct {
    prh_byte *data;
} prh_impl_array;

prh_inline prh_int *prh_impl_arr_len(prh_byte *elem_ptr) {
    return (prh_int *)elem_ptr - 1;
}

prh_inline prh_int *prh_impl_arr_cap(prh_byte *elem_ptr) {
    return (prh_int *)((prh_impl_arrdyn_hdr *)elem_ptr - 1);
}

prh_inline prh_int *prh_impl_arr_start(prh_byte *elem_ptr) {
    return (prh_int *)((prh_impl_arrlax_hdr *)elem_ptr - 1);
}

prh_inline void prh_impl_array_clear(prh_byte *elem_ptr) {
    prh_assert(elem_ptr != prh_null);
    *prh_impl_arr_len(elem_ptr) = 0;
}

#define prh_arrfit_clear(s, a) prh_impl_array_clear(prh_impl_arrfit_data((s), a))
#define prh_arrdyn_clear(s, a) prh_impl_array_clear(prh_impl_arrdyn_data((s), a))
#define prh_arrlax_clear(s, a) prh_impl_array_clear(prh_impl_arrlax_data((s), a))

#define prh_arrfit_v_clear(a) prh_impl_array_clear(prh_impl_arrfit_dvar_data(a))
#define prh_arrdyn_v_clear(a) prh_impl_array_clear(prh_impl_arrdyn_dvar_data(a))
#define prh_arrlax_v_clear(a) prh_impl_array_clear(prh_impl_arrlax_dvar_data(a))

void prh_impl_array_free(prh_impl_array *array, prh_int hdr_size);

#define prh_arrfit_free(s, a) prh_impl_array_free(prh_impl_arrfit_addr((s), a), sizeof(prh_impl_arrfit_hdr))
#define prh_arrdyn_free(s, a) prh_impl_array_free(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr))
#define prh_arrlax_free(s, a) prh_impl_array_free(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr))

#define prh_arrfit_a_free(a) prh_impl_array_free(prh_impl_arrfit_avar_addr(a), sizeof(prh_impl_arrfit_hdr))
#define prh_arrdyn_a_free(a) prh_impl_array_free(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr))
#define prh_arrlax_a_free(a) prh_impl_array_free(prh_impl_arrlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr))

void *prh_impl_arrdyn_init(prh_int capacity, prh_int elem_size);
void *prh_impl_arrlax_init(prh_int capacity, prh_int elem_size);

#define prh_arrfit_init(s, a, cap) prh_impl_arrfit_elt((s), a) = prh_impl_arrdyn_init((cap), prh_impl_arrfit_size((s), a))
#define prh_arrdyn_init(s, a, cap) prh_impl_arrdyn_elt((s), a) = prh_impl_arrdyn_init((cap), prh_impl_arrdyn_size((s), a))
#define prh_arrlax_init(s, a, cap) prh_impl_arrlax_elt((s), a) = prh_impl_arrlax_init((cap), prh_impl_arrlax_size((s), a))

void prh_impl_array_reserve(prh_impl_array *array, prh_int hdr_size, prh_int new_cap, prh_int elem_size);
void prh_impl_array_expand(prh_impl_array *array, prh_int hdr_size, prh_int num_grow_elts, prh_int elem_size);
void prh_impl_array_shrink(prh_impl_array *array, prh_int hdr_size, prh_int new_cap, prh_int elem_size);

#define prh_arrdyn_expand(s, a, num_grow_elts) prh_impl_array_expand(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), (num_grow_elts), prh_impl_arrdyn_size((s), a))
#define prh_arrlax_expand(s, a, num_grow_elts) prh_impl_array_expand(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), (num_grow_elts), prh_impl_arrlax_size((s), a))
#define prh_arrdyn_shrink(s, a, new_cap) prh_impl_array_shrink(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), (new_cap), prh_impl_arrdyn_size((s), a))
#define prh_arrlax_shrink(s, a, new_cap) prh_impl_array_shrink(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), (new_cap), prh_impl_arrlax_size((s), a))

#define prh_arrdyn_a_expand(a, num_grow_elts) prh_impl_array_expand(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), (num_grow_elts), prh_impl_arrdyn_avar_size(a))
#define prh_arrlax_a_expand(a, num_grow_elts) prh_impl_array_expand(prh_impl_arrlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), (num_grow_elts), prh_impl_arrlax_avar_size(a))
#define prh_arrdyn_a_shrink(a, new_cap) prh_impl_array_shrink(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), (new_cap), prh_impl_arrdyn_avar_size(a))
#define prh_arrlax_a_shrink(a, new_cap) prh_impl_array_shrink(prh_impl_arrlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), (new_cap), prh_impl_arrlax_avar_size(a))

void prh_impl_arrfit_resize(prh_byte *elem_ptr, prh_int new_len);
void prh_impl_arrdyn_resize(prh_impl_array *array, prh_int hdr_size, prh_int new_len, prh_int elem_size);

#define prh_arrfit_resize(s, a, new_len) prh_impl_arrfit_resize(prh_impl_arrfit_data((s), a), (new_len))
#define prh_arrdyn_resize(s, a, new_len) prh_impl_arrdyn_resize(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), (new_len), prh_impl_arrdyn_size((s), a))
#define prh_arrlax_resize(s, a, new_len) prh_impl_arrdyn_resize(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), (new_len), prh_impl_arrlax_size((s), a))

#define prh_arrfit_v_resize(a, new_len) prh_impl_arrfit_resize(prh_impl_arrfit_dvar_data(a), (new_len))
#define prh_arrdyn_a_resize(a, new_len) prh_impl_arrdyn_resize(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), (new_len), prh_impl_arrdyn_avar_size(a))
#define prh_arrlax_a_resize(a, new_len) prh_impl_arrdyn_resize(prh_impl_arrlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), (new_len), prh_impl_arrlax_avar_size(a))

void *prh_impl_arrfit_push(prh_byte *elem_ptr, prh_int num_elts, prh_int elem_size);
void *prh_impl_arrfit_try_push(prh_byte *elem_ptr, prh_int *num_elts_ptr, prh_int elem_size);
void *prh_impl_arrdyn_push(prh_impl_array *array, prh_int hdr_size, prh_int num_elts, prh_int elem_size);
void *prh_impl_array_unchecked_push(prh_byte *elem_ptr, prh_int num_elts, prh_int elem_size);

#define prh_arrfit_push(s, a, elt_value) {                                      \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_arrfit_push(prh_impl_arrfit_data((s), a), 1, prh_impl_arrfit_size((s), a)); \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_arrdyn_push(s, a, elt_value) {                                      \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), 1, prh_impl_arrdyn_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_push(s, a, elt_value) {                                      \
    prh_impl_arrlax_type((s), a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), 1, prh_impl_arrlax_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrfit_v_push(a, elt_value) {                                       \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_arrfit_push(prh_impl_arrfit_dvar_data(a), 1, prh_impl_arrfit_dvar_size(a)); \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_arrdyn_a_push(a, elt_value) {                                       \
    prh_impl_arrdyn_avar_type(a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), 1, prh_impl_arrdyn_avar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_a_push(a, elt_value) {                                       \
    prh_impl_arrlax_avar_type(a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), 1, prh_impl_arrlax_avar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrfit_push_many(s, a, elt_ptr, num_elts) {                         \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_arrfit_push(prh_impl_arrfit_data((s), a), prh_impl_n, prh_impl_arrfit_size((s), a)); \
    if (prh_impl_p) memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrfit_size((s), a)); \
}
#define prh_arrdyn_push_many(s, a, elt_ptr, num_elts) {                         \
    prh_int prh_impl_n = (prh_int)(num_elts); /*个数可以为零此时拷贝不生效*/       \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), prh_impl_n, prh_impl_arrdyn_size((s), a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrdyn_size((s), a));   \
}
#define prh_arrlax_push_many(s, a, elt_ptr, num_elts) {                         \
    prh_int prh_impl_n = (prh_int)(num_elts); /*个数可以为零此时拷贝不生效*/       \
    prh_impl_arrlax_type((s), a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), prh_impl_n, prh_impl_arrlax_size((s), a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrlax_size((s), a));   \
}
#define prh_arrfit_v_push_many(a, elt_ptr, num_elts) {                          \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_arrfit_push(prh_impl_arrfit_dvar_data(a), prh_impl_n, prh_impl_arrfit_dvar_size(a)); \
    if (prh_impl_p) memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrfit_dvar_size(a)); \
}
#define prh_arrdyn_a_push_many(a, elt_ptr, num_elts) {                          \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_impl_arrdyn_avar_type(a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), prh_impl_n, prh_impl_arrdyn_avar_size(a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrdyn_avar_size(a));   \
}
#define prh_arrlax_a_push_many(a, elt_ptr, num_elts) {                          \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_impl_arrlax_avar_type(a) prh_impl_p = prh_impl_arrdyn_push(prh_impl_arrlax_avar_addr((s), a), sizeof(prh_impl_arrlax_hdr), prh_impl_n, prh_impl_arrlax_avar_size(a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrlax_avar_size(a));   \
}

#define prh_arrfit_unchecked_push(s, a, elt_value) {                            \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrfit_data((s), a), 1, prh_impl_arrfit_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrdyn_unchecked_push(s, a, elt_value) {                            \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrdyn_data((s), a), 1, prh_impl_arrdyn_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_unchecked_push(s, a, elt_value) {                            \
    prh_impl_arrlax_type((s), a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrlax_data((s), a), 1, prh_impl_arrlax_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrfit_v_unchecked_push(a, elt_value) {                             \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrfit_dvar_data(a), 1, prh_impl_arrfit_dvar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrdyn_v_unchecked_push(a, elt_value) {                             \
    prh_impl_arrdyn_dvar_type(a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrdyn_dvar_data(a), 1, prh_impl_arrdyn_dvar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_v_unchecked_push(a, elt_value) {                             \
    prh_impl_arrlax_dvar_type(a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrlax_dvar_data(a), 1, prh_impl_arrlax_dvar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrfit_unchecked_push_many(s, a, elt_ptr, num_elts) {               \
    prh_int prh_impl_n = (prh_int)(num_elts); /*个数可以为零此时拷贝不生效*/       \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrfit_data((s), a), prh_impl_n, prh_impl_arrfit_size((s), a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrfit_size((s), a));   \
}
#define prh_arrdyn_unchecked_push_many(s, a, elt_ptr, num_elts) {               \
    prh_int prh_impl_n = (prh_int)(num_elts); /*个数可以为零此时拷贝不生效*/       \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrdyn_data((s), a), prh_impl_n, prh_impl_arrdyn_size((s), a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrdyn_size((s), a));   \
}
#define prh_arrlax_unchecked_push_many(s, a, elt_ptr, num_elts) {               \
    prh_int prh_impl_n = (prh_int)(num_elts); /*个数可以为零此时拷贝不生效*/       \
    prh_impl_arrlax_type((s), a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrlax_data((s), a), prh_impl_n, prh_impl_arrlax_size((s), a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrlax_size((s), a));   \
}
#define prh_arrfit_v_unchecked_push_many(a, elt_ptr, num_elts) {                \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrfit_dvar_data(a), prh_impl_n, prh_impl_arrfit_dvar_size(a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrfit_dvar_size(a));   \
}
#define prh_arrdyn_v_unchecked_push_many(a, elt_ptr, num_elts) {                \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_impl_arrdyn_dvar_type(a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrdyn_dvar_data(a), prh_impl_n, prh_impl_arrdyn_dvar_size(a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrdyn_dvar_size(a));   \
}
#define prh_arrlax_v_unchecked_push_many(a, elt_ptr, num_elts) {                \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_impl_arrlax_dvar_type(a) prh_impl_p = prh_impl_array_unchecked_push(prh_impl_arrlax_dvar_data(a), prh_impl_n, prh_impl_arrlax_dvar_size(a)); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n * prh_impl_arrlax_dvar_size(a));   \
}

void prh_impl_array_erase(prh_byte *elem_ptr, prh_int start, prh_int i, prh_int elem_size);

#define prh_arrfit_erase(s, a, i) prh_impl_array_erase(prh_impl_arrfit_data((s), a), 0, (i), prh_impl_arrfit_size((s), a))
#define prh_arrdyn_erase(s, a, i) prh_impl_array_erase(prh_impl_arrdyn_data((s), a), 0, (i), prh_impl_arrdyn_size((s), a))
#define prh_arrlax_erase(s, a, i) {                                             \
    prh_byte *elem_ptr = prh_impl_arrlax_data((s), a);                          \
    prh_impl_array_erase(elem_ptr, *prh_impl_arr_start(elem_ptr), (i), prh_impl_arrlax_size((s), a)); \
}
#define prh_arrfit_v_erase(a, i) prh_impl_array_erase(prh_impl_arrfit_dvar_data(a), 0, (i), prh_impl_arrfit_dvar_size(a))
#define prh_arrdyn_v_erase(a, i) prh_impl_array_erase(prh_impl_arrdyn_dvar_data(a), 0, (i), prh_impl_arrdyn_dvar_size(a))
#define prh_arrlax_v_erase(a, i) {                                              \
    prh_byte *elem_ptr = prh_impl_arrlax_dvar_data(a);                          \
    prh_impl_array_erase(elem_ptr, *prh_impl_arr_start(elem_ptr), (i), prh_impl_arrlax_dvar_size(a)); \
}

prh_inline prh_int prh_impl_array_unordered_erase(void *elem_ptr, prh_int i) {
    prh_int *count_ptr = prh_impl_arr_len((prh_byte *)elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i < *count_ptr); // 数组为空时无法移除元素
    return --(*count_ptr);
}

prh_inline prh_int prh_impl_array_unordered_insert(void *elem_ptr, prh_int i) {
    prh_int *count_ptr = prh_impl_arr_len((prh_byte *)elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i <= *count_ptr);
    return (*count_ptr)++;
}

#define prh_arrfit_unordered_erase(s, a, i) {                                   \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_arrfit_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i);                                          \
    prh_impl_p[prh_impl_i] = prh_impl_p[prh_impl_array_unordered_erase(prh_impl_p, prh_impl_i)]; \
}
#define prh_arrdyn_unordered_erase(s, a, i) {                                   \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_arrdyn_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i);                                          \
    prh_impl_p[prh_impl_i] = prh_impl_p[prh_impl_array_unordered_erase(prh_impl_p, prh_impl_i)]; \
}
#define prh_arrlax_unordered_erase(s, a, i) {                                   \
    prh_impl_arrlax_type((s), a) prh_impl_p = prh_impl_arrlax_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i);                                          \
    prh_impl_p += *prh_impl_arr_start((prh_byte *)prh_impl_p);                  \
    prh_impl_p[prh_impl_i] = prh_impl_p[prh_impl_array_unordered_erase(prh_impl_p, prh_impl_i)]; \
}
#define prh_arrfit_v_unordered_erase(a, i) {                                    \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_arrfit_dvar(a);          \
    prh_int prh_impl_i = (prh_int)(i);                                          \
    prh_impl_p[prh_impl_i] = prh_impl_p[prh_impl_array_unordered_erase(prh_impl_p, prh_impl_i)]; \
}
#define prh_arrdyn_v_unordered_erase(a, i) {                                    \
    prh_impl_arrdyn_dvar_type(a) prh_impl_p = prh_impl_arrdyn_dvar(a);          \
    prh_int prh_impl_i = (prh_int)(i);                                          \
    prh_impl_p[prh_impl_i] = prh_impl_p[prh_impl_array_unordered_erase(prh_impl_p, prh_impl_i)]; \
}
#define prh_arrlax_v_unordered_erase(a, i) {                                    \
    prh_impl_arrlax_dvar_type(a) prh_impl_p = prh_impl_arrlax_dvar(a);          \
    prh_int prh_impl_i = (prh_int)(i);                                          \
    prh_impl_p += *prh_impl_arr_start((prh_byte *)prh_impl_p);                  \
    prh_impl_p[prh_impl_i] = prh_impl_p[prh_impl_array_unordered_erase(prh_impl_p, prh_impl_i)]; \
}

void *prh_impl_arrfit_insert(prh_byte *elem_ptr, prh_int i, prh_int elem_size);
void *prh_impl_arrfit_unchecked_insert(prh_byte *elem_ptr, prh_int i, prh_int elem_size);
void *prh_impl_arrdyn_insert(prh_byte *elem_ptr, prh_int start, prh_int i, prh_int elem_size);

#define prh_arrfit_insert(s, a, i, elt_value) {                                 \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_arrfit_insert(prh_impl_arrfit_data((s), a), (i), prh_impl_arrfit_size((s), a)); \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_arrdyn_insert(s, a, i, elt_value) {                                 \
    prh_impl_array_expand(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), 1, prh_impl_arrdyn_size((s), a)); \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_arrdyn_insert(prh_impl_arrdyn_data((s), a), 0, (i), prh_impl_arrdyn_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_insert(s, a, i, elt_value) {                                 \
    prh_impl_array_expand(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), 1, prh_impl_arrlax_size((s), a)); \
    prh_byte *prh_impl_p = prh_impl_arrlax_data((s), a);                        \
    prh_impl_arrlax_type((s), a) prh_impl_elt = prh_impl_arrdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i), prh_impl_arrlax_size((s), a)); \
    *prh_impl_elt = (elt_value);                                                \
}
#define prh_arrfit_v_insert(a, i, elt_value) {                                  \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_arrfit_insert(prh_impl_arrfit_dvar_data(a), (i), prh_impl_arrfit_dvar_size(a)); \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_arrdyn_a_insert(a, i, elt_value) {                                  \
    prh_impl_array_expand(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), 1, prh_impl_arrdyn_avar_size(a)); \
    prh_impl_arrdyn_avar_type(a) prh_impl_p = prh_impl_arrdyn_insert(prh_impl_arrdyn_avar_data(a), 0, (i), prh_impl_arrdyn_avar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_a_insert(a, i, elt_value) {                                  \
    prh_impl_array_expand(prh_impl_arrlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), 1, prh_impl_arrlax_avar_size(a)); \
    prh_byte *prh_impl_p = prh_impl_arrlax_avar_data(a);                        \
    prh_impl_arrlax_avar_type(a) prh_impl_elt = prh_impl_arrdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i), prh_impl_arrlax_avar_size(a)); \
    *prh_impl_elt = (elt_value);                                                \
}

#define prh_arrfit_unchecked_insert(s, a, i, elt_value) {                       \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_arrfit_unchecked_insert(prh_impl_arrfit_data((s), a), (i), prh_impl_arrfit_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrdyn_unchecked_insert(s, a, i, elt_value) {                       \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_arrdyn_insert(prh_impl_arrdyn_data((s), a), 0, (i), prh_impl_arrdyn_size((s), a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_unchecked_insert(s, a, i, elt_value) {                       \
    prh_byte *prh_impl_p = prh_impl_arrlax_data((s), a);                        \
    prh_impl_arrlax_type((s), a) prh_impl_elt = prh_impl_arrdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i), prh_impl_arrlax_size((s), a)); \
    *prh_impl_elt = (elt_value);                                                \
}
#define prh_arrfit_v_unchecked_insert(a, i, elt_value) {                        \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_arrfit_unchecked_insert(prh_impl_arrfit_dvar_data(a), (i), prh_impl_arrfit_dvar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrdyn_v_unchecked_insert(a, i, elt_value) {                        \
    prh_impl_arrdyn_dvar_type(a) prh_impl_p = prh_impl_arrdyn_insert(prh_impl_arrdyn_dvar_data(a), 0, (i), prh_impl_arrdyn_dvar_size(a)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_arrlax_v_unchecked_insert(a, i, elt_value) {                        \
    prh_byte *prh_impl_p = prh_impl_arrlax_dvar_data(a);                        \
    prh_impl_arrlax_dvar_type(a) prh_impl_elt = prh_impl_arrdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i), prh_impl_arrlax_dvar_size(a)); \
    *prh_impl_elt = (elt_value);                                                \
}

#define prh_arrfit_unordered_insert(s, a, i, elt_value) {                       \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_arrfit_elt((s), a);      \
    if (*prh_impl_arr_len((prh_byte *)prh_impl_p) < *prh_impl_arr_cap((prh_byte *)prh_impl_p)) { \
        prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
        prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                     \
        prh_impl_p[prh_impl_i] = (elt_value);                                   \
    }                                                                           \
}
#define prh_arrdyn_unordered_insert(s, a, i, elt_value) {                       \
    prh_impl_array_expand(prh_impl_arrdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), 1, prh_impl_arrdyn_size((s), a)); \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_arrdyn_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrlax_unordered_insert(s, a, i, elt_value) {                       \
    prh_impl_array_expand(prh_impl_arrlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), 1, prh_impl_arrlax_size((s), a)); \
    prh_impl_arrlax_type((s), a) prh_impl_p = prh_impl_arrlax_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_start = *prh_impl_arr_start((prh_byte *)prh_impl_p); \
    prh_int prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p += prh_impl_start;                                               \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrfit_unchecked_unordered_insert(s, a, i, elt_value) {             \
    prh_impl_arrfit_type((s), a) prh_impl_p = prh_impl_arrfit_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrdyn_unchecked_unordered_insert(s, a, i, elt_value) {             \
    prh_impl_arrdyn_type((s), a) prh_impl_p = prh_impl_arrdyn_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrlax_unchecked_unordered_insert(s, a, i, elt_value) {             \
    prh_impl_arrlax_type((s), a) prh_impl_p = prh_impl_arrlax_elt((s), a);      \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_start = *prh_impl_arr_start((prh_byte *)prh_impl_p); \
    prh_int prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p += prh_impl_start;                                               \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}

#define prh_arrfit_v_unordered_insert(a, i, elt_value) {                        \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_arrfit_dvar(a);          \
    if (*prh_impl_arr_len((prh_byte *)prh_impl_p) < *prh_impl_arr_cap((prh_byte *)prh_impl_p)) { \
        prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
        prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                     \
        prh_impl_p[prh_impl_i] = (elt_value);                                   \
    }                                                                           \
}
#define prh_arrdyn_a_unordered_insert(a, i, elt_value) {                        \
    prh_impl_array_expand(prh_impl_arrdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), 1, prh_impl_arrdyn_avar_size(a)); \
    prh_impl_arrdyn_avar_type(a) prh_impl_p = prh_impl_arrdyn_avar(a);          \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrlax_a_unordered_insert(a, i, elt_value) {                        \
    prh_impl_array_expand(prh_impl_arrlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), 1, prh_impl_arrlax_avar_size(a)); \
    prh_impl_arrlax_avar_type(a) prh_impl_p = prh_impl_arrlax_avar(a);          \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_start = *prh_impl_arr_start((prh_byte *)prh_impl_p); \
    prh_int prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p += prh_impl_start;                                               \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrfit_v_unchecked_unordered_insert(a, i, elt_value) {              \
    prh_impl_arrfit_dvar_type(a) prh_impl_p = prh_impl_arrfit_dvar(a);          \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrdyn_v_unchecked_unordered_insert(a, i, elt_value) {              \
    prh_impl_arrdyn_dvar_type(a) prh_impl_p = prh_impl_arrdyn_dvar(a);          \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}
#define prh_arrlax_v_unchecked_unordered_insert(a, i, elt_value) {              \
    prh_impl_arrlax_dvar_type(a) prh_impl_p = prh_impl_arrlax_dvar(a);          \
    prh_int prh_impl_i = (prh_int)(i), prh_impl_start = *prh_impl_arr_start((prh_byte *)prh_impl_p); \
    prh_int prh_impl_last = prh_impl_array_unordered_insert(prh_impl_p, prh_impl_i); \
    prh_impl_p += prh_impl_start;                                               \
    prh_impl_p[prh_impl_last] = prh_impl_p[prh_impl_i];                         \
    prh_impl_p[prh_impl_i] = (elt_value);                                       \
}

#define prh_arrfix_len(s, a) *prh_impl_arr_len(prh_impl_arrfix_data((s), a))
#define prh_arrfix_data(s, a) prh_impl_arrfix_elt((s), a)
#define prh_arrfix_end(s, a) prh_impl_arrfix_elt((s), a) + *prh_impl_arr_len(prh_impl_arrfix_data((s), a))
#define prh_arrfix_get(s, a, i) prh_impl_arrfix_elt((s), a) + (i)
#define prh_arrfix_v_len(a) *prh_impl_arr_len(prh_impl_arrfix_dvar_data(a))
#define prh_arrfix_v_data(a) prh_impl_arrfix_dvar(a)
#define prh_arrfix_v_end(a) prh_impl_arrfix_dvar(a) + *prh_impl_arr_len(prh_impl_arrfix_dvar_data(a))
#define prh_arrfix_v_get(a, i) prh_impl_arrfix_dvar(a) + (i)

#define prh_arrfit_cap(s, a) *prh_impl_arr_cap(prh_impl_arrfit_data((s), a))
#define prh_arrfit_len(s, a) *prh_impl_arr_len(prh_impl_arrfit_data((s), a))
#define prh_arrfit_data(s, a) prh_impl_arrfit_elt((s), a)
#define prh_arrfit_end(s, a) prh_impl_arrfit_elt((s), a) + *prh_impl_arr_len(prh_impl_arrfit_data((s), a))
#define prh_arrfit_get(s, a, i) prh_impl_arrfit_elt((s), a) + (i)
#define prh_arrfit_v_cap(a) *prh_impl_arr_cap(prh_impl_arrfit_dvar_data(a))
#define prh_arrfit_v_len(a) *prh_impl_arr_len(prh_impl_arrfit_dvar_data(a))
#define prh_arrfit_v_data(a) prh_impl_arrfit_dvar(a)
#define prh_arrfit_v_end(a) prh_impl_arrfit_dvar(a) + *prh_impl_arr_len(prh_impl_arrfit_dvar_data(a))
#define prh_arrfit_v_get(a, i) prh_impl_arrfit_dvar(a) + (i)

#define prh_arrdyn_cap(s, a) *prh_impl_arr_cap(prh_impl_arrdyn_data((s), a))
#define prh_arrdyn_len(s, a) *prh_impl_arr_len(prh_impl_arrdyn_data((s), a))
#define prh_arrdyn_data(s, a) prh_impl_arrdyn_elt((s), a)
#define prh_arrdyn_end(s, a) prh_impl_arrfdyn_elt((s), a) + *prh_impl_arr_len(prh_impl_arrdyn_data((s), a))
#define prh_arrdyn_get(s, a, i) prh_impl_arrdyn_elt((s), a) + (i)
#define prh_arrdyn_v_cap(a) *prh_impl_arr_cap(prh_impl_arrdyn_dvar_data(a))
#define prh_arrdyn_v_len(a) *prh_impl_arr_len(prh_impl_arrdyn_dvar_data(a))
#define prh_arrdyn_v_data(a) prh_impl_arrdyn_dvar(a)
#define prh_arrdyn_v_end(a) prh_impl_arrfdyn_dvar(a) + *prh_impl_arr_len(prh_impl_arrdyn_dvar_data(a))
#define prh_arrdyn_v_get(a, i) prh_impl_arrdyn_dvar(a) + (i)

#define prh_arrlax_cap(s, a) *prh_impl_arr_cap(prh_impl_arrlax_data((s), a))
#define prh_arrlax_len(s, a) *prh_impl_arr_len(prh_impl_arrlax_data((s), a))
#define prh_arrlax_data(s, a) prh_impl_arrlax_elt((s), a) + *prh_impl_arr_start(prh_impl_arrlax_data((s), a))
#define prh_arrlax_end(s, a) prh_arrlax_data((s), a) + *prh_impl_arr_len(prh_impl_arrlax_data((s), a))
#define prh_arrlax_get(s, a, i) prh_arrlax_data((s), a) + (i)
#define prh_arrlax_v_cap(a) *prh_impl_arr_cap(prh_impl_arrlax_dvar_data(a))
#define prh_arrlax_v_len(a) *prh_impl_arr_len(prh_impl_arrlax_dvar_data(a))
#define prh_arrlax_v_data(a) prh_impl_arrlax_dvar(a) + *prh_impl_arr_start(prh_impl_arrlax_dvar_data(a))
#define prh_arrlax_v_end(a) prh_arrlax_v_data(a) + *prh_impl_arr_len(prh_impl_arrlax_dvar_data(a))
#define prh_arrlax_v_get(a, i) prh_arrlax_v_data(a) + (i)

#define prh_arrfix_foreach(s, a, it)                                            \
    for (prh_impl_arrfix_type((s), a) prh_impl_start = prh_impl_arrfix_elt((s), a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrfit_foreach(s, a, it)                                            \
    for (prh_impl_arrfit_type((s), a) prh_impl_start = prh_impl_arrfit_elt((s), a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrdyn_foreach(s, a, it)                                            \
    for (prh_impl_arrdyn_type((s), a) prh_impl_start = prh_impl_arrdyn_elt((s), a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrlax_foreach(s, a, it)                                            \
    for (prh_impl_arrlax_type((s), a) prh_impl_start = prh_impl_arrlax_elt((s), a) + *prh_impl_arr_start(prh_impl_arrlax_data((s), a)), \
            it = prh_impl_start, count_ptr = (void *)prh_impl_arr_len(prh_impl_arrlax_data((s), a)); \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)

#define prh_arrfix_v_foreach(a, it)                                             \
    for (prh_impl_arrfix_dvar_type(a) prh_impl_start = prh_impl_arrfix_dvar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrfit_v_foreach(a, it)                                             \
    for (prh_impl_arrfit_dvar_type(a) prh_impl_start = prh_impl_arrfit_dvar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrdyn_v_foreach(a, it)                                             \
    for (prh_impl_arrdyn_dvar_type(a) prh_impl_start = prh_impl_arrdyn_dvar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrlax_v_foreach(a, it)                                             \
    for (prh_impl_arrlax_dvar_type(a) prh_impl_start = prh_impl_arrlax_dvar(a) + *prh_impl_arr_start(prh_impl_arrlax_dvar_data(a)), \
            it = prh_impl_start, count_ptr = (void *)prh_impl_arr_len(prh_impl_arrlax_dvar_data(a)); \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)

#define prh_arrfix_a_foreach(a, it)                                             \
    for (prh_impl_arrfix_avar_type(a) prh_impl_start = prh_impl_arrfix_avar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrfit_a_foreach(a, it)                                             \
    for (prh_impl_arrfit_avar_type(a) prh_impl_start = prh_impl_arrfit_avar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrdyn_a_foreach(a, it)                                             \
    for (prh_impl_arrdyn_avar_type(a) prh_impl_start = prh_impl_arrdyn_avar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)
#define prh_arrlax_a_foreach(a, it)                                             \
    for (prh_impl_arrlax_avar_type(a) prh_impl_start = prh_impl_arrlax_avar(a) + *prh_impl_arr_start(prh_impl_arrlax_avar_data(a)), \
            it = prh_impl_start, count_ptr = (void *)prh_impl_arr_len(prh_impl_arrlax_avar_data(a)); \
            it < prh_impl_start + *(prh_int *)count_ptr; it += 1)

#define prh_arrfix_while(s, a, it)                                              \
    for (prh_impl_arrfix_type((s), a) prh_impl_start = prh_impl_arrfix_elt((s), a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrfit_while(s, a, it)                                              \
    for (prh_impl_arrfit_type((s), a) prh_impl_start = prh_impl_arrfit_elt((s), a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrdyn_while(s, a, it)                                              \
    for (prh_impl_arrdyn_type((s), a) prh_impl_start = prh_impl_arrdyn_elt((s), a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrlax_while(s, a, it)                                              \
    for (prh_impl_arrlax_type((s), a) prh_impl_start = prh_impl_arrlax_elt((s), a) + *prh_impl_arr_start(prh_impl_arrlax_data((s), a)), \
            it = prh_impl_start, count_ptr = (void *)prh_impl_arr_len(prh_impl_arrlax_data((s), a)); \
            it < prh_impl_start + *(prh_int *)count_ptr;)

#define prh_arrfix_v_while(a, it)                                               \
    for (prh_impl_arrfix_dvar_type(a) prh_impl_start = prh_impl_arrfix_dvar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrfit_v_while(a, it)                                               \
    for (prh_impl_arrfit_dvar_type(a) prh_impl_start = prh_impl_arrfit_dvar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrdyn_v_while(a, it)                                               \
    for (prh_impl_arrdyn_dvar_type(a) prh_impl_start = prh_impl_arrdyn_dvar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrlax_v_while(a, it)                                               \
    for (prh_impl_arrlax_dvar_type(a) prh_impl_start = prh_impl_arrlax_dvar(a) + *prh_impl_arr_start(prh_impl_arrlax_dvar_data(a)), \
            it = prh_impl_start, count_ptr = (void *)prh_impl_arr_len(prh_impl_arrlax_dvar_data(a)); \
            it < prh_impl_start + *(prh_int *)count_ptr;)

#define prh_arrfix_a_while(a, it)                                               \
    for (prh_impl_arrfix_avar_type(a) prh_impl_start = prh_impl_arrfix_avar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrfit_a_while(a, it)                                               \
    for (prh_impl_arrfit_avar_type(a) prh_impl_start = prh_impl_arrfit_avar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrdyn_a_while(a, it)                                               \
    for (prh_impl_arrdyn_avar_type(a) prh_impl_start = prh_impl_arrdyn_avar(a), it = prh_impl_start, \
            count_ptr = (void *)prh_impl_arr_len((prh_byte *)it);               \
            it < prh_impl_start + *(prh_int *)count_ptr;)
#define prh_arrlax_a_while(a, it)                                               \
    for (prh_impl_arrlax_avar_type(a) prh_impl_start = prh_impl_arrlax_avar(a) + *prh_impl_arr_start(prh_impl_arrlax_avar_data(a)), \
            it = prh_impl_start, count_ptr = (void *)prh_impl_arr_len(prh_impl_arrlax_avar_data(a)); \
            it < prh_impl_start + *(prh_int *)count_ptr;)

#ifdef PRH_ARRAY_STRIP_PREFIX
#endif // PRH_ARRAY_STRIP_PREFIX

#ifdef PRH_ARRAY_IMPLEMENTAION
void *prh_impl_arrdyn_init(prh_int capacity, prh_int elem_size) {
    prh_impl_array array = {prh_null};
    if (capacity <= 0) return prh_null;
    prh_impl_array_reserve(&array, sizeof(prh_impl_arrdyn_hdr), capacity, elem_size);
    prh_byte *elem_ptr = array->data;
    prh_impl_arrdyn_hdr *p = (prh_impl_arrdyn_hdr *)elem_ptr - 1;
    p->capacity = capacity;
    p->count = 0;
    return elem_ptr;
}

void *prh_impl_arrlax_init(prh_int capacity, prh_int elem_size) {
    prh_impl_array array = {prh_null};
    if (capacity <= 0) return prh_null;
    prh_impl_array_reserve(&array, sizeof(prh_impl_arrlax_hdr), capacity, elem_size);
    prh_byte *elem_ptr = array->data;
    prh_impl_arrlax_hdr *p = (prh_impl_arrlax_hdr *)elem_ptr - 1;
    memset(p, 0, sizeof(prh_impl_arrlax_hdr));
    p->capacity = capacity;
    return elem_ptr;
}

void prh_impl_array_free(prh_impl_array *array, prh_int hdr_size) {
    prh_byte *elem_ptr = array->data;
    prh_assert(elem_ptr != prh_null);
    prh_free(elem_ptr - hdr_size);
    array->data = prh_null;
}

void *prh_impl_arrfit_push(prh_byte *elem_ptr, prh_int num_elts, prh_int elem_size) {
    prh_impl_arrfit_hdr *p = (prh_impl_arrfit_hdr)elem_ptr - 1;
    prh_assert(elem_ptr != prh_null);
    prh_assert(num_elts >= 0); // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    prh_int count = p->count;
    if (count + num_elts > p->capacity) {
        return prh_null;
    }
    p->count += num_elts;
    return elem_ptr + count * elem_size;
}

void *prh_impl_arrfit_try_push(prh_byte *elem_ptr, prh_int *num_elts_ptr, prh_int elem_size) { // push many as possible
    prh_assert(elem_ptr != prh_null);
    prh_assert(*num_elts_ptr >= 0);
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_int capacity = *prh_impl_arr_cap(elem_ptr);
    prh_int count = *count_ptr, num_elts = *num_elts_ptr;
    if (count + num_elts > capacity) {
        num_elts = capacity - count;
        *num_elts_ptr = num_elts;
    }
    *count_ptr += num_elts; // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    return elem_ptr + count * elem_size;
}

void *prh_impl_arrdyn_push(prh_impl_array *array, prh_int hdr_size, prh_int num_elts, prh_int elem_size) {
    prh_impl_array_expand(array, hdr_size, num_elts, elem_size);
    prh_byte *elem_ptr = array->data;
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_int count = *count_ptr;
    *count_ptr += num_elts; // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    return elem_ptr + count * elem_size;
}

void *prh_impl_array_unchecked_push(prh_byte *elem_ptr, prh_int num_elts, prh_int elem_size) {
    prh_byte *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(num_elts >= 0); // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    prh_int count = *count_ptr;
    *count_ptr += num_elts;
    return elem_ptr + count * elem_size;
}

void prh_impl_array_erase(prh_byte *elem_ptr, prh_int start, prh_int i, prh_int elem_size) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i < *count_ptr); // 数组为空不能移除元素
    prh_int new_count = --(*count_ptr);
    prh_byte *dest = elem_ptr + (start + i) * elem_size;
    memmove(dest, dest + elem_size, (new_count - i) * elem_size); // 长度参数可为零
}

void *prh_impl_arrfit_insert(prh_byte *elem_ptr, prh_int i, prh_int elem_size) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i <= *count_ptr);
    prh_int count = *count_ptr;
    if (count >= *prh_impl_arr_cap(elem_ptr)) {
        return prh_null;
    }
    *count_ptr += 1;
    prh_byte *source = elem_ptr + i * elem_size;
    memmove(source + elem_size, source, (count - i) * elem_size); // 长度参数可为零
    return source;
}

void *prh_impl_arrfit_unchecked_insert(prh_byte *elem_ptr, prh_int i, prh_int elem_size) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i <= *count_ptr);
    prh_int count = (*count_ptr)++;
    prh_byte *source = elem_ptr + i * elem_size;
    memmove(source + elem_size, source, (count - i) * elem_size); // 长度参数可为零
    return source;
}

void *prh_impl_arrdyn_insert(prh_byte *elem_ptr, prh_int start, prh_int i, prh_int elem_size) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i <= *count_ptr);
    prh_int count = (*count_ptr)++;
    prh_byte *source = elem_ptr + (start + i) * elem_size;
    memmove(source + elem_size, source, (count - i) * elem_size); // 长度参数可为零
    return source;
}

void prh_impl_array_expand(prh_impl_array *array, prh_int hdr_size, prh_int num_grow_elts, prh_int elem_size) {
    prh_byte *elem_ptr = array->data;
    prh_assert(num_grow_elts >= 0);
    prh_int new_cap;
    if (elem_ptr == prh_null) {
        new_cap = num_grow_elts;
    } else {
        new_cap = num_grow_elts + *prh_impl_arr_len(elem_ptr);
    }
    prh_impl_array_reserve(array, hdr_size, new_cap, elem_size);
}

void prh_impl_array_reserve(prh_impl_array *array, prh_int hdr_size, prh_int new_cap, prh_int elem_size) {
    prh_byte *alloc_hdr, *array_hdr;
    prh_byte *elem_ptr = array->data;
    prh_int capacity;
    if (elem_ptr == prh_null) {
        capacity = 0;
        array_hdr = prh_null;
    } else {
        capacity = *prh_impl_arr_cap(elem_ptr);
        array_hdr = elem_ptr - hdr_size;
    }
    if (new_cap > capacity) {
        if (capacity == 0) { capacity = 2; }
        while (new_cap > capacity) { capacity *= 2; }
        alloc_hdr = prh_realloc(array_hdr, hdr_size + capacity * elem_size);
        prh_assert(alloc_hdr != prh_null); // memory overflow
        array->data = alloc_hdr + hdr_size;
        *prh_impl_arr_cap(array->data) = capacity;
    }
}

void prh_impl_array_shrink(prh_impl_array *array, prh_int hdr_size, prh_int new_cap, prh_int elem_size) {
    prh_byte *alloc_hdr, *elem_ptr = array->data;
    prh_assert(elem_prt != prh_null); // 数组为空不能缩减容量
    prh_byte *array_hdr = elem_ptr - hdr_size;
    prh_int capacity = *prh_impl_arr_cap(elem_ptr) / 2;
    prh_int count = *prh_impl_arr_len(elem_ptr);
    if (new_cap < count) new_cap = count;
    if (new_cap <= capacity) {
        while (new_cap <= capacity / 2) { capacity /= 2; }
        if (capacity == 0) { capacity = 2; }
        alloc_hdr = prh_realloc(array_hdr, hdr_size + capacity * elem_size);
        prh_assert(alloc_hdr != prh_null); // memory overflow
        array->data = alloc_hdr + hdr_size;
        *prh_impl_arr_cap(array->data) = capacity;
    }
}

void prh_impl_arrfit_resize(prh_byte *elem_ptr, prh_int new_len) {
    prh_assert(elem_ptr != prh_null); // 数组为空不能缩减长度
    prh_assert(new_len >= 0);
    if (new_len <= *prh_impl_arr_cap(elem_ptr)) {
        *prh_impl_arr_len(array->data) = new_len;
    }
}

void prh_impl_arrdyn_resize(prh_impl_array *array, prh_int hdr_size, prh_int new_len, prh_int elem_size) {
    prh_assert(new_len >= 0);
    prh_impl_array_reserve(array, hdr_size, new_len, elem_size);
    *prh_impl_arr_len(array->data) = new_len;
}
#endif // PRH_ARRAY_IMPLEMENTAION
#endif // PRH_ARRAY_INCLUDE

#ifdef PRH_STRING_INCLUDE
#define prh_impl_strfix_data(s, a) (s->prh_macro_concat_name(a, _strfix_type))
#define prh_impl_strfit_data(s, a) (s->prh_macro_concat_name(a, _strfit_type))
#define prh_impl_strdyn_data(s, a) (s->prh_macro_concat_name(a, _string_type))
#define prh_impl_strlax_data(s, a) (s->prh_macro_concat_name(a, _strlax_type))

#define prh_impl_strfix_addr(s, a) (prh_impl_array *)&prh_impl_strfix_data(s, a)
#define prh_impl_strfit_addr(s, a) (prh_impl_array *)&prh_impl_strfit_data(s, a)
#define prh_impl_strdyn_addr(s, a) (prh_impl_array *)&prh_impl_strdyn_data(s, a)
#define prh_impl_strlax_addr(s, a) (prh_impl_array *)&prh_impl_strlax_data(s, a)

#define prh_impl_strfix_dvar(a) prh_macro_concat_name(a, _strfix_type)
#define prh_impl_strfit_dvar(a) prh_macro_concat_name(a, _strfit_type)
#define prh_impl_strdyn_dvar(a) prh_macro_concat_name(a, _string_type)
#define prh_impl_strlax_dvar(a) prh_macro_concat_name(a, _strlax_type)

#define prh_impl_strfix_avar(a) (*prh_macro_concat_name(a, _strfix_addr_type))
#define prh_impl_strfit_avar(a) (*prh_macro_concat_name(a, _strfit_addr_type))
#define prh_impl_strdyn_avar(a) (*prh_macro_concat_name(a, _string_addr_type))
#define prh_impl_strlax_avar(a) (*prh_macro_concat_name(a, _strlax_addr_type))

#define prh_impl_strfix_avar_addr(a) (prh_impl_array *)prh_impl_strfix_avar(a)
#define prh_impl_strfit_avar_addr(a) (prh_impl_array *)prh_impl_strfit_avar(a)
#define prh_impl_strdyn_avar_addr(a) (prh_impl_array *)prh_impl_strdyn_avar(a)
#define prh_impl_strlax_avar_addr(a) (prh_impl_array *)prh_impl_strlax_avar(a)

typedef struct {
    prh_int count; // count bytes of string
    prh_byte *data;
} prh_strvew; // read-only string view

prh_inline void prh_impl_string_view(prh_byte *elem_ptr, prh_strvew *view) {
    prh_assert(prh_impl_p != prh_null);
    view->count = *prh_impl_arr_len(prh_impl_p);
    view->data = prh_impl_p;
}

prh_inline void prh_impl_strlax_view(prh_byte *elem_ptr, prh_strvew *view) {
    prh_assert(prh_impl_p != prh_null);
    view->count = *prh_impl_arr_len(prh_impl_p);
    view->data = prh_impl_p + *prh_impl_arr_start(prh_impl_p);
}

#define prh_strfit_view(s, a, view) prh_impl_string_view(prh_impl_strfit_data((s), a), view)
#define prh_string_view(s, a, view) prh_impl_string_view(prh_impl_strdyn_data((s), a), view)
#define prh_strlax_view(s, a, view) prh_impl_strlax_view(prh_impl_strlax_data((s), a), view)

#define prh_strfit_v_view(a, view) prh_impl_string_view(prh_impl_strfit_dvar(a), view)
#define prh_string_v_view(a, view) prh_impl_string_view(prh_impl_strdyn_dvar(a), view)
#define prh_strlax_v_view(a, view) prh_impl_strlax_view(prh_impl_strlax_dvar(a), view)

#define prh_strfit_a_view(a, view) prh_impl_string_view(prh_impl_strfit_avar(a), view)
#define prh_string_a_view(a, view) prh_impl_string_view(prh_impl_strdyn_avar(a), view)
#define prh_strlax_a_view(a, view) prh_impl_strlax_view(prh_impl_strlax_avar(a), view)

#define prh_string_addr(s, a) &prh_impl_string_data((s), a)
#define prh_strlax_addr(s, a) &prh_impl_strlax_data((s), a)

#define prh_strfit_clear(s, a) prh_impl_array_clear(prh_impl_strfit_data((s), a))
#define prh_string_clear(s, a) prh_impl_array_clear(prh_impl_string_data((s), a))
#define prh_strlax_clear(s, a) prh_impl_array_clear(prh_impl_strlax_data((s), a))

#define prh_strfit_v_clear(a) prh_impl_array_clear(prh_impl_strfit_dvar_data(a))
#define prh_string_v_clear(a) prh_impl_array_clear(prh_impl_string_dvar_data(a))
#define prh_strlax_v_clear(a) prh_impl_array_clear(prh_impl_strlax_dvar_data(a))

#define prh_strfit_free(s, a) prh_impl_array_free(prh_impl_strfit_addr((s), a), sizeof(prh_impl_arrfit_hdr))
#define prh_string_free(s, a) prh_impl_array_free(prh_impl_string_addr((s), a), sizeof(prh_impl_arrdyn_hdr))
#define prh_strlax_free(s, a) prh_impl_array_free(prh_impl_strlax_addr((s), a), sizeof(prh_impl_arrlax_hdr))

#define prh_strfit_a_free(a) prh_impl_array_free(prh_impl_strfit_avar_addr(a), sizeof(prh_impl_arrfit_hdr))
#define prh_string_a_free(a) prh_impl_array_free(prh_impl_string_avar_addr(a), sizeof(prh_impl_arrdyn_hdr))
#define prh_strlax_a_free(a) prh_impl_array_free(prh_impl_strlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr))

#define prh_strfit_init(s, a, cap) prh_impl_strfit_data((s), a) = prh_impl_arrdyn_init((cap), 1)
#define prh_string_init(s, a, cap) prh_impl_string_data((s), a) = prh_impl_arrdyn_init((cap), 1)
#define prh_strlax_init(s, a, cap) prh_impl_strlax_data((s), a) = prh_impl_arrlax_init((cap), 1)

#define prh_string_expand(s, a, num_grow_elts) prh_impl_array_expand(prh_impl_strdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), (num_grow_elts), 1)
#define prh_strlax_expand(s, a, num_grow_elts) prh_impl_array_expand(prh_impl_strlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), (num_grow_elts), 1)
#define prh_string_shrink(s, a, new_cap) prh_impl_array_shrink(prh_impl_strdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), (new_cap), 1)
#define prh_strlax_shrink(s, a, new_cap) prh_impl_array_shrink(prh_impl_strlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), (new_cap), 1)

#define prh_string_a_expand(a, num_grow_elts) prh_impl_array_expand(prh_impl_strdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), (num_grow_elts), 1)
#define prh_strlax_a_expand(a, num_grow_elts) prh_impl_array_expand(prh_impl_strlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), (num_grow_elts), 1)
#define prh_string_a_shrink(a, new_cap) prh_impl_array_shrink(prh_impl_strdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), (new_cap), 1)
#define prh_strlax_a_shrink(a, new_cap) prh_impl_array_shrink(prh_impl_strlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), (new_cap), 1)

#define prh_strfit_resize(s, a, new_len) prh_impl_arrfit_resize(prh_impl_strfit_data((s), a), (new_len))
#define prh_string_resize(s, a, new_len) prh_impl_arrdyn_resize(prh_impl_strdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), (new_len), 1)
#define prh_strlax_resize(s, a, new_len) prh_impl_arrdyn_resize(prh_impl_strlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), (new_len), 1)

#define prh_strfit_v_resize(a, new_len) prh_impl_arrfit_resize(prh_impl_strfit_dvar(a), (new_len))
#define prh_string_a_resize(a, new_len) prh_impl_arrdyn_resize(prh_impl_strdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), (new_len), 1)
#define prh_strlax_a_resize(a, new_len) prh_impl_arrdyn_resize(prh_impl_strlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), (new_len), 1)

prh_byte *prh_impl_strfit_push(prh_byte *elem_ptr, prh_int num_elts);
prh_byte *prh_impl_strfit_try_push(prh_byte *elem_ptr, prh_int *num_elts_ptr);
prh_byte *prh_impl_strdyn_push(prh_impl_array *array, prh_int hdr_size, prh_int num_elts);
prh_byte *prh_impl_string_unchecked_push(prh_byte *elem_ptr, prh_int num_elts);

#define prh_strfit_push(s, a, elt_value) {                                      \
    prh_byte *prh_impl_p = prh_impl_strfit_push(prh_impl_strfit_data((s), a), 1); \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_string_push(s, a, elt_value) {                                      \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_push(s, a, elt_value) {                                      \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strfit_v_push(a, elt_value) {                                       \
    prh_byte *prh_impl_p = prh_impl_strfit_push(prh_impl_strfit_dvar(a), 1);    \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_string_a_push(a, elt_value) {                                       \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_a_push(a, elt_value) {                                       \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strfit_push_many(s, a, elt_ptr, num_elts) {                         \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_strfit_push(prh_impl_strfit_data((s), a), prh_impl_n); \
    if (prh_impl_p) memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                  \
}
#define prh_string_push_many(s, a, elt_ptr, num_elts) {                         \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_strlax_push_many(s, a, elt_ptr, num_elts) {                         \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_strfit_v_push_many(a, elt_ptr, num_elts) {                          \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_strfit_push(prh_impl_strfit_dvar_data(a), prh_impl_n); \
    if (prh_impl_p) memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                  \
}
#define prh_string_a_push_many(a, elt_ptr, num_elts) {                          \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_strlax_a_push_many(a, elt_ptr, num_elts) {                          \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_strdyn_push(prh_impl_strlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}

#define prh_strfit_unchecked_push(s, a, elt_value) {                            \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strfit_data((s), a), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_string_unchecked_push(s, a, elt_value) {                            \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strdyn_data((s), a), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_unchecked_push(s, a, elt_value) {                            \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strlax_data((s), a), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strfit_v_unchecked_push(a, elt_value) {                             \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strfit_dvar(a), 1);    \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_string_v_unchecked_push(a, elt_value) {                             \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strdyn_dvar(a), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_v_unchecked_push(a, elt_value) {                             \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strlax_dvar(a), 1); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strfit_unchecked_push_many(s, a, elt_ptr, num_elts) {               \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strfit_data((s), a), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_string_unchecked_push_many(s, a, elt_ptr, num_elts) {               \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strdyn_addr((s), a), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_strlax_unchecked_push_many(s, a, elt_ptr, num_elts) {               \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strlax_addr((s), a), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_strfit_v_unchecked_push_many(a, elt_ptr, num_elts) {                \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strfit_dvar(a), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_string_v_unchecked_push_many(a, elt_ptr, num_elts) {                \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strdyn_dvar(a), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}
#define prh_strlax_v_unchecked_push_many(a, elt_ptr, num_elts) {                \
    prh_int prh_impl_n = (prh_int)(num_elts);                                   \
    prh_byte *prh_impl_p = prh_impl_string_unchecked_push(prh_impl_strlax_dvar(a), prh_impl_n); \
    memcpy(prh_impl_p, (elt_ptr), prh_impl_n);                                  \
}

void prh_impl_strdyn_erase(prh_byte *elem_ptr, prh_int start, prh_int i);

#define prh_strfit_erase(s, a, i) prh_impl_strdyn_erase(prh_impl_strfit_data((s), a), 0, (i))
#define prh_string_erase(s, a, i) prh_impl_strdyn_erase(prh_impl_strdyn_data((s), a), 0, (i))
#define prh_strlax_erase(s, a, i) {                                             \
    prh_byte *elem_ptr = prh_impl_strlax_data((s), a);                          \
    prh_impl_strdyn_erase(elem_ptr, *prh_impl_arr_start(elem_ptr), (i));        \
}
#define prh_strfit_v_erase(a, i) prh_impl_strdyn_erase(prh_impl_strfit_dvar(a), 0, (i))
#define prh_string_v_erase(a, i) prh_impl_strdyn_erase(prh_impl_strdyn_dvar(a), 0, (i))
#define prh_strlax_v_erase(a, i) {                                              \
    prh_byte *elem_ptr = prh_impl_strlax_dvar(a);                               \
    prh_impl_strdyn_erase(elem_ptr, *prh_impl_arr_start(elem_ptr), (i));        \
}

prh_byte *prh_impl_strfit_insert(prh_byte *elem_ptr, prh_int i);
prh_byte *prh_impl_strfit_unchecked_insert(prh_byte *elem_ptr, prh_int i);
prh_byte *prh_impl_strdyn_insert(prh_byte *elem_ptr, prh_int start, prh_int i);

#define prh_strfit_insert(s, a, i, elt_value) {                                 \
    prh_byte *prh_impl_p = prh_impl_strfit_insert(prh_impl_strfit_data((s), a), (i)); \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_string_insert(s, a, i, elt_value) {                                 \
    prh_impl_array_expand(prh_impl_strdyn_addr((s), a), sizeof(prh_impl_arrdyn_hdr), 1, 1); \
    prh_byte *prh_impl_p = prh_impl_strdyn_insert(prh_impl_strdyn_data((s), a), 0, (i)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_insert(s, a, i, elt_value) {                                 \
    prh_impl_array_expand(prh_impl_strlax_addr((s), a), sizeof(prh_impl_arrlax_hdr), 1, 1); \
    prh_byte *prh_impl_p = prh_impl_strlax_data((s), a);                        \
    prh_byte *prh_impl_elt = prh_impl_strdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i)); \
    *prh_impl_elt = (elt_value);                                                \
}
#define prh_strfit_v_insert(a, i, elt_value) {                                  \
    prh_byte *prh_impl_p = prh_impl_strfit_insert(prh_impl_strfit_dvar(a), (i)); \
    if (prh_impl_p) *prh_impl_p = (elt_value);                                  \
}
#define prh_string_a_insert(a, i, elt_value) {                                  \
    prh_impl_array_expand(prh_impl_strdyn_avar_addr(a), sizeof(prh_impl_arrdyn_hdr), 1, 1); \
    prh_byte *prh_impl_p = prh_impl_strdyn_insert(prh_impl_strdyn_avar(a), 0, (i)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_a_insert(a, i, elt_value) {                                  \
    prh_impl_array_expand(prh_impl_strlax_avar_addr(a), sizeof(prh_impl_arrlax_hdr), 1, 1); \
    prh_byte *prh_impl_p = prh_impl_strlax_avar(a);                             \
    prh_byte *prh_impl_elt = prh_impl_strdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i)); \
    *prh_impl_elt = (elt_value);                                                \
}

#define prh_strfit_unchecked_insert(s, a, i, elt_value) {                       \
    prh_byte *prh_impl_p = prh_impl_strfit_unchecked_insert(prh_impl_strfit_data((s), a), (i)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_string_unchecked_insert(s, a, i, elt_value) {                       \
    prh_byte *prh_impl_p = prh_impl_strdyn_insert(prh_impl_strdyn_data((s), a), 0, (i)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_unchecked_insert(s, a, i, elt_value) {                       \
    prh_byte *prh_impl_p = prh_impl_strlax_data((s), a);                        \
    prh_byte *prh_impl_elt = prh_impl_strdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i)); \
    *prh_impl_elt = (elt_value);                                                \
}
#define prh_strfit_v_unchecked_insert(a, i, elt_value) {                        \
    prh_byte *prh_impl_p = prh_impl_strfit_unchecked_insert(prh_impl_strfit_dvar(a), (i)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_string_v_unchecked_insert(a, i, elt_value) {                        \
    prh_byte *prh_impl_p = prh_impl_strdyn_insert(prh_impl_strdyn_avar(a), 0, (i)); \
    *prh_impl_p = (elt_value);                                                  \
}
#define prh_strlax_v_unchecked_insert(a, i, elt_value) {                        \
    prh_byte *prh_impl_p = prh_impl_strlax_avar(a);                             \
    prh_byte *prh_impl_elt = prh_impl_strdyn_insert(prh_impl_p, *prh_impl_arr_start(prh_impl_p), (i)); \
    *prh_impl_elt = (elt_value);                                                \
}

#define prh_strfix_len(s, a) *prh_impl_arr_len(prh_impl_strfix_data((s), a))
#define prh_strfix_data(s, a) prh_impl_strfix_data((s), a)
#define prh_strfix_end(s, a) prh_impl_strfix_data((s), a) + *prh_impl_arr_len(prh_impl_strfix_data((s), a))
#define prh_strfix_get(s, a, i) prh_impl_strfix_data((s), a) + (i)
#define prh_strfix_v_len(a) *prh_impl_arr_len(prh_impl_strfix_dvar(a))
#define prh_strfix_v_data(a) prh_impl_strfix_dvar(a)
#define prh_strfix_v_end(a) prh_impl_strfix_dvar(a) + *prh_impl_arr_len(prh_impl_strfix_dvar(a))
#define prh_strfix_v_get(a, i) prh_impl_strfix_dvar(a) + (i)

#define prh_strfit_cap(s, a) *prh_impl_arr_cap(prh_impl_strfit_data((s), a))
#define prh_strfit_len(s, a) *prh_impl_arr_len(prh_impl_strfit_data((s), a))
#define prh_strfit_data(s, a) prh_impl_strfit_data((s), a)
#define prh_strfit_end(s, a) prh_impl_strfit_data((s), a) + *prh_impl_arr_len(prh_impl_strfit_data((s), a))
#define prh_strfit_get(s, a, i) prh_impl_strfit_data((s), a) + (i)
#define prh_strfit_v_cap(a) *prh_impl_arr_cap(prh_impl_strfit_dvar(a))
#define prh_strfit_v_len(a) *prh_impl_arr_len(prh_impl_strfit_dvar(a))
#define prh_strfit_v_data(a) prh_impl_strfit_dvar(a)
#define prh_strfit_v_end(a) prh_impl_strfit_dvar(a) + *prh_impl_arr_len(prh_impl_strfit_dvar(a))
#define prh_strfit_v_get(a, i) prh_impl_strfit_dvar(a) + (i)

#define prh_string_cap(s, a) *prh_impl_arr_cap(prh_impl_strdyn_data((s), a))
#define prh_string_len(s, a) *prh_impl_arr_len(prh_impl_strdyn_data((s), a))
#define prh_string_data(s, a) prh_impl_strdyn_data((s), a)
#define prh_string_end(s, a) prh_impl_strdyn_data((s), a) + *prh_impl_arr_len(prh_impl_strdyn_data((s), a))
#define prh_string_get(s, a, i) prh_impl_strdyn_data((s), a) + (i)
#define prh_string_v_cap(a) *prh_impl_arr_cap(prh_impl_strdyn_dvar(a))
#define prh_string_v_len(a) *prh_impl_arr_len(prh_impl_strdyn_dvar(a))
#define prh_string_v_data(a) prh_impl_strdyn_dvar(a)
#define prh_string_v_end(a) prh_impl_strfdyn_dvar(a) + *prh_impl_arr_len(prh_impl_strdyn_dvar(a))
#define prh_string_v_get(a, i) prh_impl_strdyn_dvar(a) + (i)

#define prh_strlax_cap(s, a) *prh_impl_arr_cap(prh_impl_strlax_data((s), a))
#define prh_strlax_len(s, a) *prh_impl_arr_len(prh_impl_strlax_data((s), a))
#define prh_strlax_data(s, a) prh_impl_strlax_data((s), a) + *prh_impl_arr_start(prh_impl_strlax_data((s), a))
#define prh_strlax_end(s, a) prh_strlax_data((s), a) + *prh_impl_arr_len(prh_impl_strlax_data((s), a))
#define prh_strlax_get(s, a, i) prh_strlax_data((s), a) + (i)
#define prh_strlax_v_cap(a) *prh_impl_arr_cap(prh_impl_strlax_dvar(a))
#define prh_strlax_v_len(a) *prh_impl_arr_len(prh_impl_strlax_dvar(a))
#define prh_strlax_v_data(a) prh_impl_strlax_dvar(a) + *prh_impl_arr_start(prh_impl_strlax_dvar(a))
#define prh_strlax_v_end(a) prh_strlax_v_data(a) + *prh_impl_arr_len(prh_impl_strlax_dvar(a))
#define prh_strlax_v_get(a, i) prh_strlax_v_data(a) + (i)

#ifdef PRH_STRING_STRIP_PREFIX
#endif // PRH_STRING_STRIP_PREFIX

#ifdef PRH_STRING_IMPLEMENTATION
prh_byte *prh_impl_strfit_push(prh_byte *elem_ptr, prh_int num_elts) {
    prh_impl_arrfit_hdr *p = (prh_impl_arrfit_hdr)elem_ptr - 1;
    prh_assert(elem_ptr != prh_null);
    prh_assert(num_elts >= 0); // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    prh_int count = p->count;
    if (count + num_elts > p->capacity) {
        return prh_null;
    }
    p->count += num_elts;
    return elem_ptr + count;
}

prh_byte *prh_impl_string_unchecked_push(prh_byte *elem_ptr, prh_int num_elts) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(num_elts >= 0); // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    prh_int count = *count_ptr;
    *count_ptr += num_elts;
    return elem_ptr + count;
}

prh_byte *prh_impl_strfit_try_push(prh_byte *elem_ptr, prh_int *num_elts_ptr) { // push many as possible
    prh_assert(elem_ptr != prh_null);
    prh_assert(*num_elts_ptr >= 0);
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_int capacity = *prh_impl_arr_cap(elem_ptr);
    prh_int count = *count_ptr, num_elts = *num_elts_ptr;
    if (count + num_elts > capacity) {
        num_elts = capacity - count;
        *num_elts_ptr = num_elts;
    }
    *count_ptr += num_elts; // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    return elem_ptr + count;
}

prh_byte *prh_impl_strdyn_push(prh_impl_array *array, prh_int hdr_size, prh_int num_elts) {
    prh_impl_array_expand(array, hdr_size, num_elts, 1);
    prh_byte *elem_ptr = s->data;
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_int count = *count_ptr;
    *count_ptr += num_elts; // num_elts 可以为零，虽然返回了有效指针但外面的拷贝会不生效
    return elem_ptr + count;
}

void prh_impl_strdyn_erase(prh_byte *elem_ptr, prh_int start, prh_int i) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i < *count_ptr); // 数组为空不能移除元素
    prh_int new_count = --(*count_ptr);
    prh_byte *dest = elem_ptr + start + i;
    memmove(dest, dest + 1, new_count - i); // 长度参数可为零
}

prh_byte *prh_impl_strfit_insert(prh_byte *elem_ptr, prh_int i) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i <= *count_ptr);
    prh_int count = *count_ptr;
    if (count >= *prh_impl_arr_cap(elem_ptr)) {
        return prh_null;
    }
    *count_ptr += 1;
    prh_byte *source = elem_ptr + i;
    memmove(source + 1, source, count - i); // 长度参数可为零
    return source;
}

prh_byte *prh_impl_strfit_unchecked_insert(prh_byte *elem_ptr, prh_int i) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i <= *count_ptr);
    prh_int count = (*count_ptr)++;
    prh_byte *source = elem_ptr + i;
    memmove(source + 1, source, count - i); // 长度参数可为零
    return source;
}

prh_byte *prh_impl_strdyn_insert(prh_byte *elem_ptr, prh_int start, prh_int i) {
    prh_int *count_ptr = prh_impl_arr_len(elem_ptr);
    prh_assert(elem_ptr != prh_null);
    prh_assert(i >= 0 && i <= *count_ptr);
    prh_int count = (*count_ptr)++;
    prh_byte *source = elem_ptr + start + i;
    memmove(source + 1, source, count - i); // 长度参数可为零
    return source;
}
#endif // PRH_STRING_IMPLEMENTATION
#endif // PRH_STRING_INCLUDE

#ifdef PRH_LIST_INCLUDE
typedef struct prh_snode { // node single linked
    struct prh_snode *next;
} prh_snode;

typedef struct prh_data_snode {
    struct prh_data_snode *next;
    void *data; // node only contain a data pointer
} prh_data_snode;

typedef struct {
    prh_data_snode *next;
} prh_data_snode_head;

typedef struct { // zero initialized
    prh_snode *head;
    prh_snode *tail;
} prh_slist;

typedef struct prh_xnode { // node xored double linked (both = prev XOR next)
    struct prh_xnode *both;
} prh_xnode;

typedef struct prh_data_xnode {
    struct prh_data_xnode *both;
    void *data; // node only contain a data pointer
} prh_data_xnode;

typedef struct { // zero initialized
    prh_xnode *head;
    prh_xnode *tail;
} prh_xlist;

typedef struct prh_node { // node double linked
    struct prh_node *next;
    struct prh_node *prev;
} prh_node;

typedef struct prh_data_node {
    struct prh_data_node *next;
    struct prh_data_node *prev;
    void *data; // node only contain a data pointer
} prh_data_node;

typedef struct { // zero initialized
    prh_node *head;
    prh_node *tail;
} prh_list;

typedef struct prh_tnode { // node tripple linked
    struct prh_tnode *next;
    struct prh_tnode *prev;
    struct prh_tnode *parent;
} prh_tnode;

typedef struct prh_data_tnode {
    struct prh_data_tnode *next;
    struct prh_data_tnode *prev;
    struct prh_data_tnode *parent;
    void *data; // node only contain a data pointer
} prh_data_tnode;

typedef struct { // zero initialized
    prh_tnode *root;
} prh_tree;

void prh_impl_list_free_each_node(prh_snode *head, void (*node_free)(void *));
void prh_impl_list_free_each_node_and_clear(prh_slist *list, void (*node_free)(void *));

#ifdef PRH_LIST_IMPLEMENTATION
void prh_impl_list_free_each_node(prh_snode *head, void (*node_free)(void *)) {
    while (head) {
        prh_snode *curr = head;
        head = head->next; // get next before free
        node_free(curr);
    }
}

void prh_impl_list_free_each_node_and_clear(prh_slist *list, void (*node_free)(void *)) {
    if (node_free) prh_impl_list_free_each_node(list->head, node_free);
    list->head = prh_null;
}
#endif // PRH_LIST_IMPLEMENTATION
#endif // PRH_LIST_INCLUDE

#ifdef PRH_STACK_INCLUDE
// Just link the node into the stack, dont allocate any memory. Each node can
// have different size, but must cotain prh_snode as the header.
//  struct stack_custom_node {
//      prh_snode head; // must be 1st field
//      ... other custom node data ...
//  };
typedef struct { // zero initialize
    prh_snode *head;
} prh_nstack;

prh_inline void prh_nstack_init(prh_nstack *s) {
    s->head = prh_null;
}

prh_inline void prh_nstack_clear(prh_nstack *s, void (*node_free_func)(void *)) {
    prh_impl_list_free_each_node_and_clear((prh_slist *)s, node_free_func);
}

prh_inline prh_snode *prh_nstack_top(prh_nstack *s) {
    return s->head;
}

prh_inline bool prh_nstack_empty(prh_nstack *s) {
    return s->head == prh_null;
}

void prh_nstack_push(prh_nstack *s, prh_snode *node);
prh_snode *prh_nstack_pop(prh_nstack *s);

#ifdef PRH_STACK_STRIP_PREFIX
#define nstack_t         prh_nstack
#define nstack_init      prh_nstack_init
#define nstack_clear     prh_nstack_clear
#define nstack_empty     prh_nstack_empty
#define nstack_push      prh_nstack_push
#define nstack_pop       prh_nstack_pop
#define nstack_top       prh_nstack_top
#endif

#ifdef PRH_STACK_IMPLEMENTATION
void prh_nstack_push(prh_nstack *s, prh_snode *node) {
    node->next = s->head;
    s->head = node;
}

prh_snode *prh_nstack_pop(prh_nstack *s) {
    prh_snode *head = s->head;
    if (head == prh_null) return prh_null;
    s->head = head->next;
    return head;
}
#endif // PRH_STACK_IMPLEMENTATION
#endif // PRH_STACK_INCLUDE

#ifdef PRH_QUEUE_INCLUDE
// Array queue with power of 2 capacity size.
//
//  struct StructContainArrayQueue {
//      Object *<qname>_arrque_type;
//  };
//
//  StructContainArrayQueue s;
//  prh_arrque_init(&s, qname, 64);
//  Object *item = prh_arrque_push(&s, qname);
//  .. initialize the new item ..
//  item = prh_arrque_pop(&s, qname);
//  .. using the poped item, may be null ..
//  prh_arrque_free(&s, qname);
typedef struct {
    prh_int capacity;
    prh_int head;
    prh_int tail;
} prh_impl_arrque;

#define prh_impl_arrque(s, q) ((s)->prh_macro_concat_name(q, _arrque_type))
#define prh_impl_arrque_nextpos(h, pos) (((pos) + 1) & ((h)->capacity - 1))
void *prh_impl_arrque_init(prh_int capacity, prh_int object_size);
void prh_impl_arrque_free(void *queue);
void *prh_impl_arrque_pop(void *queue, prh_int object_size);
void *prh_impl_arrque_push(void *queue, prh_int object_size);
prh_int prh_impl_arrque_len(void *queue);

prh_inline bool prh_impl_arrque_empty(prh_impl_arrque *h) {
    return h->head == h->tail;
}

prh_inline bool prh_impl_arrque_full(prh_impl_arrque *h) {
    return prh_impl_arrque_nextpos(h, h->tail) == h->head;
}

prh_inline prh_int prh_impl_arrque_cap(void *queue) {
    prh_impl_arrque *h = ((prh_impl_arrque *)queue) - 1;
    return h->capacity;
}

#define prh_arrque_init(s, q, size) {                                           \
    prh_impl_arrque((s), q) = prh_impl_arrque_init((size), sizeof(*prh_impl_arrque((s), q))); \
}

#define prh_arrque_free(s, q) {                                                 \
    prh_impl_arrque_free(prh_impl_arrque((s), q));                              \
    prh_impl_arrque((s), q) = prh_null;                                         \
}

#define prh_arrque_push(s, q) (prh_typeof(prh_impl_arrque((s), q)))prh_impl_arrque_push(prh_impl_arrque((s), q), sizeof(*prh_impl_arrque((s), q)))
#define prh_arrque_pop(s, q) (prh_typeof(prh_impl_arrque((s), q)))prh_impl_arrque_pop(prh_impl_arrque((s), q), sizeof(*prh_impl_arrque((s), q)))

#define prh_arrque_cap(s, q) prh_impl_arrque_cap(prh_impl_arrque((s), q))
#define prh_arrque_len(s, q) prh_impl_arrque_len(prh_impl_arrque((s), q))

#ifdef PRH_QUEUE_STRIP_PREFIX
#define arrque_init    prh_arrque_init
#define arrque_free    prh_arrque_free
#define arrque_push    prh_arrque_push
#define arrque_pop     prh_arrque_pop
#define arrque_cap     prh_arrque_cap
#define arrque_len     prh_arrque_len
#endif

#ifdef PRH_QUEUE_IMPLEMENTATION
void *prh_impl_arrque_init(prh_int capacity, prh_int object_size) {
    if (capacity == 0) return prh_null;
    assert(prh_is_power_of_2(capacity));
    prh_int alloc_size = sizeof(prh_impl_arrque) + prh_round_ptrsize(object_size * capacity);
    prh_impl_arrque *p = prh_malloc(alloc_size);
    p->capacity = capacity;
    p->head = p->tail = 0;
    return p + 1;
}

void prh_impl_arrque_free(void *queue) {
    if (queue == prh_null) return;
    prh_free(((prh_impl_arrque *)queue) - 1);
}

prh_int prh_impl_arrque_len(void *queue) {
    prh_impl_arrque *h = ((prh_impl_arrque *)queue) - 1;
    prh_int len = h->tail - h->head;
    if (len < 0) return len + h->capacity;
    return len;
}

void *prh_impl_arrque_pop(void *queue, prh_int object_size) {
    prh_impl_arrque *h = ((prh_impl_arrque *)queue) - 1;
    if (prh_impl_arrque_empty(h)) return prh_null;
    void *top = (char *)queue + object_size * h->head;
    h->head = prh_impl_arrque_nextpos(h, h->head);
    return top;
}

void *prh_impl_arrque_push(void *queue, prh_int object_size) {
    prh_impl_arrque *h = ((prh_impl_arrque *)queue) - 1;
    if (prh_impl_arrque_full(h)) return prh_null;
    void *tail = (char *)queue + object_size * h->tail;
    h->tail = prh_impl_arrque_nextpos(h, h->tail);
    return tail;
}
#endif // PRH_QUEUE_IMPLEMENTATION

// Just link the node into the queue, dont allocate any memory. Each node can
// have different size, but must cotain prh_snode as the header.
typedef struct { // zero initialize
    prh_snode *head;
    prh_snode *tail;
} prh_quefit;

typedef struct { // zero initialize
    prh_data_snode *head;
    prh_data_snode *tail;
} prh_data_quefit;

prh_inline void prh_quefit_init(prh_quefit *q) {
    q->head = q->tail = prh_null;
}

prh_inline bool prh_quefit_empty(prh_quefit *q) {
    return q->head == prh_null;
}

prh_inline prh_snode *prh_quefit_top(prh_quefit *q) {
    return q->head;
}

void prh_quefit_push(prh_quefit *q, prh_snode *new_node);
prh_snode *prh_quefit_pop(prh_quefit *q);
prh_quefit prh_quefit_move(prh_quefit *q);

#define prh_quefit_for_begin(quefit) {                                          \
        prh_snode *it = (quefit)->head;                                         \
        prh_snode *prh_impl_next;                                               \
        for (; it; it = prh_impl_next) {                                        \
            prh_impl_next = it->next;

#define prh_quefit_for_end()                                                    \
        }                                                                       \
    }

#define prh_data_quefit_for_begin(data_quefit) {                                \
        prh_data_snode *it = (data_quefit)->head;                               \
        prh_data_snode *prh_impl_next;                                          \
        for (; it; it = prh_impl_next) {                                        \
            prh_impl_next = it->next;

#define prh_data_quefit_for_end()                                               \
        }                                                                       \
    }

prh_inline void prh_data_quefit_init(prh_data_quefit *q) {
    prh_quefit_init((prh_quefit *)q);
}

prh_inline bool prh_data_quefit_empty(prh_data_quefit *q) {
    return prh_quefit_empty((prh_quefit *)q);
}

prh_data_snode *prh_data_quefit_top(prh_data_quefit *q) {
    return (prh_data_snode *)prh_quefit_top((prh_quefit *)q);
}

prh_data_snode *prh_data_quefit_pop(prh_data_quefit *q) {
    return (prh_data_snode *)prh_quefit_pop((prh_quefit *)q);
}

prh_inline void prh_data_quefit_push(prh_data_quefit *q, prh_data_snode *node) {
    prh_quefit_push((prh_quefit *)q, (prh_snode *)node);
}

#ifdef PRH_QUEUE_IMPLEMENTATION
prh_quefit prh_quefit_move(prh_quefit *q) {
    prh_quefit que = *q;
    prh_quefit_init(q);
    return que;
}

void prh_quefit_push(prh_quefit *q, prh_snode *new_node) {
    prh_snode *tail = q->tail;
    if (tail == prh_null) tail = (prh_snode *)q;
    q->tail = tail->next = new_node;
    new_node->next = prh_null;
}

void prh_quefit_push_front(prh_quefit *q, prh_snode *new_node) {
    prh_snode *head = q->head;
    new_node->next = head;
    q->head = new_node;
    if (q->tail == prh_null) q->tail = new_node;
}

prh_snode *prh_quefit_pop(prh_quefit *q) {
    prh_snode *top = q->head;
    if (top == prh_null) return prh_null;
    q->head = top->next;
    if (top == q->tail) {
        q->tail = prh_null;
    }
    return top;
}
#endif // PRH_QUEUE_IMPLEMENTATION

// 队列不负责分配实际的节点，整个节点都由使用者提供，节点可以是任意长度，其内容由使用者
// 决定没有限制。队列仅将用户提供的节点链接到队列中，节点的类型由用户自定义，只要其中包
// 含一个自身类型的 next 指针即可，这个 next 指针的名字可以自定义，且这个 next 指针可
// 以定义在任何位置，不需要像 prh_quefit 和 prh_data_quefit 一样，next 指针必须是结
// 构体头部的第一个成员。
//
// 队列也由用户自定义，但必须按顺序包含 head 和 tail 两个成员。用户自定义节点类型、队列
// 类型示例如下：
//
// struct user_node {
//      int a, b, c;
//      struct user_node *chain;
//      void *d, *e;
// };
//
// struct user_relaxed_quefit { // 零初始化
//      struct user_node *head; // 第一个成员
//      struct user_node *tail; // 第二个成员
// };
//
// struct user_relaxed_quefit q = {0};
// prh_relaxed_quefit_push(&q, user_node, chain);

void prh_impl_relaxed_quefit_push(void **quefit, void *node, prh_i32 next_offset);
void prh_impl_relaxed_quefit_push_front(void **quefit, void *node, prh_i32 next_offset);
void prh_impl_relaxed_quefit_push_queue(void **quefit, void **from_que, prh_i32 next_offset); // from_que is not cleared
void prh_impl_relaxed_quefit_push_queue_front(void **quefit, void **from_que, prh_i32 next_offset); // form_que is not cleared
void *prh_impl_relaxed_quefit_pop(void **quefit, prh_i32 next_offset);

#define prh_relaxed_quefit_init(relaxed_quefit_ptr) { /* zero initialization */ \
    prh_typeof(relaxed_quefit_ptr) prh_impl_que_ptr = (relaxed_quefit_ptr);     \
    relaxed_quefit_ptr->head = relaxed_quefit_ptr->tail = prh_null;             \
}

#define prh_relaxed_quefit_push(relaxed_quefit_ptr, node_type_ptr, next_field_name) { \
    prh_typeof(relaxed_quefit_ptr) prh_impl_que_ptr = (relaxed_quefit_ptr);     \
    typedef prh_typeof(*(prh_impl_que_ptr->tail)) prh_impl_node_type;           \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    prh_impl_node_type *prh_impl_node_ptr = (node_type_ptr);                    \
    prh_impl_relaxed_quefit_push((void **)prh_impl_que_ptr, prh_impl_node_ptr, (prh_i32)prh_offsetof(prh_impl_node_type, next_field_name)); \
}

#define prh_relaxed_quefit_push_queue(relaxed_quefit_ptr, from_quefit_ptr, next_field_name) { \
    typedef prh_typeof(relaxed_quefit_ptr) prh_impl_quefit_ptr_type;            \
    prh_impl_quefit_ptr_type prh_impl_que_ptr = (relaxed_quefit_ptr);           \
    prh_impl_quefit_ptr_type prh_impl_from_que = (from_quefit_ptr);             \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    typedef prh_typeof(*(prh_impl_que_ptr->tail)) prh_impl_node_type;           \
    prh_impl_relaxed_quefit_push_queue((void **)prh_impl_que_ptr, (void **)prh_impl_from_que, (prh_i32)prh_offsetof(prh_impl_node_type, next_field_name)); \
}

#define prh_relaxed_quefit_push_front(relaxed_quefit_ptr, node_type_ptr, next_field_name) { \
    prh_typeof(relaxed_quefit_ptr) prh_impl_que_ptr = (relaxed_quefit_ptr);     \
    typedef prh_typeof(*(prh_impl_que_ptr->head)) prh_impl_node_type;           \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    prh_impl_node_type *prh_impl_node_ptr = (node_type_ptr);                    \
    prh_impl_relaxed_quefit_push_front((void **)prh_impl_que_ptr, prh_impl_node_ptr, (prh_i32)prh_offsetof(prh_impl_node_type, next_field_name)); \
}

#define prh_relaxed_quefit_push_queue_front(relaxed_quefit_ptr, from_quefit_ptr, next_field_name) { \
    typedef prh_typeof(relaxed_quefit_ptr) prh_impl_quefit_ptr_type;            \
    prh_impl_quefit_ptr_type prh_impl_que_ptr = (relaxed_quefit_ptr);           \
    prh_impl_quefit_ptr_type prh_impl_from_que = (from_quefit_ptr);             \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    typedef prh_typeof(*(prh_impl_que_ptr->head)) prh_impl_node_type;           \
    prh_impl_relaxed_quefit_push_queue_front((void **)prh_impl_que_ptr, (void **)prh_impl_from_que, (prh_i32)prh_offsetof(prh_impl_node_type, next_field_name)); \
}

#define prh_relaxed_quefit_empty(relaxed_quefit_ptr) ((relaxed_quefit_ptr)->head == prh_null)
#define prh_relaxed_quefit_not_empty(relaxed_quefit_ptr) ((relaxed_quefit_ptr)->head != prh_null)

#define prh_relaxed_quefit_top(relaxed_quefit_ptr, node_out) {                  \
    node_out = (relaxed_quefit_ptr)->head;                                      \
}

#define prh_relaxed_quefit_pop(relaxed_quefit_ptr, node_out, next_field_name) { \
    prh_typeof(relaxed_quefit_ptr) prh_impl_que_ptr = (relaxed_quefit_ptr);     \
    typedef prh_typeof(*(prh_impl_que_ptr->head)) prh_impl_node_type;           \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    node_out = (prh_impl_node_type *)prh_impl_relaxed_quefit_pop((void **)prh_impl_que_ptr, (prh_i32)prh_offsetof(prh_impl_node_type, next_field_name)); \
}

#if defined(PRH_QUEUE_IMPLEMENTATION)
void prh_impl_relaxed_quefit_push(void **quefit, void *node, prh_i32 next_offset) {
    prh_byte *tail = quefit[1]; // 由于next可能不是第一个成员，因此不能定义成循环列表，因为无法定义一个固定的包含next的头节点
    if (tail == prh_null) {
        quefit[0] = node;
    } else {
        *(void **)(tail + next_offset) = node;
    }
    *(void **)((prh_byte *)node + next_offset) = prh_null;
    quefit[1] = node;
}

void prh_impl_relaxed_quefit_push_queue(void **quefit, void **from_que, prh_i32 next_offset) {
    prh_byte *from_tail = from_que[1];
    if (from_tail == prh_null) return;
    prh_byte *tail = quefit[1];
    if (tail == prh_null) {
        quefit[0] = from_que[0];
    } else {
        *(void **)(tail + next_offset) = from_que[0];
    }
    quefit[1] = from_tail;
}

void prh_impl_relaxed_quefit_push_front(void **quefit, void *node, prh_i32 next_offset) {
    prh_byte *head = quefit[0];
    quefit[0] = node;
    *(void **)((prh_byte *)node + next_offset) = head;
    if (head == prh_null) {
        quefit[1] = node;
    }
}

void prh_impl_relaxed_quefit_push_queue_front(void **quefit, void **from_que, prh_i32 next_offset) {
    prh_byte *from_head = from_que[0];
    if (from_head == prh_null) return;
    prh_byte *head = quefit[0];
    quefit[0] = from_head;
    prh_byte *from_tail = from_que[1];
    *(void **)((prh_byte *)from_tail + next_offset) = head;
    if (head == prh_null) {
        quefit[1] = from_tail;
    }
}

void *prh_impl_relaxed_quefit_pop(void **quefit, prh_i32 next_offset) {
    prh_byte *head = quefit[0];
    if (head) {
        quefit[0] = *(void **)(head + next_offset);
        if (head == quefit[1]) {
            quefit[1] = prh_null;
        }
    }
    return head;
}
#endif /* PRH_QUEUE_IMPLEMENTATION */

// Dynamic allocated fixed size queue. The node can only contain a data pointer.
typedef struct { // zero initialize
    prh_data_snode *head;
    prh_data_snode *tail;
} prh_quefix;

// Dynamic allocated variable size queue. The node can contain any object and
// with any different size. If the size < sizeof(void *), the size is rounded
// up to sizeof(void *).
typedef struct { // zero initialize
    prh_snode *head;
    prh_snode *tail;
} prh_quedyn;

prh_inline void prh_quedyn_init(prh_quedyn *q) {
    prh_quefit_init((prh_quefit *)q);
}

prh_inline bool prh_quedyn_empty(prh_quedyn *q) {
    return prh_quefit_empty((prh_quefit *)q);
}

prh_inline void prh_quedyn_free_node(void *ptr_node_object) {
    if (ptr_node_object) prh_free((prh_snode *)ptr_node_object - 1);
}

void prh_quedyn_clear(prh_quedyn *q, void (*object_deinit_func)(void *));
void *prh_quedyn_push(prh_quedyn *q, int object_size); // return allocated zero initialized object address
void *prh_quedyn_top(prh_quedyn *q); // return null or object address inside the top node
void *prh_quedyn_pop(prh_quedyn *q); // pop top node but dont free it, return object address inside the node

#ifdef PRH_QUEUE_STRIP_PREFIX
#define quedyn_t         prh_quedyn
#define quedyn_init      prh_quedyn_init
#define quedyn_empty     prh_quedyn_empty
#define quedyn_free_node prh_quedyn_free_node
#define quedyn_clear     prh_quedyn_clear
#define quedyn_move      prh_quedyn_move
#define quedyn_push      prh_quedyn_push
#define quedyn_top       prh_quedyn_top
#define quedyn_pop       prh_quedyn_pop
#endif

#ifdef PRH_QUEUE_IMPLEMENTATION
void prh_quedyn_clear(prh_quedyn *q, void (*object_deinit_func)(void *)) {
    prh_snode *next = q->head;
    while (next) {
        prh_snode *curr = next;
        next = next->next; // get next before free the node
        if (object_deinit_func) {
            object_deinit_func(curr + 1);
        }
        prh_free(curr);
    }
    prh_quedyn_init(q);
}

prh_quedyn prh_quedyn_move(prh_quedyn *q) {
    prh_quedyn que = *q;
    prh_quedyn_init(q);
    return que;
}

void *prh_quedyn_push(prh_quedyn *q, int object_size) {
    if (object_size < sizeof(void *)) object_size = sizeof(void *);
    prh_snode *new_node = prh_calloc(sizeof(prh_snode) + prh_round_ptrsize(object_size));
    prh_quefit_push((prh_quefit *)q, new_node);
    return (new_node + 1);
}

void *prh_quedyn_top(prh_quedyn *q) {
    prh_snode *top = q->head;
    return (top == prh_null) ? prh_null : (top + 1);
}

void *prh_quedyn_pop(prh_quedyn *q) {
    prh_snode *top = prh_quefit_pop((prh_quefit *)q);
    return (top == prh_null) ? prh_null : (top + 1);
}
#endif // PRH_QUEUE_IMPLEMENTATION
#endif // PRH_QUEUE_INCLUDE

// https://gcc.gnu.org/wiki/SplitStacks
//
// 分段栈（Split Stacks），分段栈的目标是允许一个不连续的栈，该栈会根据需要自动增长。这
// 意味着你可以运行多个线程，每个线程都从一个较小的栈开始，并且栈会根据程序的需求自动增长
// 和收缩。因此，在编写多线程程序时，不再需要考虑栈的大小。典型的多线程程序的内存使用量可
// 以显著减少，因为每个线程不需要最坏情况下的栈大小。在 32 位地址空间中，可以运行数百万个
// 线程（无论是完整的 NPTL 线程还是协程）。目前，分段栈已在运行 GNU/Linux 的 32 位和
// 64 位 x86 目标上实现，适用于 gcc 4.6.0 及更高版本。要获得完整功能，你必须使用 gold
// 链接器，可以通过使用 binutils 2.21 或更高版本并带有 --enable-gold 选项来构建。
//
// 栈将有一个始终可用的保证区域（guaranteed zone），保证区域的大小将因目标平台而异。它
// 将包含足够的栈空间来实际分配更多的栈空间，每个函数都必须验证当前栈是否有足够的空间来
// 执行。基本验证是栈指针与当前栈底部加上保证区域大小之间的比较，这必须是函数中的第一个
// 操作，并且也是目标特定的。它必须快速执行，因为每个被调用的函数都会执行它。有两种情况，
// 对于需要小于保证区域大小的栈帧的函数，我们可以简单地比较栈指针和栈限制。对于需要更大
// 栈帧的函数，我们必须进行包括栈帧大小的比较。
//
// 可能的策略：（一）保留一个寄存器来保存栈底加保证区大小，这必须是一个由被调用者保护的寄
// 存器。每个需要小栈帧的函数都以两条指令序列开始：
//          cmp     %sp,%reg    // 大部分情况多两条指令
//          bg      1f
//          expand stack
//      1:
// （二）使用 TLS 变量。在共享库的一般情况下，这将需要调用 __tls_get_addr 函数。这意味
// 着该函数必须在不需要任何额外栈空间的情况下工作。除非整个系统都使用分段栈编译，否则这是
// 不可行的。它需要设置 LD_BIND_NOW，以便在程序启动时解析 __tls_get_addr 函数。即使这
// 样，除非我们能够确保变量的空间完全分配，否则这可能也不够。一般来说，我认为我们不能确保
// 这一点，因为 dlopen 可能会导致线程请求更多的空间用于 TLS 变量，而这些空间需要在第一次
// 调用 __tls_get_addr 时分配，因此我认为这种方法通常无法工作。
// （三）让栈始终在 N 位边界结束。例如，如果我们总是以 4K（0x1000）的倍数分配栈段，那么
// 将每个栈段对齐，使栈始终在 12 位边界结束。然后栈上剩余的空间是 SP & 0xfff，栈底地址
// 值最大 0x1000，栈顶地址值最小 0x0000，32位栈指针的合法范围 [0x0ffc, 0x0000]，栈指
// 指针指向当前合法的栈顶元素。0x0000 & 0xfff = 0x0000 表示已经没有空间，0x0ffc &
// 0x0fff = 0x0ffc 表示只使用了 4 字节空间，还有 0x0ffc 大小的空间可用。
//          mov     %sp,%junkreg    // 大部分情况多四条指令
//          and     $0xfff,%junkreg
//          cmp     $framesize plus slop,%junkreg
//          bg      1f
//          expand stack
//      1:
// （四）引入一个新的函数调用来处理栈指针和栈扩展的比较。这个函数调用将是线程库的一部分，
// 并且可以使用特殊知识——例如，快速定位当前帧的栈信息的能力。这个函数调用将使用一个特殊
// 的调用约定序列，带有一个参数，即栈帧的大小。然后每个函数都以调用这个函数开始。在某些
// 目标上，这个函数调用可以与PIC代码所需的函数调用结合，例如 __i686.get_pc_thunk.bx。
// （五）重用栈保护器（stack protector）支持字段。当使用 glibc 时，每个线程描述符都有
// 一个字段用于栈保护器。我们可以重用该字段来保存栈限制。当然，然后就不可能将分段栈与栈
// 保护器一起使用。
// （六）至少在 x86 上，安排在 TCB 头中分配一个新字段，可以通过 %fs 或 %gs 访问。这可
// 能是最好的解决方案，也是为 i386 和 x86_64 实现的解决方案。
//
// 扩展栈：扩展栈需要分配额外的内存。这些额外的内存必须仅使用仅由的栈空间余量（stack
// space slop）来分配。所有用于分配额外栈空间的函数都必须编译为不使用分段栈。因此引入一
// 个新的函数属性 no_split_stack，表示栈不能被分段。也可以确保栈足够大，以至于在分配函
// 数调用期间不需要分段栈。
// 扩展栈后，函数将把任何基于栈的参数从旧栈复制到新栈。幸运的是，所有需要复制或移动构造
// 函数的 C++ 对象都是隐式地通过引用传递的，因此在栈上复制参数是可以的。对于可变参数函
// 数，这在一般情况下是不可能的，因此我们将以不同的方式编译可变参数函数：它们将使用一个
// 参数指针，参数指针不需要基于帧指针。对于返回栈上对象的函数，对象将在旧栈上返回。这通
// 常会自动发生，因为初始隐藏参数将自然地指向旧栈。
// 扩展栈时，函数的返回地址将被篡改，指向一个释放分配的栈块并重置栈指针到调用者的函数。
// 旧栈块的地址和旧栈指针将被保存在新栈块的某个位置。
//
// 向后兼容性：我们希望能够在预编译的库（这些库没有使用分段栈编译）的系统上使用分段栈程
// 序。这意味着我们需要确保在调用任何此类函数之前有足够的栈空间。
// 每个以分段栈模式编译的对象文件都将被注释以表明函数使用分段栈，但 GNU as 没有创建任意
// 注释的一般支持。因此，每个以分段栈模式编译的对象文件将有一个特殊名称的空节：
// .note.GNU-split-stack。如果以分段栈模式编译的对象文件包含一些带有 no_split_stack
// 属性的函数，那么该对象文件也将有一个 .note.GNU-no-split-stack 节。这将告诉链接器有
// 些函数可能没有预期的分段栈前序代码。
// 当链接器链接可执行文件或共享库时，它将寻找从分段栈代码到非分段栈代码的调用。这将包括
// 对非分段栈共享库的调用（因此，一个链接到分段栈共享库的程序如果在运行时动态链接器发现
// 了一个非分段栈共享库，可能会失败；可能希望使用一个新的段类型来检测这种情况）。
// 对于从分段栈代码到非分段栈代码的调用，链接器将更改分段栈（调用者）函数的初始指令。这
// 意味着链接器将需要对编译器发出的指令有特殊了解。这些更改的效果将是将所需的帧大小增加
// 一个足够大的数字，以合理地适用于非分段栈。这将是一个目标依赖的数字；默认值将是类似
// 64K 的东西。请注意，当分段栈函数返回时，这个大的栈将被释放。请注意，我忽略了在共享库
// 中的分段栈代码调用主可执行文件中的非分段栈代码的情况，这似乎是一个不太可能的问题。
// 函数指针是一个棘手的情况。一般来说，我们不知道函数指针是否指向分段栈代码。因此，所有
// 通过函数指针的调用都将被修改为调用（或跳转到）一个特殊的函数 __fnptr_morestack。这
// 将使用目标特定的函数调用约定序列，并且将被实现为好像它本身是一个函数调用指令。也就是
// 说，所有参数都将被设置好，然后代码将跳转到 __fnptr_morestack。__fnptr_morestack
// 函数接受两个参数：要调用的函数指针和推送到栈上的参数字节数。（这尚未实现。）
//
// 调试：调试器将需要了解分段栈模式。有两个主要问题：展开栈和查找可变参数函数的参数。幸
// 运的是，DWARF 足够复杂，可以表示分段栈函数使用的不寻常的返回序列。因此，唯一的主要问
// 题是 gdb 期望所有栈地址单调递减。需要某种标记来告诉 gdb 禁用这个合理性检查（sanity
// check）。对于可变参数函数，可变参数将通过一个参数指针而不是在栈上访问。gdb 可能需要
// 调整以理解这一点。
//
// 实现步骤：
// 添加一个新的目标平台函数 bool supports_split_stack() 以及 TARGET_SUPPORTS_SPLIT_STACK。
// 在 gcc 中添加 -fsplit-stack 选项。只有当 targetm.supports_split_stack() 返回 true 时，才允许使用此选项。
// 添加 no_split_stack 函数属性。如果使用 -fno-split-stack，则忽略此属性。
// 如果使用 -fsplit-stack，则在对象文件中生成分段栈注释。
// 编写 __generic_morestack 常规函数。它必须在不自己分段栈的情况下分配所需的内存量。这可能是操作系统依赖的。
// alloca 和动态数组必须改变以检查可用的栈空间。如果没有足够的可用空间，它们必须调用堆分配例程。空间必须在函数退出时释放。
// 对于每个目标平台：
//      定义小栈帧函数和大栈帧函数的函数入口序列。
//      定义 __morestack 常规函数的调用约定。
//      当使用 -fsplit-stack 且未使用 no_split_stack 属性时，在前序代码中实现此序列。
//      如果需要，以不同的方式实现可变参数，使用一个单独的参数指针。
//      定义 __fnptr_morestack 常规函数的调用约定。更改 call define_expands 以使用新的调用约定来调用 REG。
//      编写 __morestack 常规函数。它必须以目标依赖的方式获取其参数。它必须保存寄存器并切换到一个备用栈。然后它可以调用
//      __generic_morestack。它必须将调用函数的返回值更改为指向 __releasestack。函数入口序列必须允许可靠地完成此操作。
//      编写 __releasestack 常规函数。它必须从分配的栈中检索原始栈指针和原始返回地址，释放分配的栈，并将栈指针调整为指向旧栈。
//      编写 __morestack_nosplit 常规函数。当分段栈代码调用非分段栈代码时，将调用此函数。它类似于 __morestack，但使用目标依赖的大栈大小。
//      编写 __fnptr_morestack 常规函数。在某些情况下，直接检查函数指针处的代码以查看它是否检查分段栈。如果它这样做，代码可以直接
//      跳转到被调用的函数。否则，它必须像 __morestack_nosplit 一样分配栈，然后将参数字节复制到新栈上。
//      如果分段栈对象中的代码调用非分段栈对象中的代码，则更改代码的初始启动序列，始终调用 __morestack_nosplit。
//      在包含分段栈对象的链接中，如果以非调用方式引用非分段栈对象中的函数（如获取其地址），则改为获取链接器生成的存根函数（stub
//      function）的地址。存根函数将调用 __morestack_nosplit，如果需要，将参数复制到新栈上，然后跳转到真正的函数。复制参数将不得
//      不对参数数量做出假设，因此如果传递的参数过多，这将失败。我不确定如何最好地处理这个不太可能的情况。我们只应在一个调用来源于
//      分段栈函数时才分配新栈。
// 更改链接器以查找新的分段栈注释。
// 如果使用 ld -r 将带有分段栈注释的对象与没有分段栈注释的对象链接，发出错误。
// 调整 gdb 以理解新代码。
// 想出将此高效地整合到 glibc 的最佳方法。可能需要将 glibc 既编译成分段栈，也编译成非分段栈，并以某种方式安排分段栈代码在运行时获取
// 分段栈的 glibc。如果程序链接器和动态链接器都能达成一致，或者程序链接器能够以某种方式知道在运行时将使用分段栈的 glibc，那将最好。
// 理想情况下，glibc 动态链接器应该知道程序是否期望共享库的分段栈版本，并拒绝将其与该库的非分段栈版本链接。
#ifdef PRH_CORO_INCLUDE
typedef struct prh_impl_coro prh_coro;
#define prh_coro_proc prh_fastcall(void)
prh_fastcall_typedef(void, prh_coroproc_t)(prh_coro *);

typedef struct prh_impl_soro prh_soro;
#define prh_soro_proc prh_fastcall(void)
prh_fastcall_typedef(void, prh_soroproc_t)(prh_soro *);

typedef struct prh_impl_coro_main prh_impl_coro_main;
typedef struct prh_impl_coro_struct prh_coro_struct;

prh_fastcall(void) prh_impl_asm_coro_yield(prh_coro *coro, prh_coro *next);
prh_coro *prh_impl_next_resume_coro(void);
int prh_coro_self_id(prh_coro *coro);
int prh_soro_self_id(prh_soro *soro);

prh_inline void prh_coro_yield(prh_coro *coro) {
    prh_impl_asm_coro_yield(coro, prh_impl_next_resume_coro());
}

prh_inline void prh_soro_yield(prh_soro *soro) {
    extern prh_thread_local prh_impl_coro_main *PRH_IMPL_CORO_MAIN;
    prh_impl_asm_coro_yield((prh_coro *)soro, (prh_coro *)PRH_IMPL_CORO_MAIN);
}

prh_inline void *prh_coro_data(prh_coro *coro) {
    return *(void **)((prh_byte *)coro + 8);
}

prh_inline void *prh_soro_data(prh_soro *soro) {
    return *(void **)((prh_byte *)soro + 8);
}

prh_coro_struct *prh_coro_init(int start_id, int maxcoros);
void prh_coro_create(prh_coro_struct *s, prh_coroproc_t proc, int stack_size, void *userdata);
void *prh_coro_creatx(prh_coro_struct *s, prh_coroproc_t proc, int stack_size, int maxudsize);
bool prh_coro_cycle_start(prh_coro_struct *s);
bool prh_coro_start(prh_coro_struct *s, int index);
void prh_coro_reload(prh_coro_struct *s, int index, prh_coroproc_t proc); // udata is not reset
void prh_coro_finish(prh_coro_struct **s);

typedef struct {
    prh_i32 start_id; // 1st field dont move
    prh_coro *coro; // last field dont move, only used by soro_struct
} prh_soro_struct;

prh_inline int prh_soro_main_id(prh_soro_struct *s) {
    return s->start_id;
}

prh_inline int prh_coro_main_id(prh_coro_struct *s) {
    return prh_soro_main_id((prh_soro_struct *)s);
}

void prh_soro_init(prh_soro_struct *s, int start_id);
void prh_soro_create(prh_soro_struct *s, prh_soroproc_t proc, int stack_size, void *userdata);
void *prh_soro_creatx(prh_soro_struct *s, prh_soroproc_t proc, int stack_size, int maxudsize);
bool prh_soro_start(prh_soro_struct *s);
void prh_soro_reload(prh_soro_struct *s, prh_soroproc_t proc); // udata is not reset
void prh_soro_finish(prh_soro_struct *s);

#ifdef PRH_CORO_STRIP_PREFIX
#define coro_t                  prh_coro
#define soro_t                  prh_soro
#define coro_struct_t           prh_coro_struct
#define soro_struct_t           prh_soro_struct
#define coro_proc_func          prh_coro_proc
#define soro_proc_func          prh_soro_proc
#define coro_get_data           prh_coro_data
#define soro_get_data           prh_soro_data
#define coro_self_id            prh_coro_self_id
#define soro_self_id            prh_soro_self_id
#define coro_main_id            prh_coro_main_id
#define soro_main_id            prh_soro_main_id
#define coro_yield              prh_coro_yield
#define soro_yield              prh_soro_yield
#define coro_init               prh_coro_init
#define coro_create             prh_coro_create
#define coro_creatx             prh_coro_creatx
#define coro_cycle_start        prh_coro_cycle_start
#define coro_start              prh_coro_start
#define coro_reload             prh_coro_reload
#define coro_finish             prh_coro_finish
#define soro_init               prh_soro_init
#define soro_create             prh_soro_create
#define soro_creatx             prh_soro_creatx
#define soro_start              prh_soro_start
#define soro_reload             prh_soro_reload
#define soro_finish             prh_soro_finish
#endif // PRH_CORO_STRIP_PREFIX

#ifdef PRH_CORO_IMPLEMENTATION
#ifndef PRH_CORO_DEBUG
#define PRH_CORO_DEBUG PRH_DEBUG
#endif

#define prh_lower_guard_word 0x5a5a5a5a
#define prh_upper_guard_word 0xa5a5a5a5

struct prh_impl_coro_guard {
    prh_u32 lower_guard_word;
    prh_u32 upper_guard_word;
};

prh_fastcall(int) prh_impl_asm_stack_init_depth(void);
prh_fastcall(void) prh_impl_asm_coro_call(void);
prh_fastcall(void) prh_impl_asm_soro_call(void);

void prh_impl_coro_stack_segmentation_fault(prh_coro *coro) {
    printf("coro %p stack segmentation fault\n", (void *)coro);
    exit(__LINE__);
}

// coroutine stack layout:
//  lower memery address
//  [prh_impl_coro_guard    ]
//  [       ...             ]
//  [       ...             ]          ^
//  [       ...             ]          ^
//  [stack memory data      ] <-- rsp  | <-- stack top
//  [stack memory data      ]
//  [stack memory data      ] <-- stack bottom
//  [prh_coro: rspoffset    ] <-- coro
//  [prh_coro: ...          ]
//  [coroutine userdata area]
//  upper memory address
struct prh_impl_coro {
    prh_i32 rspoffset; // 1st field dont move
    prh_i32 loweraddr;
    void *userdata;
};

// prh_coro_struct layout:
//  [prh_coro_struct]
//  [prh_coro *]
//  [prh_coro *]
struct prh_impl_coro_struct {
    prh_i32 start_id; // 1st field dont move
    prh_i32 real_cnt;
    prh_i32 maxcoros;
    prh_i32 coro_cnt;
};

typedef struct prh_impl_coro_main {
    prh_i32 rspoffset; // 1st field dont move
    prh_i32 start_id; // 2nd field dont move
    struct prh_impl_coro_main *next; // 3rd field dont move
    prh_i32 curr_index; // coro index start from 1
    prh_u32 end_count: 31, yield_cycle: 1;
    prh_coro **coro_list;
} prh_impl_coro_main;

typedef struct {
    prh_i32 rspoffset; // 1st field dont move
    prh_i32 start_id; // 2nd field dont move
    prh_impl_coro_main *next; // 3rd field dont move
} prh_impl_soro_main;

prh_inline int prh_impl_coro_struct_alloc_size(int maxcoros) {
    assert(maxcoros >= 1);
    assert(sizeof(prh_coro_struct) % sizeof(void *) == 0);
    return (int)(sizeof(prh_coro_struct) + maxcoros * sizeof(void *));
}

prh_coro_struct *prh_coro_init(int start_id, int maxcoros) {
    int alloc_size = prh_impl_coro_struct_alloc_size(maxcoros);
    prh_coro_struct *s = (prh_coro_struct *)prh_calloc(alloc_size);
    s->start_id = start_id;
    s->maxcoros = maxcoros;
    return s;
}

prh_inline struct prh_impl_coro_guard *prh_impl_coro_guard(prh_coro *coro) {
    return (struct prh_impl_coro_guard *)((char *)coro - coro->loweraddr);
}

void prh_impl_coro_guard_verify(prh_coro *coro) {
    struct prh_impl_coro_guard *g = prh_impl_coro_guard(coro);
    if (g->lower_guard_word != prh_lower_guard_word || g->upper_guard_word != prh_upper_guard_word) {
        prh_impl_coro_stack_segmentation_fault(coro);
    }
}

#define prh_impl_coro_size 16
prh_static_assert(sizeof(prh_coro) <= prh_impl_coro_size);

prh_inline int prh_impl_coro_extend_size(int coro_extend_size) {
    return prh_impl_coro_size + (int)prh_round_16_byte(coro_extend_size);
}

prh_coro *prh_impl_coro_alloc(int stack_size, int coro_extend_size, int maxudsize) {
    assert(maxudsize >= 0);
    int coro_size = prh_impl_coro_extend_size(coro_extend_size);
    int data_size = (int)prh_round_16_byte(maxudsize);
    assert(stack_size > coro_size + data_size + (int)sizeof(struct prh_impl_coro_guard) + prh_impl_asm_stack_init_depth());

    int stackallocsize = (int)prh_round_16_byte(stack_size);
    char *p = (char *)prh_16_byte_aligned_malloc(stackallocsize);
    prh_coro *coro = (prh_coro *)(p + stackallocsize - coro_size - data_size);
    memset(coro, 0, coro_size + data_size);
    coro->loweraddr = (prh_i32)((char *)coro - p);
    coro->userdata = (char *)coro + coro_size;

    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    guard->lower_guard_word = prh_lower_guard_word;
    guard->upper_guard_word = prh_upper_guard_word;
    return coro;
}

prh_coro *prh_impl_coro_alloc_set(prh_coro **slot, int stack_size, int coro_extend_size, int maxudsize) {
    assert(*slot == prh_null);
    prh_coro *coro = prh_impl_coro_alloc(stack_size, coro_extend_size, maxudsize);
    *slot = coro;
    return coro;
}

void prh_impl_coro_load_stack(prh_coro *coro, prh_void_ptr proc, prh_void_ptr coro_call_func) {
    prh_impl_coro_guard_verify(coro);
    // [32-bit architecture]
    // pstack + alloc <-- 00 <-- 16字节对齐
    //           proc <-- 12
    //                <-- 08 prh_impl_asm_coro_call
    // [64-bit architecture]
    // pstack + alloc <-- 00 <-- 16字节对齐
    //           proc <-- 08
    //                <-- 00 prh_impl_asm_coro_call
    assert((prh_unt)coro % 16 == 0); // initial rsp shall 16-byte aligned
    void **rsp = (void **)coro;
    *(--rsp) = (void *)proc;
    *(--rsp) = (void *)coro_call_func;
    coro->rspoffset = (prh_i32)((char *)coro - (char *)rsp);
}

prh_inline void prh_impl_coro_load(prh_coro *coro, prh_coroproc_t proc) {
    prh_impl_coro_load_stack(coro, (prh_void_ptr)proc, (prh_void_ptr)prh_impl_asm_coro_call);
}

prh_inline void prh_impl_soro_load(prh_coro *coro, prh_soroproc_t proc) {
    prh_impl_coro_load_stack(coro, (prh_void_ptr)proc, (prh_void_ptr)prh_impl_asm_soro_call);
}

prh_coro *prh_impl_soro_create(prh_soroproc_t proc, int stack_size, int coro_extend_size, void *userdata) {
    prh_coro *coro = prh_impl_coro_alloc(stack_size, coro_extend_size, 0);
    prh_impl_soro_load(coro, proc);
    coro->userdata = userdata;
    return coro;
}

prh_coro *prh_impl_soro_creatx(prh_soroproc_t proc, int stack_size, int coro_extend_size, int maxudsize) {
    prh_coro *coro = prh_impl_coro_alloc(stack_size, coro_extend_size, maxudsize);
    prh_impl_soro_load(coro, proc);
    return coro;
}

prh_coro **prh_impl_coro_list(prh_coro_struct *s) {
    return (prh_coro **)(s + 1);
}

prh_coro **prh_impl_coro_find_free_slot(prh_coro_struct *s) {
    assert(s->coro_cnt >= 0 && s->coro_cnt < s->maxcoros);
    prh_coro **coro_list = prh_impl_coro_list(s);
    for (int i = s->coro_cnt; i < s->maxcoros; i += 1) {
        if (coro_list[i] == prh_null) {
            s->coro_cnt = i + 1;
            return coro_list + i;
        }
    }
    return prh_null;
}

void prh_coro_create(prh_coro_struct *s, prh_coroproc_t proc, int stack_size, void *userdata) {
    prh_coro **slot = prh_impl_coro_find_free_slot(s);
    prh_coro *coro = prh_impl_coro_alloc_set(slot, stack_size, 0, 0);
    prh_impl_coro_load(coro, proc);
    coro->userdata = userdata;
#if PRH_CORO_DEBUG
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p create -- lower %p (left %d) rsp %p coro %p (size %d) data %p (size %d) -- stack %d\n",
        s->start_id + s->coro_cnt, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata, 0, stack_size);
#endif
}

void *prh_coro_creatx(prh_coro_struct *s, prh_coroproc_t proc, int stack_size, int maxudsize) {
    prh_coro **slot = prh_impl_coro_find_free_slot(s);
    prh_coro *coro = prh_impl_coro_alloc_set(slot, stack_size, 0, maxudsize);
    prh_impl_coro_load(coro, proc);
#if PRH_CORO_DEBUG
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p create -- lower %p (left %d) rsp %p coro %p (size %d) data %p (size %d) -- stack %d\n",
        s->start_id + s->coro_cnt, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata, (int)prh_round_16_byte(maxudsize), stack_size);
#endif
    return coro->userdata;
}

prh_coro *prh_impl_coro_get(prh_coro_struct *s, int index) {
    assert(index > 0 && index <= s->coro_cnt);
    return prh_impl_coro_list(s)[index - 1];
}

void prh_coro_reload(prh_coro_struct *s, int index, prh_coroproc_t proc) {
    prh_coro *coro = prh_impl_coro_get(s, index);
    assert(coro && coro->rspoffset == 0); // only finished coro can reload
    prh_impl_coro_load(coro, proc);
#if PRH_CORO_DEBUG
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p reload -- lower %p (left %d) rsp %p coro %p (size %d) data %p --\n",
        s->start_id + index, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata);
#endif
}

// 跨线程协程在每个线程中都有一个主协程，每个线程可以有多个主协程，不管是线程主动创建多
// 个主协程，还是一个主协程涉及的子协程创建了主协程，一个线程同一时间只会处理一个主协程
// 及其关联的子协程组合，而且只有当最深层的组合处理完毕才会处理浅层的组合。
prh_thread_local prh_impl_coro_main *PRH_IMPL_CORO_MAIN;

int prh_coro_self_id(prh_coro *coro) {
    struct prh_impl_coro_main *m = PRH_IMPL_CORO_MAIN;
    return m->start_id + m->curr_index;
}

int prh_soro_self_id(prh_soro *coro) {
    struct prh_impl_coro_main *m = PRH_IMPL_CORO_MAIN;
    return m->start_id + 1;
}

void prh_impl_coro_main_yield(prh_impl_coro_main *m, prh_coro *next) {
    m->next = PRH_IMPL_CORO_MAIN;
    PRH_IMPL_CORO_MAIN = m;
    prh_impl_asm_coro_yield((prh_coro *)m, next);
    PRH_IMPL_CORO_MAIN = m->next;
}

prh_coro *prh_impl_get_next_resume(prh_impl_coro_main *m) {
    for (int i = m->curr_index; i < (int)m->end_count; i += 1) {
        prh_coro *next = m->coro_list[i];
        if (next && next->rspoffset) {
            m->curr_index = i + 1;
            return next;
        }
    }
    m->curr_index = 0;
    return (prh_coro *)m;
}

prh_coro *prh_impl_next_resume_coro(void) {
    struct prh_impl_coro_main *m = PRH_IMPL_CORO_MAIN;
    if (m->yield_cycle == 0) return (prh_coro *)m;
    prh_coro *next = prh_impl_get_next_resume(m);
    return next;
}

prh_fastcall(void *) prh_impl_asm_coro_finish(prh_coro *coro) {
    coro->rspoffset = 0;
    prh_impl_coro_guard_verify(coro);
    return prh_impl_next_resume_coro();
}

prh_fastcall(void *) prh_impl_asm_soro_finish(prh_coro *coro) {
    coro->rspoffset = 0;
    prh_impl_coro_guard_verify(coro);
    return PRH_IMPL_CORO_MAIN;
}

#ifndef PRH_CONO_IMPLEMENTATION
prh_fastcall(void *) prh_impl_asm_cono_finish(prh_coro *coro) {
    return prh_impl_asm_soro_finish(coro);
}
#endif

bool prh_coro_cycle_start(prh_coro_struct *s) {
    prh_impl_coro_main mainstack;
    mainstack.coro_list = prh_impl_coro_list(s);
    mainstack.start_id = s->start_id;
    mainstack.end_count = s->coro_cnt;
    mainstack.curr_index = 0;
    mainstack.yield_cycle = 1;
    prh_coro *next = prh_impl_get_next_resume(&mainstack);
    if (next == (prh_coro *)&mainstack) {
        return false;
    }
#if PRH_CORO_DEBUG
    printf("[coro %02d] %p start %02d\n", s->start_id, (void *)&mainstack, s->start_id + mainstack.curr_index);
#endif
    prh_impl_coro_main_yield(&mainstack, next);
    return true;
}

bool prh_coro_start(prh_coro_struct *s, int index) {
    prh_impl_coro_main mainstack;
    mainstack.coro_list = prh_impl_coro_list(s);
    mainstack.start_id = s->start_id;
    mainstack.end_count = s->coro_cnt;
    mainstack.yield_cycle = 0;
    prh_coro *next = prh_impl_coro_get(s, index);
    if (next == prh_null || next->rspoffset == 0) {
        return false;
    }
    mainstack.curr_index = index;
#if PRH_CORO_DEBUG
    printf("[coro %02d] %p start %02d\n", s->start_id, (void *)&mainstack, s->start_id + mainstack.curr_index);
#endif
    prh_impl_coro_main_yield(&mainstack, next);
    return true;
}

prh_inline void prh_impl_coro_free(prh_coro *coro) {
    prh_aligned_free(prh_impl_coro_guard(coro));
}

void prh_coro_finish(prh_coro_struct **main) {
    prh_coro_struct *s = *main;
    if (s == prh_null) return;
    prh_coro **start = prh_impl_coro_list(s);
    for (int i = 0; i < s->maxcoros; i += 1) {
        prh_coro *coro = start[i];
        if (coro) {
            prh_impl_coro_free(coro);
            start[i] = prh_null;
        }
    }
    *main = prh_null;
    prh_free(s);
}

void prh_soro_init(prh_soro_struct *s, int start_id) {
    s->start_id = start_id;
    s->coro = prh_null;
}

void prh_soro_create(prh_soro_struct *s, prh_soroproc_t proc, int stack_size, void *userdata) {
    assert(s->coro == prh_null);
    s->coro = prh_impl_soro_create(proc, stack_size, 0, userdata);
#if PRH_CORO_DEBUG
    prh_coro *coro = s->coro;
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p create -- lower %p (left %d) rsp %p coro %p (size %d) data %p (size %d) -- stack %d\n",
        s->start_id + 1, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata, 0, stack_size);
#endif
}

void *prh_soro_creatx(prh_soro_struct *s, prh_soroproc_t proc, int stack_size, int maxudsize) {
    assert(s->coro == prh_null);
    prh_coro *coro = prh_impl_soro_creatx(proc, stack_size, 0, maxudsize);
    s->coro = coro;
#if PRH_CORO_DEBUG
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p create -- lower %p (left %d) rsp %p coro %p (size %d) data %p (size %d) -- stack %d\n",
        s->start_id + 1, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata, (int)prh_round_16_byte(maxudsize), stack_size);
#endif
    return coro->userdata;
}

void prh_soro_reload(prh_soro_struct *s, prh_soroproc_t proc) {
    prh_coro *coro = s->coro;
    assert(coro && coro->rspoffset == 0); // only finished coro can reload
    prh_impl_soro_load(coro, proc);
#if PRH_CORO_DEBUG
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p reload -- lower %p (left %d) rsp %p coro %p (size %d) data %p --\n",
        s->start_id + 1, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata);
#endif
}

void prh_impl_soro_start(int start_id, prh_coro *coro) {
    assert(coro && coro->rspoffset);
    prh_impl_soro_main mainstack;
    mainstack.start_id = start_id;
    prh_impl_coro_main_yield((prh_impl_coro_main *)&mainstack, coro);
}

bool prh_soro_start(prh_soro_struct *s) {
    prh_impl_soro_main mainstack;
    mainstack.start_id = s->start_id;
    prh_coro *next = s->coro;
    if (next == prh_null || next->rspoffset == 0) {
        return false;
    }
#if PRH_CORO_DEBUG
    printf("[soro %02d] %p start %02d\n", s->start_id, (void *)&mainstack, s->start_id + 1);
#endif
    prh_impl_coro_main_yield((prh_impl_coro_main *)&mainstack, next);
    return true;
}

void prh_soro_finish(prh_soro_struct *main) {
    prh_coro *coro = main->coro;
    if (coro == prh_null) return;
    prh_impl_coro_free(coro);
    main->coro = prh_null;
}
#endif // PRH_CORO_IMPLEMENTATION
#endif // PRH_CORO_INCLUDE

// https://en.cppreference.com/w/c/atomic/memory_order
// https://learn.microsoft.com/en-us/cpp/standard-library/atomic
// https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
// https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
// https://research.swtch.com/mm
#ifdef PRH_ATOMIC_INCLUDE
// 当多个线程访问一个原子对象时，所有的原子操作都会针对该原子对象产生明确的行为：在任
// 何其他原子操作能够访问该对象之前，每个原子操作都会在该对象上完整地执行完毕。这就保
// 证了在这些对象上不会出现数据竞争，而这也正是定义原子性的关键特征。
#include <stdatomic.h>

typedef prh_byte prh_atom_bool;
typedef prh_i32 prh_atom_i32;
typedef prh_u32 prh_atom_u32;
typedef prh_int prh_atom_int;
typedef prh_unt prh_atom_unt;
typedef prh_unt prh_atom_ptr;

prh_static_assert(prh_alignof(prh_atom_bool) == prh_alignof(atomic_bool));
prh_static_assert(prh_alignof(prh_atom_i32) == prh_alignof(atomic_int));
prh_static_assert(prh_alignof(prh_atom_u32) == prh_alignof(atomic_uint));
prh_static_assert(prh_alignof(prh_atom_int) == prh_alignof(atomic_intptr_t));
prh_static_assert(prh_alignof(prh_atom_unt) == prh_alignof(atomic_uintptr_t));
prh_static_assert(prh_alignof(prh_atom_ptr) == prh_alignof(atomic_uintptr_t));

prh_static_assert(sizeof(prh_atom_bool) == sizeof(atomic_bool));
prh_static_assert(sizeof(prh_atom_i32) == sizeof(atomic_int));
prh_static_assert(sizeof(prh_atom_u32) == sizeof(atomic_uint));
prh_static_assert(sizeof(prh_atom_int) == sizeof(atomic_intptr_t));
prh_static_assert(sizeof(prh_atom_unt) == sizeof(atomic_uintptr_t));
prh_static_assert(sizeof(prh_atom_ptr) == sizeof(atomic_uintptr_t));

// Initializes an existing atomic object.
// The function is not atomic: concurrent access from another thread, even
// through an atomic operation, is a data race.
// Calling this function on an atomic object that has already been initialized
// (either on construction or by calling this function earlier) causes
// undefined behavior.
// atomic_init is the only way to initialize dynamically-allocated atomic
// objects. For example:
//      _Atomic int *p = malloc(sizeof(_Atomic int));
//      atomic_init(p, 42);
prh_inline void prh_atom_bool_init(prh_atom_bool *a, bool b) { atomic_init((atomic_bool *)a, b); }
prh_inline void prh_atom_i32_init(prh_atom_i32 *a, prh_i32 b) { atomic_init((atomic_int *)a, b); }
prh_inline void prh_atom_u32_init(prh_atom_u32 *a, prh_u32 b) { atomic_init((atomic_uint *)a, b); }
prh_inline void prh_atom_int_init(prh_atom_int *a, prh_int b) { atomic_init((atomic_intptr_t *)a, b); }
prh_inline void prh_atom_unt_init(prh_atom_unt *a, prh_unt b) { atomic_init((atomic_uintptr_t *)a, b); }
prh_inline void prh_atom_ptr_init(prh_atom_ptr *a, void *b) { atomic_init((atomic_uintptr_t *)a, (prh_unt)b); }

// Atomically perform load (atomic read) operation and return the value.
// The default memory_order type is memory_order_seq_cst.
prh_inline bool prh_atom_bool_read(const prh_atom_bool *a) { return atomic_load((atomic_bool *)a); } // return read value
prh_inline prh_i32 prh_atom_i32_read(const prh_atom_i32 *a) { return atomic_load((atomic_int *)a); }
prh_inline prh_u32 prh_atom_u32_read(const prh_atom_u32 *a) { return atomic_load((atomic_uint *)a); }
prh_inline prh_int prh_atom_int_read(const prh_atom_int *a) { return atomic_load((atomic_intptr_t *)a); }
prh_inline prh_unt prh_atom_unt_read(const prh_atom_unt *a) { return atomic_load((atomic_uintptr_t *)a); }
prh_inline void *prh_atom_ptr_read(const prh_atom_ptr *a) { return (void *)atomic_load((atomic_uintptr_t *)a); }

// Atomically perform write or read-modify-write operation.
// The default memory_order type is memory_order_seq_cst.
prh_inline void prh_atom_bool_write(prh_atom_bool *a, bool b) { atomic_store((atomic_bool *)a, b); }
prh_inline void prh_atom_i32_write(prh_atom_i32 *a, prh_i32 b) { atomic_store((atomic_int *)a, b); }
prh_inline void prh_atom_u32_write(prh_atom_u32 *a, prh_u32 b) { atomic_store((atomic_uint *)a, b); }
prh_inline void prh_atom_int_write(prh_atom_int *a, prh_int b) { atomic_store((atomic_intptr_t *)a, b); }
prh_inline void prh_atom_unt_write(prh_atom_unt *a, prh_unt b) { atomic_store((atomic_uintptr_t *)a, b); }
prh_inline void prh_atom_ptr_write(prh_atom_ptr *a, void *b) { atomic_store((atomic_uintptr_t *)a, (prh_unt)b); }

prh_inline bool prh_atom_bool_fetch_write(prh_atom_bool *a, bool b) { return atomic_exchange((atomic_bool *)a, b); } // return old value
prh_inline prh_i32 prh_atom_i32_fetch_write(prh_atom_i32 *a, prh_i32 b) { return atomic_exchange((atomic_int *)a, b); }
prh_inline prh_u32 prh_atom_u32_fetch_write(prh_atom_u32 *a, prh_u32 b) { return atomic_exchange((atomic_uint *)a, b); }
prh_inline prh_int prh_atom_int_fetch_write(prh_atom_int *a, prh_int b) { return atomic_exchange((atomic_intptr_t *)a, b); }
prh_inline prh_unt prh_atom_unt_fetch_write(prh_atom_unt *a, prh_unt b) { return atomic_exchange((atomic_uintptr_t *)a, b); }
prh_inline void *prh_atom_ptr_fetch_write(prh_atom_ptr *a, void *b) { return (void *)atomic_exchange((atomic_uintptr_t *)a, (prh_unt)b); }

// Atomically perform read-modify-wirte operation, and return the old value.
// The modify operation can be add (+) sub (-) and (&) or (|) xor (^).
// The default memory_order type is memory_order_seq_cst.
prh_inline void prh_atom_i32_inc(prh_atom_i32 *a) { atomic_fetch_add((atomic_int *)a, 1); }
prh_inline void prh_atom_u32_inc(prh_atom_u32 *a) { atomic_fetch_add((atomic_uint *)a, 1); }
prh_inline void prh_atom_int_inc(prh_atom_int *a) { atomic_fetch_add((atomic_intptr_t *)a, 1); }
prh_inline void prh_atom_unt_inc(prh_atom_unt *a) { atomic_fetch_add((atomic_uintptr_t *)a, 1); }

prh_inline void prh_atom_i32_add(prh_atom_i32 *a, prh_i32 b) { atomic_fetch_add((atomic_int *)a, b); }
prh_inline void prh_atom_u32_add(prh_atom_u32 *a, prh_u32 b) { atomic_fetch_add((atomic_uint *)a, b); }
prh_inline void prh_atom_int_add(prh_atom_int *a, prh_int b) { atomic_fetch_add((atomic_intptr_t *)a, b); }
prh_inline void prh_atom_unt_add(prh_atom_unt *a, prh_unt b) { atomic_fetch_add((atomic_uintptr_t *)a, b); }

prh_inline void prh_atom_i32_dec(prh_atom_i32 *a) { atomic_fetch_sub((atomic_int *)a, 1); }
prh_inline void prh_atom_u32_dec(prh_atom_u32 *a) { atomic_fetch_sub((atomic_uint *)a, 1); }
prh_inline void prh_atom_int_dec(prh_atom_int *a) { atomic_fetch_sub((atomic_intptr_t *)a, 1); }
prh_inline void prh_atom_unt_dec(prh_atom_unt *a) { atomic_fetch_sub((atomic_uintptr_t *)a, 1); }

prh_inline void prh_atom_i32_sub(prh_atom_i32 *a, prh_i32 b) { atomic_fetch_sub((atomic_int *)a, b); }
prh_inline void prh_atom_u32_sub(prh_atom_u32 *a, prh_u32 b) { atomic_fetch_sub((atomic_uint *)a, b); }
prh_inline void prh_atom_int_sub(prh_atom_int *a, prh_int b) { atomic_fetch_sub((atomic_intptr_t *)a, b); }
prh_inline void prh_atom_unt_sub(prh_atom_unt *a, prh_unt b) { atomic_fetch_sub((atomic_uintptr_t *)a, b); }

// Atomically perform compare-and-exchange (or compare-and-swap CAS) operation.
//      if read value == *expect value { write newvalue and return true }
//      else { *expect = read value and return false }
// If success it operform atomic read-modify-write operation, otherwise it
// opertion load (load actual content into *expect) operation.
// The weak forms of the functions are allowed to fail spuriously, that is,
// even if `read value == *expect value` it still goes to else and return
// false. On these spurious failures, the function returns false while not
// modifying expected.
// This may be acceptable behavior for certain looping algorithms, and
// may lead to significantly better performance on some platforms.
// For non-looping algorithms, atomic_compare_exchange_strong is generally
// preferred.
// The default memory_order type is memory_order_seq_cst.
prh_inline bool prh_atom_bool_compare_write(prh_atom_bool *a, bool *expect, bool b) { return atomic_compare_exchange_weak((atomic_bool *)a, expect, b); }
prh_inline bool prh_atom_i32_compare_write(prh_atom_i32 *a, prh_i32 *expect, prh_i32 b) { return atomic_compare_exchange_weak((atomic_int *)a, expect, b); }
prh_inline bool prh_atom_u32_compare_write(prh_atom_u32 *a, prh_u32 *expect, prh_u32 b) { return atomic_compare_exchange_weak((atomic_uint *)a, expect, b); }
prh_inline bool prh_atom_int_compare_write(prh_atom_int *a, prh_int *expect, prh_int b) { return atomic_compare_exchange_weak((atomic_intptr_t *)a, expect, b); }
prh_inline bool prh_atom_unt_compare_write(prh_atom_unt *a, prh_unt *expect, prh_unt b) { return atomic_compare_exchange_weak((atomic_uintptr_t *)a, expect, b); }
prh_inline bool prh_atom_ptr_compare_write(prh_atom_ptr *a, void **expect, void *b) { return atomic_compare_exchange_weak((atomic_uintptr_t *)a, (prh_unt *)expect, (prh_unt)b); }

prh_inline bool prh_atom_bool_strong_compare_write(prh_atom_bool *a, bool *expect, bool b) { return atomic_compare_exchange_strong((atomic_bool *)a, expect, b); }
prh_inline bool prh_atom_i32_strong_compare_write(prh_atom_i32 *a, prh_i32 *expect, prh_i32 b) { return atomic_compare_exchange_strong((atomic_int *)a, expect, b); }
prh_inline bool prh_atom_u32_strong_compare_write(prh_atom_u32 *a, prh_u32 *expect, prh_u32 b) { return atomic_compare_exchange_strong((atomic_uint *)a, expect, b); }
prh_inline bool prh_atom_int_strong_compare_write(prh_atom_int *a, prh_int *expect, prh_int b) { return atomic_compare_exchange_strong((atomic_intptr_t *)a, expect, b); }
prh_inline bool prh_atom_unt_strong_compare_write(prh_atom_unt *a, prh_unt *expect, prh_unt b) { return atomic_compare_exchange_strong((atomic_uintptr_t *)a, expect, b); }
prh_inline bool prh_atom_ptr_strong_compare_write(prh_atom_ptr *a, void **expect, void *b) { return atomic_compare_exchange_strong((atomic_uintptr_t *)a, (prh_unt *)expect, (prh_unt)b); }

prh_inline bool prh_atom_bool_compare_write_false(prh_atom_bool *a) { bool expect = true; return atomic_compare_exchange_weak((atomic_bool *)a, &expect, false); }
prh_inline bool prh_atom_bool_compare_write_true(prh_atom_bool *a) { bool expect = false; return atomic_compare_exchange_weak((atomic_bool *)a, &expect, true); }
prh_inline bool prh_atom_bool_strong_compare_write_false(prh_atom_bool *a) { bool expect = true; return atomic_compare_exchange_strong((atomic_bool *)a, &expect, false); }
prh_inline bool prh_atom_bool_strong_compare_write_true(prh_atom_bool *a) { bool expect = false; return atomic_compare_exchange_strong((atomic_bool *)a, &expect, true); }

prh_inline bool prh_atom_i32_compare_zero_write(prh_atom_i32 *a, prh_i32 b) { prh_i32 expect = 0; return atomic_compare_exchange_weak((atomic_int *)a, &expect, b); }
prh_inline bool prh_atom_u32_compare_zero_write(prh_atom_u32 *a, prh_u32 b) { prh_u32 expect = 0; return atomic_compare_exchange_weak((atomic_uint *)a, &expect, b); }
prh_inline bool prh_atom_int_compare_zero_write(prh_atom_int *a, prh_int b) { prh_int expect = 0; return atomic_compare_exchange_weak((atomic_intptr_t *)a, &expect, b); }
prh_inline bool prh_atom_unt_compare_zero_write(prh_atom_unt *a, prh_unt b) { prh_unt expect = 0; return atomic_compare_exchange_weak((atomic_uintptr_t *)a, &expect, b); }
prh_inline bool prh_atom_ptr_compare_null_write(prh_atom_ptr *a, void *b) { prh_unt expect = 0; return atomic_compare_exchange_weak((atomic_uintptr_t *)a, &expect, (prh_unt)b); }

prh_inline bool prh_atom_i32_strong_compare_zero_write(prh_atom_i32 *a, prh_i32 b) { prh_i32 expect = 0; return atomic_compare_exchange_strong((atomic_int *)a, &expect, b); }
prh_inline bool prh_atom_u32_strong_compare_zero_write(prh_atom_u32 *a, prh_u32 b) { prh_u32 expect = 0; return atomic_compare_exchange_strong((atomic_uint *)a, &expect, b); }
prh_inline bool prh_atom_int_strong_compare_zero_write(prh_atom_int *a, prh_int b) { prh_int expect = 0; return atomic_compare_exchange_strong((atomic_intptr_t *)a, &expect, b); }
prh_inline bool prh_atom_unt_strong_compare_zero_write(prh_atom_unt *a, prh_unt b) { prh_unt expect = 0; return atomic_compare_exchange_strong((atomic_uintptr_t *)a, &expect, b); }
prh_inline bool prh_atom_ptr_strong_compare_null_write(prh_atom_ptr *a, void *b) { prh_unt expect = 0; return atomic_compare_exchange_strong((atomic_uintptr_t *)a, &expect, (prh_unt)b); }

// Atomic singly linked queue for only 1 producer and 1 consumer. Each node has
// a prh_snode header and a data pointer. The node has fixed size and can only
// contain a data pointer. The fixed size node is dynamic alloced by the queue.
typedef struct {
    prh_data_snode *head; // 只由单一消费者读写, only modified by pop thread
    prh_data_snode *tail; // 只由单一生产者读写, only modified by push thread
    prh_data_snode_head free; // 仅由生产者读写，only modified by push thread
    prh_atom_ptr last; // 生产者无条件写，消费者负责读和有条件写
} prh_atom_data_quefix;

void prh_atom_data_quefix_init(prh_atom_data_quefix *q);
void prh_atom_data_quefix_push(prh_atom_data_quefix *s, void *data); // data shall not be null
void *prh_atom_data_quefix_top(prh_atom_data_quefix *s); // return null means empty
void *prh_atom_data_quefix_pop(prh_atom_data_quefix *s); // return null means empty
bool prh_atom_data_quefix_pop_all(prh_atom_data_quefix *q, prh_data_quefit *out);
void prh_atom_data_quefix_free_node(prh_data_quefit *q);

#ifdef PRH_ATOMIC_IMPLEMENTATION
#define PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE PRH_CACHE_LINE_SIZE // block size shall be power of 2
#define PRH_IMPL_ATOM_DYNQUE_NODE_COUNT (PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE / sizeof(prh_data_snode))
prh_static_assert(PRH_IMPL_ATOM_DYNQUE_NODE_COUNT >= 2); // 如果不能分配2个或2个以上，间接块分配没有意义
prh_static_assert(PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE % sizeof(prh_data_snode) == 0);

void prh_impl_atom_data_quefix_more_free(prh_atom_data_quefix *q, prh_data_snode *node) {
    prh_data_snode *last = node + PRH_IMPL_ATOM_DYNQUE_NODE_COUNT - 1;
    prh_data_snode *first = q->free.next;
    q->free.next = node; // add node list to free head
    last->next = first;
    for (; ;) {
        node->next = node + 1;
        node = node + 1;
        if (node == last) {
            break;
        }
    }
}

prh_data_snode *prh_impl_atom_data_quefix_aligned_alloc(prh_atom_data_quefix *q) { // called by push thread
    prh_data_snode *node;
    if (q->free.next == prh_null) {
        assert(prh_is_power_of_2(PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE));
        prh_impl_atom_data_quefix_more_free(q, prh_cache_line_aligned_malloc(PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE));
    }
    node = q->free.next; // remove first free node
    q->free.next = node->next;
    node->next = prh_null; // node->data is set by push
    return node;
}

void prh_impl_atom_data_quefix_aligned_free(prh_data_snode *node) { // called by pop thread
    if (((prh_unt)(node + 1) & (prh_unt)(PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE - 1)) == 0) {
        prh_aligned_free(node + 1 - PRH_IMPL_ATOM_DYNQUE_NODE_COUNT);
    }
}

void prh_atom_data_quefix_init(prh_atom_data_quefix *q) {
    q->free.next = prh_null;
    q->head = q->tail = prh_impl_atom_data_quefix_aligned_alloc(q); // 总是分配一个tail空节点，让非空head永远追不上tail
    prh_atom_ptr_init(&q->last, prh_null);
}

typedef struct {
    prh_atom_data_quefix *queue;
    prh_data_snode *alloc_tail;
#if PRH_DEBUG
    prh_data_snode *queue_tail;
#endif
} prh_impl_atom_data_quefix_allocator;

void prh_impl_atom_data_quefix_alloc_begin(prh_atom_data_quefix *q, prh_impl_atom_data_quefix_allocator *p) {
    p->queue = q;
    p->alloc_tail = q->tail;
#if PRH_DEBUG
    p->queue_tail = q->tail;
#endif
}

prh_data_snode *prh_impl_atom_data_quefix_alloc_node(prh_impl_atom_data_quefix_allocator *p, void *data) {
    assert(data != prh_null);
    prh_data_snode *node = p->alloc_tail;
    p->alloc_tail = prh_impl_atom_data_quefix_aligned_alloc(p->queue);
    node->data = data;
    return node;
}

void prh_impl_atom_data_quefix_alloc_end_and_push_node_back(prh_impl_atom_data_quefix_allocator *p) {
    prh_atom_data_quefix *q = p->queue;
    prh_data_snode *tail = p->alloc_tail;
#if PRH_DEBUG
    assert(q->tail == p->queue_tail);
#endif
    q->tail = tail;
    tail->next = prh_null;
}

void prh_atom_data_quefix_push(prh_atom_data_quefix *q, void *data) {
    assert(data != prh_null); // push不会读写head，也不会读写已经存在的节点
    prh_data_snode *null_tail = prh_impl_atom_data_quefix_aligned_alloc(q);
    prh_data_snode *last = q->tail;
    q->tail = null_tail; // push只会更新tail和tail空节点，且push对应单一生产者，因此tail不需要atom
    last->next = null_tail;
    last->data = data;
    assert(q->tail == null_tail); // 只允许唯一生产者
    prh_atom_ptr_write(&q->last, last); // 此步骤执行完毕以上更新必须对所有cpu生效
}

bool prh_atom_data_quefix_empty(prh_atom_data_quefix *q) {
    return prh_atom_ptr_read(&q->last) == prh_null;
}

void *prh_atom_data_quefix_top(prh_atom_data_quefix *q) {
    if (!prh_atom_ptr_read(&q->last)) return prh_null;
    return q->head->data;
}

void *prh_atom_data_quefix_pop(prh_atom_data_quefix *q) {
    prh_data_snode *last = prh_atom_ptr_read(&q->last);
    if (last == prh_null) return prh_null;
    prh_data_snode *head = q->head;
    prh_data_snode *next = head->next;
    void *data = head->data; // pop不会读写tail，也不会读写tail空节点
    q->head = next; // pop只会更新head和读写已存在的头节点，且pop对应单一消费者，因此head不需要atom
    if (head == last) { // 使用 compare write 是因为 q->last 会随时被 push 更新
        prh_atom_ptr_strong_compare_write(&q->last, (void **)&last, prh_null);
    }
    prh_impl_atom_data_quefix_aligned_free(head);
    assert(q->head == next); // 只允许唯一消费者
    return data;
}

bool prh_atom_data_quefix_pop_all(prh_atom_data_quefix *q, prh_data_quefit *out) {
    prh_data_snode *last = prh_atom_ptr_read(&q->last);
    if (last == prh_null) return false;
    out->tail = last;
    out->head = q->head;
    q->head = last->next; // pop all
    prh_atom_ptr_strong_compare_write(&q->last, (void **)&last, prh_null);
    assert(q->head == last->next); // 只允许唯一消费者
    last->next = prh_null; // quefit最后一个节点指向空，方便遍历和移动
    return true;
}

void prh_atom_data_quefix_free_node(prh_data_quefit *q) {
    prh_data_snode *head = q->head;
    for (; head; head = head->next) {
        prh_impl_atom_data_quefix_aligned_free(head);
    }
}
#endif // PRH_ATOMIC_IMPLEMENTATION

typedef struct prh_hive_quefix_block {
    void **tail;
    struct prh_hive_quefix_block *next;
} prh_hive_quefix_block;

// 单生产者单消费者内存块链队列，每个内存块的大小固定
typedef struct {
    prh_hive_quefix_block *head; // 仅由pop线程访问
    prh_int head_index;          // 仅由pop线程访问
    prh_hive_quefix_block *tail; // 仅由push线程访问
    prh_atom_int len;
} prh_atom_hive_quefix;

prh_inline bool prh_atom_hive_quefix_empty(prh_atom_hive_quefix *q) {
    return !prh_atom_int_read(&q->len);
}

void prh_atom_hive_quefix_init(prh_atom_hive_quefix *q);
void prh_atom_hive_quefix_push(prh_atom_hive_quefix *q, void *data);
void *prh_atom_hive_quefix_top(prh_atom_hive_quefix *q);
void *prh_atom_hive_quefix_pop(prh_atom_hive_quefix *q);
bool prh_atom_hive_quefix_pop_all(prh_atom_hive_quefix *q, void (*cb)(void *priv, void *data), void *priv);

#ifdef PRH_ATOMIC_IMPLEMENTATION
#define PRH_IMPL_ATOM_HIVQUE_BLOCK_SIZE PRH_CACHE_LINE_SIZE // block size shall be power of 2
#define PRH_IMPL_ATOM_HIVQUE_BELT_COUNT (PRH_IMPL_ATOM_HIVQUE_BLOCK_SIZE / sizeof(void *))
#define PRH_IMPL_ATOM_HIVQUE_HEAD_COUNT (prh_int)(sizeof(prh_hive_quefix_block) / sizeof(void *))
prh_static_assert(PRH_IMPL_ATOM_HIVQUE_BELT_COUNT >= 2); // 如果不能分配2个或2个以上，间接块分配没有意义
prh_static_assert(PRH_IMPL_ATOM_HIVQUE_BLOCK_SIZE % PRH_CACHE_LINE_SIZE == 0);

prh_inline void **prh_impl_atom_hive_quefix_block_end(prh_hive_quefix_block *b) {
    return (void **)((prh_byte *)b + PRH_IMPL_ATOM_HIVQUE_BLOCK_SIZE);
}

prh_hive_quefix_block *prh_impl_atom_hive_quefix_alloc_block(void) {
    prh_hive_quefix_block *b = prh_cache_line_aligned_malloc(PRH_IMPL_ATOM_HIVQUE_BLOCK_SIZE);
    b->tail = (void **)(b + 1);
    b->next = prh_null;
    return b;
}

void prh_atom_hive_quefix_init(prh_atom_hive_quefix *q) {
    prh_hive_quefix_block *b = prh_impl_atom_hive_quefix_alloc_block();
    q->head = q->tail = b;
    q->head_index = PRH_IMPL_ATOM_HIVQUE_HEAD_COUNT;
    prh_atom_int_init(&q->len, 0);
}

void prh_atom_hive_quefix_push(prh_atom_hive_quefix *q, void *data) {
    assert(data != prh_null);
    prh_hive_quefix_block *b = q->tail;
    *b->tail++ = data;
    if (b->tail >= prh_impl_atom_hive_quefix_block_end(b)) {
        q->tail = b->next = prh_impl_atom_hive_quefix_alloc_block();
    }
    prh_atom_int_inc(&q->len); // 此步骤执行完毕以上更新必须对所有cpu生效
}

void *prh_atom_hive_quefix_top(prh_atom_hive_quefix *q) {
    if (!prh_atom_int_read(&q->len)) return prh_null;
    return *((void **)q->head + q->head_index);
}

void *prh_atom_hive_quefix_pop(prh_atom_hive_quefix *q) {
    if (!prh_atom_int_read(&q->len)) return prh_null;
    prh_hive_quefix_block *head = q->head;
    void *data = *((void **)head + q->head_index++);
    if (q->head_index >= PRH_IMPL_ATOM_HIVQUE_BELT_COUNT) {
        q->head = head->next;
        q->head_index = PRH_IMPL_ATOM_HIVQUE_HEAD_COUNT;
        prh_aligned_free(head);
    }
    prh_atom_int_dec(&q->len); // 此步骤执行完毕以上更新必须对所有cpu生效
    return data;
}

bool prh_atom_hive_quefix_pop_all(prh_atom_hive_quefix *q, void (*cb)(void *priv, void *data), void *priv) {
    prh_int len = prh_atom_int_read(&q->len); assert(len >= 0);
    if (len == 0) return false;
    prh_hive_quefix_block *head = q->head;
    prh_int head_index = q->head_index;
    for (prh_int i = 0; i < len; i += 1) {
        cb(priv, *((void **)head + head_index++));
        if (head_index >= PRH_IMPL_ATOM_HIVQUE_BELT_COUNT) {
            prh_hive_quefix_block *next = head->next;
            head_index = PRH_IMPL_ATOM_HIVQUE_HEAD_COUNT;
            prh_aligned_free(head);
            head = next;
        }
    }
    q->head = head;
    q->head_index = head_index;
    prh_atom_int_sub(&q->len, len);
    return true;
}
#endif // PRH_ATOMIC_IMPLEMENTATION

// 单生产者单消费者固定大小的数组队列，数组长度必须是2的幂
typedef struct prh_atom_i32_arrque prh_atom_i32_arrque;
typedef struct prh_atom_u32_arrque prh_atom_u32_arrque;
typedef struct prh_atom_int_arrque prh_atom_int_arrque;
typedef struct prh_atom_unt_arrque prh_atom_unt_arrque;
typedef struct prh_atom_ptr_arrque prh_atom_ptr_arrque;

// fixed size array atomic queue for only 1 producer and 1 consumer, the size must be power of 2
void *prh_impl_atom_arrque_alloc(prh_int size, prh_int alloc);
void prh_impl_atom_arrque_free(void *q);
prh_int prh_impl_atom_arrque_len(void *q);
bool prh_impl_atom_arrque_push_u32(void *q, prh_u32 a);
bool prh_impl_atom_arrque_push_unt(void *q, prh_unt a);
bool prh_impl_atom_arrque_pop_all_begin(void *arrque, void *out);
void prh_impl_atom_arrque_pop_all_end(void *arrque, void *data);
prh_u32 prh_impl_atom_arrque_top_u32(void *q);
prh_unt prh_impl_atom_arrque_top_unt(void *q);
prh_u32 prh_impl_atom_arrque_pop_u32(void *q);
prh_unt prh_impl_atom_arrque_pop_unt(void *q);

prh_inline prh_atom_i32_arrque *prh_atom_i32_arrque_init(prh_int size) { return prh_impl_atom_arrque_alloc(size, sizeof(prh_i32) * size); }
prh_inline prh_atom_u32_arrque *prh_atom_u32_arrque_init(prh_int size) { return prh_impl_atom_arrque_alloc(size, sizeof(prh_u32) * size); }
prh_inline prh_atom_int_arrque *prh_atom_int_arrque_init(prh_int size) { return prh_impl_atom_arrque_alloc(size, sizeof(prh_int) * size); }
prh_inline prh_atom_unt_arrque *prh_atom_unt_arrque_init(prh_int size) { return prh_impl_atom_arrque_alloc(size, sizeof(prh_unt) * size); }
prh_inline prh_atom_ptr_arrque *prh_atom_ptr_arrque_init(prh_int size) { return prh_impl_atom_arrque_alloc(size, sizeof(void *) * size); }

prh_inline void prh_atom_i32_arrque_free(prh_atom_i32_arrque *q) { prh_impl_atom_arrque_free(q); }
prh_inline void prh_atom_u32_arrque_free(prh_atom_u32_arrque *q) { prh_impl_atom_arrque_free(q); }
prh_inline void prh_atom_int_arrque_free(prh_atom_int_arrque *q) { prh_impl_atom_arrque_free(q); }
prh_inline void prh_atom_unt_arrque_free(prh_atom_unt_arrque *q) { prh_impl_atom_arrque_free(q); }
prh_inline void prh_atom_ptr_arrque_free(prh_atom_ptr_arrque *q) { prh_impl_atom_arrque_free(q); }

prh_inline bool prh_atom_i32_arrque_empty(prh_atom_i32_arrque *q) { return prh_impl_atom_arrque_len(q) == 0; }
prh_inline bool prh_atom_u32_arrque_empty(prh_atom_u32_arrque *q) { return prh_impl_atom_arrque_len(q) == 0; }
prh_inline bool prh_atom_int_arrque_empty(prh_atom_int_arrque *q) { return prh_impl_atom_arrque_len(q) == 0; }
prh_inline bool prh_atom_unt_arrque_empty(prh_atom_unt_arrque *q) { return prh_impl_atom_arrque_len(q) == 0; }
prh_inline bool prh_atom_ptr_arrque_empty(prh_atom_ptr_arrque *q) { return prh_impl_atom_arrque_len(q) == 0; }

prh_inline prh_int prh_atom_i32_arrque_len(prh_atom_i32_arrque *q) { return prh_impl_atom_arrque_len(q); }
prh_inline prh_int prh_atom_u32_arrque_len(prh_atom_u32_arrque *q) { return prh_impl_atom_arrque_len(q); }
prh_inline prh_int prh_atom_int_arrque_len(prh_atom_int_arrque *q) { return prh_impl_atom_arrque_len(q); }
prh_inline prh_int prh_atom_unt_arrque_len(prh_atom_unt_arrque *q) { return prh_impl_atom_arrque_len(q); }
prh_inline prh_int prh_atom_ptr_arrque_len(prh_atom_ptr_arrque *q) { return prh_impl_atom_arrque_len(q); }

prh_inline bool prh_atom_i32_arrque_push(prh_atom_i32_arrque *q, prh_i32 a) { return prh_impl_atom_arrque_push_u32(q, a); }
prh_inline bool prh_atom_u32_arrque_push(prh_atom_u32_arrque *q, prh_u32 a) { return prh_impl_atom_arrque_push_u32(q, a); }
prh_inline bool prh_atom_int_arrque_push(prh_atom_int_arrque *q, prh_int a) { return prh_impl_atom_arrque_push_unt(q, a); }
prh_inline bool prh_atom_unt_arrque_push(prh_atom_unt_arrque *q, prh_unt a) { return prh_impl_atom_arrque_push_unt(q, a); }
prh_inline bool prh_atom_ptr_arrque_push(prh_atom_ptr_arrque *q, void *a) { return prh_impl_atom_arrque_push_unt(q, (prh_unt)a); }

prh_inline prh_i32 prh_atom_i32_arrque_top(prh_atom_i32_arrque *q) { return (prh_i32)prh_impl_atom_arrque_top_u32(q); }
prh_inline prh_u32 prh_atom_u32_arrque_top(prh_atom_u32_arrque *q) { return (prh_u32)prh_impl_atom_arrque_top_u32(q); }
prh_inline prh_int prh_atom_int_arrque_top(prh_atom_int_arrque *q) { return (prh_int)prh_impl_atom_arrque_top_unt(q); }
prh_inline prh_unt prh_atom_unt_arrque_top(prh_atom_unt_arrque *q) { return (prh_unt)prh_impl_atom_arrque_top_unt(q); }
prh_inline void *prh_atom_ptr_arrque_top(prh_atom_ptr_arrque *q) { return (void *)prh_impl_atom_arrque_top_unt(q); }

prh_inline prh_i32 prh_atom_i32_arrque_pop(prh_atom_i32_arrque *q) { return (prh_i32)prh_impl_atom_arrque_pop_u32(q); }
prh_inline prh_u32 prh_atom_u32_arrque_pop(prh_atom_u32_arrque *q) { return (prh_u32)prh_impl_atom_arrque_pop_u32(q); }
prh_inline prh_int prh_atom_int_arrque_pop(prh_atom_int_arrque *q) { return (prh_int)prh_impl_atom_arrque_pop_unt(q); }
prh_inline prh_unt prh_atom_unt_arrque_pop(prh_atom_unt_arrque *q) { return (prh_unt)prh_impl_atom_arrque_pop_unt(q); }
prh_inline void *prh_atom_ptr_arrque_pop(prh_atom_ptr_arrque *q) { return (void *)prh_impl_atom_arrque_pop_unt(q); }

typedef struct { prh_i32 *elem; prh_int head; prh_int len; prh_int size; } prh_atom_i32_arrque_data;
typedef struct { prh_u32 *elem; prh_int head; prh_int len; prh_int size; } prh_atom_u32_arrque_data;
typedef struct { prh_int *elem; prh_int head; prh_int len; prh_int size; } prh_atom_int_arrque_data;
typedef struct { prh_unt *elem; prh_int head; prh_int len; prh_int size; } prh_atom_unt_arrque_data;
typedef struct { void **elem; prh_int head; prh_int len; prh_int size; } prh_atom_ptr_arrque_data;

prh_inline bool prh_atom_i32_arrque_pop_all_begin(prh_atom_i32_arrque *q, prh_atom_i32_arrque_data *data) { return prh_impl_atom_arrque_pop_all_begin(q, data); }
prh_inline bool prh_atom_u32_arrque_pop_all_begin(prh_atom_u32_arrque *q, prh_atom_u32_arrque_data *data) { return prh_impl_atom_arrque_pop_all_begin(q, data); }
prh_inline bool prh_atom_int_arrque_pop_all_begin(prh_atom_int_arrque *q, prh_atom_int_arrque_data *data) { return prh_impl_atom_arrque_pop_all_begin(q, data); }
prh_inline bool prh_atom_unt_arrque_pop_all_begin(prh_atom_unt_arrque *q, prh_atom_unt_arrque_data *data) { return prh_impl_atom_arrque_pop_all_begin(q, data); }
prh_inline bool prh_atom_ptr_arrque_pop_all_begin(prh_atom_ptr_arrque *q, prh_atom_ptr_arrque_data *data) { return prh_impl_atom_arrque_pop_all_begin(q, data); }

prh_inline prh_i32 prh_atom_i32_arrque_get_data(prh_atom_i32_arrque_data *p, prh_int index) { return p->elem[((p->head + index) & (p->size - 1))]; }
prh_inline prh_u32 prh_atom_u32_arrque_get_data(prh_atom_u32_arrque_data *p, prh_int index) { return p->elem[((p->head + index) & (p->size - 1))]; }
prh_inline prh_int prh_atom_int_arrque_get_data(prh_atom_int_arrque_data *p, prh_int index) { return p->elem[((p->head + index) & (p->size - 1))]; }
prh_inline prh_unt prh_atom_unt_arrque_get_data(prh_atom_unt_arrque_data *p, prh_int index) { return p->elem[((p->head + index) & (p->size - 1))]; }
prh_inline void *prh_atom_ptr_arrque_get_data(prh_atom_ptr_arrque_data *p, prh_int index) { return p->elem[((p->head + index) & (p->size - 1))]; }

prh_inline void prh_atom_i32_arrque_pop_all_end(prh_atom_i32_arrque *q, prh_atom_i32_arrque_data *data) { prh_impl_atom_arrque_pop_all_end(q, data); }
prh_inline void prh_atom_u32_arrque_pop_all_end(prh_atom_u32_arrque *q, prh_atom_u32_arrque_data *data) { prh_impl_atom_arrque_pop_all_end(q, data); }
prh_inline void prh_atom_int_arrque_pop_all_end(prh_atom_int_arrque *q, prh_atom_int_arrque_data *data) { prh_impl_atom_arrque_pop_all_end(q, data); }
prh_inline void prh_atom_unt_arrque_pop_all_end(prh_atom_unt_arrque *q, prh_atom_unt_arrque_data *data) { prh_impl_atom_arrque_pop_all_end(q, data); }
prh_inline void prh_atom_ptr_arrque_pop_all_end(prh_atom_ptr_arrque *q, prh_atom_ptr_arrque_data *data) { prh_impl_atom_arrque_pop_all_end(q, data); }

#ifdef PRH_ATOMIC_IMPLEMENTATION
typedef struct {
    prh_int head; // 仅由pop线程读写，head永远追不上tail
    prh_int tail; // 仅由push线程读写
    prh_int size; // 数组固定大小，必须是2的幂，只读
    prh_atom_int len;
} prh_impl_atom_arrque;

typedef struct {
    void *elem;
    prh_int head;
    prh_int len;
    prh_int size;
} prh_impl_atom_arrque_data;

void *prh_impl_atom_arrque_alloc(prh_int size, prh_int alloc) {
    assert(prh_is_power_of_2(size));
    prh_impl_atom_arrque *q = prh_malloc(sizeof(prh_impl_atom_arrque) + alloc);
    q->head = q->tail = 0;
    q->size = size;
    prh_atom_int_init(&q->len, 0);
    return q;
}

void prh_impl_atom_arrque_free(void *q) {
    prh_free(q);
}

prh_int prh_impl_atom_arrque_len(void *q) {
    return prh_atom_int_read(&((prh_impl_atom_arrque *)q)->len);
}

prh_inline bool prh_impl_atom_arrque_full(prh_impl_atom_arrque *q) {
    return prh_atom_int_read(&q->len) >= (q->size - 1);
}

#define prh_impl_atom_arrque_next_pos(q, pos) (((pos) + 1) & ((q)->size - 1))
prh_inline void *prh_impl_atom_arrque_elem(prh_impl_atom_arrque *q) {
    return q + 1;
}

bool prh_impl_atom_arrque_push_u32(void *arrque, prh_u32 a) {
    prh_impl_atom_arrque *q = arrque; // push线程只操作q->tail 永远不读写q->head
    if (prh_impl_atom_arrque_full(q)) return false;
    prh_int tail = q->tail; assert(a != 0);
    ((prh_u32 *)prh_impl_atom_arrque_elem(q))[tail] = a;
    assert(q->tail == tail); // 只允许一个生产者
    q->tail = prh_impl_atom_arrque_next_pos(q, tail);
    prh_atom_int_inc(&q->len);
    return true;
}

bool prh_impl_atom_arrque_push_unt(void *arrque, prh_unt a) {
    prh_impl_atom_arrque *q = arrque; // push线程只操作q->tail 永远不读写q->head
    if (prh_impl_atom_arrque_full(q)) return false;
    prh_int tail = q->tail; assert(a != 0);
    ((prh_unt *)prh_impl_atom_arrque_elem(q))[tail] = a;
    assert(q->tail == tail); // 只允许一个生产者
    q->tail = prh_impl_atom_arrque_next_pos(q, tail);
    prh_atom_int_inc(&q->len);
    return true;
}

prh_u32 prh_impl_atom_arrque_top_u32(void *arrque) {
    prh_impl_atom_arrque *q = arrque;
    if (!prh_impl_atom_arrque_len(q)) return 0;
    return ((prh_u32 *)prh_impl_atom_arrque_elem(q))[q->head];
}

prh_unt prh_impl_atom_arrque_top_unt(void *arrque) {
    prh_impl_atom_arrque *q = arrque;
    if (!prh_impl_atom_arrque_len(q)) return 0;
    return ((prh_unt *)prh_impl_atom_arrque_elem(q))[q->head];
}

prh_u32 prh_impl_atom_arrque_pop_u32(void *arrque) {
    prh_impl_atom_arrque *q = arrque; // push线程只操作q->head 永远不读写q->tail
    if (!prh_impl_atom_arrque_len(q)) return 0;
    prh_int head = q->head;
    prh_u32 a = ((prh_u32 *)prh_impl_atom_arrque_elem(q))[head];
    assert(q->head == head); // 只允许一个消费者
    q->head = prh_impl_atom_arrque_next_pos(q, head);
    prh_atom_int_dec(&q->len);
    return a;
}

prh_unt prh_impl_atom_arrque_pop_unt(void *arrque) {
    prh_impl_atom_arrque *q = arrque; // push线程只操作q->head 永远不读写q->tail
    if (!prh_impl_atom_arrque_len(q)) return 0;
    prh_int head = q->head;
    prh_unt a = ((prh_unt *)prh_impl_atom_arrque_elem(q))[head];
    assert(q->head == head); // 只允许一个消费者
    q->head = prh_impl_atom_arrque_next_pos(q, head);
    prh_atom_int_dec(&q->len);
    return a;
}

bool prh_impl_atom_arrque_pop_all_begin(void *arrque, void *out) {
    prh_impl_atom_arrque *q = arrque; // push线程只操作q->head 永远不读写q->tail
    prh_impl_atom_arrque_data *data = out;
    prh_int len = prh_impl_atom_arrque_len(q);
    if (!len) return false;
    prh_int head = q->head;
    data->elem = prh_impl_atom_arrque_elem(q);
    data->size = q->size;
    data->head = head;
    data->len = len;
    assert(q->head == head); // 只允许一个消费者
    q->head = ((head + len) & (q->size - 1));
    return true;
}

void prh_impl_atom_arrque_pop_all_end(void *arrque, void *data) {
    prh_impl_atom_arrque *q = arrque;
    prh_atom_int_sub(&q->len, ((prh_impl_atom_arrque_data *)data)->len);
}

#ifdef PRH_TEST_IMPLEMENTATION
void prh_impl_atomic_test(void) {
    atomic_flag f; atomic_bool b; atomic_int i; atomic_uint u;
    atomic_intptr_t ip; atomic_uintptr_t up; atomic_size_t sz; atomic_ptrdiff_t pd;
    atomic_char ch; atomic_schar sc; atomic_uchar uc; atomic_short sh;
    atomic_ushort uh; atomic_long sl; atomic_ulong ul; atomic_llong ll; atomic_ullong ull;
    printf("\n\n[atomic]\n");
    printf("atomic_flag size %d align %d lock free %d\n", (int)sizeof(atomic_flag), (int)prh_alignof(atomic_flag), atomic_is_lock_free(&f));
    printf("atomic_bool size %d align %d lock free %d\n", (int)sizeof(atomic_bool), (int)prh_alignof(atomic_bool), atomic_is_lock_free(&b));
    printf("atomic_int size %d align %d lock free %d\n", (int)sizeof(atomic_int), (int)prh_alignof(atomic_int), atomic_is_lock_free(&i));
    printf("atomic_uint size %d align %d lock free %d\n", (int)sizeof(atomic_uint), (int)prh_alignof(atomic_uint), atomic_is_lock_free(&u));
    printf("atomic_intptr_t size %d align %d lock free %d\n", (int)sizeof(atomic_intptr_t), (int)prh_alignof(atomic_intptr_t), atomic_is_lock_free(&ip));
    printf("atomic_uintptr_t size %d align %d lock free %d\n", (int)sizeof(atomic_uintptr_t), (int)prh_alignof(atomic_uintptr_t), atomic_is_lock_free(&up));
    printf("atomic_size_t size %d align %d lock free %d\n", (int)sizeof(atomic_size_t), (int)prh_alignof(atomic_size_t), atomic_is_lock_free(&sz));
    printf("atomic_ptrdiff_t size %d align %d lock free %d\n", (int)sizeof(atomic_ptrdiff_t), (int)prh_alignof(atomic_ptrdiff_t), atomic_is_lock_free(&pd));
    printf("atomic_char size %d align %d lock free %d\n", (int)sizeof(atomic_char), (int)prh_alignof(atomic_char), atomic_is_lock_free(&ch));
    printf("atomic_schar size %d align %d lock free %d\n", (int)sizeof(atomic_schar), (int)prh_alignof(atomic_schar), atomic_is_lock_free(&sc));
    printf("atomic_uchar size %d align %d lock free %d\n", (int)sizeof(atomic_uchar), (int)prh_alignof(atomic_uchar), atomic_is_lock_free(&uc));
    printf("atomic_short size %d align %d lock free %d\n", (int)sizeof(atomic_short), (int)prh_alignof(atomic_short), atomic_is_lock_free(&sh));
    printf("atomic_ushort size %d align %d lock free %d\n", (int)sizeof(atomic_ushort), (int)prh_alignof(atomic_ushort), atomic_is_lock_free(&uh));
    printf("atomic_long size %d align %d lock free %d\n", (int)sizeof(atomic_long), (int)prh_alignof(atomic_long), atomic_is_lock_free(&sl));
    printf("atomic_ulong size %d align %d lock free %d\n", (int)sizeof(atomic_ulong), (int)prh_alignof(atomic_ulong), atomic_is_lock_free(&ul));
    printf("atomic_llong size %d align %d lock free %d\n", (int)sizeof(atomic_llong), (int)prh_alignof(atomic_llong), atomic_is_lock_free(&ll));
    printf("atomic_ullong size %d align %d lock free %d\n", (int)sizeof(atomic_ullong), (int)prh_alignof(atomic_ullong), atomic_is_lock_free(&ull));
}
#endif // PRH_TEST_IMPLEMENTATION
#endif // PRH_ATOMIC_IMPLEMENTATION
#endif // PRH_ATOMIC_INCLUDE

#ifdef PRH_TIME_INCLUDE
// 1-sec 秒 second
// 1000-msec 毫秒 millisecond
// 1000000-usec 微妙 microsecond
// 1000000000-nsec 纳秒 nanosecond 最大值10亿可以用int32表示，int32最大值为20亿
// 1GHZ 1000MHZ 1000000KHZ 1000000000HZ 相当于 1-nsec per tick
// 一天24小时*3600秒，共86400秒*365天，一年共31536000秒
//
// int64 保持秒可以表示正负2.9千亿年        int32 保存秒可以表示68年
//       如果保存毫秒可以表示2.9亿年              保存毫秒可以表示24天
//       如果保存微妙可以表示29万年               保存微秒可以表示35分钟
//       如果保存纳秒可以表示292年                保存纳秒可以表示2秒
//
// The epoch of system_clock is unspecified, but most implementations use Unix
// Time (i.e., time since 00:00:00 Coordinated Universal Time (UTC), Thursday,
// 1 January 1970, not counting leap seconds).
//
// UTC时间从1970/1/1 00:00:00开始，uint32 只能表示 136 年大约在 2106 年失效。在32位
// Linux系统上，time_t是一个32位有符号整数，可以表示的日期范围从 1901/12/13 20:45:52
// 至 2038/1/19 03:14:07。

// Epoch delta from 0000/1/1 to 1970/1/1
#define PRH_EPOCH_DELTA_SECS 0x0000000E79844E00LL // 62168256000-sec
#define PRH_EPOCH_DELTA_MSEC 0x0000388AACD0B000LL // 62168256000000-msec
#define PRH_EPOCH_DELTA_USEC 0x00dcddb30f2f8000LL // 62168256000000000-usec

#define PRH_MSEC_PER_SEC 1000
#define PRH_USEC_PER_SEC 1000000
#define PRH_NSEC_PER_SEC 1000000000

typedef prh_i64 prh_timesec; // 最大可以表示正负2.9千亿年

typedef struct {
    prh_timesec sec;
    prh_i32 usec;
} prh_timeusec;

typedef struct {
    prh_timesec sec;
    prh_i32 nsec;
} prh_timensec;

typedef struct {
    prh_i32 year;   // 正负20亿年
    prh_i32 usec;   // 0 ~ 999999
    prh_byte month; // 1 ~ 12
    prh_byte mday;  // 1 ~ 31
    prh_byte wday;  // 0 ~ 6 (sunday = 0)
    prh_byte hour;  // 0 ~ 23
    prh_byte min;   // 0 ~ 59
    prh_byte sec;   // 0 ~ 60 since C99
} prh_datetime;

#define prh_abs_sec_to_utc(abs) ((abs) - PRH_EPOCH_DELTA_SECS)
#define prh_utc_sec_to_abs(utc) ((utc) + PRH_EPOCH_DELTA_SECS)

void prh_time_init(void);
prh_i64 prh_system_secs(void);
prh_i64 prh_system_msec(void);
prh_i64 prh_system_usec(void);
prh_i64 prh_steady_secs(void);
prh_i64 prh_steady_msec(void);
prh_i64 prh_steady_usec(void);
prh_i64 prh_steady_nsec(void);
prh_i64 prh_clock_ticks(void); // 如果精度为纳秒（精度为1000000000每秒，1GHZ）可以表示292年
prh_i64 prh_elapse_secs(prh_i64 ticks);
prh_i64 prh_elapse_msec(prh_i64 ticks);
prh_i64 prh_elapse_usec(prh_i64 ticks);
prh_i64 prh_elapse_nsec(prh_i64 ticks);
prh_i64 prh_thread_time(void); // 当前线程执行时间，包含用户以及内核的执行时间

void prh_system_time(prh_timeusec *t);
void prh_system_date(prh_datetime *utc_date);
void prh_system_date_from(prh_datetime *utc_date, const prh_timeusec *utc_time);
void prh_system_time_from_date(prh_timeusec *utc_time, const prh_datetime *utc_date);
void prh_system_time_from_local_date(prh_timeusec *utc_time, const prh_datetime *local_date);
void prh_local_date(prh_datetime *local_date);
void prh_local_date_from(prh_datetime *local_date, const prh_timeusec *utc_time);

prh_inline prh_i64 prh_system_msec_from(const prh_timeusec *p) {
    return p->sec * PRH_MSEC_PER_SEC + p->usec / PRH_MSEC_PER_SEC;
}

prh_inline prh_i64 prh_system_usec_from(const prh_timeusec *p) {
    return p->sec * PRH_USEC_PER_SEC + p->usec;
}

// https://github.com/adobe/chromium/blob/master/base/time_mac.cc
// https://github.com/adobe/chromium/blob/master/base/time_posix.cc
// https://github.com/adobe/chromium/blob/master/base/time_win.cc
#ifdef PRH_TIME_IMPLEMENTATION
typedef struct {
    prh_i64 ticks_per_sec;
} prh_impl_timeinit;

prh_impl_timeinit PRH_IMPL_TIMEINIT;

// https://github.com/rust-lang/rust/blob/3809bbf47c8557bd149b3e52ceb47434ca8378d5/src/libstd/sys_common/mod.rs#L124
// Computes (value*numer)/denom without overflow, as long as both
// (numer*denom) and the overall result fit into i64 (which is the case
// for our time conversions). 分子（numerator）分母（denominator）
// Decompose value as (value/denom*denom + value%denom):
//      v * n / d = (v / d * d + v % d) * n / d =
//      (v / d * d * n / d) + (v % d * n / d) =
//      v / d * n + v % d * n / d = q * n + r * n / d
prh_i64 prh_impl_mul_div(prh_i64 value, prh_i64 numer, prh_i64 denom) {
    prh_i64 q = value / denom;
    prh_i64 r = value % denom;
    return q * numer + r * numer / denom;
}

#if defined(prh_plat_windows)
// https://learn.microsoft.com/en-us/windows/win32/winmsg/about-timers
//
// High-resolution timer 性能计数器的值是一个单调递增的计数值，其频率可以通过
// QueryPerformanceFrequency 获取。
//
// A counter is a general term used in programming to refer to an incrementing
// variable. Some systems include a high-resolution performance counter that
// provides high-resolution elapsed times.
//
// If a high-resolution performance counter exists on the system, you can use
// the QueryPerformanceFrequency function to express the frequency, in counts
// per second. The value of the count is processor dependent. On some
// processors, for example, the count might be the cycle rate of the processor
// clock.
//
// The QueryPerformanceCounter function retrieves the current value of the
// high-resolution performance counter. By calling this function at the
// beginning and end of a section of code, an application essentially uses
// the counter as a high-resolution timer.
//
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
//
// Acquiring high-resolution time stamps
//
// Windows 提供了一些可以用来获取高分辨率时间戳或测量时间间隔的 API。对于本地代码，
// 主要的 API 是 QueryPerformanceCounter（QPC）。对于设备驱动程序，内核模式的 API
// 是 KeQueryPerformanceCounter。对于托管代码，System.Diagnostics.Stopwatch 类
// 使用 QPC 作为其精确时间基础。QPC 是独立的，并且不与任何外部时间参考同步。如果要获
// 取可以与外部时间参考（例如协调世界时，UTC）同步的时间戳（用于高分辨率的时间测量），
// 请使用 GetSystemTimePreciseAsFileTime。时间戳和时间间隔测量是计算机和网络性能测
// 量的重要组成部分。这些性能测量操作包括计算响应时间、吞吐量和延迟，以及对代码执行进
// 行剖析。所有这些操作都涉及对在时间间隔内发生的活动的测量，该时间间隔由开始和结束事
// 件定义，这些事件可以独立于任何外部时间参考。QPC 通常是用于对同一系统或虚拟机上发生
// 的事件进行时间戳标记和测量小时间间隔的最佳方法。如果要在多台机器之间对事件进行时间
// 戳标记，并且每台机器都参与了某种时间同步方案（例如网络时间协议，NTP），请考虑使用
// GetSystemTimePreciseAsFileTime。QPC 帮助你避免使用其他时间测量方法（例如直接读
// 取处理器的时间戳计数器，TSC）时可能遇到的困难。
//
// QPC is available on Windows XP and Windows 2000 and works well on most
// systems. However, some hardware systems' BIOS didn't indicate the hardware
// CPU characteristics correctly (a non-invariant TSC), and some multi-core
// or multi-processor systems used processors with TSCs that couldn't be
// synchronized across cores. Systems with flawed firmware that run these
// versions of Windows might not provide the same QPC reading on different
// cores if they used the TSC as the basis for QPC.
//
// All computers that shipped with Windows Vista and Windows Server 2008 used
// a platform counter (High Precision Event Timer (HPET)) or the ACPI Power
// Management Timer (PM timer) as the basis for QPC. Such platform timers
// have higher access latency than the TSC and are shared between multiple
// processors. This limits scalability of QPC if it is called concurrently
// from multiple processors.
//
// The majority of Windows 7 and Windows Server 2008 R2 computers have
// processors with constant-rate TSCs and use these counters as the basis
// for QPC. TSCs are high-resolution per-processor hardware counters that
// can be accessed with very low latency and overhead (in the order of 10s
// or 100s of machine cycles, depending on the processor type). Windows 7
// and Windows Server 2008 R2 use TSCs as the basis of QPC on single-clock
// domain systems where the operating system (or the hypervisor) is able to
// tightly synchronize the individual TSCs across all processors during
// system initialization. On such systems, the cost of reading the
// performance counter is significantly lower compared to systems that use
// a platform counter. Furthermore, there is no added overhead for concurrent
// calls and user-mode queries often bypass system calls, which further
// reduces overhead. On systems where the TSC is not suitable for timekeeping,
// Windows automatically selects a platform counter (either the HPET timer or
// the ACPI PM timer) as the basis for QPC.
//
// Windows 8, Windows 8.1, Windows Server 2012, and Windows Server 2012 R2
// use TSCs as the basis for the performance counter. The TSC synchronization
// algorithm was significantly improved to better accommodate large systems
// with many processors. In addition, support for the new precise time-of-day
// API was added, which enables acquiring precise wall clock time stamps from
// the operating system. For more info, see GetSystemTimePreciseAsFileTime.
// On Windows RT and Windows 11 and Windows 10 devices using Arm processors,
// the performance counter is based on either a proprietary platform counter
// or the system counter provided by the Arm Generic Timer if the platform
// is so equipped.
//
// Windows 一直在并将继续投资于提供可靠且高效的性能计数器。当你需要具有 1 微秒或更高
// 分辨率的时间戳，并且不需要这些时间戳与外部时间参考同步时，应选择
// QueryPerformanceCounter、KeQueryPerformanceCounter 或
// KeQueryInterruptTimePrecise。当你需要具有 1 微秒或更高分辨率且与协调世界时
// （UTC）同步的时间戳时，应选择 GetSystemTimePreciseAsFileTime 或
// KeQuerySystemTimePrecise。
//
// 在少数无法使用 TSC（时间戳计数器）寄存器来实现 QPC 的平台上，例如由于硬件时钟信息
// （Hardware timer info）中解释的原因，获取高分辨率时间戳的代价可能显著高于获取低分
// 辨率时间戳。如果 10 到 16 毫秒的分辨率已经足够，你可以使用 GetTickCount64、
// QueryInterruptTime、QueryUnbiasedInterruptTime、KeQueryInterruptTime 或
// KeQueryUnbiasedInterruptTime 来获取不与外部时间参考同步的时间戳。对于需要与 UTC
// 同步的时间戳，可以使用 GetSystemTimeAsFileTime 或 KeQuerySystemTime。如果需要
// 更高分辨率，可以使用 QueryInterruptTimePrecise、
// QueryUnbiasedInterruptTimePrecise 或 KeQueryInterruptTimePrecise 来获取时间
// 戳。
//
// 一般来说，在多核和多处理器系统中，性能计数器的结果在所有处理器上是一致的，即使在不
// 同线程或进程中测量也是如此。但存在以下一些例外情况：
//
//      在某些处理器上运行 Windows Vista 之前的操作系统可能由于以下原因之一而违反这
//      种一致性：硬件处理器具有非不变的 TSC（时间戳计数器），并且 BIOS 没有正确指示
//      这种情况；使用的 TSC 同步算法不适合拥有大量处理器的系统。
//
//      当你在多线程环境中从不同线程获取性能计数器的时间戳时，可能会遇到一个问题：由
//      于硬件和调度的不确定性，两个时间戳之间的差异可能只有 ±1 个时钟周期。这种情况
//      下，时间戳的顺序可能是模糊的（ambiguous）。换句话说，你无法确定哪个时间戳真
//      正地早于或晚于另一个时间戳。例如如果 T2 - T1 = ±1 个时钟周期，那么你无法确
//      定 T1 是否早于 T2，或者它们是否几乎同时发生。
//
//      如果两个时间戳是从同一个线程获取的，那么这种 ±1 个时钟周期的不确定性不适用，
//      在同一线程中，时间戳的顺序是明确的，因为它们是在同一个上下文中生成的，不会受
//      到线程切换或硬件同步问题的影响。在此上下文中，术语 “时钟周期” 指的是等于 1 ÷
//      （通过 QueryPerformanceFrequency 获取的性能计数器频率）的时间间隔。
//
// 在具有多个时钟域且这些时钟域在硬件中未同步的大型服务器系统上使用性能计数器时，
// Windows 会确定 TSC 不能用于计时目的，并选择一个平台计数器作为 QPC 的基础。尽管在
// 这种情况下仍然可以提供可靠的时间戳，但访问延迟和可扩展性会受到不利影响。因此，如前
// 面的使用指导所述，只有在需要 1 微秒或更高分辨率时，才使用提供这种分辨率的 API。在
// 包含硬件同步（对所有处理器时钟域）的多时钟域系统上，TSC 被用作 QPC 的基础，因为这
// 实际上它们等效于作为一个单一时钟域系统来运行。
//
// 性能计数器预计可以在正确实现的虚拟化管理程序上运行的所有客户虚拟机中可靠工作。然而，
// 符合虚拟化管理程序版本 1.0 接口并提供参考时间增强功能的虚拟化管理程序可以显著降低
// 开销。有关虚拟化管理程序接口和增强功能的更多信息，请参阅 虚拟化管理程序规范。
//
// 我们强烈不推荐使用 RDTSC 或 RDTSCP 处理器指令直接查询时间戳计数器（TSC），因为在
// 某些版本的 Windows 上、虚拟机的实时迁移过程中，以及在没有不变或紧密同步的 TSC 的
// 硬件系统上，你无法获得可靠的结果。相反，我们建议你使用 QPC，以利用其提供的抽象性、
// 一致性和可移植性。
//
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps#hardware-timer-info
//
// Hardware timer info
//
// TSC 寄存器（x86 和 x64）
//
// 所有现代的 Intel 和 AMD 处理器都包含一个 TSC（时间戳计数器）寄存器，这是一个 64
// 位的寄存器，以很高的速率递增，通常等于处理器的时钟频率。通过 RDTSC 或 RDTSCP 指
// 令可以读取该计数器的值，访问时间非常短，计算成本也很低，通常在几十到几百个机器周
// 期内，具体取决于处理器。尽管 TSC 寄存器看起来像是一个理想的时间戳机制，但在以下情
// 况下，它无法可靠地用于计时目的：
//
// 可移植性问题：并非所有处理器都有可用的 TSC 寄存器，因此直接在软件中使用 TSC 寄存
// 器会带来可移植性问题。Windows 会为 QPC 选择替代的时间源，从而避免了可移植性问题。
// 非不变 TSC：某些处理器可以改变 TSC 时钟的频率，或者停止 TSC 寄存器的递增，这使得
// 这些处理器上的 TSC 不适合用于计时。这些处理器被认为具有非不变 TSC 寄存器。Windows
// 会自动检测这种情况，并为 QPC 选择替代的时间源。
// 虚拟机实时迁移问题：即使虚拟化主机有一个可用的 TSC，当运行中的虚拟机进行实时迁移，
// 而目标虚拟化主机没有或不使用硬件辅助的 TSC 缩放时，可能会导致虚拟机看到 TSC 频率
// 的变化。如果这种类型的实时迁移对虚拟机是可能的，那么虚拟化管理程序应该清除 CPUID
// 中的不变 TSC 特性位。
// 多处理器或多核系统中的时钟同步问题：某些处理器和系统无法将每个核心上的时钟同步到相
// 同的值。Windows 会自动检测这种情况，并为 QPC 选择替代的时间源。
// 大型多处理器系统中的时钟同步问题：即使处理器具有不变 TSC，也可能无法将处理器时钟同
// 步到相同的值。Windows 会自动检测这种情况，并为 QPC 选择替代的时间源。
// 指令乱序执行问题：某些处理器会乱序执行指令。这可能导致在使用 RDTSC 对指令序列进行
// 计时时出现错误的周期计数，因为 RDTSC 指令可能与你在程序中指定的不同时间执行。为了
// 解决这个问题，某些处理器引入了 RDTSCP 指令。
//
// 与其他计时器一样，TSC 基于一个晶体振荡器，其确切频率事先并不知道，并且存在频率偏移
// 误差。因此，在使用之前，必须使用另一个时间参考对其进行校准。在系统初始化期间，
// Windows 会检查 TSC 是否适合用于计时目的，并执行必要的频率校准和核心同步。
//
// PM 时钟（x86 和 x64）
//
// ACPI 定时器（也称为 PM 时钟）被添加到系统架构中，以提供与处理器速度无关的可靠时间
// 戳。由于这是该定时器的唯一目标，它可以在一个时钟周期内提供时间戳，但它不提供任何其
// 他功能。
//
// HPET 定时器（x86 和 x64）
//
// 高精度事件定时器（HPET）是由 Intel 和 Microsoft 联合开发的，以满足多媒体和其他对
// 时间敏感的应用程序的计时需求。与 TSC（每个处理器都有一个）不同，HPET 是一个共享
// 的、平台范围内的资源，尽管一个系统可以有多个 HPET。自 Windows Vista 以来，
// Windows 一直支持 HPET，Windows 7 和 Windows 8 硬件徽标认证要求硬件平台支持
// HPET。
//
// 通用定时器系统计数器（Arm）
//
// 基于 Arm 的平台没有 Intel 或 AMD 平台上那样的 TSC、HPET 或 PM 时钟。相反，Arm 处
// 理器提供了通用定时器（有时也称为通用间隔定时器，或 GIT），其中包含一个系统计数器寄
// 存器（例如，CNTVCT_EL0）。通用定时器系统计数器是一个固定频率的平台范围内的时钟源。
// 它在启动时从零开始，并以很高的速率递增。在 Armv8.6 或更高版本中，这被定义为正好 1
// GHz，但应通过读取由早期引导固件设置的时钟频率寄存器来确定。有关更多详细信息，请参
// 阅《Arm 架构参考手册（A-profile 架构）》（DDI 0487）中的 “AArch64 状态中的通用
// 定时器” 一章。
//
// 循环计数器（Arm）
//
// 基于 Arm 的平台提供了一个性能监控循环计数器（Performance Monitors Cycle
// Counter）寄存器（例如，PMCCNTR_EL0）。这个计数器计算处理器时钟周期。它是非不变
// 的，其单位可能与实际时间无关。不建议使用该寄存器来获取时间戳。
//
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps#general-faq-about-qpc-and-tsc
//
// General FAQ about QPC and TSC
//
// 以下是关于 QPC 和时间戳计数器 TSC 的常见问题解答：
//
// 问：QueryPerformanceCounter 是否与 Win32 的 GetTickCount 或 GetTickCount64 函
// 数相同？
// 答：不是。GetTickCount 和 GetTickCount64 与 QPC 没有关系。GetTickCount 和
// GetTickCount64 返回自系统启动以来的毫秒数。
//
// 问：我应该使用 QPC，还是直接调用 RDTSC/RDTSCP 指令？
// 答：为了避免错误和可移植性问题，我们强烈建议你使用 QPC，而不是直接使用 TSC 寄存器
// 或 RDTSC 或 RDTSCP 处理器指令。
//
// 问：QPC 与外部时间纪元（epoch）有何关系？它可以与外部纪元（如 UTC）同步吗？
// 答：QPC 基于一个硬件计数器，该计数器无法与外部时间参考（如 UTC）同步。如果需要可以
// 与外部 UTC 参考同步的精确时间戳，请使用 GetSystemTimePreciseAsFileTime。
//
// 问：QPC 是否受夏令时、闰秒、时区或管理员更改系统时间的影响？
// 答：不会。QPC 完全独立于系统时间和 UTC。
//
// 问：QPC 的准确性是否受电源管理或 Turbo Boost 技术导致的处理器频率变化的影响？
// 答：不会。如果处理器具有不变 TSC，则 QPC 不受这些变化的影响。如果处理器没有不变
// TSC，则 QPC 将改用不受处理器频率变化或 Turbo Boost 技术影响的平台硬件计时器。
//
// 问：QPC 是否能在多处理器系统、多核系统以及具有超线程技术的系统上可靠工作？
// 答：是的。
//
// 问：我如何确定并验证 QPC 在我的机器上是否工作正常？
// 答：你无需进行此类检查。
//
// 问：哪些处理器具有非不变 TSC？我如何检查我的系统是否具有非不变 TSC？
// 答：你无需自行进行此检查。Windows 操作系统在系统初始化时会进行多项检查，以确定
// TSC 是否适合作为 QPC 的基础。不过，出于参考目的，你可以通过以下方式之一确定你的
// 处理器是否具有不变 TSC：
// - 使用 Windows Sysinternals 的 Coreinfo.exe 工具
// - 检查与 TSC 特性相关的 CPUID 指令返回的值
// - 查阅处理器制造商的文档
//
// 以下是 Windows Sysinternals Coreinfo.exe 工具（www.sysinternals.com）提供的
// TSC-INVARIANT 信息示例（带星号表示“真”）：
//
// > Coreinfo.exe
// RDTSCP          * Supports RDTSCP instruction
// TSC             * Supports RDTSC instruction
// TSC-DEADLINE    - Local APIC supports one-shot deadline timer
// TSC-INVARIANT   * TSC runs at constant rate
//
// 问：QPC 是否能在 Windows RT 硬件平台上可靠工作？
// 答：是的。
//
// 问：QPC 的回绕频率是多少？
// 答：从最近一次系统启动起，至少 100 年内不会回绕，具体取决于所用的基础硬件计时器。
// 对于大多数应用程序来说，回绕并不是一个需要担心的问题。
//
// 问：调用 QPC 的计算成本是多少？
// 答：调用 QPC 的计算成本主要取决于基础硬件平台。如果以 TSC 寄存器作为 QPC 的基础，
// 则计算成本主要取决于处理器处理 RDTSC 指令所需的时间。根据所用的处理器，这个时间范
// 围从几十个 CPU 周期到几百个 CPU 周期不等。如果无法使用 TSC，则系统会选择其他硬件
// 时间基础。由于这些时间基础位于主板上（例如，在 PCI 南桥或 PCH 上），每次调用的计
// 算成本会高于 TSC，通常根据处理器速度和其他硬件因素，接近 0.8 - 1.0 微秒。这个成
// 本主要由访问主板上的硬件设备所需的时间决定。
//
// 问：调用 QPC 是否需要进行内核转换（系统调用）？
// 答：如果系统可以使用 TSC 寄存器作为 QPC 的基础，则不需要内核转换。如果系统必须使
// 用其他时间基础（如 HPET 或 PM 时钟），则需要系统调用。
//
// 问：性能计数器是否单调（非递减）？
// 答：是的。QPC 不会倒退。
//
// 问：性能计数器是否可用于对事件按时间排序？
// 答：可以。但是，当比较从不同线程获取的性能计数器结果时，相差 ±1 个时钟周期的值具有
// 模糊的顺序，就好像它们具有相同的时间戳一样。
//
// 问：性能计数器的准确性如何？
// 答：这取决于多种因素。更多信息，请参阅低级硬件时钟特性（Low-level hardware clock
// characteristics）。
//
// 问：我需要将 QPC 输出转换为毫秒。如何避免在转换为双精度或单精度浮点数时丢失精度？
// 答：在对整数性能计数器进行计算时，需要注意以下几点：
// - 整数除法会丢失余数。在某些情况下，这可能会导致精度丢失。
// - 在 64 位整数和浮点数（双精度）之间进行转换可能会导致精度丢失，因为浮点数的尾数部
//   分无法表示所有可能的整数值。
// - 64 位整数的乘法可能会导致整数溢出。
// - 一般来说，尽可能延迟这些计算和转换，以避免累积引入的误差。
//
// 问：我如何将 QPC 转换为 100 纳秒的时间间隔，以便将其添加到 FILETIME 中？
// 答：文件时间是一个 64 位值，表示自 1601 年 1 月 1 日 12:00 A.M.（UTC）以来经过
// 的 100 纳秒间隔数。文件时间用于返回时间戳的 Win32 API 调用，例如
// GetSystemTimeAsFileTime 和 GetSystemTimePreciseAsFileTime。相比之下，
// QueryPerformanceCounter 返回的值表示以 QueryPerformanceFrequency 获取的性能计
// 数器频率为单位的时间。在两者之间进行转换需要计算 QPC 时间间隔和 100 纳秒时间间隔
// 的比率。注意避免因值较小（例如 0.0000001 / 0.000000340）而丢失精度。
//
// 问：为什么 QPC 返回的时间戳是有符号整数？
// 答：涉及 QPC 时间戳的计算可能涉及减法。通过使用有符号值，可以处理可能得出负值的计
// 算。
//
// 问：我如何从托管代码中获取高分辨率时间戳？
// 答：调用 System.Diagnostics.Stopwatch 类中的 Stopwatch.GetTimeStamp 方法。有
// 关如何使用 Stopwatch.GetTimeStamp 的示例，请参阅从托管代码中获取高分辨率时间戳。
//
// 问：在什么情况下，QueryPerformanceFrequency 返回 FALSE，或者
// QueryPerformanceCounter 返回零？
// 答：在运行 Windows XP 或更高版本的任何系统上，这种情况都不会发生。
//
// 问：我是否需要将线程亲和性设置为单个核心才能使用 QPC？
// 答：不需要。这种场景既不必要也不理想。执行此操作可能会通过将处理限制在一个核心上，
// 或者在多个线程将亲和性设置为同一个核心时调用 QueryPerformanceCounter，从而在单个
// 核心上创建瓶颈，从而对应用程序的性能产生不利影响。This scenario is neither
// necessary nor desirable. Performing this scenario might adversely affect
// your application's performance by restricting processing to one core or
// by creating a bottleneck on a single core if multiple threads set their
// affinity to the same core when calling QueryPerformanceCounter.
//
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps#low-level-hardware-clock-characteristics
//
// 底层硬件时钟特性（Low-level hardware clock characteristics）
//
// Absolute Clocks and Difference Clocks
//
// 绝对时钟提供准确的时间读数。它们通常基于协调世界时（UTC），因此其准确性部分取决于它
// 们与外部时间参考的同步程度。例如，通过网络时间协议（NTP）或全球定位系统（GPS）来同
// 步。差分时钟用于测量时间间隔，通常不基于外部时间纪元。QPC 是一个差分时钟，不与外部
// 时间纪元或参考同步。当你使用 QPC 进行时间间隔测量时，通常会比使用基于绝对时钟的时间
// 戳获得更高的精度。这是因为同步绝对时钟的时间可能会引入相位和频率变化，从而增加短期时
// 间间隔测量的不确定性。当绝对时钟与外部时间源同步时，可能会出现时间的跳跃（相位变
// 化）。例如，如果系统时钟与 NTP 服务器同步，可能会出现几毫秒甚至更长时间的调整。为了
// 使系统时钟与外部时间源保持同步，系统可能会调整时钟频率。这种频率调整会影响短期时间间
// 隔的测量精度。
//
// Resolution, Precision, Accuracy, and Stability
//
// QPC 以硬件计数器为基础。硬件计时器由三部分组成：一个时钟脉冲生成器、一个用于计数
// 时钟脉冲的计数器，以及一种取回计数器值的方法。这三部分的特性决定了 QPC 的分辨率、
// 精度、准确性和稳定性。
//
// 如果硬件生成器以恒定速率提供时钟脉冲，那么可以通过简单地计数这些时钟脉冲来测量时间
// 间隔。时钟脉冲生成的速率称为频率，以赫兹（Hz）表示。频率的倒数称为周期或时钟脉冲间
// 隔，以适当的国际单位制（SI）时间单位表示（例如，秒、毫秒、微秒或纳秒）。计时器的分
// 辨率等于周期。分辨率决定了区分任意两个时间戳的能力，并为可测量的最小时间间隔设定了
// 下限。这有时被称为时钟脉冲分辨率。
//
// 由于数字计数器以离散步骤前进，而时间是连续流逝的，因此数字时间测量引入了 ±1 个时钟
// 脉冲的测量不确定性。这种不确定性被称为量化误差。对于典型的时间间隔测量，这种效应通
// 常可以忽略不计，因为量化误差远小于正在测量的时间间隔。然而，如果被测量的周期很小，
// 并且接近计时器的分辨率，那么你需要考虑这种量化误差。引入的误差大小为一个时钟周期。
// 以下两个图表通过使用分辨率为 1 个时间单位的计时器，说明了 ±1 个时钟脉冲不确定性的
// 影响。
//          两个时间戳的读数都是0，因为分辨率的原因它们不可分辨
//          |      |
//          v      v
//          |-------|-------|-------|-------|
//          0       1       2       3       4
//
//          两个时间戳的间隔为1个时间单位，但实际的间隔比这个小很多
//                         | |
//                         v v
//          |-------|-------|-------|-------|
//          0       1       2       3       4
//
// QueryPerformanceFrequency 返回 QPC 的频率，周期和分辨率等于该值的倒数。
// QueryPerformanceFrequency 返回的性能计数器频率是在系统初始化期间确定的，并且在系
// 统运行期间不会改变。通常，QueryPerformanceFrequency 并不返回硬件时钟脉冲生成器的
// 实际频率。例如，在某些旧版本的 Windows 中，QueryPerformanceFrequency 返回 TSC
// 频率值除以 1024；而在运行支持虚拟化程序版本 1.0 接口（或在某些较新版本的 Windows
// 中始终如此）的虚拟化程序时，性能计数器频率被固定为 10 MHz（100nsec）。因此，不要
// 假设 QueryPerformanceFrequency 会返回基于硬件频率的值。QueryPerformanceCounter
// 读取性能计数器，并返回自 Windows 操作系统启动以来发生的总时钟脉冲数，包括机器处于
// 待机、休眠或连接待机等睡眠状态的时间。
//
// 从软件中访问（读取）时钟脉冲计数器需要时间，这种访问时间可能会降低时间测量的精度。
// 这是因为最小可测量时间间隔（能够测量的最小时间间隔）是分辨率和访问时间中较大的那个：
// 精度 = MAX [分辨率，访问时间]。例如，假设有一个假设性的硬件计时器，其分辨率为 100
// 纳秒，访问时间为 800 纳秒。如果使用平台计时器而不是 TSC 寄存器作为 QPC 的基础，就
// 可能出现这种情况。因此，精度将是 800 纳秒，而不是 100 纳秒。
//
// 如果访问时间大于分辨率，不要试图通过猜测来提高精度。换句话说，假设时间戳正好在函数
// 调用的中间、开始或结束时被获取，这是一种错误。相比之下，考虑以下示例，其中 QPC 的
// 访问时间仅为 20 纳秒，而硬件时钟的分辨率为 100 纳秒。如果使用 TSC 寄存器作为 QPC
// 的基础，就可能出现这种情况。在这里，精度由时钟分辨率限制。在实践中，你可以找到读取
// 计数器所需的时间大于或小于分辨率的时间源。在这两种情况下，精度将是两者中较大的那个。
// 下表提供了各种时钟的近似分辨率、访问时间和精度的信息。请注意，其中一些值会因不同的
// 处理器、硬件平台和处理器速度而有所不同。
//
//      时钟源              正常时钟频率    时钟分辨率  一般访问时间    精度
//      PC RTC                  64Hz        15.625ms    N/A         N/A
//      QPC TSC with 3GHz       3MHz        333ns       30ns        333ns
//      processor clock
//      RDTSC instruction       3GHz        333picosec  30ns        30ns
//      with 3GHz cycle time
//
// 因为 QPC 使用硬件计数器，所以当你了解一些硬件计数器的基本特性时，你就能了解 QPC 的
// 能力和限制。最常用的硬件时钟脉冲生成器是晶体振荡器。晶体是石英或其他陶瓷材料的小片，
// 它具有压电特性，能够提供一个廉价的频率参考，具有极好的稳定性和准确性。这个频率用于
// 生成时钟计数的时钟脉冲。时钟的准确性指的是与真实或标准值的一致程度。这主要取决于晶
// 体振荡器以指定频率提供时钟脉冲的能力。如果振荡频率过高，时钟会“走得快”，测量的时间
// 间隔会显得比实际长；如果频率过低，时钟会“走得慢”，测量的时间间隔会显得比实际短。对
// 于短时间间隔的典型时间间隔测量（例如，响应时间测量、网络延迟测量等），硬件振荡器的
// 准确性通常就足够了。然而，对于某些测量，振荡器频率的准确性变得很重要，特别是对于长
// 时间间隔或当你想比较在不同机器上进行的测量时。本节的其余部分探讨了振荡器准确性的影
// 响。
//
// 晶体的振荡频率是在制造过程中设定的，制造商以“百万分率”（ppm，parts per million）
// 表示最大频率偏移，称为最大频率偏移。一个标称频率为 1,000,000 Hz、最大频率偏移为
// ±10 ppm 的晶体，如果其实际频率在 999,990 Hz 到 1,000,010 Hz 之间，则符合规格。
// 将“百万分率”替换为“每秒微秒数”，我们可以将这种频率偏移误差应用于时间间隔测量。一个
// 偏移为 +10 ppm 的振荡器每秒会有 10 微秒的误差。例如测量 1 秒的时间间隔，可能将 1
// 秒的时间间隔测量为 0.999990 秒。一个方便的参考是，100 ppm 的频率误差会导致 24 小
// 时后产生 8.64 秒的误差。下表列出了由于累积误差导致的更长时间间隔的测量不确定性。
//
//      时间间隔        时钟频率偏移误差±10ppm产生的时间偏移
//      1usec           ±10picosec 每微妙10皮秒误差
//      1msec           ±10nsec 每毫秒10纳秒误差
//      1sec            ±10usec 每秒10微妙误差
//      1min            ±600usec
//      1hour           ±36msec
//      1day            ±0.86sec
//      1week           ±6.08sec
//
// 上表表明，对于短时间间隔，频率偏移误差（frequency offset error）通常可以忽略不计。
// 然而，对于长时间间隔，即使是小的频率偏移也可能导致相当大的测量不确定性。个人计算机
// 和服务器中使用的晶体振荡器通常制造时的频率公差为 ±30 到 50 ppm，很少情况下，晶体的
// 偏差可能高达 500 ppm。尽管可以提供频率偏移公差更小的晶体，但它们更昂贵，因此在大多
// 数计算机中没有使用。为了减少这种频率偏移误差的不利影响，最近版本的 Windows，特别是
// Windows 8，使用多个硬件计时器来检测频率偏移，并尽可能进行补偿。这个校准过程在启动
// Windows 时执行。如以下示例所示，硬件时钟的频率偏移误差影响了可实现的准确性，而时钟
// 的分辨率可能没那么重要。总之，当测量长时间间隔以及在不同系统之间比较测量结果时，频
// 率偏移误差变得越来越重要。时钟的稳定性描述了时钟频率是否会随时间变化，例如由于温度
// 变化的结果。用作计算机时钟脉冲生成器的石英晶体将表现出随温度变化的频率小幅度变化。
// 对于常见的温度范围，由热漂移引起的误差通常比频率偏移误差小得多。然而，为便携式设备
// 或受大温度波动影响的设备设计软件的人员可能需要考虑这种效应。
//
// 示例 1：假设你使用一个 1 MHz 的振荡器进行时间间隔测量，其分辨率为 1 微秒，最大频率
// 偏移误差为 ±50 ppm。现在，假设偏移量正好是 +50 ppm。这意味着实际频率将是
// 1,000,050 Hz。如果我们测量一个 24 小时的时间间隔，我们的测量值将比实际值短 4.3
// 秒（实际 24:00:00.000000，测量值为 23:59:55.700000）。
//      一天中的秒数 = 86,400
//      频率偏移误差 = 50ppm = 0.00005 每秒偏50微妙
//      86,400 秒 × 0.00005 = 4.3 秒
//
// 示例 2：假设处理器的 TSC 时钟由一个晶体振荡器控制，其标称频率为 3 GHz。这意味着分
// 辨率将是 1/3,000,000,000，约为 333 皮秒。假设用于控制处理器时钟的晶体的实际频率误
// 差为 ±50 ppm，并且实际偏移量为 +50 ppm。尽管分辨率令人印象深刻，但测量一个 24 小
// 时的时间间隔仍将比实际值短 4.3 秒（实际 24:00:00.0000000000，测量值为
// 23:59:55.7000000000）。
//      一天中的秒数 = 86,400
//      频率偏移误差 = 50 ppm = 0.00005
//      86,400 秒 × 0.00005 = 4.3 秒
// 这表明，高分辨率的 TSC 时钟不一定比低分辨率的时钟提供更准确的测量结果。
//
// 示例 3：考虑使用两台不同的计算机来测量同一个 24 小时的时间间隔。两台计算机的振荡器
// 的最大频率偏移均为 ±50 ppm。这两台系统对同一时间间隔的测量结果最多能相差多远？与前
// 面的示例一样，±50 ppm 在 24 小时内产生的最大误差为 ±4.3 秒。如果一个系统快了 4.3
// 秒，而另一个系统慢了 4.3 秒，那么 24 小时后的最大误差可能为 8.6 秒。
//      一天中的秒数 = 86,400
//      频率偏移误差 = ±50 ppm = ±0.00005
//      ±(86,400 秒 × 0.00005) = ±4.3 秒
//
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/time-functions
// https://learn.microsoft.com/en-us/windows/win32/multimedia/multimedia-timers
//
// FILETIME structure minwinbase.h (include Windows.h) Windows 2000
// Contains a 64-bit value representing the number of 100-nanosecond intervals
// since January 1, 1601 (UTC).
//      typedef struct _FILETIME {
//          DWORD dwLowDateTime;
//          DWORD dwHighDateTime;
//      } FILETIME;
//
// void GetSystemTimeAsFileTime(FILETIME *out); Windows 2000
// sysinfoapi.h (include Windows.h) Kernel32.lib Kernel32.dll
//      Retrieves the current system date and time. The information is in
//      Coordinated Universal Time (UTC) format.
//
// void GetSystemTimePreciseAsFileTime(FILETIME *out); Windows 8 Windows Server 2012
// sysinfoapi.h (include Windows.h) Kernel32.lib Kernel32.dll
//      The GetSystemTimePreciseAsFileTime function retrieves the current
//      system date and time with the highest possible level of precision
//      (<1us). The retrieved information is in Coordinated Universal Time
//      (UTC) format.
//      Note  This function is best suited for high-resolution time-of-day
//      measurements, or time stamps that are synchronized to UTC. For
//      high-resolution interval measurements, use QueryPerformanceCounter
//      or KeQueryPerformanceCounter. For more info about acquiring
//      high-resolution time stamps, see Acquiring high-resolution timestamps.
//
// ULONGLONG GetTickCount64(); Windows Vista Windows Server 2008
// sysinfoapi.h (include Windows.h) Kernel32.lib Kernel32.dll
//      Retrieves the number of milliseconds that have elapsed since the system
//      was started.
//      The resolution of the GetTickCount64 function is limited to the
//      resolution of the system timer, which is typically in the range of
//      10 milliseconds to 16 milliseconds. The resolution of the
//      GetTickCount64 function is not affected by adjustments made by the
//      GetSystemTimeAdjustment function.
//      If you need a higher resolution timer, use a multimedia timer or a
//      high-resolution timer.
//      To compile an application that uses this function, define _WIN32_WINNT
//      as 0x0600 or later. For more information, see Using the Windows
//      Headers.
//
// LARGE_INTEGER union (winnt.h)
//      The LARGE_INTEGER structure is actually a union. If your compiler has
//      built-in support for 64-bit integers, use the QuadPart member to store
//      the 64-bit integer. Otherwise, use the LowPart and HighPart members to
//      store the 64-bit integer.
//      typedef union _LARGE_INTEGER {
//          struct {
//              DWORD LowPart;
//              LONG HighPart;
//          } DUMMYSTRUCTNAME;
//          struct {
//              DWORD LowPart;
//              LONG HighPart;
//          } u;
//          LONGLONG QuadPart; // a signed 64-bit integer
//      } LARGE_INTEGER;
//
// BOOL QueryPerformanceCounter(LARGE_INTEGER *count); Windows 2000
// profileapi.h (include Windows.h) Kernel32.lib Kernel32.dll
//      If the function fails, the return value is zero. To get extended error
//      information, call GetLastError. On systems that run Windows XP or
//      later, the function will always succeed and will thus never return
//      zero.
//      For more info about this function and its usage, see Acquiring
//      high-resolution time stamps.
//
// BOOL QueryPerformanceFrequency(LARGE_INTEGER *frequency); Windows 2000
// profileapi.h (include Windows.h) Kernel32.lib Kernel32.dll
//      Retrieves the frequency of the performance counter. The frequency of
//      the performance counter is fixed at system boot and is consistent
//      across all processors. Therefore, the frequency need only be queried
//      upon application initialization, and the result can be cached.
//      A pointer to a variable that receives the current performance-counter
//      frequency, in counts per second. If the installed hardware doesn't
//      support a high-resolution performance counter, this parameter can be
//      zero (this will not occur on systems that run Windows XP or later).
//      If the function fails, the return value is zero. To get extended error
//      information, call GetLastError. On systems that run Windows XP or
//      later, the function will always succeed and will thus never return
//      zero.
#define PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_SECS 0x00000002B6109100LL // 11644473600-sec
#define PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_MSEC 0x00000A9730B66800LL // 11644473600000-msec
#define PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_USEC 0x00295E9648864000LL // 11644473600000000-usec
#define PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_100N 0x019DB1DED53E8000LL // 116444736000000000-100nsec

void prh_impl_1970_utc_secs_to_filetime(prh_i64 secs, FILETIME *f) {
    // The time functions included in the C run-time use the time_t type to
    // represent the number of seconds elapsed since midnight, January 1, 1970.
    // The following example converts a time_t value to a FILETIME.
    //      void TimetToFileTime(time_t t, FILETIME *f) {
    //          ULARGE_INTEGER time_value;
    //          time_value.QuadPart = (t * 10000000LL) + 116444736000000000LL;
    //          f->dwLowDateTime = time_value.LowPart;
    //          f->dwHighDateTime = time_value.HighPart;
    //      }
    // FILETIME 单位为 100 纳秒，从 1601/1/1 为基准开始。
    secs = (secs + PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_SEC) * 10000000;
    f->dwLowDateTime = (DWORD)(secs & 0xFFFFFFFF);
    f->dwHighDateTime = (DWORD)(secs >> 32);
}

prh_i64 prh_impl_1970_utc_time_secs(const FILETIME *f) {
    prh_i64 secs = ((prh_i64)f->dwHighDateTime << 32) | f->dwLowDateTime;
    return secs / 10000000 - PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_SEC;
}

prh_i64 prh_impl_1970_utc_time_msec(const FILETIME *f) {
    prh_i64 msec = ((prh_i64)f->dwHighDateTime << 32) | f->dwLowDateTime;
    return msec / 10000 - PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_MSEC;
}

prh_i64 prh_impl_1970_utc_time_usec(const FILETIME *f) {
    prh_i64 usec = ((prh_i64)f->dwHighDateTime << 32) | f->dwLowDateTime;
    return usec / 10 - PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_USEC;
}

prh_i64 prh_impl_1970_utc_time_nsec(const FILETIME *f) {
    prh_i64 nsec = ((prh_i64)f->dwHighDateTime << 32) | f->dwLowDateTime;
    return (nsec - PRH_IMPL_FILETIME_DELTA_FROM_EPOCH_100N) * 100;
}

prh_i64 prh_impl_filetime_nsec(const FILETIME *f) {
    prh_i64 nsec = ((prh_i64)f->dwHighDateTime << 32) | f->dwLowDateTime;
    return nsec * 100;
}

prh_i64 prh_system_secs(void) { // 可表示2.9千亿年
    FILETIME f;
    GetSystemTimeAsFileTime(&f);
    return prh_impl_1970_utc_time_secs(&f);
}

prh_i64 prh_system_msec(void) { // 可表示2.9亿年
    FILETIME f;
    GetSystemTimeAsFileTime(&f);
    return prh_impl_1970_utc_time_msec(&f);
}

void prh_impl_system_filetime(FILETIME *f) {
#if (_WIN32_WINNT >= 0x0602) // Windows 8
    GetSystemTimePreciseAsFileTime(f);
#else
    GetSystemTimeAsFileTime(f);
#endif
}

prh_i64 prh_system_usec(void) { // 可表示29万年
    FILETIME f;
    prh_impl_system_filetime(&f);
    return prh_impl_1970_utc_time_usec(&f);
}

void prh_impl_usec_to_timeusec(prh_timeusec *utc_time, prh_i64 usec) {
    utc_time->sec = usec / PRH_USEC_PER_SEC;
    utc_time->usec = usec % PRH_USEC_PER_SEC;
}

void prh_system_time(prh_timeusec *utc_time) {
    prh_impl_usec_to_timeusec(utc_time, prh_system_usec());
}

void prh_impl_date_time(prh_datetime *t, const SYSTEMTIME *s, prh_i32 usec) {
    // typedef struct _SYSTEMTIME {
    //      WORD wYear;         // the valid value is 1601 ~ 30827
    //      WORD wMonth;        // 1 ~ 12
    //      WORD wDayOfWeek;    // 0 ~ 6 (Sunday = 0)
    //      WORD wDay;          // 1 ~ 31
    //      WORD wHour;         // 0 ~ 23
    //      WORD wMinute;       // 0 ~ 59
    //      WORD wSecond;       // 0 ~ 59
    //      WORD wMilliseconds; // 0 ~ 999
    // } SYSTEMTIME;
    // Windows 2000 Professional Windows 2000 Server
    // minwinbase.h (include Windows.h)
    //      The system can periodically refresh the time by synchronizing with
    //      a time source. Because the system time can be adjusted either
    //      forward or backward, do not compare system time readings to
    //      determine elapsed time.
    //      The SYSTEMTIME does not check to see if the date represented is a
    //      real and valid date. When working with this API, you should ensure
    //      its validity, especially in leap year scenarios. See leap day
    //      readiness for more information.
    //      https://techcommunity.microsoft.com/blog/azuredevcommunityblog/it%e2%80%99s-2020-is-your-code-ready-for-leap-day/1157279
    t->year = s->wYear;
    t->month = (prh_byte)s->wMonth;
    t->mday = (prh_byte)s->wDay;
    t->wday = (prh_byte)s->wDayOfWeek;
    t->hour = (prh_byte)s->wHour;
    t->min = (prh_byte)s->wMinute;
    t->sec = (prh_byte)s->wSecond;
    t->usec = usec;
}

void prh_impl_system_date(SYSTEMTIME *s, const prh_datetime *t) {
    s->wYear = (WORD)t->year;
    s->wMonth = t->month;
    s->wDayOfWeek = t->wday;
    s->wDay = t->mday;
    s->wHour = t->hour;
    s->wMinute = t->min;
    s->wSecond = t->sec;
    s->wMilliseconds = 0;
}

void prh_system_time_from_date(prh_timeusec *utc_time, const prh_datetime *utc_date) {
    SYSTEMTIME s; FILETIME f;
    prh_impl_system_date(&s, utc_date);
    PRH_WINOS_BOOLRET(SystemTimeToFileTime(&s, &f));
    prh_impl_usec_to_timeusec(utc_time, prh_impl_1970_utc_time_usec(&f) + utc_date->usec);
}

void prh_system_time_from_local_date(prh_timeusec *utc_time, const prh_datetime *local_date) {
    SYSTEMTIME local, s; FILETIME f;
    prh_impl_system_date(&local, local_date);
    PRH_WINOS_BOOLRET(TzSpecificLocalTimeToSystemTime(prh_null, &local, &s));
    PRH_WINOS_BOOLRET(SystemTimeToFileTime(&s, &f));
    prh_impl_usec_to_timeusec(utc_time, prh_impl_1970_utc_time_usec(&f) + utc_date->usec);
}

void prh_system_date(prh_datetime *utc_date) {
    FILETIME f; SYSTEMTIME s;
    prh_impl_system_filetime(&f);
    prh_i64 filetime = ((prh_i64)f->dwHighDateTime << 32) | f->dwLowDateTime;
    prh_boolret(FileTimeToSystemTime(&f, &s));
    prh_impl_date_time(utc_date, &s, filetime / 10 % 1000000);
}

void prh_local_date(prh_datetime *local_date) {
    FILETIME f; SYSTEMTIME s, local;
    prh_impl_system_filetime(&f);
    prh_i64 filetime = ((prh_i64)f->dwHighDateTime << 32) | f->dwLowDateTime;
    prh_boolret(FileTimeToSystemTime(&f, &s));
    prh_boolret(SystemTimeToTzSpecificLocalTime(prh_null, &s, &local));
    prh_impl_date_time(local_date, &local, filetime / 10 % 1000000);
}

void prh_local_date_from(prh_datetime *local_date, const prh_timeusec *utc_time) {
    FILETIME f; SYSTEMTIME s, local;
    prh_impl_1970_utc_secs_to_filetime(utc_time->sec, &f);
    PRH_WINOS_BOOLRET(FileTimeToSystemTime(&f, &s));
    PRH_WINOS_BOOLRET(SystemTimeToTzSpecificLocalTime(prh_null, &s, &local));
    prh_impl_date_time(local_date, &local, utc_time->usec);
}

void prh_system_date_from(prh_datetime *utc_date, const prh_timeusec *utc_time) {
    // void GetSystemTime(SYSTEMTIME *SystemTime);
    // void GetLocalTime(SYSTEMTIME *SystemTime);
    // Windows 2000 Professional Windows 2000 Server
    // sysinfoapi.h (include Windows.h) Kernel32.lib Kernel32.dll
    //      GetSystemTime retrieves the current system date and time in
    //      Coordinated Universal Time (UTC) format.
    //      GetLocalTime retrieves the current local date and time.
    //
    // BOOL SystemTimeToTzSpecificLocalTime(
    //          const TIME_ZONE_INFORMATION *TimeZoneInformation, [optional]
    //          const SYSTEMTIME *UniversalTime,
    //          SYSTEMTIME *LocalTime);
    // Windows 2000 Professional Windows 2000 Server
    // timezoneapi.h (include Windows.h) Kernel32.lib Kernel32.dll
    //      Converts a time in Coordinated Universal Time (UTC) to a specified
    //      time zone's corresponding local time.
    //      TimeZoneInformation pointer to a TIME_ZONE_INFORMATION structure
    //      that specifies the time zone of interest. If TimeZoneInformation
    //      is NULL, the function uses the currently active time zone.
    //      The SystemTimeToTzSpecificLocalTime function takes into account
    //      whether daylight saving time (DST) is in effect for the local time
    //      to which the system time is to be converted.
    //      The SystemTimeToTzSpecificLocalTime function may calculate the
    //      local time incorrectly under the following conditions: The time
    //      zone uses a different UTC offset for the old and new years. The
    //      UTC time to be converted and the calculated local time are in
    //      different years.
    //      If the function fails, the return value is zero. To get extended
    //      error information, call GetLastError.
    //
    // BOOL TzSpecificLocalTimeToSystemTime(
    //          const TIME_ZONE_INFORMATION *TimeZoneInformation, [optional]
    //          const SYSTEMTIME *LocalTime,
    //          SYSTEMTIME *UniversalTime);
    // Windows XP Windows Server 2003
    // timezoneapi.h (include Windows.h) Kernel32.lib Kernel32.dll
    //      Converts a local time to a time in Coordinated Universal Time
    //      (UTC). TimeZoneInformation pointer to a TIME_ZONE_INFORMATION
    //      structure that specifies the time zone for the time specified
    //      in LocalTime. If TimeZoneInformation is NULL, the function uses
    //      the currently active time zone.
    //      If the function fails, the return value is zero. To get extended
    //      error information, call GetLastError.
    //      TzSpecificLocalTimeToSystemTime takes into account whether daylight
    //      saving time (DST) is in effect for the local time to be converted.
    //
    // BOOL FileTimeToSystemTime(const FILETIME *FileTime, SYSTEMTIME *out);
    // BOOL SystemTimeToFileTime(const SYSTEMTIME *SystemTime, FILETIME *out);
    // Windows XP Windows Server 2003
    // timezoneapi.h (include Windows.h) Kernel32.lib Kernel32.dll
    //      FileTimeToSystemTime converts a file time to system time format.
    //      System time is based on Coordinated Universal Time (UTC).
    //      FileTime must be less than 0x8000000000000000. Otherwise, the
    //      function fails.
    //      SystemTimeToFileTime converts a system time to file time format.
    //      System time is based on Coordinated Universal Time (UTC).
    //      The wDayOfWeek member of the SYSTEMTIME structure is ignored.
    //      A False return value can indicate that the passed SYSTEMTIME
    //      structure represents an invalid date. Certain situations, such as
    //      the additional day added in a leap year, can result in application
    //      logic unexpectedly creating an invalid date. For more information
    //      on avoiding these issues, see leap year readiness.
    //      If the function fails, the return value is zero. To get extended
    //      error information, call GetLastError.
    FILETIME f; SYSTEMTIME s;
    prh_impl_1970_utc_secs_to_filetime(utc_time->sec, &f);
    PRH_WINOS_BOOLRET(FileTimeToSystemTime(&f, &s));
    prh_impl_date_time(utc_date, &s, utc_time->usec);
}

// Windows time is the number of milliseconds elapsed since the system was
// last started. You typically use the GetTickCount or GetTickCount64
// function to get the current Windows time. GetTickCount and
// GetTickCount64 are limited to the resolution of the system timer, which
// is approximately 10 milliseconds to 16 milliseconds. The elapsed time
// retrieved by GetTickCount or GetTickCount64 includes time the system
// spends in sleep or hibernation.（精度 10msec ~ 16msec）
//
// If you need a higher resolution timer, use the QueryUnbiasedInterruptTime
// function, a multimedia timer, or a high-resolution timer. The elapsed
// time retrieved by the QueryUnbiasedInterruptTime function includes only
// time that the system spends in the working state. The
// QueryUnbiasedInterruptTime function is available starting with Windows 7
// and Windows Server 2008 R2.（精度 0.5msec ~ 15.625msec）
//
// You can use the System Up Time performance counter to obtain the number
// of seconds elapsed since the computer was started. This performance
// counter can be retrieved from the performance data in the registry key
// HKEY_PERFORMANCE_DATA. The value returned is an 8-byte value. Windows
// Performance Counters provide a high-level abstraction layer that
// provides a consistent interface for collecting various kinds of system
// data such as CPU, memory, and disk usage. System administrators often
// use performance counters to monitor systems for performance or behavior
// problems. Software developers often use performance counters to examine
// the resource usage of their programs.
// https://learn.microsoft.com/en-us/windows/win32/perfctrs/performance-counters-portal
//
// https://learn.microsoft.com/en-us/windows/win32/sysinfo/interrupt-time
// Interrupt time is the amount of time since the system was last started,
// in 100-nanosecond intervals. The interrupt-time count begins at zero
// when the system starts and is incremented at each clock interrupt by
// the length of a clock tick. The exact length of a clock tick depends
// on underlying hardware and can vary between systems.
//
// Unlike system time, the interrupt-time count is not subject to
// adjustments by users or the Windows time service, making it a better
// choice for measuring short durations. Applications that require greater
// precision than the interrupt-time count (system timer tick) should use
// a high-resolution timer (QPC, cpu clock tick).
//
// The QueryInterruptTime[Precise], QueryUnbiasedInterruptTime[Precise]
// functions can be used to retrieve the interrupt-time count. Unbiased
// interrupt-time means that only time that the system is in the working
// state is counted — therefore, the interrupt-time count is not "biased"
// by time the system spends in sleep or hibernation. 无偏差中断时间，即不
// 会因为系统睡眠或休眠而产生偏差，其实就是对睡眠或休眠无感只统计实际工作状态的
// 时间。
//
// The timer resolution set by the timeBeginPeriod and timeEndPeriod
// functions affects the resolution of the QueryInterruptTime and
// QueryUnbiasedInterruptTime functions. However, increasing the timer
// resolution is not recommended because it can reduce overall system
// performance and increase power consumption by preventing the processor
// from entering power-saving states. Instead, applications should use a
// high-resolution timer.
//
// These functions produces different results on debug ("checked") builds
// of Windows, because the interrupt-time count and tick count are advanced
// by approximately 49 days. This helps to identify bugs that might not
// occur until the system has been running for a long time. 在调试模式下，
// 中断时间被设置大约最多可表示49天时间，以帮助方便鉴别程序问题。
//
// void QueryInterruptTime(ULONGLONG *InterruptTime); // 精度 0.5msec ~ 15.625msec
// void QueryInterruptTimePrecise(ULONGLONG *InterruptTime);
// Windows 7 Windows Server 2008 R2
// realtimeapiset.h (include Windows.h) Mincore.lib Kernel32.dll
//      Receive the interrupt-time count in system time units of 100
//      nanoseconds (same as FILETIME).
//      QueryInterruptTimePrecise is similar to the QueryInterruptTime
//      routine, but is more precise. The interrupt time reported by
//      QueryInterruptTime is based on the latest tick of the system clock
//      timer. The system clock timer is the hardware timer that
//      periodically generates interrupts for the system clock. The uniform
//      period between system clock timer interrupts is referred to as a
//      system clock tick, and is typically in the range of 0.5 milliseconds
//      to 15.625 milliseconds, depending on the hardware platform. The
//      interrupt time value retrieved by QueryInterruptTime is accurate
//      within a system clock tick.
//      To provide a system time value that is more precise than that of
//      QueryInterruptTime, QueryInterruptTimePrecise reads the timer
//      hardware directly, therefore a QueryInterruptTimePrecise call can
//      be slower than a QueryInterruptTime call.
//      Call the KeQueryTimeIncrement routine to determine the duration of
//      a system clock tick. At startup time, the operating system
//      determines the time increment to use for the system time. This time
//      increment remains constant until the computer restarts. During this
//      time, calls to KeQueryTimeIncrement always return the same time
//      increment value. The time increment does not change while the
//      computer is running, and it does not change as the result of a
//      suspend-resume cycle.
//      To compile an application that uses this function, define
//      _WIN32_WINNT as 0x0601 or later.

prh_i64 prh_steady_secs(void) {
    return (prh_i64)GetTickCount64() / PRH_MSEC_PER_SEC; // 包含睡眠时间
}

prh_i64 prh_steady_msec(void) {
#if _WIN32_WINNT >= 0x0601
    ULONGLONG t;
    QueryInterruptTime(&t); // 精度 0.5msec ~ 15.625msec，包含睡眠时间
    return (prh_i64)t / 10000;
#else
    return (prh_i64)GetTickCount64(); // 精度 10msec ~ 16msec，包含睡眠时间
#endif
}

prh_i64 prh_steady_usec(void) {
    prh_i64 ticks = prh_clock_ticks();
    return prh_elapse_usec(ticks);
}

prh_i64 prh_steady_nsec(void) { // 保存纳秒只能表示292年
    prh_i64 ticks = prh_clock_ticks(); // 精度小于1微妙（<1us），包含待机休眠等睡眠时间
    return prh_elapse_nsec(ticks);
}

// BOOL GetThreadTimes(HANDLE hThread,
//          FILETIME *CreationTime, // the creation time of the thread
//          FILETIME *ExitTime,     // the exit time of the thread. If the thread has not exited, the content of this structure is undefined.
//          FILETIME *KernelTime,   // the amount of time that the thread has executed in kernel mode
//          FILETIME *UserTime);    // the amount of time that the thread has executed in user mode
// Windows XP Windows Server 2003
// processthreadsapi.h (include Windows.h) Kernel32.lib Kernel32.dll
//      Retrieves timing information for the specified thread.
//      If the function fails, the return value is zero. To get extended error
//      information, call GetLastError.
//      Thread creation and exit times are points in time expressed as the
//      amount of time that has elapsed since midnight on January 1, 1601 at
//      Greenwich, England.
//      Thread kernel mode and user mode times are amounts of time. For
//      example, if a thread has spent one second in kernel mode, this function
//      will fill the FILETIME structure specified by lpKernelTime with a
//      64-bit value of ten million. That is the number of 100-nanosecond units
//      in one second.
//      To retrieve the number of CPU clock cycles used by the threads, use
//      the QueryThreadCycleTime function.
//
// BOOL QueryThreadCycleTime(HANDLE handle, ULONG64 CycleTime);
// Windows Vista Windows Server 2008
// realtimeapiset.h (include Windows.h) Mincore.lib Kernel32.dll
//      Retrieves the cycle time for the specified thread. If the function
//      fails, the return value is zero. To get extended error information,
//      call GetLastError.
//      The number of CPU clock cycles used by the thread. This value includes
//      cycles spent in both user mode and kernel mode.
//      To enumerate the threads of the process, use the Thread32First and
//      Thread32Next functions. To get the thread handle for a thread
//      identifier, use the OpenThread function.
//      Do not attempt to convert the CPU clock cycles returned by
//      QueryThreadCycleTime to elapsed time. This function uses timer
//      services provided by the CPU, which can vary in implementation.
//      For example, some CPUs will vary the frequency of the timer when
//      changing the frequency at which the CPU runs and others will leave
//      it at a fixed rate. The behavior of each CPU is described in the
//      documentation provided by the CPU vendor.
//      To compile an application that uses this function, define _WIN32_WINNT
//      as 0x0600 or later.
//
// HANDLE GetCurrentThread();
// Windows XP Windows Server 2003
// processthreadsapi.h (include Windows.h) Kernel32.lib Kernel32.dll
//      Retrieves a pseudo handle for the calling thread.
//      The function cannot be used by one thread to create a handle that can
//      be used by other threads to refer to the first thread. The handle is
//      always interpreted as referring to the thread that is using it. A
//      thread can create a "real" handle to itself that can be used by other
//      threads, or inherited by other processes, by specifying the pseudo
//      handle as the source handle in a call to the DuplicateHandle function.
//      The pseudo handle need not be closed when it is no longer needed.
//      Calling the CloseHandle function with this handle has no effect. If
//      the pseudo handle is duplicated by DuplicateHandle, the duplicate
//      handle must be closed.
prh_i64 prh_thread_time(void) {
    HANDLE pseudo_handle = GetCurrentThread();
    FILETIME creation_time, exit_time, kernel_time, user_time;
    PRH_WINOS_BOOLRET(GetThreadTimes(pseudo_handle, &creation_time, &exit_time, &kernal_time, &user_time));
    return prh_impl_filetime_nsec(&kernel_time) + prh_impl_filetime_nsec(&user_time);
}

prh_i64 prh_clock_ticks(void) {
    LARGE_INTEGER ticks;
    prh_boolret(QueryPerformanceCounter(&ticks));
    return ticks.QuadPart; // LONGLONG
}

void prh_time_init(void) {
    LARGE_INTEGER freq;
    prh_boolret(QueryPerformanceFrequency(&freq));
    PRH_IMPL_TIMEINIT.ticks_per_sec = freq.QuadPart;
}

prh_i64 prh_elapse_secs(prh_i64 ticks) {
    return ticks / PRH_IMPL_TIMEINIT.ticks_per_sec;
}

prh_i64 prh_elapse_msec(prh_i64 ticks) {
    // To guard against loss-of-precision, we convert to microseconds
    // *before* dividing by ticks-per-second.
    return prh_impl_mul_div(ticks, PRH_MSEC_PER_SEC, PRH_IMPL_TIMEINIT.ticks_per_sec);
}

prh_i64 prh_elapse_usec(prh_i64 ticks) {
    // To guard against loss-of-precision, we convert to microseconds
    // *before* dividing by ticks-per-second.
    return prh_impl_mul_div(ticks, PRH_USEC_PER_SEC, PRH_IMPL_TIMEINIT.ticks_per_sec);
}

prh_i64 prh_elapse_nsec(prh_i64 ticks) {
    // To guard against loss-of-precision, we convert to nanoseconds
    // *before* dividing by ticks-per-second.
    return prh_impl_mul_div(ticks, PRH_NSEC_PER_SEC, PRH_IMPL_TIMEINIT.ticks_per_sec);
}

#ifdef PRH_TEST_IMPLEMENTATION
#include <time.h>
void prh_impl_time_test(void) {
    prh_time_init();
    printf("\n\n[MSC][time]\n");
    printf("clock tick frequency %lli\n", (long long)PRH_IMPL_TIMEINIT.ticks_per_sec);
    printf("time_t %zi-byte\n", sizeof(time_t)); // seconds
    printf("clock_t %zi-byte\n", sizeof(clock_t));
    printf("CLOCKS_PER_SEC %li\n", CLOCKS_PER_SEC);
    int i, n = 30, count = 0; prh_i64 t1, t2;
    for (i = 0; i < n; i += 1) {
        printf("system time: %lli\n", (long long)prh_system_usec());
    }
    t1 = prh_steady_msec();
    for (i = 0; i < 10; i += 1, count = 0) {
        while ((t2 = prh_steady_msec()) == t1) {
            count += 1;
        }
        printf("steady msec: %lli count %d\n", (long long)t1, count);
        t1 = t2;
    }
    for (i = 0; i < n; i += 1) {
        printf("steady usec: %lli\n", (long long)prh_steady_usec());
    }
    for (i = 0; i < n; i += 1) {
        printf("steady nsec: %lli\n", (long long)prh_steady_nsec());
    }
    for (i = 0; i < n; i += 1) {
        printf("thread nsec: %lli\n", (long long)prh_thread_time());
    }
    for (i = 0; i < n; i += 1) {
        printf("clock ticks: %lli\n", (long long)prh_clock_ticks());
    }
    prh_release_assert(prh_impl_mul_div(1000000000001LL, 1000000000LL, 1000000LL) == 1000000000001000LL);
    prh_release_assert((1000000000001LL * 1000000000LL / 1000000LL) != 1000000000001000LL);
}
#endif // PRH_TEST_IMPLEMENTATION
#else // WINDOWS end POSIX begin
// 无论地理位置如何，UNIX系统内部对时间的表示方式均是以自Epoch以来的秒数来度量的，
// Epoch亦即通用协调时间（UTC，以前也称为格林威治时间或GMT）的1970年1月1日早晨零点。
// 这也是UNIX系统问世的大致日期。日历时间存储于类型为time_t的变量中，此类型是由SUSv3
// 定义的整数类型。在32位Linux系统，time_t是一个有符号整数，可以表示的日期范围从1901
// 年12月13日20时45分52秒至2038年1月19日03时14分07秒，SUSv3未定义time_t值为负数时
// 的含义。因此当前许多32位UNIX系统都面临一个2038年的理论问题，如果执行的计算工作涉及
// 未来日期，纳秒在2038年之前就会与之遭遇。事实上到了2038年，可能所有的UNIX系统都早已
// 升级为64位或更多位数的系统，这一问题也许会随之而大为缓解。然而32位嵌入式系统，由于
// 其寿命较台式机硬件更长，故而仍然会少此问题的困扰。此外，对于依然以32位time_t格式
// 保存时间的历史数据和应用程序，这个问题将依然存在。
//
// #include <sys/time.h>
// int gettimeofday(struct timeval *tv, struct timezone *tz);
// struct timeval {
//     time_t tv_sec;       // seconds since 00:00:00 1 Jan 1970 UTC
//     suseconds_t tv_usec; // additional microseconds (long int)
// };
//      Return 0 for success. On error, -1 is returned and errno is set to
//      indicate the error.
//      参数tz是历史产物，早期的UNIX实现用其来获取系统的时区信息，目前已遭废弃，应
//      始终将其置为NULL。
// #include <time.h>
// time_t time(NULL);
//      返回相当于gettimeofday的tv_sec值，之所以存在两个本质上目的相同的系统调用，
//      自有历史原因。早期的UNIX实现提供了time()，而4.3BSD又补充了更为精确的
//      gettimeofday系统调用。
//
// SVr4, 4.3BSD. POSIX.1-2001 describes gettimeofday() but not settimeofday().
// POSIX.1-2008 marks gettimeofday() as obsolete, recommending the use of
// clock_gettime(2) instead.
// The time returned by gettimeofday() is affected by discontinuous jumps in
// the system time (e.g., if the system administrator manually changes the
// system time). If you need a monotonically increasing clock, see
// clock_gettime(2).
//
// 这里所描述的时间相关的各种系统调用的精度是受限于系统软件时钟（software clock）的
// 分辨率，它的度量单位被称为 jiffies。jiffies 的大小是定义在内核源代码的常量HZ。这
// 是内核按照round-robin的分时调度算法分配CPU进程的单位。
// 在2.4或以上版本的Linux/x86-32内核中，软件时钟速度是100赫兹，也就是说一个jiffy是
// 10毫秒。自Linux面世以来，由于CPU的速度已大大增加，Linux/x86-32 2.6.0内核的软件
// 时钟速度已经提高到1000赫兹。更高的软件时钟速率意味着定时器可以有更高的操作精度和
// 时间可以拥有更高的测量精度。然而这并非可以任意提高时钟频率，因为每个时钟中断会消
// 耗少量的CPU时间，这部分时间CPU无法执行任何操作。
// 经过内核开发人员之间的讨论，最终导致软件时钟频率成为一个可配置的内核选项。自2.6.13
// 内核，时钟可以设置到100、250（默认）或1000赫兹，对应的jiffy值分别为10、4、1毫秒。
// 自内核2.6.20，增加了一个频率300赫兹，它可以被两种常见的视频帧速率25帧每秒（PAL）
// 和30帧每秒（NTSC）整除。
#if defined(prh_plat_apple)
// https://developer.apple.com/forums/thread/735632 (Availability of Low-Level APIs)
// https://developer.apple.com/documentation/os/reading-unix-manual-pages
// https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/
// https://github.com/apple/darwin-xnu/blob/main/EXTERNAL_HEADERS/AvailabilityMacros.h
// https://developer.apple.com/documentation/kernel/mach
// https://github.com/phracker/MacOSX-SDKs/tree/master/MacOSX11.3.sdk
// https://www.manpagez.com/man/3/clock_gettime/
#include <mach/task.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#include <time.h> // clock_gettime time_t struct timespec
#include <sys/time.h> // gettimeofday time_t struct timeval

// The system clock represents the system-wide real time wall clock. It may
// not be monotonic: on most systems, the system time can be adjusted at any
// moment. System clock measures Unix Time (i.e., time since 00:00:00
// Coordinated Universal Time (UTC), Thursday, 1 January 1970, not counting
// leap seconds). The system clock's time value can be internally adjusted
// at any time by the operating system, for example due to NTP synchronization
// or the user changing the system's clock. Daylight Saving Time and time zone
// changes, however, do not affect it since it is based on the UTC time-zone.

prh_i64 prh_system_secs(void) {
#if defined(CLOCK_REALTIME)
    struct timespec ts;
    prh_zeroret(clock_gettime(CLOCK_REALTIME, &ts));
    return (prh_i64)ts.tv_sec;
#else
    struct timeval tv;
    prh_zeroret(gettimeofday(&tv, prh_null));
    return (prh_i64)tv.tv_sec;
#endif
}

prh_i64 prh_system_msec(void) {
#if defined(CLOCK_REALTIME)
    struct timespec ts;
    prh_zeroret(clock_gettime(CLOCK_REALTIME, &ts));
    return (prh_i64)ts.tv_sec * PRH_MSEC_PER_SEC + (ts.tv_nsec / 1000000);
#else
    struct timeval tv;
    prh_zeroret(gettimeofday(&tv, prh_null));
    return (prh_i64)tv.tv_sec * PRH_MSEC_PER_SEC + (tv.tv_usec / 1000);
#endif
}

prh_i64 prh_system_usec(void) {
#if defined(CLOCK_REALTIME)
    struct timespec ts;
    prh_zeroret(clock_gettime(CLOCK_REALTIME, &ts));
    return (prh_i64)ts.tv_sec * PRH_USEC_PER_SEC + (ts.tv_nsec / 1000);
#else
    struct timeval tv;
    prh_zeroret(gettimeofday(&tv, prh_null));
    return (prh_i64)tv.tv_sec * PRH_USEC_PER_SEC + tv.tv_usec;
#endif
}

void prh_system_time(prh_timeusec *t) {
#if defined(CLOCK_REALTIME)
    struct timespec ts;
    prh_zeroret(clock_gettime(CLOCK_REALTIME, &ts));
    t->sec = ts.tv_sec;
    t->usec = ts.tv_nsec / 1000;
#else
    struct timeval tv;
    prh_zeroret(gettimeofday(&tv, prh_null));
    t->sec = tv.tv_sec;
    t->usec = tv.tv_usec;
#endif
}

void prh_impl_date_time(prh_datetime *t, const struct tm *tm, prh_i32 usec) {
    // struct tm {
    //     int tm_sec;   // 0 ~ 60 since C99 allows for a positive leap second
    //     int tm_min;   // 0 ~ 59
    //     int tm_hour;  // 0 ~ 23
    //     int tm_mday;  // 1 ~ 31
    //     int tm_mon;   // 0 ~ 11
    //     int tm_year;  // year since 1900
    //     int tm_wday;  // 0 ~ 6 (sunday = 0)
    //     int tm_yday;  // 0 ~ 365
    //     int tm_isdst; // > 0 dst in effect, = 0 dst not effect, < 0 dst not available
    // };
    t->year = tm->tm_year + 1900; // 正负20亿年
    t->month = tm->tm_mon + 1; // 1 ~ 12
    t->mday = tm->tm_mday; // 1 ~ 31
    t->wday = tm->tm_wday; // 0 ~ 6 (sunday = 0)
    t->hour = tm->tm_hour; // 0 ~ 23
    t->min = tm->tm_min; // 0 ~ 59
    t->sec = tm->tm_sec; // 0 ~ 60 since C99
    t->usec = usec; // 0 ~ 999999
}

void prh_impl_system_date(struct tm *tm, const prh_datetime *t) {
    memset(tm, 0, sizeof(struct tm));
    tm->tm_year = t->year - 1900;
    tm->tm_mon = t->month - 1;
    tm->tm_mday = t->mday;
    tm->tm_hour = t->hour;
    tm->tm_min = t->min;
    tm->tm_sec = t->sec;
    tm->tm_wday = t->wday;
    tm->tm_isdst = -1; // not available
}

void prh_system_time_from_date(prh_timeusec *utc_time, const prh_datetime *utc_date) {
    // #include <time.h>
    // [[deprecated]] time_t timelocal(struct tm *tm);
    // time_t timegm(struct tm *tm);
    // The functions timelocal() and timegm() are the inverses of
    // localtime(3) and gmtime(3).
    struct tm utc;
    prh_impl_system_date(&utc, utc_date);
    time_t utc_secs = timegm(&utc); // timegm 是 gmtime_r 的反操作，忽略 tm_wday 和 tm_yady
    if (utc_secs == (time_t)-1) prh_abort_error(errno);
    utc_time->sec = utc_secs;
    utc_time->usec = utc_date->usec;
}

void prh_system_time_from_local_date(prh_timeusec *utc_time, const prh_datetime *local_date) {
    struct tm local;
    prh_impl_system_date(&local, local_date);
    time_t utc_secs = mktime(&local); // mktime 是 localtime_r 的反操作，忽略 tm_wday 和 tm_yady
    if (utc_secs == (time_t)-1) prh_abort_error(errno);
    utc_time->sec = utc_secs;
    utc_time->usec = local_date->usec;
}

void prh_system_date(prh_datetime *utc_date) {
    prh_timeusec utc_time;
    prh_system_time(&utc_time);
    time_t time = (time_t)utc_time.sec;
    struct tm tm;
    prh_boolret(gmtime_r(&time, &tm));
    prh_impl_date_time(utc_date, &tm, utc_time.usec);
}

void prh_local_date(prh_datetime *local_date) {
    prh_timeusec utc_time;
    prh_system_time(&utc_time);
    time_t time = (time_t)utc_time.sec;
    struct tm tm;
    prh_boolret(localtime_r(&time, &tm));
    prh_impl_date_time(local_date, &tm, utc_time.usec);
}

void prh_local_date_from(prh_datetime *local_date, const prh_timeusec *utc_time) {
    time_t time = (time_t)utc_time->sec;
    struct tm tm;
    prh_boolret(localtime_r(&time, &tm));
    prh_impl_date_time(local_date, &tm, utc_time->usec);
}

void prh_system_date_from(prh_datetime *utc_date, const prh_timeusec *utc_time) {
    // 夏令时（Daylight Saving Time，DST）是一种为了节约能源而人为调整时钟的做法。
    // 具体来说，它通过在夏季将时钟拨快一定时间（通常是1小时）。夏令时的主要目的是减
    // 少照明需求。通过将时钟拨快1小时，人们在夏季的傍晚可以更晚地开灯，从而节省电力。
    // 在一些国家和地区，夏令时被认为可以提高工作效率，因为人们可以在自然光照下工作
    // 更长时间。
    // #include <time.h>
    // struct tm *gmtime(const time_t *timer);
    // struct tm *gmtime_r(const time_t *timer, struct tm *result);
    //      If an error is detected, gmtime shall return a null pointer
    //      and set errno to indicate the error.
    //      The gmtime() function need not be thread-safe. The gmtime()
    //      function shall convert the time in seconds since the Epoch
    //      pointed to by timer into a broken-down time, expressed as
    //      Coordinated Universal Time (UTC).
    // struct tm *localtime(const time_t *timer);
    // struct tm *localtime_r(const time_t *timer, struct tm *result);
    //      If an error is detected, localtime shall return a null pointer
    //      and set errno to indicate the error.
    //      The localtime() function need not be thread-safe. The localtime()
    //      function shall convert the time in seconds since the Epoch pointed
    //      to by timer into a broken-down time, expressed as a local time.
    //      The function corrects for the timezone and any seasonal time
    //      adjustments. Local timezone information is used as though
    //      localtime() calls tzset().
    // time_t mktime(struct tm *timeptr);
    //      The mktime() function shall return the specified time since the
    //      Epoch encoded as a value of type time_t. If the time since the
    //      Epoch cannot be represented, the function shall return the value
    //       (time_t)-1 and set errno to indicate the error.
    //      The mktime() function shall convert the broken-down time,
    //      expressed as **local time**, in the structure pointed to by
    //      timeptr, into a time since the Epoch value with the same encoding
    //      as that of the values returned by time(). The original values of
    //      the tm_wday and tm_yday components of the structure shall be
    //      ignored.
    //      A positive or 0 value for tm_isdst shall cause mktime() to presume
    //      initially that Daylight Savings Time, respectively, is or is not
    //      in effect for the specified time. A negative value for tm_isdst
    //      shall cause mktime() to attempt to determine whether Daylight
    //      Savings Time is in effect for the specified time.
    //      Upon successful completion, the values of the tm_wday and tm_yday
    //      components of the structure shall be set appropriately, and the
    //      other components shall be set to represent the specified time
    //      since the Epoch, but with their values forced to the ranges
    //      indicated in the <time.h> entry; the final value of tm_mday shall
    //      not be set until tm_mon and tm_year are determined.
    time_t time = (time_t)utc_time->sec;
    struct tm tm;
    prh_boolret(gmtime_r(&time, &tm));
    prh_impl_date_time(utc_date, &tm, utc_time->usec);
}

// #include <time.h>
// int clock_gettime(clockid_t clockid, struct timespec *ts);
// int clock_getres(clockid_t clockid, struct timespec *res);
// struct timespec {
//     time_t     tv_sec;   /* Seconds */
//     /* ... */  tv_nsec;  /* Nanoseconds [0, 999'999'999] */
// };
//
// _POSIX_C_SOURCE >= 199309L POSIX.1-2001, SUSv2. Linux 2.6.
//
// On POSIX systems on which these functions are available, the symbol
// _POSIX_TIMERS is defined in <unistd.h> to a value greater than 0.
// POSIX.1-2008 makes these functions mandatory. The symbols _POSIX_MONOTONIC_CLOCK,
// _POSIX_CPUTIME, _POSIX_THREAD_CPUTIME indicate that CLOCK_MONOTONIC,
// CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID are available. See also
// sysconf(3).
//
// Return 0 for success. On error, -1 is returned and errno is set to
// indicate the error. EOVERFLOW - The timestamp would not fit in time_t
// range. This can happen if an executable with 32-bit time_t is run on
// a 64-bit kernel when the time is 2038-01-19 03:14:08 UTC or later.
// However, when the system time is out of time_t range in other
// situations, the behavior is undefined.
//
// According to POSIX.1-2001, a process with "appropriate privileges"
// may set the CLOCK_PROCESS_CPUTIME_ID and CLOCK_THREAD_CPUTIME_ID
// clocks using clock_settime(). On Linux, these clocks are not settable
// (i.e., no process has "appropriate privileges"). C library/kernel
// differences: On some architectures, an implementation of clock_gettime()
// is provided in the vdso(7).
//
// The steady clock represents a monotonic clock. The time points of this
// clock cannot decrease as physical time moves forward and the time between
// ticks of this clock is constant. This clock is not related to wall clock
// time (for example, it can be time since last reboot), and is most suitable
// for measuring intervals.
//
// https://www.man7.org/linux/man-pages/man3/clock_gettime.3.html
//
// CLOCK_REALTIME
//      A settable system-wide clock that measures real (i.e., wall-clock)
//      time. Setting this clock requires appropriate privileges. This clock
//      is affected by discontinuous jumps in the system time (e.g., if the
//      system administrator manually changes the clock), and by frequency
//      adjustments performed by NTP and similar applications via adjtime(3),
//      adjtimex(2), clock_adjtime(2), and ntp_adjtime(3). This clock normally
//      counts the number of seconds since 1970-01-01 00:00:00 Coordinated
//      Universal Time (UTC) except that it ignores leap seconds; near a leap
//      second it is typically adjusted by NTP to stay roughly in sync with
//      UTC. 不包含闰秒的 UTC 时间。
// CLOCK_REALTIME_ALARM (since Linux 3.0; Linux-specific)
//      Like CLOCK_REALTIME, but not settable. See timer_create(2) for further
//      details. 不可设置。
// CLOCK_REALTIME_COARSE (since Linux 2.6.32; Linux-specific)
//      A faster but less precise version of CLOCK_REALTIME. This clock is not
//      settable. Use when you need very fast, but not fine-grained timestamps.
//      Requires per-architecture support, and probably also architecture
//      support for this flag in the vdso(7).
//      不可设置，比 CLOCK_REALTIME 速度快，但损失精度。
// CLOCK_TAI (since Linux 3.10; Linux-specific)
//      A nonsettable system-wide clock derived from wall-clock time but
//      counting leap seconds. This clock does not experience discontinuities
//      or frequency adjustments caused by inserting leap seconds as
//      CLOCK_REALTIME does. The acronym TAI refers to International Atomic
//      Time. 不可设置，但是包含闰秒。
//
// CLOCK_MONOTONIC
//      A nonsettable system-wide clock that represents monotonic time
//      since — as described by POSIX — "some unspecified point in the past".
//      On Linux, that point corresponds to the number of seconds that the
//      system has been running since it was booted.
//      The CLOCK_MONOTONIC clock is not affected by discontinuous
//      jumps in the system time (e.g., if the system administrator
//      manually changes the clock), but is affected by frequency
//      adjustments（会被频率调整影响）. This clock does not count time
//      that the system is suspended. All CLOCK_MONOTONIC variants
//      guarantee that the time returned by consecutive calls will
//      not go backwards, but successive calls may — depending on the
//      architecture — return identical (not-increased) time values.
//      不包含系统挂起的时间。由于精度原因，后续调用可能返回相同的未增加的时间值。
// CLOCK_MONOTONIC_RAW (since Linux 2.6.28; Linux-specific)
//      Similar to CLOCK_MONOTONIC, but provides access to a raw
//      hardware-based time that is not subject to frequency
//      adjustments. This clock does not count time that the system is
//      suspended. 不受频率调整影响，不包含系统挂起时间。
// CLOCK_MONOTONIC_COARSE (since Linux 2.6.32; Linux-specific)
//      A faster but less precise version of CLOCK_MONOTONIC. Use when you
//      need very fast, but not fine-grained timestamps. Requires
//      per-architecture support, and probably also architecture support
//      for this flag in the vdso(7). 比 CLOCK_MONOTONIC 更快，但是损失精度。
//
// CLOCK_BOOTTIME (since Linux 2.6.39; Linux-specific)
//      A nonsettable system-wide clock that is identical to CLOCK_MONOTONIC,
//      except that it also includes any time that the system is suspended.
//      This allows applications to get a suspend-aware monotonic clock
//      without having to deal with the complications of CLOCK_REALTIME,
//      which may have discontinuities if the time is changed using
//      settimeofday(2) or similar. 与 CLOCK_MONOTONIC 类似，但包含系统挂起时间。
// CLOCK_BOOTTIME_ALARM (since Linux 3.0; Linux-specific)
//      Like CLOCK_BOOTTIME. See timer_create(2) for further
//      details.
//
// CLOCK_PROCESS_CPUTIME_ID (since Linux 2.6.12)
//      This is a clock that measures CPU time consumed by this process
//      (i.e., CPU time consumed by all threads in the process). On Linux,
//      this clock is not settable.
// CLOCK_THREAD_CPUTIME_ID (since Linux 2.6.12)
//      This is a clock that measures CPU time consumed by this thread.
//      On Linux, this clock is not settable.
//
// https://man.freebsd.org
// FreeBSD 15.0
//      CLOCK_MONOTONIC
//      CLOCK_MONOTONIC_PRECISE
//      CLOCK_MONOTONIC_FAST
//      CLOCK_MONOTONIC_COARSE
//      CLOCK_BOOTTIME
//          包含系统挂起时间，CLOCK_BOOTTIME 是 CLOCK_MONOTONIC 的别名。
//      CLOCK_UPTIME
//      CLOCK_UPTIME_PRECISE
//      CLOCK_UPTIME_FAST
//          不包含系统挂起时间。
// FreeBSD 14.0
//      CLOCK_MONOTONIC
//      CLOCK_MONOTONIC_PRECISE
//      CLOCK_MONOTONIC_FAST
//      CLOCK_MONOTONIC_COARSE
//          包含系统挂起时间。
//      CLOCK_UPTIME
//      CLOCK_UPTIME_PRECISE
//      CLOCK_UPTIME_FAST
//      CLOCK_BOOTTIME
//          不包含系统挂起时间，CLOCK_BOOTTIME 是 CLOCK_UPTIME 的别名。
//
// #if defined(__APPLE__)
// https://www.manpagez.com/man/3/clock_gettime/
// https://github.com/phracker/MacOSX-SDKs/blob/master/MacOSX11.3.sdk/usr/include/time.h
//
// #include <time.h>
// int clock_gettime(clockid_t clock_id, struct timespec *tp);
// int clock_settime(clockid_t clock_id, const struct timespec *tp);
// int clock_getres(clockid_t clock_id, struct timespec *tp);
// uint64_t clock_gettime_nsec_np(clockid_t clock_id);
//
// These functions first appeared in Mac OSX 10.12. The clock_gettime(),
// clock_settime(), and clock_getres() system calls conform to IEEE Std
// 1003.1b-1993 (POSIX.1). cleck_gettime_nsec_np() is a non-portable
// Darwin extension. The clock IDs CLOCK_MONOTONIC_RAW and CLOCK_UPTIME_RAW
// are extensions to the POSIX interface.
//
// #if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
//      #define CLOCK_MONOTONIC_RAW         _CLOCK_MONOTONIC_RAW
//      #define CLOCK_MONOTONIC_RAW_APPROX  _CLOCK_MONOTONIC_RAW_APPROX
//      #define CLOCK_UPTIME_RAW            _CLOCK_UPTIME_RAW
//      #define CLOCK_UPTIME_RAW_APPROX     _CLOCK_UPTIME_RAW_APPROX
// #endif
//
// For clock_gettime_nsec_np() a return value of non-0 indicates success.
// A 0 return value indicates an error occurred and an error code is stored
// in errno.
//
// CLOCK_REALTIME
//      the system's real time (i.e. wall time) clock, expressed as the
//      amount of time since the Epoch. This is the same as the value
//      returned by gettimeofday(2).
//
// CLOCK_MONOTONIC
//      clock that increments monotonically, tracking the time since an
//      arbitrary point, and will continue to increment while the system
//      is asleep. 包含睡眠时间。
// CLOCK_MONOTONIC_RAW
//      clock that increments monotonically, tracking the time since an
//      arbitrary point like CLOCK_MONOTONIC. However, this clock is
//      unaffected by frequency or time adjustments. It should not be
//      compared to other system time sources. 不受频率调整、时间调整的影响。
// CLOCK_MONOTONIC_RAW_APPROX
//      like CLOCK_MONOTONIC_RAW, but reads a value cached by the system
//      at context switch. This can be read faster, but at a loss of
//      accuracy as it may return values that are milliseconds old.
//      比 CLOCK_MONOTONIC_RAW 更快，但是损失精度，可能返回几毫秒前的时间。
//
// CLOCK_UPTIME_RAW
//      clock that increments monotonically, in the same manner as
//      CLOCK_MONOTONIC_RAW, but that does not increment while the system
//      is asleep. The returned value is identical to the result of
//      **mach_absolute_time()** after the appropriate mach_timebase
//      conversion is applied. 与 CLOCK_MONOTONIC_RAW 类似，但不包含睡眠时间。
// CLOCK_UPTIME_RAW_APPROX
//      like CLOCK_UPTIME_RAW, but reads a value cached by the system at
//      context switch. This can be read faster, but at a loss of accuracy
//      as it may return values that are milliseconds old.
//      比 CLOCK_UPTIME_RAW 更快，但是损失精度，可能返回几毫秒前的时间。
//
// CLOCK_PROCESS_CPUTIME_ID
//      clock that tracks the amount of CPU (in user- or kernel-mode) used
//      by the calling process.
// CLOCK_THREAD_CPUTIME_ID
//      clock that tracks the amount of CPU (in user- or kernel-mode) used
//      by the calling thread.
//
// clock_getres()
//      The resolution of a clock is returned by the clock_getres() call.
//      This value is placed in a (non-null) *tp. This value may be smaller
//      than the actual precision of the underlying clock, but represents
//      a lower bound on the resolution.
// clock_gettime_nsec_np()
//      As a non-portable extension, the clock_gettime_nsec_np() function
//      will return the clock value in 64-bit nanoseconds.
//
// https://developer.apple.com/documentation/driverkit/3433733-mach_timebase_info/
// https://developer.apple.com/documentation/kernel/1462446-mach_absolute_time
// https://developer.apple.com/documentation/kernel/1646199-mach_continuous_time
//
// typedef struct mach_timebase_info *mach_timebase_info_t;
// typedef struct mach_timebase_info {
//      uint32_t denom;
//      uint32_t numer;
// } mach_timebase_info_data_t; // macOS 10.0+
//      Raw Mach Time API In general prefer to use the <time.h> API
//      clock_gettime_nsec_np(3), which deals in the same clocks
//      (and more) in ns units. Conversion of ns to (resp. from) tick
//      units as returned by the mach time APIs is performed by division
//      (resp. multiplication) with the fraction returned by
//      mach_timebase_info(). 纳秒数除以这个分数可以得到时钟 tick 数。
//
// kern_return_t mach_timebase_info(mach_timebase_info_t info); // DriverKit 24.4+
//      Returns fraction to multiply a value in mach tick units with to
//      convert it to nanoseconds. Return KERN_SUCCESS if info was filled
//      in. 返回一个分数 numer / denom，这个分数乘以 tick 数可以转换成纳秒。
//
// uint64_t mach_absolute_time(void); // macOS 10.0+
//      Return current value of a clock that increments monotonically
//      in tick units (starting at an arbitrary point), this clock does
//      not increment while the system is asleep. 不包含系统睡眠时间。
//      等价于 clock_gettime_nsec_np(CLOCK_UPTIME_RAW) 返回的纳秒数。
//
// uint64_t mach_continuous_time(void); // macOS 10.12+
//      Returns current value of a clock that increments monotonically
//      in tick units (starting at an arbitrary point), including while
//      the system is asleep. 包含系统睡眠时间。
//      等价于 clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW) 返回的纳秒数。
// #endif

prh_i64 prh_clock_ticks(void) {
#if defined(prh_plat_apple)
    #if defined(AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER) // macOS 10.12+
    return (prh_i64)clock_gettime_nsec_np(CLOCK_MONOTONIC_RAW); // 包含睡眠时间，不受频率或时间调整的影响，等价于 mach_continuous_time
    #else // macOS 10.0+
    return (prh_i64)mach_absolute_time(); // 不包含睡眠时间，不受频率或时间调整的影响
    #endif
#elif defined(prh_plat_linux)
    #if defined(CLOCK_BOOTTIME)
    clockid_t clock_id = CLOCK_BOOTTIME; // Linux 2.6.39，包含睡眠时间
    #elif defined(CLOCK_MONOTONIC_RAW)
    clockid_t clock_id = CLOCK_MONOTONIC_RAW; // Linux 2.6.28，不包含睡眠时间，不受频率或时间调整的影响
    #else
    clockid_t clock_id = CLOCK_MONOTONIC; // 不包含睡眠时间，受频率或时间调整的影响
    #endif
    struct timespec ts;
    prh_zeroret(clock_gettime(clock_id, &ts));
    return (prh_i64)ts.tv_sec * PRH_NSEC_PER_SEC + ts.tv_nsec;
#elif defined(CLOCK_MONOTONIC)
    struct timespec ts; // FreeBSD-like system, CLOCK_MONOTONIC 包含系统睡眠时间
    prh_zeroret(clock_gettime(CLOCK_MONOTONIC, &ts));
    return (prh_i64)ts.tv_sec * PRH_NSEC_PER_SEC + ts.tv_nsec;
#else
    struct timeval tv;
    prh_zeroret(gettimeofday(&tv, prh_null));
    return (prh_i64)tv.tv_sec * 1000000000 + tv.tv_usec * 1000;
#endif
}

#if defined(prh_plat_apple) && !defined(AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER)
#define PRH_IMPL_NSEC_PRECISE 0
void prh_time_init(void) {
    mach_timebase_info_data_t info; // ticks * n / d = nsecs => nsecs / (n / d) = ticks => nsecs * d / n = ticks
    mach_timebase_info(&info); // 1-sec = 1000000000-nsec = 1000000000 * d / n ticks
    PRH_IMPL_TIMEINIT.ticks_per_sec = prh_impl_mul_div(PRH_NSEC_PER_SEC, info.denom, info.numer);
}
#else
#define PRH_IMPL_NSEC_PRECISE 1
void prh_time_init(void) {
    PRH_IMPL_TIMEINIT.ticks_per_sec = PRH_NSEC_PER_SEC;
}
#endif

prh_i64 prh_elapse_secs(prh_i64 ticks) {
#if PRH_IMPL_NSEC_PRECISE
    return ticks / PRH_NSEC_PER_SEC;
#else
    return ticks / PRH_IMPL_TIMEINIT.ticks_per_sec;
#endif
}

prh_i64 prh_elapse_msec(prh_i64 ticks) {
#if PRH_IMPL_NSEC_PRECISE
    return ticks / 1000000;
#else
    // To guard against loss-of-precision, we convert to microseconds
    // *before* dividing by ticks-per-second.
    return prh_impl_mul_div(ticks, PRH_MSEC_PER_SEC, PRH_IMPL_TIMEINIT.ticks_per_sec);
#endif
}

prh_i64 prh_elapse_usec(prh_i64 ticks) {
#if PRH_IMPL_NSEC_PRECISE
    return ticks / 1000;
#else
    // To guard against loss-of-precision, we convert to microseconds
    // *before* dividing by ticks-per-second.
    return prh_impl_mul_div(ticks, PRH_USEC_PER_SEC, PRH_IMPL_TIMEINIT.ticks_per_sec);
#endif
}

prh_i64 prh_elapse_nsec(prh_i64 ticks) {
#if PRH_IMPL_NSEC_PRECISE
    return ticks;
#else
    // To guard against loss-of-precision, we convert to nanoseconds
    // *before* dividing by ticks-per-second.
    return prh_impl_mul_div(ticks, PRH_NSEC_PER_SEC, PRH_IMPL_TIMEINIT.ticks_per_sec);
#endif
}

prh_i64 prh_steady_secs(void) {
    return prh_elapse_secs(prh_clock_ticks());
}

prh_i64 prh_steady_msec(void) {
    return prh_elapse_msec(prh_clock_ticks());
}

prh_i64 prh_steady_usec(void) { // 保存微妙可以表示29.2万年
    return prh_elapse_usec(prh_clock_ticks());
}

prh_i64 prh_steady_nsec(void) { // 保存纳秒只能表示292年
    return prh_elapse_nsec(prh_clock_ticks());
}

prh_i64 prh_thread_time(void) {
    // https://www.man7.org/linux/man-pages/man7/posixoptions.7.html
    // defined(_POSIX_THREAD_CPUTIME) && (_POSIX_THREAD_CPUTIME > 0)
#if defined(CLOCK_THREAD_CPUTIME_ID)
    struct timespec ts;
    prh_zeroret(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts));
    return (prh_i64)ts.tv_sec * PRH_NSEC_PER_SEC + ts.tv_nsec;
#else
    return prh_steady_nsec();
#endif
}

#ifdef PRH_TEST_IMPLEMENTATION
void prh_impl_time_test(void) {
    prh_time_init();
    printf("\n\n[GNU][time]\n");
    printf("clock tick frequency %lli\n", (long long)PRH_IMPL_TIMEINIT.ticks_per_sec);
    printf("time_t %zi-byte\n", sizeof(time_t)); // seconds, it is signed integer
    printf("clock_t %zi-byte\n", sizeof(clock_t));
    printf("CLOCKS_PER_SEC %li\n", CLOCKS_PER_SEC);
    int i, n = 30, count = 0; prh_i64 t1, t2;
    for (i = 0; i < n; i += 1) {
        printf("system time: %lli\n", (long long)prh_system_usec());
    }
    t1 = prh_steady_msec();
    for (i = 0; i < 10; i += 1, count = 0) {
        while ((t2 = prh_steady_msec()) == t1) {
            count += 1;
        }
        printf("steady msec: %lli count %d\n", (long long)t1, count);
        t1 = t2;
    }
    for (i = 0; i < n; i += 1) {
        printf("steady usec: %lli\n", (long long)prh_steady_usec());
    }
    for (i = 0; i < n; i += 1) {
        printf("steady nsec: %lli\n", (long long)prh_steady_nsec());
    }
    for (i = 0; i < n; i += 1) {
        printf("thread nsec: %lli\n", (long long)prh_thread_time());
    }
    for (i = 0; i < n; i += 1) {
        printf("clock ticks: %lli\n", (long long)prh_clock_ticks());
    }
    prh_release_assert(prh_impl_mul_div(1000000000001LL, 1000000000LL, 1000000LL) == 1000000000001000LL);
}
#endif // PRH_TEST_IMPLEMENTATION
#endif // POSIX end
#endif // PRH_TIME_IMPLEMENTATION
#endif // PRH_TIME_INCLUDE

#ifdef PRH_THRD_INCLUDE
typedef struct {
    void *userdata;
    prh_unt impl_tid_;
    prh_i32 thrd_id;
    prh_u32 extra;
} prh_thrd;

typedef struct {
    prh_i32 maxthrds, thrd_cnt;
    prh_thrd *main;
    // [prh_thrd *]
    // [prh_thrd *]
} prh_thrd_struct;

prh_inline prh_thrd **prh_thrd_list_begin(prh_thrd_struct *s) {
    return &s->main;
}

prh_inline prh_thrd **prh_thrd_list_end(prh_thrd_struct *s) {
    return &s->main + s->maxthrds + 1;
}

prh_inline prh_thrd *prh_thrd_get(prh_thrd_struct *s, int index) {
    assert(index >= 0 && index <= s->maxthrds);
    return prh_thrd_list_begin(s)[index]; // 0 for main thread
}

prh_inline prh_thrd *prh_thrd_main(prh_thrd_struct *s) {
    assert(s->main == prh_thrd_get(s, 0));
    return s->main;
}

#define prh_thrd_for_begin(thrd_struct) {                                       \
        prh_thrd_struct *prh_impl_s = (thrd_struct);                            \
        prh_thrd **prh_impl_p = prh_thrd_list_begin(prh_impl_s);                \
        prh_thrd **prh_impl_last = prh_thrd_list_end(prh_impl_s);               \
        while (prh_impl_p < prh_impl_last) {                                    \
            prh_thrd *it = *prh_impl_p++;                                       \
            if (it == prh_null) continue;

#define prh_thrd_for_end()                                                      \
        }                                                                       \
    }

typedef prh_void_ptr (*prh_thrdproc_t)(prh_thrd* thrd);
typedef void (*prh_thrdfree_t)(void *userdata, int thrd_index); // thrd_index 0 for main thrd

prh_thrd *prh_thrd_self(void);
prh_thrd *prh_thrd_main(prh_thrd_struct *s);
prh_thrd *prh_thrd_get(prh_thrd_struct *s, int thrd_index);

prh_inline void *prh_thrd_data(prh_thrd *thrd) {
    return thrd->userdata;
}

prh_inline int prh_thrd_id(prh_thrd *thrd) {
    return thrd->thrd_id;
}

prh_inline int prh_thrd_self_id(void) {
    return prh_thrd_id(prh_thrd_self());
}

prh_inline void *prh_thrd_self_data(void) {
    return prh_thrd_data(prh_thrd_self());
}

prh_inline int prh_thrd_main_id(prh_thrd_struct *s) {
    return prh_thrd_id(prh_thrd_main(s));
}

prh_inline void *prh_thrd_main_data(prh_thrd_struct *s) {
    return prh_thrd_data(prh_thrd_main(s));
}

prh_thrd_struct *prh_thrd_init(int start_id, int maxthrds, void *userdata);
prh_thrd_struct *prh_thrd_inix(int start_id, int maxthrds, int mainudsize); // get prh_thrd_main_data() to init userdata
void *prh_thrd_creatx(prh_thrd_struct *s, int thrdudsize); // only create thrd, after init userdata, call prh_thrd_starx
int prh_thrd_start(prh_thrd_struct *s, prh_thrdproc_t proc, int stacksize, void *userdata); // return thrd index
int prh_thrd_starx(prh_thrd_struct *s, void *thrd_create_data, prh_thrdproc_t proc, int stacksize); // return thrd index
void prh_thrd_join(prh_thrd_struct *s, int thrd_index, prh_thrdfree_t udatafree); // remember to free main thrd using prh_thrd_free
void prh_thrd_jall(prh_thrd_struct **s, prh_thrdfree_t udatafree); // free all joined thrd including main thrd
void prh_thrd_free(prh_thrd_struct **s, prh_thrdfree_t udatafree); // free main thrd

typedef struct prh_thrd_mutex prh_thrd_mutex;
prh_thrd_mutex *prh_thrd_mutex_init(void);
prh_thrd_mutex *prh_thrd_recursive_mutex_init(void);
void prh_thrd_mutex_free(prh_thrd_mutex **p);
void prh_thrd_mutex_lock(prh_thrd_mutex *p);
void prh_thrd_mutex_unlock(prh_thrd_mutex *p);

typedef struct prh_thrd_cond prh_thrd_cond;
prh_thrd_cond *prh_thrd_cond_init(void);
void prh_thrd_cond_free(prh_thrd_cond **p);
void prh_thrd_cond_lock(prh_thrd_cond *p);
void prh_thrd_cond_unlock(prh_thrd_cond *p);
void prh_thrd_cond_wait(prh_thrd_cond *p, bool (*cond_meet)(void *), void *param);
bool prh_thrd_cond_timedwait(prh_thrd_cond *p, prh_u32 msec, bool (*cond_meet)(void *), void *param);
void prh_thrd_cond_signal(prh_thrd_cond *p);
void prh_thrd_cond_broadcast(prh_thrd_cond *p);

typedef struct prh_thrd_cond prh_thrd_sem;
prh_inline prh_thrd_sem *prh_thrd_sem_init(void) { return (prh_thrd_sem *)prh_thrd_cond_init(); }
prh_inline void prh_thrd_sem_free(prh_thrd_sem **s) { prh_thrd_cond_free((prh_thrd_cond **)s); }
void prh_thrd_sem_wait(prh_thrd_sem *s);
void prh_thrd_sem_post(prh_thrd_sem *s, int n);

typedef struct prh_sleep_cond prh_sleep_cond;
prh_sleep_cond *prh_sleep_cond_init(void);
void prh_sleep_cond_free(prh_sleep_cond **p);
void prh_thrd_sleep(prh_sleep_cond *p);
void prh_thrd_wakeup(prh_sleep_cond *p);

#ifdef PRH_THRD_STRIP_PREFIX
#define thrd_t                      prh_thrd
#define thrd_struct_t               prh_thrd_struct
#define thrd_id                     prh_thrd_id
#define thrd_data                   prh_thrd_data
#define thrd_self                   prh_thrd_self
#define thrd_main                   prh_thrd_main
#define thrd_main_id                prh_thrd_main_id
#define thrd_main_data              prh_thrd_main_data
#define thrd_get                    prh_thrd_get
#define thrd_init                   prh_thrd_init
#define thrd_inix                   prh_thrd_inix
#define thrd_creatx                 prh_thrd_creatx
#define thrd_start                  prh_thrd_start
#define thrd_starx                  prh_thrd_starx
#define thrd_join                   prh_thrd_join
#define thrd_jall                   prh_thrd_jall
#define thrd_free                   prh_thrd_free
#define thrd_mutex_t                prh_thrd_mutex
#define thrd_mutex_init             prh_thrd_mutex_init
#define thrd_mutex_free             prh_thrd_mutex_free
#define thrd_recursive_mutex_init   prh_thrd_recursive_mutex_init
#define thrd_mutex_lock             prh_thrd_mutex_lock
#define thrd_mutex_unlock           prh_thrd_mutex_unlock
#define thrd_cond_t                 prh_thrd_cond
#define thrd_cond_init              prh_thrd_cond_init
#define thrd_cond_free              prh_thrd_cond_free
#define thrd_cond_lock              prh_thrd_cond_lock
#define thrd_cond_unlock            prh_thrd_cond_unlock
#define thrd_cond_wait              prh_thrd_cond_wait
#define thrd_cond_timedwait         prh_thrd_cond_timedwait
#define thrd_cond_signal            prh_thrd_cond_signal
#define thrd_cond_broadcast         prh_thrd_cond_broadcast
#define thrd_sem_t                  prh_thrd_sem
#define thrd_sem_init               prh_thrd_sem_init
#define thrd_sem_free               prh_thrd_sem_free
#define thrd_sem_wait               prh_thrd_sem_wait
#define thrd_sem_post               prh_thrd_sem_post
#define sleep_cond_init             prh_sleep_cond_init
#define sleep_cond_free             prh_sleep_cond_free
#define thrd_sleep                  prh_thrd_sleep
#define thrd_wakeup                 prh_thrd_wakeup
#endif

#ifdef PRH_THRD_IMPLEMENTATION
#ifndef PRH_THRD_DEBUG
#define PRH_THRD_DEBUG PRH_DEBUG
#endif

#if defined(prh_plat_windows)

#else // WINDOWS end PTHREAD begin
// 线程间除全局内存还共享以下属性，它们对于进程而言是全局的，并非针对某个特定线程：
//
// 1. 进程ID和父进程ID，进程组ID与会话（session）ID，进程凭证（credential）
// 2. 控制终端，打开的文件描述符
// 3. 由 fcntl() 创建的记录锁（record lock）
// 4. 信号处置，间隔定时器和POSIX定时器
// 5. 文件系统的相关信息，例如文件权限掩码、当前工作目录和根目录
// 6. 系统 V（System V）信号量撤销（undo semadj）
// 7. 资源限制（resource limit），资源消耗（getrusage()）
// 8. CPU 时间消耗（times()），nice 值（setpriority() nice()）
//
// 各线程所独有的属性包括：
//
// 1. 线程ID，线程本地存储数据，errno 变量，浮点环境（fenv(3)）
// 2. 信号掩码，备选信号栈（sigaltstack()）
// 3. 实时调度策略和优先级
// 4. CPU 亲和力（affinity，Linux 所特有）
// 5. 能力（capability，Linux 所特有）
// 6. 线程栈，本地变量和函数的调用链接信息
//
// 所有的线程栈均驻留于同一虚拟地址空间，这意味着利用合适的指针，各线程可以在对方栈中
// 相互共享数据。这种方法偶尔也能派上用场，但由于局部变量的状态有效与否依赖于其所驻留
// 栈帧的生命周期，故而需要谨慎处理这一问题。当函数返回时，该函数栈帧所占用的内存区域
// 有可能为后续的函数调用所重用。如果线程终止，那么新线程有可能会对已终止线程的所占用
// 的内存空间重新加以利用。若无法正确处理这一依赖关系，由此产生的程序问题将难以捕获。
//
// 在多线程应用中处理信号，需要小心设计，作为通则一般建议在多线程程序中避免使用信号。
// UNIX 信号模型是基于 UNIX进程模型而设计的，问世比 pthreads 要早几十年。自然而然，
// 信号与线程模型之间存在一些明显的冲突。注意是因为，一方面针对单线程进程要保持传统的
// 信号语义（pthreads不应该改变传统进行的信号语义），与此同时有需要开发出适用于多线程
// 进程环境的新信号模型。信号与线程模型之间的差异意味着，将二者结合使用将会非常复杂，
// 应尽可能加以避免。尽管如此，有的时候还是必须在多线程中处理信号问题。
//
// 1995年POSIX.1c对POSIX线程API进行了标准化，该标准后来为SUSv3所接纳。Pthreads API
// 定义了一系列数据类型，如下所示。SUSv3 并未规定如何实现这些数据类型，可移植的程序应
// 该将其视为透明数据，程序应避免对此类数据类型变量的结构或内容产生任何依赖。 ::
//
//      pthread_t               线程ID
//      pthread_mutex_t         互斥对象
//      pthread_mutexattr_t     互斥属性
//      pthread_cond_t          条件变量
//      pthread_condattr_t      条件变量属性
//      pthread_key_t           线程特有数据的键
//      pthread_once_t          一次性初始化控制上下文
//      pthread_attr_t          线程属性
//
// 启动程序时产生的进程只有单条线程，称之为初始或主线程。调用 pthread_create() 负责
// 创建一个新的线程。新线程通过调用带有参数arg的函数start开始执行，并带有返回值类型
// 为 void*，当经强制转换的整数作为线程start函数的返回值时，必须避免与实现所定义的常
// 量 PTHREAD_CANCELED 值相同，否则线程退出后不能分辨是正常退出还是被取消而退出。参
// 数thread用户获取线程ID，SUSv3明确指出，在新线程开始执行之前，实现无需对thread参数
// 进行赋值，即新线程可能会在pthread_create返回给调用者之前已经开始运行。如新线程需
// 要获取自己的线程ID，则只能使用pthread_self()获取。调用pthread_create()之后，应用
// 程序无法确定接着会调度哪一个线程来使用CPU资源，程序不能隐含认为对特性调度顺序的依
// 赖。可以给线程设置线程属性，如果将attr设置为NULL，那么创建新线程时将使用各种默认属
// 性。
//
//      int pthread_create(
//          pthread_t *thread,
//          const pthread_attr_t *attr,
//          void *(*start)(void *),
//          void *arg);
//
// pthread_exit()函数用于终止调用线程，其返回值可由另一线程通过调用pthread_join()函
// 数来获取。调用pthread_exit相当于在线程start函数中执行return，不同之处在于，可在线
// 程start函数所调用的任意函数中调用pthread_exit()。参数retval执行了线程的返回值，
// retval所指向的内容不应该分配在线程栈中，因为线程终止后，将无法确定线程栈的内容是否
// 有效，例如系统可能会立刻将改进程虚拟内存的这片区域重新分配，供一个新的线程栈使用。
// 处于同样的理由，也不应该在线程栈中分配线程start函数的返回值。 ::
//
//      void pthread_exit(void *retval);
//
// 线程可以以以下方式终止运行：
//
// 1. 线程start函数执行return语句并返回指定值
// 2. 线程调用pthread_exit()
// 3. 调用pthread_cancel()取消线程
// 4. 任意线程调用了exit()，或者主线程执行了return语句，会导致进程中所有线程终止
// 5. 如果主线程调用了pthread_exit()，那么其他线程将继续运行
//
// 进程内部的每个线程都有一个唯一的标识（同一个进程中保证唯一，但不同进程中的线程ID可
// 能重名），称为线程ID。线程ID的用处：不同的pthreads函数利用线程ID来识别要操作的目标
// 线程，这些函数包括pthread_join() pthread_detach() pthread_cancel()
// pthread_kill()等；在一些应用中，以特定线程的线程ID作为动态数据结构的标签，以识别某
// 个数据结构的创建者和操作者。函数pthread_equal()可检查两个线程ID是否相同。因为必须
// 将pthread_t作为一种透明的数据类型对待，所以函数pthread_equal是必须的。Linux将
// pthread_t定义为无符号长整数，但在其他实现中则有可能是一个指针或结构体。SUSv3并未要
// 求将pthread_t实现为一个标量类型，该类型也可以是一个结构体。因此将线程ID强制类型转
// 换成整数值并不具有可移植性。在Linux的线程实现中，线程ID在所有进程中都是唯一的，不过
// 在其他实现中则未必如此，SUSv3特别之处，应用程序若使用线程ID来标识其他进程的线程，其
// 可移植性无法得到保证。此外，在对已终止线程施以pthread_join()，或者在已分离线程退出
// 后（detached thread），实现可以复用该线程的线程ID。 ::
//
//      pthread_t pthread_self(void);
//      int pthread_equal(pthread_t a, pthread_t b);
//
// POSIX线程ID与Linux专用的系统调用gettid()所返回的线程ID并不同。POSIX线程ID由线程
// 库实现和维护。gettid()返回的线程ID是一个内核分配的数字，类似于进程ID。虽然在Linux
// NPTL（Native POSIX Threads Library）实现中，每个POSIX线程都对应一个唯一的内核线
// 程ID，但应用程序一般无需了解内核线程ID，况且如果程序依赖于这一信息也将无法移植。
//
// 连接（joining）已终止的线程，函数pthread_join()等待对应的线程终止，如果线程已经
// 终止，pthread_join()会立即返回。retval可以获取线程return或通过pthread_exit()
// 终止时的返回值。如果前面一个终止的线程已经完成join，再次调用pthread_join将会导致
// 无法预知的行为。例如，相同的线程ID可能在join以后已经为另一个新建线程所重用，在度
// 调用pthread_join()可能等待的这个新线程。若线程并未分离（detached），则必须进行
// join。如果未能完成join，那么线程终止时将产生僵尸线程，与僵尸进程的概念相类似。除了
// 浪费系统资源以外，僵尸线程若积累过多，应用将再也无法创建新的线程。 ::
//
//      int pthread_join(pthread_t thread, void **retval);
//      int pthread_detach(pthread_t thread);
//      pthread_detach(pthread_self());
//
// pthread_join()执行的功能类似于针对进程的waitpid()，不过二者之间存在一些显著差别。
// 线程之间的关系时对等的（peers），进程中的任意线程均可以调用 pthread_join() 与该
// 进程的任何其他线程连接起来。例如如果线程A创建线程B，线程B再创建线程C，那么线程A可
// 以连接线程C，线程C也可以连接线程A。这与进程间的层次关系不同。父进程如果使用fork()
// 创建了子进程，那么它也是唯一能够对子进程调用wait()的进程。调用pthread_create()
// 创建的新线程与发起调用的线程之间，就没有这样的关系。无法“连接任何线程”，而对于进程
// 则可以通过调用waitpid(-1,&status,options)做得这一点，也不能以非阻塞方式进行连接，
// 类似于设置WHOHANG标志的waitpid()。限制pthread_join()只能连接特定线程ID，这样做
// 是“别有用心”的。其用意在于，程序应只能连接它所知道的线程。线程之间并无层次关系，如
// 果听任与任意线程连接的操作发生，那么所谓“任意”线程就可以包括由库函数私自创建的线
// 程，从而带来问题，而后面展示的使用条件变量技术也只运行线程连接它知道的其他线程。结
// 果是，函数库再获取线程返回状态时将不再能与该线程连接，只会一错再错，试图连接一个已
// 经过的线程ID。换言之，连接任意线程的操作与模块化的程序涉及理念背道而驰。
//
// 默认情况下，线程是可连接的（joinable），也就是说当线程退出时，其他线程可以通过调用
// pthread_join()获取其返回状态。有时程序员并不关心线程的返回状态，只是希望再线程终
// 止时能够自动清理并移除。在这种情况下，可以调用pthread_detach()将对应的线程标记为
// 分离（detached）状态。例如以下的代码，一个线程自己可以进行自我分离。一旦线程处于分
// 离状态，就不能再使用pthread_join()来获取其状态，也无法使其重返可连接状态。其他线程
// 调用了exit()或是主线程执行return语句时，即便遭到分离的线程也还是会受到影响。此时，
// 不管线程处于可连接状态还是已分离状态，进程的所有线程会立即终止。换言之，
// pthread_detach()只能控制线程终止之后所发生的事情，而非何时或如何终止线程。一个已经
// 处于分离状态的线程ID，不能再次使用它来调用pthread_join()或pthread_detach()。
//
// 可以使用 pthread_attr_t 设置线程的属性，包括初始的 detach 状态，栈大小和栈地址，
// 以及其他一些设定。其中新创建线程的 detach 状态默认是 PTHREAD_CREATE_JOINABLE。
// 创建线程时，每个线程都有一个属于自己的线程栈，且大小固定。在 Linux/x86-32架构上，
// 除主线程外的所有线程，其栈的缺省大小均为2MB，在一些64位架构下默认尺寸要大一些。为
// 了应对栈的增长，主线程的空间要打出许多。偶尔也需要改变线程栈的大小，再通过线程属性
// 对像创建线程时，调用函数pthread_attr_setstacksize()所设置的线程属性决定了线程栈
// 的大小。而使用与之相关的另一函数pthread_attr_setstack()可以同时控制线程栈的大小
// 和位置，不过设置栈的地址将降低程序的可移植性。更大的线程栈可以容纳大型的自动变量或
// 者深度的嵌套调用，这是改变每个线程栈大小的原因之一。而另一方面应用程序可能希望减少
// 每个线程栈，易变进程可以创建更多的线程。例如在x86-32系统中，用户模式可访问的虚拟地
// 址空间是3GB，而2MB的缺省栈大小则意味着最多只能创建1500各线程，更为准确的最大值还视
// 代码段、数据段、共享函数库等对虚拟内存的消耗量。特定架构的系统上，可采用的线程栈大
// 小最小值可以通过调用sysconf(_SC_THREAD_STACK_MIN)来确定，在Linux/x86-32上的
// NPTL实现中，该调用返回16384。在NPTL线程实现中，如果对线程栈尺寸资源限制
//（RLIMIT_STACK）的设置不同于unlimited，那么创建线程时会以其作为默认值。对该限制的
// 设置必须在运行程序之前，通过执行shell内建命令ulimit -s完成，在C shell下命令
// 为limit stacksize。在主程序中调用setrlimit()来设置限制的办法可能行不通，因为NPTL
// 在调用main()之前的运行时初始化期间就已经确定了默认栈的大小。
//
// pthread_attr_setstacksize() 函数将栈大小属性设置为 stacksize。栈大小属性决定了
// 线程分配的最小栈空间大小（以字节为单位）。pthread_attr_setstacksize() 可能会因以
// 下错误而失败：EINVAL 栈大小小于 PTHREAD_STACK_MIN（16384 字节）；在某些系统中，
// 如果 stacksize 不是系统页面大小的倍数，可能会返回 EINVAL 错误。线程的栈大小在创建
// 线程时就已固定，只有主线程的栈可以动态增长。缺陷说明：截至 glibc 2.8，如果指定的
// stacksize 不是 STACK_ALIGN（在大多数架构上为 16 字节）的倍数，它可能会向下取整，
// 这违反了 POSIX.1 标准。POSIX.1 规定分配的栈空间至少应为 stacksize 字节，而向下
// 取整会小于提供的 stacksize 参数值。
//
// pthread_attr_setstack() 函数将栈地址和栈大小属性设置为stackaddr和stacksize。这
// 些属性指定了线程应使用的栈的位置和大小。stackaddr 应指向调用者分配的大小为
// stacksize 字节的缓冲区的最低可寻址字节。已分配缓冲区的页面应同时具备可读和可写权
// 限。pthread_attr_setstack() 可能会因以下错误而失败：EINVAL：stacksize 小于
// PTHREAD_STACK_MIN（16384 字节）。在某些系统中，如果 stackaddr 或
// stackaddr + stacksize 未正确对齐，也可能会出现此错误。EACCES：POSIX.1 标准还提
// 到，如果 stackaddr 和 stacksize 所描述的栈区域对于调用者而言并非既可读又可写，则
// 会返回此错误。这些函数是为那些必须确保线程栈位于特定位置的应用程序提供的。对于大多
// 数应用程序来说，这并非必要，应避免使用这些函数。如果应用程序只是需要非默认的栈大小，
// 可使用 pthread_attr_setstacksize(3)。当应用程序使用 pthread_attr_setstack()
// 时，它就承担起了分配栈的责任。使用 pthread_attr_setguardsize(3) 设置的任何警戒区
// 大小值都将被忽略。如果认为有必要，应用程序有责任分配一个警戒区（一个或多个受保护的
// 页面，禁止读写）来处理栈溢出的可能性。stackaddr 中指定的地址应正确对齐：为了实现完
// 全的可移植性，应将其对齐到页面边界 sysconf(_SC_PAGESIZE)，使用 posix_memalign(3)
// 分配可能会很有用。可能地，stacksize 也应该是系统页面大小的倍数。如果使用 attr 创建
// 多个线程，那么调用者必须在每次调用 pthread_create(3) 之间更改栈地址属性；否则，这
// 些线程将尝试使用相同的内存区域作为它们的栈，从而导致混乱。
//
// pthread_attr_setguardsize() 函数将警戒区大小属性设置为 guardsize。如果大于 0，
// 那么系统会在线程栈的末尾额外分配至少 guardsize 字节的区域，作为栈的警戒区。如果
// guardsize 为 0，那么不会有警戒区。默认的警戒区大小与系统页面大小相同。如果已经设
// 置了栈地址属性（使用 pthread_attr_setstack() 或 pthread_attr_setstackaddr()），
// 这意味着调用者正在为线程分配栈，那么警戒区大小属性将被忽略（系统不会创建警戒区）：
// 应用程序有责任处理栈溢出问题。也许可以使用 mprotect() 手动在其分配的栈末尾定义一个
// 警戒区。警戒区由受保护的虚拟内存页面组成，以防止读写访问。如果一个线程的栈溢出到警
// 戒区，在大多数硬件架构上，它会收到一个 SIGSEGV 信号，从而得知发生了栈溢出。警戒区
// 从页面边界开始，并且在创建线程时，警戒区大小会在内部向上取整为系统页面大小。将警戒
// 区大小设置为 0 对于创建大量线程且确定不会发生栈溢出的应用程序来说，有助于节省内存。
// 如果线程在栈上分配大型数据结构，选择大于默认大小的警戒区可能对于检测栈溢出是必要的。
// 缺陷说明：截至 glibc 2.8，NPTL 线程实现将警戒区包含在栈大小分配之内，而不是像
// POSIX.1 要求的那样在栈末尾分配额外的空间。如果警戒区大小值太大，导致没有实际栈的空
// 间，这可能会导致 pthread_create(3) 返回 EINVAL 错误。过时的 LinuxThreads 实现则
// 做对了，它在栈末尾分配额外的空间作为警戒区。 ::
//
//      int pthread_attr_init(pthread_attr_t *attr);
//      int pthread_attr_destroy(pthread_attr_t *attr);
//      int pthread_attr_setdetachstate(attr, int detachstate);
//      int pthread_attr_getdetachstate(attr, int *detachstate);
//      int pthread_attr_setstacksize(attr, size_t stacksize);
//      int pthread_attr_getstacksize(attr, size_t *stacksize);
//      int pthread_attr_setstack(attr, void *stackaddr, size_t stacksize);
//      int pthread_attr_getstack(attr, void **stackaddr, size_t *stacksize);
//      int pthread_attr_setguardsize(attr, size_t guardsize);
//      int pthread_attr_getguardsize(attr, size_t *guardsize);
//      int pthread_attr_setscope(attr, int scope);
//      int pthread_attr_getscope(attr, int *scope);
//      int pthread_attr_setinheritsched(attr, int inheritsched);
//      int pthread_attr_getinheritsched(attr, int *inheritsched);
//      int pthread_attr_setschedpolicy(attr, int policy);
//      int pthread_attr_getschedpolicy(attr, int *policy);
//      int pthread_attr_setschedparam(attr, const struct sched_param *param);
//      int pthread_attr_getschedparam(attr, struct sched_param *param);
//
//      detachstate: PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_DETACHED
//      scope: PTHREAD_SCOPE_SYSTEM PTHREAD_SCOPE_PROCESS
//      inheritsched: PTHREAD_INHERIT_SCHED PTHREAD_EXPLICIT_SCHED
//      policy: SCHED_FIFO SCHED_RR SCHED_OTHER
//      struct sched_param { int sched_priority; } 0 ~ 99
//
// pthread_attr_setscope() 设置线程的竞争范围属性，竞争范围定义了一个线程为获取CPU
// 资源而与之竞争的线程集合。其中PTHREAD_SCOPE_SYSTEM表示所有进程中的线程都是对等的，
// 相当于它们基于调度策略和优先级对等的使用内核CPU。而PTHREAD_SCOPE_PROCESS线程只会
// 于进程内同样具有PTHREAD_SCOPE_PROCESS属性的线程竞争资源。POSIX.1要求实现至少支持
// 其中的一种竞争范围，Linux 支持 PTHREAD_SCOPE_SYSTEM，但不支持
// PTHREAD_SCOPE_PROCESS。在支持多种竞争范围的系统上，为了使
// pthread_attr_setscope()所做的参数设置在调用pthread_create()时生效，调用者必须使
// 用pthread_attr_setinheritsched()将属性对象的继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED。PTHREAD_SCOPE_SYSTEM 竞争范围通常表明一个用户空间线程直
// 接绑定到单个内核调度实体。在 Linux 系统中，过时的 LinuxThreads 实现和现代的 NPTL
// 实现都是如此，它们均为 1:1 线程实现（即一个用户空间线程对应一个内核线程）。POSIX.1
// 标准规定，默认的竞争范围由具体实现来定义。
//
// pthread_attr_setinheritsched() 函数将继承调度器属性设置为 inheritsched 指定的
// 值。继承调度器属性决定是从调用线程继承其调度属性，还是从 attr 中获取这些属性。以下
// 调度属性会受到该属性的影响：调度策略pthread_attr_setschedpolicy(3)、调度优先级
// pthread_attr_setschedparam(3) 和竞争范围 pthread_attr_setscope(3)。新初始化的
// 线程属性对象中，继承调度器属性的默认值是 PTHREAD_INHERIT_SCHED。
// pthread_attr_setinheritsched() 可能会因以下错误而失败：EINVAL：inheritsched 值
// 无效。ENOTSUP：POSIX.1 还记录了 pthread_attr_setinheritsched() 可能出现的一个
// 可选错误 ENOTSUP（“尝试将属性设置为不支持的值”）。缺陷说明：截至 glibc 2.8，如果
// 使用 pthread_attr_init(3) 初始化一个线程属性对象，那么该属性对象的调度策略会被设
// 置为 SCHED_OTHER，调度优先级会被设置为 0。然而，如果随后将继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED，那么使用该属性对象创建的线程会错误地从创建线程继承其调度
// 属性。如果在调用 pthread_create(3) 之前，在线程属性对象中显式设置了调度策略或调度
// 优先级属性，这个缺陷就不会出现。
//
// pthread_attr_setschedpolicy() 函数将调度策略属性设置为 policy。支持的值为
// SCHED_FIFO、SCHED_RR 和 SCHED_OTHER，其语义在 sched(7) 中有详细描述。
// 为了使 pthread_attr_setschedpolicy() 设置的策略在调用 pthread_create(3) 时生
// 效，调用者必须使用 pthread_attr_setinheritsched(3) 将继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED。pthread_attr_setschedpolicy() 可能会因以下错误而失败：
// EINVAL：policy 中的值无效。ENOTSUP：POSIX.1 还记录了
// pthread_attr_setschedpolicy() 可能出现的一个可选错误 ENOTSUP（“尝试将属性设置为
// 不支持的值”）。pthread_attr_setschedparam() 函数将调度参数属性设置为 param。
// 调度参数使用以下结构体：struct sched_param { int sched_priority; };关于每种调度
// 策略下允许的调度优先级范围的详细信息，请参阅 sched(7)。为了使
// pthread_attr_setschedparam() 设置的参数在调用 pthread_create(3) 时生效，调用者
// 必须使用 pthread_attr_setinheritsched(3) 将继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED。pthread_attr_setschedparam() 可能会因以下错误而失败：
// EINVAL：param 中指定的优先级对于当前的调度策略没有意义。ENOTSUP：POSIX.1 记录了
// pthread_attr_setschedparam() 可能出现的 ENOTSUP 错误。在 Linux 上，这个值永远
// 不会返回（但具有可移植性和前瞻性的应用程序仍然应该处理这个错误返回值）。
//
// 另外还有线程相关的CPU时间和调度函数： ::
//
// int pthread_getcpuclockid(pthread_t thread, clockid_t *clockid);
// int pthread_setschedprio(pthread_t thread, int prio);
// int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
// int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);
// nice getcpu getpriority setpriority
// sched_getcpu sched_setscheduler sched_setparam sched_getparam
// sched_get_priority_max sched_get_priority_min sched_rr_get_interval
// sched_yield sched_setaffinity sched_getaffinity sched_setattr sched_getattr
//
// 调度策略。调度器是内核组件，它决定下一个由 CPU 执行的可运行线程。每个线程都有一个
// 关联的调度策略和一个静态调度优先级 sched_priority。调度器根据系统中所有线程的调度
// 策略和静态优先级来做出决策。对于采用普通调度策略（SCHED_OTHER、SCHED_IDLE、
// SCHED_BATCH）调度的线程，sched_priority不用于调度决策（必须指定为 0）。采用实时
// 策略（SCHED_FIFO、SCHED_RR）调度的线程程，其 sched_priority 值的范围是 1（低）到
// 99（高）。如数字所示，实时线程的优先级总是高于普通线程。请注意：POSIX.1 要求实现仅
// 为实时策略支持至少 32 个不同的优先级级别，有些系统仅提供这个最小值。可移植的程序应
// 该使用 sched_get_priority_min(2) 和 sched_get_priority_max(2) 来查找特定策略支
// 持的优先级范围。从概念上讲，调度器为每个可能的 sched_priority 值维护一个可运行线
// 程列表。为了确定下一个运行的线程，调度器会查找具有最高静态优先级的非空列表，并选择
// 该列表头部的线程。线程的调度策略决定了它将被插入到具有相同静态优先级的线程列表中的
// 位置，以及它将如何在该列表中移动。所有调度都是抢占式的：如果一个具有更高静态优先级
// 的线程准备好运行，当前正在运行的线程将被抢占，并返回到其静态优先级级别的等待列表中。
// 调度策略仅决定具有相同静态优先级的可运行线程列表内的顺序。
//
// SCHED_FIFO先进先出调度，只能用于静态优先级高于0的情况，这意味着当一个SCHED_FIFO
// 线程变为可运行状态时，它将始终立即抢占任何当前正在运行的SCHED_OTHER、SCHED_BATCH
// 或SCHED_IDLE线程。SCHED_FIFO是一种简单的无时间片划分的调度算法。对于采用
// SCHED_FIFO策略调度的线程，适用以下规则：一个正在运行的SCHED_FIFO线程如果被另一个
// 更高优先级的线程抢占，它将停留在其优先级列表的头部，并在所有更高优先级的线程再次阻
// 塞后立即恢复执行。当一个阻塞的SCHED_FIFO线程变为可运行状态时，它将被插入到其优先级
// 列表的末尾。如果调用 sched_setscheduler(2)、sched_setparam(2)、
// sched_setattr(2)、pthread_setschedparam(3) 或 pthread_setschedprio(3) 更改由
// pid 标识的正在运行或可运行的 SCHED_FIFO 线程的优先级，对该线程在列表中位置的影响
// 取决于线程优先级的变化方向：如果线程的优先级提高，它将被放置在其新优先级列表的尾部。
// 因此，它可能会抢占当前正在运行的具有相同优先级的线程。如果线程的优先级不变，它在运行
// 列表中的位置不变。如果线程的优先级降低，它将被放置在其新优先级列表的头部。根据
// POSIX.1 - 2008，使用除 pthread_setschedprio(3) 之外的任何机制更改线程的优先级
//（或策略），应导致该线程被放置在其优先级列表的末尾。调用 sched_yield(2) 的线程将被
// 放置在列表的末尾。没有其他事件会使采用 SCHED_FIFO 策略调度的线程在具有相同静态优先
// 级的可运行线程等待列表中移动。一个 SCHED_FIFO 线程会一直运行，直到它因 I/O 请求而
// 阻塞、被更高优先级的线程抢占，或者调用 sched_yield(2)。
//
// SCHED_RR 轮转调度是 SCHED_FIFO 的简单增强。上述关于SCHED_FIFO的所有描述也适用于
// SCHED_RR，不同之处在于每个线程只允许运行最长一个时间片。如果一个 SCHED_RR 线程已
// 经运行了等于或超过时间片的时间段，它将被放置在其优先级列表的末尾。一个被更高优先级
// 线程抢占并随后作为运行线程恢复执行的 SCHED_RR 线程将完成其轮转时间片的未使用部分。
// 可以使用 sched_rr_get_interval(2) 来获取时间片的长度。
//
// SCHED_OTHER 默认的 Linux 分时调度。SCHED_OTHER 只能用于静态优先级为 0 的情况
//（即采用实时策略的线程的优先级总是高于 SCHED_OTHER 线程）。SCHED_OTHER 是标准的
// Linux 分时调度器，用于所有不需要特殊实时机制的线程。要运行的线程是从静态优先级为
// 0 的列表中根据动态优先级选择的，该动态优先级仅在这个列表内确定。动态优先级基于
// nice 值，并且对于线程准备运行但被调度器拒绝运行的每个时间片，动态优先级都会增加。
// 这确保了所有 SCHED_OTHER 线程的公平进展。在 Linux 内核源代码中，SCHED_OTHER 策
// 略实际上被命名为 SCHED_NORMAL。
//
// SCHED_BATCH 批量调度（从 Linux 2.6.16 版本开始支持）仅能在静态优先级为 0 的情况
// 下使用。该策略与 SCHED_OTHER 类似，都是根据线程的动态优先级（基于 nice 值）来调
// 度线程。不同之处在于，此策略会让调度器始终假定该线程是 CPU 密集型的。因此，调度器
// 在唤醒行为方面会施加一个小的调度惩罚，使得该线程在调度决策中略微处于劣势。这个策略
// 适用于非交互式的工作负载，且这些工作负载不想降低其 nice 值；同时也适用于那些希望
// 采用确定性调度策略，避免交互性导致额外抢占（在工作负载的任务之间）的工作负载。
//
// SCHED_IDLE：极低优先级作业调度（从 Linux 2.6.23 版本开始支持）仅能在静态优先级为
// 0 的情况下使用；进程的 nice 值对此策略没有影响。此策略旨在以极低的优先级运行作业，
// 甚至比采用 SCHED_OTHER 或 SCHED_BATCH 策略且 nice 值为 +19 的优先级还要低。
//
// nice 值是一个属性，可用于影响 CPU 调度器在调度决策中对某个进程的偏好或不偏好。它
// 会影响 SCHED_OTHER 和 SCHED_BATCH 线程的调度。可以使用 nice(2)、setpriority(2)
// 或 sched_setattr(2) 来修改 nice 值。根据 POSIX.1，nice 值是每个进程的属性；也
// 就是说，一个进程中的线程应该共享一个 nice 值。然而，在 Linux 上，nice 值是每个线
// 程的属性：同一进程中的不同线程可能有不同的 nice 值。nice 值的范围在不同的 UNIX
// 系统中有所不同。在现代 Linux 上，范围是 -20（高优先级）到 +19（低优先级）。在其他
// 一些系统上，范围是 -20 到 20。非常早期的 Linux 内核（Linux 2.0 之前）的范围是负
// 无穷到 15。nice 值对 SCHED_OTHER 进程相对调度的影响程度在不同的 UNIX 系统和不同
// 的 Linux 内核版本中也有所不同。随着 Linux 2.6.23 中 CFS 调度器的出现，Linux 采
// 用了一种算法，使得 nice 值的相对差异产生更强的影响。在当前实现中，两个进程的 nice
// 值每相差一个单位，调度器对优先级较高的进程的偏好程度就会增加 1.25 倍。这使得非常
// 低的 nice 值（+19）在系统上有任何其他更高优先级负载时，确实为进程提供很少的 CPU
// 资源，而高 nice 值（-20）则为需要的应用程序（例如，一些音频应用程序）提供大部分
// CPU 资源。在 Linux 上，RLIMIT_NICE 资源限制可用于定义非特权进程的 nice 值可以提
// 高的上限。
//
// nice 值与组调度。当调度非实时进程（即采用 SCHED_OTHER、SCHED_BATCH 和
// SCHED_IDLE 策略调度的进程）时，如果内核配置了 CONFIG_FAIR_GROUP_SCHED 选项（通
// 常都是如此），CFS 调度器会采用一种称为 “组调度” 的技术。在组调度模式下，线程会以
// “任务组” 为单位进行调度。任务组之间存在层次关系，其根节点是系统中的初始任务组，也
// 就是 “根任务组”。任务组在以下情况下形成：一个 CPU 控制组（cgroup）中的所有线程会
// 组成一个任务组。该任务组的父任务组是对应父 cgroup 的任务组。如果启用了自动分组功
// 能，那么所有（隐式地）被划分到同一个自动组（即通过 setsid(2) 创建的同一个会话）中
// 的线程会组成一个任务组。因此，每个新的自动组都是一个独立的任务组。根任务组是所有这
// 些自动组的父任务组。如果启用了自动分组功能，根任务组则由根 CPU cgroup 中未被隐式
// 划分到新自动组的所有进程组成。如果禁用了自动分组功能，根任务组则由根 CPU cgroup 中
// 的所有进程组成。如果禁用了组调度（即内核配置时未开启 CONFIG_FAIR_GROUP_SCHED），
// 那么系统中的所有进程理论上会被划分到同一个任务组中。在组调度模式下，一个线程的nice
// 值仅在与同一任务组中的其他线程进行调度决策时才会产生影响。这与 UNIX 系统中 nice
// 值的传统语义相比，会产生一些令人意外的结果。特别是，如果启用了自动分组功能（这在各
// 种发行版中是默认设置），那么对某个进程使用 setpriority(2) 或 nice(1) 命令，其影
// 响仅体现在与同一会话（通常是同一个终端窗口）中执行的其他线程的调度关系上。相反，对
// 于分别处于不同会话（例如不同的终端窗口，每个窗口中的作业都与不同的自动组相关联）中
// 的两个 CPU 密集型进程，修改其中一个会话中进程的 nice 值，在调度器的决策中，对另一
// 个会话中的进程没有任何影响。这里有一个可能有用的解决方法，即使用如下命令修改终端会
// 话中所有进程的自动组 nice 值：echo 10 > /proc/self/autogroup。
//
// 线程取消机制。在通常情况下，程序中的多个线程会并发执行，每个线程各司其职直至其决意
// 退出，随即会调用函数pthread_exit()或者从线程启动函数中返回。有时候需要将一个线程
// 取消，即向线程发送一个请求要求其立即退出。比如一组线程正在执行一个运算一旦某个线程
// 检测到错误发生需要其他线程退出，取消线程的功能这时就派上用场。还有一种情况，一个由
// 图形界面驱动的应用程序可能会提供一个“取消”按钮，以便用户可以终止后台某个线程正在执
// 行的任务。这种情况下，主线程（控制图形界面）需要请求后台线程退出。 ::
//
//      int pthread_cancel(pthread_t thread);
//      int pthread_setcancelstate(int state, int *oldstate);
//      int pthread_setcanceltype(int type, int *oldtype);
//      void pthread_testcancel(void);
//      void pthread_cleanup_push(void *(routine)(void *), void *arg);
//      void pthread_cleanup_pop(int execute);
//
//      state:  PTHREAD_CANCEL_DISABLE PTHREAD_CANCEL_ENABLE
//      type:   PTHREAD_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_DEFERED
//
// 发出取消请求后，函数pthread_cancel()会立即返回，不会等待目标线程的退出。准确地说，
// 目标线程会发生什么，何时发生，这都取决于线程取消状态（state）和类型（type）。函数
// pthread_setcancelstate() 和 pthread_setcanceltype() 会设定标志，允许线程对取消
// 请求的响应过程加以控制。函数 pthread_setcancelstate() 会将调用线程的取消性状态置
// 为参数state所给定的值。其中 PTHREAD_CANCEL_DISABLE 设置线程不可取消，如果此类线
// 程收到取消请求则会挂起取消请求，直至将取消状态置为启用。PTHREAD_CANCEL_ENABLE
// 设置线程可以取消，这是新建线程取消性状态的默认值。而参数 oldstate 可以获取前一状态
// 的值，如果对前一状态没有兴趣，Linux 允许将oldstate置为NULL。不过SUSv3并没有规范
// 这一特性，所以要保证应用的可移植性，就不能依赖这一特性，应该总是为oldstate设置一个
// 非NULL的值。如果线程执行的代码片段需要不间断地一气呵成，那么临时屏蔽线程的取消行状
// 态就变得很有必要。
//
// 如果线程的取消行状态为启用状态，那么对取消请求的处理则取决于线程的取消性类型，该类
// 型可以通过调用函数 pthread_setcanceltype() 进行设定。其中
// PTHREAD_CANCEL_ASYNCHRONOUS 可能会在任何时点（也许是立即取消，但不一定）取消线程。
// 异步取消的应用场景很少，后面单独介绍。另一个值 PTHREAD_CANCEL_DEFERED 将取消请求
// 保持在挂起状态，直至到达取消点（cancellation point），这也是新建线程的缺省类型。
// 当某线程调用fork()时，子进程会继承调用线程的取消性类型及状态。而当某线程调用exec()
// 时，会将新程序主线程的取消性类型及状态分别重置为 PTHREAD_CANCEL_ENABLE 和
// PTHREAD_CANCEL_DEFERRED。
//
// 若将线程的取消性状态和类型分别置为启用和延迟，仅当线程抵达某个取消点时，取消请求才
// 会起作用。取消点即是对由实现定义的一组函数之一加以调用。SUSv3规定，实现若提供了下
// 表所列的函数，则这些函数必须是取消点。其中的大部分函数都有能力将线程无限期地阻塞起
// 来。 ::
//
//      accept aio_suspend clock_nanosleep close connect creat fcntl(F_SETLKW)
//      fsync fdatasync getmsg getpmsg lockf(F_LOCK) mq_receive mq_send
//      mq_timedreceive mq_timedsend msgrcv msgsnd msync nanosleep open
//      pause poll pread pselect pthread_cond_timedwait pthread_cond_wait
//      pthread_join pthread_testcancel putmsg putpmsg pwrite read readv
//      recv recvfrom recvmsg select sem_timedwait sem_wait send sendmsg
//      sendto sigpause sigsuspend sigtimedwait sigwait sigwaitinfo sleep
//      system tcdrain usleep wait waitid waitpid write writev
//
// 除这些函数外，SUSv3还指定了大量函数，系统实现可以将其定义为取消点。其中包括 stdio
// dlopen syslog ntfw popen semop unlink，以及从诸如 utmp 之类的系统文件中获取信
// 息的各种函数。可移植应用程序必须正确处理这一情况：线程在调用这些函数时有可能遭到取
// 消。SUSv3规定，处理上述两组必须或可能是取消点的函数之外，不得将标准中的任何其他函数
// 视为取消点，即调用这些函数不会招致线程取消，可移植程序加以处理。SUSv4在必须的可取消
// 函数列表中增加了openat，并移除了函数sigpause和usleep。系统实现可随意将标准并未规范
// 的其他函数标记为取消点。任何可能造成阻塞的函数，有可能是因为需要访问文件，都是取消
// 点的理想候选对象。处于这一理由，glibc将其中的许多非标准函数标记为取消点。
//
// 线程一旦收到取消请求，且启用了取消状态并将类型置为延迟，则会在下次抵达取消点时终止。
// 如果该线程尚未分离（detached），那么为防止其编程僵尸线程，必须由其他线程对其进行连
// 接（join）。连接之后，返回至函数 pthread_join() 中第二个参数的值将是一个特殊值，
// PTHREAD_CANCED。
//
// 如果一个需要取消的线程，其执行的代码并没有调用存在取消点的函数，例如计算密集型循环，
// 这时线程永远也不会响应取消请求。函数 pthread_testcancel() 的目的很简单，就是产生
// 一个取消点。线程如果已有处于挂起状态的取消请求，那么只要调用该函数，线程就会随之终
// 止。当线程执行的代码未包含取消点时，可以周期性地调用 pthread_testcancel()，以确保
// 对其他线程向其发送的取消请求做出及时响应。
//
// 一旦有取消请求处于挂起状态，线程在执行到取消点时如果只是草草收场，这会将共享变量以及
// pthreads对象，例如互斥量置于一种不一致状态，可能导致进程中其他线程产生错误结果、死
// 锁，甚至造成程序崩溃。为规避这一问题，线程可以设置一个或多个清理函数，当线程遭到取消
// 时会自动运行这些函数，在线程终止之前可执行诸如修改全局变量，解锁互斥量等动作。每个
// 线程都可以拥有一个清理函数栈，当线程遭取消时，会该栈自顶向下依次执行清理函数，首先
// 会执行最近设置的函数，接着时次新的函数，依次类推。当执行完所以清理函数后，线程终止。
// 函数 pthread_cleanup_push() 和 pthread_cleanup_pop() 分别负责向调用线程的清理
// 函数栈添加和移除清理函数。push 时还传入了一个 arg 参数，这个参数在清理函数调用时当
// 作参数传给清理函数。 **注意**，若线程因调用 pthread_exit() 而终止，则也会自动执行
// 尚未从清理函数栈中弹出的清理函数，线程正常返回（return）则不会执行清理函数。尽管这
// 里把 pthread_cleanup_push() 和 pthread_cleanup_pop() 描述为函数，SUSv3 却允许
// 将它们实现为宏，可展开为分别由 { 和 } 所包裹的语句序列。并非所有的 UNIX 都这样做，
// 不过包括 LINUX 在内的很多系统都是使用宏来实现的。由于这个限制必须在线程相同的作用
// 域中已匹配对的形式调用这两个函数，pthread_cleanup_push 的宏定义可以包含字符 {，
// 这种情况下，在 pthread_cleanup_pop 的定义中会包含对应的匹配字符 }。
//
// 如果设定线程为可异步取消时（PTHREAD_CANCEL_ASYNCHRONOUS），可以在任何时点将其取
// 消（即执行任何机器指令时），取消动作不会拖延到下一个取消点才执行。异步取消的问题在
// 于，尽管清理函数依然会得以执行，但处理函数却无从得知线程的具体状态。清理函数无法知
// 道将在哪里发生取消动作，或者准确的说，清理函数不清除需要执行哪些清理步骤。此外，由
// 于可以在任意点都可能被取消，线程很可能还处于某种未知状态，例如一个动态分配的函数本
// 来是需要在清理函数中清理的，但可能在执行malloc()期间就被取消了，这极有可能造成后续
// 的混乱。作为一般性原则，可异步取消的线程不应该分配任何资源，也不能获取互斥量或锁，
// 这导致大量库函数无法使用，其中就包括pthreads函数的大部分。SUSv3中有3各例外是
// pthread_cancel() pthread_setcancelstate() pthread_setcanceltype()，规范明确
// 要求将它们实现为异步取消安全。换言之，异步取消功能鲜有应用场景，其中之一是取消在执
// 行计算密集型循环的线程。
//
// 综上所述，函数 pthread_cancel() 允许某线程向另外一个线程发送取消请求，要求目的线
// 程终止。目标线程如何响应，取决于其取消性状态和类型。如果禁用线程的取消性状态，那么
// 会挂起（pending）取消请求，直至将线程的取消性状态置为启用。如果启用取消状态，那么
// 线程何时响应则依赖于取消性类型。若类型为延迟取消，则在线程下一次调用某个取消点函数
// 时进行取消。如果为异步取消类型，取消动作随时可能发送。线程可以设置一个清理函数栈，
// 其中的清理函数属于有开发人员定义的函数，当线程遭到取消时，会自动调用这些函数以执行
// 清理工作，例如恢复共享变量状态或解锁互斥量。
//
// 线程实现模型，实现线程API的三种不同模型，其实现差异主要集中在线程如何与内核调度实体
// （KSE, Kernel Scheduling Entity）相映射。KSE是内核分配CPU以及其他系统资源的单位，
// 在早于线程而出现的传统UNIX中，KSE等同于进程。
//
// 多对一（M:1）实现，用户级线程。在M:1线程实现中，关乎线程创建、调度以及同步的所有细
// 节全部由进程内用户空间的线程库来处理。对于进程中存在的多个线程，内核一无所知。M:1
// 实现的优势不多，其中最大的有点在于，许多线程操作速度都很快，因为无需切换到内核模式。
// 此外由于线程库无需内核支持，所以M:1实现在系统间的移植相对要容易一些。不过M:1实现也
// 存在一些严重缺陷：当一线程发起系统调用比如read()时，控制由用户空间的线程库转交给内
// 核，这就意味着如果read()调用遭到阻塞，那么所有的线程都会被阻塞。内核无法调度进程中
// 的这些线程，因为内核并不知道进程中存在这些线程，也就无法在多处理器平台上将各线程调
// 度给不同的处理器。另外，也不可能将一进程中某线程的优先级调整为高于其他进程的中的线
// 程，这是没有一样的，因为对线程的调度完全在进程中处理。
//
// 一对一（1:1）实现，内核级线程。在1:1线程实现中，每一线程映射为以恶单独的KSE。内核
// 分别对每个线程做调度处理。线程同步操作通过内核系统调用实现。1:1实现消除了M:1实现的
// 种种弊端。遭阻塞的系统调用不会导致进程的所有线程被阻塞，在多处理硬件平台上，内核还
// 可以将进程中的多个线程调度到不同的CPU上。不过，因为需要切换到内核模式，所以诸如线程
// 创建、上下文切换以及同步操作就需要慢一些。另外，为每个线程分别维护一个KSE也需要开
// 销，如果应用程序包含大量线程，则可能对内核调度造成严重的负担，降低系统的整体性能。
// 尽管有这些缺点，1:1实现通常更胜于M:1实现，LinuxThreads和NPTL都采用1:1模型。在NPTL
// 的开发期间，为了使得包含数千计数的进程得以高效运行，投入了巨大的努力。对内核调度进行
// 了重写并设计了新的线程实现。
//
// 多对多（M:N）实现，两级模型。M:N实现旨在结合1:1和M:1模型的优点，避免二者的缺点。在
// M:N模型中，每个进程都可拥有多个与之相关的KSE，并且也可以把多个线程映射到一个KSE。
// 这种设计运行内核将同一应用的线程调度到不同的CPU上运行，同时也解决了随线程数量大而放
// 大的性能问题。M:N模型的最大问题是过于复杂，线程调度任务由内核及用户空间的线程库共同
// 承担，二者之间势必要进行分工协作和信息交换。在M:N模型下，按照SUSv3标准要求来管理信
// 号也极为复杂。最初增考虑采用M:N模型来实现NPTL线程库，但若要保证Linux调度器即使在处
// 理大量KSE的情况下也能应对自如，则需要对内核所作的改动范围过大，可能也没有必要，故而
// 否决了这一方案。
//
// 针对pthreads API，Linux下由两种实现：LinuxThreads 这是最初的Linux线程实现，NPTL
// （Native POSIX Threads Library）这是Linux线程实现的现代版，以取代LinuxThreads。
// NPTL的性能优于LinuxThreads，也更符合SUSv3的pthreads标准。对NPTL的支持需要修改内
// 核，这始于Linux 2.6。值得强调的是，LinuxThreads实现已经过时，并且glibc从2.4版本
// 开始也已不再支持它，所有新的线程库开发都基于NPTL。

#if PRH_THRD_DEBUG && defined(prh_impl_pthread_getattr)
void prh_impl_print_thrd_info(prh_thrd *thrd) {
    pthread_t tid = pthread_self();
    void *stack_addr = prh_null;
    size_t stack_size = 0, guard_size = 0;
    pthread_attr_t attr;
    // For pthread_attr_get_np() attr should be initialized prior to the call
    // by using pthread_attr_init(3). pthread_getattr_np() does this
    // automatically.
#if defined(prh_impl_pthread_attr_get_np)
    prh_zeroret(pthread_attr_init(&attr));
#endif
    prh_zeroret(prh_impl_pthread_getattr(tid, &attr));
    prh_zeroret(pthread_attr_getstack(&attr, &stack_addr, &stack_size));
    prh_zeroret(pthread_attr_getguardsize(&attr, &guard_size));
    prh_zeroret(pthread_attr_destroy(&attr));
    printf("[thrd %02d] stack %p %d-byte guard %d-byte\n",
        prh_thrd_id(thrd), stack_addr, (int)stack_size, (int)guard_size);
}
#else
#define prh_impl_print_thrd_info(thrd)
#endif

prh_thread_local prh_thrd *PRH_IMPL_THRD = prh_null;

prh_thrd *prh_thrd_self(void) {
    return PRH_IMPL_THRD;
}

#define prh_impl_thrd_head_size sizeof(prh_thrd)
#define prh_impl_thrd_extra_len sizeof(prh_u32)

prh_static_assert(prh_impl_thrd_head_size % 8 == 0);

int prh_impl_thrd_size(int thrdudsize) {
    assert(thrdudsize >= 0);
    return (int)prh_round_cache_line_size(prh_impl_thrd_head_size + thrdudsize);
}

int prh_impl_thrd_struct_size(int maxthrds) {
    assert(maxthrds >= 0);
    return (int)(sizeof(prh_thrd_struct) + sizeof(void *) * maxthrds);
}

prh_thrd *prh_impl_thrd_create(int thrd_id, int thrdudsize) {
    assert(thrdudsize >= 0);
    int thrd_size = prh_impl_thrd_size(thrdudsize);
    prh_thrd *thrd = prh_cache_line_aligned_malloc(thrd_size);
    memset(thrd, 0, thrd_size);
    thrd->userdata = thrd + 1;
    thrd->thrd_id = thrd_id;
    return thrd;
}

prh_inline prh_thrd *prh_impl_data_to_thrd(void *userdata) {
    return (prh_thrd *)userdata - 1;
}

prh_thrd_struct *prh_thrd_inix(int start_id, int maxthrds, int mainudsize) {
    prh_thrd_struct *s = prh_calloc(prh_impl_thrd_struct_size(maxthrds));
    prh_thrd *main = prh_impl_thrd_create(start_id, mainudsize);
    main->impl_tid_ = (prh_unt)pthread_self();
    s->maxthrds = maxthrds;
    s->main = main;
    PRH_IMPL_THRD = main;
#if PRH_THRD_DEBUG
    prh_impl_print_thrd_info(main);
#endif
    return s;
}

prh_thrd_struct *prh_thrd_init(int start_id, int maxthrds, void *userdata) {
    prh_thrd_struct *s = prh_thrd_inix(start_id, maxthrds, 0);
    prh_thrd *main = prh_thrd_main(s);
    main->userdata = userdata;
    return s;
}

typedef struct {
    prh_thrd *thrd;
    prh_thrdproc_t proc;
    prh_thrd_sem *sync;
} prh_impl_thrd_para_t;

static void *prh_impl_thrd_start_proc(void *param) {
    prh_impl_thrd_para_t *p = (prh_impl_thrd_para_t *)param;
    prh_thrd *thrd = PRH_IMPL_THRD = p->thrd;
    prh_thrdproc_t proc = p->proc;
#if PRH_THRD_DEBUG
    prh_impl_print_thrd_info(thrd);
#endif
    prh_thrd_sem_post(p->sync, 1);
    return (void *)proc(thrd);
}

int prh_impl_thread_stack_size(long stacksize) {
    if (stacksize <= 0) return 0;
    // https://www.man7.org/linux/man-pages/man3/sysconf.3.html
    // https://www.man7.org/linux/man-pages/man3/sysconf.3p.html
    // 从sysconf获得的值是系统配置常量，在进程的生命周期内不会改变。
    // _SC_PAGESIZE: Size of a page in bytes. Must not be less than 1.
    // _SC_THREAD_STACK_MIN: minimal pthread stack size (16384B/16KB)
    long pagesize = sysconf(_SC_PAGESIZE);
    long minsize = sysconf(_SC_THREAD_STACK_MIN);
    if (minsize > 0 && stacksize < minsize) {
        stacksize = minsize;
    }
    if (pagesize > 0) {
        long times = (stacksize + pagesize - 1) / pagesize;
        stacksize = pagesize * times;
    }
    return prh_round_16_byte(stacksize); // stack align 16-byte
}

void prh_impl_thrd_start(prh_thrd *thrd, prh_thrdproc_t proc, int stacksize) {
    prh_impl_thrd_para_t para = {thrd, proc};
    pthread_t *tid = (pthread_t *)&thrd->impl_tid_;
    pthread_attr_t *attr_ptr = prh_null;
    pthread_attr_t attr;
    int n;

    stacksize = prh_impl_thread_stack_size(stacksize);
    if (stacksize > 0) {
        attr_ptr = &attr;
        prh_zeroret(pthread_attr_init(attr_ptr));
        n = pthread_attr_setstacksize(attr_ptr, stacksize);
        prh_abort_if(n);
    }

    para.sync = prh_thrd_sem_init();
    n = pthread_create(tid, attr_ptr, prh_impl_thrd_start_proc, &para);
    prh_abort_if(n) else {
        prh_thrd_sem_wait(para.sync); // 如果wait之前新线程已经post，这里会直接返回，否则会等待新线程
    }

    if (attr_ptr) {
        prh_zeroret(pthread_attr_destroy(attr_ptr));
    }

    prh_thrd_sem_free(&para.sync);
}

prh_thrd *prh_impl_create_set(prh_thrd_struct *s, int thrdudsize) {
    assert(s->thrd_cnt >= 0 && s->thrd_cnt < s->maxthrds);
    int thrd_index = ++s->thrd_cnt;
    prh_thrd *thrd = prh_impl_thrd_create(prh_thrd_main_id(s) + thrd_index, thrdudsize);
    prh_thrd_list_begin(s)[thrd_index] = thrd;
    return thrd;
}

void *prh_thrd_creatx(prh_thrd_struct *s, int thrdudsize) {
    prh_thrd *thrd = prh_impl_create_set(s, thrdudsize);
    return thrd->userdata;
}

int prh_thrd_index(prh_thrd_struct *s, prh_thrd *thrd) {
    return thrd->thrd_id - s->main->thrd_id;
}

int prh_thrd_starx(prh_thrd_struct *s, void *thrd_create_data, prh_thrdproc_t proc, int stacksize) {
    prh_thrd *thrd = prh_impl_data_to_thrd(thrd_create_data);
    prh_impl_thrd_start(thrd, proc, stacksize);
    return prh_thrd_index(s, thrd);
}

int prh_thrd_start(prh_thrd_struct *s, prh_thrdproc_t proc, int stacksize, void *userdata) {
    prh_thrd *thrd = prh_impl_create_set(s, 0);
    thrd->userdata = userdata;
    prh_impl_thrd_start(thrd, proc, stacksize);
    return prh_thrd_index(s, thrd);
}

void prh_impl_thrd_join(prh_thrd **thrd_list, int thrd_index, prh_thrdfree_t udatafree) {
    prh_thrd *thrd = thrd_list[thrd_index];
    if (thrd == prh_null) return;
    void *retv = prh_null;
    pthread_t *tid_ptr = (pthread_t *)&thrd->impl_tid_;
    int n = pthread_join(*tid_ptr, &retv);
    if (n != 0) {
        prh_prerr(n);
    } else if ((prh_unt)retv != 0) { // -1 is PTHREAD_CANCELED
        prh_prerr((int)(prh_unt)retv);
    }
#if PRH_THRD_DEBUG
    if ((prh_unt)retv == (prh_unt)PTHREAD_CANCELED) {
        printf("[thrd %02d] canceled join\n", prh_thrd_id(thrd));
    } else {
        printf("[thrd %02d] join retval %d\n", prh_thrd_id(thrd), (int)(prh_unt)retv);
    }
#endif
    if (udatafree) {
        udatafree(prh_thrd_data(thrd), thrd_index);
    }
    prh_aligned_free(thrd);
    thrd_list[thrd_index] = prh_null;
}

void prh_thrd_join(prh_thrd_struct *s, int thrd_index, prh_thrdfree_t udatafree) {
    assert(thrd_index > 0 && thrd_index <= s->maxthrds);
    prh_impl_thrd_join(prh_thrd_list_begin(s), thrd_index, udatafree);
}

void prh_thrd_free(prh_thrd_struct **main, prh_thrdfree_t udatafree) {
    prh_thrd_struct *s = *main;
    if (s == prh_null) return;
    prh_thrd *main_thrd = s->main;
    if (main_thrd) {
        if (udatafree) {
            udatafree(prh_thrd_data(main_thrd), 0);
        }
        prh_aligned_free(main_thrd);
        s->main = prh_null;
    }
    prh_free(s);
    *main = prh_null;
}

void prh_thrd_join_except_main(prh_thrd_struct *s, prh_thrdfree_t udatafree) {
    prh_thrd **thrd_list = prh_thrd_list_begin(s);
    for (int i = 1; i <= s->maxthrds; i += 1) {
        prh_impl_thrd_join(thrd_list, i, udatafree);
    }
}

void prh_thrd_jall(prh_thrd_struct **main, prh_thrdfree_t udatafree) {
    prh_thrd_struct *s = *main;
    if (s == prh_null) return;
    prh_thrd **thrd_list = prh_thrd_list_begin(s);
    for (int i = 1; i <= s->maxthrds; i += 1) {
        prh_impl_thrd_join(thrd_list, i, udatafree);
    }
    prh_thrd_free(main, udatafree);
}

// 互斥量类型，PTHREAD_MUTEX_NORMAL 不具死锁自检功能，线程加锁已经由自己锁定的互斥量
// 会发生死锁，线程解锁未锁定的或由其他线程锁定的互斥量会导致不确定的结果，但在Linux上
// 对这类互斥量的这两种操作都会成功。
//
// PTHREAD_MUTEX_ERRORCHECK 会对上述情况进行错误检查并返回相关错误，这类互斥量运行起
// 来比一般类型要慢，不过可将其作为调试工具，已发现程序在哪里违反了互斥量使用的基本原
// 则。
//
// PTHREAD_MUTEX_RECURSIVE 递归互斥量维护由一个锁计数器，同一线程重复锁定会增加锁计
// 数，当解锁时会递减计数只有当锁计数器值降至0时才会释放该互斥量，解锁时如果互斥量处于
// 未锁定状态或已由其他线程锁定，则操作会失败。
//
// 如果互斥量不设置pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST)，当线
// 程在解锁之前异常退出，互斥锁将保持锁定状态导致死锁。
//
// 条件变量总是结合互斥量使用，条件变量就共享变量的状态改变发出通知，而互斥量则提供该
// 共享变量访问的互斥。

struct prh_thrd_mutex {
    pthread_mutex_t mutex;
};

struct prh_thrd_cond {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    prh_int wakeup_semaphore;
};

void prh_impl_pthread_mutex_init(pthread_mutex_t *mutex) {
#if PRH_THRD_DEBUG
    pthread_mutexattr_t attr;
    prh_zeroret(pthread_mutexattr_init(&attr));
    prh_zeroret(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK));
    prh_zeroret(pthread_mutex_init(mutex, &attr));
    prh_zeroret(pthread_mutexattr_destroy(&attr));
#else
    prh_zeroret(pthread_mutex_init(mutex, prh_null));
#endif
}

prh_thrd_mutex *prh_thrd_mutex_init(void) {
    prh_thrd_mutex *p = prh_malloc(sizeof(prh_thrd_mutex));
    prh_impl_pthread_mutex_init(&p->mutex);
    return p;
}

prh_thrd_mutex *prh_thrd_recursive_mutex_init(void) {
    prh_thrd_mutex *p = prh_malloc(sizeof(prh_thrd_mutex));
    pthread_mutexattr_t attr;
    prh_zeroret(pthread_mutexattr_init(&attr));
    prh_zeroret(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
    prh_zeroret(pthread_mutex_init(&p->mutex, &attr));
    prh_zeroret(pthread_mutexattr_destroy(&attr));
    return p;
}

void prh_thrd_mutex_free(prh_thrd_mutex **mtx) {
    prh_thrd_mutex *p = *mtx;
    if (p == prh_null) return;
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
    prh_free(p);
    *mtx = prh_null;
}

void prh_impl_thrd_mutex_init(prh_thrd_mutex *p) {
    prh_impl_pthread_mutex_init(&p->mutex);
}

void prh_impl_thrd_mutex_free(prh_thrd_mutex *p) {
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
}

void prh_thrd_mutex_lock(prh_thrd_mutex *p) {
    prh_zeroret(pthread_mutex_lock(&p->mutex));
}

void prh_thrd_mutex_unlock(prh_thrd_mutex *p) {
    prh_zeroret(pthread_mutex_unlock(&p->mutex));
}

// It is advised that an application should not use a PTHREAD_MUTEX_RECURSIVE
// mutex with condition variables because the implicit unlock performed for
// a pthread_cond_timedwait() or pthread_cond_wait() may not actually release
// the mutex (if it had been locked multiple times). If this happens, no other
// thread can satisfy the condition of the predicate.
prh_thrd_cond *prh_thrd_cond_init(void) {
    prh_thrd_cond *p = prh_malloc(sizeof(prh_thrd_cond));
    prh_zeroret(pthread_cond_init(&p->cond, prh_null));
    prh_impl_pthread_mutex_init(&p->mutex);
    p->wakeup_semaphore = 0;
    return p;
}

void prh_thrd_cond_free(prh_thrd_cond **cond) {
    // 仅当没有任何线程等待条件变量，将其销毁才是安全的，经销毁的条件变量之后可以调用
    // pthread_cond_init() 对其进行重新初始化。
    prh_thrd_cond *p = *cond;
    if (p == prh_null) return;
    *cond = prh_null;
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
    prh_zeroret(pthread_cond_destroy(&p->cond));
    prh_free(p);
}

void prh_impl_thrd_cond_init(prh_thrd_cond *p) {
    prh_zeroret(pthread_cond_init(&p->cond, prh_null));
    prh_impl_pthread_mutex_init(&p->mutex);
    p->wakeup_semaphore = 0;
}

void prh_impl_thrd_cond_free(prh_thrd_cond *p) {
    // 仅当没有任何线程等待条件变量，将其销毁才是安全的，经销毁的条件变量之后可以调用
    // pthread_cond_init() 对其进行重新初始化。
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
    prh_zeroret(pthread_cond_destroy(&p->cond));
}

// 互斥量必须在当前线程锁定的情况下调用该函数，该函数在进入休眠前会自动解锁互斥
// 量。当线程被唤醒该函数返回时，会自动用当前线程锁定互斥量。
// 条件变量的一个通用设计原则：必须由一个while循环而不是if来控制对pthread_cond_wait()
// 的调用，这是因为当代码从pthread_cond_wait()返回时并不能确定判断条件的状态，所
// 以应该立即重新检查判断条件，在条件不满足的情况下继续休眠等待。
// 从pthread_cond_wait()返回时，之所以不能对判断条件的状态做任何假设，是因为：
// 1. 其他线程可能会率先醒来，也许有多个线程在等待获取与条件变量相关的互斥量。即使
//    就互斥量发出通知的线程将判断条件置为预期状态，其他线程依然有可能率先获取互斥
//    量并改变相关共享变量的状态，进而改变判断条件的状态。
// 2. 设计时设置宽松的判断条件或许更为简单，有时用条件变量来表征可能性而非确定性在
//    设计应用程序时会更为简单。换言之就条件变量发送信号意味着可能有些事情需要接收
//    信号的信号去响应，而不是一定有一些事情要做。适用这种方法，可以基于判断条件的
//    近似情况来发送条件变量通知，接收信号的线程可以通过再次检查判断条件来确定是否
//    需要做些什么。
// 3. 可能会发生虚假唤醒的情况，在一些实现中即使没有任何其他线程真地就条件变量发出
//    信号，等待此条件变量的线程仍有可能醒来。在一些多处理器系统上，为确保高效实现
//    而采用的技术会导致此类不常见的虚假唤醒。
// 线程等待条件变量的步骤：
// 1. pthread_mutex_lock
// 2. ... operation before wait ...
// 3. while condition not meet
// 4.   pthread_mutex_unlock and enter sleep
// 5.   wakeup and pthread_mutex_lock (may block if other thread hold lock)
// 6. mutex locked and condition meet
// 7. ... operation after wait ...
// 8. pthread_mutex_unlock

void prh_thrd_cond_lock(prh_thrd_cond *p) {
    prh_zeroret(pthread_mutex_lock(&p->mutex));
}

void prh_thrd_cond_wait(prh_thrd_cond *p, bool (*cond_meet)(void *), void *param) {
    while (!cond_meet(param)) {
        prh_zeroret(pthread_cond_wait(&p->cond, &p->mutex));
    }
    // thread locked and wakeup and cond_meet
}

bool prh_thrd_cond_timedwait(prh_thrd_cond *p, prh_u32 msec, bool (*cond_meet)(void *), void *param) {
    prh_i64 abstime = prh_system_msec() + msec; // u32 msec 最大可以表示48天
    struct timespec ts = {.tv_sec = (time_t)(abstime / 1000), .tv_nsec = (int)((abstime % 1000) * 1000000)};
    int n = 0;
    while (n != ETIMEDOUT && !cond_meet(param)) {
        n = pthread_cond_timedwait(&p->cond, &p->mutex, &ts);
    }
    // thread locked and wakeup and (ETIMEDOUT or cond_meet)
    if (n == ETIMEDOUT) {
        return true; // true WAIT TIMEOUT false WAIT SUCCESS
    }
    prh_prerr_if(n,);
    return false;
}

void prh_thrd_cond_unlock(prh_thrd_cond *p) {
    prh_zeroret(pthread_mutex_unlock(&p->mutex));
}

// The pthread_cond_broadcast() function shall wakeup all threads currently
// blocked on the specified condition variable cond.
// The pthread_cond_signal() function shall wakeup at least one of the
// threads that are blocked on the specified condition variable cond (if
// any threads are blocked on cond).
// If more than one thread is blocked on a condition variable, the scheduling
// policy shall determine the order in which threads are unblocked.
// The pthread_cond_broadcast() and pthread_cond_signal() functions shall
// have no effect if there are no threads currently blocked on cond.

void prh_thrd_cond_signal(prh_thrd_cond *p) {
    // 唤醒至少一个等待的线程，比broadcast更高效。应用这种方式的典型情况是，所有等待
    // 的线程都在执行完全相同的任务。这种情况下，可以避免唤醒所有等待的线程，然后某一
    // 线程获得调度，此线程检查了共享变量的状态（在相关互斥量的保护下），发现有任务需
    // 要完成并执行所需工作并改变共享变量状态，最后释放对相关互斥量的锁定。如果唤醒了
    // 多余的线程，会额外轮流等待锁定互斥量然后检测共享变量的状态，不过由于第一个线程
    // 已经完成了工作这些多余的线程会发现无事可做，随即解锁互斥量继续休眠。
    prh_zeroret(pthread_cond_signal(&p->cond));
}

void prh_thrd_cond_broadcast(prh_thrd_cond *p) {
    // 唤醒所有等待的线程，通常适用的情况是处于等待状态的所有线程执行的任务不同，其各
    // 自关联于条件变量的判定条件不同。条件变量并不保存变量，只是传递应用程序状态信息
    // 的一种通讯机制。发送信号时若无任何线程在等待该条件变量，这个信号也就会不了了
    // 之，线程如果在此后等待该条件变量，只有当再次收到此条件变量的下一信号时才能解除
    // 阻塞状态。
    prh_zeroret(pthread_cond_broadcast(&p->cond));
}

void prh_thrd_sem_wait(prh_thrd_sem *p) {
    prh_zeroret(pthread_mutex_lock(&p->mutex));
    while (p->wakeup_semaphore == 0) {
        prh_zeroret(pthread_cond_wait(&p->cond, &p->mutex));
    }
    p->wakeup_semaphore -= 1;
    prh_zeroret(pthread_mutex_unlock(&p->mutex));
}

void prh_thrd_sem_post(prh_thrd_sem *p, int new_semaphores) {
    assert(new_semaphores > 0);
    prh_zeroret(pthread_mutex_lock(&p->mutex));
    p->wakeup_semaphore += new_semaphores;
    prh_zeroret(pthread_mutex_unlock(&p->mutex));
    if (new_semaphores == 1) { // one semaphore available, can wakeup one thread to handle
        prh_zeroret(pthread_cond_signal(&p->cond));
    } else { // multi semaphore available, all thread can racing to handle them
        prh_zeroret(pthread_cond_broadcast(&p->cond));
    }
}

typedef struct prh_sleep_cond {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    prh_atom_bool wakeup_semaphore;
} prh_sleep_cond;

prh_sleep_cond *prh_sleep_cond_init(void) {
    prh_sleep_cond *p = prh_malloc(sizeof(prh_sleep_cond));
    prh_zeroret(pthread_cond_init(&p->cond, prh_null));
    prh_impl_pthread_mutex_init(&p->mutex);
    prh_atom_bool_init(&p->wakeup_semaphore, false);
    return p;
}

void prh_sleep_cond_free(prh_sleep_cond **cond) {
    // 仅当没有任何线程等待条件变量，将其销毁才是安全的，经销毁的条件变量之后可以调用
    // pthread_cond_init() 对其进行重新初始化。
    prh_sleep_cond *p = *cond;
    if (p == prh_null) return;
    *cond = prh_null;
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
    prh_zeroret(pthread_cond_destroy(&p->cond));
    prh_free(p);
}

void prh_impl_sleep_cond_init(prh_sleep_cond *p) {
    prh_zeroret(pthread_cond_init(&p->cond, prh_null));
    prh_impl_pthread_mutex_init(&p->mutex);
    prh_atom_bool_init(&p->wakeup_semaphore, false);
}

void prh_impl_sleep_cond_free(prh_sleep_cond *p) {
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
    prh_zeroret(pthread_cond_destroy(&p->cond));
}

bool prh_thrd_try_sleep(prh_sleep_cond *p) {
    if (prh_atom_bool_strong_compare_write_false(&p->wakeup_semaphore)) {
        return false;
    } else {
        return true;
    }
}

void prh_thrd_sleep(prh_sleep_cond *p) {
    if (prh_atom_bool_strong_compare_write_false(&p->wakeup_semaphore)) return; // 已经有唤醒存在，不需要睡眠
    prh_zeroret(pthread_mutex_lock(&p->mutex));
    while (!prh_atom_bool_read(&p->wakeup_semaphore)) {
        prh_zeroret(pthread_cond_wait(&p->cond, &p->mutex));
    }
    prh_atom_bool_write(&p->wakeup_semaphore, false);
    prh_zeroret(pthread_mutex_unlock(&p->mutex));
}

void prh_thrd_wakeup(prh_sleep_cond *p) {
    if (prh_atom_bool_read(&p->wakeup_semaphore)) return; // 已经有唤醒存在，无需重新唤醒
    prh_zeroret(pthread_mutex_lock(&p->mutex));
    prh_atom_bool_write(&p->wakeup_semaphore, true);
    prh_zeroret(pthread_mutex_unlock(&p->mutex));
    prh_zeroret(pthread_cond_signal(&p->cond));
}

#ifdef PRH_TEST_IMPLEMENTATION
#include <sys/resource.h> // getrlimit POSIX.1-2008
// In SUSv2 the getpagesize() call was labeled LEGACY, and it was removed in
// POSIX.1-2001.
int getpagesize(void);
// sysconf/fpathconf/pathconf/confstr获取的值是系统配置常量，在一个进程的生命
// 期间都不会改变。
// https://www.man7.org/linux/man-pages/man2/getrlimit.2.html
// https://www.man7.org/linux/man-pages/man7/posixoptions.7.html
// https://www.man7.org/linux/man-pages/man3/confstr.3.html
// https://www.man7.org/linux/man-pages/man3/confstr.3p.html
// https://www.man7.org/linux/man-pages/man3/sysconf.3.html
// https://www.man7.org/linux/man-pages/man3/sysconf.3p.html
void prh_impl_thrd_test(void) {
    long n = 0;
    char buf[32];
    struct rlimit l = {0};

    printf("\n\n[GNU][posix]\n");
    printf("_SC_VERSION: %ldL\n", sysconf(_SC_VERSION));
    printf("Resource limit infinity: RLIM_INFINITY = %lld = unlimited\n", (long long)RLIM_INFINITY);
    n = getrlimit(RLIMIT_AS, &l); // address space
    printf("[%d] The max size of the process's virtual memory: %lldB max %lldB\n", (int)n, (long long)l.rlim_cur, (long long)l.rlim_max);
    n = getrlimit(RLIMIT_STACK, &l);
    printf("[%d] The max size of the process stack space: %lldB max %lldB\n", (int)n, (long long)l.rlim_cur, (long long)l.rlim_max);
    n = getrlimit(RLIMIT_NICE, &l);
    printf("[%d] The max nice value can be raised: %lldB max %lldB\n", (int)n, (long long)l.rlim_cur, (long long)l.rlim_max);
    n = sysconf(_SC_PAGESIZE);
    printf("_SC_PAGESIZE: %ld-byte\n", n);
    printf("getpagesize() %d-byte\n", getpagesize());
    n = sysconf(_SC_2_VERSION);
    printf("_SC_2_VERSION: %ldL\n", n);
    n = sysconf(_SC_THREADS);
    printf("_SC_THREADS support: %ldL\n", n);
    n = sysconf(_SC_RAW_SOCKETS);
    printf("_SC_RAW_SOCKETS support: %ldL\n", n);
    n = sysconf(_SC_THREAD_ATTR_STACKADDR);
    printf("_SC_THREAD_ATTR_STACKADDR support: %ldL\n", n);
    n = sysconf(_SC_THREAD_ATTR_STACKSIZE);
    printf("_SC_THREAD_ATTR_STACKSIZE support: %ldL\n", n);
    n = sysconf(_SC_THREAD_CPUTIME);
    printf("_SC_THREAD_CPUTIME support: %ldL\n", n);
    n = sysconf(_SC_THREAD_PRIORITY_SCHEDULING);
    printf("_SC_THREAD_PRIORITY_SCHEDULING support: %ldL\n", n);
    n = sysconf(_SC_ATEXIT_MAX);
    printf("_SC_ATEXIT_MAX: %ld\n", n);
    n = sysconf(_SC_HOST_NAME_MAX);
    printf("_SC_HOST_NAME_MAX: %ld\n", n);
    n = sysconf(_SC_LOGIN_NAME_MAX);
    printf("_SC_LOGIN_NAME_MAX: %ld\n", n);
    n = sysconf(_SC_OPEN_MAX);
    printf("_SC_OPEN_MAX: %ld\n", n);
    n = sysconf(_SC_THREAD_KEYS_MAX);
    printf("_SC_THREAD_KEYS_MAX: %ld\n", n);
    n = sysconf(_SC_THREAD_STACK_MIN);
    printf("_SC_THREAD_STACK_MIN: %ld\n", n);
    n = sysconf(_SC_THREAD_THREADS_MAX);
    printf("_SC_THREAD_THREADS_MAX: %ld\n", n);
    n = sysconf(_SC_MAPPED_FILES);
    printf("_SC_MAPPED_FILES: %ld\n", n);
    confstr(_CS_GNU_LIBC_VERSION, buf, sizeof(buf));
    printf("_CS_GNU_LIBC_VERSION: %s\n", buf);
    confstr(_CS_GNU_LIBPTHREAD_VERSION, buf, sizeof(buf));
    printf("_CS_GNU_LIBPTHREAD_VERSION: %s\n", buf);

    printf("\n\n[GNU][pthread]\n");
    printf("pthread_t %d-byte\n", (int)sizeof(pthread_t));
    printf("pthread_key_t %d-byte\n", (int)sizeof(pthread_key_t));
    printf("pthread_attr_t %d-byte\n", (int)sizeof(pthread_attr_t));
    printf("pthread_once_t %d-byte\n", (int)sizeof(pthread_once_t));
    printf("pthread_mutex_t %d-byte\n", (int)sizeof(pthread_mutex_t));
    printf("pthread_mutexattr_t %d-byte\n", (int)sizeof(pthread_mutexattr_t));
    printf("pthread_cond_t %d-byte\n", (int)sizeof(pthread_cond_t));
    printf("pthread_mutexattr_t %d-byte\n", (int)sizeof(pthread_mutexattr_t));
    printf("PTHREAD_CANCELED %d\n", (int)(prh_unt)PTHREAD_CANCELED);
    printf("ETIMEDOUT = %d\n", ETIMEDOUT);
    printf("EDEADLK = %d\n", EDEADLK);
    printf("EINVAL = %d\n", EINVAL);
    printf("ENOTSUP = %d\n", ENOTSUP);
    printf("ESRCH = %d\n", ESRCH);
    assert(sizeof(pthread_t) <= sizeof(void *));
}
#endif // PRH_TEST_IMPLEMENTATION
#endif // PTHREAD end
#endif // PRH_THRD_IMPLEMENTATION
#endif // PRH_THRD_INCLUDE

#ifdef PRH_FILE_INCLUDE

#ifdef PRH_FILE_IMPLEMENTATION
#if defined(prh_plat_windows)

#else
// 所有执行 I/O 操作的系统调用都以文件描述符，一个非负整数，来指代打开的文件。文件描述符
// 可以表示所有类型的已打开文件，包括管道（pipe）、FIFO、套接字、终端、设备、普通文件。
// 文件描述符的分配基于进程，不同的进程对文件描述符的分配互不干扰。有三个始终打开的文件描
// 述符 0 1 2，分别表示标准输入（STDIN_FILENO）、表示输出（STDOUT_FILENO）、错误输出
// （STDERR_FILENO）。更确切的说，每个程序都继承了 shell 文件描述符的副本，在程序执行
// 之前，shell 代表这个程序为其打开了这3各文件描述符。在 shell 的日常交互中，这3个文件
// 描述符是始终打开的，这3个文件描述符通常执行 shell 运行所在的终端。这3个文件描述符可以
// 对其进行重定向。可以使用 0 1 2 来代表这 3 个文件描述符，但是例如使用 freopen 对标准
// 输出 stdout 进行重定向，无法保证 stdout 变量值仍然为 1。另外如果关闭了 0 1 2 文件
// 描述符，在创建新的文件描述符时会重用这些已经释放的文件描述符。
#endif // POSIX IMPLEMENTATION
#ifdef PRH_TEST_IMPLEMENTATION

#endif // PRH_TEST_IMPLEMENTATION
#endif // PRH_FILE_IMPLEMENTATION
#endif // PRH_FILE_INCLUDE

#define PRH_SOCK_INCLUDE
#define PRH_SOCK_IMPLEMENTATION
#undef prh_plat_windows
#define prh_plat_linux

// 列出系统中的网络接口，其中包括 MTU 信息：
//      sudo apt install net-tools
//      netstat -i
// 临时端口的分配范围：
//      cat /proc/sys/net/ipv4/ip_local_port_range
// 本地记录的主机名或域名到IP地址之间的映射：
//      cat /etc/hosts
// 服务名称与端口号之间的映射：
//      cat /etc/services
// 网络协议（IP协议）分配的号码：
//      cat /etc/protocols
//      www.iana.org/assignments/protocol-numbers
// 域名解析配置文件：
//      cat /etc/resolv.conf
// 每个 DNS 服务器都知道的一组根域名服务器：
//      dig . ns
//      https://root-servers.org/
// 目的套接字地址的排序配置（RFC 3484）：
//      cat /etc/gai.conf
// 向 inetd 守护进程发送 SIGHUP 信号：
//      killall -HUP inetd
// 列出当前运行的进行信息：
//      ps
//      ps -C program -o "pid ppid pgid sid tty command"
// 查看套接字状态：
//      ./program port=55555 &
//      netstat -a | egrep '(Address|55555)'
//          Proto   Recv-Q  Send-Q  Local Address   Foreign Address     State
//          tcp     0       0       *:55555         *:*                 LISTEN
//          tcp     0       0       localhost:32835 localhost:55555     ESTABLISHED
//          tcp     0       0       localhost:55555 localhost:32835     ESTABLISHED
// 套接字可以发送的附属数据（ancillary data）的最大大小：
//      cat /proc/sys/net/core/optmem_max
#ifdef PRH_SOCK_INCLUDE

#ifdef PRH_SOCK_IMPLEMENTATION
#ifndef PRH_SOCK_DEBUG
#define PRH_SOCK_DEBUG PRH_DEBUG
#endif

#if defined(prh_plat_windows)

#else

#if defined(prh_plat_linux)
// EPOLL API 是 Linux 专有的特性，首次出现是在 Linux 2.6 版中。同 I/O 多路复用 API
// 一样，epoll api 允许进程同时检查多个文件描述符，看其中任意一个是否能执行 I/O 操作。
// 同信号驱动 I/O 一样，当同时检查大量文件描述符时，epoll 能提供更好的性能。实际上 I/O
// 多路复用、信号驱动 I/O 以及 epoll 都是用来实现同一个目标的技术，即同时检查多个文件
// 描述符，看它们是否已经准备好执行 I/O 操作，准确地说，是看 I/O 系统调用是否可以非阻塞
// 地执行。文件描述符就绪状态的转化是通过一些 I/O 事件来触发的，比如输入数据到达，套接字
// 连接建立完成，或者是之前满载的套接字发送缓冲区在 TCP 发送数据传送到对端之后有了剩余
// 空间。同时检查多个文件描述符在类似网络服务器的应用中很有用处，或者是那些必须同时检查
// 终端以及管道或套接字输入的应用程序。需要注意的是这些技术都不会执行实际的I/O操作，它们
// 只是告诉我们某个文件描述符已经处于就绪状态了，这时需要调用其他的系统调用来完成实际的
// I/O操作。
//
// 这里没有介绍的一种 I/O 模型是 POSIX 异步 I/O （AIO）。POSIX AIO 允许进程将 I/O
// 操作排列到一个文件中，当操作完成后得到通知。POSIX AIO 的优点在于最初的 I/O 调用将
// 立刻返回，因此进程不会一直等待数据传输到内核或者等待操作完成。这使得进程可以同 I/O
// 操作一起并行处理其他任务（包括未来可能进入队列的其他I/O操作）。对于特定类型的应用，
// POSIX AIO 能提供有用的性能优势。目前，Linux 在 glibc 中提供有基于线程的 POSIX
// AIO 实现，并且正在朝着内核化的 POSIX AIO 实现而努力，这应该能提供更好的伸缩性能。
//
// 同 epoll 一样，信号驱动 I/O 可以让应用程序高效地检查大量的文件描述符。然而 epoll
// 有着信号驱动 I/O 没有的一些优点：避免了处理信号的复杂性；可以指定想要检查的事件类型，
// 例如读就绪或者写就绪；可以选择一水平触发或边缘触发的形式来通知进程。为了不同系统的
// 可移植性，一个名为 libevent 的软件层库提供了文件描述符 I/O 事件的抽象，已经移植到
// 了多个 UNIX 系统中。Libevent 的底层机制能够以透明的方式，应用各种I/O模型：select、
// poll、信号驱动I/O或者epoll，同样也支持 Solaris 专有的 /dev/poll 接口和 BSD 系统
// 的 kqueue 接口。对于最基础的 select() 和 poll() 在 UNIX 系统中已经存在很长时间，
// 相比其他技术，它最主要的优势是可移植性，但是缺点在于当同时检查大量的（数百或数千个）
// 文件描述符时性能延展性不佳。
//
// 在讨论I/O机制之前，我们需要先区分两种文件描述符准备就绪的通知模式。水平触发通知：如果
// 文件描述符上可以非阻塞地执行I/O系统调用，此时认为它已经就绪。边缘触发通知：如果文件
// 描述符自上次状态检查以来有了新的I/O活动，比如新的输入，此时需要触发通知。其中 select
// 和 poll 使用的是水平触发，信号驱动I/O使用的是边缘触发，epoll 默认使用水平触发但也
// 支持边缘触发。
//
// 当采用水平触发通知时，我们可以在任意时刻检查文件描述符的就绪状态。这表示当我们确定了
// 文件描述符就绪态时，比如存在有输入的数据，就可以对其执行一些I/O操作。水平触发模式
// 因为可以在任意时刻重复检查I/O状态，就没有必要每次就绪后必须尽可能多地执行I/O，因为即
// 使执行少量的I/O或者不执行，下次检查就绪状态都还在，可以继续再执行。
//
// 但是如果使用边缘触发，只有当I/O事件发生时我们才会收到通知，在另一个I/O事件到了前不会
// 再收到任何新的通知。另外，当文件描述符收到I/O事件通知时，通常我们并不知道要处理多少
// I/O，例如有多少字节可读。因此，采用边缘触发通知的程序通常要按照以下规则来设计。
//
// （一）在收到一个I/O事件通知后，程序在某个时刻应该在相应的文件描述符上尽可能多地执行
// I/O，比如尽可能多地读取字节。如果程序没这么做，那么就可能失去执行I/O的机会。因为直到
// 产生另一个I/O事件为止，在此之前程序都不会再接收到通知了，因此也就不知道此时应该执行
// I/O操作。这将导致数据丢失或者程序中出现阻塞。这里提到的程序在“某个时刻”应该执行尽可能
// 多的I/O操作，是因为有时候我们确定了文件描述符处于就绪状态时，可能并不适合马上执行所有
// 的I/O操作。原因是因为如果我们仅对一个文件描述符执行大量的I/O操作，可能会让其他文件
// 描述符处于饥饿状态。
//
// （二）如果程序采用循环来对文件描述符执行尽可能多的I/O，而文件描述符又被置为可阻塞的，
// 那么最终当没有更多的I/O可执行时，I/O系统调用就会阻塞。基于这个原因，每个被检查的文件
// 描述符通常都应该设为非阻塞模式，在得到I/O事件通知后重复执行I/O操作，直到相应的系统
// 调用，比如 read() write() 以错误码 EAGAIN 或 EWOULDBLOCK 的形式失败。
//
// 这里的 I/O 模型通常和非阻塞I/O（O_NONBLOCK标志）一起使用，下面列出了一些例子，以说
// 明为什么这么做会很有用：非阻塞I/O通常和提供有边缘触发通知机制的I/O模型一起使用；如果
// 多个进程（线程）在同一个打开的文件描述符上执行I/O操作，那么从某个特定进程的角度来看，
// 文件描述符的就绪状态可能会在通知就绪和执行后续I/O调用之间发生改变，结果就是一个阻塞
// 式的I/O调用将阻塞，进行再也不能去检查其他的文件描述符；尽管水平触发模式的API通知我们
// 流式套接字的文件描述符已经写就绪了，如果我们在单个write()或send()调用中写入足够大块
// 的数据，那么该调用将阻塞；在非常罕见的情况下，水平触发型API比如select和poll，会返回
// 虚假的就绪通知，它们会错误地通知我们文件描述符已经就绪了，这可能式由内核bug造成的，
// 或非普通情况下的设计方案所期望的行为。一个BSD系统上的监听套接字的虚假就绪通知的例子，
// 如果客户端先连接到服务器的监听套接字上，然后再重置连接，服务器的select()调用在这两个
// 事件之间将提示监听套接字为可读就绪，但随后当客户端重置连接后，服务器端的accept()调用
// 阻塞。
//
// Linux 的 epoll（event poll）API 的主要优点：当检查大量的文件描述符时，epoll 的性能
// 延展性比 select() 和 poll() 高很多；另外 epoll 既支持水平触发也支持边缘触发；而于
// 信号驱动I/O相比，epoll 可以避免复杂的信号处理流程 ，例如信号队列溢出时的处理，还更
// 灵活，可以指定我们希望检查的事件类型，例如检查读就绪、写就绪、或两者同时指定。
//
// epoll api 的核心数据结构称作 epoll 实例，它和一个打开的文件描述符相关联。这个文件
// 描述符不是用来做I/O操作的，它只是内核数据结构的句柄，该数据结构记录了在进程中声明过
// 的感兴趣的文件描述符列表，兴趣列表（interest list），还维护了处于 I/O 就绪态的文件
// 描述符列表，就绪列表（ready list）。就绪列表中的成员是兴趣列表的子集。epoll api
// 由以下三个系统调用组成：epoll_create 创建一个 epoll 实例，返回代表该实例的文件描述
// 符；epoll_ctl 操作同 epoll 实例相关联的兴趣列表；epoll_wait 返回与 epoll 实例
// 相关联的就绪列表中的成员。通过 epoll_ctl 可以增加新的描述符到列表中，或移除已有的文
// 件描述符。
//
// https://www.man7.org/linux/man-pages/man2/epoll_create.2.html
// https://www.man7.org/linux/man-pages/man7/epoll.7.html
//
// #include <sys/epoll.h>
// int epoll_create(int size); Linux 2.6, glibc 2.3.2.
// int epoll_create1(int flags); Linux 2.6.27, glibc 2.9.
//
// epoll_create() creates a new epoll(7) instance. Since Linux 2.6.8, the size
// argument is ignored, but must be greater than zero. epoll_create1() is the
// same as epoll_create(). The following value can be included in flags to
// obtain different behavior: EPOLL_CLOEXEC.
//
// 参数 flags 可以设置为 0 或者 EPOLL_CLOEXEC 标志，如果 flags 为 0，则 epoll_create1()
// 与 epoll_create() 的行为相同，只是省略了过时的 size 参数。EPOLL_CLOEXEC 标志会给
// epoll 文件描述符设置 FD_CLOEXEC 标志，这确保在执行 exec 系统调用时，该文件描述符不
// 会被继承到子进程中。详情可以参考 open(2) 中对 O_CLOEXEC 标志的描述。在多线程环境中，
// 如果不设置 FD_CLOEXEC 标志，子进程可能会意外地继承父进程的文件描述符，这可能导致安全
// 问题或资源泄漏。即在创建 epoll 文件描述符的同时就设置好“关闭执行”标志，避免多线程竞争
// 导致该标志设置失效，设置此标志后，子进程不能访问这个在父进程中的文件描述符。
//
// 如果不是一创建就设置，使用单独的 fcntl(2) 设置 FD_CLOEXEC 标志可能会导致竞争条件。
// 例如，一个线程打开一个文件描述符并尝试使用 fcntl(2) 设置其关闭执行标志，而另一个线程
// 同时执行 fork(2) 和 execve(2)。根据执行顺序，这种竞争条件可能导致 open() 返回的文
// 件描述符意外地泄露到由 fork(2) 创建的子进程中。
//
// 可能错误：EMFILE 进程达到最大打开的文件描述符数量；ENFILE 系统达到最大可打开的文件
// 描述符总数；ENOMEM 没有足够的内存空间用于创建内核对象。Prior to Linux 2.6.29, a
// /proc/sys/fs/epoll/max_user_instances kernel parameter limited live epolls
// for each real user ID, and caused epoll_create() to fail with EMFILE on
// overrun. 版本 2.6.29 之前，有内核参数限制每个真实用户ID中活动的 epoll 最大数量。
//
// 默认情况下，程序中打开的所有文件描述符在 exec() 执行新程序的过程中保持打开并有效。这
// 通常很实用，因为文件描述符在新程序中自动有效，让新程序无需再去了解文件名或重新打开。但
// 一些情况下在执行 exec() 前确保关闭某些特定的文件描述符，尤其是在特权进程中调用 exec()
// 来启动一个未知程序时，或启动程序并不需要这些已打开的文件描述符时，从安全编程的角度出发，
// 应当在加载新程序之前关闭那些不必要的文件描述符。为此，内核为每个文件描述符提供了执行时
// 关闭标志，如果设置了这一标志，如果调用 exec() 成功会自动关闭该文件描述符，如果调用
// exec() 失败则文件描述符会继续保持打开。系统调用 fork() 允许一进程（父进程）创建一新
// 进程（子进程），子进程几乎是父进程的翻版（可认为父进程一分为二了），它获得了父进程的栈、
// 数据段、堆和执行文本段的拷贝。执行 fork() 时，子进程会获得父进程所有文件描述符的副本，
// 这些副本的创建方式类似于 dup()，这也意味着父子进程中对应的描述符均指向相同的打开文件句
// 柄。文件句柄包含了当前文件偏移量以及文件状态标志，因此这些属性是在父子进程间共享的，例
// 如子进程如果更新了文件偏移量，那么这种改变也会影响到父进程中相应的描述符。系统调用
// exec() 用于执行一个新程序，它将新程序加载到当前进程中，这将丢弃现存的程序段，并为新程
// 序重新创建栈、数据段以及堆。
//
// 通过进程的 /proc/pid/fdinfo 目录中 epoll 文件描述符的条目，可以查看正通过一个 epoll
// 文件描述符监控的文件描述符集合。请参阅 proc(5) 以获取更多详细信息。假如 epfd 的值为
// 5，当前进程 pid 为 1234，则可以查看： cat /proc/1234/fdinfo/5 。
//
// kcmp(2) 的 KCMP_EPOLL_TFD 操作可用于测试一个文件描述符是否在一个 epoll 实例中：
//      if (kcmp(getpid(), getpid(), KCMP_EPOLL_TFD, epfd, fd) == 0) {
//          printf("fd is in the epoll instance\n");
//      }
//
// https://www.man7.org/linux/man-pages/man2/epoll_ctl.2.html
// https://www.man7.org/linux/man-pages/man3/epoll_event.3type.html
// https://www.man7.org/linux/man-pages/man2/ioctl_eventpoll.2.html
//
// #include <sys/epoll.h> // epoll_ctl 返回 0 表示成功，返回 -1 表示失败 errno
// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *_Nullable event);
// struct epoll_event { // 该结构体指定内核在文件描述符就绪时，需要保存和返回的数据
//     uint32_t      events;  /* Epoll events */ 设置关注的事件类型和事件标志
//     epoll_data_t  data;    /* User data variable */
// };
// union epoll_data {
//     void     *ptr;
//     int       fd;
//     uint32_t  u32;
//     uint64_t  u64;
// };
// typedef union epoll_data epoll_data_t;
//
// 从 epoll 的兴趣列表中新增、删除、或修改对应的文件描述符 fd，该描述符可以是代表管道、
// FIFO、套接字、POSIX 消息队列、inotify 实例、终端、设备、甚至是另一个 epoll 实例的
// 文件描述符（例如我们可以为受检查的描述符建立起一种层次关系）。但是，这里的 fd 不能是
// 普通文件或目录的文件描述符，会出现 EPERM 错误。epoll 支持所有在 poll(2) 中支持的
// 文件描述符。
//      EPOLL_CTL_ADD - 如果兴趣列表中已经存在该文件描述符，报 EEXIST 错误。
//      EPOLL_CTL_MOD - 如果文件描述符不在兴趣列表中，会报 ENOENT 错误。
//      EPOLL_CTL_DEL - 如果文件描述符不在兴趣列表中，会报 ENOENT 错误。如果要兼容
//          Linux 2.6.9 之前的版本，event 参数虽然会被忽略，但不能传递空指针。
//          Before Linux 2.6.9, the EPOLL_CTL_DEL operation required a non-null
//          pointer in event, even though this argument is ignored. Since Linux
//          2.6.9, event can be specified as NULL when using EPOLL_CTL_DEL.
//          Applications that need to be portable to kernels before Linux 2.6.9
//          should specify a non-null pointer in event.
//          关闭一个文件描述符，会自动将其从所有的 epoll 实例的兴趣列表中移除。其实具体
//          的，epoll 实例中保存的文件描述符是位于内核中的文件描述（file description），
//          用户空间中可以存在多个文件描述符（file descriptor）指向这个内核中的文件描述，
//          只有所有用户空间中的执行同一个文件描述的文件描述符都关闭了，内核才知道此时
//          已经没有引用的用户空间的程序，可以将文件描述回收。因此准确的说，当所有指向
//          打开的文件描述的文件描述符都被关闭后，这个打开的文件描述才从 epoll 的兴趣
//          列表中移除。
//
// 参数 events 是比特位，可以或上零个或多个事件类型（event type），事件类型也会在文件
// 描述符就绪时被 epoll_wait 返回，还可以或上事件标志（event flag），事件标志会影响
// 文件描述符就绪的触发行为，但不会被 epoll_wait 返回。事件类型包括：
//      EPOLLIN - 文件可以进行 read 操作，读取非优先级数据。
//      EPOLLPRI - There is an exceptional condition on the file descriptor.
//          See the discussion of POLLPRI in poll(2). 文件描述符上有异常条件，这
//          通常用于检测和读取紧急数据（如带外数据），具体可能包括：
//          •  There is out-of-band data on a TCP socket (see tcp(7)).
//          •  A pseudoterminal master in packet mode has seen a state
//             change on the slave (see ioctl_tty(2)).
//          •  A cgroup.events file has been modified (see cgroups(7)).
//      EPOLLOUT - 文件可以进行 write 操作
//      EPOLLERR - 关联文件描述符有错误事件发生，或当管道的读取端已经关闭时写入端也会
//          也会触发该事件。 This event is also reported for the write end of a
//          pipe when the read end has been closed.
//          错误事件总会被 epoll_wait 上报，调用 epoll_ctl 设置该类型是不必须的。
//          epoll_wait(2) will always report for this event; it is not necessary
//          to set it in events when calling epoll_ctl().
//      EPOLLRDHUP (Linux 2.6.17) - Stream socket peer closed connection, or
//          shut down writing half of connection. (This flag is especially
//          useful for writing simple code to detect peer shutdown when using
//          edge-triggered monitoring.) 对方挂断事件，即流式套接字对端关闭了连接，或
//          关闭了连接的写入部分。这个标志特别适用于使用边缘触发监控对端的关闭情况。
//      EPOLLHUP - 文件描述符发生了挂断事件，epoll_wait(2) 总是会上报这个事件，因此
//          不需要特别设置。从管道或流式套接字等通道进行读取操作时，这个事件仅表示对端
//          关闭了其端的通道，只有当通道中所有未处理的数据都会消费后，后续的读取操作
//          才会返回0（文件尾）。例如读取标准输入流 STDIN_FILENO，在终端上的写入端输入
//          CTRL+D 表示 EOF，来关闭写入端的通道。
//          Note that when reading from a channel such as a pipe or a stream
//          socket, this event merely indicates that the peer closed its end of
//          the channel. Subsequent reads from the channel will return 0 (end
//          of file) only after all outstanding data in the channel has been
//          consumed.
//          只要关联的文件已经关闭或已经断开连接，或者读取端检测到对端关闭了写入端，都会
//          触发该事件。EPOLLRDHUP 适用于精确检测对端关闭连接的场景，EPOLLHUP 适用于
//          需要处理文件描述符不再可用的场景。
//
// 可用的事件标志包括：
//      EPOLLET - 默认是水平触发（level-triggered），设置该表示表示边缘触发（edge-triggered）。
//      EPOLLONESHOT (Linux 2.6.2) - 一次性通知，当 epoll_wait 通知完后，会将对应的
//          文件描述符设为禁用不会再报告任何事件，如果要继续监控，需要调用 epoll_ctl
//          EPOLL_CTL_MOD 重新启用文件描述符并设置新的事件掩码。
//      EPOLLWAKEUP (Linux 3.5) - If EPOLLONESHOT and EPOLLET are clear and the
//          process has the CAP_BLOCK_SUSPEND capability, ensure that the system
//          does not enter "suspend" or "hibernate" while this event is pending
//          or being processed. 如果没有设置 EPOLLET 和 EPOLLONESHOT，EPOLLWAKEUP
//          确保 CAP_BLOCK_SUSPEND 能力的进程在文件描述符对应的事件待处理或处理期间，
//          系统不会进入“挂起”或“休眠”状态。适用于需要在事件处理期间防止系统进入低功耗
//          状态的场景，例如后台服务或实时应用程序。事件被视为“处理中”的时间是从 epoll_wait(2)
//          返回事件开始，直到下一次对同一个 epoll 文件描述符调用 epoll_wait(2)，或者
//          关闭该文件描述符，或者使用 EPOLL_CTL_DEL 移除事件文件描述符，或者使用
//          EPOLL_CTL_MOD 清除事件文件描述符的 EPOLLWAKEUP。
//          如果 EPOLLWAKEUP 被设置，但调用进程不具备 CAP_BLOCK_SUSPEND 能力，EPOLLWAKEUP
//          标志将被静默忽略。一个健壮的应用程序在尝试使用 EPOLLWAKEUP 标志时，应该先
//          检查它是否具有 CAP_BLOCK_SUSPEND 能力。
//          Capabilities are a per-thread attribute. CAP_BLOCK_SUSPEND (since
//          Linux 3.5) Employ features that can block system suspend (epoll(7)
//          EPOLLWAKEUP, /proc/sys/wake_lock).
//      https://www.man7.org/linux/man-pages/man7/epoll.7.html
//          如果系统通过 /sys/power/autosleep 处于自动睡眠模式，并且发生了一个事件，该
//          事件使设备从睡眠状态中唤醒，设备驱动程序将只保持设备唤醒状态，直到该事件被排队。
//          为了使设备在发生的事件被处理并在处理完成之前，一直保持唤醒状态，需要使用 epoll_ctl(2)
//          的 EPOLLWAKEUP 标志。
//          当 EPOLLWAKEUP 标志设置在 struct epoll_event 的 events 字段中时，系统将
//          从事件被排队的那一刻起保持唤醒状态，通过返回事件的 epoll_wait(2) 调用，直到
//          随后的 epoll_wait(2) 调用。如果事件需要在那之后的时间内保持系统唤醒，那么应
//          该在第二次 epoll_wait(2) 调用之前获取一个单独的 wake_lock。
//      EPOLLEXCLUSIVE (Linux 4.5) - 设置独占唤醒模式，适用于需要避免多个 epoll 实例
//          同时唤醒引发“雷鸣事件”，从而减少资源竞争和性能问题。雷鸣般的牧群问题（thundering
//          herd problems）。
//          同一个文件描述符可能被添加到多个 epoll 实例中，如果这些 epoll 实例都设置了
//          EPOLLEXCLUSIVE，那么只有至少一个 epoll 会被唤醒。而默认没有设置 EPOLLEXCLUSIVE
//          的情况下，所有epoll 都会被唤醒。
//          If the same file descriptor is in multiple epoll instances, some
//          with the EPOLLEXCLUSIVE flag, and others without, then events will
//          be provided to all epoll instances that did not specify EPOLLEXCLUSIVE,
//          and at least one of the epoll instances that did specify EPOLLEXCLUSIVE.
//          如果其中一些设置了，而另外一些没有设置，没有设置的所有 epoll 都会收到通知，
//          设置了的只有至少一个收到通知。
//          以下值可以与 EPOLLEXCLUSIVE 一起指定：EPOLLIN、EPOLLOUT、EPOLLWAKEUP 和
//          EPOLLET，也可以指定 EPOLLHUP 和 EPOLLERR，但这不是必需的。在 events 中指
//          定其他值会导致 EINVAL 错误。
//          EPOLLEXCLUSIVE 可能只能在 EPOLL_CTL_ADD 操作中使用，尝试在 EPOLL_CTL_MOD
//          操作中使用它会导致错误。如果已经使用 epoll_ctl() 设置了 EPOLLEXCLUSIVE，
//          那么对同一 epfd 和 fd 对的后续 EPOLL_CTL_MOD 操作将导致错误。同样，如果在
//          events 中指定 EPOLLEXCLUSIVE 并将 epoll 实例作为目标文件描述符传入也会失
//          败。在所有这些情况下，错误都是 EINVAL。
//          A call to epoll_ctl() that specifies EPOLLEXCLUSIVE in events and
//          specifies the target file descriptor fd as an epoll instance will
//          likewise fail.
//      https://www.man7.org/linux/man-pages/man7/epoll.7.html
//          如果多个线程（或者进程，如果子进程通过 fork(2) 继承了 epoll 文件描述符）在
//          epoll_wait(2) 中阻塞，等待同一个 epoll 文件描述符，且关注列表中一个标记为
//          边缘触发（EPOLLET）通知的文件描述符准备就绪，那么只有一个线程（或进程）会从
//          epoll_wait(2) 中被唤醒。这在某些场景下提供了一个有用的优化，用于避免 thundering
//          herd 唤醒。
//          1. 多个线程等待同一个 epfd，边缘触发文件描述符如果就绪，只会通知一个线程
//          2. 多个进行等待同一个 epfd，边缘触发文件描述符如果就绪，只会通知一个进程
//          3. 多个 epfd 关注同一个 fd，如果该文件描述符就绪，没有给该 fd 设置独占标志
//             的所有 epfd 都会被唤醒，所有给 fd 设置了独占标志的 epfd 只有至少一个被
//             唤醒
//
// 可能返回的错误：
//      EBADF - epfd 或 fd 是非法文件描述符。
//      EEXIST - EPOLL_CTL_ADD 一个已经注册的 fd。
//      EINVAL - epfd 不是一个 epoll 文件描述符, 或者 fd 与 epfd 相同，或者提供的 op
//          操作不被支持。不允许一起指定的事件类型，指定了 EPOLLEXCLUSIVE 标志。
//          EPOLL_CTL_MOD 操作一起指定了 EPOLLEXCLUSIVE 标志。如果 epfd fd 对之前
//          设置了 EPOLLEXCLUSIVE 标志，再 EPOLL_CTL_MOD 进行修改。设置了 EPOLLEXCLUSIVE
//          并且 fd 指向另一个 epoll 实例。
//      ELOOP - EPOLL_CTL_ADD 添加一个指向另一个 epoll 实例的 fd，导致 epoll 实例
//          引用循环，或者 epoll 的嵌套深度超过了 5 次。
//      ENOENT - EPOLL_CTL_MOD 或 EPOLL_CTL_DEL 一个没有注册的 fd。
//      ENOMEM - 没有足够的内存空间来完成对应的操作。
//      ENOSPC - The limit imposed by /proc/sys/fs/epoll/max_user_watches was
//          encountered while trying to register (EPOLL_CTL_ADD) a new file
//          descriptor on an epoll instance. See epoll(7) for further details.
//          注册的监控个数达到 epoll 用户配置的最大值。因为每个注册到 epoll 实例的文件
//          描述符需要占用一小段不能被交换的内核内存空间，因此内核提供了一个接口用来定义
//          每个用户可以注册到 epoll 实例上的文件描述符总数。这个上限值可以通过 max_user_watches
//          文件来查看和修改。默认的上限值根据可用的系统内存来计算得出，参考 epoll(7)
//          用户手册页。
//      https://www.man7.org/linux/man-pages/man7/epoll.7.html
//          以下接口可用于限制 epoll 消耗的内核内存量：
//          /proc/sys/fs/epoll/max_user_watches (自 Linux 2.6.28 起可用)
//          它指定一个用户可以在系统上所有 epoll 实例中注册的文件描述符总数的限制。
//          在 32 位内核上，每个注册的文件描述符大约占用 90 字节；在 64 位内核上，大约
//          占用 160 字节。
//          Currently, the default value for max_user_watches is 1/25 (4%) of
//          the available low memory, divided by the registration cost in bytes.
//          当前 max_user_watches 的默认值是，例如64位系统可用低内存为 1GB，那么其值
//          为 1GB * 0.04 / 160-byte ≈ 26214。
//      EPERM - 文件描述符指向的目标文件不支持 epoll，例如是一个普通文件或目录。
//
// #include <sys/epoll.h>
// #include <sys/ioctl.h>
// struct epoll_params {
//     uint32_t busy_poll_usecs;    /* Number of usecs to busy poll */
//     uint16_t busy_poll_budget;   /* Max packets per poll */
//     uint8_t prefer_busy_poll;    /* Boolean preference  */
//     /* pad the struct to a multiple of 64 bits */
//     uint8_t __pad;   必须为零，这是一个填充字段，用于对齐和扩展性。
// }; // 成功返回 0，失败返回 -1 和 errno，Linux 6.9 glibc 2.40
// int ioctl(int epfd, EPIOCSPARAMS, const struct epoll_params *argp);
// int ioctl(int epfd, EPIOCGPARAMS, struct epoll_params *argp);
//
// 获取（EPIOCGPARAMS）和设置（EPIOCSPARAMS）epoll 的参数，通过合理配置这些参数，可以
// 优化网络处理的性能，特别是在高负载的系统中。epoll_params 结构体用于配置 epoll 的参数，
// 特别是在 Linux 6.9 及更高版本中引入的忙碌轮询（busy polling）功能。
// busy_poll_usecs - 表示网络协议栈将进行忙碌轮询的微秒数，在此时间段内，网络设备将被反
//      复轮询以获取数据包，该值不能超过 INT_MAX。
// busy_poll_budget - 表示网络协议栈在每次轮询尝试中可取出的最大数据包数量，该值不能超
//      过 NAPI_POLL_WEIGHT（截至 Linux 6.9，其值为 64），除非进程以 CAP_NET_ADMIN
//      权限运行。
// prefer_busy_poll - 如果启用，这表示向网络协议栈表明忙碌轮询是处理网络数据的首选方法，
//      网络栈应给予应用程序进行忙碌轮询的机会。如果没有此选项，非常繁忙的系统可能会继续
//      通过正常的 IRQ 触发 softIRQ 和 NAPI 的方法进行网络处理。
// 另见 linux.git/Documentation/networking/napi.rst
//      linux.git/Documentation/admin-guide/sysctl/net.rst
//
// 可能的错误：
//      EOPNOTSUPP - 内核的编译版本不支持忙碌轮询（busy poll）功能
//      EINVAL - epfd 是一个非法的文件描述符，__pad 参数不是零，busy_poll_usecs 超
//          过 INT_MAX 的大小，prefer_busy_poll 不是 0 也不是 1
//      EPERM - 运行线程没有 CAP_NET_ADMIN 能力并且指定的 busy_poll_budget 超过
//          NAPI_POLL_WEIGHT 的大小
//      EFAULT - 传入的 argp 是一个非法地址
//
// https://www.man7.org/linux/man-pages/man2/epoll_wait.2.html
//
// #include <sys/epoll.h> // 返回 0 表示超时返回并没有任何就绪文件描述符，大于 0 表示就绪文件描述符的个数，返回 -1 表示错误
// int epoll_wait(int epfd, struct epoll_event events[.maxevents], int maxevents, int timeout); // Linux 2.6, glibc 2.3.2
// int epoll_pwait(... int timeout, const sigset_t *_Nullable sigmask); // // Linux 2.6.19, glibc 2.6
// int epoll_pwait2(... const struct timespec *_Nullable timeout, const sigset_t *_Nullable sigmask); // Linux 5.11
//
// 系统调用 epoll_wait() 返回 epoll 实例中处于就绪状态的文件描述符信息，单次调用能返回
// 多个就绪文件描述符的信息。数组 events 的空间由调用者负责申请，其能包含的元素个数由
// maxevents 参数指定，maxevents 参数必须大于零。每个数组元素，返回的都是单个就绪文件
// 描述符的信息，events 字段返回了在该描述符上已经发生的事件类型。如果 timeout 设置为
// -1 则表示一直阻塞，直到兴趣列表中的文件描述符有事件发生，或者调用被信号处理函数中断。
// 如果等于 0 则表示执行一次非阻塞式检查。如果大于 0 表示最多阻塞 timeout 毫秒，使用的
// 是 CLOCK_MONOTONIC 时钟，注意时间会近似到系统时钟的颗粒度，并且内核调度的延迟会让阻
// 塞间隔超长一小段时间。另外 epoll_pwait2 提供纳秒级别的等待时间，如果传递空指针则表示
// 一直阻塞。
//
// 在 Linux 2.6.37 之前，大于大约 LONG_MAX / HZ 毫秒的超时值会被当作 -1（无穷大）。
// 因此，例如，在一个 sizeof(long) 为 4 且内核 HZ 值为 1000 的系统上，这意味着大于
// 35.79 分钟的超时值会被当作无穷大，即 2147483647 / 1000。HZ 表示内核的时钟频率，在
// 许多系统上，HZ 的默认值为 1000。
//
// epoll_pwait 可以完全的让应用程序安全的等待文件描述符就绪，或者接收到一个信号。如果
// 提供的参数 sigmask 为空，epoll_pwait 等价于 epoll_wait。下面的 epoll_pwait 调用：
//      ready = epoll_pwait(epfd, &events, n, timeout, &sigmask);
// 等价于原子的执行以下代码：
//      sigset_t origmask;
//      pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);
//      ready = epoll_wait(epfd, &events, maxevents, timeout);
//      pthread_sigmask(SIG_SETMASK, &origmask, NULL);
// 原始的 epoll_pwait() 和 epoll_pwait2() 系统调用有一个第六个参数，size_t sigsetsize，
// 它指定了 sigmask 参数的大小（以字节为单位）。Glibc 的 epoll_pwait() 包装函数将这个
// 参数指定为一个固定值 sizeof(sigset_t)。
//
// 在多线程程序中，可以一个线程 epoll_wait() 一边检测，另一个线程使用 epoll_ctl() 来添
// 加或修改感兴趣的文件描述符。需要注意的是，可以在一个当前关注列表为空的 epoll 实例上调
// 用 epoll_wait()，或者因为其他线程关闭或移除了文件描述符，导致关注列表变为空。调用将阻
// 塞，直到另一个线程将某个文件描述符添加到关注列表中，并且该文件描述符准备就绪。
// While one thread is blocked in a call to epoll_wait(), it is possible for
// another thread to add a file descriptor to the waited-upon epoll instance.
// If the new file descriptor becomes ready, it will cause the epoll_wait() call
// to unblock.
//
// 如果在调用 epoll_wait() 时有超过 maxevents 个文件描述符准备就绪，那么连续的 epoll_wait()
// 调用将依次轮询这些准备就绪的文件描述符。这种行为有助于避免饥饿场景，即一个进程因为专注
// 于一组已经就绪的文件描述符，而未能注意到其他文件描述符也已经就绪。
//
// 可能的错误：
//      EBADF - epfd 是一个非法的文件描述符
//      EFAULT - events 指向的内存区域没有写权限
//      EINTR - 在收到文件描述符事件或超时之前，调用被信号处理函数中断，参考 signal(7)
//      EINVAL - epfd 不是一个合法的 epoll 文件描述符，或者 maxevents 小于等于 0
//
// 当采用边缘触发通知时避免出现文件描述符饥饿现象：假设我们采用边缘触发通知监控多个文件
// 描述符，其中一个处于就绪状态的文件描述符上有着大量的输入存在，可能是一个不间断的输入
// 流。如果在检测到该文件描述符处于就绪状态后，就通过非阻塞式的读操作将所有的输入都读取，
// 那么此时就会有使其他文件描述符处于饥饿状态的风险，因为非阻塞式的不断读取可能因为输入
// 的不断到来一直读取不完或花费很长时间才读完。该问题的一种解决方案是让应用程序维护一个
// 列表，列表中存放着已经被通知为就绪状态的文件描述符，通过一个循环按照如下方式不断处理：
// （一）调用 epoll_wait() 检查文件描述符，将处于就绪态的描述符添加到应用程序维护的列表
// 中。如果没有就绪的文件描述符，应用程序就继续处理已经处于就绪态的文件描述符。
// （二）在应用程序维护的列表中，只在已经就绪的文件描述符上进行一定限度的I/O操作，可能是
// 轮转方式循环处理（round-robin），而不是每次 epoll_wait() 调用后都从列表开头开始处理。
// 当相关非阻塞I/O系统调用出现 EAGAIN 或 EWOULDBLOCK 时，文件描述符就可以从维护列表中
// 移除。
// （三）尽管采用这种方法需要做一些额外的编程工作，但是除了能避免出现文件描述符饥饿现象外，
// 我们还能获得其他益处。比如，我们可以在上述循环中加入其他的步骤，比如处理定时器以及用
// sigwaitinfo() 或其他类似的机制来接收信号。
//
// 因为信号驱动I/O也是采用边缘触发通知机制，因此也需要考虑文件描述符饥饿的情况。与之相反，
// 在采用水平触发通知机制的应用程序中，考虑文件描述符饥饿的情况并不是必须的。因为我们可以
// 采用水平触发通知在非阻塞式的文件描述符上通过循环连续地检查描述符的就绪状态，然后在下一
// 次检查文件描述符的状态前在处于就绪态的描述符上做一些I/O处理就可以了。
//
// https://www.man7.org/linux/man-pages/man7/epoll.7.html
//
// 可能的陷阱及避免方法：（一）饥饿（边缘触发），如果有巨大数值的 I/O 空间，尝试耗尽它可
// 能会导致其他文件无法得到处理，从而造成饥饿（这个问题并不特定于 epoll）。解决方案是维护
// 一个就绪列表，并在其关联的数据结构中标记文件描述符为就绪，从而使应用程序记住哪些文件需
// 要处理，并在所有就绪文件之间轮询，这也支持忽略后续收到的已就绪文件的事件。
// （二）如果使用事件缓存，如果你使用事件缓存或存储从 epoll_wait(2) 返回的所有文件描述符，
// 则确保提供一种动态标记其关闭的方法。假设你从 epoll_wait(2) 收到 100 个事件，而在事件
// #47 中，一个条件导致事件 #13 被关闭。如果你直接移除其结构并调用 close(2) 关闭对应的
// 文件描述符，那么你的事件缓存可能仍然会显示该文件描述符有事件等待。解决这个问题的一个方
// 法是在处理事件 47 时，调用 epoll_ctl(EPOLL_CTL_DEL) 删除文件描述符 13 并调用 close(2)，
// 然后将其关联的数据结构标记为已移除，并将其添加到清理列表中。如果你在批量处理中发现另一
// 个事件为文件描述符 13，你将发现该文件描述符之前已被移除，因此不会产生混淆。
//
// 问题与解答
// 1. 如何区分关注列表（interest list）中注册的文件描述符？
//      在区分时使用的键是用户空间文件描述符编号（file descriptor number）和内核打开的
//      文件描述（open file description 或 open file handle）的组合。
// 2. 如果在同一个 epoll 实例中注册同一个文件描述符两次会发生什么？
//      你可能会收到 EEXIST 错误。然而，可以将一个重复的（dup(2)、dup2(2)、fcntl(2)
//      F_DUPFD）文件描述符添加到同一个 epoll 实例中。如果重复的文件描述符用不同的事件
//      掩码注册，这可以是一种有用的事件过滤技术。
// 3. 两个 epoll 实例可以等待同一个文件描述符吗？如果是，事件会报告给两个 epoll 文件描述符吗？
//      是的，事件会报告给两个。然而，正确实现这一点可能需要谨慎编程。
// 4. epoll 文件描述符本身可以被 poll/epoll/select 吗？
//      可以。如果一个 epoll 文件描述符有等待的事件，它将表示为可读。
// 5. 如果试图将一个 epoll 文件描述符放入自己的文件描述符集中会发生什么？
//      epoll_ctl(2) 调用会失败（EINVAL）。然而，你可以将一个 epoll 文件描述符添加到
//      另一个 epoll 文件描述符集中。
// 6. 我可以将一个 epoll 文件描述符通过 UNIX 域套接字发送给另一个进程吗？
//      可以，但这没有意义，因为接收进程不会有关注列表中文件描述符的副本。
// 7. 关闭一个文件描述符会导致它从所有 epoll 关注列表中移除吗？
//      是的，但请注意以下要点。文件描述符是对打开文件描述的引用，参见 open(2)。每当文件
//      描述符通过 dup(2)、dup2(2)、fcntl(2) F_DUPFD 或 fork(2) 复制时，都会创建一个
//      指向相同打开文件描述的新文件描述符。一个打开文件描述会一直存在，直到所有引用它的
//      文件描述符都被关闭。
//      只有在所有引用底层打开文件描述的文件描述符都被关闭后，文件描述符才会从关注列表中
//      移除。这意味着，即使一个属于关注列表的文件描述符已经被关闭，如果还有其他引用相同
//      底层文件描述的文件描述符保持打开状态，事件仍可能为该文件描述符报告。为了避免这种
//      情况发生，必须在复制文件描述符之前，显式地从关注列表中移除该文件描述符（使用
//      EPOLL_CTL_DEL）。或者，应用程序必须确保所有文件描述符都被关闭（这可能很困难，如
//      果文件描述符被库函数在幕后通过 dup(2) 或 fork(2) 复制）。
// 8. 如果在两次 epoll_wait(2) 调用之间发生多个事件，它们是合并还是分别报告？
//      它们将被合并。
// 9. 对文件描述符的操作会影响已经收集但尚未报告的事件吗？
//      在已经存在的文件描述符上，你只能执行两种操作，移除在这种情况下没有意义，修改将重
//      新读取可用的 I/O。
// 10. 使用 EPOLLET 标志（边缘触发行为）时，我需要持续读取/写入文件描述符，直到遇到 EAGAIN 吗？
//      从 epoll_wait(2) 接收到事件，表示这样的文件描述符已准备好进行请求的 I/O 操作。
//      你应该认为它一直可用，直到下一次（非阻塞）读取/写入返回 EAGAIN。何时以及如何使用
//      文件描述符完全取决于你。
//      对于分组（packed-oriented）或令牌导向（token-oriented）的文件，例如数据报套接
//      字、处于规范模式的终端，检测读取/写入 I/O 空间结束的唯一方法是继续读取/写入，直
//      到遇到 EAGAIN。
//      对于流导向（stream-oriented）文件，例如管道、FIFO、流套接字，也可以通过检查从
//      目标文件描述符读取/写入的数据量来检测读取/写入 I/O 空间耗尽的情况。例如，如果你
//      通过调用 read(2) 要求读取一定数量的数据，而 read(2) 返回的字节数较少，你可以确
//      信该文件描述符的读取 I/O 空间已经耗尽，写入时使用 write(2) 也是如此。如果不能
//      保证监控的文件描述符总是引用流导向的文件，则避免使用后一种技术。
// 11. 边缘触发文件描述符就绪后，在进行 read(2) 的期间，又有新的数据到来，后续的 read(2)
//     操作会直接读到新来的数据吗，还是必须再一次 epoll_wait(2) 后才能读到新的数据？
//     对于 write(2) 呢，也一样吗？
//      TODO: ？？？
#include <sys/epoll.h>
#include <sys/ioctl.h>

int prh_epoll_create(void) {
    int fd = epoll_create1(EPOLL_CLOEXEC);
    assert(fd >= 0); // 文件描述符是一个非负值
    return fd;
}

void prh_epoll_del(int epfd, int fd) {
    struct epoll_event event;
    prh_zeroret(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event));
}

#include <sys/capability.h> // Link with -lcap
#include <sys/types.h>

bool prh_impl_epoll_cap_block_suspend_get(void) {
    assert(CAP_IS_SUPPORTED(CAP_BLOCK_SUSPEND));
    cap_t caps = cap_get_proc();
    assert(caps != prh_null);
    cap_flag_value_t cap_set = CAP_CLEAR;
    prh_zeroret(cap_get_flag(caps, CAP_BLOCK_SUSPEND, CAP_EFFECTIVE, &cap_set));
    prh_zeroret(cap_free(caps));
    return cap_set == CAP_SET;
}

void prh_impl_epoll_cap_block_suspend_set(void) {
    assert(CAP_IS_SUPPORTED(CAP_BLOCK_SUSPEND));
    cap_t caps = cap_get_proc();
    assert(caps != prh_null);
    cap_flag_value_t cap_set = CAP_CLEAR;
    prh_zeroret(cap_get_flag(caps, CAP_BLOCK_SUSPEND, CAP_EFFECTIVE, &cap_set));
    prh_defer_if(cap_set == CAP_SET,);
    prh_zeroret(cap_get_flag(caps, CAP_SETPCAP, CAP_EFFECTIVE, &cap_set));
    prh_defer_if(cap_set != CAP_SET, prh_prerr(CAP_SETPCAP));
    cap_value_t cap_list[1] = {CAP_BLOCK_SUSPEND};
    prh_zeroret(cap_set_flag(caps, CAP_EFFECTIVE, 1, cap_list, CAP_SET));
    prh_zeroret(cap_set_proc(caps)); // 设置 CAP_BLOCK_SUSPEND 需要具有相应的权限，通常只有 root 用户或具有 CAP_SETPCAP 权限的用户才能设置其他能力。
label_defer:
    prh_zeroret(cap_free(caps));
}
#endif // prh_plat_linux

// RFC 791 - Internet Protocol, J. Postel (ed.), 1981
// RFC 950 - Internet Standard Subnetting Procedure, J. Mogul J. Postel, 1985
// RFC 793 - Transmission Control Protocol, J. Postel (ed.), 1981
// RFC 768 - User Datagram Protocol, J. Postel (ed.), 1980
// RFC 1122 - Requirements for Internet Hosts Communication Layers, R. Braden (ed.), 1989 对早期 TCP/IP 的扩展和修正
// RFC 1323 - TCP Extensions for High Performance, 1992
// RFC 2018 - TCP Selective Acknowledgment Options, 1996
// RFC 2581 - TCP Congestion Control, 1999
// RFC 2861 - TCP Congestion Window Validation, 2000
// RFC 2883 - An Extension to the Selective Acknowledgement (SACK) Option, 2000
// RFC 2988 - Computing TCP's Retransmission Timer, 2000
// RFC 3168 - The Addition of Explicit Congestion Notification (ECN) to IP, 2001
// RFC 3390 - Increasing TCP's Initial Window, 2002
//
// 流式套接字（SOCK_STREAM）提供可靠双向的字节流通信信道，字节流表示与管道一样不存在消息
// 边界的概念。流式套接字通常被称为面向连接的，术语对等套接字指连接另一端的套接字，对等
// 地址表示对端地址。数据包套接字（SOCK_DGRAM）运行数据以数据报的消息的形式进行交换，在
// 数据报套接字中，消息边界得到了保留，但数据传输是不可靠的。消息的到达可能是无序的、重复
// 的或者根本就无法到达。数据报套接字是更一般的无连接套接字，与流式套接字不同，一个数据报
// 套接字在使用时无需与另一个套接字连接。数据包套接字可以与另一个套接字进行连接，但其语义
// 与连接的流式套接字不同。在 internet domain 中，数据包套接字使用了用户数据报协议（UDP），
// 而流式套接字则通常使用传输控制协议（TCP），一般来讲在称呼这两种套接字时更偏向于使用术语
// UDP 套接字和 TCP 套接字。
//
// 套接字 I/O 操作可以使用传统的 read() 和 write() 系统调用，或者使用一组套接字特有的
// 系统调用，如 send() recv() sendto() recvfrom() 来完成。在默认情况下，这些系统调用
// 在 I/O 操作无法被立即完成时会阻塞。通过使用 fcntl() F_SETFL 操作启用 O_NONBLOCK
// 打开文件状态标记可以执行非阻塞 I/O。
//
// UDP 协议格式：
//      [源IP地址 4B|目的IP地址 4B|0 1B|17 1B|UDP长度 2B]      伪首部
//      [源端口 2B|目的端口 2B|整体长度 2B|校验和 2B][数据部分]  UDP数据报
//
// UDP 用户数据报头部中校验和的计算方法有些特殊，在计算校验和时，要在UDP用户数据报之前
// 增加12个字节的伪首部，伪首部仅仅为了计算校验和而用。UDP 和 TCP 使用的校验和的长度只有
// 16位，并且只是简单的“总结性”校验和，因此无法检测出特定的错误，其结果是无法提供较强的
// 错误检测机制。繁忙的互联网服务器通常只能每隔几天看一下未检测出的传输错误的平均情况。需
// 要更多确保数据完整性的应用程序可以使用安全套接字层（Secure Sockets Layer, SSL），它
// 不仅仅提供了安全的通信，而且还提供更加严格的错误检测过程。或者应用程序也可以实现自己的
// 错误控制机制。
//
// https://www.man7.org/linux/man-pages/man7/tcp.7.html
//
// TCP 协议格式：前20个字节固定，后面有4n字节根据需要增加选项
//      [源端口 2B][目的端口 2B]
//      [序号 4B]                                   TCP是面向字节流的，在TCP连接中传送的每个字节都按顺序编号，字节流的起始序号在连接建立时设置，序号字段表示的是第一个字节的序号
//      [确认号 4B]                                 期望收到对方下一个报文段的第一个数据字节的序号
//      [数据偏移|保留|URG|ACK|PSH|RST|SYN|FIN 2B]   数据偏移字段（4-bit）表示数据部分距离头部起始位置有多远，以4字节为单位，最大可以表示15*4=60字节，这也是TCP首部的最大长度
//      [窗口 2B]                                   发送本报文段的一方的接收窗口大小，窗口值告诉对方：从本报文段首部中的确认号算起，接收方目前可以接收的数据量
//      [校验和 2B][紧急指针 2B]                     紧急指针仅在 URG=1 时才有意义，指出本报文段中的紧急数据的字节数，紧急数据结束后才是普通数据，注意即使窗口为零也可发送紧急数据
//      [可选选项|填充]                              可选字段最长40个字节
//      [数据部分]
//      紧急 URG=1 表示紧急指针字段有效，它告诉系统此报文段中有紧急数据，应尽快传送（相当于高优先级数据），而不要按原来的排队顺序传送
//      确认 ACK=1 表示确认号字段有效，TCP 规定，在连接建立后所有传送的报文段都必须把 ACK 置为 1
//      推送 PSH=1 当两应用交互时，有时希望键入命令后立即就能收到对方的响应，此时TCP可以使用推送操作，把PSH置1并立即创建一个报文发送出去，接收方也会尽快交付不等缓存满，虽然程序可以选择推送操作但还很少使用
//      复位 RST=1 表示TCP连接出现了严重差错，如主机崩溃或其他原因，必须释放连接，然后再重新建立连接，复位还用来拒绝一个非法的报文段或拒绝打开一个连接
//      同步 SYN=1 在连接建立时用来同步序号，当SYN=1而ACK=0时表明这是一个连接请求报文，对方若同意建立连接则应响应SYN=1和ACK=1
//      终止 FIN=1 用来释放连接，当FIN=1时表示此报文段的发送方的数据已发送完毕，并要求释放连接
//      窗口字段明确指出现在允许发送方可以发送的数据量，窗口值是经常在动态变化的值。
//      可选选项最初只有最大报文段长度 MSS（Max Segment Size），即 TCP 数据报数据字段的最大长度；后面增加了窗口扩大选项、时间戳选项、选择确认（SACK）选项等等。
//      窗口扩大选项是为了扩大窗口，原始窗口大小最大 64KB 字节，虽然对早期的网络是足够的，但对于包含卫星信道的网络，传播时延和带宽都很大，要获得高吞吐率需要更大的窗口大小。
//      时间戳选项占10字节，其中最主要的字段时间戳值（4字节）和时间戳回送回答（4字节），用来计算往返时间 RTT，以及处理 TCP 序号超过 32-bit 的情况，又称为防止序号绕回 PAWS。
//
// 确认、重传、超时：当一个 TCP 段无错地到达目的地时，接收 TCP 会向发送者发送一个确认，
// 通知它数据接收成功。如果一个段在到达时存在错误，那么这个段会被丢弃，确认也不会被发送。
// 为了处理段永远不能到达或被丢弃的情况，发送者在发送每一个段时会开启一个定时器。如果在
// 定时器超时之前没有收到确认，那么就会重传这个段。由于所使用的网络以及当前的流量负载会
// 影响传输一个段和接收其确认所需的时间，因此 TCP 采用了一个算法来动态地调整重传超时时间
// （RTO）的大小。接收 TCP 可能不会立即发送确认，而是会等待即毫秒来观察一下是否可以将确
// 认塞进接收者返回给发送者的响应中。这项被称为延迟 ACK 的技术的目的时能少发一个 TCP
// 段，从而降低网络中包的数量以及降低发送和接收主机的负载。
//
// MSS 并不是考虑接收方应用层的接收缓存可能放不下 TCP 报文段中的数据，实际上 MSS 与接收
// 窗口值没有关系，它考虑的时网络底层的数据传送能力。我们知道 TCP 报文段的数据部分，至少
// 要加上 40 字节的头部（包括TCP头部和IP头部）才能组装成一个IP数据报，若选择较小的 MSS
// 网络利用率就较低。但反过来，若TCP报文段非常长，那么在 IP 层传输时就可能要分解成多个短
// 数据片，在终点要把收到的各个短数据分片装配成原来的TCP报文段。当传输出错时还要进行重传，
// 这些也都会使开销增大。因此，MSS 应尽可能大些，只要在 IP 层传输时不需要再分片就行。由于
// IP 数据报所经历的路径时动态变化的，因此在这条路径上确定的不需要分片的 MSS，如果改走另
// 一条路径就可能需要进行分片，因此最佳的 MSS 是很难确定的。在连接建立的过程中，双方都把
// 字节能够支持的 MSS 写入这一字段，以后就按照这个数值传送数据，两个传送方向可以有不同的
// MSS 值。RFC 879 指出，流行的一种说法是在 TCP 连接建立阶段双方协商 MSS 值，但这是错误
// 的，因为这里并不存在任何的协商，而只是一方把 MSS 值设定好以后通知另一方而已。若主机未
// 填写这一项，则 MSS 的默认值是 536 字节，因此所有在因特网上的主机都应能接受的数据报长度
// 为 536 + 20（TCP固定首部）+ 20（IP固定首部）= 576 字节。
//
// 拥塞控制：慢启动和拥塞避免算法。TCP 的拥塞控制算法被设计用来防止快速的发送者压垮整个
// 网络。如果发送 TCP 的速度要快于一个中间路由器转发的速度，那么该路由器就会开始丢弃包。
// 这将会导致较高的包丢失率，其结果是如果 TCP 保持以相同的速度发送这些被丢弃的段就会极大
// 地降低网络性能。TCP 的拥塞控制算法在以下两个场景中比较重要：（一）在连接建立后或空闲
// 一段时间后继续发送，发送者可以立即向网络中注入尽可能多的分段，只要接收者报告的窗口大小
// 允许即可。这里的问题在于如果网络无法处理这种分段洪泛，那么发送者会存在立即压垮整个网络
// 的风险。（二）当拥塞被检测到时，如果发送 TCP 检测到发生了拥塞，那么它就必须降低其传输
// 速率，TCP 根据分段丢失来检测是否发生了拥塞，因为传输错误率时非常低的，即如果一个包丢
// 失了就认为发生了拥塞。
//
// TCP 的拥塞控制策略组合采用了两种算法：慢启动和拥塞避免。慢启动算法会使发送 TCP 在一开
// 始的时候以低速传输分段，但同时允许它以指数级的速度提高其速率，只要这些分段都得到接收
// TCP 的确认。慢启动能够防止一个快速的 TCP 发送者压垮整个网络。但如果不加限制的话，慢
// 启动在传输速率上的指数级增长意味着发送者在短时间内就会压垮整个网络。TCP 的拥塞控制避免
// 算法用来防止这种情况的发生，它为速率的增长安排了一个管理实体。有了拥塞避免之后，在连接
// 刚建立时，发送 TCP 会使用一个较小的拥塞窗口，它会限制所能传输的未确认的数据数量。当发
// 送者从对等 TCP 处收到确认时，拥塞窗口在一开始时会呈现指数级增长。但一旦拥塞窗口增长到
// 一个被认为是接近网络传输容量的阈值时，其增长速度就会编程线性，而不是指数级的。对网络
// 容量的估算时根据检测到拥塞时的传输速率来计算得出的，或者在一开始建立连接时设定为一个
// 固定值。在任何时刻，发送 TCP 传输的数据数量还会受到接收 TCP 的接收窗口和本地的 TCP
// 发送缓冲器大小的限制。慢启动和拥塞避免算法组合起来使得发送者可以快速地将传输速度提升
// 至网络的可用容量，并且不会超出该容量。这些算法的作用时允许数据传输快速地到达一个平衡
// 状态，即发送者传输包的速率与它从接收者处接收确认的速率一致。
//
// TCP 连接的建立：
//      主机（A）                               主机（B）
//  CLOSED                                  CLOSED（LISTEN）
//      SYN=1 seq=x ACK=0 ---------------------->           （1）
//  SYN-SENT                                SYN-RCVD
//      <-------------- SYN=1 seq=y ACK=1 ack=x+1           （2）如果不想接受连接，不予理睬或发送RST
//  ESTABLISHED
//      SYN=0 seq=x+1 ACK=1 ack=y+1 ------------>           （3）第一个数据包
//                                          ESTABLISHED
//
// TCP 规定 SYN=1 的报文段不能携带数据，但要消耗掉一个字节序号，请求端和接收端都要选择一
// 个自己的起始序号。TCP 规定 ACK=1 的报文段可以携带数据，但如果不携带数据则不消耗序号。
// 例如第（3）步中如果不携带数据，下一个数据报文段的序号仍是 seq=x+1。三次握手，请求端
// 为什么要再次发送一次确认呢？这主要是为了防止已经失效的连接请求报文突然又被传送到接收端
// 而引起错误。
//
// 所谓的“已失效的连接请求报文”是这样产生的，考虑一种正常情况，A 发送连接请求，但因连接
// 报文丢失而未收到确认，于是 A 再重传一次连接请求。后来收到了确认，建立了连接，这里 A
// 共发了两个连接请求报文，其中第一个丢失，第二个到达了 B。现在假设出现一种异常情况，即
// A 发送出去的第一个连接请求并没有丢失，而是在某些网络节点长时间滞留了，以致延误到连接
// 释放以后的某个时间才到达 B，本来这是一个早已失效的报文。但 B 收到后误认为 A 又发出了
// 一次连接请求，于是就像 A 发出确认同一建立连接。由于现在 A 并没有发出建立连接的请求，
// 因此不会理睬 B，但 B 却以为新的连接已经建立了，并一直等待 A 发送数据，这样 B 的许多
// 资源就白白浪费了。而三次握手需要 A 的确认 B 才能认为连接建立成功，即使第一个丢失的请
// 求在连接释放后再次到达 B，B 发送了确认，但没有 A 的确认 B 不会认为连接建立成功。
//
// TCP 连接的释放：数据传输结束后，通信的双方都可以释放连接，u 等于前面已经传送过的数据的最后一个字节的序号加1
//      主机（A）                               主机（B）
//  ESTABLISHED                             ESTABLISHED
//      =============== 数据传输完毕 ============>
//      FIN=1 seq=u ACK=1 ack=v ---------------->           （1）接收端通知应用程序，等待应用程序发送被动关闭
//  FIN-WAIT-1                              CLOSE-WAIT
//      <-------------- FIN=0 seq=v ACK=1 ack=u+1           （2）
//  FIN-WAIT-2                              CLOSE-WAIT
//      <============== 数据传输完毕 =============
//      <-------------- FIN=1 seq=w ACK=1 ack=u+1           （3）接收端应用程序最终决定关闭
//  TIME-WAIT                               LAST-ACK
//      FIN=0 seq=u+1 ACK=1 ack=w+1 ------------>           （4）
//  TIME-WAIT                               CLOSED
//  等待 2MSL
//  CLOSED
//
// TCP 规定，FIN=1 报文即使不携带数据，也要消耗掉一个序号。TCP 连接的释放分为两个方向，
// 两个方向都释放才完成。第（2）之后，一个方向的连接就释放了，此时 TCP 连接处于半关闭
// （half-close）状态，即 A 已经没有数据要发送了，但 B 若发送数据，A 仍然要接收。也就
// 是说，从 B 到 A 这个方向的连接并未关闭，这个状态可能会持续一些时间。若 B 也已经没有
// 要向 A 发送的数据，其应用程序就通知 TCP 释放连接，这时 B 发送释放请求，等待释放的最
// 后确认（LAST-ACK），B 只要收到最后确认就认为连接成功释放了。
//
// 但在 A 发送最后确认之后，需要等待 TIME-WAIT 计时器设置的时间 2MSL 之后，A 才进入释放
// 状态。时间 MSL 是最长报文段寿命（Max Segment Lifetime），RFC 793 建议设置为 2 分钟。
// 但这完全是从工程上来考虑，对于现在的网络，MSL=2分钟可能太长了一些，因此 TCP 允许不同
// 的实现可根据具体情况使用更小的 MSL 值。也因此，A 在 TIME-WAIT 之后需要经过 4 分钟后
// 才进入释放状态，才能开始建立下一个新的连接。为什么必须等待 2MSL 时间呢，有两个理由。
// （一）为了保证 A 发送的最后确认报文段能够到达 B，这个确认报文段有可能丢失，B 收不到
// 最后的确认会超时重传断连请求，这是 A 就能在 2MSL 时间内收到这个重传请求接着重发一次
// 确认再重新启动 2MSL 计时器。（二）防止上文提到的“已失效的连接请求”出现在本连接中，A
// 在发送完最后一个确认报文后，再经过 2MSL 时间，就可以使本连接持续的时间内所产生的所有
// 报文段都从网络中消失。这样就可以使下一个新的连接中不会出现这种旧的连接请求报文段。
//
// 另外，TCP 还设有一个保活计时器（keeplive timer），设想这样的情况，客户已主动与服务器
// 建立了 TCP 连接，但后来客户端主机突然出故障。显然服务器以后就不能再收到客户发来的数据，
// 因此应当有措施使服务器不要再白白等待下去。这就是使用保活计时器，服务器每收到一次客户的
// 数据，就重新设置保活计时器，时间的设置通常是两小时。若两小时没有收到客户的数据，服务器
// 就发送一个探测报文段，以后则每隔75分钟发送一次，若一连发送10个探测报文后仍无客户的响应，
// 服务器就认为客户端出了故障，接着就关闭这个连接。
//
// IP 协议格式：前20个字节固定，后面4n字节可选
//      [版本|首部长度 1B][区分服务 1B][总长度 2B]      区分服务（DS）实际上一直没有被用过
//      [标识 2B]                                    对每一个用户数据报标识都会加一，如果由于超过MTU而需要分片，分片的IP数据包中的标识都相同
//      [标志|片偏移 2B]                              标志：MF=1 还有分片 MF=0 最后一个分片 DF=1 不能分片 DF=0 可以分片；片偏移（13-bit）：分片在原片中的偏移，以8字节为单位
//      [生存时间 1B]                                 TTL（Time To Live）：数据报在网络中的寿命，定义为被路由器转发的跳数限制，而设置为1则只能在本局域网中传送
//      [协议 1B][首部检验和 2B]                      协议：ICMP 1 IGMP 2 IP 4 TCP 6 EGP 8 IGP 9 UDP 17 IPv6 41 ESP 50 OSPF 89
//      [源地址 4B][目的地址 4B]
//      [可选字段|填充]
//      [数据部分]
//
// IP 层之下的每一种数据链路层协议都规定了一个数据帧中的数据字段的最大长度，称为最大传送
// 单元 MTU （Max Transfer Unit）。当一个IP数据报封装成链路层帧时，此数据报的总长度，即
// IP 首部加上数据部分一定不能超过下面的数据链路层所规定的MTU值。例如，最常用的以太网就
// 规定器 MTU 值是 1500 字节。若所传送的数据报长度超过数据链路层的MTU值，就必须把过长的
// 数据报进行分片处理。虽然使用尽可能长的IP数据报会使传送效率提高（因为头部长度占比就会
// 变小），但数据报短些也有好处。每一个IP数据报越短，路由器转发的速度就越快。为此 IP 协议
// 规定，在因特网中所有的主机和路由器，必须能够接受长度不超过 576 字节的数据报。这是假定
// 上层交下来的数据长度有 512 字节（合理的长度），加上最长的IP首部 60 字节，再加上 4 字
// 节的富裕量，就得到 576 字节。当主机需要发送长度超过 576 字节的数据报时，应当先了解一
// 下，目的主机能否接受所要发送的数据报长度，否则就要进行分片。
//
// IP 是一种无连接不可靠协议，它尽最大可能将数据报从发送者传输给接收者，但并不保证包到达
// 的顺序会与它们被传输的顺序一致，也不保证包是否重复，甚至都不保证包是否到达接收者。IP
// 也没有提高错误恢复，头信息错误的包会被静默地丢弃。可靠性是通过使用一个可靠的传输层协议
// 例如TCP或应用程序本身来保证的。
//
// IP 数据报分片的发生对于高层协议是透明的，但一般来讲并不希望分片的发生。这里的问题在于
// IP 并不进行重传并且只有在所有分片都到达目的地之后才能对数据报进行组装，因此如果其中一
// 些分片丢失或包含传输错误的话，会导致整个数据报不可用。在一些情况下，这会导致极高的数据
// 丢失率或降低传输速率。现代 TCP 实现采用了一些算法（路径MTU发现）来确定主机之间的一条
// 路径的 MTU，并根据该值对传递给 IP 的数据进行分解，这样 IP 就不会碰到需要传输大小超过
// MTU 的数据报的情况了。UDP 并没有提供这种机制，基于 UDP 的应用程序通常不会知道源主机
// 和目的主机之间路径的 MTU。一般来讲，基于 UDP 的应用程序会采用保守的方法来避免 IP 分片，
// 即确保传输的 IP 数据报的大小小于 IPv4 的组装缓冲区大小的最小值 576 字节。
//
// 众所周知的端口 [0, 1023] 已经永久地分配给特定的应用程序，它是由中央授权机构互联网号码
// 分配局（IANA）来分配的。IANA 还记录着注册端口，对这些端口的分配就不那么严格了，这也意
// 味着一个实现无需保证这些端口是否真正用于它们注册时申请的用途。IANA 注册端口范围是
// [1024, 41951]，不是所有位于这个范围内的端口都被注册了。IANA 众所周知端口和注册端口分
// 配情况可查看：http://www.iana.org/assignments/port-numbers 。在大多数 TCP/IP 实现
// 中，包括 Linux，范围在 0 到 1023 间的端口号也是特权端口，这意味着只有特权 CAP_NET_BIND_SERVICE
// 进程可以绑定到这些端口上，从而防止普通用户通过实现恶意程序，如伪造 ssh，来获取密码。有
// 些时候，特权端口也被称为保留端口。
//
// 尽管端口号相同的 TCP 和 UDP 端口是不同的实体，但同一个众所周知的端口号通常会同时被分配
// 给基于 TCP 和 UDP 的服务，即使该服务通常只提供其中一种协议服务。这种惯例避免了端口号
// 在两个协议中产生混淆的情况。
//
// 如果一个应用程序没有选择一个特定的端口，即没有调用 bind() 将套接字绑定到一个特定的端口
// 上，那么 TCP 和 UDP 会为该套接字分配一个唯一的临时端口（即存活时间较短）。在这种情况
// 下，应用程序，通常是一个客户端，并不关心它所使用的端口号。TCP 和 UDP 在将套接字绑定到
// 端口 0 也会分配一个临时端口号。IANA 将位于 [49152, 65535] 之间的端口称为动态或私有
// 端口，这表示这些端口可供本地应用程序使用或作为临时端口分配。然后不同的实现可能会在不同
// 的范围内分配临时端口。在 Linux 上，这个范围是有包含在以下文件中的两个数值来定义的：
// cat /proc/sys/net/ipv4/ip_local_port_range
//
// https://www.man7.org/linux/man-pages/man2/socket.2.html
//
// #include <sys/socket.h>
// int socket(int domain, int type, int protocol);
//      domain - 指定套接字通信域，AF_UNIX AF_LOCAL sockaddr_un，AF_INET sockaddr_in，AF_INET6 sockaddr_in6
//      type - 流式套接字 SOCK_STREAM，数据报套接字 SOCK_DGRAM，裸套接字 SOCK_RAW。
//          从内核 2.6.27 开始，Linux 为 type 参数提供了第二种用途，允许两个非标准的标
//          记 SOCK_CLOEXEC SOCK_NONBLOCK，第一个标记参见 open() O_CLOEXEC，第二个
//          标记可以让内核在创建套接字文件描述符后直接设置好 O_NONBLOCK 标记，从而无需
//          再通过 fcntl() 来设置。
//      protocol - 一般总是指定为 0，但在裸套接字中（SOCK_RAW）会指定为 IPPROTO_RAW
// int bind(int sofd, const struct sockaddr *addr, socklen_t addrlen);
//      将套接字绑定到一个本地地址上，一般来讲，会将一个服务器的套接字绑定到一个众所周知
//      的地址上，即一个固定的与服务器进行通信的客户端应用程序提前知道的地址。除此之外还
//      有其他做法，例如对于一个 internet domain 套接字来说，服务器可以不调用 bind()
//      而直接调用 listen()，这将会导致内核为该套接字选择一个临时端口，之后服务器可以
//      使用 getsockname() 来获取套接字的地址。在这种场景中，服务器必须要发布其地址使得
//      客户端能够知道如何定位到服务器的套接字。这种发布可以通过向一个中心目录服务应用程
//      序注册服务器的地址来完成，之后客户端可以通过这个服务来获取服务器的地址。当然，目录
//      服务应用程序的套接字必须要位于一个众所周知的地址上。
//      #include <netinet/in.h>
//      struct sockaddr {
//          sa_family_t sa_family;
//          char sa_data[14];
//      };
//      struct sockaddr_in {            // 'in' is for internet
//          sa_family_t    sin_family;  // address family: AF_INET
//          in_port_t      sin_port;    // port in network byte-order 端口和地址必须是网络字节序
//          struct in_addr sin_addr;    // internet address in network byte-order: struct in_addr { uint32_t s_addr; } INADDR_ANY INADDR_LOOPBACK
//          unsigned char __pad[X];     // pad to size of sockaddr (16-byte)
//      };
//      struct sockaddr_in6 {
//          sa_family_t     sin6_family;   // AF_INET6
//          in_port_t       sin6_port;     // port number
//          uint32_t        sin6_flowinfo; // IPv6 flow information
//          struct in6_addr sin6_addr;     // IPv6 address: struct in6_addr { unsigned char s6_addr[16]; } IN6ADDR_ANY_INIT
//          uint32_t        sin6_scope_id; // Scope ID (new in kernel 2.4)
//      };
//      struct sockaddr_storage { // IPv6 套接字 API 中引入了一个通用的 sockaddr_storage 结构，提供的空间足以存储任意类型的套接字地址
//          sa_family_t ss_family;
//          __ss_aligntype __ss_align;
//          char __ss_padding[SS_PADSIZE];
//      };
// int listen(int sofd, int backlog);
//      将流式套接字标记为被动，这个套接字后面会被动接受来自于其他套接字的连接。无法在一
//      个已连接的套接字（即已经成功执行 connect 的套接字或由 accept 调用返回的套接字）
//      上执行 listen()。监听套接字会保持打开状态，并且可以持续接受后续的连接请求。
//      参数 backlog，要理解 backlog 的用途首先需要注意客户端可能会在服务器调用 accept
//      之前调用 connect，这种情况是有可能发生的，如服务器可能正忙于处理其他客户端。这将
//      产生一个未决的连接。内核必须要记录所有未决的连接请求的相关信息，这样后续 accept
//      就能够处理这些请求。backlog 参数指定这种未决连接的最大数量，在这个限制之内的连接
//      请求会立即成功，之外的连接请求就会阻塞直到一个未决的连接请求被 accept 调用接受，
//      并从未决连接队列删除为止。SUSv3 允许实现为 backlog 的可取值规定一个上限并允许一
//      个实现静默地将 backlog 值向下舍入到这个限制值。SUSv3 规定实现应该通过在定义
//      SOMAXCONN 常量来发布这个限制，在 Linux 上这个常量的值被定义程了 128。但从内核
//      2.4.25 起，Linux 允许在运行时通过 Linux 特有的 /proc/sys/net/core/somaxconn
//      文件来调整这个限制。在最初的 BSD 套接字实现中，backlog 的上限是 5，并且在较早
//      的代码中可以看到这个数值。所有现代实现允许为 backlog 指定更高的值，这对于使用
//      TCP 套接字实现大量客户的网络服务器来讲是有必要的。
//      cat /proc/sys/net/core/somaxconn
//      4096
// int accept(int sofd, struct sockaddr *addr, socklen_t *addrlen);
//      接受监听流套接字上的一个接入连接，如果不存在未决的连接，那么调用 accept 就会阻塞
//      直到有连接请求到达为止。理解 accept() 的关键点是它会创建一个新 socket，并且正是
//      这个新套接字会与执行 connect() 的对等套接字进行连接。accept() 调用的返回结果是
//      已连接的套接字文件描述符。
//      addrlen 是一个 in-out 参数，执行之前必须将它初始化为 addr 指向的缓冲区的大小，
//      这样内核就知道有多少空间可用于返回套接字地址了。当 accept 返回后，这个值会被设置
//      程实际被复制进缓冲区的数据的字节数。如果不关心对等套接字的地址，可以将 addr 和
//      addrlen 设置为 NULL 和 0。这样可以在后面某个时刻使用 getpeername() 系统调用来
//      获取对端的地址。
//      从内核 2.6.28 开始，Linux 支持一个新的非标准系统调用 accept4()，这个系统调用
//      执行的任务与 accept() 相同，但支持一个额外的参数 flags，可以设置 SOCK_CLOEXEC
//      和 SOCK_NONBLOCK 两个标志。
//      由于 accept() 在一些老式实现版本中并不是一个原子化的系统调用，因此可能需要通过一
//      些互斥技术，以确保每次只有一个进程/线程可以执行 accept() 调用。
// int connect(int sofd, const struct sockaddr *addr, socklen_t addrlen);
//      如果 connect 失败并且希望重新进行连接，那么 SUSv3 规定完成这个任务的可移植的方
//      法是关闭这个套接字，创建一个新套接字，在该新套接字上重新进行连接。
//
// 流套接字就像是打电话进行通信，而数据包套接字就像是邮政系统，socket() 的调用等价于创建
// 一个邮箱，与邮政系统一样，当从一个地址向另一个地址发送多个数据包（信）时无法保证它们
// 按照被发送的顺序到达，甚至无法保证它们都能够到达。数据包还新增了邮政系统所不具备的一个
// 特定，由于底层网络协议有时候会重新传输一个数据包，因此同样的数据包可能会多次到达。
// 在 Linux 上可以使用 sendto() 发送长度为 0 的数据报，但不是所有的 UNIX 实现都允许这样
// 做。SUSv3 在解除对等关系方面的论断是比较模糊的，它只是声称通过调用一个指定了“空地址”
// 的 connect() 调用可以重置一个连接。SUSv4 则明确规定了需要使用 AF_UNSPEC。在一些 TCP/IP
// 实践中，将一个数据报连接到一个对等套接字能够带来性能上的提升。在 Linux 上连接一个数据
// 报套接字能对性能产生些许差异。设置一个对等套接字主要对那些需要向单个对等套接字（通常是
// 某种数据报客户端）发送多个数据报的应用程序是比较有用的。
//
// 尽管数据报套接字是无连接的，但在数据报套接字上应用 connect() 系统调用仍然起作用。在
// 数据报套接字上调用 connect() 会导致内核记录这个套接字的对等地址。此后发送数据就不要
// 带目的地址的参数了，数据报的发送可以使用 write() 或 send() 来完成，与 sendto() 一样，
// 每个 write() 调用会发送一个独立的数据报。另外这个套接字上只能读取对等套接字发送的数据
// 报。通过再调用一次 connect() 可以修改已经绑定的目的套接字地址，此外通过指定一个地址族
// 为 AF_UNSPEC 的地址结构还可以解除地址绑定，但注意的是，其他很多 UNIX 实现并不支持将
// AF_UNSPEC 用于这个用途。

int prh_raw_socket(void) {
    int raw_socket = socket(AF_INET, SOCK_RAW | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_RAW);
    assert(raw_socket >= 0);
    return raw_socket;
}
int prh_tcp_socket(void) {
    int tcp_socket = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    assert(tcp_socket >= 0);
    return tcp_socket;
}
int prh_udp_socket(void) {
    int udp_socket = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    assert(udp_socket >= 0);
    return udp_socket;
}

// #include <sys/socket.h>
// int shutdown(int sockfd, int how);
//
// shutdown() 调用会关闭与 sockfd 关联的套接字上的全双工连接的全部或部分。SHUT_RD 之后
// 不允许继续读，SHUT_WR 之后不允许继续写，SHUT_RDWR 之后不允许继续读写。
// EBADF - sockfd 是非法文件描述符。EINVAL - how 是非法值。ENOTCONN - 指定的套接字没
// 有连接。ENOTSOCK - sockfd 不是套接字文件描述符。
void prh_sock_shut_read(int sofd) {
    // 关闭连接的读取端，之后的读操作将返回文件尾。数据仍然可以写入到套接字上，在 UNIX
    // 域（UNIX domain）流式套接字上执行 SHUT_RD 操作后，对端应用程序将接收到 SIGPIPE
    // 信号，如果继续尝试在对端套接字上做写操作将产生 EPIPE 错误。SHUT_RD 对于 TCP 套
    // 接字来说没有什么意义。
    prh_zeroret(shutdown(sofd, SHUT_RD));
}
void prh_sock_shut_write(int sofd) {
    // 关闭连接的写入端，后续对本地套接字的写操作将产生 SIGPIPE 信号以及 EPIPE 错误，而
    // 由对端写入的数据仍然可以在套接字上传送。换句话说，这个操作允许我们仍然能读取对端
    // 发来的数据的同时，通知对方自己的写入端已经关闭，对方可以通过读取到文件尾来完成所有
    // 的文件读取。在 shutdown() 中最常用到的操作就是 SHUT_WR，有时候也被称为半关闭套
    // 接字。
    prh_zeroret(shutdown(sofd, SHUT_WR));
}
void prh_sock_shut_read_write(int sofd) {
    // 将连接的读取端和写入端都关闭，这等用于先执行 SHUT_RD 跟着再执行 SHUT_WR 操作。
    // 除了 how 参数的语义外，shutdown() 同 close() 之间的另一个重要区别是：无论套接字
    // 上是否还关联其他的文件描述符，shutdown() 都会关闭套接字通道。换句话说，shutdown
    // 是根据打开的文件描述（open file description）来执行操作，而与文件描述符无法。
    // 例如 fd2 = dup(sockfd); close(sockfd); 连接仍然会保证打开状态，我们仍然可以通
    // 过文件描述符 fd2 在该连接上执行 I/O 操作。但是 shutdown(sockfd, SHUT_RDWR) 后
    // 那么该连接的双向通道都会关闭，通过 fd2 也无法再执行 I/O 操作了。如果套接字文件描
    // 述符在 fork() 时被复制，那么此时也会出现相似的场景。如果在 fork() 调用之后，一个
    // 进程在描述符的副本上执行一次 SHUT_RDWR 操作，那么其他进程就无法再再这个文件描述符
    // 执行 I/O 操作了。**需要注意的是**，shutdown() 并不会关闭文件描述符，就是参数指定
    // 为 SHUT_RDWR 时也一样，要关闭文件标书费，我们必须另外调用 close()。
    prh_zeroret(shutdown(sofd, SHUT_RDWR));
}

// #include <unistd.h>
// int close(int fd);
//
// close() 关闭一个文件描述符，使其不再指向任何文件，并可被重用。该描述符关联的文件上由
// 本进程持有的所有记录锁（参见 fcntl(2)）都会被移除，无论当初是通过哪个文件描述符获得的
// 锁。这会带来一些不幸的后果，因此在使用建议性记录锁（advisory record lock）时应格外小
// 心。有关风险与后果的讨论，以及（可能更推荐的）open file description locks，请参见
// fcntl(2)。如果 fd 是底层打开文件描述（参见 open(2)）的最后一个引用，则与该打开文件
// 描述相关的资源将被释放；如果该文件描述符是对一个已通过 unlink(2) 删除的文件的最后一个
// 引用，则该文件会被删除。
//
// 成功返回0，错误返回-1，errno记录对应的错误。EBADF 非法文件描述符。EINTR 被信号中断。
// EIO 发生I/O错误。ENOSPC EDQUOT 在 NFS 上，该错误通常不会在对首次超出可用存储空间的
// write() 调用中立即返回，而是在随后的 write(2)、fsync(2) 或 close() 中才报告。
// close() 在出错后不应该重试。
//
// 一次成功的 close 并不保证数据已真正写回磁盘，因为内核利用缓冲区缓存来延迟写操作。通常，
// 文件系统在文件关闭时不会刷新缓冲区。如果你必须确认数据已物理写入底层磁盘，请使用
// fsync(2)，此时还取决于磁盘硬件本身。close-on-exec 文件描述符标志可确保在成功执行
// execve(2) 时自动关闭该描述符；详见 fcntl(2)。
//
// 多线程进程与 close()：在同一进程的其他线程可能正在使用该文件描述符进行系统调用时关闭
// 它，可能并不明智。由于文件描述符可能被重用，存在一些隐蔽的竞争条件，可能导致意想不到的
// 副作用。进一步考虑以下场景：两个线程对同一文件描述符进行操作：(1) 一个线程在该描述符上
// 的 I/O 系统调用中阻塞。例如，它正尝试 write(2) 到一个已满的管道，或尝试 read(2) 一个
// 当前没有可用数据的数据流套接字。(2) 另一个线程关闭了该文件描述符。此情况下的行为因系统
// 而异。在某些系统上，当文件描述符被关闭时，阻塞的系统调用会立即返回并报告错误。在 Linux
// （可能还有其他一些系统）上，行为不同：阻塞的 I/O 系统调用持有对底层打开文件描述的引用，
// 该引用会在 I/O 系统调用完成前保持描述符打开，参见 open(2) 中关于打开文件描述的讨论。
// 因此，第一个线程中的阻塞系统调用可能在第二个线程执行 close() 后仍然成功完成。
//
// 处理 close() 的错误返回值：谨慎的程序员会检查 close() 的返回值，因为先前 write(2)
// 操作的错误可能仅在最终释放打开文件描述的 close() 时才报告。关闭文件时不检查返回值可能
// 导致数据静默丢失。这种情况在使用 NFS 和磁盘配额（disk quota）时尤为明显。然而请注意，
// 失败返回仅应用于诊断目的（即向应用程序警告可能仍有 I/O 未完成或曾发生失败的 I/O）或补
// 救目的（例如，再次写入文件或创建备份）。在 close() 返回失败后重试 close() 是错误的，
// 因为这可能导致关闭另一个线程已重用的文件描述符。这是因为 Linux 内核总是在关闭操作早期
// 就释放文件描述符，使其可被重用；而可能返回错误的步骤（如将数据刷新到文件系统或设备）发
// 生在关闭操作的后期。许多其他实现同样总是关闭文件描述符（EBADF 情况除外，表示文件描述符
// 无效），即使随后从 close() 返回时报告错误。POSIX.1 目前对此未作规定，但计划在下一次
// 主要标准修订中强制这一行为。
//
// 希望了解 I/O 错误的谨慎程序员可在 close() 之前调用 fsync(2)。EINTR 错误是一种较为特
// 殊的情况。关于 EINTR 错误，POSIX.1-2008 规定：如果 close() 被要捕获的信号中断，它将
// 返回 -1 并将 errno 设为 EINTR，此时 fildes 的状态未定义。这允许发生在 Linux 和许多
// 其他实现上的行为：如同 close() 可能报告的其他错误一样，文件描述符保证已被关闭。然而，
// 它也允许另一种可能：实现返回 EINTR 错误并保持文件描述符打开。根据其文档，HP-UX 的
// close() 就是这样。调用者必须再次使用 close() 关闭该文件描述符，以避免文件描述符泄漏。
// 实现行为上的这种差异为可移植应用程序带来了难题，因为在许多实现上，EINTR 错误后绝不能
// 再次调用 close()，而在至少一种实现上必须再次调用。POSIX.1 下一次主要发布计划解决这一
// 困境。
void prh_sock_close(int fd) {
    prh_preno_if(close(fd));
}

// #include <sys/socket.h>
// int getsockname(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
// int getpeername(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
// getsockname() 可以获取映射绑定的套接字本地地址，包括ip和端口。内核会在出现如下情况时
// 执行一个隐式绑定：（1）已经在 TCP 套接字上执行了 connect() 或 listen() 调用，但之前
// 并没有调用 bind() 绑定到一个地址上。（2）当在 UDP 套接字上首次调用 sendto() 时，该
// 套接字还没有绑定到地址上。（3）调用 bind() 时将端口号设置为 0，这种情况下内核会选择一
// 个临时的端口号给该套接字使用。
#include <sys/socket.h>
void prh_sock_local_address(int sofd, struct sockaddr_in *addr) {
    socklen_t addrlen = sizeof(struct sockaddr_in); // EBADF sockfd invalid, EFAULT addr invalid, EINVAL addrlen, ENOBUFS insufficient resources, ENOTSOCK sockfd
    prh_zeroret(getsockname(sofd, (struct sockaddr *)addr, &addrlen));
    assert(addrlen == sizeof(struct sockaddr_in));
}
void prh_sock_peer_address(int sofd, struct sockaddr_in *addr) {
    socklen_t addrlen = sizeof(struct sockaddr_in); // ENOTCONN socket is not connected
    prh_zeroret(getpeername(sofd, (struct sockaddr *)addr, &addrlen));
    assert(addrlen == sizeof(struct sockaddr_in));
}

// #include <arpa/inet.h>
// int inet_pton(int af, const char *restrict src, void *restrict dst); 返回1成功，0非法地址字符串，-1非法地址族
// const char *inet_ntop(int af, const void *restrict src, char dst[restrict .size], socklen_t size);
#include <arpa/inet.h>
prh_u32 prh_sock_ip_address(const char *ip_string) {
    struct in_addr out; // ddd.ddd.ddd.ddd => u32 网络字节序，d 的范围 [0, 255]，每个字节最多3个d
    assert(ip_string != prh_null);
    prh_numbret(1, inet_pton(AF_INET, ip_string, &out));
    return out.s_addr;
}
void prh_sock_ip_string(prh_u32 ip, char *arr_16_byte) {
    assert(arr_16_byte != prh_null); // src 指向网络字节序的 struct in_addr，
    assert(16 >= INET_ADDRSTRLEN); // size 至少为 INET_ADDRSTRLEN
    prh_boolret(inet_ntop(AF_INET, &ip, arr_16_byte, 16));
}

// 域名系统（DNS）维护域名和IP地址之间的映射关系，在 DNS 出现以前，主机名和IP地址之间的
// 映射关系是在一个手工维护的本地文件 /etc/hosts 中进行定义的：
//      127.0.0.1       localhost
//      127.0.1.1       home.localdomain home
//      ::1             ip6-localhost ip6-loopback
//      fe00::0         ip6-localnet
// 函数 getaddrinfo() 通过搜索这个文件并找出与规范主机名，或其中一个别名（可选的以空格
// 分隔），匹配的记录来获取对应的 IP 地址。然而，/etc/hosts 模式的扩展性较差，并且随着
// 网络主机数量的增长（如因特网中存在者数以亿记的主机），这种方式已经变得不太可行。
//
// DNS 被设计用来解决这个问题，DNS 将主机名组织在一个层级名空间中，DNS 层级中的每个节点
// 都有一个名字，该名字最多可包含63个字符。层级的根是一个无名字的节点，即“匿名节点”。一个
// 节点的域名由该节点到根节点的路径中所有节点的名字连接而成，例如节点 baidu 对应的域名为
// baidu.com。完全限定域名（FQDN fully qualified domain name），如 www.baidu.com.，
// 标识处了层级中的一台主机。区分一个完全限定域名的方法是看名字是否已点结尾，但在很多情况
// 下这个点会省略。
//      [匿名根]
//      [顶级域名] 通用域名 com edu net org 国家域名 cn de eu us
//      [二级域名] baidu kernel gnu
//                www ftp
// 没有一个组织或系统会管理整个层级，相反，存在一个 DNS 服务器层级，每台服务器管理树的一
// 个分支（一个区域）。通常，每个区域都有一个主要域名服务器，此外还包含一个或多个从域名服
// 务器，它们在主要域名服务器崩溃时提供备份。区域本身可以被划分成一个个单独管理的更小的区
// 域。当一台主机被添加到一个区域中，或主机名到IP地址之间的映射关系发生变化时，管理员负责
// 更新本地域名服务器上域名数据中对应的名字，无需手动更改层级中其他域名服务器数据库。当一
// 个程序调用 getaddrinfo() 获取主机名对应的IP地址时，它会与本地DNS服务器通信，如果这个
// 服务器无法提供所需的信息，那么它就会与位于层级中的其他DNS服务器进行通信以便获取信息。
// 有时候，这个解析过程可能会花费很多时间，DNS服务器采用缓存技术来避免在查询常见域名时所
// 发生的不必要的通信。使用这种方法使得DNS能够处理大规模的名字空间，同时无需对域名进行集
// 中管理。
//
// DNS 解析请求可以分为两类，递归和迭代。在一个递归请求中，请求者要求服务器处理整个解析任
// 务，包括在必要时与其他DNS服务器进行通信。当位于本地主机上的一个应用程序调用getaddrinfo()
// 时，该函数会向本地DNS服务器发起一个递归请求。如果本地 DNS 服务器自己并没有相关信息来
// 完成解析，那么它就会迭代地解析这个域名。下面通过一个例子来解释迭代域名。假设本地 DNS
// 服务器需要解析 www.otago.ac.nz，要完成这个任务，它首先与每个 DNS 服务器都知道的一组
// 根域名服务器中的一个进行通信。给定 www.otago.ac.nz 根域名服务器会告诉本地 DNS 服务器
// 到其中一台 nz 服务器上查询，然后本地服务器会在 nz 服务器上查询，并收到 ac.nz 服务器
// 信息，之后继续在 ac.nz 服务器上查询得到 otago.ac.nz 服务器信息，最后本地 DNS 服务器
// 会在 otago.ac.nz 域名服务器上查询到 www.otago.ac.nz 并获取到所需的 IP 地址。如果
// 向 getaddrinfo() 传递了一个不完整域名，那么解析器在解析之前会尝试补全。域名补全规则
// 是在 /etc/resolv.conf 中定义的，参见 resolv.conf(5) 手册。在默认情况下，解析器至少
// 会使用本机的域名来补全。
//
// 众所周知的端口，与对应的服务协议名称，之间的映射。由于服务名称是集中管理并且不会像IP
// 那样频繁变化，因此没必要采用DNS服务器来管理它们。相反，端口号和服务名称会记录在文件
// /etc/services 中。函数 getaddrinfo() 和 getnameinfo() 会使用这个文件中的信息在
// 服务名称和端口号之间进行转换。
//      tcpmux          1/tcp                           # TCP port service multiplexer
//      echo            7/tcp
//      echo            7/udp
//      systat          11/tcp          users
//      daytime         13/tcp
//      daytime         13/udp
//      netstat         15/tcp
//      ssh             22/tcp                          # SSH Remote Login Protocol
//      telnet          23/tcp
//      smtp            25/tcp          mail （别名）
//      time            37/tcp          timserver
//      time            37/udp          timserver
//      whois           43/tcp          nicname
//      domain          53/tcp                          # Domain Name Server
//      domain          53/udp
//      http            80/tcp          www             # WorldWideWeb HTTP
//      pop3            110/tcp         pop-3           # POP version 3
//      ntp             123/udp                         # Network Time Protocol
//      https           443/tcp                         # http protocol over TLS/SSL
//
// TCP 和 UDP 的端口号是相互独立的，相同的端口号可以同时分配给 TCP 和 UDP 用，且可以表
// 示不同发服务。但 IANA 的策略是将两个端口都分配给同一个服务，即使服务只使用了其中一种
// 协议，例如 telnet ssh http smtp 都只使用了 TCP，但对应的 UDP 端口也被分配给了这些
// 服务。响应的 ntp 只使用 udp 但 TCP 端口 123 也分配给了 ntp。在一些情况下，一个服务
// 既会使用 TCP 也会使用 UDP，例如 DNS 和 echo。最后，还有极少出现的情况将数值相同的
// UDP 和 TCP 端口分配给了不同的服务，如 rsh 使用 TCP 端口 514，而 syslog daemon 使用
// DUP 端口 514，这是因为这些端口在采用现行的 IANA 策略之前就分配出去了。
//
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h> // getaddrinfo() 将域名和服务名称，转换成IP地址和端口号的一个链表，getnameinfo() 将IP地址和端口号，转换成域名和服务名称
// int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
// void freeaddrinfo(struct addrinfo *res);
// const char *gai_strerror(int errcode);
// int getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host _Nullable, socklen_t hostlen, char *serv _Nullable, socklen_t servlen, int flags);
// NI_MAXHOST 1025 域名的最大长度 NI_MAXSERV 32 服务名称的最大长度；getnameinfo() 中的 host 和 serv 至少提供一个。
// flags - NI_DGRAM 默认返回流式套接字对应的地址字符串，指定该标志标识返回数据报套接字对应的地址字符串，通常这是无关紧要的因为，因为相同的端口对应的服务名通常相同。
// flags - NI_NAMEREQD 默认情况下，如果无法解析获取到主机名，那么 host 中会返回数据地址字符串，如果制定了该标志，那么会返回错误 EAI_NONAME。
// flags - NI_NOFQDN 默认情况下会返回主机的完全限定域名，指定该标志时如果主机位于局域网中只返回名字的第一部分（即主机名）。
// flags - NI_NUMERICHOST 强制返回数值型地址字符串，禁止耗时的 DNS 查询；不指定该标志，仍然可能返回数值型地址，因为可能域名解析获取不到主机名。
// flags - NI_NUMERICSERV 强制返回数值型服务名称，避免搜索 /etc/services，没有指定该标志也可能返回数值端口号字符串，因为一些端口号根本就不对应任何服务名。
// EAI_AGAIN EAI_BADFLAGS EAI_FAIL EAI_FAMILY 地址族不能识别或地址长度非法，EAI_MEMORY，EAI_NONAME 指定了NI_NAMEREQD但解析失败或参数host serv都没有提供，
// EAI_OVERFLOW 参数 host 或 serv 缓存长度不够，EAI_SYSTEM 系统错误 errno。
#include <sys/types.h>
#include <netdb.h>
prh_u32 prh_sock_resolve_name(const char *domain_name) {
    // struct addrinfo {
    //      int              ai_flags;
    //      int              ai_family; // AF_UNSPEC any family, AF_INET, AF_INET6
    //      int              ai_socktype; // SOCK_STREAM, SOCK_DGRAM, or 0
    //      int              ai_protocol; // 0
    //      socklen_t        ai_addrlen;
    //      struct sockaddr *ai_addr;
    //      char            *ai_canonname;
    //      struct addrinfo *ai_next; };
    struct addrinfo hints = {0}, *addr_info = prh_null, *p;
    struct sockaddr_in *sock_addr; int n;
    // 将 hints 设置为空，等价于：ai_family AF_UNSPEC, ai_socktype ai_protocol 0, ai_flags AI_V4MAPPED|AI_ADDRCONFIG.
    // 根据 POSIX.1 标准，如果将 hints 参数指定为 NULL，则应默认将 ai_flags 视为 0。然而，GNU C 库（glibc）在这种情况下默认使用
    // (AI_V4MAPPED|AI_ADDRCONFIG) 的值，因为这一默认值被认为是对标准的一种改进。AI_V4MAPPED：允许 IPv6 地址映射为 IPv4，提高兼
    // 容性。AI_ADDRCONFIG：根据系统配置返回地址，避免返回不适用的地址（如在没有 IPv6 支持的系统上返回 IPv6 地址）。
    hints.ai_family = AF_INET; // hints 只需设置前四个参数，其他字段必须为零
    // 参数 node 和 service 其中一个可以为 NULL。如果 service 是服务名称，则会被转换成对应的端口号，如果是数值字符串则直接将字符串
    // 转换成整数。如果 service 为 NULL，返回的所有套接字地址中的端口都不进行初始化。如果设置了 AI_NUMERICSERV 标志，并且 service
    // 不为空，那么 service 必须是数值字符串。
    // ai_flags - AI_NUMERICHOST 地址字符串必须是数值型字符串，设置这个标志可以避免昂贵的域名解析流程。数值型字符串可以是，十六进制
    // 和冒号的 IPv6 地址，十进制或者八进制（0）或十六进制（0x）和点号的 IPv4 地址（a.b.c.d a.b.u16 a.u24 u32）。
    // ai_flags - AI_PASSIVE 标志如果设置且 node 为 NULL（即只提供了端口号），则返回的套接字地址适用于 bind(2) 用来 accept(2)
    // 连接，这时返回的套接字地址是通配地址（wildcard address），INADDR_ANY 或 IN6ADDR_ANY_INIT。通配地址常适用于服务器，它可以
    // 接收本地主机任意网络地址上的连接。如果 node 不为空，则 AI_PASSIVE 标志会被忽略。如果没有 AI_PASSIVE 标志，则返回的套接字地
    // 址适用于 connect(2) sendto(2) sendmsg(2)，如果 node 为空返回的是回环地址（loopback address)，INADDR_LOOPBACK 或者
    // IN6ADDR_LOOPBACK_INIT。
    // ai_flags - AI_CANONNAME 标志如果指定，返回的第一个地址中的 ai_canonname 指向规范域名（official name of the host）。
    // ai_flags - AI_ADDRCONFIG 标志如果指定，只有当本地系统配置了至少一个 IPv4 地址时才返回 IPv4 地址，只有当配置了至少一个 IPv6
    // 地址时才返回 IPv6 地址。并且回环地址不被认为是一个有效的配置地址。例如在只配置有 IPv4 的系统上，这个标志可以保证不返回 IPv6
    // 套接字地址。
    // ai_flags - AI_V4MAPPED 标志如果设定，并且 hints.ai_family 设定为 AF_INET6，并且没有找到匹配的 IPv6 地址，那么 IPv4 映射
    // 的 IPv6 地址会被返回。如果同时指定了 AI_V4MAPPED 和 AI_ALL，IPv6 地址和映射的 IPv4 地址都会返回。如果 AI_V4MAPPED 没有指
    // 定，那么 AI_ALL 会被忽略。
    assert(domain_name != prh_null);
    n = getaddrinfo(domain_name, prh_null, &hints, &addr_info);
    // 成功返回0，否则返回以下错误：
    // EAI_ADDRFAMILY - 对应的网络主机（node）没有任何与指定的地址族匹配的网络地址。
    // EAI_AGAIN - 域名服务器返回一个临时失败条件，请稍后重试。
    // EAI_BADFLAGS - hints.ai_flags 包含非法标志，或者包含 AI_CANONNAME 但是 node 为空。
    // EAI_FAIL - 域名服务器返回一个永久失败条件，不可恢复的错误。
    // EAI_FAMILY - 指定的地址族不支持。
    // EAI_MEMORY - 内存耗尽。
    // EAI_NODATA - 存在指定的网络主机，但是该主机没有定义任何网络地址。
    // EAI_NONAME - node 或者 service 非法，或者都是 NULL，或者制定了 AI_NUMERICSERV 但 service 不是数值字符串。
    // EAI_SERVICE - 找不到指定类型（ai_socktype）的服务，例如指定了 SOCK_DGRAM 但只有流式套接字服务，例如设定了 service 但是 ai_socktype 是 SOCK_RAW。
    // EAI_SOCKTYPE - 指定的 ai_socktype 不支持，例如与指定的 ai_protocol 不匹配，例如 SOCK_DGRAM 和 IPPROTO_TCP。
    // EAI_SYSTEM - 其他系统错误，errno 返回错误码。
    if (n != 0) {
#if PRH_SOCK_DEBUG
        printf("getaddrinfo %d %s\n", n, gai_strerror(n));
#endif
        if (addr_info) freeaddrinfo(addr_info);
        return 0;
    }
    // 返回的套接字地址的排序规则定义在 RFC3484 中，在一些平台上可以编辑 /etc/gai.conf 配置其行为。域名和服务
    // 名称对，可以映射多个套接字地址的原因是，例如域名对应的网络主机配置了多个主机地址（the network host is
    // multihomed），例如既可以通过 IPv4 也可以通过 IPv6 地址访问，或者一个服务提供了多种套接字访问类型（例如
    // 返回的一个地址是 SOCK_STREAM 类型，另一个地址是 SOCK_DGRAM 类型）。正常情况下，应用程序应该按套接字地
    // 址的返回顺序优先使用靠前的地址。
    for (p = addr_info; p; p = p->ai_next) {
        sock_addr = (struct sockaddr_in *)p->ai_addr;
        freeaddrinfo(addr_info);
        return sock_addr->sin_addr.s_addr;
    }
    prh_unreachable();
}

// 对于使用套接字的网络服务器程序，有两种常见的设计方式。迭代型：服务器每次只处理一个客户
// 请求，只有当完全处理完一个客户端的请求后才去处理下一个客户端。并发型：可以同时处理多个
// 客户请求。迭代型服务器通常只适用于能够快速处理客户请求的场景，因为每个客户都必须等待前
// 面的客户请求完成。迭代型服务器的典型场景是当客户端和服务器之间交换单个请求和响应时。
//
// 并发服务器的其他解决方案，采用服务器集群（server farm）。构建服务器集群最简单的一种方
// 法是 DNS 轮转负载共享（DNS round-robin load sharing）或称为负载分发（load distribution），
// 一个地区的域名权威服务器将同一个域名映射到多个IP地址上，即多台服务器共享同一个域名。
// 后续对 DNS 服务器的域名解析请求将以循环轮转的方式以不同的顺序返回这些 IP 地址。DNS 循
// 环轮转的优势是成本低，而且容易实施。但是，它也存在一些问题。其中一个问题是远端DNS服务
// 器上所执行的缓存操作，这意味着今后位于某个特定主机（或一组主机）上的客户端发出的请求会
// 绕过循环轮转DNS服务器，并总是由同一个服务器来负责处理。此外，循环轮转DNS并没有任何内建
// 的用来确保达到良好负责均衡（不同客户端在服务上产生的负载不同）或者是确保高可用性的机制
// （如果其中一台服务器宕机或者运行的服务器程序崩溃了怎么办）。在许多采用多台服务器设备的
// 设计中，另一个我们需要考虑的因素是服务器的亲和性（server affinity）。这就是说，确保
// 来自同一个客户端的请求序列能够全部定向到同一台服务器上，这样由服务器维护的任何有关客户
// 端状态的信息都能保持准确，这里涉及保存的客户注册信息在多个服务器之间的同步问题。
//
// 一个更灵活但也更复杂的解决方案是服务器负载均衡（server load balancing）。在这种场景
// 下，由一台负载均衡服务器将客户端请求路由到服务器集群中的其中一个成员上。为了确保高可用
// 性，可能还会有一台备用的服务器，一旦负载均衡主服务器崩溃，备用服务器就立刻接管主服务器
// 的任务。这消除了由远端DNS缓存所引起的问题，因为服务器集群只对外提供一个单独的IP地址，
// 也就是负载均衡服务器的IP地址。负载均衡服务器结合一些算法来衡量或计算服务器负载，可能是
// 服务器集群的成员所提供的量值，并智能化地将负载分发到集群中的各个成员之上。负载均衡服务
// 器也会自动检测集群中失效的成员，如果需要还会自动检测新增加的服务器成员。最后，负载均衡
// 服务器可能还会提供对服务器亲和力的支持。
//
// 如果我们查看一下 /etc/services 的内容，可以看到列出了数百个不同的服务项目，这意味着
// 一个系统理论上可以运行数量庞大的服务器进程。但是大部分服务器进程通常只是等待着偶尔发送
// 过来的连接请求或数据报，除此之外它们什么都不做。所有这些服务器进程依然会占用内核进程表
// 中的资源，而且也会占用一些内存和交换空间，因而对系统产生负载。守护进程 inetd 被设计用
// 来消除运行大量非常用服务器进程的需要。inetd 可提供两个主要好处：与其为每个服务器运行一
// 个单独的守护进程，现在只需用一个进程（inetd）就可以监视一组指定的套接字端口，并按照需
// 要启动其他的服务。因此可以降低系统上运行的进程数量。inetd 简化了启动其他服务的编程工
// 作，因为由 inetd 执行的一些步骤通常在所有的网络服务启动时都会用到。由于inetd监管着一
// 系列的服务，可按照需要启动其他的服务，因此 inetd 有时候也被称为 Internet超 级服务器。
// 在一些 Linux 发行版本中提供有 inetd 的扩展版本 xinetd，关于 xinetd 的 相关信息可参
// 考 www.xinetd.org。
//
// inetd 守护进程通常在系统启动时运行，在称为守护进程后，inetd 执行如下步骤：
//      1. 对于 /etc/inetd.conf 中配置的每一项服务，inetd 都会创建一个恰当类型的套接字
//      （即流式套接字或数据报套接字），然后绑定到指定的端口上。此外，每个TCP套接字都会
//      通过 listen() 调用运行客户端发来连接。
//      2. 通过 select() 调用，inetd 对前一步中创建的所有套接字进行监控，看是否有数据
//      报或请求连接发送过来。
//      3. select() 调用进入阻塞状态，直到一个UDP套接字上有数据报可读或者TCP套接字上收
//      到连接请求。在TCP连接中，inetd 在进入下一个步骤之前纤维连接执行 accept() 调用。
//      4. 要启动这个套接字上指定的服务，inetd 调用 fork() 创建一个新的进程，然后通过
//      exec() 启动服务器程序。在执行exec()前，子进程执行如下的步骤：除了用于 UDP 数据
//      报和接收TCP连接的文件描述符外，将其他所有从父进程继承而来的文件描述符都关闭；在
//      文件描述符0、1、2上复制套接字文件描述符，并关闭套接字文件描述符本身，因为已经不再
//      需要它们了，完成这一步之后，启动的服务器进程就能通过这三个标准文件描述符同套接字
//      通信了；之后这一步是可选的，为启动的服务器进程设定用户和组ID，设定的值需要配置在
//      /etc/inetd.conf 中的相应条目中。
//      5. 第3步骤中，如果在TCP套接字上接收了一个连接，inetd 就关闭这个连接套接字，因为
//      这个套接字只会在稍后启动的服务器进行中使用。
//      6. inetd 服务跳转回第2步进行执行。
// 配置文件 /etc/inetd.conf 中的每一行都描述了一种由 inetd 处理的服务：
//      # echo stream tcp nowait root internal
//      # echo dgram  udp wait   root internal
//      ftp    stream tcp nowait root /usr/sbin/tcpd in.ftpd
//      talnet stream tcp nowait root /usr/sbin/tcpd in.telnetd
//      login  stream tcp nowait root /usr/sbin/tcpd in.rlogind
//
// 第一列是服务名称，结合第三列协议字段，可以通过 /etc/services 确定服务的端口号。第三
// 列指定了套接字所使用的协议，/etc/protocols 中列出了所有的 Internet 协议，但大多数服
// 务使用的都是TCP或UDP。第五列登录名，确定运行的服务器程序的用户ID和组ID，由于inetd以
// root方式运行，它的子进程也同样是特权级的，因而可以在有需要的时候通过 setuid() 以及
// setgid() 来修改进程的凭据。第六列服务器程序，该字段指定了被执行的服务器程序的路径名。
// 第七列服务器程序参数，该字段指定一个或多个参数，参数之间有空格分隔。一些内置服务，例如
// UDP 和 TCP 的 echo 服务，是由 inetd 服务本身实现的，对于这样的服务，服务器程序字段
// 应该是 internal，而服务器程序参数字段将被忽略。要启动 echo 服务，将上面 echo 开头的
// 注释 # 去掉即可。当我们修改了 /etc/inetd.conf 文件后，需要发送一个 SIGHUP 信号给
// inetd，请求它重新读取配置文件：killall -HUP inetd。
//
// 通过 inetd 调用的流式套接字（TCP）服务器通常都被设计为只处理一个单独的客户端连接，处
// 理完后就终止，把监听其他连接的任务留给 inetd。对于这样的服务，第四列的参数应该设置为
// nowait。相反如果是有被执行的服务进程来接受连接的话，那么该字段就应该设置为 wait，此时
// inetd 不会去接受连接，而是将监听套接字的文件描述符当作描述符0传递给被执行的服务进程。
// 对于大部分的UDP服务器，该字段应该设为 wait。由 inetd 调用的 UDP 服务器通常被设计为读
// 取并处理所有套接字上未完成的数据报，然后终止。从套接字中读取数据报时，通常需要一些超时
// 机制，这样在指定的时间间隔内如果没有新的数据报到来，服务进程就会终止。通过指定为 wait，
// 我们可以阻止 inetd 在套接字上同时尝试做 select() 操作，因为 inetd 可能会在检查数据
// 报的时候同 UDP 服务之间产生竞争条件，从事如果 inetd 赢了它还会启动另一个 UDP 服务进
// 程实例。由于 inetd 操作以及它的配置文件的格式并没有在 SUSv3 中指定，因此在配置文件
// /etc/inetd.conf 中指定的值可能会有一些变动。
//
// 守护进程（daemon）是一种具备下列特征的进程：它的声明周期很长，通常一个守护进程会在系统
// 启动的时候被创建并一直运行直至系统被关闭；它在后台运行并且不拥有控制终端，控制终端的缺
// 失确保内核永远不会为守护进程自动生成任何任务控制信号以及终端相关的信号，例如 SIGINT、
// SIGTSTP、SIGHUP。很多标准 daemon 会作为特权进行运行（即有效用户ID为0），因此在编写
// 守护进程时应该遵循特别的安全规范。特权程序能够访问普通用户无法访问的特性和资源，一个程
// 序可以下面两种方式已特权方式运行：程序在一个特权用户ID下启动，很多守护进程和网络服务器
// 通常以 root 身份运行，它们就属于这种类别；另外程序设置了 set-user-Id 或 set-group-ID
// 权限位，当一个 set-user-ID（set-group-ID）程序被执行之后，它会将进程的有效用户（组）
// ID修改为与程序文件的所有者（组）一样的ID。如果一个特权程序包含 bug 或可以被恶意用户破
// 坏，那么系统或应用程序的安全性就会受到影响。从安全的角度来讲，在编写程序的时候应该将系
// 统受到安全威胁的可能性以及受到安全威胁时产生的损失降到最小。
//
// 通常会将守护进程程序的名称以字母d结尾，但并不是所有人都遵循这个惯例。在 Linux 上，特定
// 的守护进程会作为内核线程运行。实现此类 daemon 的代码时内核的一部分，它们通常在系统启动
// 的时候被创建。当使用 ps(1) 列出线程时，这些守护进程的名称会用方括号[]括起来。要创建一
// 个守护进程，需要：
// （一）执行一个 fork() 之后父进程退出，子进程继续执行，结果时守护进程称为 init 进程的
// 子进程。之所以要做这一步是因为下面两个原因：假设守护进程是从命令行启动的，父进程的终止
// 会被 shell 发现，shell 在发现之后会显示出另一个 shell 提示符并让子进程继续在后台运行；
// 子进程被确保不会成为一个进程组首进程，因为它从其父进程那里继承了进程组ID并且拥有了自己
// 的唯一的进程ID，而这个进程ID与继承而来的进程组ID是不同的，这样才能够成功地执行下面的
// 一个步骤。
// （二）子进程调用 setsid() 开启一个新会话，并释放它与控制终端之间的所有关联关系。
// （三）如果守护进程从来没有打开过终端设备，那么就无需担心守护进程会重新请求一个控制终端
// 了。如果守护进程后面可能会打开一个终端设备，那么必须要采取措施来确保这个设备不会成为
// 控制终端。这可以通过下面两种方式实现：在所有可能应用到一个终端设备上的 open() 调用中
// 指定 O_NOCTTY 标记；或者更简单的说，在 setsid() 调用之后执行第二个 fork()，然后再次
// 让父进程退出并让孙子进程继续执行。这样就确保了子进程不会成为会话组长，因此根据 System
// V 中获取终端的规则（Linux 也遵循这个规则），进程永远不会重新请求一个控制终端。在遵循
// BSD 规则的实现中，一个进程只能通过一个显式的 ioctl() TIOCSCTTY 操作来获取一个控制
// 终端，因此第二个 fork() 调用对控制终端的获取并没有任何影响，但多一个 fork() 调用不会
// 带来任何坏处。
// （四）清除进程的 umask 以确保当守护进程创建文件和目录时拥有所需的权限。
// （五）修改进程的当前工作目录，同辉改为根目录，这样做是有必要的，因为守护进程通常会一直
// 运行直至系统关闭为止。如果守护进程的当前工作目路是不包含/的文件系统，那么就无法卸载该
// 文件系统。或者守护进程可以将工作目录改为完成任务时所在的目录或在配置文件中定义的一个目
// 录，只要包含这个目录的文件系统永远不会被卸载即可。如 cron 会将自身放在 /var/spool/cron
// 目录下。
// （六）关闭守护进程从其父进程继承而来的所有打开这的文件描述符，守护进程可能需要保持继承
// 而来的文件描述的打开状态，因此这一步时可选的。之所以需要这样做的原因有很多，由于守护
// 进程失去了控制终端并且是在后台运行的，因此让守护进程保持文件描述符 0 1 2 的打开状态毫
// 无意义，因为它们指向的就是控制终端。此外，无法卸载长时间运行的守护进程打开的文件所在的
// 文件系统，因此通常的做法时关闭所有无用的打开着的文件描述符，因为文件描述符时一种有限的
// 资源。一些 UNIX 实现提供了一个名为 closefrom(n) 或类似名称的函数，它关闭嗦鱼大于或
// 等于 n 的文件描述符。Linux 上并不存在这个函数。
// （七）在关闭了文件描述符 0 1 2 之后，守护进程通常会打开 /dev/null 并使用 dup2() 或
// 类似的函数，使所有这些描述符指向这个设备。之所以要这样做时因为：它确保了当守护进程调用
// 了在这些描述符上执行 I/O 的库函数时不会出乎意外地失败；它防止了守护进程后面使用描述符
// 1 或 2 打开一个文件的情况，因为库函数会将这些描述符当作标准输出和标准错误来写入数据。
// /dev/null 是一个虚拟设备，它总会将写入的数据丢弃。当需要删除一个 shell 命令的标准输
// 出和错误时可以将它们重定向到这个文件。从这个设备中读取数据总是会返回文件结束的错误。
//
// 一个守护进程通常只有在系统关闭的时候才会终止。很多标准的守护进程通过在系统关闭时执行
// 特定于应用程序的脚本来停止。而那些不以这种方式终止的守护进程会收到一个 SIGTERM 信号，
// 因为在系统关闭的时候 init 进程会向所有其子进程发送这个信号。在默认情况下，SIGTERM
// 信号会终止一个进程。如果守护进程在终止之前需要做些清理工作，那么就需要为这个信号建立
// 一个处理函数。这个处理函数必须能快速的完成清理工作，因为 init 在发完 SIGTERM 信号 5
// 秒之后会发送一个 SIGKILL 信号。这并不意味着这个守护进程能够执行 5 秒的 CPU 时间，因
// 为 init 会同时向系统中的所有进程发送信号，而它们可能都试图在 5 秒内完成清理工作。由于
// 守护进程是长时间运行的，因此要特别小心潜在的内存泄露问题和文件描述符泄露。很多守护进程
// 需要确保同一时刻只有一个进程实例处于活跃状态。
//
// 由于很多守护进程需要持续执行，因此在设计守护进程时需要克服一些障碍：通常守护进程会在启
// 动时从相关的配置文件中读取操作参数，但有时候需要在不重启守护进程的情况下快速修改这些参
// 数；一些守护进程会产生日志文件，如果守护进程永远不关闭日志文件的话，那么日志文件就会无
// 限制地增长，最终会阻塞文件系统。即使删除了一个文件的文件名，只要有进程还打开着这个文件，
// 那么这个文件就会一直存在下去。这里需要有一种机制来告诉守护进程关闭其日志文件并打开一个
// 新文件，这样就能够在需要的时候旋转日志文件了。解决这两个问题的方案时让守护进程为 SIGHUP
// 建立一个处理函数，并在收到这个信号时采取所需的措施。当控制进程与控制终端断开连接之后就
// 会生成 SIGHUP 信号。由于守护进程没有控制终端，因此内核永远不会向守护进程发送这个信号，
// 这样守护进程就可以使用 SIGHUP 信号来达到这个目的。logrotate 程序可以自动旋转守护进程
// 的日志文件，具体参考 logrotate(8) 手册。一些守护进程在收到 SIGHUP 信号时会使用其他方
// 法来重新初始化自身：它们会关闭所有文件，然后使用 exec() 重新启动自身。
//
// 在编写守护进程时碰到的一个问题时如何显式错误消息，由于守护进程是后台运行的，因此通常无
// 法像其他消息输出到关联终端上。这个问题的一种解决方式是将消息写入到一个特定于应用程序的
// 日志文件中。这种方式存在的一个主要问题是让系统管理员管理多个应用程序日志文件和监控其中
// 是否存在错误比较困难，syslog 工具就用于解决这个问题。syslog 工具提供了一个集中式日志
// 工具，系统中的所有应用程序都可以使用这个工具来记录日志消息。
//
// 服务器写入一个对方已经关闭的套接字 SIGPIPE
// TCP SO_REUSEADDR
// 处理同一个客户同时建立多个TCP连接的情况，TCP运行两个端口之间能建立多条连接吗？但是一个
// 客户账号可以用两台不同的机器来连接，这时就需要对客户账号进行唯一性验证。或者它在同一个
// 主机上使用不同的端口号进行连接呢，也是需要进行客户账号进行唯一性验证。
//
// 如果套接字上可用的数据比在 read() 中请求的数据要少，那就可能会出现部分读的现象，在这种
// 情况下，read() 简单地返回可用的字节数。如果没有足够的缓冲空间来传输所有请求字节，并满
// 足以下条件之一，可能会出现部分写的现象：在 write() 调用过程中传输了部分请求后被信号处
// 理函数中断；套接字工作在非阻塞模式下，可能当前只能传输一部分请求的字节；在部分请求的字
// 节已经完成传输后出现了一个异步错误，异步错误例如由于 TCP 连接出现问题，可能会使对端的
// 应用程序崩溃。如果出现了部分 I/O 现象，那么有时候需要重新调用系统调用来完成全部数据的
// 传输。
//
// #include "rdwrn.h"
// ssize_t readn(int fd, void *buffer, size_t count); 返回读取的字节数，文件尾返回0，错误返回-1
// ssize_t writen(int fd, const void *buffer, size_t count); 返回写入的字节数，错误返回-1
//
// 函数 readn() 和 writen() 的参数与 read() 和 write() 相同，但是这两个函数使用循环
// 重新启用这些系统调用，因此确保请求的字节数总是能够全部得到传输，除非出现错误或者read()
// 检测到文件结束符。
//
// #include <sys/types.h>
// #include <unistd.h>
// ssize_t read(int fd, void buf[.count], size_t count); 返回读取的字节数，文件尾返回0，错误返回-1
// ssize_t pread(int fd, void *buf, size_t count, off_t offset); 不会修改文件偏移，文件必须支持 seeking
//
// 如果 count 为零，read() 可用来检测下面的错误，如果没有错误或者 read() 不检测错误，
// read() 会简单的返回0，不会有其他效果。根据 POSIX.1 如果 count 大于 SSIZE_MAX，其
// 行为是具体实现定义的。在 Linux 上，read() 和类似的系统调用最多传输 0x7fff_f000 个
// 字节（21_4747_9552），返回实际传输的字节数，在 32-bit 和 64-bit 系统上都成立。
//
// 在 NFS 文件系统上，读取少量数据时，时间戳仅在第一次读取时更新，后续调用可能不会更新。
// 这是由于客户端属性缓存导致的，因为大多数（如果不是全部）NFS 客户端将 st_atime（最后
// 文件访问时间）的更新留给服务器处理，而客户端缓存满足的读取不会导致服务器上的 st_atime
// 更新，因为服务器端没有发生读取操作。可以通过禁用客户端属性缓存来获得 UNIX 语义，但在
// 大多数情况下，这会显著增加服务器负载并降低性能。第一次读取：客户端从服务器读取数据时，
// 会更新 st_atime，并将新值缓存。后续读取：如果数据在客户端缓存中，客户端直接从缓存读取，
// 不会再次访问服务器，因此不会更新 st_atime。
//
// 根据 POSIX.1-2008/SUSv4 第 XSI 2.9.7 节（“线程与常规文件操作的交互”）：以下所有函
// 数在 POSIX.1-2008 指定的效果方面，当它们对常规文件或符号链接进行操作时，彼此之间的操
// 作都应该是原子的：…… 随后列出的 API 包括 read() 和 readv(2)。而在线程（和进程）之间
// 应该是原子的效果中，包括文件偏移量的更新。然而，在 Linux 3.14 之前，情况并非如此：如
// 果两个共享打开文件描述，参见 open(2)，的进程同时执行 read() 或 readv(2)，那么 I/O
// 操作在更新文件偏移量方面不是原子的，结果可能导致两个进程的读取操作（错误地）在它们获取
// 的数据块上发生重叠。这个问题在 Linux 3.14 中得到了修复。
//
// 可能返回的错误：
//      EAGAIN - 非套接字文件描述符，设置成了非阻塞，该错误表示已经没有可读的内容，稍后尝试。
//      EAGAIN 或 EWOULDBLOCK - 非阻塞套接字文件描述符，该错误表示稍后尝试，POSIX.1-2002 允许返回这两个值中
//          的任意一个，且没有要求这两个值必须是相同的，因此可移植程序应该同时检查这两个值。
//      EBADF - fd 非法文件描述符，或者这个文件打开的目的不是用于读取的。
//      EFAULT - buf 地址非法。
//      EINTR - 在没有读取任何数据之前，被信号中断。
//      EINVAL - fd 对应的对象不适合读取，或者文件使用 O_DIRECT 打开并且buf/count/文件偏移没有适当对齐。
//          文件描述符是通过 timerfd_create(2) 创建的，但是 read() 提供了错误大小的缓存空间。
//      EISDIR - fd 对应的是一个文件目录。
//      EIO - 表示发生了输入/输出错误。例如后台进程组读取控制终端，进程处于后台进程组，尝试从控制终端读取数据，
//          进程忽略或阻塞 SIGTTIN 信号，或进程组是孤儿进程组，此时 read() 返回 EIO，表示不允许后台进程读取
//          控制终端。或者从磁盘或磁带读取数据时发生低级 I/O 错误。还如在网络文件系统（如 NFS）上，如果对文件
//          描述符持有的建议锁（advisory lock）丢失，read() 可能返回 EIO。建议锁用于协调多个进程对文件的访
//          问，锁丢失可能导致数据不一致。参考：详见 fcntl(2) 的“丢失的锁”部分。
//      根据fd关联的对象，可能发生其他错误
//          例如：如果 fd 指向一个套接字，read() 可能返回 ECONNRESET（连接被重置）或 ENOTCONN（套接字未连
//          接）。如果 fd 指向一个管道，read() 可能返回 EPIPE（管道破裂）。
//
// 系统调用 pread() 和 pwrite() 在多线程程序中特别有用，它允许多线程在同一个文件上执行
// I/O 操作，而不受其他线程修改文件偏移的影响。POSIX 规定：用 O_APPEND 标志打开文件时，
// pwrite() 的写入位置不应受该标志影响；也就是说，即使文件以追加方式打开，pwrite() 仍应
// 严格按照调用者给出的 offset 参数在指定偏移处写入数据。然而，在 Linux 上，如果文件以
// O_APPEND 方式打开，pwrite() 会忽略 offset 参数，始终将数据追加到文件末尾。
//
// #include <unistd.h>
// ssize_t write(int fd, const void buf[.count], size_t count); 返回写入的字节数，错误返回-1
// ssize_t pwrite(int fd, const void buf[.count], size_t count, off_t offset); 不会更新文件偏移
//
// write() 将缓冲区 buf 起始处最多 count 字节的数据写入文件描述符 fd 所指向的文件。实际
// 写入的字节数可能小于 count，例如：底层物理介质空间不足；遇到 RLIMIT_FSIZE 资源限制
// （参见 setrlimit(2)）；在写入所有数据之前被信号处理程序中断；非阻塞描述符暂时无法写入
// 更多数据。
//
// 对于位置可以偏移的文件（即可对其使用 lseek(2) 的文件，例如普通文件）：写入发生在当前
// 文件偏移量处，写入完成后偏移量增加实际写入字节数。若文件以 O_APPEND 打开，则先原子性
// 将偏移量设为文件末尾，再执行写入。偏移量调整与写入操作视为原子步骤。
//
// POSIX 要求：任何可证明在 write() 返回之后发生的 read(2) 必须返回新数据。注意，并非
// 所有文件系统都符合 POSIX。根据 POSIX.1，若 count 大于 SSIZE_MAX，结果由实现定义。
// 在 Linux 上，write()（以及类似的系统调用）一次最多传输 0x7ffff000（2,147,479,552）
// 字节，并返回实际传输的字节数。这在 32 位和 64 位系统上都成立。
//
// 成功时返回实际写入的字节数。出错时返回 -1，并设置 errno 指示错误。注意：成功写入的字
// 节数可能小于 count（部分写入）。部分写入原因包括：磁盘空间不足、阻塞写入被信号中断等。
// 调用者可再次调用 write() 传输剩余字节，后续调用可能继续传输或返回错误（如磁盘已满）。
// 若 count 为 0 且 fd 指向普通文件：若检测到错误则返回失败状态；若未检测到错误，返回 0
// 且不产生其他效果。若 count 为 0 且 fd 指向非普通文件，结果未定义。
//
// write() 的成功返回并不保证数据已提交到磁盘。在某些文件系统（包括 NFS）上，它甚至不保
// 证已为数据成功预留空间。在这种情况下，某些错误可能会延迟到后续的 write()、fsync(2)
// 或甚至 close(2) 才出现。唯一确保的方法是，在完成所有数据写入后调用 fsync(2)。
//
// 标准 POSIX.1-2008，历史 Svr4, 4.3BSD, POSIX.1-2001。在 SVr4 之前，write 可能在任
// 意时刻被中断并返回 EINTR，而不仅仅是在尚未写入任何数据之前。当前标准，如果 write()
// 在尚未写入任何字节时被信号处理程序中断，则调用失败并返回错误 EINTR；如果在已写入至少
// 一个字节后被中断，则调用成功，并返回已写入的字节数。
//
// 在使用直接 I/O 执行 write() 时，返回错误并不意味着整个写入失败。可能已写入部分数据，
// 此时应认为 write() 所尝试的文件偏移处的数据是不一致的。
//
// 可能返回的错误：
//      EAGAIN - 非套接字文件描述符，设置成了非阻塞，该错误表示已经没有可读的内容，稍后尝试。
//      EAGAIN 或 EWOULDBLOCK - 非阻塞套接字文件描述符，该错误表示稍后尝试，POSIX.1-2002 允许返回这两个值中
//          的任意一个，且没有要求这两个值必须是相同的，因此可移植程序应该同时检查这两个值。
//      EBADF - fd 非法文件描述符，或者这个文件打开的目的不是用于写入的。
//      EDESTADDRREQ - fd 对应数据报套接字，目的地址没有通过 connect(2) 设置。
//      EDQUOT - 用户在该文件所在文件系统上的磁盘块配额已用尽。
//      EFBIG - 试图写入的文件大小超过了实现定义的最大文件尺寸，或超出了进程的文件大小限制，或写入位置超过了允许的最大偏移量。
//      EFAULT - buf 地址非法。
//      EINTR - 在没有读取任何数据之前，被信号中断。
//      EINVAL - fd 对应的对象不适合写入，或者文件使用 O_DIRECT 打开并且buf/count/文件偏移没有适当对齐。
//          文件描述符是通过 timerfd_create(2) 创建的，但是 read() 提供了错误大小的缓存空间。
//      EIO - 在修改 inode 时发生了底层 I/O 错误。该错误可能与之前某次 write() 写入的数据回写有关，而那次
//          write() 可能使用了同一文件的不同文件描述符。自 Linux 4.13 起，回写产生的错误保证：后续 write()
//          请求可能会再次报告该错误；后续 fsync(2) 一定会报告该错误，无论 write() 是否已报告。在网络文件系
//          统上，另一个导致 EIO 的原因是：文件描述符上曾持有的建议锁已丢失。详见 fcntl(2) 的 “Lost locks”
//          部分。
//      ENOSPC - 包含该文件的设备已无剩余空间存放数据。
//      EPERM - 操作被文件 seal 阻止；参见 fcntl(2)。
//      EPIPE - fd 连接到一个管道或套接字，其读端已关闭，此时写进程还会收到 SIGPIPE 信号。因此只有在程序捕获、
//          阻塞或忽略该信号时，才能看到 write 的返回值。
//      根据fd关联的对象，可能发生其他错误
//
// #include <sys/uio.h>
// ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
// ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
//
// #include <sys/socket.h>
// ssize_t recv(int sockfd, void *buf, size_t size, int flags);
// ssize_t recvfrom(int sockfd, void *buf, size_t size, int flags, struct sockaddr *_Nullable src_addr, socklen_t *_Nullable addrlen);
//
// recv() 和 send() 系统调用可在已连接的套接字上执行I/O操作，它们提供了专属于套接字的功
// 能，而这些功能在传统的 read() 和 write() 系统调用上是没有的。recv() 和 send() 的返
// 回值和前三个参数同 read() 和 write() 一样。最后一个参数 flags 是一个位掩码，用来修改
// I/O 操作的行为。
//
// 这三个调用成功时都会返回消息的大小。如果消息太长，无法完全放入提供的缓冲区中，根据消息
// 来源的套接字类型，多余的字节可能会被丢弃。如果在套接字上没有可用的消息，接收调用会等待
// 消息到达，除非套接字是非阻塞的，这种情况下会返回值 -1 并将 errno 设置为 EAGAIN 或
// EWOULDBLOCK。接收调用通常会返回任何可用的数据，最多是请求的数量，而不是等待接收完整数
// 量的消息。
//
// 这些调用返回接收到的字节数，如果发生错误则返回 -1 设置 errno。当流式套接字的对端执行
// 有序关闭（orderly shutdown）时，返回值将是 0（传统的“文件结束”返回值）。数据报套接字
// （包括UNIX 域和 Internet 域）允许零大小的数据报，当接收到这样的数据报时，返回值是 0。
// 如果调用 recv() 时指定的接收字节数为 0，也会返回 0。
//
// 如果 recv() 提供的 flags 为零，基本上功能与 read() 一样。唯一不同的是，如果零字节长
// 度用户数据报正等待处理时，read() 不产生任何效果，数据报仍然在等待，但是 recv() 会消耗
// 掉这个数据报。
//
// 而 recv() 与 recvfrom()，系统调用 recv（sockfd, buf, size, flags) 通常用于已经连
// 接的套接字，它等价于 recvfrom(sockfd, buf, size, falgs, NULL, NULL)。recvfrom()
// 将接收到的消息放入缓冲区 buf 中，如果 src_addr 不为 NULL，并且底层协议提供了消息的源
// 地址，则该源地址将被放置在 src_addr 指向的缓冲区中。在这种情况下，addrlen 是一个
// in-out 参数。在调用之前，应将其初始化为与 src_addr 相关联的缓冲区的大小。返回时，
// addrlen 会被更新为包含源地址的实际大小。如果提供的缓冲区太小，返回的地址将被截断；在
// 这种情况下，addrlen 将返回一个大于调用时提供的值。如果调用者对源地址不感兴趣，则应将
// src_addr 和 addrlen 指定为 NULL。
//
// 标准 POSIX. 1-2008，历史 POSIX.1-2001, 4.4BSD (第一次出现在 4.2BSD)，POSIX.1 仅
// 描述了 MSG_OOB、MSG_PEEK、MSG_WAITALL 这三个标志。
//
// MSG_OOB - 此标志请求接收不会在正常数据流中接收的带外数据。某些协议将加急数据放在正常
//      数据队列的前面，因此不能与这些协议一起使用此标志。
// MSG_PEEK - 此标志导致接收操作从接收队列的开头返回数据，但不从队列中移除这些数据。因此，
//      后续的接收调用将返回相同的数据。
// MSG_WAITALL Linux 2.2 - 此标志请求操作阻塞，直到满足完整请求为止。然而，如果捕获到
//      信号、发生错误或连接断开，或者要接收的下一条数据与返回的数据类型不同，调用仍可能
//      返回少于请求的数据。此标志对数据报套接字无效。
// MSG_TRUNC Linux 2.2 - 对于原始（AF_PACKET）、网络数据报（Linux 2.4.27/2.6.8）、
//      netlink（Linux 2.6.22）、UNIX 数据报包括有序数据包（sequenced-packet）（Linux 3.4）
//      套接字：会返回数据包（packet）或数据报（datagram）的真实大小，即使它比传递的缓
//      存大小要长。对于该标志对流式套接字的作用，参考 tcp(7)。
// MSG_DONTWAIT Linux 2.2 - 启用非阻塞操作；如果操作会阻塞，则调用失败并返回错误 EAGAIN
//      或 EWOULDBLOCK。这与通过 fcntl(2) 的 F_SETFL 操作设置 O_NONBLOCK 标志类似，
//      但有所不同：MSG_DONTWAIT 是每次调用的选项，而 O_NONBLOCK 是打开的文件描述符上
//      的设置（参见 open(2)），它会影响调用进程中的所有线程以及其他持有相同打开文件描述
//      符的进程。
// MSG_CMSG_CLOEXEC （仅限 recvmsg()；Linux 2.6.23） - 为通过 UNIX 域文件描述符使用
//      SCM_RIGHTS 操作，参考 unix(7)，接收的文件描述符设置 close-on-exec 标志。此标
//      志的作用与 open(2) 中的 O_CLOEXEC 标志相同。
// MSG_ERRQUEUE Linux 2.2 - 此标志指定应从套接字错误队列中接收排队的错误。错误通过附属
//      数据传递（ancillary data），数据类型取决于协议（对于 IPv4 是 IP_RECVERR）。
//      用户应提供足够大的缓冲区，请参见 cmsg(3) 和 ip(7)。导致错误的原始数据包的负载
//      （payload）作为普通数据通过 msg_iov 传递。导致错误的数据报的原始目标地址通过
//      msg_name 提供。错误通过 sock_extended_err 结构传递：
//      #define SO_EE_ORIGIN_NONE    0
//      #define SO_EE_ORIGIN_LOCAL   1
//      #define SO_EE_ORIGIN_ICMP    2
//      #define SO_EE_ORIGIN_ICMP6   3
//      struct sock_extended_err {
//          uint32_t ee_errno;   /* Error number */ 错误码
//          uint8_t  ee_origin;  /* Where the error originated */ 错误来源的来源
//          uint8_t  ee_type;    /* Type */ 其他字段是协议特定的
//          uint8_t  ee_code;    /* Code */
//          uint8_t  ee_pad;     /* Padding */
//          uint32_t ee_info;    /* Additional information */
//          uint32_t ee_data;    /* Other data */
//          /* More data may follow */ };
//      struct sockaddr *SO_EE_OFFENDER(struct sock_extended_err *); 犯错误的人
//      宏 SO_EE_OFFENDER 根据附属数据的指针返回导致错误的网络对象的地址指针。如果该地
//      址未知，则 sockaddr 的 sa_family 成员包含 AF_UNSPEC，sockaddr 的其他字段未
//      定义。导致错误的数据包的负载（payload）会作为普通数据传递。
//      对于本地错误，不传递地址（可以通过 cmsghdr 的 cmsg_len 成员检查）。对于错误接
//      收，msghdr 中设置了 MSG_ERRQUEUE 标志。传递错误后，会根据下一个排队的错误重新
//      生成挂起的套接字错误，并在下一次套接字操作中传递。
//
// ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
// struct msghdr {
//      void         *msg_name;       /* Optional address */ 由调用者分配
//      socklen_t     msg_namelen;    /* Size of address */
//      struct iovec *msg_iov;        /* Scatter/gather array */
//      size_t        msg_iovlen;     /* # elements in msg_iov，根据 POSIX.1 类型应该是 int */
//      void         *msg_control;    /* ancillary data, see below */ 附属数据，实际的缓存由调用者提供
//      size_t        msg_controllen; /* ancillary data buffer size, 根据 POSIX.1 类型应该是 socklen_t */
//      int           msg_flags;      /* Flags on received message */ }; 接收到的消息拥有的标志
// struct cmsghdr {
//      size_t cmsg_len;    /* Data byte count, including header (type is socklen_t in POSIX) */
//      int    cmsg_level;  /* Originating protocol */ 来源协议
//      int    cmsg_type;   /* Protocol-specific type */ 协议特定类型
//      /* followed by unsigned char cmsg_data[]; */ };
//
// 另外，Linux 提供 recvmmsg(2) 可用来通过一个系统调用接收多个数据报。recvmsg() 使用
// msghdr 结构来减少直接传递的参数数量。msg_name 指向一个由调用者分配的缓冲区，用于返回
// 源地址（如果套接字未连接），调用者在调用前应将 msg_namelen 设置为该缓冲区的大小，成功
// 调用后，msg_namelen 将包含返回地址的大小。如果应用程序不需要知道源地址，可以将 msg_name
// 指定为 NULL。msg_iov 和 msg_iovlen 字段描述了分散/聚集位置，类似于 readv(2) 中讨论
// 的内容，它们用于指定多个缓冲区，数据将被分散到这些缓冲区中。msg_control 指向一个缓冲
// 区，用于存储协议控制相关的消息或其他附属数据，调用 recvmsg() 时，msg_controllen 应
// 包含 msg_control 缓冲区的大小，成功调用后它将包含控制消息序列的大小。在 recvmsg()
// 返回时，msghdr 中的 msg_flags 字段会被设置，可能包含以下标志：MSG_EOR - 表示记录结束，
// 返回的数据结束一个记录，通常用于 SOCK_SEQPACKET 类型的套接字。MSG_TRUNC - 表示数据
// 报的尾部被丢弃，因为数据报大于提供的缓冲区。MSG_CTRUNC - 表示由于附属数据缓冲区空间
// 不足，某些控制数据被丢弃。MSG_OOB - 表示接收到加急或带外数据。MSG_ERRQUEUE - 表示没
// 有接收到数据，但有一个来自套接字错误队列的扩展错误。MSG_CMSG_CLOEXEC Linux 2.6.23
// 表示在 recvmsg() 的 flags 参数中指定了 MSG_CMSG_CLOEXEC。
//
// 附属数据以 cmsghdr 结构的形式出现，附属数据应仅通过 cmsg(3) 中定义的宏来访问。例如
// Linux 使用这种附属数据机制来传递扩展错误、IP 选项或通过 UNIX 域套接字传递文件描述符。
// 关于在各种套接字域中使用附属数据的更多信息，请参见 unix(7) 和 ip(7)。
//
// recv() recvfrom() recvmsg() 可能返回的错误：
//      以下是由套接字层产生的一些标准错误，底层协议模块可能会产生并返回其他错误，请参考
//      协议手册。
//      EAGAIN 或 EWOULDBLOCK - 套接字被标记为非阻塞，且接收操作会阻塞，或者设置了接收
//          超时，而在接收到数据之前已经超时。
//      EBADF - 参数 sockfd 是一个无效的文件描述符。
//      ECONNREFUSED - 远程主机拒绝接受网络连接（通常是因为它没有运行请求的服务）。
//      EFAULT - 接收缓冲区指针指向进程地址空间之外。
//      EINTR - 在任何数据可用之前，接收操作被信号中断；请参阅 signal(7)。
//      EINVAL - 传递了无效的参数。
//      ENOMEM - 无法为 recvmsg() 分配内存。
//      ENOTCONN - 套接字与面向连接的协议相关但尚未连接，参考 connect(2) accept(2)。
//      ENOTSOCK - 文件描述符 sockfd 不指向套接字。
//
// #include <sys/socket.h>
// ssize_t send(int sockfd, const void *buf, size_t size, int flags);
// ssize_t sendto(int sockfd, const void *buf, size_t size, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
// ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
//
// 系统调用 send()、sendto() 和 sendmsg() 用于将消息发送到另一个套接字。send() 调用仅
// 在套接字处于连接状态时（即已知目标接收者）才可用。send() 和 write(2) 之间的唯一区别
// 在于 flags 参数的存在。如果 flags 参数为零，则 send() 等同于 write(2)。另外
// send(sockfd, buf, size, flags) 等价于 sendto(sockfd,buf,size,flags,NULL,0)。
//
// 如果在面向连接的套接字（SOCK_STREAM、SOCK_SEQPACKET）上使用 sendto()，则会忽略参数
// dest_addr 和 addrlen（如果它们不是 NULL 和 0，则可能返回错误 EISCONN），并且如果套
// 接字实际上未连接，则返回错误 ENOTCONN。否则，目标地址由 dest_addr 给出，addrlen 指
// 定其大小。对于 sendmsg()，目标地址由 msg.msg_name 给出，msg.msg_namelen 指定其大小。
//
// 对于 send() 和 sendto()，消息存储在 buf 中，大小为 size。对于 sendmsg()，消息由数
// 组 msg.msg_iov 的元素指向。sendmsg() 调用还允许发送附属数据（也称为控制信息）。如果
// 消息太长，无法通过底层协议原子性地传递，则返回错误 EMSGSIZE，并且消息不会被传输。
// send() 操作本身并不隐含发送失败的指示。本地检测到的错误通过返回值 -1 表示。当消息无法
// 放入套接字的发送缓冲区时，send() 通常会阻塞，除非套接字已置于非阻塞 I/O 模式。在这种
// 情况下，如果处于非阻塞模式，则会以错误 EAGAIN 或 EWOULDBLOCK 失败。可以使用
// select(2) 调用来确定何时可以发送更多数据。
//
// sendmsg() 使用 msghdr 结构来减少直接传递的参数数量。另外，Linux 特有的 sendmmsg(2)
// 可用一个系统调用发送多个数据报。
//
// msg_name msg_namelen - 在未连接的套接字上用于指定数据报的目标地址，它指向一个包含地
//      址的缓冲区；msg_namelen 字段应设置为地址的大小；对于已连接的套接字，这些字段应
//      分别指定为 NULL 和 0。
// msg_iov msg_iovlen - 这些字段指定分散/聚集位置，类似于 writev(2)。
// msg_control msg_controllen - 指定发送控制信息（或称为附属数据），内核可以处理的附属
//      数据缓冲区的最大大小由 /proc/sys/net/core/optmem_max 中的值限制；更多信息请参
//      见 socket(7)。关于在各种套接字域中使用附属数据的进一步信息，请参见 unix(7) 和
//      ip(7)。
// msg_flags - 该字段被忽略。
//
// 标准 POSIX.1-2008 MSG_CONFIRM 是一个 Linux 扩展标志。历史 4.4BSD, SVr4,
// POSIX.1-2001，第一次出现在 4.2BSD。POSIX.1-2001 仅定义了 MSG_OOB 和 MSG_EOR
// 标志。POSIX.1-2008 增加了对 MSG_NOSIGNAL 标志的定义。
//
// MSG_OOB - 在支持这一概念的套接字（如 SOCK_STREAM 类型）上发送带外数据；底层协议也必
//      须支持带外数据。
// MSG_EOR Linux 2.2 - 终止一条记录（在支持这一概念的情况下，例如对于 SOCK_SEQPACKET
//      类型的套接字）。
// MSG_NOSIGNAL Linux 2.2 - 如果面向流的套接字的对端已关闭连接，则不会生成 SIGPIPE
//      信号，但仍会返回 EPIPE 错误。这与使用 sigaction(2) 忽略 SIGPIPE 行为类似，但
//      不同的是：MSG_NOSIGNAL 是每次调用的特性，而忽略 SIGPIPE 则设置了一个影响进程
//      内所有线程的进程属性。
// MSG_DONTROUTE - 发送数据包时不使用网关，仅向直接连接的网络上的主机发送。通常仅由诊断
//      或路由程序使用。仅针对会进行路由的协议族定义；数据包套接字（packet socket）不适
//      用。
// MSG_DONTWAIT Linux 2.2 - 启用非阻塞操作；如果操作会阻塞，则返回 EAGAIN 或 EWOULDBLOCK。
//      这与通过 fcntl(2) 的 F_SETFL 操作设置 O_NONBLOCK 标志类似，但有所不同：MSG_DONTWAIT
//      是每次调用的选项，而 O_NONBLOCK 是打开文件描述符上的设置（参见 open(2)），它会
//      影响调用进程中的所有线程以及其他持有相同打开文件描述符的进程。
// MSG_MORE Linux 2.4.4 - 调用者还有更多数据要发送。此标志用于 TCP 套接字，以实现与
//      TCP_CORK 套接字选项（参见 tcp(7)）相同的效果，不同之处在于，此标志可以在每次调
//      用的基础上设置。自 Linux 2.6 起，此标志也适用于 UDP 套接字，并告知内核将所有设
//      置了此标志的调用中发送的数据打包到一个数据报中，仅在未设置此标志的调用执行时才发
//      送该数据报，参考 udp(7) 中描述的 UDP_CORK 套接字选项。
// MSG_FASTOPEN Linux 3.7 - 尝试 TCP 快速打开（RFC7413），像 connect(2) 和 write(2)
//      的组合一样，在 SYN 中发送数据，通过执行隐式的 connect(2) 操作来实现。它会阻塞，
//      直到数据被缓冲并且握手完成。对于非阻塞套接字，它会返回在 SYN 数据包中缓冲并发送
//      的字节数。如果本地没有可用的 cookie，则返回 EINPROGRESS，并自动发送一个带有
//      Fast Open cookie 请求的 SYN。当套接字连接成功后，调用者需要重新写入数据。如果
//      握手失败，它会设置与 connect(2) 相同的 errno。此标志需要在 sysctl net.ipv4.tcp_fastopen
//      中启用 TCP Fast Open 客户端支持。请参考 tcp(7) 中的 TCP_FASTOPEN_CONNECT 套
//      接字选项，了解另一种方法。
// MSG_CONFIRM Linux 2.3.15 - 是 Linux 系统的扩展标志，告知链路层已取得进展：你从对端
//      收到了成功的回复。如果链路层没有收到这个信号，它会定期重新探测邻居（例如，通过单
//      播 ARP）。仅在 SOCK_DGRAM 和 SOCK_RAW 套接字上有效，目前仅针对 IPv4 和 IPv6
//      实现。详情请参见 arp(7)。
//
// 可能的错误：以下是由套接字层产生的一些标准错误。底层协议模块可能会产生并返回其他错误；
// 请参阅它们各自的手册页。
// EACCES -（对于通过路径名标识的 UNIX 域套接字）对目标套接字文件的写权限被拒绝，或者路
//      径前缀中的某个目录的搜索权限被拒绝，参见 path_resolution(7)。（对于UDP套接字）
//      像给单播地址发送数据一样给网络/广播地址发送数据。
// EAGAIN EWOULDBLOCK - 套接字被标记为非阻塞，且请求的操作会阻塞。POSIX.1-2001 允许对
//      这种情况返回任一错误，并且不要求这些常量具有相同的值，因此可移植的应用程序应检查
//      这两种可能性。
// EAGAIN -（Internet 域数据报套接字）由 sockfd 指向的套接字之前未绑定到地址，并且在尝
//      试将其绑定到一个临时端口时，发现临时端口范围内的所有端口号当前都在使用。请参见
//      ip(7) 中关于 /proc/sys/net/ipv4/ip_local_port_range 的讨论。
// EALREADY - 另一个快速打开操作正在进行。
// EBADF - sockfd 不是一个有效的打开文件描述符。
// ECONNRESET - 连接被对端重置。
// EDESTADDRREQ - 套接字不是面向连接的，并且未设置对端地址。
// EFAULT - 指定了无效的用户空间地址作为参数。
// EINTR - 在传输任何数据之前被信号中断；请参见 signal(7)。
// EINVAL - 传递了无效的参数。
// EISCONN - 面向连接的套接字已经连接，但指定了一个接收者。现在要么返回此错误，要么忽略
//      接收者指定。
// EMSGSIZE - 套接字类型要求消息以原子方式发送，但要发送的消息大小使得这不可能。
// ENOBUFS - 网络接口的输出队列已满。这通常表明接口已停止发送，但可能是由瞬态拥塞引起的。
//      通常在 Linux 中不会出现这种情况。当设备队列溢出时，数据包会被静默丢弃。
// ENOMEM - 没有可用内存。
// ENOTCONN - 套接字未连接，并且未指定目标。Linux 上可能返回 EPIPE 而不是 ENOTCONN。
// ENOTSOCK - 文件描述符 sockfd 不指向套接字。
// EOPNOTSUPP - flags 参数中的某些位对于套接字类型不适合。
// EPIPE - 在面向连接的套接字上，本地端已经关闭。在这种情况下，除非设置了 MSG_NOSIGNAL，
//      否则进程还会收到 SIGPIPE。
//
// #include <sys/sendfile.h>
// ssize_t sendfile(int out_fd, int in_fd, off_t *_Nullable offset, size_t count);

// #include <fcntl.h>
// int fcntl(int fd, int op, ... /* arg */ );
#include <fcntl.h>
void prh_sock_set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);
    assert(flags != -1);
    if (flags & O_NONBLOCK) return;
    flags |= O_NONBLOCK;
    prh_nnegret(fcntl(fd, F_SETFL, flags));
}
void prh_sock_set_block(int fd) {
    int flags = fcntl(fd, F_GETFL);
    assert(flags != -1);
    if ((flags & O_NONBLOCK) == 0) return;
    flags &= ~O_NONBLOCK;
    prh_nnegret(fcntl(fd, F_SETFL, flags));
}

// #include <sys/socket.h> 成功返回 0，失败返回 -1 和 errno
// int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
// int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

typedef struct {
    prh_u32 src_addr;
    prh_u32 dst_addr;
    prh_u16 src_port;
    prh_u16 dst_port;
} prh_sock_port;

typedef struct {
    prh_sock_port port;
} prh_tcp_client;

typedef struct {
    prh_sock_port port;
    int backlog;
} prh_tcp_server; // tcpd

typedef struct {
    prh_sock_port port;
} prh_tcp_conn;

#endif

// https://www.man7.org/linux/man-pages/man7/packet.7.html
//
// libpcap 使用 socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)) 创建原始套接字，接收
// 所有链路层数据帧。其接收函数为 packet_rcv()，该函数将数据包放入对应 socket 的接收队
// 列中。BPF 是 libpcap 的核心过滤机制，它允许用户定义过滤规则（如只捕获 TCP 端口 80
// 的数据包），并在内核中执行过滤逻辑，减少不必要的数据拷贝。libpcap 的改进版本（如
// libpcap-mmap）使用 mmap 技术将内核缓冲区映射到用户空间，避免了额外的数据拷贝，提高了
// 性能。libpcap 在数据链路层插入一个“旁路”，不干扰系统正常协议栈处理。它通过创建 PF_PACKET
// 类型的原始套接字，直接从链路层驱动获取数据包，绕过 TCP/IP 协议栈，从而提高捕获效率。
// 当数据包到达网卡时，libpcap 通过以下路径捕获数据：
// 网卡接收数据包 → 触发中断 → 网卡驱动分配 sk_buff 并拷贝数据（第一次拷贝）；
// 根据是否启用 NAPI，调用 netif_rx() 或 netif_rx_schedule() 将数据包送入内核网络栈；
// 触发软中断 NET_RX_SOFTIRQ，执行 net_rx_action()；
// 调用 netif_receive_skb()，若有抓包程序，数据包进入 BPF 过滤器；
// 匹配成功的数据包被拷贝到内核缓冲区（第二次拷贝）；
// 用户空间通过 recvfrom() 系统调用将数据包从内核缓冲区拷贝到用户缓冲区（第三次拷贝）。
//

#ifdef PRH_TEST_IMPLEMENTATION
void prh_impl_sock_test(void) {
#ifdef INET_ADDRSTRLEN
    printf("INET_ADDRSTRLEN %d\n", INET_ADDRSTRLEN);
#endif
#ifdef INET6_ADDRSTRLEN
    printf("INET6_ADDRSTRLEN %d\n", INET6_ADDRSTRLEN);
#endif
#ifdef NI_MAXHOST
    printf("NI_MAXHOST %d\n", NI_MAXHOST); // 最大域名长度
#endif
#ifdef NI_MAXSERV
    printf("NI_MAXSERV %d\n", NI_MAXSERV); // 最大的端口服务名称长度
#endif
}
#endif // PRH_TEST_IMPLEMENTATION
#endif // PRH_SOCK_IMPLEMENTATION
#endif // PRH_SOCK_INCLUDE

#ifdef PRH_CONO_INCLUDE
typedef struct prh_impl_cono prh_cono;
#define prh_cono_proc prh_fastcall(void)
prh_fastcall_typedef(void, prh_conoproc_t)(prh_cono *);

typedef struct prh_spawn_data prh_spawn_data;
typedef struct prh_await_data prh_await_data;

void prh_cono_main(int thrd_start_id, int num_thread, prh_conoproc_t main_proc, int stack_size);
void *prh_cono_spawn(prh_conoproc_t proc, int stack_size, int maxudsize);
void prh_cono_start(prh_cono *coro, prh_spawn_data *cono_spawn_data);
void *prh_cono_await(prh_cono *coro);
void prh_cono_continue(prh_cono *coro, prh_await_data *cono_await_data);
void prh_impl_cross_thread_coro_yield(prh_cono *coro);

prh_inline void prh_cono_yield(prh_cono *coro) {
    prh_impl_cross_thread_coro_yield(coro);
    prh_soro_yield((prh_soro *)coro);
}

prh_inline void *prh_cono_data(prh_cono *coro) {
    return prh_soro_data((prh_soro *)coro);
}

#ifdef PRH_CONO_IMPLEMENTATION
#ifndef PRH_CONO_DEBUG
#define PRH_CONO_DEBUG PRH_DEBUG
#endif

typedef struct prh_real_cono prh_real_cono;
typedef struct {
    prh_real_cono *head;
    prh_real_cono *tail;
} prh_cono_quefit;

typedef struct prh_cono_thrd prh_cono_thrd;
struct prh_real_cono {
    // 仅由执行线程访问
    prh_i32 cono_id;
    prh_byte action;
    bool idle_await;
    prh_real_cono *caller;
    prh_real_cono *await_callee;
    prh_real_cono *start_callee;
    // 仅由特权线程访问
    prh_cono_quefit callee_que;
    prh_real_cono *cono_chain;
    prh_cono_thrd *assign_thrd;
    prh_quefit pdata_que;
};

prh_inline void prh_impl_cono_init(prh_real_cono *cono, prh_i32 cono_id) {
    cono->cono_id = cono_id;
}

prh_inline prh_coro *prh_impl_coro_from_cono(prh_real_cono *cono) {
    return (prh_coro *)((char *)cono - prh_impl_coro_size);
}

prh_inline void prh_impl_cono_free(prh_real_cono *cono) {
    assert(prh_relaxed_quefit_empty(&cono->callee_que));
    assert(prh_quefit_empty(&cono->pdata_que));
    prh_impl_coro_free(prh_impl_coro_from_cono(cono));
}

prh_inline bool prh_impl_cono_finished(prh_real_cono *cono) {
    return prh_impl_coro_from_cono(cono)->rspoffset == 0;
}

prh_inline prh_real_cono *prh_impl_cono_from_coro(prh_cono *coro) {
    return (prh_real_cono *)((char *)coro + prh_impl_coro_size);
}

prh_inline int prh_impl_cono_size(void) {
    return (int)prh_round_16_byte(sizeof(prh_real_cono));
}

prh_inline void *prh_impl_get_cono_data(prh_real_cono *cono) {
    return (char *)cono + prh_impl_cono_size();
}

prh_inline prh_real_cono *prh_impl_cono_from_data(void *data) {
    return (prh_real_cono *)((char *)data - prh_impl_cono_size());
}

struct prh_cono_thrd {
    prh_int cono_thrd_index;
    prh_real_cono *grabbed_cono;
    prh_atom_data_quefix cono_req_que; // 由当前线程写入，由特权线程读取
    prh_atom_ptr ready_cono; // 由特权线程写入，由特权线程窃取清空，或由当前协程读取清空
    prh_sleep_cond sleep_cond;
}; // 每个线程尽量指定在单一的CPU上运行避免线程切换

#define prh_impl_cono_thrd_size (int)sizeof(prh_cono_thrd)

prh_inline prh_byte *prh_impl_thrd_pending_work(prh_cono_thrd *cono_thrd) {
    return ((prh_byte *)cono_thrd - prh_impl_thrd_extra_len); // 仅由当前线程访问
}

prh_inline prh_cono_thrd *prh_impl_cono_thrd(prh_thrd *thrd) {
    return (prh_cono_thrd *)((char *)thrd + prh_impl_thrd_head_size);
}

prh_inline int prh_cono_thrd_id(prh_cono_thrd *cono_thrd) {
    return prh_thrd_id((prh_thrd *)((char *)cono_thrd - prh_impl_thrd_head_size));
}

void prh_impl_cono_thrd_init(prh_cono_thrd *cono_thrd, int thrd_index) {
    cono_thrd->cono_thrd_index = thrd_index;
    prh_atom_data_quefix_init(&cono_thrd->cono_req_que);
    prh_atom_ptr_init(&cono_thrd->ready_cono, prh_null);
    prh_impl_sleep_cond_init(&cono_thrd->sleep_cond);
}

void prh_impl_cono_thrd_free(void *userdata, int thrd_index) {
    prh_cono_thrd *cono_thrd = userdata;
    prh_release_assert(prh_atom_data_quefix_empty(&cono_thrd->cono_req_que));
    prh_release_assert(prh_atom_ptr_read(&cono_thrd->ready_cono) == prh_null);
    prh_impl_sleep_cond_free(&cono_thrd->sleep_cond);
}

typedef enum {
    PRH_CONO_ID_NULL,
    PRH_FIXED_CONO_MAX,
} prh_impl_cono_id;

typedef struct {
    prh_thrd_struct *thrd_struct; // 初始化后只读
    prh_cono_thrd *main_thread; // 初始化后只读
    prh_real_cono *entry_cono; // 初始化后只读
    prh_real_cono *fixed_cono[PRH_FIXED_CONO_MAX]; // 初始化后只读
    // 多线程读写
    prh_cono_quefit ready_queue;
    prh_i32 cono_id_seed;
    prh_atom_ptr privilege_thread;
    prh_atom_bool thrd_exit;
} prh_cono_struct;

static prh_cono_struct PRH_IMPL_CONO_STRUCT;
prh_fastcall(void) prh_impl_asm_cono_call(void);

prh_inline void prh_impl_cono_load(prh_coro *coro, prh_conoproc_t proc) {
    prh_impl_coro_load_stack(coro, (prh_void_ptr)proc, (prh_void_ptr)prh_impl_asm_cono_call);
}

prh_coro *prh_impl_cono_creatx(prh_conoproc_t proc, int stack_size, int coro_extend_size, int maxudsize) {
    prh_coro *coro = prh_impl_coro_alloc(stack_size, coro_extend_size, maxudsize);
    prh_impl_cono_load(coro, proc);
#if PRH_CONO_DEBUG
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[cono ##] %p create -- lower %p (left %d) rsp %p coro %p (size %d) data %p (size %d) -- stack %d\n",
        (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata, (int)prh_round_16_byte(maxudsize), stack_size);
#endif
    return coro;
}

prh_real_cono *prh_impl_cono_create(prh_conoproc_t proc, int stack_size, void *userdata) {
    prh_coro *coro = prh_impl_cono_creatx(proc, stack_size, sizeof(prh_real_cono), 0);
    coro->userdata = userdata;
    return prh_impl_cono_from_coro((prh_cono *)coro);
}

prh_cono_struct *prh_impl_cono_struct_init(int thrd_start_id, int num_thread, prh_conoproc_t main_proc, int stack_size) {
    prh_cono_struct *s = &PRH_IMPL_CONO_STRUCT;
    prh_thrd_struct *thrd_struct = prh_thrd_inix(thrd_start_id, num_thread, prh_impl_cono_thrd_size);
    s->thrd_struct = thrd_struct;
    s->main_thread = prh_impl_cono_thrd(thrd_struct->main);
    s->entry_cono = prh_impl_cono_create(main_proc, stack_size, 0);
    prh_impl_cono_thrd_init(s->main_thread, 0);
    prh_atom_ptr_init(&s->privilege_thread, prh_null);
    prh_atom_bool_init(&s->thrd_exit, false);
    return s;
}

void prh_impl_fixed_cono_init(prh_real_cono *cono, prh_i32 cono_id) {
    prh_impl_cono_init(cono, cono_id);
    assert(cono_id < PRH_FIXED_CONO_MAX);
    PRH_IMPL_CONO_STRUCT.fixed_cono[cono_id] = cono;
}

prh_real_cono *prh_impl_get_fixed_coro(prh_int cono_id) {
    assert(cono_id < PRH_FIXED_CONO_MAX);
    return PRH_IMPL_CONO_STRUCT.fixed_cono[cono_id];
}

void prh_impl_cono_struct_free(void) {
    prh_cono_struct *s = &PRH_IMPL_CONO_STRUCT;
    prh_release_assert(prh_relaxed_quefit_empty(&s->ready_queue));
    for (int i = 0; i < PRH_FIXED_CONO_MAX; i += 1) {
        prh_real_cono *fixed_cono = s->fixed_cono[i];
        if (fixed_cono) prh_impl_cono_free(fixed_cono);
    }
    prh_impl_cono_free(s->entry_cono);
    prh_thrd_free(&PRH_IMPL_CONO_STRUCT.thrd_struct, prh_impl_cono_thrd_free);
}

// 要实现无锁且省去N*M爆炸内存，必须有一个线程担任中间角色，请求者与中间角色是一对一的，
// 中间角色与执行者也是一对一的，从而能够实现无锁。但中间角色的加入，必然带来一定延迟，
// 因此必须保证中间角色负责的中转任务即使在任务繁重的情况下，也能很快的高优先级完成。因
// 此必须有一个线程偏向性的来执行这些任务，避免所有线程都被一些费时任务霸占，导致轻量任
// 务被迫挨饿。
//
// 可以让主线程偏重执行这个任务，轻量执行时间短且优先级高的任务都属于此类。基本类型的原子
// 操作可以实现N个线程的竞争执行，但对于队列等复杂数据结构的无锁操作需要依赖一个线程读一
// 个线程写的一对一模型。如果不依靠一对一模型，N个线程同时竞争，一个任务就需要维护N个数据
// 结构，这样每个线程才能无锁的互不干扰的对这个任务进行读写。相当于用内存隔离来避免线程的
// 访问竞争，但是有M个任务就必须爆炸式的分配N*M份内存，在嵌入式等内存短缺的平台上不是一
// 种好的解决方案。
//
// 使用一对一的单生产者和单消费者的模式，请求线程将请求数据暂存到自己的发送队列，作为中间
// 角色的特权线程，遍历全部线程的发送队列，将发送队列中的请求数据转发到执行协程的接收队列
// 中，此执行协程此时可能正在某个线程中执行自己的任务，但如果执行协程没有在执行需要将该协
// 程添加到等待队列中去进行调度。
//
// root 协程执行轻量的高优先级任务，主线程完全偏向执行这个协程，另外所有其他线程可以平等
// 地像执行其他普通协程一样竞争执行这个协程，执行 root 协程的线程也被称为特权线程。主线
// 程除了最高优先级执行 root 协程外，还可以安排执行其他特殊的轻量高优先级任务。
//
// 协程间消息通信汇总：父协程也可理解为请求协程，子协程也可理解为执行协程
// 1. 子协程创建时给特权协程发送创建消息（只需给特权协程发送消息）
//          父协程的状态：挂起，等待特权协程创建好子协程后分配给线程执行，协程记录在子协程中
//          子协程的状态：还未创建，等待特权协程创建好之后直接分配给线程执行，协程记录在请求消息中
// 2. 如果创建独立的公共执行协程，有父协程的话父协程会等待执行协程创建，然后给执行协程发送消息之后才能等待执行协程，如果没有父协程也就不会挂起父协程，特权协程也不会直接执行父协程（只需给特权协程发送消息）
//          子协程的状态：还未创建，等待特权协程创建好之后直接分配给线程执行，协程记录在请求消息中
// 3. 需要提交结果的子协程给父协程发送消息提交结果（需要给父协程发送消息，然后父协程给特权协程发送消息）
//          父协程的状态：未知
//          子协程的状态：挂起，等待父协程处理消息，然后父协程给特权协程发送消息直接执行子协程
// 4. 如果父协程处理完子协程提交的结果后，也需要继续给子协程数据通信（父协程给特权协程发送消息）
//          父协程的状态：处理完子协程递交的结果后，直接将相关数据写入子协程，然后给特权协程发送消息直接执行子协程
//          子协程的状态：需要提交结果的子协程，一般执行完一次父协程给的任务后就会执行完毕，如果是多次执行任务，需要父协程继续提交任务直到父协程明确不需要继续执行任务后结束
// 5. 如果子协程不需要给父协程递交结果（需要父协程给请求协程发送消息）
//          子协程的状态：挂起后，如果已经执行完毕则释放协程，否则等待请求协程发送任务请求消息
// 6. 子协程给父协程发送消息递交结果，或者父协程给执行协程发送消息请求任务，此时目标协程因为状态未知，需要特殊处理
// 7. 第一种情况，子协程执行完任务后挂起，可以不给父进程发送消息，而是交由特权线程处理
//          子协程的状态：挂起后给特权协程发送消息，表示等待父协程的WAIT挂起，只要父协程WAIT挂起就直接执行父协程
//          特权协程实现：收到子协程的消息后，将其挂到父协程的WAIT队列中，当父协程WAIT挂起时，给特权线程发送消息，特权线程收到消息后取出一个子协程让父协程去执行，当然优先分配给当前线程执行
// 8. 第二种情况：不需要提交结果的执行协程没有任务执行时，也会给特权线程发送消息，特权协程也会取出一个请求数据直接分配执行协程直接执行
//          清求协程状态：请求协程发送请求数据给特权协程，特权协程将数据挂到执行协程的WAIT队列中，然后等待执行协程的WAIT挂起
//          执行协程状态：当执行协程执行完当前任务挂起后，给特权协程发送消息，特权协程收到消息后，取出一个数据直接执行执行协程
//          特权协程实现：如果执行协程先挂起，但队列中已经没有要处理的任务了，就会设置idle_wait标记，下次接收到请求数据就直接激活执行协程的执行
typedef enum {
    PRH_CONO_CONTINUE,
    PRH_CONO_START, // 启动新建协程
    PRH_CONO_YIELD,
    PRH_CONO_AWAIT,
    PRH_CONO_PDATA,
    PRH_CONO_PWAIT,
    PRH_CONO_REQ_MAX
} prh_impl_cono_action;

#if PRH_CONO_DEBUG
const char *prh_impl_cono_action_string(int action) {
    const char *action_string[PRH_CONO_REQ_MAX] = {
        "CONTINUE", "START", "YIELD", "AWAIT", "PDATA", "PWAIT",
    };
    return action_string[action];
}

int prh_impl_curr_cono_thrd_id(void) {
    return prh_thrd_id(prh_thrd_self());
}
#endif

void prh_impl_cono_wakeup_all_thrd(void) {
    prh_thrd_struct *thrd_struct = PRH_IMPL_CONO_STRUCT.thrd_struct;
    prh_thrd **it = prh_thrd_list_begin(thrd_struct);
    for (; it < prh_thrd_list_end(thrd_struct); it += 1) {
        if (*it == prh_null) continue;
        prh_cono_thrd *thrd = prh_impl_cono_thrd(*it);
        prh_thrd_wakeup(&thrd->sleep_cond);
    }
}

void prh_impl_cono_start_exit(void) {
    prh_atom_bool_write(&PRH_IMPL_CONO_STRUCT.thrd_exit, true);
    prh_impl_cono_wakeup_all_thrd();
}

void prh_impl_process_cono_continue_req(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {
    if (prh_impl_cono_finished(req_cono)) {
        if (req_cono == PRH_IMPL_CONO_STRUCT.entry_cono) {
            prh_impl_cono_start_exit();
        } else {
#if PRH_CONO_DEBUG
            printf("[thrd %02d] cono %02d finished\n", prh_impl_curr_cono_thrd_id(), req_cono->cono_id);
#endif
            prh_impl_cono_free(req_cono);
        }
    } else { // 继续执行协程
        prh_relaxed_quefit_push(ready_queue, req_cono, cono_chain);
    }
}

#define PRH_IMPL_CALLER_YIELD_WHEN_CREATE_NEW_CORO 0

#if PRH_IMPL_CALLER_YIELD_WHEN_CREATE_NEW_CORO
void prh_impl_process_cono_start_req(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {
    prh_real_cono *callee = req_cono->start_callee;
    prh_impl_cono_init(callee, ++PRH_IMPL_CONO_STRUCT.cono_id_seed); // 初始化新协程
    callee->assign_thrd = req_cono->assign_thrd;
#if PRH_CONO_DEBUG
    printf("[thrd %02d] cono %02d created by cono %02d\n", prh_impl_curr_cono_thrd_id(), callee->cono_id, req_cono->cono_id);
#endif
    prh_relaxed_quefit_push(ready_queue, callee, cono_chain); // 优先调度新建协程
    prh_relaxed_quefit_push(ready_queue, req_cono, cono_chain);
}
#else
void prh_impl_process_cono_start_req(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {
    prh_real_cono *callee = req_cono;
    prh_impl_cono_init(callee, ++PRH_IMPL_CONO_STRUCT.cono_id_seed); // 初始化新协程
#if PRH_CONO_DEBUG
    printf("[thrd %02d] cono %02d created by cono %02d\n", prh_impl_curr_cono_thrd_id(), callee->cono_id, callee->caller->cono_id);
#endif
    prh_relaxed_quefit_push(ready_queue, callee, cono_chain);
}
#endif

void prh_impl_process_cono_yield_req(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {
    prh_real_cono *caller = req_cono->caller;
    if (caller) { // 需要提交执行结果给请求协程
        if (caller->idle_await) { // 请求协程正在等待执行结果，且其结果队列为空
            caller->await_callee = req_cono;
            caller->idle_await = false; // 将结果提交给请求协程，并立即调度其执行
            prh_relaxed_quefit_push(ready_queue, caller, cono_chain);
        } else { // 将执行协程插入请求协程的结果队列中，等待请求下一次WAIT挂起继续执行
            prh_relaxed_quefit_push(&caller->callee_que, req_cono, cono_chain);
        }
    } else { // 执行协程不需要将结果提交给请求协程，执行结束或者继续无条件执行
        prh_impl_process_cono_continue_req(req_cono, ready_queue);
    }
}

void prh_impl_process_cono_await_req(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {
    prh_real_cono *callee; // 等待执行协程的执行结果
    prh_relaxed_quefit_pop(&req_cono->callee_que, callee, cono_chain);
    if (callee) { // 协程给特权发送消息，都会挂起等待特权调度继续执行，因此协程的请求都是顺序的，对于一个协程不可能同时有多个等待执行的请求存在
        req_cono->await_callee = callee; // 有结果存在，直接交给请求协程继续执行
        prh_relaxed_quefit_push(ready_queue, req_cono, cono_chain);
    } else {
        req_cono->idle_await = true; // 暂时没有执行结果可以处理，等待下一次执行协程的结果
    }
}

void prh_impl_process_cono_pdata_req(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {

}

void prh_impl_process_cono_pwait_req(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {

}

typedef void (*prh_impl_cono_req_func)(prh_real_cono *req_cono, prh_cono_quefit *ready_queue);
static prh_impl_cono_req_func PRH_IMPL_CONO_REQ_FUNC[PRH_CONO_REQ_MAX] = {
    prh_impl_process_cono_continue_req,
    prh_impl_process_cono_start_req,
    prh_impl_process_cono_yield_req,
    prh_impl_process_cono_await_req,
    prh_impl_process_cono_pdata_req,
    prh_impl_process_cono_pwait_req,
};

void prh_impl_send_cono_req(prh_real_cono *req_cono, int action) {
    prh_cono_thrd *thrd = prh_thrd_self_data();
#if PRH_CONO_DEBUG
    if (action != PRH_CONO_START) { // 需要启动的新建协程的 cono_id 此时还没有分配
        printf("[thrd %02d] cono %02d request %s\n", prh_impl_curr_cono_thrd_id(), req_cono->cono_id, prh_impl_cono_action_string(action));
    }
#endif
    req_cono->action = action;
    prh_atom_data_quefix_push(&thrd->cono_req_que, req_cono);
}

void prh_impl_callee_continue(prh_real_cono *caller) {
    if (caller->await_callee) {
        prh_impl_send_cono_req(caller->await_callee, PRH_CONO_CONTINUE);
        caller->await_callee = prh_null;
    }
}

void prh_impl_cross_thread_coro_yield(prh_cono *coro) {
    prh_real_cono *cono = prh_impl_cono_from_coro(coro);
    cono->action = PRH_CONO_YIELD;
}

prh_fastcall(void *) prh_impl_asm_cono_finish(prh_coro *coro) {
    prh_impl_cross_thread_coro_yield((prh_cono *)coro);
    return prh_impl_asm_soro_finish(coro);
}

void prh_impl_cono_wait(prh_real_cono *cono, int action) {
    cono->action = action;
    prh_soro_yield((prh_soro *)prh_impl_coro_from_cono(cono));
}

void *prh_cono_spawn(prh_conoproc_t proc, int stack_size, int maxudsize) {
    prh_coro *coro = prh_impl_cono_creatx(proc, stack_size, sizeof(prh_real_cono), maxudsize);
    return prh_coro_data(coro);
}

#if PRH_IMPL_CALLER_YIELD_WHEN_CREATE_NEW_CORO
void prh_cono_start(prh_cono *coro, prh_spawn_data *cono_spawn_data) {
    prh_real_cono *caller = prh_impl_cono_from_coro(coro);
    prh_real_cono *callee = prh_impl_cono_from_data(cono_spawn_data);
    callee->caller = caller;
    caller->start_callee = callee;
    prh_impl_cono_wait(caller, PRH_CONO_START);
    return callee->cono_id;
}
#else
void prh_cono_start(prh_cono *coro, prh_spawn_data *cono_spawn_data) {
    prh_real_cono *caller = prh_impl_cono_from_coro(coro);
    prh_real_cono *callee = prh_impl_cono_from_data(cono_spawn_data);
    callee->caller = caller;
    prh_impl_send_cono_req(callee, PRH_CONO_START);
}
#endif

void *prh_cono_await(prh_cono *coro) {
    prh_real_cono *caller = prh_impl_cono_from_coro(coro);
    prh_impl_cono_wait(caller, PRH_CONO_AWAIT);
    return prh_impl_get_cono_data(caller->await_callee);
}

void prh_cono_continue(prh_cono *coro, prh_await_data *cono_await_data) {
    prh_real_cono *caller = prh_impl_cono_from_coro(coro);
    assert(caller->await_callee == prh_impl_cono_from_data(cono_await_data));
    prh_impl_callee_continue(caller);
}

void prh_impl_cono_execute(int thrd_id, prh_real_cono *cono) {
    prh_coro *coro = prh_impl_coro_from_cono(cono);
#if PRH_CONO_DEBUG
    printf("[thrd %02d] cono %02d start running\n", thrd_id, cono->cono_id);
#endif
    if (cono->action == PRH_CONO_AWAIT) {
        prh_impl_soro_start(thrd_id, coro);
        prh_impl_callee_continue(cono);
    } else { // 继续执行当前协程，直到协程挂起或执行完毕返回到主协程
        prh_impl_soro_start(thrd_id, coro);
    }
    prh_impl_send_cono_req(cono, cono->action); // 返回主协程之前，cono会设置好action
}

prh_inline void prh_impl_cono_req_process(prh_real_cono *req_cono, prh_cono_quefit *ready_queue) {
    assert(req_cono->action < PRH_CONO_REQ_MAX);
    PRH_IMPL_CONO_REQ_FUNC[req_cono->action](req_cono, ready_queue);
}

bool prh_impl_privilege_task(prh_cono_thrd *curr_thrd, bool strong_check) {
    prh_atom_ptr *privilege_thread = &PRH_IMPL_CONO_STRUCT.privilege_thread;
    prh_cono_quefit *ready_queue = &PRH_IMPL_CONO_STRUCT.ready_queue;
    prh_thrd_struct *thrd_struct = PRH_IMPL_CONO_STRUCT.thrd_struct;
    prh_data_quefit all_req;
    prh_real_cono *ready_cono;
    prh_cono_thrd *thrd;

    if (!prh_atom_ptr_compare_null_write(privilege_thread, curr_thrd)) { // 获取特权
        return false;
    }

    if (strong_check) {
        curr_thrd->grabbed_cono = prh_null;
        *prh_impl_thrd_pending_work(curr_thrd) = false;
    }

    prh_thrd **thrd_begin = prh_thrd_list_begin(thrd_struct);
    prh_thrd **thrd_end = prh_thrd_list_end(thrd_struct);
    prh_thrd **thrd_it = thrd_begin;

    for (; thrd_it < thrd_end; thrd_it += 1) { // 处理特权消息，将就绪协程插入就绪队列
        if (*thrd_it == prh_null) continue;
        thrd = prh_impl_cono_thrd(*thrd_it);
        if (!prh_atom_data_quefix_pop_all(&thrd->cono_req_que, &all_req)) {
            continue;
        }
        prh_data_quefit_for_begin(&all_req) {
            prh_impl_cono_req_process((prh_real_cono *)it->data, ready_queue);
            prh_data_quefit_for_end();
        }
        prh_atom_data_quefix_free_node(&all_req);
    }

    thrd_it = thrd_begin;
    for (; ;) { // 将就绪队列中的协程分配给线程执行
        prh_relaxed_quefit_pop(ready_queue, ready_cono, cono_chain);
        if (ready_cono == prh_null) break;
        thrd = ready_cono->assign_thrd;
        if (thrd && prh_atom_ptr_strong_compare_null_write(&thrd->ready_cono, ready_cono)) {
#if PRH_CONO_DEBUG
            printf("[thrd %02d] cono %02d => orig thrd %02d\n", prh_impl_curr_cono_thrd_id(), ready_cono->cono_id, prh_cono_thrd_id(thrd));
#endif
            prh_thrd_wakeup(&thrd->sleep_cond);
            continue; // 尽量将协程分配在同一个线程中执行，保证空间访问局部性
        }
        for (; thrd_it < thrd_end; thrd_it += 1) { // 将协程分配给空闲线程
            if (*thrd_it == prh_null) continue;
            thrd = prh_impl_cono_thrd(*thrd_it);
            if (prh_atom_ptr_strong_compare_null_write(&thrd->ready_cono, ready_cono)) {
                ready_cono->assign_thrd = thrd;
#if PRH_CONO_DEBUG
                printf("[thrd %02d] cono %02d => idle thrd %02d\n", prh_impl_curr_cono_thrd_id(), ready_cono->cono_id, prh_cono_thrd_id(thrd));
#endif
                prh_thrd_wakeup(&thrd->sleep_cond);
                ready_cono = prh_null;
                break;
            }
        }
        if (ready_cono) { // 没有足够的线程处理该协程，将协程重新插入就绪队列，并退出循环
            prh_relaxed_quefit_push_front(ready_queue, ready_cono, cono_chain);
            break;
        }
    }

    if (strong_check && curr_thrd != PRH_IMPL_CONO_STRUCT.main_thread && !curr_thrd->ready_cono) { // 如果当前线程没有分配到协程，从其他线程争抢已分配的协程来执行
        thrd_it = thrd_begin; // 主线程不需要争抢任务，如果就绪队列里面有任务，第一个分配的线程就是主线程，如果就绪队列里已经没有任务了，表示任务都已经分配完毕了，不需要主线程帮忙
        for (; thrd_it < thrd_end; thrd_it += 1) {
            if (*thrd_it == prh_null) continue;
            thrd = prh_impl_cono_thrd(*thrd_it);
            if ((ready_cono = prh_atom_ptr_read(&thrd->ready_cono)) && prh_atom_ptr_compare_write(&thrd->ready_cono, (void **)&ready_cono, prh_null)) {
#if PRH_CONO_DEBUG
                printf("[thrd %02d] cono %02d grabbed from thrd %d\n", prh_impl_curr_cono_thrd_id(), ready_cono->cono_id, prh_cono_thrd_id(thrd));
#endif
                curr_thrd->grabbed_cono = ready_cono; // 不赋值给 curr_thrd->ready_cono 是避免刚抢的协程又被别的线程抢掉
                ready_cono->assign_thrd = curr_thrd;
                break;
            }
        }
    }

    if (strong_check && curr_thrd == PRH_IMPL_CONO_STRUCT.main_thread) {
        if (curr_thrd->ready_cono) {
            *prh_impl_thrd_pending_work(curr_thrd) = true;
        } else {
            // 如果不进行下面的检查，会发生所有线程都睡眠的情况，例如：
            // 1. 主线程唤醒，抢到特权，将一个子协程分配给第二个线程处理
            // 2. 第二个线程唤醒，处理完这个子协程再睡眠，然后主线程将入口协程分配给第二个线程
            // 3. 第二个线程唤醒，处理入口协程，创建一个新协程到消息队列，因为抢不到特权而睡眠
            // 4. 主线程处理完特权任务后，没有检查各线程中是否还有等待的消息，就退出特权然后睡眠
            // 5. 此时所有线程都睡眠，但线程中还有特处理的消息
            thrd_it = thrd_begin;
            for (; thrd_it < thrd_end; thrd_it += 1) {
                if (*thrd_it == prh_null) continue;
                thrd = prh_impl_cono_thrd(*thrd_it); // 查看各线程是否还有遗留的未处理的消息
                if (!prh_atom_data_quefix_empty(&thrd->cono_req_que)) {
                    *prh_impl_thrd_pending_work(curr_thrd) = true;
                    break;
                }
            }
        }
    }

    prh_atom_ptr_write(privilege_thread, prh_null); // 释放特权
    return true;
}

prh_void_ptr prh_impl_cono_thrd_proc(prh_thrd* thrd) {
    prh_cono_thrd *cono_thrd = prh_impl_cono_thrd(thrd);
    prh_cono_thrd *main_thrd = PRH_IMPL_CONO_STRUCT.main_thread;
    prh_atom_ptr *thrd_ready_cono = &cono_thrd->ready_cono;
    prh_real_cono *ready_cono;
    bool req_que_not_empty;
    int thrd_id = prh_thrd_id(thrd);
#if PRH_CONO_DEBUG
    int atom_write_succ = 0;
    int atom_write_fail = 0;
#endif

    for (; ;) {
        while ((ready_cono = prh_atom_ptr_read(thrd_ready_cono))) {
label_cont_execute:
            if (prh_atom_ptr_compare_write(thrd_ready_cono, (void **)&ready_cono, prh_null)) {
#if PRH_CONO_DEBUG
                atom_write_succ += 1;
#endif
                prh_impl_cono_execute(thrd_id, ready_cono);
            } else { // 自己的任务可能被别的线程抢掉
#if PRH_CONO_DEBUG
                atom_write_fail += 1;
#endif
                prh_impl_privilege_task(cono_thrd, false);
            }
        }
        if (prh_impl_privilege_task(cono_thrd, true) && cono_thrd->grabbed_cono) {
            prh_impl_cono_execute(thrd_id, cono_thrd->grabbed_cono);
        }
        if ((ready_cono = prh_atom_ptr_read(thrd_ready_cono))) {
            goto label_cont_execute; // 被安排新任务，或在没抢到特权的情况下，可能被其他特权线程安排任务
        }
        // 如果在特权线程处理完待处理消息并将要退出的前一刻，当前线程又向消息队列投递了
        // 消息，此时特权线程包括所有其他线程可能都进入睡眠，遗留一些消息得不到处理，因
        // 此当前线程在睡眠前，必须先检查是否还有消息未处理，有的话就必须保证主线程不能
        // 睡，让主线程兜底，主线程会至少抢到特权一次，保证当前线程调用 prh_thrd_wakeup
        // 之后，主线程至少会最后一次重新检查未处理的消息。
        if ((req_que_not_empty = !prh_atom_data_quefix_empty(&cono_thrd->cono_req_que))) {
            prh_thrd_wakeup(&main_thrd->sleep_cond);
        }
        if (prh_atom_bool_read(&PRH_IMPL_CONO_STRUCT.thrd_exit)) {
            if (!prh_thrd_try_sleep(&cono_thrd->sleep_cond)) {
                continue;
            }
            if (req_que_not_empty) {
                continue; // 如果还有消息未处理，不能退出线程
            }
            break;
        }
#if PRH_CONO_DEBUG
        printf("[thrd %02d] sleep %d %d\n", thrd_id, atom_write_succ, atom_write_fail);
#endif
        prh_thrd_sleep(&cono_thrd->sleep_cond);
#if PRH_CONO_DEBUG
        printf("[thrd %02d] wakeup\n", thrd_id);
#endif
    }

    return 0;
}

void prh_cono_main(int thrd_start_id, int num_thread, prh_conoproc_t main_proc, int stack_size) {
    prh_cono_struct *s = prh_impl_cono_struct_init(thrd_start_id, num_thread, main_proc, stack_size);
    prh_thrd_struct *thrd_struct = s->thrd_struct;
    prh_cono_thrd *main_thrd = s->main_thread;
    prh_atom_ptr *thrd_ready_cono = &main_thrd->ready_cono;
    int i = 0, thrd_id = prh_cono_thrd_id(main_thrd);
    prh_real_cono *ready_cono;

    for (; i < num_thread; i += 1) { // 启动所有线程
        prh_cono_thrd *cono_thrd = prh_thrd_creatx(thrd_struct, prh_impl_cono_thrd_size);
        prh_impl_cono_thrd_init(cono_thrd, i + 1);
        prh_thrd_starx(thrd_struct, cono_thrd, prh_impl_cono_thrd_proc, 0);
    }

    // 启动入口协程（主线程的第一个协程），程序的执行从主线程执行第一个协程开始，入口协
    // 程并没有绑定在主线程上，只是通过主线程启动，后续执行也只是像普通协程一样，可能在
    // 任意线程中执行。
    prh_impl_cono_execute(thrd_id, s->entry_cono);

    for (; ;) {
        while ((ready_cono = prh_atom_ptr_read(thrd_ready_cono))) {
            if (prh_atom_ptr_compare_write(thrd_ready_cono, (void **)&ready_cono, prh_null)) {
                prh_impl_cono_execute(thrd_id, ready_cono);
            }
            prh_impl_privilege_task(main_thrd, false);
        }
        while (!prh_impl_privilege_task(main_thrd, true)) ; // 为了兜底，主线程必须抢到一次特权
        if (*prh_impl_thrd_pending_work(main_thrd)) {
            continue;
        }
        if (prh_atom_bool_read(&PRH_IMPL_CONO_STRUCT.thrd_exit)) {
            if (!prh_thrd_try_sleep(&main_thrd->sleep_cond)) {
                continue;
            }
            prh_thrd_join_except_main(thrd_struct, prh_impl_cono_thrd_free);
#if PRH_CONO_DEBUG
            printf("[thrd %02d] exit\n", thrd_id);
#endif
            break;
        }
#if PRH_CONO_DEBUG
        printf("[thrd %02d] sleep\n", thrd_id);
#endif
        prh_thrd_sleep(&main_thrd->sleep_cond);
#if PRH_CONO_DEBUG
        printf("[thrd %02d] wakeup\n", thrd_id);
#endif
    }

    prh_impl_cono_struct_free();
}

#ifdef PRH_TEST_IMPLEMENTATION
void prh_impl_cono_test(void) {

}
#endif // PRH_TEST_IMPLEMENTATION
#endif // PRH_CONO_IMPLEMENTATION
#endif // PRH_CONO_INCLUDE

#ifdef __cplusplus
}
#endif
#endif // PRH_IMPL_INCLUDE_H
/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2025 swdayu <github.com/swdayu>
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