// prh_include.h - public domain - swdayu <github.com/swdayu>
//
// Version Conventions
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
// Naming Conventions
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
// Stripping Off Prefix
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

#ifndef PRH_ARRAY_SIZE
    #define PRH_ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

#ifndef PRH_NOT_USE
    #define PRH_NOT_USE(a) ((void)(a))
#endif

#ifndef PRH_DEBUG
#ifdef NDEBUG
    #define PRH_DEBUG 0
#else
    #define PRH_DEBUG 1
#endif
#endif

#ifndef prh_macro_concat_name
#define prh_macro_concat_name(a, b) prh_impl_macro_concat_name(a, b)
#define prh_impl_macro_concat_name(a, b) a ## b
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
    #define prh_prerr(err) prh_impl_prerr((err), __LINE__)
    void prh_impl_prerr(int err, int line);
#endif

#ifndef prh_zeroret
#if PRH_DEBUG // macro arg 'a' can only expand once
    #define prh_zeroret(a) if ((a) != 0) { exit(__LINE__); }
#else
    #define prh_zeroret(a) a
#endif
#endif

#ifndef prh_null
    #if defined(__cplusplus)
    // The macro NULL is an implementation-defined null pointer constant.
    // In C, the macro NULL may have the type void*, but that is not
    // allowed in C++ because null pointer constants cannot have that
    // type.
    #if __cplusplus >= 201103L // C++11 keyword
        #define prh_null 0
    #else
        #define prh_null nullptr
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
    typedef int prh_int;
    typedef unsigned long long prh_u64;
    typedef long long prh_i64;
#if prh_arch_bits == 32
    typedef prh_u32 prh_unsigned;
    typedef prh_i32 prh_signed;
#elif prh_arch_bits == 64
    #ifdef prh_32_bits_pointer
        typedef prh_u32 prh_unsigned;
        typedef prh_i32 prh_signed;
    #else
        typedef prh_u64 prh_unsigned;
        typedef prh_i64 prh_signed;
    #endif
#else
    #error unsupported architecture
#endif
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
    prh_static_assert(sizeof(prh_int) == 4);
    prh_static_assert(sizeof(prh_u64) == 8);
    prh_static_assert(sizeof(prh_i64) == 8);
    prh_static_assert(sizeof(prh_unsigned) == sizeof(void *));
    prh_static_assert(sizeof(prh_signed) == sizeof(void *));
    prh_static_assert(sizeof(prh_f32) == 4);
    prh_static_assert(sizeof(prh_f64) == 8);
    prh_static_assert(sizeof(prh_float) == 4);
#endif

#ifndef prh_malloc
#define prh_malloc(size) malloc(size)
// void *malloc(size_t size);
// the newly allocated block of memory is not initialized, remaining with indeterminate values.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.
#endif

#ifndef prh_calloc
#define prh_calloc(size) calloc(1, (size))
// void *calloc(size_t num, size_t size);
// allocates a block of memory for an array of num elements, each of them size bytes long, and
// initializes all its bits to zero. the effective result is the allocation of a zero-initialized
// memory block of (num*size) bytes.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.
#endif

#ifndef prh_realloc
#define prh_realloc realloc
// void *realloc(void *ptr, size_t size);
// if ptr == prh_null { return malloc(size) }
// if size == 0 { may be free(ptr) or depends on library implementation }
// if size > ptr old size { may return the new location and the newer portion is indeterminate }
// if fails to allocate the requested block of memory, null is returned and ptr remain unchanged.
#endif

#ifndef prh_free
#define prh_free free
// void free(void *ptr);
// if ptr == prh_null { the function does nothing }
#endif

#ifndef prh_round_ptrsize
#define prh_round_ptrsize(n) (((prh_unsigned)(n) + (prh_unsigned)(sizeof(void*)-1)) & (~(prh_unsigned)(sizeof(void*)-1)))
#define prh_round_16_byte(n) (((prh_unsigned)(n) + 15) & (~(prh_unsigned)15))
// 0000 & 0000 -> 0000
// 0001 & 1111 -> 0001
// 1010 & 0110 -> 0010
prh_inline prh_unsigned prh_lower_most_bit(prh_unsigned n) {
    return n & (-(prh_signed)n);
}
prh_inline prh_unsigned prh_remove_lower_most_bit(prh_unsigned n) {
    return n & (n - 1);
}
prh_inline bool prh_is_power_of_2(prh_unsigned n) {
    return prh_remove_lower_most_bit(n) == 0; // power of 2 or zero
}
prh_inline prh_unsigned prh_to_power_of_2(prh_unsigned n) {
    if (prh_is_power_of_2(n)) return n;
    // TODO: 字节序交换然后计算lower most bit
    prh_unsigned m = prh_lower_most_bit(n);
    while (m < n) m <<= 1;
    return m;
}
#endif

#ifdef PRH_CONC_IMPLEMENTATION
#if defined(_WIN32)

#else
    #define _POSIX_C_SOURCE 200809L
    // glibc https://www.gnu.org/software/libc/
    // getconf GNU_LIBC_VERSION, ldd --version, ldd `which ls` | grep libc
    #define _GNU_SOURCE // pthread_getattr_np glibc 2.2.3, __GLIBC__ __GLIBC_MINOR__
    #include <pthread.h> // pthread_create POSIX.1-2008
    #include <unistd.h> // sysconf confstr POSIX.1-2008
    #include <errno.h> // ETIMEDOUT
    #ifdef PRH_CONC_TEST
    #include <sys/resource.h> // getrlimit POSIX.1-2008
    #endif
#endif
#include <stdatomic.h>
#include <assert.h> // assert
#include <stdlib.h> // malloc calloc realloc free exit
#include <string.h> // memcpy memove memset
// void *memcpy(void *dest, const void *src, size_t num);
// void *memmove(void *dest, const void *src, size_t num);
// void *memset(void *ptr, int value, size_t num);
// if either dest or src is an invalid or null pointer { undefined behavior }
// if dest and src memory overlap for memcpy { undefined behavior }
// the num parameter can be set to zero value.
#include <stdio.h> // printf fprintf
#endif

#ifdef PRH_IMPLEMENTATION
#include <stdio.h>
void prh_impl_prerr(int err, int line) {
    fprintf(stderr, "error: %d at %d\n", err, line);
}
#endif

#ifdef PRH_ARRAY_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#endif

#ifdef PRH_CORO_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#endif

#ifndef PRH_GLIBC_VERSION
#if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
    #define PRH_GLIBC_VERSION (__GLIBC__ * 100 + __GLIBC_MINOR__)
#endif
#endif

// ARRAYS - Very simple single file style library for array data structures
//
//  * fixed runtime length array
//              [len][elem][elem]...
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
//      int *fixed_array;
//   };
//   struct dyn_ints {
//      nt *dynamic_array;
//   };
//   struct dea_ints {
//      int *double_ended_array;
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
//   array_push_elts(&dyn_ints, elts, PRH_ARRAY_SIZE(elts));
//
//   array_pop(&dyn_ints);
//   array_pop_elts(&dyn_ints, 2);
//
//   array_len(&dyn_ints);
//   array_clear(&dyn_ints);
//   array_free(&dyn_ints);
#ifdef PRH_ARRAY_INCLUDE

#ifndef PRH_ARRAY_INIT_ELEMS
#define PRH_ARRAY_INIT_ELEMS 4
#endif

#ifndef PRH_ARRAY_SIZE_EXPAND // if <= 0 { cap *= 2 } else { cap += EXPAND }
#define PRH_ARRAY_SIZE_EXPAND 0
#endif

struct prh_impl_array_header {
    prh_signed cap;
    prh_signed len;
};

#define prh_impl_array_elt_bytes(p) sizeof(*((p)->dynamic_array))
#define prh_impl_array_get_header(p) ((p)->dynamic_array ? (((struct prh_impl_array_header *)(p)->dynamic_array) - 1) : prh_null)

prh_inline prh_signed prh_impl_array_cap(void *elt_ptr) {
    return elt_ptr ? (((struct prh_impl_array_header *)elt_ptr) - 1)->cap : 0;
}

prh_inline prh_signed prh_impl_array_len(void *elt_ptr) {
    return elt_ptr ? (((struct prh_impl_array_header *)elt_ptr) - 1)->len : 0;
}

prh_inline void prh_impl_array_clear(void *elt_ptr) {
    if (elt_ptr) {
        (((struct prh_impl_array_header *)elt_ptr) - 1)->len = 0;
    }
}

#define prh_impl_array_expand(prh_p, elt_count) \
    prh_signed prh_new_elts = (prh_signed)(elt_count); \
    struct prh_impl_array_header *prh_h = prh_impl_array_get_header(prh_p); \
    if (prh_h == prh_null || prh_h->len + prh_new_elts > prh_h->cap) { \
        prh_signed prh_len = 0, prh_cap = PRH_ARRAY_INIT_ELEMS; \
        if (prh_h) { prh_len = prh_h->len; prh_cap = prh_h->cap; } \
        prh_cap = PRH_ARRAY_SIZE_EXPAND > 0 ? (prh_cap + PRH_ARRAY_SIZE_EXPAND) : (prh_cap * 2); \
        if (prh_len + prh_new_elts > prh_cap) prh_cap = prh_len + prh_new_elts; \
        prh_h = prh_realloc(prh_h, sizeof(struct prh_impl_array_header) + prh_cap * prh_impl_array_elt_bytes(prh_p)); \
        assert(prh_h != prh_null && "oops memory overflow"); prh_h->cap = prh_cap; prh_h->len = prh_len; \
        prh_p->dynamic_array = (prh_typeof(prh_p->dynamic_array))(prh_h + 1); \
    }

#define prh_array_free(arr_ptr) \
    do { \
        prh_typeof(arr_ptr) prh_p = (arr_ptr); \
        if (prh_p->dynamic_array) \
        { \
            prh_free(prh_impl_array_get_header(prh_p)); \
            prh_p->dynamic_array = prh_null; \
        } \
    } while (0)

#define prh_array_expand(arr_ptr, num_elts) \
    do { \
        prh_typeof(arr_ptr) prh_p = (arr_ptr); \
        prh_signed prh_n = (prh_signed)(num_elts); assert(prh_n > 0); \
        prh_impl_array_expand(prh_p, prh_n); \
    } while (0)

#define prh_array_push(arr_ptr, arr_elt_value) \
    do { \
        prh_typeof(arr_ptr) prh_p = (arr_ptr); \
        prh_impl_array_expand(prh_p, 1); \
        prh_p->dynamic_array[prh_h->len++] = (arr_elt_value); \
    } while (0)

#define prh_array_push_elt(arr_ptr, arr_elt_value_ptr) \
    do { \
        prh_typeof(arr_ptr) prh_p = (arr_ptr); \
        prh_impl_array_expand(prh_p, 1); \
        prh_p->dynamic_array[prh_h->len++] = *(arr_elt_value_ptr); \
    } while (0)

#define prh_array_push_elts(arr_ptr, arr_elt_ptr, num_elts) \
    do { \
        prh_typeof(arr_ptr) prh_p = (arr_ptr); prh_signed prh_n = (num_elts); \
        prh_typeof(prh_p->dynamic_array) prh_elt_ptr = (arr_elt_ptr); \
        if (prh_elt_ptr && prh_n > 0) { \
            prh_impl_array_expand(prh_p, prh_n); \
            memcpy(p->dynamic_array + prh_h->len, prh_elt_ptr, prh_n * prh_impl_array_elt_bytes(prh_p)); \
            h->len += prh_n; \
        } \
    } while (0)

#define prh_array_del(arr_ptr, i) \
    do { \
        prh_typeof(arr_ptr) prh_p = (arr_ptr); prh_signed prh_i = (prh_signed)(i); \
        struct prh_impl_array_header *prh_h = prh_impl_array_get_header(prh_p); \
        if (prh_h && prh_i >= 0 && prh_i < prh_h->len) { \
            prh_h->len -= 1; unsigned prh_signed prh_move_bytes = (prh_h->len - prh_i) * prh_impl_array_elt_bytes(prh_p); \
            memmove(prh_p->dynamic_array + prh_i, prh_p->dynamic_array + prh_i + 1, prh_move_bytes); \
        } \
    } while (0)

#define prh_array_pop_elts(arr_ptr, n) \
    do { \
        prh_typeof(arr_ptr) prh_p = (arr_ptr); prh_signed prh_elts = (prh_signed)(n); \
        struct prh_impl_array_header *prh_h = prh_impl_array_get_header(prh_p); \
        assert(prh_h && prh_elts >= 0 && prh_elts <= prh_h->len); prh_h->len -= prh_elts; \
    } while (0)

#define prh_array_pop(arr_ptr) prh_array_pop_elts((arr_ptr), 1)
#define prh_array_clear(arr_ptr) prh_impl_array_clear((arr_ptr)->dynamic_array)
#define prh_array_cap(arr_ptr) prh_impl_array_cap((arr_ptr)->dynamic_array)
#define prh_array_len(arr_ptr) prh_impl_array_len((arr_ptr)->dynamic_array)
#define prh_array_end(arr_ptr) ((arr_ptr)->dynamic_array + prh_array_len(arr_ptr))
#define prh_array_get(arr_ptr, i) ((arr_ptr)->dynamic_array[i])
#define prh_array_elt_ptr(arr_ptr, i) ((arr_ptr)->dynamic_array + (i))

#ifdef PRH_ARRAY_STRIP_PREFIX
#define array_free          prh_array_free
#define array_expand        prh_array_expand
#define array_push          prh_array_push
#define array_push_elt      prh_array_push_elt
#define array_push_elts     prh_array_push_elts
#define array_pop           prh_array_pop
#define array_pop_elts      prh_array_pop_elts
#define array_del           prh_array_del
#define array_clear         prh_array_clear
#define array_cap           prh_array_cap
#define array_len           prh_array_len
#define array_end           prh_array_end
#define array_get           prh_array_get
#define array_elt_ptr       prh_array_elt_ptr
#endif // PRH_ARRAY_STRIP_PREFIX
#endif // PRH_ARRAY_INCLUDE

#ifdef PRH_CONC_INCLUDE
#define PRH_LIST_INCLUDE
#define PRH_LIST_QUEUE_INCLUDE
#ifdef PRH_CONC_IMPLEMENTATION
    #define PRH_LIST_IMPLEMENTATION
#endif
#endif

#ifdef PRH_LIST_INCLUDE
typedef struct prh_snode {
    struct prh_snode *next;
} prh_snode_t;

typedef struct prh_xnode {
    struct prh_xnode *both; // both = prev XOR next
} prh_xnode_t;

typedef struct prh_node {
    struct prh_node *next;
    struct prh_node *prev;
} prh_node_t;

#ifdef PRH_LIST_QUEUE_INCLUDE
// Dynamic allocated link list queue. Each node can have different size, but
// must cotain prh_snode_t as the header.
//
//  struct ListQueueNode {
//      prh_snode_t snode_head; // must be 1st field
//      ... other custom node data ...
//  };

typedef struct { // zero initialize
    prh_snode_t head;
    prh_snode_t *tail;
} prh_list_queue_t;

prh_inline void prh_list_queue_init(prh_list_queue_t *q) {
    q->head.next = prh_null;
    q->tail = prh_null;
}

prh_inline bool prh_list_queue_empty(prh_list_queue_t *q) {
    return q->head.next == prh_null;
}

void prh_list_queue_clear(prh_list_queue_t *q, void (*node_free_func)(void *));
prh_list_queue_t prh_list_queue_move(prh_list_queue_t *q);
void prh_list_queue_push(prh_list_queue_t *q, prh_snode_t *new_node);
prh_snode_t *prh_list_queue_pop(prh_list_queue_t *q);

#ifdef PRH_LIST_STRIP_PREFIX
#define list_queue_t        prh_list_queue_t
#define list_queue_init     prh_list_queue_init
#define list_queue_empty    prh_list_queue_empty
#define list_queue_clear    prh_list_queue_clear
#define list_queue_move     prh_list_queue_move
#define list_queue_push     prh_list_queue_push
#define list_queue_pop      prh_list_queue_pop
#endif // PRH_LIST_STRIP_PREFIX

#ifdef PRH_LIST_IMPLEMENTATION
void prh_list_queue_clear(prh_list_queue_t *q, void (*node_free_func)(void *)) {
    if (node_free_func) {
        prh_snode_t *node = q->head.next;
        for (; node; node = node->next) {
            node_free_func(node);
        }
    }
    prh_list_queue_init(q);
}

prh_list_queue_t prh_list_queue_move(prh_list_queue_t *q) {
    prh_list_queue_t que = *q;
    prh_list_queue_init(q);
    return que;
}

void prh_list_queue_push(prh_list_queue_t *q, prh_snode_t *new_node) {
    prh_snode_t *tail = q->tail;
    if (tail == prh_null) tail = &q->head;
    q->tail = tail->next = new_node;
    new_node->next = prh_null;
}

prh_snode_t *prh_list_queue_pop(prh_list_queue_t *q) {
    prh_snode_t *top = q->head.next;
    if (top == prh_null) return prh_null;
    q->head.next = top->next;
    if (top == q->tail) {
        q->tail = prh_null;
    }
    return top;
}
#endif

// Dynamic allocated link list queue. The node can contain any object and with
// different size. If the size < sizeof(void *), the size is rounded up to
// sizeof(void *).
typedef struct {
    prh_snode_t head;
    prh_snode_t *tail;
} prh_size_queue_t;

#endif // PRH_LIST_QUEUE_INCLUDE
#endif // PRH_LIST_INCLUDE

// COROUTINES - Very simple single file style coroutine library
#ifdef PRH_CORO_INCLUDE

typedef struct prh_coro prh_coro_t;
typedef struct prh_coro_struct prh_coro_struct;
typedef struct prh_impl_coro_main prh_impl_coro_main_t;

typedef struct {
    prh_coro_struct *chain; // 1st field dont move
    prh_coro_t *coro; // 2nd field dont move
    prh_impl_coro_main_t *main; // 3rd field dont move
} prh_solo_struct;

#define prh_coro_proc prh_fastcall(void)
prh_fastcall_typedef(void, prh_coroproc_t)(prh_coro_t *);

prh_fastcall(bool) prh_impl_asm_coro_yield(prh_coro_t *coro, prh_coro_t *next);
prh_coro_t *prh_impl_next_resume_coro(void);

void *prh_coroutine_userdata(prh_coro_t *coro);
int prh_coroutine_id(prh_coro_t *coro);

prh_inline void prh_coroutine_yield(prh_coro_t *coro) {
    prh_impl_asm_coro_yield(coro, prh_impl_next_resume_coro());
}

prh_inline void *prh_coroutine_main(prh_coro_struct *main) {
    return ((prh_solo_struct *)main)->main;
}

int prh_coroutine_alloc_size(int maxcoros);
prh_coro_struct *prh_coroutine_init_inplace(void *addr, int maxcoros); // addr shall be previously zero initialized
prh_coro_struct *prh_coroutine_init(int maxcoros);
void prh_coroutine_create(prh_coro_struct *main, prh_coroproc_t proc, int stack_size, void *userdata);
void *prh_coroutine_ext_create(prh_coro_struct *main, prh_coroproc_t proc, int stack_size, int userdata_bytes);

bool prh_coroutine_start_cycle(prh_coro_struct *main);
bool prh_coroutine_start(prh_coro_struct *main, int index);
void prh_coroutine_reload(prh_coro_struct *main, int index, prh_coroproc_t proc);
void prh_coroutine_ext_reload(prh_coro_struct *main, int index, prh_coroproc_t proc, void *userdata);
void prh_coroutine_finish(prh_coro_struct **main);

void prh_solo_create(prh_solo_struct *main, prh_coroproc_t proc, int stack_size, void *userdata);
void *prh_solo_ext_create(prh_solo_struct *main, prh_coroproc_t proc, int stack_size, int maxudsize);
bool prh_solo_start(prh_solo_struct *main);
void prh_solo_reload(prh_solo_struct *main, prh_coroproc_t proc);
void prh_solo_ext_reload(prh_solo_struct *main, prh_coroproc_t proc, void *userdata);
void prh_solo_finish(prh_solo_struct *main);

#ifdef PRH_CORO_STRIP_PREFIX
#define coro_t                  prh_coro_t
#define coro_struct             prh_coro_struct
#define solo_struct             prh_solo_struct
#define coro_proc               prh_coro_proc
#define coroutine_userdata      prh_coroutine_userdata
#define coroutine_id            prh_coroutine_id
#define coroutine_yield         prh_coroutine_yield
#define coroutine_main          prh_coroutine_main
#define coroutine_alloc_size    prh_coroutine_alloc_size
#define coroutine_init_inplace  prh_coroutine_init_inplace
#define coroutine_init          prh_coroutine_init
#define coroutine_create        prh_coroutine_create
#define coroutine_ext_create    prh_coroutine_ext_create
#define coroutine_start_cycle   prh_coroutine_start_cycle
#define coroutine_start         prh_coroutine_start
#define coroutine_reload        prh_coroutine_reload
#define coroutine_ext_reload    prh_coroutine_ext_reload
#define coroutine_finish        prh_coroutine_finish
#define solo_create             prh_solo_create
#define solo_ext_create         prh_solo_ext_create
#define solo_start              prh_solo_start
#define solo_reload             prh_solo_reload
#define solo_ext_reload         prh_solo_ext_reload
#define solo_finish             prh_solo_finish
#endif // PRH_CORO_STRIP_PREFIX

#ifdef PRH_CORO_IMPLEMENTATION

#ifndef PRH_CORO_DEBUG
#define PRH_CORO_DEBUG PRH_DEBUG
#endif

#define prh_lower_guard_word 0x5a5a5a5a
#define prh_upper_guard_word 0xa5a5a5a5

#if defined(prh_cl_msc)
    #define prh_aligned_malloc(size) _aligned_malloc(size, 16)
    #define prh_aligned_free(p) _aligned_free(p)
#elif defined(prh_cl_gnu)
    void *aligned_alloc(size_t alignment, size_t size);
    #define prh_aligned_malloc(size) aligned_alloc(16, size)
    #define prh_aligned_free(p) free(p)
#else
    #error unsupported compiler
#endif

prh_fastcall(int) prh_impl_asm_stack_init_depth(void);
prh_fastcall(void) prh_impl_asm_coro_call(void); // args are on coro stack

void prh_impl_coro_stack_segmentation_fault(prh_coro_t *coro) {
    printf("coro %p stack segmentation fault\n", (void *)coro);
    exit(__LINE__);
}

// coroutine stack layout:
//
//  lower memery address
//  [coroutine userdata area    ]
//  [prh_impl_coro_guard_t      ]
//  [       ...                 ]
//  [       ...                 ]          ^
//  [       ...                 ]          ^
//  [stack memory data          ] <-- rsp  | <-- stack top
//  [stack memory data          ]
//  [stack memory data          ] <-- stack bottom
//  [prh_coro_t: rspoffset      ] <-- coro
//  upper memory address
struct prh_coro {
    prh_u32 rspoffset; // 1st field dont move
    prh_u32 loweraddr;
    prh_u32 maxudsize: 31, ptr_param: 1;
    prh_int coro_id;
};

struct prh_impl_coro_main {
    prh_u32 rspoffset; // 1st field dont move
    prh_u32 yield_cycle;
};

typedef struct {
    prh_u32 lower_guard_word;
    prh_u32 upper_guard_word;
} prh_impl_coro_guard_t;

prh_inline void *prh_impl_coro_udata(prh_coro_t *coro) {
    return (char *)coro - coro->loweraddr;
}

prh_inline prh_impl_coro_guard_t *prh_impl_coro_guard(prh_coro_t *coro) {
    void *p = prh_impl_coro_udata(coro);
    return (prh_impl_coro_guard_t *)((char *)p + coro->maxudsize);
}

void *prh_coroutine_userdata(prh_coro_t *coro) {
    void *p = prh_impl_coro_udata(coro);
    if (coro->ptr_param) return *(void **)p;
    return ((int)coro->maxudsize > 0) ? p : prh_null;
}

int prh_coroutine_id(prh_coro_t *coro) {
    return coro->coro_id;
}

prh_static_assert(sizeof(prh_coro_t) <= 16);

// prh_coro_struct layout:
//  [prh_coro_t_struct]
//  [prh_coro_t *]
//  [prh_coro_t *]
//  ...
struct prh_coro_struct {
    prh_coro_struct *chain; // 1st field dont move
    prh_coro_t *coro; // 2nd field dont move
    prh_impl_coro_main_t *main; // 3rd field dont move
    prh_u32 maxcoros: 31, inplace: 1;
    prh_int coro_cnt;
};

prh_inline int prh_impl_coro_struct_alloc_size(int maxcoros) {
    return (int)(sizeof(prh_coro_struct) + maxcoros * sizeof(void *));
}

int prh_coroutine_alloc_size(int maxcoros) {
    assert(maxcoros >= 0);
    return prh_impl_coro_struct_alloc_size(maxcoros);
}

prh_coro_struct *prh_coroutine_init_inplace(void *addr, int maxcoros) {
    assert(maxcoros >= 0);
    prh_coro_struct *s = (prh_coro_struct *)addr;
    s->maxcoros = maxcoros;
    s->inplace = 1;
    return s;
}

prh_coro_struct *prh_coroutine_init(int maxcoros) {
    int alloc_size = prh_coroutine_alloc_size(maxcoros);
    prh_coro_struct *s = (prh_coro_struct *)prh_calloc(alloc_size);
    prh_coroutine_init_inplace(s, maxcoros);
    s->inplace = 0;
    return s;
}

void prh_impl_guard_coro_stack(prh_coro_t *coro) {
    prh_impl_coro_guard_t *g = prh_impl_coro_guard(coro);
    if (g->lower_guard_word != prh_lower_guard_word || g->upper_guard_word != prh_upper_guard_word) {
        prh_impl_coro_stack_segmentation_fault(coro);
    }
}

prh_inline int prh_impl_coro_self_size(int maxudsize) {
    return 16 + prh_round_ptrsize(maxudsize) + sizeof(prh_impl_coro_guard_t); // 16 for prh_coro_t
}

prh_coro_t *prh_impl_coro_alloc(int stack_size, int userdata_bytes) {
    if (userdata_bytes < 0) userdata_bytes = 0;

    assert(stack_size > prh_impl_coro_self_size(userdata_bytes) + prh_impl_asm_stack_init_depth());

    int stackallocsize = prh_round_16_byte(stack_size);
    char *p = (char *)prh_aligned_malloc(stackallocsize);
    prh_coro_t *coro = (prh_coro_t *)(p + stackallocsize - 16);
    coro->rspoffset = 0;
    coro->loweraddr = (prh_u32)((char *)coro - p);
    coro->maxudsize = prh_round_ptrsize(userdata_bytes);
    coro->ptr_param = 0;

    prh_impl_coro_guard_t *guard = prh_impl_coro_guard(coro);
    guard->lower_guard_word = prh_lower_guard_word;
    guard->upper_guard_word = prh_upper_guard_word;
    return coro;
}

prh_coro_t **prh_impl_coro_list(prh_coro_struct *main) {
    return (prh_coro_t **)(main + 1);
}

prh_coro_t *prh_impl_coroutine_alloc(prh_coro_struct *main, int stack_size, int userdata_bytes) {
    if ((prh_u32)main->coro_cnt >= main->maxcoros) return prh_null;
    prh_coro_t *coro = prh_impl_coro_alloc(stack_size, userdata_bytes);
    prh_impl_coro_list(main)[main->coro_cnt] = coro;
    main->coro_cnt += 1;
    coro->coro_id = main->coro_cnt;
    return coro;
}

#define PRH_IMPL_CORO_INIT_PTR_PARAM 1
#define PRH_IMPL_CORO_ZERO_USER_DATA 2
#define PRH_IMPL_CORO_UPDT_PTR_PARAM 3

void prh_impl_coro_reset_udata(prh_coro_t *coro, void *userdata, int cond) {
    void *p = prh_impl_coro_udata(coro);
    if ((int)coro->maxudsize <= 0) return;
    if (cond == PRH_IMPL_CORO_INIT_PTR_PARAM) {
        coro->ptr_param = 1;
        *(void **)p = userdata;
    } else if (cond == PRH_IMPL_CORO_ZERO_USER_DATA) {
        memset(p, 0, coro->maxudsize);
    } else if (cond == PRH_IMPL_CORO_UPDT_PTR_PARAM) {
        if (coro->ptr_param) {
            *(void **)p = userdata;
        }
    }
}

void *prh_impl_coroutine_load(prh_coro_t *coro, prh_coroproc_t proc, void *userdata, int cond) {
    if (coro == prh_null) return prh_null;

    prh_impl_guard_coro_stack(coro);

    prh_impl_coro_reset_udata(coro, userdata, cond);

    void **rsp = (void **)coro;
    void *rspaligned_to_16 = (void *)rsp;
    // [32-bit architecture]
    // pstack + alloc <-- 00 <-- 16字节对齐
    //           proc <-- 12
    //                <-- 08 prh_impl_asm_coro_call
    // [64-bit architecture]
    // pstack + alloc <-- 00 <-- 16字节对齐
    //           proc <-- 08
    //                <-- 00 prh_impl_asm_coro_call
    *(--rsp) = (void *)(prh_unsigned)proc;
    *(--rsp) = (void *)(prh_unsigned)prh_impl_asm_coro_call;
    coro->rspoffset = (prh_u32)((char *)coro - (char *)rsp);

    assert((prh_unsigned)rspaligned_to_16 % 16 == 0);

#if PRH_CORO_DEBUG
    void *udata = prh_impl_coro_udata(coro);
    rsp = (void **)((char *)rspaligned_to_16 - prh_impl_asm_stack_init_depth());
    printf("[coro] %p #%d created %08x left %08x rsp %p depth %08x high %p proc %p\n",
        (void *)coro, coro->coro_id, (int)(coro->maxudsize + sizeof(prh_impl_coro_guard_t)),
        (int)((char *)rsp - (char *)udata), (void *)rsp,
        (int)((char *)rspaligned_to_16 + 16 - (char *)rsp),
        (char *)rspaligned_to_16 + 16, (void *)(prh_unsigned)proc);
#endif

    return prh_coroutine_userdata(coro);
}

prh_coro_t *prh_coro_create(int coro_id, prh_coroproc_t proc, int stack_size, void *userdata) {
    prh_coro_t *coro = prh_impl_coro_alloc(stack_size, sizeof(void *));
    if (coro) coro->coro_id = coro_id;
    prh_impl_coroutine_load(coro, proc, userdata, PRH_IMPL_CORO_INIT_PTR_PARAM);
    return coro;
}

prh_coro_t *prh_coro_ext_create(int coro_id, prh_coroproc_t proc, int stack_size, int userdata_bytes) {
    prh_coro_t *coro = prh_impl_coro_alloc(stack_size, userdata_bytes);
    if (coro) coro->coro_id = coro_id;
    prh_impl_coroutine_load(coro, proc, prh_null, PRH_IMPL_CORO_ZERO_USER_DATA);
    return coro;
}

void prh_coro_reload(prh_coro_t *coro, prh_coroproc_t proc) {
    if (coro && coro->rspoffset == 0) {
        prh_impl_coroutine_load(coro, proc, prh_null, 0);
    }
}

void prh_coro_ext_reload(prh_coro_t *coro, prh_coroproc_t proc, void *userdata) {
    if (coro && coro->rspoffset == 0) {
        prh_impl_coroutine_load(coro, proc, userdata, PRH_IMPL_CORO_UPDT_PTR_PARAM);
    }
}

void prh_coroutine_create(prh_coro_struct *main, prh_coroproc_t proc, int stack_size, void *userdata) {
    prh_coro_t *coro = prh_impl_coroutine_alloc(main, stack_size, sizeof(void *));
    prh_impl_coroutine_load(coro, proc, userdata, PRH_IMPL_CORO_INIT_PTR_PARAM);
}

void *prh_coroutine_ext_create(prh_coro_struct *main, prh_coroproc_t proc, int stack_size, int userdata_bytes) {
    prh_coro_t *coro = prh_impl_coroutine_alloc(main, stack_size, userdata_bytes);
    return prh_impl_coroutine_load(coro, proc, prh_null, PRH_IMPL_CORO_ZERO_USER_DATA);
}

prh_coro_t *prh_impl_coro_get(prh_coro_struct *main, int index) {
    assert(index > 0 && index <= main->coro_cnt);
    return prh_impl_coro_list(main)[index - 1];
}

void prh_coroutine_reload(prh_coro_struct *main, int index, prh_coroproc_t proc) {
    prh_coro_t *coro = prh_impl_coro_get(main, index);
    prh_coro_reload(coro, proc);
}

void prh_coroutine_ext_reload(prh_coro_struct *main, int index, prh_coroproc_t proc, void *userdata) {
    prh_coro_t *coro = prh_impl_coro_get(main, index);
    prh_coro_ext_reload(coro, proc, userdata);
}

// 跨线程协程在每个线程中都有一个主协程，每个线程可以有多个主协程，不管是线程主动创建多
// 个主协程，还是一个主协程涉及的子协程创建了主协程，一个线程同一时间只会处理一个主协程
// 及其关联的子协程组合，而且只有当最深层的组合处理完毕才会处理浅层的组合。
prh_thread_local prh_coro_struct *PRH_IMPL_CORO_CHAIN = prh_null;

void prh_impl_push_main_coro(prh_coro_struct *main) {
    main->chain = PRH_IMPL_CORO_CHAIN;
    PRH_IMPL_CORO_CHAIN = main;
}

prh_coro_struct *prh_impl_pop_main_coro(void) {
    prh_coro_struct *top = PRH_IMPL_CORO_CHAIN;
    PRH_IMPL_CORO_CHAIN = top->chain;
    return top;
}

prh_coro_t *prh_impl_get_next_resume(prh_coro_struct *s, int start_index) {
    for (int i = start_index; i < s->coro_cnt; i += 1) {
        prh_coro_t *next = prh_impl_coro_list(s)[i];
        if (next && next->rspoffset) {
            return next;
        }
    }
    return (prh_coro_t *)s->main;
}

prh_coro_t *prh_impl_next_resume_coro(void) {
    prh_coro_struct *top = PRH_IMPL_CORO_CHAIN;
    prh_impl_coro_main_t *main = top->main;
    prh_coro_t *curr = top->coro;
    if (main->yield_cycle == 0) return (prh_coro_t *)main;
    prh_coro_t *next = prh_impl_get_next_resume(top, curr->coro_id);
    if (next != (prh_coro_t *)main) {
        top->coro = next; // dont move out, solo need coro unchanged
    }
    return next;
}

bool prh_impl_coro_main_yield(prh_coro_struct *main, prh_coro_t *next) {
    prh_impl_push_main_coro(main);
    main->coro = next;
    bool result = prh_impl_asm_coro_yield((prh_coro_t *)main->main, next);
    prh_impl_guard_coro_stack(next);
    prh_impl_pop_main_coro();
    return result;
}

prh_fastcall(void *) prh_impl_asm_coro_finish(prh_coro_t *coro) {
    coro->rspoffset = 0;
    prh_impl_guard_coro_stack(coro);
    return prh_impl_next_resume_coro();
}

bool prh_coroutine_start_cycle(prh_coro_struct *main) {
    prh_impl_coro_main_t mainstack;
    main->main = &mainstack;
    mainstack.yield_cycle = 1;
    prh_coro_t *next = prh_impl_get_next_resume(main, 0);
    if (next == (prh_coro_t *)&mainstack) {
        return false;
    }
    return prh_impl_coro_main_yield(main, next);
}

bool prh_coroutine_start(prh_coro_struct *main, int index) {
    prh_impl_coro_main_t mainstack;
    main->main = &mainstack;
    mainstack.yield_cycle = 0;
    prh_coro_t *next = prh_impl_coro_get(main, index);
    if (next == prh_null || next->rspoffset == 0) {
        return false;
    }
    return prh_impl_coro_main_yield(main, next);
}

prh_inline void prh_impl_coro_free(prh_coro_t *coro) {
    prh_aligned_free(prh_impl_coro_udata(coro));
}

void prh_coroutine_finish(prh_coro_struct **main) {
    prh_coro_struct *s = *main;
    if (s == prh_null) return;
    prh_coro_t **start = prh_impl_coro_list(s);
    for (int i = 0; i < (int)s->coro_cnt; i += 1) {
        prh_coro_t *coro = start[i];
        if (coro) {
            prh_impl_coro_free(coro);
            start[i] = prh_null;
        }
    }
    *main = prh_null;
    if (!s->inplace) {
        prh_free(s);
    }
}

void prh_solo_create(prh_solo_struct *main, prh_coroproc_t proc, int stack_size, void *userdata) {
    if (main->coro) return; // already initialized
    main->coro = prh_coro_create(1, proc, stack_size, userdata);
}

void *prh_solo_ext_create(prh_solo_struct *main, prh_coroproc_t proc, int stack_size, int maxudsize) {
    if (main->coro) return prh_null; // already initialized
    prh_coro_t *solo = prh_coro_ext_create(1, proc, stack_size, maxudsize);
    if (solo == prh_null) return prh_null;
    main->coro = solo;
    return coroutine_userdata(solo);
}

void prh_solo_reload(prh_solo_struct *main, prh_coroproc_t proc) {
    prh_coro_reload(main->coro, proc);
}

void prh_solo_ext_reload(prh_solo_struct *main, prh_coroproc_t proc, void *userdata) {
    prh_coro_ext_reload(main->coro, proc, userdata);
}

bool prh_solo_start(prh_solo_struct *main) {
    prh_impl_coro_main_t mainstack;
    main->main = &mainstack;
    mainstack.yield_cycle = 0;
    prh_coro_t *next = main->coro;
    if (next == prh_null || next->rspoffset == 0) {
        return false;
    }
    return prh_impl_coro_main_yield((prh_coro_struct *)main, next);
}

void prh_solo_finish(prh_solo_struct *main) {
    prh_coro_t *solo = main->coro;
    if (solo == prh_null) return;
    prh_impl_coro_free(solo);
    main->coro = prh_null;
}
#endif // PRH_CORO_IMPLEMENTATION
#endif // PRH_CORO_INCLUDE

// CONCURRENCY - Very simple single file style concurrency library
#ifdef PRH_CONC_INCLUDE

// 当多个线程访问一个原子对象时，所有的原子操作都会针对该原子对象产生明确的行为：在任
// 何其他原子操作能够访问该对象之前，每个原子操作都会在该对象上完整地执行完毕。这就保
// 证了在这些对象上不会出现数据竞争，而这也正是定义原子性的关键特征。
#define prh_atombool_t _Atomic bool
#define prh_atomint_t _Atomic int
#define prh_atomptr_t _Atomic uintptr_t

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
#define prh_atomtype_init(a, v) atomic_init((a), (v))

// Atomically perform load (atomic read) operation and return the value.
// The default memory_order type is memory_order_seq_cst.
#define prh_atomtype_load(a) atomic_load((a)) // return read value

// Atomically perform write or read-modify-write operation.
// The default memory_order type is memory_order_seq_cst.
#define prh_atomtype_store(a, v) atomic_store((a), (v)) // return void
#define prh_atomtype_exchange(a, v) atomic_exchange((a), (v)) // return old value

// Atomically perform compare-and-exchange (or compare-and-swap CAS)
// operation.
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
#define prh_atomtype_cas(a, expect_inout, new_value) \
    atomic_compare_exchange_weak((a), (expect_inout), (new_value))

// Atomically perform read-modify-wirte operation, and return the old value.
// The modify operation can be add (+) sub (-) and (&) or (|) xor (^).
// The default memory_order type is memory_order_seq_cst.
#define prh_atomint_inc(a) atomic_fetch_add((a), 1)
#define prh_atomint_dec(a) atomic_fetch_sub((a), 1)

// Dynamic allocated link list atomic queue for only 1 producer and 1 consumer.
// The queue node size is fixed, and must cotain prh_snode_t as the header.
//
//  struct QueueNode {
//      prh_snode_t snode_head; // must be 1st field
//      ... queue node other custom data ...
//  };
//
//  struct StructContainQueue {
//      ...
//      QueueNode *<PREFIX>_atomlistque_head; // only R/W by consumer
//      ...
//      QueueNode *<PREFIX>_atomlistque_tail; // only R/W by producer
//      ...
//      prh_atomint_t <PREFIX>_atomlistque_len; // R/W by both
//      ...
//  };
//
//  StructContainQueue que;
//  prh_atomlistque_init(&que, PREFIX);
//  QueueNode *node = prh_atomlistque_node(&que, PREFIX);
//  ... initializing the new node ...
//  prh_atomlistque_push(&que, PREFIX); // push a new node
//  QueueNode *top = prh_atomlistque_top(&que, PREFIX);
//  ... using the top node ...
//  prh_atomlistque_pop(&que, PREFIX); // pop off and free top node
//  prh_atomlistque_free(&que, PREFIX, node_deinit_func); // free all nodes
#define prh_impl_atomlistque_head(PREFIX) prh_impl_que->prh_macro_concat_name(PREFIX, _atomlistque_head)
#define prh_impl_atomlistque_tail(PREFIX) prh_impl_que->prh_macro_concat_name(PREFIX, _atomlistque_tail)
#define prh_impl_atomlistque_len(PREFIX) prh_impl_que->prh_macro_concat_name(PREFIX, _atomlistque_len)
void prh_impl_atomlistque_free(prh_snode_t *head, void (*node_deinit_func)(void *node));

#define prh_atomlistque_init(ptr_struct_contain_queue, PREFIX) { \
    prh_typeof(ptr_struct_contain_queue) prh_impl_que = (ptr_struct_contain_queue); \
    typedef prh_typeof(prh_impl_atomlistque_head(PREFIX)) prh_impl_qnode_ptr_t; \
    prh_impl_qnode_ptr_t prh_impl_new_node = prh_calloc(sizeof(*prh_impl_atomlistque_head(PREFIX)); \
    prh_impl_atomlistque_head(PREFIX) = prh_impl_new_node; /* always alloc a null tail node */ \
    prh_impl_atomlistque_tail(PREFIX) = prh_impl_new_node; /* let head pointer always valid */ \
    prh_atomint_t *prh_impl_len = &prh_impl_atomlistque_len(PREFIX); \
    prh_atomtype_init(prh_impl_len, 0); \
}

#define prh_atomlistque_free(ptr_struct_contain_queue, PREFIX, node_deinit_func) { \
    prh_typeof(ptr_struct_contain_queue) prh_impl_que = (ptr_struct_contain_queue); \
    typedef prh_typeof(prh_impl_atomlistque_head(PREFIX)) prh_impl_qnode_ptr_t; \
    prh_impl_qnode_ptr_t *prh_impl_head = prh_impl_atomlistque_head(PREFIX); \
    if (prh_impl_head) { \
        prh_impl_atomlistque_free(&prh_impl_head->snode_head, node_deinit_func); \
        prh_impl_atomlistque_head(PREFIX) = prh_null; \
        prh_impl_atomlistque_tail(PREFIX) = prh_null; \
    } \
}

#define prh_atomlistque_node(ptr_struct_contain_queue, PREFIX) \
    ((ptr_struct_contain_queue)->prh_macro_concat_name(PREFIX, _atomlistque_tail))

#define prh_atomlistque_top(ptr_struct_contain_queue, PREFIX) \
    ((prh_atomtype_load(&(ptr_struct_contain_queue)->prh_macro_concat_name(PREFIX, _atomlistque_len)) <= 0) ? prh_null : \
    ((ptr_struct_contain_queue)->prh_macro_concat_name(PREFIX, _atomlistque_head)))

// push 不会读写 head，也不会读写已经存在的非空节点，tail 总是指向尾部空节点，有效的
// head 总是追不上 tail。push 只会更新 tail 和 tail 空节点，且 push 只被单一生产者
// 调用，因此 tail 不需要原子操作。
#define prh_atomlistque_push(ptr_struct_contain_queue, PREFIX) { \
    prh_typeof(ptr_struct_contain_queue) prh_impl_que = (ptr_struct_contain_queue); \
    typedef prh_typeof(prh_impl_atomlistque_head(PREFIX)) prh_impl_qnode_ptr_t; \
    prh_impl_qnode_ptr_t prh_impl_new_node = prh_calloc(sizeof(*prh_impl_atomlistque_head(PREFIX)); \
    prh_impl_qnode_ptr_t prh_impl_tail = prh_impl_atomlistque_tail(PREFIX); \
    prh_impl_tail->snode_head.next = &prh_impl_new_node->snode_head; \
    prh_impl_atomlistque_tail(PREFIX) = prh_impl_new_node; \
    assert(prh_impl_tail->snode_head.next == &prh_impl_atomlistque_tail(PREFIX)->snode_head); /* allow only one producer */ \
    prh_atomint_t *prh_impl_len = &prh_impl_atomlistque_len(PREFIX); \
    prh_atomint_inc(prh_impl_len); /* 更新len，此步骤执行完毕后以上更新必须对所有cpu生效 */ \
}

// pop 不会读写 tail，也不会读写 tail 空节点，pop 只会更新 head 和读写已经存在的并且
// 有效的头节点，且 pop 只被单一消费者调用，因此 head 不需要原子操作。
#define prh_atomlistque_pop(ptr_struct_contain_queue, PREFIX) { \
    prh_typeof(ptr_struct_contain_queue) prh_impl_que = (ptr_struct_contain_queue); \
    typedef prh_typeof(prh_impl_atomlistque_head(PREFIX)) prh_impl_qnode_ptr_t; \
    prh_atomint_t *prh_impl_len = &prh_impl_atomlistque_len(PREFIX); \
    if (prh_atomtype_load(prh_impl_len) <= 0) return; \
    prh_impl_qnode_ptr_t prh_impl_head = prh_impl_atomlistque_head(PREFIX); \
    prh_impl_qnode_ptr_t prh_impl_head_next = (prh_impl_qnode_ptr_t)prh_impl_head->snode_head.next; \
    prh_impl_atomlistque_head(PREFIX) = prh_impl_head_next; \
    prh_free(prh_impl_head); /* free head node */ \
    assert(prh_impl_head_next == prh_impl_atomlistque_head(PREFIX)); /* allow only one consumer */ \
    prh_atomint_dec(prh_impl_len); /* 更新len，此步骤执行完毕以上更新必须对所有cpu生效 */ \
}

// Dynamic allocated link list atomic queue for only 1 producer and 1 consumer.
// Each node has a prh_snode_t header and a node item pointer. Each node can
// only contain a pointer.
typedef struct prh_atomptrlistque prh_atomptrlistque_t;
prh_atomptrlistque_t *prh_atomptrlistque_init(void);
void prh_atomptrlistque_free(prh_atomptrlistque_t **q, void (*free_item)(void *item));
void prh_atomptrlistque_push(prh_atomptrlistque_t *q, void *item); // item shall not be null
void *prh_atomptrlistque_pop(prh_atomptrlistque_t *q); // return null means empty
void *prh_atomptrlistque_top(prh_atomptrlistque_t *q);
int prh_atomptrlistque_len(prh_atomptrlistque_t *q);

// Fixed array atomptr queue for only 1 producer and 1 consumer
typedef struct prh_atomptrque prh_atomptrque_t;
int prh_atomptrque_alloc_size(int size); // size must be power of 2
prh_atomptrque_t *prh_atomptrque_init_inplace(void *addr, int size);
prh_atomptrque_t *prh_atomptrque_init(int size); // size must be power of 2
void prh_atomptrque_free(prh_atomptrque_t **p);
bool prh_atomptrque_push(prh_atomptrque_t *q, void *v); // v shall not be null
void *prh_atomptrque_pop(prh_atomptrque_t *q); // return null means empty
void *prh_atomptrque_top(prh_atomptrque_t *q);
int prh_atomptrque_len(prh_atomptrque_t *q);
int prh_atomptrque_cap(prh_atomptrque_t *q);

// Fixed array atomint queue for only 1 producer and 1 consumer
typedef struct prh_atomintque prh_atomintque_t;
int prh_atomintque_alloc_size(int size); // size must be power of 2
prh_atomintque_t *prh_atomintque_init_inplace(void *addr, int size);
prh_atomintque_t *prh_atomintque_init(int size); // size must be power of 2
void prh_atomintque_free(prh_atomintque_t **p);
bool prh_atomintque_push(prh_atomintque_t *q, int v); // v shall not be 0
int prh_atomintque_pop(prh_atomintque_t *q); // return 0 means empty
int prh_atomintque_top(prh_atomintque_t *q);
int prh_atomintque_len(prh_atomintque_t *q);
int prh_atomintque_cap(prh_atomintque_t *q);

typedef struct prh_thrd prh_thrd_t;
typedef struct prh_thrd_struct prh_thrd_struct;
typedef int (*prh_thrdproc_t)(prh_thrd_t* thrd);

int prh_thread_id(prh_thrd_t *thrd);
int prh_thread_index(prh_thrd_t *thrd);
void *prh_thread_userdata(prh_thrd_t *thrd);
int prh_thread_count(prh_thrd_struct *s);
prh_thrd_t *prh_thread_main(prh_thrd_struct *s);
prh_thrd_t *prh_thread_get(prh_thrd_struct *s, int index);
prh_thrd_t *prh_thread_self(void);

int prh_thread_alloc_size(int maxthreads, int mainudsize);
prh_thrd_struct *prh_thread_init_inplace(void *addr, int start_id, int maxthreads, int mainudsize); // addr shall be previously zero initialized
prh_thrd_struct *prh_thread_init(int start_id, int maxthreads, int mainudsize);
void *prh_thread_create(prh_thrd_struct *s, prh_thrdproc_t proc, int stacksize, int thrdudsize);
void prh_thread_join(prh_thrd_struct **s, void (*thrd_udata_free)(void *));

typedef struct prh_thrd_mutex prh_thrd_mutex_t;
prh_thrd_mutex_t *prh_thrd_mutex_init(void);
prh_thrd_mutex_t *prh_thrd_recursive_mutex_init(void);
void prh_thrd_mutex_lock(prh_thrd_mutex_t *p);
void prh_thrd_mutex_unlock(prh_thrd_mutex_t *p);
void prh_thrd_mutex_free(prh_thrd_mutex_t **p);

typedef struct prh_thrd_cond prh_thrd_cond_t;
prh_thrd_cond_t *prh_thrd_cond_init(void);
void prh_thrd_cond_wait(prh_thrd_cond_t *p);
bool prh_thrd_cond_timedwait(prh_thrd_cond_t *p, prh_u32 msec);
void prh_thrd_cond_signal(prh_thrd_cond_t *p);
void prh_thrd_cond_broadcast(prh_thrd_cond_t *p);
void prh_thrd_cond_free(prh_thrd_cond_t **p);

#ifdef PRH_CONC_TEST
    void prh_conc_test(void);
#else
    #define prh_conc_test()
#endif

#ifdef PRH_CONC_STRIP_PREFIX
#define thrd_t                  prh_thrd_t
#define thrd_struct             prh_thrd_struct
#define thread_id               prh_thread_id
#define thread_index            prh_thread_index
#define thread_userdata         prh_thread_userdata
#define thread_count            prh_thread_count
#define thread_main             prh_thread_main
#define thread_get              prh_thread_get
#define thread_self             prh_thread_self
#define thread_alloc_size       prh_thread_alloc_size
#define thread_init_inplace     prh_thread_init_inplace
#define thread_init             prh_thread_init
#define thread_create           prh_thread_create
#define thread_join             prh_thread_join
#define thrd_mutex_t            prh_thrd_mutex_t
#define thrd_mutex_init         prh_thrd_mutex_init
#define thrd_recursive_mutex_init prh_thrd_recursive_mutex_init
#define thrd_mutex_lock         prh_thrd_mutex_lock
#define thrd_mutex_unlock       prh_thrd_mutex_unlock
#define thrd_mutex_free         prh_thrd_mutex_free
#define thrd_cond_t             prh_thrd_cond_t
#define thrd_cond_init          prh_thrd_cond_init
#define thrd_cond_wait          prh_thrd_cond_wait
#define thrd_cond_timedwait     prh_thrd_cond_timedwait
#define thrd_cond_signal        prh_thrd_cond_signal
#define thrd_cond_broadcast     prh_thrd_cond_broadcast
#define thrd_cond_free          prh_thrd_cond_free
#define atombool_t              prh_atombool_t
#define atomint_t               prh_atomint_t
#define atomptr_t               prh_atomptr_t
#define atomtype_init           prh_atomtype_init
#define atomtype_load           prh_atomtype_load
#define atomtype_store          prh_atomtype_store
#define atomtype_exchange       prh_atomtype_exchange
#define atomtype_compare_exchange prh_atomtype_compare_exchange
#define atomint_inc             prh_atomint_inc
#define atomint_dec             prh_atomint_dec
#define atomptrlistque_t           prh_atomptrlistque_t
#define atomptrlistque_init        prh_atomptrlistque_init
#define atomptrlistque_free        prh_atomptrlistque_free
#define atomptrlistque_push        prh_atomptrlistque_push
#define atomptrlistque_pop         prh_atomptrlistque_pop
#define atomptrlistque_top         prh_atomptrlistque_top
#define atomptrlistque_len         prh_atomptrlistque_len
#define atomptrque_t               prh_atomptrque_t
#define atomptrque_alloc_size      prh_atomptrque_alloc_size
#define atomptrque_init_inplace    prh_atomptrque_init_inplace
#define atomptrque_init            prh_atomptrque_init
#define atomptrque_free            prh_atomptrque_free
#define atomptrque_push            prh_atomptrque_push
#define atomptrque_pop             prh_atomptrque_pop
#define atomptrque_top             prh_atomptrque_top
#define atomptrque_len             prh_atomptrque_len
#define atomptrque_cap             prh_atomptrque_cap
#define atomintque_t            prh_atomintque_t
#define atomintque_alloc_size   prh_atomintque_alloc_size
#define atomintque_init_inplace prh_atomintque_init_inplace
#define atomintque_init         prh_atomintque_init
#define atomintque_free         prh_atomintque_free
#define atomintque_push         prh_atomintque_push
#define atomintque_pop          prh_atomintque_pop
#define atomintque_top          prh_atomintque_top
#define atomintque_len          prh_atomintque_len
#define atomintque_cap          prh_atomintque_cap
#endif // PRH_CONC_STRIP_PREFIX

#ifdef PRH_CONC_IMPLEMENTATION
#ifndef PRH_THRD_DEBUG
#define PRH_THRD_DEBUG PRH_DEBUG
#endif

#ifndef PRH_CONC_DEBUG
#define PRH_CONC_DEBUG PRH_DEBUG
#endif

#ifdef PRH_CONC_TEST
void prh_impl_atomic_test(void) {
    atomic_flag f; atomic_bool b; atomic_int i; atomic_uint u;
    atomic_intptr_t ip; atomic_uintptr_t up; atomic_size_t sz; atomic_ptrdiff_t pd;
    atomic_char ch; atomic_schar sc; atomic_uchar uc; atomic_short sh;
    atomic_ushort uh; atomic_long sl; atomic_ulong ul; atomic_llong ll; atomic_ullong ull;
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
#endif

void prh_impl_atomlistque_free(prh_snode_t *head, void (*node_deinit_func)(void *item)) {
    while (head != prh_null) {
        prh_snode_t *p = head;
        head = head->next;
        if (node_deinit_func) {
            node_deinit_func(p);
        }
        prh_free(p);
    }
}

typedef struct prh_impl_atomptrlistque_node {
    struct prh_impl_atomptrlistque_node *next;
    void *item;
} prh_impl_atomptrlistque_node_t;

struct prh_atomptrlistque {
    prh_impl_atomptrlistque_node_t *head; // 只由单一消费者读写
    prh_impl_atomptrlistque_node_t *tail; // 只由单一生成者读写
    prh_atomint_t count;
};

prh_atomptrlistque_t *prh_atomptrlistque_init(void) {
    // 永远分配一个tail空节点，让head永远触碰不到tail节点
    prh_impl_atomptrlistque_node_t *tail_null = prh_calloc(sizeof(prh_impl_atomptrlistque_node_t));
    prh_atomptrlistque_t *q = prh_malloc(sizeof(prh_atomptrlistque_t));
    q->head = tail_null;
    q->tail = tail_null;
    prh_atomtype_init(&q->count, 0);
    return q;
}

void prh_atomptrlistque_free(prh_atomptrlistque_t **que, void (*free_item)(void *item)) {
    prh_atomptrlistque_t *q = *que;
    if (q == prh_null) return;
    prh_impl_atomptrlistque_node_t *node = q->head;
    while (node != prh_null) {
        prh_impl_atomptrlistque_node_t *p = node;
        node = node->next;
        if (free_item && p->item) {
            free_item(p->item);
        }
        prh_free(p);
    }
    prh_free(q);
    *que = prh_null;
}

void prh_atomptrlistque_push(prh_atomptrlistque_t *q, void *item) {
    // push不会读写head，也不会读写已经存在的节点
    if (item == prh_null) return; // item shall not be null
    // 分配新NULL节点，tail总指向尾部空节点，不让head追上tail
    prh_impl_atomptrlistque_node_t *null_node = prh_calloc(sizeof(prh_impl_atomptrlistque_node_t));
    prh_impl_atomptrlistque_node_t *tail = q->tail;
    q->tail = null_node;
    // push只会更新tail和tail空节点，且push对应单一生产者，因此tail不需要atom
    tail->next = null_node;
    tail->item = item;
    // 只允许唯一生产者
    assert(tail->next == q->tail);
    // 更新count，此步骤执行完毕以上更新必须对所有cpu生效
    prh_atomint_inc(&q->count);
}

void *prh_atomptrlistque_pop(prh_atomptrlistque_t *q) {
    // pop不会读写tail，也不会读写tail空节点
    if (prh_atomtype_load(&q->count) <= 0) return prh_null;
    prh_impl_atomptrlistque_node_t *head = q->head; // 保存head
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

void *prh_atomptrlistque_top(prh_atomptrlistque_t *q) {
    if (prh_atomtype_load(&q->count) <= 0) return prh_null;
    return q->head->item;
}

int prh_atomptrlistque_len(prh_atomptrlistque_t *q) {
    return prh_atomtype_load(&q->count);
}

#define prh_impl_atomptrque_nextpos(q, pos) (((pos) + 1) & ((q)->u.size - 1))

struct prh_atomptrque {
    union { prh_u32 size: 31, free: 1; void *align; } u;
    prh_atomint_t head;
    prh_atomint_t tail;
};

bool prh_impl_atomptrque_full(prh_atomptrque_t *q) { // is not an atomic operation
    return (prh_impl_atomptrque_nextpos(q, prh_atomtype_load(&q->tail)) == prh_atomtype_load(&q->head));
}

prh_inline int prh_impl_atomptrque_alloc_size(int size, int stride) {
    assert(prh_is_power_of_2(size));
    return prh_round_ptrsize(sizeof(prh_atomptrque_t) + stride * size);
}

int prh_atomptrque_alloc_size(int size) {
    return prh_impl_atomptrque_alloc_size(size, sizeof(void *));
}

int prh_atomintque_alloc_size(int size) {
    return prh_impl_atomptrque_alloc_size(size, sizeof(int));
}

prh_atomptrque_t *prh_atomptrque_init_inplace(void *addr, int size) {
    prh_atomptrque_t *q = (prh_atomptrque_t *)addr;
    q->u.size = size;
    q->u.free = 0;
    prh_atomtype_init(&q->head, 0);
    prh_atomtype_init(&q->tail, 0);
    return q;
}

prh_atomintque_t *prh_atomintque_init_inplace(void *addr, int size) {
    return (prh_atomintque_t *)prh_atomptrque_init_inplace(addr, size);
}

prh_atomptrque_t *prh_atomptrque_init(int size) {
    void *addr = prh_malloc(atomptrque_alloc_size(size));
    prh_atomptrque_t *q = prh_atomptrque_init_inplace(addr, size);
    q->u.free = 1;
    return q;
}

prh_atomintque_t *prh_atomintque_init(int size) {
    void *addr = prh_malloc(prh_atomintque_alloc_size(size));
    prh_atomintque_t *q = prh_atomintque_init_inplace(addr, size);
    ((prh_atomptrque_t *)q)->u.free = 1;
    return q;
}

void prh_atomptrque_free(prh_atomptrque_t **p) {
    prh_atomptrque_t *q = *p;
    if (q == prh_null)
        return;
    if (q->u.free) {
        prh_free(q);
    }
    *p = prh_null;
}

void prh_atomintque_free(prh_atomintque_t **p) {
    prh_atomptrque_free((prh_atomptrque_t **)p);
}

int prh_atomptrque_len(prh_atomptrque_t *q) { // is not an atomic operation
    int len = prh_atomtype_load(&q->tail) - prh_atomtype_load(&q->head);
    if (len < 0)
        len += q->u.size;
    return len;
}

int prh_atomptrque_cap(prh_atomptrque_t *q) {
    return q->u.size;
}

int prh_atomintque_len(prh_atomintque_t *q) {
    return prh_atomptrque_len((prh_atomptrque_t *)q);
}

int prh_atomintque_cap(prh_atomintque_t *q) {
    return prh_atomptrque_cap((prh_atomptrque_t *)q);
}

bool prh_impl_atomptrque_finish_push(prh_atomptrque_t *q, int tail) {
    int next = prh_impl_atomptrque_nextpos(q, tail);
    if (next == prh_atomtype_load(&q->head))
        return false;
    assert(prh_atomtype_load(&q->tail) == tail); // only allow one writer
    prh_atomtype_store(&q->tail, next);
    return true;
}

void prh_impl_atomptrque_finish_pop(prh_atomptrque_t *q, int head) {
    assert(prh_atomtype_load(&q->head) == head); // only allow one reader
    prh_atomtype_store(&q->head, prh_impl_atomptrque_nextpos(q, head));
}

#define prh_impl_get_ptr_que(q) ((void **)((prh_atomptrque_t *)q + 1))
#define prh_impl_get_int_que(q) ((int *)((prh_atomptrque_t *)q + 1))

bool prh_atomptrque_push(prh_atomptrque_t *q, void *v) {
    if (v == prh_null) return false;
    int tail = prh_atomtype_load(&q->tail);
    prh_impl_get_ptr_que(q)[tail] = v;
    return prh_impl_atomptrque_finish_push(q, tail);
}

void *prh_atomptrque_pop(prh_atomptrque_t *q) {
    int head = prh_atomtype_load(&q->head);
    if (head == prh_atomtype_load(&q->tail)) {
        return prh_null;
    }
    void *v = prh_impl_get_ptr_que(q)[head];
    prh_impl_atomptrque_finish_pop(q, head);
    return v;
}

void *prh_atomptrque_top(prh_atomptrque_t *q) {
    int head = prh_atomtype_load(&q->head);
    if (head == prh_atomtype_load(&q->tail)) {
        return prh_null;
    }
    return prh_impl_get_ptr_que(q)[head];
}

bool prh_atomintque_push(prh_atomintque_t *q, int v) {
    if (v == 0) return false;
    int tail = prh_atomtype_load(&((prh_atomptrque_t *)q)->tail);
    prh_impl_get_int_que(q)[tail] = v;
    return prh_impl_atomptrque_finish_push((prh_atomptrque_t *)q, tail);
}

int prh_atomintque_pop(prh_atomintque_t *q) { // return 0 means empty
    int head = prh_atomtype_load(&((prh_atomptrque_t *)q)->head);
    if (head == prh_atomtype_load(&((prh_atomptrque_t *)q)->tail)) {
        return 0;
    }
    int v = prh_impl_get_int_que(q)[head];
    prh_impl_atomptrque_finish_pop((prh_atomptrque_t *)q, head);
    return v;
}

int prh_atomintque_top(prh_atomintque_t *q) {
    int head = prh_atomtype_load(&((prh_atomptrque_t *)q)->head);
    if (head == prh_atomtype_load(&((prh_atomptrque_t *)q)->tail)) {
        return 0;
    }
    return prh_impl_get_int_que(q)[head];
}

#if defined(_WIN32)

#else // _WIN32 end PTHREAD begin
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

struct prh_thrd {
    prh_thrd_struct *basestruct;
    prh_thrdproc_t proc;
    pthread_t tid_impl;
    prh_u32 index: 31, created: 1;
};

prh_thread_local prh_thrd_t *PRH_IMPL_THRD = prh_null;

// [prh_thrd_struct]
// [prh_thrd_t]
// [prh_thrd_t]
// ...
struct prh_thrd_struct {
    prh_u32 maxthreads: 31, inplace: 1;
    prh_int thread_cnt;
    prh_int start_id;
    prh_int mainudsize;
    prh_thrd_t main; // last field dont move
};

int prh_impl_thrd_struct_alloc_size(int maxthreads, int mainudsize) {
    return sizeof(prh_thrd_struct) + prh_round_ptrsize(mainudsize) + sizeof(void *) * (maxthreads + 1);
}

prh_thrd_t **prh_impl_thrd_list(prh_thrd_struct *s) {
    return (prh_thrd_t **)(((char *)(s + 1)) + s->mainudsize);
}

prh_thrd_t *prh_thread_self(void) {
    return PRH_IMPL_THRD;
}

prh_thrd_t *prh_thread_main(prh_thrd_struct *s) {
    return &s->main;
}

prh_thrd_t *prh_thread_get(prh_thrd_struct *s, int index) {
    assert(index >= 0 && index <= s->thread_cnt);
    return prh_impl_thrd_list(s)[index]; // 0 for main thread
}

void *prh_thread_userdata(prh_thrd_t *thrd) {
    return thrd + 1;
}

int prh_thread_count(prh_thrd_struct *s) {
    return (int)s->maxthreads;
}

int prh_thread_index(prh_thrd_t *thrd) {
    return (int)thrd->index;
}

int prh_thread_id(prh_thrd_t *thrd) {
    return (int)thrd->index + thrd->basestruct->start_id;
}

int prh_thread_alloc_size(int maxthreads, int mainudsize) {
    assert(maxthreads >= 0);
    if (mainudsize < 0) mainudsize = 0;
    return prh_impl_thrd_struct_alloc_size(maxthreads, mainudsize);
}

#if PRH_THRD_DEBUG && defined(PRH_GLIBC_VERSION) && (PRH_GLIBC_VERSION >= 203)
void prh_impl_print_thrd_info(prh_thrd_t *thrd) {
    pthread_t tid = pthread_self();
    void *stack_addr = prh_null;
    size_t stack_size = 0, guard_size = 0;
    pthread_attr_t attr;
    prh_zeroret(pthread_getattr_np(tid, &attr));
    prh_zeroret(pthread_attr_getstack(&attr, &stack_addr, &stack_size));
    prh_zeroret(pthread_attr_getguardsize(&attr, &guard_size));
    prh_zeroret(pthread_attr_destroy(&attr));
    printf("[thread %d] index %d stack %p size %d-byte, guard size %d-byte\n",
        prh_thread_id(thrd), prh_thread_index(thrd), stack_addr, (int)stack_size, (int)guard_size);
}
#else
#define prh_impl_print_thrd_info(thrd)
#endif

prh_thrd_struct *prh_thread_init_inplace(void *addr, int start_id, int maxthreads, int mainudsize) {
    assert(maxthreads >= 0);
    if (mainudsize < 0) mainudsize = 0;
    mainudsize = prh_round_ptrsize(mainudsize);
    prh_thrd_struct *s = (prh_thrd_struct *)addr;
    s->maxthreads = maxthreads;
    s->inplace = 1;
    s->start_id = start_id;
    s->mainudsize = mainudsize;
    s->main.basestruct = s;
    s->main.tid_impl = pthread_self();
    prh_thrd_t **thrds = prh_impl_thrd_list(s);
    PRH_IMPL_THRD = thrds[0] = &s->main;
#if PRH_THRD_DEBUG
    prh_impl_print_thrd_info(&s->main);
#endif
    return s;
}

prh_thrd_struct *prh_thread_init(int start_id, int maxthreads, int mainudsize) {
    prh_thrd_struct *s = (prh_thrd_struct *)prh_calloc(prh_thread_alloc_size(maxthreads, mainudsize));
    prh_thread_init_inplace(s, start_id, maxthreads, mainudsize);
    s->inplace = 0;
    return s;
}

static void *prh_impl_pthread_start(void *param) {
    prh_thrd_t *thrd = PRH_IMPL_THRD = (prh_thrd_t *)param;
#if PRH_THRD_DEBUG
    prh_impl_print_thrd_info(thrd);
#endif
    int n = thrd->proc(thrd);
    return (void *)(prh_unsigned)n;
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

bool prh_impl_pthread_create(prh_thrd_t *thrd, int stacksize) {
    pthread_t *tid = &thrd->tid_impl;
    pthread_attr_t attr;
    bool created = false;
    int n;

    stacksize = prh_impl_thread_stack_size(stacksize);
    if (stacksize <= 0) {
        n = pthread_create(tid, prh_null, prh_impl_pthread_start, thrd);
        prh_prerr_if(n, return false) else { return true; }
    }

    prh_zeroret(pthread_attr_init(&attr));

    n = pthread_attr_setstacksize(&attr, stacksize);
    prh_defer_if(n, prh_prerr(n));

    n = pthread_create(tid, &attr, prh_impl_pthread_start, thrd);
    prh_prerr_if(n,) else { created = true; }

label_defer:
    prh_zeroret(pthread_attr_destroy(&attr));
    return created;
}

void *prh_thread_create(prh_thrd_struct *s, prh_thrdproc_t proc, int stacksize, int thrdudsize) {
    if ((prh_u32)s->thread_cnt >= s->maxthreads) {
        return prh_null;
    }

    if (thrdudsize < 0) thrdudsize = 0;
    thrdudsize = prh_round_ptrsize(thrdudsize);
    prh_thrd_t *thrd = (prh_thrd_t *)prh_calloc(sizeof(prh_thrd_t) + thrdudsize);
    thrd->basestruct = s;
    thrd->proc = proc;

    prh_thrd_t **thrds = prh_impl_thrd_list(s);
    s->thread_cnt += 1;
    thrd->index = s->thread_cnt;
    thrds[s->thread_cnt] = thrd;

    if (prh_impl_pthread_create(thrd, stacksize)) {
        thrd->created = 1;
    }

    return thread_userdata(thrd);
}

void prh_thread_join(prh_thrd_struct **main, void (*thrd_udata_free)(void *)) {
    prh_thrd_struct *s = *main;
    if (s == prh_null) return;

    prh_thrd_t **thrds = prh_impl_thrd_list(s);
    for (int i = 1; i <= s->thread_cnt; i += 1) {
        prh_thrd_t *thrd = thrds[i];
        if (thrd == prh_null) continue;
        if (thrd->created) {
            void *retv = prh_null;
            int n = pthread_join(thrd->tid_impl, &retv);
            if (n != 0) {
                prh_prerr(n);
            } else if ((prh_unsigned)retv != 0) { // -1 is PTHREAD_CANCELED
                prh_prerr((int)(prh_unsigned)retv);
            }
#if PRH_THRD_DEBUG
            if ((prh_unsigned)retv == (prh_unsigned)PTHREAD_CANCELED) {
                printf("[thread %d] canceled join\n", prh_thread_id(thrd));
            } else {
                printf("[thread %d] join retval %d\n", prh_thread_id(thrd), (int)(prh_unsigned)retv);
            }
#endif
            thrd->created = 0;
        }
        if (thrd_udata_free) {
            thrd_udata_free(prh_thread_userdata(thrd));
        }
        prh_free(thrd);
        thrds[i] = prh_null;
    }

    prh_thrd_t *main_thrd = thrds[0];
    if (main_thrd && thrd_udata_free) {
        thrd_udata_free(prh_thread_userdata(main_thrd));
    }

    if (!s->inplace) {
        prh_free(s);
    }

    *main = prh_null;
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
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void prh_impl_thrd_mutex_init(pthread_mutex_t *mutex) {
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

prh_thrd_mutex_t *prh_thrd_mutex_init(void) {
    prh_thrd_mutex_t *p = prh_malloc(sizeof(prh_thrd_mutex_t));
    prh_impl_thrd_mutex_init(&p->mutex);
    return p;
}

prh_thrd_mutex_t *prh_thrd_recursive_mutex_init(void) {
    prh_thrd_mutex_t *p = prh_malloc(sizeof(prh_thrd_mutex_t));
    pthread_mutexattr_t attr;
    prh_zeroret(pthread_mutexattr_init(&attr));
    prh_zeroret(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
    prh_zeroret(pthread_mutex_init(&p->mutex, &attr));
    prh_zeroret(pthread_mutexattr_destroy(&attr));
    return p;
}

void prh_thrd_mutex_free(prh_thrd_mutex_t **mtx) {
    prh_thrd_mutex_t *p = *mtx;
    if (p == prh_null) return;
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
    prh_free(p);
    *mtx = prh_null;
}

void prh_thrd_mutex_lock(prh_thrd_mutex_t *p) {
    prh_zeroret(pthread_mutex_lock(&p->mutex));
}

void prh_thrd_mutex_unlock(prh_thrd_mutex_t *p) {
    prh_zeroret(pthread_mutex_unlock(&p->mutex));
}

// It is advised that an application should not use a PTHREAD_MUTEX_RECURSIVE
// mutex with condition variables because the implicit unlock performed for
// a pthread_cond_timedwait() or pthread_cond_wait() may not actually release
// the mutex (if it had been locked multiple times). If this happens, no other
// thread can satisfy the condition of the predicate.
prh_thrd_cond_t *prh_thrd_cond_init(void) {
    prh_thrd_cond_t *p = prh_malloc(sizeof(prh_thrd_cond_t));
    prh_impl_thrd_mutex_init(&p->mutex);
    prh_zeroret(pthread_cond_init(&p->cond, prh_null));
    return p;
}

void prh_thrd_cond_free(prh_thrd_cond_t **cnd) {
    // 仅当没有任何线程等待条件变量，将其销毁才是安全的，经销毁的条件变量之后可以调用
    // pthread_cond_init() 对其进行重新初始化。
    prh_thrd_cond_t *p = *cnd;
    if (p == prh_null) return;
    prh_zeroret(pthread_cond_destroy(&p->cond));
    prh_zeroret(pthread_mutex_destroy(&p->mutex));
    prh_free(p);
    *cnd = prh_null;
}

void prh_thrd_cond_wait(prh_thrd_cond_t *p) {
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
    prh_zeroret(pthread_cond_wait(&p->cond, &p->mutex));
}

bool prh_thrd_cond_timedwait(prh_thrd_cond_t *p, prh_u32 msec) {
    struct timespec abstime = {.tv_sec = msec/1000, .tv_nsec = ((msec % 1000) * 1000 * 1000)};
    // TODO: abstime += curtime
    int n = pthread_cond_timedwait(&p->cond, &p->mutex, &abstime);
    if (n == ETIMEDOUT) {
        return true; // true WAIT TIMEOUT false WAIT SUCCESS
    }
    prh_zeroret(n);
    return false;
}

void prh_thrd_cond_signal(prh_thrd_cond_t *p) {
    // 唤醒至少一个等待的线程，比broadcast更高效。应用这种方式的典型情况是，所有等待
    // 的线程都在执行完全相同的任务。这种情况下，可以避免唤醒所有等待的线程，然后某一
    // 线程获得调度，此线程检查了共享变量的状态（在相关互斥量的保护下），发现有任务需
    // 要完成并执行所需工作并改变共享变量状态，最后释放对相关互斥量的锁定。如果唤醒了
    // 多余的线程，会额外轮流等待锁定互斥量然后检测共享变量的状态，不过由于第一个线程
    // 已经完成了工作这些多余的线程会发现无事可做，随即解锁互斥量继续休眠。
    prh_zeroret(pthread_cond_signal(&p->cond));
}

void prh_thrd_cond_broadcast(prh_thrd_cond_t *p) {
    // 唤醒所有等待的线程，通常适用的情况是处于等待状态的所有线程执行的任务不同，其各
    // 自关联于条件变量的判定条件不同。条件变量并不保存变量，只是传递应用程序状态信息
    // 的一种通讯机制。发送信号时若无任何线程在等待该条件变量，这个信号也就会不了了
    // 之，线程如果在此后等待该条件变量，只有当再次收到此条件变量的下一信号时才能解除
    // 阻塞状态。
    prh_zeroret(pthread_cond_broadcast(&p->cond));
}

#ifdef PRH_CONC_TEST
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
void prh_conc_test(void) {
    long n = 0;
    char buf[32];
    struct rlimit l = {0};

    printf("\n\n[GNU][posix]\n");
#ifdef PRH_GCC_VERSION
    printf("gcc version %d\n", PRH_GCC_VERSION);
#endif
#ifdef PRH_GLIBC_VERSION
    printf("glibc version %d\n", PRH_GLIBC_VERSION);
#endif
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
    printf("PTHREAD_CANCELED %d\n", (int)(prh_unsigned)PTHREAD_CANCELED);
    printf("PTHREAD_CREATE_JOINABLE %d PTHREAD_CREATE_DETACHED %d\n", PTHREAD_CREATE_JOINABLE, PTHREAD_CREATE_DETACHED);
    printf("PTHREAD_SCOPE_SYSTEM %d PTHREAD_SCOPE_PROCESS %d\n", PTHREAD_SCOPE_SYSTEM, PTHREAD_SCOPE_PROCESS);
    printf("PTHREAD_INHERIT_SCHED %d PTHREAD_EXPLICIT_SCHED %d\n", PTHREAD_INHERIT_SCHED, PTHREAD_EXPLICIT_SCHED);
    printf("Sechedure Policy SCHED_FIFO %d SCHED_RR %d SCHED_OTHER %d\n", SCHED_FIFO, SCHED_RR, SCHED_OTHER);
    printf("ETIMEDOUT = %d\n", ETIMEDOUT);
    printf("EDEADLK = %d\n", EDEADLK);
    printf("EINVAL = %d\n", EINVAL);
    printf("ENOTSUP = %d\n", ENOTSUP);
    printf("ESRCH = %d\n", ESRCH);
    assert(sizeof(pthread_t) <= sizeof(void *));

    printf("\n\n[GNU][atomic]\n");
    prh_impl_atomic_test();
}
#endif // PRH_CONC_TEST
#endif // PTHREAD

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