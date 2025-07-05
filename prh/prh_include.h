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
    //standard keywords alignof and alignas for maximum code portability.
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

#ifndef PRH_ARRAY_LEN
    #define PRH_ARRAY_LEN(a) (sizeof(a)/sizeof((a)[0]))
#endif

#ifndef PRH_NOT_USE
    #define PRH_NOT_USE(a) ((void)(a))
#endif

#ifndef prh_macro_concat_name
#define prh_macro_concat_name(a, b) prh_impl_macro_concat_name(a, b)
#define prh_impl_macro_concat_name(a, b) a ## b
#endif

#ifndef prh_release_assert
    #define prh_release_assert(a) if (!(a)) prh_impl_assert(__LINE__)
    void prh_impl_assert(int line);
#endif

#ifndef prh_assert
#if PRH_DEBUG
    #define prh_assert(a) prh_release_assert(a)
#else
    #define prh_assert(a) ((void)0)
#endif
#endif

#ifndef prh_zeroret
#if PRH_DEBUG // macro arg 'a' can only expand once
    #define prh_zeroret(a) if ((a) != 0) { exit(__LINE__); }
    #define prh_boolret(a) if (!(a)) { exit(__LINE__); }
#else
    #define prh_zeroret(a) a
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

#ifdef RPH_STRING_INCLUDE
#define PRH_ARRAY_INCLUDE
#ifdef PRH_STRING_IMPLEMENTATION
#define PRH_ARRAY_IMPLEMENTATION
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

#ifdef PRH_CONC_INCLUDE
#define PRH_LIST_INCLUDE
#define PRH_STACK_INCLUDE
#define PRH_QUEUE_INCLUDE
#define PRH_THRD_INCLUDE
#define PRH_ATOMIC_INCLUDE
#define PRH_CORO_INCLUDE
#ifdef PRH_CONC_IMPLEMENTATION
    #define PRH_BASE_IMPLEMENTATION
    #define PRH_LIST_IMPLEMENTATION
    #define PRH_STACK_IMPLEMENTATION
    #define PRH_QUEUE_IMPLEMENTATION
    #define PRH_ATOMIC_IMPLEMENTATION
    #define PRH_THRD_IMPLEMENTATION
    #define PRH_CORO_IMPLEMENTATION
#endif
#endif

#ifdef PRH_THRD_INCLUDE
#define PRH_TIME_INCLUDE
#ifdef PRH_THRD_IMPLEMENTATION
    #define PRH_TIME_IMPLEMENTATION
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

#if defined(PRH_CONC_IMPLEMENTATION) || defined(PRH_THRD_IMPLEMENTATION) || \
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
#if defined(PRH_CONC_INCLUDE) && defined(PRH_CONC_IMPLEMENTATION)
void prh_impl_conc_test(void);
#endif
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
    printf("stdc version %d\n", __STDC_VERSION__);
#endif
#if defined(__cplusplus)
    printf("c++ version %d\n", __cplusplus);
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
#if defined(PRH_CONC_INCLUDE) && defined(PRH_CONC_IMPLEMENTATION)
    prh_impl_conc_test();
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
//   array_push_elts(&dyn_ints, elts, PRH_ARRAY_LEN(elts));
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

typedef struct { // 数组分配后容量固定，大小可动态调整
    prh_int capacity;
    prh_int count;
} prh_impl_arrfit_hdr; // the allocated memory is fixed in the place

typedef struct { // 动态容量动态大小的数组
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

typedef struct {
    prh_snode *next;
    void *data; // node only contain a data pointer
} prh_data_snode;

typedef struct { // zero initialized
    prh_snode *head;
    prh_snode *tail;
} prh_slist;

typedef struct { // zero initialized
    prh_snode *head;
    prh_snode *tail;
    prh_int size;
} prh_sized_slist;

typedef struct prh_nord { // node xored double linked (both = prev XOR next)
    struct prh_nord *both;
} prh_xnode;

typedef struct {
    prh_xnode *both;
    void *data; // node only contain a data pointer
} prh_data_xnode;

typedef struct { // zero initialized
    prh_xnode *head;
    prh_xnode *tail;
} prh_xlist;

typedef struct { // zero initialized
    prh_xnode *head;
    prh_xnode *tail;
    prh_int size;
} prh_sized_xlist;

typedef struct prh_node { // node double linked
    struct prh_node *next;
    struct prh_node *prev;
} prh_node;

typedef struct {
    prh_node *next;
    prh_node *prev;
    void *data; // node only contain a data pointer
} prh_data_node;

typedef struct { // zero initialized
    prh_node *head;
    prh_node *tail;
} prh_list;

typedef struct { // zero initialized
    prh_node *head;
    prh_node *tail;
    prh_int size;
} prh_sized_list;

typedef struct prh_tnode { // node tripple linked
    struct prh_tnode *next;
    struct prh_tnode *prev;
    struct prh_tnode *parent;
} prh_tnode;

typedef struct {
    prh_tnode *next;
    prh_tnode *prev;
    prh_tnode *parent;
    void *data; // node only contain a data pointer
} prh_data_tnode;

typedef struct { // zero initialized
    prh_tnode *root;
} prh_tree;

typedef struct { // zero initialized
    prh_tnode *root;
    prh_int size;
} prh_sized_tree;

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

typedef struct { // zero initialize
    prh_snode *head;
    prh_snode *tail;
    prh_int size;
} prh_sized_quefit;

typedef struct { // zero initialize
    prh_data_snode *head;
    prh_data_snode *tail;
    prh_int size;
} prh_sized_data_quefit;

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

#define prh_quefit_for_each_begin(quefit) {                                     \
        prh_snode *it = (quefit)->head;                                         \
        prh_snode *prh_impl_next;                                               \
        for (; it; it = prh_impl_next) {                                        \
            prh_impl_next = it->next;                                           \
#define prh_data_quefit_for_end()                                               \
        }                                                                       \
    }

#define prh_data_quefit_for_begin(data_quefit) {                                \
        prh_data_snode *it = (data_quefit)->head;                               \
        prh_data_snode *prh_impl_next;                                          \
        for (; it; it = prh_impl_next) {                                        \
            prh_impl_next = it->next;                                           \
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
    if (tail == prh_null) tail = new_node;
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
// struct user_relaxed_quefit {
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
void prh_impl_relaxed_quefit_pop(void **quefit, prh_i32 next_offset);

#define prh_relaxed_quefit_init(relaxed_quefit_ptr) {                           \
    prh_typeof(relaxed_quefit_ptr) prh_impl_que_ptr = (relaxed_quefit_ptr);     \
    relaxed_quefit_ptr->head = relaxed_quefit_ptr->tail = prh_null;             \
}

#define prh_relaxed_quefit_push(relaxed_quefit_ptr, node_type_ptr, next_field_name) { \
    prh_typeof(relaxed_quefit_ptr) prh_impl_que_ptr = (relaxed_quefit_ptr);     \
    typedef prh_typeof(*(prh_impl_que_ptr->tail)) prh_impl_node_type;           \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    prh_impl_node_type *prh_impl_node_ptr = (node_type_ptr);                    \
    prh_impl_relaxed_quefit_push((void **)prh_impl_que_ptr, prh_impl_node_ptr, (prh_i32)offsetof(prh_impl_node_type, next_field_name)); \
}

#define prh_relaxed_quefit_push_queue(relaxed_quefit_ptr, from_quefit_ptr, next_field_name) { \
    typedef prh_typeof(relaxed_quefit_ptr) prh_impl_quefit_ptr_type;            \
    prh_impl_quefit_ptr_type prh_impl_que_ptr = (relaxed_quefit_ptr);           \
    prh_impl_quefit_ptr_type prh_impl_from_que = (from_quefit_ptr);             \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    typedef prh_typeof(*(prh_impl_que_ptr->tail)) prh_impl_node_type;           \
    prh_impl_relaxed_quefit_push_queue((void **)prh_impl_que_ptr, (void **)prh_impl_from_que, (prh_i32)offsetof(prh_impl_node_type, next_field_name)); \
}

#define prh_relaxed_quefit_push_front(relaxed_quefit_ptr, node_type_ptr, next_field_name) { \
    prh_typeof(relaxed_quefit_ptr) prh_impl_que_ptr = (relaxed_quefit_ptr);     \
    typedef prh_typeof(*(prh_impl_que_ptr->head)) prh_impl_node_type;           \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    prh_impl_node_type *prh_impl_node_ptr = (node_type_ptr);                    \
    prh_impl_relaxed_quefit_push_front((void **)prh_impl_que_ptr, prh_impl_node_ptr, (prh_i32)offsetof(prh_impl_node_type, next_field_name)); \
}

#define prh_relaxed_quefit_push_queue_front(relaxed_quefit_ptr, from_quefit_ptr, next_field_name) { \
    typedef prh_typeof(relaxed_quefit_ptr) prh_impl_quefit_ptr_type;            \
    prh_impl_quefit_ptr_type prh_impl_que_ptr = (relaxed_quefit_ptr);           \
    prh_impl_quefit_ptr_type prh_impl_from_que = (from_quefit_ptr);             \
    assert((void **)prh_impl_que_ptr == (void **)&prh_impl_que_ptr->head);      \
    assert((void **)prh_impl_que_ptr + 1 == (void **)&prh_impl_que_ptr->tail);  \
    typedef prh_typeof(*(prh_impl_que_ptr->head)) prh_impl_node_type;           \
    prh_impl_relaxed_quefit_push_queue_front((void **)prh_impl_que_ptr, (void **)prh_impl_from_que, (prh_i32)offsetof(prh_impl_node_type, next_field_name)); \
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
    node_out = (prh_impl_node_type *)prh_impl_relaxed_quefit_pop((void **)prh_impl_que_ptr, (prh_i32)offsetof(prh_impl_node_type, next_field_name)); \
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

typedef struct { // zero initialize
    prh_data_snode *head;
    prh_data_snode *tail;
    prh_int size;
} prh_sized_quefix;

// Dynamic allocated variable size queue. The node can contain any object and
// with any different size. If the size < sizeof(void *), the size is rounded
// up to sizeof(void *).
typedef struct { // zero initialize
    prh_snode *head;
    prh_snode *tail;
} prh_quedyn;

typedef struct { // zero initialize
    prh_snode *head;
    prh_snode *tail;
    prh_int size;
} prh_sized_quedyn;

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

#ifdef PRH_CORO_INCLUDE
typedef struct prh_impl_coro prh_coro;
typedef struct prh_impl_coro_struct prh_coro_struct;

typedef struct {
    prh_i32 start_id, real_cnt; // 1st field dont move
    prh_coro *coro; // last field dont move, only used by soro_struct
} prh_soro_struct;

#define prh_coro_proc prh_fastcall(void)
#define prh_impl_coro_yield(coro) prh_impl_asm_coro_yield((coro), prh_impl_next_resume_coro())
prh_fastcall_typedef(void, prh_coroproc_t)(prh_coro *);

prh_fastcall(void) prh_impl_asm_coro_yield(prh_coro *coro, prh_coro *next);
prh_coro *prh_impl_next_resume_coro(void);
void prh_coro_yield(prh_coro *coro);

prh_inline void *prh_coro_data(prh_coro *coro) {
    return *(void **)((prh_byte *)coro + 8);
}

prh_inline int prh_coro_main_id(prh_coro_struct *s) {
    return ((prh_soro_struct *)s)->start_id;
}

prh_coro_struct *prh_coro_init(int start_id, int maxcoros);
void prh_coro_create(prh_coro_struct *s, prh_coroproc_t proc, int stack_size, void *userdata);
void *prh_coro_creatx(prh_coro_struct *s, prh_coroproc_t proc, int stack_size, int maxudsize);

bool prh_coro_cycle_start(prh_coro_struct *s);
bool prh_coro_start(prh_coro_struct *s, int index);
void prh_coro_reload(prh_coro_struct *s, int index, prh_coroproc_t proc); // udata is not reset
void prh_coro_finish(prh_coro_struct **s);
int prh_coro_self_id(void);

void prh_soro_init(prh_soro_struct *s, int start_id);
void prh_soro_create(prh_soro_struct *s, prh_coroproc_t proc, int stack_size, void *userdata);
void *prh_soro_creatx(prh_soro_struct *s, prh_coroproc_t proc, int stack_size, int maxudsize);
bool prh_soro_start(prh_soro_struct *s);
void prh_soro_reload(prh_soro_struct *s, prh_coroproc_t proc); // udata is not reset
void prh_soro_finish(prh_soro_struct *s);

#ifdef PRH_CORO_STRIP_PREFIX
#define coro_t                  prh_coro
#define coro_struct_t           prh_coro_struct
#define soro_struct_t           prh_soro_struct
#define coro_proc_func          prh_coro_proc
#define coro_data               prh_coro_data
#define coro_self_id            prh_coro_self_id
#define coro_main_id            prh_coro_main_id
#define coro_yield              prh_coro_yield
#define coro_init               prh_coro_init
#define coro_create             prh_coro_create
#define coro_creatx             prh_coro_creatx
#define coro_cycle_await        prh_coro_cycle_start
#define coro_await              prh_coro_start
#define coro_reload             prh_coro_reload
#define coro_finish             prh_coro_finish
#define soro_init               prh_soro_init
#define soro_create             prh_soro_create
#define soro_creatx             prh_soro_creatx
#define soro_await              prh_soro_start
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
prh_fastcall(void) prh_impl_asm_coro_call(void); // args are on coro stack

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
    prh_u32 loweraddr: 31, cross_thread: 1;
    void *userdata;
};

// prh_coro_struct layout:
//  [prh_coro_struct]
//  [prh_coro *]
//  [prh_coro *]
struct prh_impl_coro_struct {
    prh_i32 start_id, real_cnt; // 1st field dont move
    prh_i32 maxcoros, coro_cnt; // last field dont move, only used by coro_struct
};

typedef struct prh_impl_coro_main {
    prh_i32 rspoffset; // 1st field dont move
    prh_i32 curr_index; // coro index start from 1
    prh_i32 end_count;
    prh_u32 start_id: 31, yield_cycle: 1;
    prh_coro **coro_list;
    struct prh_impl_coro_main *next;
} prh_impl_coro_main;

prh_inline int prh_impl_coro_struct_alloc_size(int maxcoros) {
    assert(maxcoros >= 1);
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
prh_static_assert(sizeof(struct prh_coro) <= prh_impl_coro_size);

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

void prh_impl_coro_load(prh_coro *coro, prh_coroproc_t proc) {
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
    *(--rsp) = (void *)(prh_unt)proc;
    *(--rsp) = (void *)(prh_unt)prh_impl_asm_coro_call;
    coro->rspoffset = (prh_u32)((char *)coro - (char *)rsp);
}

prh_coro *prh_impl_coro_create(prh_coroproc_t proc, int stack_size, int coro_extend_size, void *userdata) {
    prh_coro *coro = prh_impl_coro_alloc(stack_size, coro_extend_size, 0);
    prh_impl_coro_load(coro, proc);
    coro->userdata = userdata;
    return coro;
}

prh_coro *prh_impl_coro_creatx(prh_coroproc_t proc, int stack_size, int coro_extend_size, int maxudsize) {
    prh_coro *coro = prh_impl_coro_alloc(stack_size, coro_extend_size, maxudsize);
    prh_impl_coro_load(coro, proc);
    return coro;
}

void prh_impl_coro_reload(prh_coro *coro, prh_coroproc_t proc) {
    assert(coro && coro->rspoffset == prh_null); // only finished coro can reload
    prh_impl_coro_load(coro, proc);
}

prh_coro **prh_impl_coro_list(prh_coro_struct *s) {
    return (prh_coro **)(s + 1);
}

prh_coro **prh_impl_coro_find_free_slot(prh_coro_struct *s) {
    assert(s->coro_cnt >= 0 && s->coro_cnt < s->maxcoros);
    prh_coro **slot, coro_list = prh_impl_coro_list(s);
    for (int i = s->coro_cnt; i < s->maxcoros; i += 1) {
        if ((slot = coro_list[i]) == prh_null) {
            s->coro_cnt = i + 1;
            s->real_cnt += 1;
            return slot;
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
    prh_impl_coro_reload(coro, proc);
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

int prh_coro_self_id(void) {
    struct prh_impl_coro_main *m = PRH_IMPL_CORO_MAIN;
    return m->start_id + m->curr_index;
}

void prh_impl_coro_main_yield(prh_impl_coro_main *m, prh_coro *next) {
#if PRH_CORO_DEBUG
    printf("[coro %02d] %p await %02d\n", m->start_id, (void *)m, m->start_id + m->curr_index);
#endif
    m->next = PRH_IMPL_CORO_MAIN;
    PRH_IMPL_CORO_MAIN = m;
    prh_impl_asm_coro_yield((prh_coro *)m, next);
    PRH_IMPL_CORO_MAIN = m->next;
}

prh_coro *prh_impl_get_next_resume(prh_impl_coro_main *m) {
    for (int i = m->curr_index; i < m->end_count; i += 1) { // soro_struct never goes here due to yield_cycle = 0
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

#if defined(PRH_CONC_IMPLEMENTATION)
void prh_impl_cross_thread_coro_yield(prh_coro *coro);
#endif

void prh_coro_yield(prh_coro *coro) {
#if defined(PRH_CONC_IMPLEMENTATION)
    if (coro->cross_thread) {
        prh_impl_cross_thread_coro_yield(coro);
    }
#endif
    prh_impl_coro_yield(coro);
}

prh_fastcall(void *) prh_impl_asm_coro_finish(prh_coro *coro) {
    coro->rspoffset = prh_null;
#if defined(PRH_CONC_IMPLEMENTATION)
    if (coro->cross_thread) {
        prh_impl_cross_thread_coro_yield(coro);
    }
#endif
    prh_impl_coro_guard_verify(coro);
    return prh_impl_next_resume_coro();
}

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
    if (next == prh_null || next->rspoffset == prh_null) {
        return false;
    }
    mainstack.curr_index = index;
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
    s->coro = prh_null;
    s->start_id = start_id;
}

void prh_soro_create(prh_soro_struct *s, prh_coroproc_t proc, int stack_size, void *userdata) {
    assert(s->coro == prh_null);
    s->coro = prh_impl_coro_create(proc, stack_size, 0, userdata);
#if PRH_CORO_DEBUG
    prh_coro *coro = s->coro;
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p create -- lower %p (left %d) rsp %p coro %p (size %d) data %p (size %d) -- stack %d\n",
        s->start_id + 1, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata, 0, stack_size);
#endif
}

void *prh_soro_creatx(prh_soro_struct *s, prh_coroproc_t proc, int stack_size, int maxudsize) {
    assert(s->coro == prh_null);
    prh_coro *coro = prh_impl_coro_creatx(proc, stack_size, 0, maxudsize);
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

void prh_soro_reload(prh_soro_struct *s, prh_coroproc_t proc) {
    prh_impl_coro_reload(s->coro, proc);
#if PRH_CORO_DEBUG
    prh_coro *coro = s->coro;
    char *rsp = (char *)coro - prh_impl_asm_stack_init_depth();
    struct prh_impl_coro_guard *guard = prh_impl_coro_guard(coro);
    printf("[coro %02d] %p reload -- lower %p (left %d) rsp %p coro %p (size %d) data %p --\n",
        s->start_id + 1, (void *)coro, (void *)guard, (int)(rsp - (char *)(guard + 1)), (void *)rsp,
        (void *)coro, prh_impl_coro_extend_size(0), coro->userdata);
#endif
}

bool prh_soro_start(prh_soro_struct *s) {
    prh_impl_coro_main mainstack;
    mainstack.coro_list = prh_null;
    mainstack.start_id = s->start_id;
    mainstack.yield_cycle = 0;
    prh_coro *next = s->coro;
    if (next == prh_null || next->rspoffset == prh_null) {
        return false;
    }
    mainstack.curr_index = 1;
    prh_impl_coro_main_yield(&mainstack, next);
    return true;
}

void prh_soro_finish(prh_soro_struct *main) {
    prh_coro *soro = main->coro;
    if (soro == prh_null) return;
    prh_impl_coro_free(soro);
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
prh_inline void prh_atom_ptr_init(prh_atom_ptr *a, void *b) { atomic_init((atomic_uintptr_t *)a, (uintptr_t)b); }

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
prh_inline void prh_atom_ptr_write(prh_atom_ptr *a, void *b) { atomic_store((atomic_uintptr_t *)a, (uintptr_t)b); }

prh_inline bool prh_atom_bool_fetch_write(prh_atom_bool *a, bool b) { return atomic_exchange((atomic_bool *)a, b); } // return old value
prh_inline prh_i32 prh_atom_i32_fetch_write(prh_atom_i32 *a, prh_i32 b) { return atomic_exchange((atomic_int *)a, b); }
prh_inline prh_u32 prh_atom_u32_fetch_write(prh_atom_u32 *a, prh_u32 b) { return atomic_exchange((atomic_uint *)a, b); }
prh_inline prh_int prh_atom_int_fetch_write(prh_atom_int *a, prh_int b) { return atomic_exchange((atomic_intptr_t *)a, b); }
prh_inline prh_unt prh_atom_unt_fetch_write(prh_atom_unt *a, prh_unt b) { return atomic_exchange((atomic_uintptr_t *)a, b); }
prh_inline void *prh_atom_ptr_fetch_write(prh_atom_ptr *a, void *b) { return (void *)atomic_exchange((atomic_uintptr_t *)a, (uintptr_t)b); }

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
prh_inline bool prh_atom_ptr_compare_write(prh_atom_ptr *a, void **expect, void *b) { return atomic_compare_exchange_weak((atomic_uintptr_t *)a, (uintptr_t *)expect, (uintptr_t)b); }

prh_inline bool prh_atom_bool_strong_compare_write(prh_atom_bool *a, bool *expect, bool b) { return atomic_compare_exchange_strong((atomic_bool *)a, expect, b); }
prh_inline bool prh_atom_i32_strong_compare_write(prh_atom_i32 *a, prh_i32 *expect, prh_i32 b) { return atomic_compare_exchange_strong((atomic_int *)a, expect, b); }
prh_inline bool prh_atom_u32_strong_compare_write(prh_atom_u32 *a, prh_u32 *expect, prh_u32 b) { return atomic_compare_exchange_strong((atomic_uint *)a, expect, b); }
prh_inline bool prh_atom_int_strong_compare_write(prh_atom_int *a, prh_int *expect, prh_int b) { return atomic_compare_exchange_strong((atomic_intptr_t *)a, expect, b); }
prh_inline bool prh_atom_unt_strong_compare_write(prh_atom_unt *a, prh_unt *expect, prh_unt b) { return atomic_compare_exchange_strong((atomic_uintptr_t *)a, expect, b); }
prh_inline bool prh_atom_ptr_strong_compare_write(prh_atom_ptr *a, void **expect, void *b) { return atomic_compare_exchange_strong((atomic_uintptr_t *)a, (uintptr_t *)expect, (uintptr_t)b); }

prh_inline bool prh_atom_bool_compare_write_false(prh_atom_bool *a) { bool expect = true; return atomic_compare_exchange_weak((atomic_bool *)a, &expect, false); }
prh_inline bool prh_atom_bool_compare_write_true(prh_atom_bool *a) { bool expect = false; return atomic_compare_exchange_weak((atomic_bool *)a, &expect, true); }
prh_inline bool prh_atom_bool_strong_compare_write_false(prh_atom_bool *a) { bool expect = true; return atomic_compare_exchange_strong((atomic_bool *)a, &expect, false); }
prh_inline bool prh_atom_bool_strong_compare_write_true(prh_atom_bool *a) { bool expect = false; return atomic_compare_exchange_strong((atomic_bool *)a, &expect, true); }

prh_inline bool prh_atom_i32_compare_zero_write(prh_atom_i32 *a, prh_i32 b) { prh_i32 expect = 0; return atomic_compare_exchange_weak((atomic_int *)a, &expect, b); }
prh_inline bool prh_atom_u32_compare_zero_write(prh_atom_u32 *a, prh_u32 b) { prh_u32 expect = 0; return atomic_compare_exchange_weak((atomic_uint *)a, &expect, b); }
prh_inline bool prh_atom_int_compare_zero_write(prh_atom_int *a, prh_int b) { prh_int expect = 0; return atomic_compare_exchange_weak((atomic_intptr_t *)a, &expect, b); }
prh_inline bool prh_atom_unt_compare_zero_write(prh_atom_unt *a, prh_unt b) { prh_unt expect = 0; return atomic_compare_exchange_weak((atomic_uintptr_t *)a, &expect, b); }
prh_inline bool prh_atom_ptr_compare_null_write(prh_atom_ptr *a, void *b) { uintptr_t expect = 0; return atomic_compare_exchange_weak((atomic_uintptr_t *)a, &expect, (uintptr_t)b); }

prh_inline bool prh_atom_i32_strong_compare_zero_write(prh_atom_i32 *a, prh_i32 b) { prh_i32 expect = 0; return atomic_compare_exchange_strong((atomic_int *)a, &expect, b); }
prh_inline bool prh_atom_u32_strong_compare_zero_write(prh_atom_u32 *a, prh_u32 b) { prh_u32 expect = 0; return atomic_compare_exchange_strong((atomic_uint *)a, &expect, b); }
prh_inline bool prh_atom_int_strong_compare_zero_write(prh_atom_int *a, prh_int b) { prh_int expect = 0; return atomic_compare_exchange_strong((atomic_intptr_t *)a, &expect, b); }
prh_inline bool prh_atom_unt_strong_compare_zero_write(prh_atom_unt *a, prh_unt b) { prh_unt expect = 0; return atomic_compare_exchange_strong((atomic_uintptr_t *)a, &expect, b); }
prh_inline bool prh_atom_ptr_strong_compare_null_write(prh_atom_ptr *a, void *b) { uintptr_t expect = 0; return atomic_compare_exchange_strong((atomic_uintptr_t *)a, &expect, (uintptr_t)b); }

// Atomic singly linked queue for only 1 producer and 1 consumer. Each node has
// a prh_snode header and a data pointer. The node has fixed size and can only
// contain a data pointer. The fixed size node is dynamic alloced by the queue.
typedef struct {
    prh_data_snode *head; // 只由单一消费者读写, only modified by pop thread
    prh_data_snode *tail; // 只由单一生产者读写, only modified by push thread
    prh_snode free; // 仅由生产者读写，only modified by push thread
    prh_atom_ptr last; // 生产者无条件写，消费者负责读和有条件写
} prh_atom_data_quefix;

void prh_atom_data_quefix_init(prh_atom_data_quefix *q);
void prh_atom_data_quefix_push(prh_atom_data_quefix *s, void *data); // data shall not be null
void *prh_atom_data_quefix_top(prh_atom_data_quefix *s); // return null means empty
void *prh_atom_data_quefix_pop(prh_atom_data_quefix *s); // return null means empty
bool prh_atom_data_quefix_pop_all(prh_atom_data_quefix *q, prh_data_quefit *out);
void prh_atom_data_quefix_free(prh_atom_data_quefix *q, prh_data_quefit *q);

#ifdef PRH_ATOMIC_IMPLEMENTATION
#define PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE PRH_CACHE_LINE_SIZE // block size shall be power of 2
#define PRH_IMPL_ATOM_DYNQUE_NODE_COUNT (PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE / sizeof(prh_data_snode))
prh_static_assert(PRH_IMPL_ATOM_DYNQUE_NODE_COUNT >= 2); // 如果不能分配2个或2个以上，间接块分配没有意义
prh_static_assert(PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE % sizeof(prh_data_snode) == 0);

void prh_impl_atom_data_quefix_more_free(prh_atom_data_quefix *q, prh_data_snode *node) {
    prh_data_snode *last = node + PRH_IMPL_ATOM_DYNQUE_NODE_COUNT - 1;
    prh_data_snode *first = (void *)q->free.next;
    q->free.next = (void *)node; // add node list to free head
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
    if ((prh_unt)(node + 1) & (prh_unt)(PRH_IMPL_ATOM_DYNQUE_BLOCK_SIZE - 1) == 0) {
        prh_aligned_free(node + 1 - PRH_IMPL_ATOM_DYNQUE_NODE_COUNT);
    }
}

void prh_atom_data_quefix_init(prh_atom_data_quefix *q) {
    q->free.next = prh_null;
    q->head = q->tail = prh_impl_atom_data_quefix_aligned_alloc(q); // 总是分配一个tail空节点，让非空head永远追不上tail
    prh_atom_ptr_init(&q->last, prh_null);
    return q;
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
        prh_atom_ptr_strong_compare_write(&q->last, &last, prh_null);
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
    prh_atom_ptr_strong_compare_write(&q->last, &last, prh_null);
    assert(q->head == last->next); // 只允许唯一消费者
    last->next = prh_null; // quefit最后一个节点指向空，方便遍历和移动
    return true;
}

void prh_atom_data_quefix_free_node(prh_atom_data_quefix *q, prh_data_quefit *q) {
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
    b->curr = (void **)(b + 1);
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
    prh_hive_quefix_block *tail = q->tail;
    *tail->curr++ = data;
    if (tail->curr >= prh_impl_atom_hive_quefix_block_end(tail)) {
        q->tail = tail->next = prh_impl_atom_hive_quefix_alloc_block();
    }
    prh_atom_int_inc(&q->len); // 此步骤执行完毕以上更新必须对所有cpu生效
}

void *prh_atom_hive_quefix_top(prh_atom_hive_quefix *q) {
    if (!prh_atom_int_read(&q->len)) return prh_null;
    return *(q->head->curr);
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

// Dynamic allocated link list atomic queue for only 1 producer and 1 consumer.
// The queue node size is fixed, and must cotain prh_snode as the header.
//
//  struct QueueNode {
//      prh_snode node; // must be 1st field
//      ... queue node other custom data ...
//  };
//
//  struct StructContainQueue {
//      ...
//      QueueNode *<qname>_atomnodque_head; // only R/W by consumer
//      ...
//      QueueNode *<qname>_atomnodque_tail; // only R/W by producer
//      ...
//      prh_atomint_t <qname>_atomnodque_len; // R/W by both
//      ...
//  };
//
//  StructContainQueue s;
//  prh_atomnodque_init(&s, qname);
//  QueueNode *node = prh_atomnodque_node(&s, qname);
//  ... initializing the new node ...
//  prh_atomnodque_push(&s, qname); // push a new node
//  QueueNode *top = prh_atomnodque_top(&s, qname);
//  ... using the top node ...
//  prh_atomnodque_pop(&s, qname); // pop off and free top node
//  prh_atomnodque_free(&s, qname, node_deinit_func); // free all nodes
#define prh_impl_atomnodque_head(s, q) ((s)->prh_macro_concat_name(q, _atomnodque_head))
#define prh_impl_atomnodque_tail(s, q) ((s)->prh_macro_concat_name(q, _atomnodque_tail))
#define prh_impl_atomnodque_len(s, q) ((s)->prh_macro_concat_name(q, _atomnodque_len))
void prh_impl_atomnodque_free(prh_snode *head, void (*node_deinit_func)(void *node));
void prh_impl_atomnodque_push(void **tail_addr, prh_int node_size, prh_atomint_t *len);
void prh_impl_atomnodque_copy_push(void **tail_addr, prh_int node_size, prh_atomint_t *len, prh_snode *node);
prh_snode *prh_impl_atomnodque_pop(void **head_addr, prh_atomint_t *len);

prh_inline void prh_atomnodque_free_node(prh_snode *node) {
    if (node) prh_free(node);
}

#define prh_atomnodque_init(s, q) {                                             \
    typedef prh_typeof(prh_impl_atomnodque_head((s), q)) prh_impl_node_ptr_t;   \
    prh_impl_node_ptr_t prh_impl_new_node = prh_malloc(sizeof(*prh_impl_atomnodque_head((s), q))); \
    prh_impl_atomnodque_head((s), q) = prh_impl_new_node; /* always alloc a null tail node */ \
    prh_impl_atomnodque_tail((s), q) = prh_impl_new_node; /* let head pointer always valid */ \
    ((prh_snode *)prh_impl_new_node)->next = prh_null;                          \
    prh_atomint_t *prh_impl_len = &prh_impl_atomnodque_len((s), q);             \
    prh_atomtype_init(prh_impl_len, 0);                                         \
}

#define prh_atomnodque_free(s, q, node_deinit_func) {                           \
    typedef prh_typeof(prh_impl_atomnodque_head((s), q)) prh_impl_node_ptr_t;   \
    prh_impl_node_ptr_t prh_impl_head = prh_impl_atomnodque_head((s), q);       \
    if (prh_impl_head) {                                                        \
        prh_impl_atomnodque_free((prh_snode *)prh_impl_head, (node_deinit_func)); \
        prh_impl_atomnodque_head((s), q) = prh_null;                            \
        prh_impl_atomnodque_tail((s), q) = prh_null;                            \
    }                                                                           \
}

#define prh_atomnodque_tail(s, q) prh_impl_atomnodque_tail((s), q)
#define prh_atomnodque_top(s, q) ((prh_atomtype_load(&prh_impl_atomnodque_len((s), q)) <= 0) ? prh_null : prh_impl_atomnodque_head((s), q))

// Push a new tail null node and inc the counter
#define prh_atomnodque_push(s, q) prh_impl_atomnodque_push(&prh_impl_atomnodque_tail((s), q), sizeof(*prh_impl_atomnodque_head((s), q)), &prh_impl_atomnodque_len((s), q))

// Copy node content to tail null node and push the node as the new tail null node
#define prh_atomnodque_copy_push(s, q, node) prh_impl_atomnodque_copy_push(&prh_impl_atomnodque_tail((s), q), sizeof(*prh_impl_atomnodque_head((s), q)), &prh_impl_atomnodque_len((s), q), (node))

// Pop off top node and dec the counter, dont free and return the node
#define prh_atomnodque_pop(s, q) prh_impl_atomnodque_pop(&prh_impl_atomnodque_head((s), q), &prh_impl_atomnodque_len((s), q))

#ifdef PRH_ATOMIC_STRIP_PREFIX
#define atomnodque_t                prh_atomnodque_t
#define atomnodque_init             prh_atomnodque_init
#define atomnodque_free             prh_atomnodque_free
#define atomnodque_free_node        prh_atomnodque_free_node
#define atomnodque_tail             prh_atomnodque_tail
#define atomnodque_push             prh_atomnodque_push
#define atomnodque_copy_push        prh_atomnodque_copy_push
#define atomnodque_pop              prh_atomnodque_pop
#define atomnodque_top              prh_atomnodque_top
#define atomnodque_len              prh_atomnodque_len
#endif

#ifdef PRH_ATOMIC_IMPLEMENTATION
void prh_impl_atomnodque_free(prh_snode *head, void (*node_deinit_func)(void *item)) {
    while (head != prh_null) {
        prh_snode *p = head;
        head = head->next;
        if (node_deinit_func) {
            node_deinit_func(p);
        }
        prh_free(p);
    }
}

// push 不会读写 head，也不会读写已经存在的非空节点，tail 总是指向尾部空节点，有效的
// head 总是追不上 tail。push 只会更新 tail 和 tail 空节点，且 push 只被单一生产者
// 调用，因此 tail 不需要原子操作。
void prh_impl_atomnodque_push(void **tail_addr, prh_int node_size, prh_atomint_t *len) {
    prh_snode *new_node = prh_malloc(node_size);
    prh_snode *tail = *tail_addr;
    *tail_addr = tail->next = new_node;
    new_node->next = prh_null;
    prh_atomint_inc(len); /* 更新len，此步骤执行完毕后以上更新必须对所有cpu生效 */
    assert(*tail_addr == new_node); // allow only one producer
}

void prh_impl_atomnodque_copy_push(void **tail_addr, prh_int node_size, prh_atomint_t *len, prh_snode *new_node) {
    prh_snode *tail = *tail_addr;
    *tail_addr = tail->next = new_node;
    memcpy(tail + 1, new_node + 1, node_size - sizeof(prh_snode));
    new_node->next = prh_null;
    prh_atomint_inc(len); /* 更新len，此步骤执行完毕后以上更新必须对所有cpu生效 */
    assert(*tail_addr == new_node); // allow only one producer
}

// pop 不会读写 tail，也不会读写 tail 空节点，pop 只会更新 head 和读写已经存在的并且
// 有效的头节点，且 pop 只被单一消费者调用，因此 head 不需要原子操作。
prh_snode *prh_impl_atomnodque_pop(void **head_addr, prh_atomint_t *len) {
    if (prh_atomtype_load(len) <= 0) return prh_null;
    prh_snode *head = *head_addr;
    prh_snode *head_next = head->next;
    *head_addr = head_next;
    prh_atomint_dec(len); // 更新len，此步骤执行完毕以上更新必须对所有cpu生效
    assert(head_next == *head_addr); // allow only one consumer
    return head;
}
#endif // PRH_ATOMIC_IMPLEMENTATION

// Dynamic allocated link list atomic queue for only 1 producer and 1 consumer.
// Each node has a prh_snode header and a node item pointer. Each node can
// only contain a pointer.
typedef struct prh_atompszque prh_atompszque_t;
prh_atompszque_t *prh_atompszque_init(void);
void prh_atompszque_free(prh_atompszque_t **q, void (*free_item)(void *item));
void prh_atompszque_push(prh_atompszque_t *q, void *item); // item shall not be null
void *prh_atompszque_pop(prh_atompszque_t *q); // return null means empty
void *prh_atompszque_top(prh_atompszque_t *q);
int prh_atompszque_len(prh_atompszque_t *q);

#ifdef PRH_ATOMIC_STRIP_PREFIX
#define atompszque_t                prh_atompszque_t
#define atompszque_init             prh_atompszque_init
#define atompszque_free             prh_atompszque_free
#define atompszque_push             prh_atompszque_push
#define atompszque_pop              prh_atompszque_pop
#define atompszque_top              prh_atompszque_top
#define atompszque_len              prh_atompszque_len
#endif

#ifdef PRH_ATOMIC_IMPLEMENTATION
typedef struct prh_impl_atompszque_node {
    struct prh_impl_atompszque_node *next;
    void *item;
} prh_impl_atompszque_node_t;

struct prh_atompszque {
    prh_impl_atompszque_node_t *head; // 只由单一消费者读写
    prh_impl_atompszque_node_t *tail; // 只由单一生成者读写
    prh_atomint_t count;
};

prh_atompszque_t *prh_atompszque_init(void) {
    // 永远分配一个tail空节点，让head永远触碰不到tail节点
    prh_impl_atompszque_node_t *tail_null = prh_calloc(sizeof(prh_impl_atompszque_node_t));
    prh_atompszque_t *q = prh_malloc(sizeof(prh_atompszque_t));
    q->head = tail_null;
    q->tail = tail_null;
    prh_atomtype_init(&q->count, 0);
    return q;
}

void prh_atompszque_free(prh_atompszque_t **que, void (*free_item)(void *item)) {
    prh_atompszque_t *q = *que;
    if (q == prh_null) return;
    prh_impl_atompszque_node_t *node = q->head;
    while (node != prh_null) {
        prh_impl_atompszque_node_t *p = node;
        node = node->next;
        if (free_item && p->item) {
            free_item(p->item);
        }
        prh_free(p);
    }
    prh_free(q);
    *que = prh_null;
}

void prh_atompszque_push(prh_atompszque_t *q, void *item) {
    // push不会读写head，也不会读写已经存在的节点
    if (item == prh_null) return; // item shall not be null
    // 分配新NULL节点，tail总指向尾部空节点，不让head追上tail
    prh_impl_atompszque_node_t *null_node = prh_calloc(sizeof(prh_impl_atompszque_node_t));
    prh_impl_atompszque_node_t *tail = q->tail;
    q->tail = null_node;
    // push只会更新tail和tail空节点，且push对应单一生产者，因此tail不需要atom
    tail->next = null_node;
    tail->item = item;
    // 只允许唯一生产者
    assert(tail->next == q->tail);
    // 更新count，此步骤执行完毕以上更新必须对所有cpu生效
    prh_atomint_inc(&q->count);
}

void *prh_atompszque_pop(prh_atompszque_t *q) {
    // pop不会读写tail，也不会读写tail空节点
    if (prh_atomtype_load(&q->count) <= 0) return prh_null;
    prh_impl_atompszque_node_t *head = q->head; // 保存head
    // pop只会更新head和读写已存在的头节点，且pop对应单一消费者，因此head不需要atom
    q->head = head->next;
    // 只允许唯一消费者
    assert(head->next == q->head);
    // 释放head节点
    void *item = head->item;
    prh_free(head);
    // 更新count，此步骤执行完毕以上更新必须对所有cpu生效
    prh_atomint_dec(&q->count);
    return item;
}

void *prh_atompszque_top(prh_atompszque_t *q) {
    if (prh_atomtype_load(&q->count) <= 0) return prh_null;
    return q->head->item;
}

int prh_atompszque_len(prh_atompszque_t *q) {
    return prh_atomtype_load(&q->count);
}
#endif // PRH_ATOMIC_IMPLEMENTATION

// Fixed unt array atomic queue for only 1 producer and 1 consumer
prh_atom_unt_arrque *prh_atom_unt_arrque_init(int size); // size must be power of 2
void prh_atom_unt_arrque_free(prh_atom_unt_arrque **p);
bool prh_atom_unt_arrque_push(prh_atom_unt_arrque *q, prh_unt v); // v shall not be null
prh_unt prh_atom_unt_arrque_pop(prh_atom_unt_arrque *q); // return null means empty
prh_unt prh_atom_unt_arrque_top(prh_atom_unt_arrque *q);
int prh_atom_unt_arrque_len(prh_atom_unt_arrque *q);
int prh_atom_unt_arrque_cap(prh_atom_unt_arrque *q);

typedef prh_atom_unt_arrque prh_atom_int_arrque;
prh_atom_int_arrque *prh_atom_int_arrque_init(int size); // size must be power of 2
void prh_atom_int_arrque_free(prh_atom_int_arrque **p);
bool prh_atom_int_arrque_push(prh_atom_int_arrque *q, prh_int v); // v shall not be null
prh_int prh_atom_int_arrque_pop(prh_atom_int_arrque *q); // return null means empty
prh_int prh_atom_int_arrque_top(prh_atom_int_arrque *q);
int prh_atom_int_arrque_len(prh_atom_int_arrque *q);
int prh_atom_int_arrque_cap(prh_atom_int_arrque *q);

// Fixed u32 array atomic queue for only 1 producer and 1 consumer
typedef struct prh_atom_u32_arrque prh_atom_u32_arrque;
prh_atom_u32_arrque *prh_atom_u32_arrque_init(int size); // size must be power of 2
void prh_atom_u32_arrque_free(prh_atom_u32_arrque **p);
bool prh_atom_u32_arrque_push(prh_atom_u32_arrque *q, prh_u32 v); // v shall not be 0
prh_u32 prh_atom_u32_arrque_pop(prh_atom_u32_arrque *q); // return 0 means empty
prh_u32 prh_atom_u32_arrque_top(prh_atom_u32_arrque *q);
int prh_atom_u32_arrque_len(prh_atom_u32_arrque *q);
int prh_atom_u32_arrque_cap(prh_atom_u32_arrque *q);

typedef struct prh_atom_u32_arrque prh_atom_i32_arrque;
prh_atom_i32_arrque *prh_atom_i32_arrque_init(int size); // size must be power of 2
void prh_atom_i32_arrque_free(prh_atom_i32_arrque **p);
bool prh_atom_i32_arrque_push(prh_atom_i32_arrque *q, prh_i32 v); // v shall not be 0
prh_i32 prh_atom_i32_arrque_pop(prh_atom_i32_arrque *q); // return 0 means empty
prh_i32 prh_atom_i32_arrque_top(prh_atom_i32_arrque *q);
int prh_atom_i32_arrque_len(prh_atom_i32_arrque *q);
int prh_atom_i32_arrque_cap(prh_atom_i32_arrque *q);

#ifdef PRH_ATOMIC_STRIP_PREFIX
#define atom_unt_arrque_t                prh_atom_unt_arrque
#define atom_unt_arrque_init             prh_atom_unt_arrque_init
#define atom_unt_arrque_free             prh_atom_unt_arrque_free
#define atom_unt_arrque_push             prh_atom_unt_arrque_push
#define atom_unt_arrque_pop              prh_atom_unt_arrque_pop
#define atom_unt_arrque_top              prh_atom_unt_arrque_top
#define atom_unt_arrque_len              prh_atom_unt_arrque_len
#define atom_unt_arrque_cap              prh_atom_unt_arrque_cap
#define atom_u32_arrque_t                prh_atom_u32_arrque
#define atom_u32_arrque_init             prh_atom_u32_arrque_init
#define atom_u32_arrque_free             prh_atom_u32_arrque_free
#define atom_u32_arrque_push             prh_atom_u32_arrque_push
#define atom_u32_arrque_pop              prh_atom_u32_arrque_pop
#define atom_u32_arrque_top              prh_atom_u32_arrque_top
#define atom_u32_arrque_len              prh_atom_u32_arrque_len
#define atom_u32_arrque_cap              prh_atom_u32_arrque_cap
#endif

#ifdef PRH_ATOMIC_IMPLEMENTATION
typedef struct {
    union { prh_u32 size: 31, free: 1; void *align; } u;
    prh_atom_int head;
    prh_atom_int tail;
} prh_atom_unt_arrque;

typedef struct {
    union { prh_u32 size: 31, free: 1; void *align; } u;
    prh_atom_int head;
    prh_atom_int tail;
} prh_atom_u32_arrque;

#define prh_impl_atom_unt_arrque_nextpos(q, pos) (((pos) + 1) & ((q)->u.size - 1))

bool prh_impl_atom_unt_arrque_full(prh_atom_unt_arrque *q) { // is not an atomic operation
    return (prh_impl_atom_unt_arrque_nextpos(q, prh_atomtype_load(&q->tail)) == prh_atomtype_load(&q->head));
}

prh_inline int prh_impl_atom_unt_arrque_alloc_size(int size, int stride) {
    assert(prh_is_power_of_2(size));
    return prh_round_ptrsize(sizeof(prh_atom_unt_arrque) + stride * size);
}

int prh_atom_unt_arrque_alloc_size(int size) {
    return prh_impl_atom_unt_arrque_alloc_size(size, sizeof(void *));
}

int prh_atom_u32_arrque_alloc_size(int size) {
    return prh_impl_atom_unt_arrque_alloc_size(size, sizeof(int));
}

prh_atom_unt_arrque *prh_atom_unt_arrque_init_inplace(void *addr, int size) {
    prh_atom_unt_arrque *q = (prh_atom_unt_arrque *)addr;
    q->u.size = size;
    q->u.free = 0;
    prh_atomtype_init(&q->head, 0);
    prh_atomtype_init(&q->tail, 0);
    return q;
}

prh_atom_u32_arrque *prh_atom_u32_arrque_init_inplace(void *addr, int size) {
    return (prh_atom_u32_arrque *)prh_atom_unt_arrque_init_inplace(addr, size);
}

prh_atom_unt_arrque *prh_atom_unt_arrque_init(int size) {
    void *addr = prh_malloc(atom_unt_arrque_alloc_size(size));
    prh_atom_unt_arrque *q = prh_atom_unt_arrque_init_inplace(addr, size);
    q->u.free = 1;
    return q;
}

prh_atom_u32_arrque *prh_atom_u32_arrque_init(int size) {
    void *addr = prh_malloc(prh_atom_u32_arrque_alloc_size(size));
    prh_atom_u32_arrque *q = prh_atom_u32_arrque_init_inplace(addr, size);
    ((prh_atom_unt_arrque *)q)->u.free = 1;
    return q;
}

void prh_atom_unt_arrque_free(prh_atom_unt_arrque **p) {
    prh_atom_unt_arrque *q = *p;
    if (q == prh_null)
        return;
    if (q->u.free) {
        prh_free(q);
    }
    *p = prh_null;
}

void prh_atom_u32_arrque_free(prh_atom_u32_arrque **p) {
    prh_atom_unt_arrque_free((prh_atom_unt_arrque **)p);
}

int prh_atom_unt_arrque_len(prh_atom_unt_arrque *q) { // is not an atomic operation
    int len = prh_atomtype_load(&q->tail) - prh_atomtype_load(&q->head);
    if (len < 0)
        len += q->u.size;
    return len;
}

int prh_atom_unt_arrque_cap(prh_atom_unt_arrque *q) {
    return q->u.size;
}

int prh_atom_u32_arrque_len(prh_atom_u32_arrque *q) {
    return prh_atom_unt_arrque_len((prh_atom_unt_arrque *)q);
}

int prh_atom_u32_arrque_cap(prh_atom_u32_arrque *q) {
    return prh_atom_unt_arrque_cap((prh_atom_unt_arrque *)q);
}

bool prh_impl_atom_unt_arrque_finish_push(prh_atom_unt_arrque *q, int tail) {
    int next = prh_impl_atom_unt_arrque_nextpos(q, tail);
    if (next == prh_atomtype_load(&q->head))
        return false;
    assert(prh_atomtype_load(&q->tail) == tail); // only allow one writer
    prh_atomtype_store(&q->tail, next);
    return true;
}

void prh_impl_atom_unt_arrque_finish_pop(prh_atom_unt_arrque *q, int head) {
    assert(prh_atomtype_load(&q->head) == head); // only allow one reader
    prh_atomtype_store(&q->head, prh_impl_atom_unt_arrque_nextpos(q, head));
}

#define prh_impl_get_ptr_que(q) ((void **)((prh_atom_unt_arrque *)(q) + 1))
#define prh_impl_get_int_que(q) ((int *)((prh_atom_unt_arrque *)(q) + 1))

bool prh_atom_unt_arrque_push(prh_atom_unt_arrque *q, void *v) {
    if (v == prh_null) return false;
    int tail = prh_atomtype_load(&q->tail);
    prh_impl_get_ptr_que(q)[tail] = v;
    return prh_impl_atom_unt_arrque_finish_push(q, tail);
}

void *prh_atom_unt_arrque_pop(prh_atom_unt_arrque *q) {
    int head = prh_atomtype_load(&q->head);
    if (head == prh_atomtype_load(&q->tail)) {
        return prh_null;
    }
    void *v = prh_impl_get_ptr_que(q)[head];
    prh_impl_atom_unt_arrque_finish_pop(q, head);
    return v;
}

void *prh_atom_unt_arrque_top(prh_atom_unt_arrque *q) {
    int head = prh_atomtype_load(&q->head);
    if (head == prh_atomtype_load(&q->tail)) {
        return prh_null;
    }
    return prh_impl_get_ptr_que(q)[head];
}

bool prh_atom_u32_arrque_push(prh_atom_u32_arrque *q, int v) {
    if (v == 0) return false;
    int tail = prh_atomtype_load(&((prh_atom_unt_arrque *)q)->tail);
    prh_impl_get_int_que(q)[tail] = v;
    return prh_impl_atom_unt_arrque_finish_push((prh_atom_unt_arrque *)q, tail);
}

int prh_atom_u32_arrque_pop(prh_atom_u32_arrque *q) { // return 0 means empty
    int head = prh_atomtype_load(&((prh_atom_unt_arrque *)q)->head);
    if (head == prh_atomtype_load(&((prh_atom_unt_arrque *)q)->tail)) {
        return 0;
    }
    int v = prh_impl_get_int_que(q)[head];
    prh_impl_atom_unt_arrque_finish_pop((prh_atom_unt_arrque *)q, head);
    return v;
}

int prh_atom_u32_arrque_top(prh_atom_u32_arrque *q) {
    int head = prh_atomtype_load(&((prh_atom_unt_arrque *)q)->head);
    if (head == prh_atomtype_load(&((prh_atom_unt_arrque *)q)->tail)) {
        return 0;
    }
    return prh_impl_get_int_que(q)[head];
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
    return prh_impl_filetime_nsec(&kernal_time) + prh_impl_filetime_nsec(&user_time);
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

void prh_impl_date_time(prh_datetime *t, const struct tm *s, prh_i32 usec) {
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

void prh_impl_system_date(struct tm *s, const prh_datetime *t) {
    memset(s, 0, sizeof(struct tm));
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
    utc_time->usec = local_date->usec;
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
    prh_impl_date_time(local_date, &tm, utc_time.usec);
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
    time_t time = (time_t)p->sec;
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
    prh_i64 ticks_per_sec = PRH_NSEC_PER_SEC;
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
    prh_release_assert((1000000000001LL * 1000000000LL / 1000000LL) != 1000000000001000LL);
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
void *prh_thrd_create(prh_thrd_struct *s, int thrdudsize); // only create thrd, after init userdata, call prh_thrd_starx
int prh_thrd_start(prh_thrd_struct *s, prh_thrdproc_t proc, int stacksize, void *userdata); // return thrd index
int prh_thrd_starx(prh_thrd_struct *s, void *thrd_create_data, prh_thrdproc_t proc, int stacksize); // return thrd index
void prh_thrd_join(prh_thrd_struct *s, int thrd_index, prh_thrdfree_t udfree); // remember to free main thrd using prh_thrd_free
void prh_thrd_jall(prh_thrd_struct **s, prh_thrdfree_t udfree); // free all joined thrd including main thrd
void prh_thrd_free(prh_thrd_struct **s, prh_thrdfree_t udfree); // free main thrd

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
#define thrd_create                 prh_thrd_create
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
    printf("[thread %d] stack %p size %d-byte, guard size %d-byte\n",
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
    return proc(thrd);
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

void *prh_thrd_create(prh_thrd_struct *s, int thrdudsize) {
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
    return thrd_index;
}

void prh_impl_thrd_join(prh_thrd **thrd_list, int thrd_index, prh_thrdfree_t udfree) {
    prh_thrd *thrd = thrd_list[thrd_index];
    if (thrd == prh_null) return;
    void *retv = prh_null, *userdata;
    int n = pthread_join(thrd->u.tid_impl, &retv);
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
    if (udfree) {
        udfree(prh_thrd_data(thrd), thrd_index);
    }
    prh_aligned_free(thrd);
    thrd_list[thrd_index] = prh_null;
}

void prh_thrd_join(prh_thrd_struct *s, int thrd_index, prh_thrdfree_t udfree) {
    assert(thrd_index > 0 && thrd_index <= s->maxthrds);
    prh_impl_thrd_join(prh_thrd_list_begin(s), thrd_index, udfree);
}

void prh_thrd_free(prh_thrd_struct **main, prh_thrdfree_t udfree) {
    prh_thrd_struct *s = *main;
    if (s == prh_null) return;
    prh_thrd *main_thrd = s->main;
    if (main_thrd) {
        if (udfree) {
            udfree(prh_thrd_data(main_thrd), 0);
        }
        prh_aligned_free(main_thrd);
        s->main = prh_null;
    }
    prh_free(s);
    *main = prh_null;
}

void prh_thrd_join_except_main(prh_thrd_struct **main, prh_thrdfree_t udfree) {
    prh_thrd_struct *s = *main;
    if (s == prh_null) return;
    prh_thrd *thrd_list = prh_thrd_list_begin(s);
    for (int i = 1; i <= s->maxthrds; i += 1) {
        prh_impl_thrd_join(thrd_list, i, udfree);
    }
}

void prh_thrd_jall(prh_thrd_struct **main, prh_thrdfree_t udfree) {
    prh_thrd_struct *s = *main;
    if (s == prh_null) return;
    prh_thrd *thrd_list = prh_thrd_list_begin(s);
    for (int i = 1; i <= s->maxthrds; i += 1) {
        prh_impl_thrd_join(thrd_list, i, udfree);
    }
    prh_thrd_free(main, udfree);
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

#ifdef PRH_CONC_INCLUDE
typedef struct prh_spawn_data prh_spawn_data;
typedef struct prh_await_data prh_await_data;
void prh_concurrency_main(int thrd_start_id, int num_thread, prh_coroproc_t main_proc, int stack_size);
void *prh_coro_spawn(prh_coro *coro, prh_coroproc_t proc, int stack_size, int maxudsize);
void prh_coro_sched(prh_coro *coro, prh_spawn_data *coro_spawn_data);
void *prh_coro_await(prh_coro *coro);
void prh_coro_continue(prh_coro *coro, prh_await_data *coro_await_data);

#ifdef PRH_CONC_IMPLEMENTATION
typedef enum {
    PRH_CORO_ID_NULL,
    PRH_FIXED_CORO_MAX,
} prh_impl_coro_id;

typedef enum {
    PRH_CORO_CONTINUE,
    PRH_CORO_START, // 子协程创建后开始启动
    PRH_CORO_SWAIT, // 等待子协程启动成功
    PRH_CORO_YIELD,
    PRH_CORO_AWAIT,
    PRH_CORO_PDATA,
    PRH_CORO_PWAIT,
    PRH_CORO_EXIT,
    PRH_CORO_REQ_MAX
} prh_impl_coro_action;

struct prh_cono;
typedef struct {
    struct prh_cono *head;
    struct prh_cono *tail;
} prh_cono_quefit;

typedef struct {
    // 仅由当前线程访问
    prh_soro_struct soro; // 当前执行协程
    prh_snode_t cono_2_free_node;
    bool coro_assigned;
    bool pending_work_exist;
    // 可被特权线程访问
    prh_atom_data_quefix coro_req_que;
    prh_atom_ptr ready_coro; // 由特权线程写入，由特权线程窃取清空，或由当前协程读取清空
    prh_sleep_cond sleep_cond;
} prh_cono_thrd; // 每个线程尽量指定在单一的CPU上运行避免线程切换

void prh_impl_cono_thrd_init(prh_thrd *thrd) {
    prh_cono_thrd *cono_thrd = prh_impl_cono_thrd(thrd);
    prh_soro_init(&cono_thrd->soro, prh_thrd_id(thrd));
    prh_atom_data_quefix_init(&cono_thrd->coro_req_que);
    prh_atom_ptr_init(&cono_thrd->ready_coro, prh_null);
    prh_impl_sleep_cond_init(&cono_thrd->sleep_cond);
}

void prh_impl_cono_thrd_free(prh_cono_thrd *t) {
    prh_atomnodque_free(t, txmq, prh_null);
}

typedef struct prh_cono {
    // 仅由执行线程访问
    prh_i32 coro_id;
    prh_byte action;
    bool finished;
    bool idle_wait;
    prh_cono *caller;
    prh_cono *callee;
    // 仅由特权线程访问
    prh_cono_quefit callee_que;
    prh_quefit pdata_que;
    prh_cono_thrd *assign_thrd;
    prh_cono *coro_chain;
} prh_cono; // 跨线程协程

void prh_impl_cono_init(prh_cono *cono, prh_i32 coro_id) {
    cono->coro_id = coro_id;
}

typedef struct {
    prh_thrd_struct *thrd_struct; // 初始化后只读
    prh_cono_thrd *main_thread; // 初始化后只读
    prh_cono *fixed_coro[PRH_FIXED_CORO_MAX]; // 初始化后只读
    // 多线程读写
    prh_cono_quefit ready_queue;
    prh_i32 coro_id_seed;
    prh_atom_ptr privilege_thread;
    prh_atom_bool thrd_exit;
} prh_conc_struct;

static prh_conc_struct PRH_IMPL_CONC_STRUCT;

prh_cono *prh_impl_get_fixed_coro(prh_int coro_id) {
    assert(coro_id < PRH_FIXED_CORO_MAX);
    return PRH_IMPL_CONC_STRUCT.fixed_coro[coro_id];
}

prh_inline int prh_impl_cono_size(void) {
    return (int)prh_round_16_byte(sizeof(prh_cono));
}

prh_inline void *prh_impl_cono_data(prh_cono *cono) {
    return (char *)cono + prh_impl_cono_size();
}

prh_inline prh_cono *prh_impl_cono_from_coro(prh_coro_t *coro) {
    return (prh_cono *)((char *)coro + prh_impl_coro_size);
}

prh_inline prh_cono *prh_impl_cono_from_data(void *data) {
    return (prh_cono *)((char *)data - prh_impl_cono_size());
}

prh_inline prh_cono_thrd *prh_impl_cono_thrd(prh_thrd *thrd) {
    return (prh_cono_thrd *)((char *)thrd + prh_impl_thrd_head_size);
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
typedef void (*prh_impl_coro_req_func)(prh_cono *req_coro, prh_cono_quefit *ready_queue);

void prh_impl_coro_continue_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {
    if (req_coro->finished) {
        prh_impl_coro_free(prh_impl_coro_from_cono(req_coro));
    } else { // 继续执行协程
        prh_relaxed_quefit_push(ready_queue, req_coro, coro_chain);
    }
}

void prh_impl_coro_start_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {
    prh_impl_cono_init(req_coro, ++PRH_IMPL_CONC_STRUCT.coro_id_seed); // 初始化新协程
    prh_relaxed_quefit_push(ready_queue, req_coro, coro_chain); // 新建协程可以安全插入就绪队列
}

void prh_impl_coro_swait_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {

}

void prh_impl_coro_yield_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {
    prh_cono *caller = req_coro->caller;
    if (caller) { // 需要提交执行结果给请求协程
        if (caller->idle_wait) { // 请求协程正在等待执行结果，且其结果队列为空
            caller->callee = req_coro;
            caller->idle_wait = false; // 将结果提交给请求协程，并立即调度其执行
            prh_relaxed_quefit_push(ready_queue, caller, coro_chain);
        } else { // 将执行协程插入请求协程的结果队列中，等待请求下一次WAIT挂起继续执行
            prh_relaxed_quefit_push(&caller->callee_que, req_coro, coro_chain);
        }
    } else { // 执行协程不需要将结果提交给请求协程，执行结束或者继续无条件执行
        prh_impl_coro_continue_req(req_coro);
    }
}

void prh_impl_coro_await_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {
    prh_cono *callee; // 等待执行协程的执行结果
    prh_relaxed_quefit_pop(&req_coro->callee_que, callee, coro_chain);
    if (callee) { // 协程给特权发送消息，都会挂起等待特权调度继续执行，因此协程的请求都是顺序的，对于一个协程不可能同时有多个等待执行的请求存在
        req_coro->callee = callee; // 有结果存在，直接交给请求协程继续执行
        prh_relaxed_quefit_push(ready_queue, caller, coro_chain);
    } else {
        req_coro->idle_wait = true; // 暂时没有执行结果可以处理，等待下一次执行协程的结果
    }
}

void prh_impl_coro_pdata_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {

}

void prh_impl_coro_pwait_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {

}

void prh_impl_coro_exit_req(prh_cono *req_coro, prh_cono_quefit *ready_queue) {

}

static prh_impl_coro_req_func PRH_IMPL_CORO_REQ_FUNC[PRH_CORO_REQ_MAX] = {
    prh_impl_coro_continue_req,
    prh_impl_coro_start_req,
    prh_impl_coro_swait_req,
    prh_impl_coro_yield_req,
    prh_impl_coro_await_req,
    prh_impl_coro_pdata_req,
    prh_impl_coro_pwait_req,
    prh_impl_coro_exit_req,
};

void prh_impl_coro_req(prh_cono *req_coro, int action) {
    prh_cono_thrd *thrd = req_coro->assign_thrd;
    assert(thrd == prh_thrd_self_data());
    req_coro->action = action;
    prh_atom_data_quefix_push(&thrd->coro_req_que, req_coro);
}

void prh_impl_callee_continue(prh_cono *coro) {
    if (coro->callee) {
        prh_impl_coro_req(coro->callee, PRH_CORO_CONTINUE);
        coro->callee = prh_null;
    }
}

void prh_impl_cross_thread_coro_yield(prh_coro *coro) {
    prh_cono *cono = prh_impl_cono_from_coro(coro);
    cono->finished = (coro->rspoffset == prh_null);
    cono->action = PRH_CORO_YIELD;
}

void prh_coro_sched(prh_coro *coro, prh_spawn_data *coro_spawn_data) {
    prh_cono *callee = prh_impl_cono_from_data(coro_spawn_data);
    prh_impl_coro_req(callee, PRH_CORO_START);
}

void prh_impl_cono_yield(prh_coro *coro, int action) {
    prh_cono *caller = prh_impl_cono_from_coro(coro);
    caller->action = action;
    prh_impl_coro_yield(coro);
}

void *prh_coro_await(prh_coro *coro) {
    prh_cono *caller = prh_impl_cono_from_coro(coro);
    prh_impl_cono_yield(coro, PRH_CORO_AWAIT);
    return prh_impl_cono_data(caller->callee);
}

void prh_coro_continue(prh_coro *coro, void *await_userdata) {
    prh_cono *caller = prh_impl_cono_from_coro(coro);
    assert(caller->callee == prh_impl_cono_from_data(await_userdata));
    prh_impl_callee_continue(caller);
}

void prh_impl_coro_execute(prh_cono_thrd *cono_thrd, prh_cono *coro) {
    prh_soro_struct *soro = &cono_thrd->soro;
    soro->coro = prh_impl_coro_from_cono(coro);
    if (coro->action == PRH_CORO_AWAIT) {
        prh_soro_start(soro);
        prh_impl_callee_continue(coro);
    } else {
        prh_soro_start(soro); // 继续执行当前协程，直到协程挂起或执行完毕返回到主协程
    }
    prh_impl_coro_req(coro, coro->action); // 返回主协程之前，coro 会设置好 action
}

prh_inline void prh_impl_coro_req_process(prh_cono *req_coro) {
    assert(req_coro->action < PRH_CORO_REQ_MAX);
    PRH_IMPL_CORO_REQ_FUNC[req_coro->action](req_coro);
}

bool prh_impl_privilege_task(prh_cono_thrd *curr_thrd, bool strong_check) {
    prh_atom_ptr *privilege_thread = &PRH_IMPL_CONC_STRUCT.privilege_thread;
    prh_cono_quefit *ready_queue = &PRH_IMPL_CONC_STRUCT.ready_queue;
    bool pending_work = false;
    curr_thrd->coro_assigned = false;
    curr_thrd->pending_work_exist = false;
    prh_cono_thrd *thrd;
    prh_data_quefit all_req;
    prh_cono *ready_coro;

    if (strong_check) { // 获取特权
        if (!prh_atom_ptr_strong_compare_null_write(privilege_thread, curr_thrd)) {
            return false;
        }
    } else {
        if (!prh_atom_ptr_compare_null_write(privilege_thread, curr_thrd)) {
            return false;
        }
    }

    prh_thrd **thrd_begin = prh_thrd_list_begin(PRH_IMPL_CONC_STRUCT.thrd_struct);
    prh_thrd **thrd_end = prh_thrd_list_begin(PRH_IMPL_CONC_STRUCT.thrd_struct);
    prh_thrd **thrd_it = thrd_begin;
    for (; thrd_it < thrd_end; thrd_it += 1) { // 处理特权消息，将就绪协程插入就绪队列
        if (*thrd_it == prh_null) continue;
        thrd = prh_impl_cono_thrd(*thrd_it);
        if (!prh_atom_data_quefix_pop_all(&thrd->coro_req_que, &all_req)) {
            continue;
        }
        prh_data_quefit_for_begin(&all_req) {
            prh_impl_coro_req_process((prh_cono *)it->data, ready_queue);
            prh_data_quefit_for_end();
        }
        prh_atom_data_quefix_free_node(&thrd->coro_req_que, &all_req);
    }

    thrd_it = thrd_begin;
    for (; ;) { // 将就绪队列中的协程分配给线程执行
        prh_relaxed_quefit_pop(ready_queue, ready_coro, coro_chain);
        if (ready_coro == prh_null) break;
        thrd = ready_coro->assign_thrd;
        if (thrd && prh_atom_ptr_strong_compare_null_write(&thrd->ready_coro, ready_coro)) {
            prh_thrd_wakeup(thrd->sleep_cond);
            continue; // 尽量将协程分配在同一个线程中执行
        }
        for (; thrd_it < thrd_end; thrd_it += 1) { // 将协程分配给空闲协程
            if (*thrd_it == prh_null) continue;
            thrd = prh_impl_cono_thrd(*thrd_it);
            if (prh_atom_ptr_strong_compare_null_write(&thrd->ready_coro, ready_coro)) {
                ready_coro->assign_thrd = thrd;
                prh_thrd_wakeup(thrd->sleep_cond);
                ready_coro = prh_null;
                break;
            }
        }
        if (ready_coro) { // 没有足够的线程处理该协程，将协程重新插入就绪队列，并退出循环
            prh_relaxed_quefit_push_front(ready_queue, ready_coro, coro_chain);
            break;
        }
    }

    if (curr_thrd->ready_coro) { // 如果当前线程没有分配到协程，从其他线程偷取协程来执行
        curr_thrd->coro_assigned = true;
    } else if (strong_check && curr_thrd != PRH_IMPL_CONC_STRUCT.main_thread) {
        thrd_it = thrd_begin; // 主线程不需要偷取任务，如果就绪队列里面有任务，第一个分配的线程就是主线程，如果就绪队列里已经没有任务了，表示任务都已经分配完毕了，不需要主线程帮忙
        for (; thrd_it < thrd_end; thrd_it += 1) {
            if (*thrd_it == prh_null) continue;
            thrd = prh_impl_cono_thrd(*thrd_it);
            if ((ready_coro = prh_atom_ptr_read(&thrd->ready_coro)) && prh_atom_compare_write(&thrd->ready_coro, &ready_coro, prh_null)) {
                curr_thrd->ready_coro = ready_coro;
                ready_coro->assign_thrd = curr_thrd;
                curr_thrd->coro_assigned = true;
                break;
            }
        }
    }

    if (strong_check && curr_thrd == PRH_IMPL_CONC_STRUCT.main_thread) {
        if (prh_relaxed_quefit_not_empty(ready_queue)) {
            curr_thrd->pending_work_exist = true; // 就绪队列不为空，表示所有线程都安排工作后，还有剩余的协程，相当于所有的线程都处于忙碌状态
        } else {
            thrd_it = thrd_begin;
            for (; thrd_it < thrd_end; thrd_it += 1) {
                if (*thrd_it == prh_null) continue;
                thrd = prh_impl_cono_thrd(*thrd_it);
                if ((ready_coro = prh_atom_ptr_read(&thrd->ready_coro))) {
                    curr_thrd->pending_work_exist = true; // 还有分配的协程没有执行
                    break;
                }
            }
        }
    }

    prh_atom_ptr_store(privilege_thread, prh_null); // 释放特权
    return true;
}

prh_void_ptr prh_impl_cono_thrd_proc(prh_thrd* thrd) {
    prh_cono_thrd *cono_thrd = prh_impl_cono_thrd(thrd);
    prh_atom_ptr *thrd_ready_coro = &cono_thrd->ready_coro;
    prh_cono *ready_coro;

    for (; ;) {
        while ((ready_coro = prh_atom_ptr_read(thrd_ready_coro))) {
            if (prh_atom_ptr_compare_write(thrd_ready_coro, &ready_coro, prh_null)) {
                prh_impl_coro_execute(cono_thrd, ready_coro);
            } else { // 自己的任务可能被别的线程抢掉
                prh_impl_privilege_task(cono_thrd, false);
            }
        }
        prh_impl_privilege_task(cono_thrd, true);
        if (cono_thrd->coro_assigned || prh_atom_ptr_read(thrd_ready_coro)) {
            continue; // 被安排新任务，或在没抢到特权的情况下，可能被其他特权线程安排任务
        }
        if (prh_atom_bool_read(PRH_IMPL_CONC_STRUCT.thrd_exit)) {
            break;
        }
        prh_thrd_sleep(&thrd->sleep_cond);
    }

    return 0;
}

prh_coro *prh_impl_cono_create(prh_coroproc_t proc, int stack_size, int maxudsize) {
    prh_coro *coro = prh_impl_coro_creatx(proc, stack_size, sizeof(prh_cono), maxudsize);
    coro->cross_thread = 1;
    return coro;
}

void *prh_cono_create_type_1_coro(prh_coro *caller, prh_coroproc_t proc, int stack_size, int maxudsize) {
    prh_coro *coro = prh_impl_cono_create(proc, stack_size, maxudsize);
    void *userdata = prh_coro_data(coro);
    prh_cono *cono = prh_impl_cono_from_data(userdata);
    cono->caller_need_result = prh_impl_cono_from_coro(caller);
    cono->coro_type = PRH_CORO_TYPE_1;
    return userdata;
}

prh_coro *prh_cono_type_2_coro(prh_coroproc_t proc, int stack_size, prh_void_ptr userdata) {
    prh_coro *coro = prh_impl_cono_create(proc, stack_size, 0);
    prh_cono *cono = prh_impl_cono_from_coro(coro);
    cono->coro_type = PRH_CORO_TYPE_2;
    coro->userdata = (void *)userdata;
    return coro;
}

void *prh_cono_creatx_type_2_coro(prh_coroproc_t proc, int stack_size, int maxudsize) {
    prh_coro *coro = prh_impl_cono_create(proc, stack_size, maxudsize);
    void *userdata = prh_coro_data(coro);
    prh_cono *cono = prh_impl_cono_from_data(userdata);
    cono->coro_type = PRH_CORO_TYPE_2;
    return userdata;
}

prh_cono *prh_impl_cono_init_fixed_coro(prh_coro *coro, prh_i32 coro_id) {
    assert(coro_id < PRH_FIXED_CORO_MAX);
    prh_cono *cono = prh_impl_cono_from_coro(coro);
    prh_impl_cono_init(cono, coro_id);
    PRH_IMPL_CONC_STRUCT.fixed_coro[coro_id] = coro;
    return cono;
}

prh_cono *prh_impl_cono_create_root(void) {
    prh_coro *coro = prh_cono_type_2_coro(prh_impl_root_coro_proc, PRH_IMPL_ROOT_CORO_STACK_SIZE, PRH_FIXED_CORO_MAX);
    prh_cono *cono = prh_impl_cono_init_fixed_coro(coro, PRH_CORO_ID_ROOT);
    cono->already_in_ready_queue = 1; // 根协程由特权线程直接执行，不需要插入就绪队列
    return cono;
}

void prh_concurrency_main(int thrd_start_id, int num_thread, prh_coroproc_t main_proc, int stack_size) {
    prh_conc_struct *s = &PRH_IMPL_CONC_STRUCT;
    prh_thrd_struct *pool = prh_thread_init(thrd_start_id, num_thread, sizeof(prh_cono_thrd));
    prh_thread_init_main_userdata(pool, (prh_thrdinit_t)prh_impl_cono_thrd_init);
    prh_cono_thrd *main = prh_thread_main_userdata(pool);
    prh_atomtype_store(&main->thread_available, false);
    s->coro_thrd_pool = pool;
    s->coro_thrd_list = pool->thrd;
    s->numcorothrds = (num_thread > 0 ? num_thread : 0) + 1;
    prh_quefit_init(&s->coro_ready_que);
    prh_atomtype_init(&s->privilege_thread, prh_atomnull);
    for (int i = 0; i < num_thread; i += 1) {
        prh_thread_ext_create(pool, prh_impl_cono_thrd_proc, 0, sizeof(prh_cono_thrd), (prh_thrdinit_t)prh_impl_cono_thrd_init);
    }

    prh_conc_struct *s = &PRH_IMPL_CONC_STRUCT;
    prh_thrd_struct *thrd_struct = s->coro_thrd_pool;
    prh_thrd *thrd = prh_thrd_main(thrd_struct);
    prh_cono_thrd *main_thrd = prh_thrd_data(thrd);
    prh_atomtype_store(&main->thread_available, true);

    prh_atom_ptr *thrd_ready_coro = &main_thrd->ready_coro;
    prh_cono *ready_coro;

    // 启动入口协程（主协程）
    // 程序的执行从主线程执行第一个协程开始，这个协程可以称为主协程，但主协程没有与主
    // 线程进行绑定，只是由主线程发起了执行的动作，后续恢复挂起的主协程的执行可以在任意
    // 线程，主协程也只是一个普通的协程而已。
    prh_impl_cono_thrd_init(thrd);
    prh_impl_coro_execute(main_thrd, prh_impl_cono_create(main_proc, stack_size, 0));

    for (; ;) {
        while ((ready_coro = prh_atom_ptr_read(thrd_ready_coro))) {
            if (prh_atom_ptr_compare_write(thrd_ready_coro, &ready_coro, prh_null)) {
                prh_impl_coro_execute(main_thrd, ready_coro);
            }
            prh_impl_privilege_task(main_thrd, false);
        }
        prh_impl_privilege_task(main_thrd, true);
        if (curr_thrd->pending_work_exist || prh_atom_ptr_read(thrd_ready_coro)) {
            continue;
        }
        if (prh_atom_bool_read(PRH_IMPL_CONC_STRUCT.thrd_exit)) {
            prh_thrd_join_except_main(thrd_struct, prh_impl_cono_thrd_free);
            while (!prh_impl_privilege_task(main_thrd, true)) ;
            if (curr_thrd->pending_work_exist || prh_atom_ptr_read(thrd_ready_coro) {
                continue;
            }
            break;
        }
        prh_thrd_sleep(thrd->sleep_cond);
    }

    prh_thrd_free(&PRH_IMPL_CONC_STRUCT.thrd_struct, prh_impl_cono_thrd_free);
}

#ifdef PRH_TEST_IMPLEMENTATION
void prh_impl_conc_test(void) {

}
#endif // PRH_TEST_IMPLEMENTATION
#endif // PRH_CONC_IMPLEMENTATION
#endif // PRH_CONC_INCLUDE

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