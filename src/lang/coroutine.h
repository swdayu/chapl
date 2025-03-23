// coroutine.h - v0.3.0 - public domain - Swdayu, Mar 2025
// Very simple C coroutine implementation based on the ASM code for various
// architectures.
//
// VERSION HISTORY
//
//   v0.3.0 (2025-03-23) First public release
//
// LICENSE
//
//   See end of the file for license information.
//
// CONVENTIONS
//
//   Most preclude macros using the prefix `magic_`, while other preclude
//   names always using the `prh_` prefix or 'prh_impl_` prefix for private.
//   You can define PRH_STRIP_PREFIX macro before including the header file
//   to strip out `prh_` prefix.
//
//   The code always use `//` style comments to keep code clean.
//
//   We are following the https://semver.org/ version conventions as the format
//   MAJOR.MINOR.PATCH:
//
//      Modifying comments does not update the version.
//
//      PATCH is incremented in case of a bug fix or refactoring without
//      touching the API.
//
//      MINOR is incremented when new functions and/or types are added in a way
//      that does not break any existing user code. We want to do this in the
//      majority of the situation.
//
//      If we want to delete a certain function or type in favor of another one
//      we should just add the new function/type and deprecate the old one in a
//      backward compatible way and let them co-exist for a while.
//
//      MAJOR update should be just a periodic cleanup of the deprecated
//      functions and types without really modifying any existing functionality.
//
// CONFIGURATION MACROS
//
//      PRH_STRIP_PREFIX
//
// MAGIC MACROS
//
//      magic_arch_bits
//      magic_arch_32
//      magic_arch_64
//      magic_lit_endian
//      magic_big_endian
//      magic_inline
//      magic_noinline
//      magic_unused
//      magic_debug
//      magic_align
//      magic_align_16_byte
//      magic_ispow2
//      magic_fastcall
//      magic_naked_fastcall
//      magic_fastcall_typedef
//      magic_coroproc
//
// PRH_STRIP_PREFIX INTERFACES
//
//      ARRAY_LEN
//      ARRAY_GET
//      static_must
//      byte
//      int1
//      int2
//      int4
//      int8
//      intp
//
// COROUTINE TYPES
//
//      struct coro
//      coro_struct
//      solo_struct
//      void (*coroproc)(struct coro *coro)
//
// COROUTINE FUNCTIONS
//
//      coroutine_init
//      coroutine_alloc_size
//      coroutine_init_inplace
//      coroutine_create
//      coroutine_ext_create
//      coroutine_start_cycle
//      coroutine_start
//      coroutine_reload
//      coroutine_ext_reload
//      coroutine_finish
//      coroutine_get
//      coroutine_userdata
//      coroutine_self
//      coroutine_yield
//
//      solo_create
//      solo_ext_create
//      solo_start
//      solo_reload
//      solo_ext_reload
//      solo_finish

#ifndef prh_magic_coroutine_h
#define prh_magic_coroutine_h
#ifdef __cplusplus
extern "C" {
#endif

// architecture
#ifndef magic_arch_bits
#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(_M_ARM64) || defined(__powerpc64__)
    #if defined(__x86_64__) || defined(_M_X64)
        #define magic_arch_x64 1
    #elif defined(__aarch64__) || defined(_M_ARM64)
        #define magic_arch_a64 1
    #endif
    #define magic_arch_bits 64
    #define magic_arch_32 0
    #define magic_arch_64 64
#else
    #if defined(__i386) || defined(_M_IX86)
        #define magic_arch_x86 1
    #elif defined(__arm__) || defined(_M_ARM)
        #define magic_arch_arm 1
    #endif
    #define magic_arch_bits 32
    #define magic_arch_32 32
    #define magic_arch_64 0
#endif
#endif // magic_arch_bits

// byte endian
#ifndef magic_lit_endian
#if defined(magic_arch_x86) || defined(magic_arch_x64) || defined(magic_arch_arm) || defined(magic_arch_a64)
    #define magic_lit_endian 1
    #define magic_big_endian 0
#endif
#ifndef magic_lit_endian
    #define magic_lit_endian 1
    #define magic_big_endian 0
#endif
#endif // magic_lit_endian

// inline noinline
#ifndef magic_inline
#ifdef _MSC_VER
    #define magic_inline static __forceinline
    #if _MSC_VER >= 1400 // noinline was introduced in Visual Studio 2005
        #define magic_noinline __declspec(noinline)
    #else
        #define magic_noinline
    #endif
#elif defined(__GNUC__)
    // I've had a bug report where GCC is emitting warnings about functions possibly not being inlineable. This warning happens when
    // the __attribute__((always_inline)) attribute is defined without an "inline" statement. I think therefore there must be some
    // case where "__inline__" is not always defined, thus the compiler emitting these warnings. When using -std=c89 or -ansi on the
    // command line, we cannot use the "inline" keyword and instead need to use "__inline__". In an attempt to work around this issue
    // I am using "__inline__" only when we're compiling in strict ANSI mode.
    #if defined(__STRICT_ANSI__)
        #define magic_gnuc_inline_hint __inline__
    #else
        #define magic_gnuc_inline_hint inline
    #endif
    #if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 2)) || defined(__clang__)
        #define magic_inline static magic_gnuc_inline_hint __attribute__((always_inline))
        #define magic_noinline __attribute__((noinline))
    #else
        #define magic_inline static magic_gnuc_inline_hint
        #define magic_noinline __attribute__((noinline))
    #endif
#elif defined(__WATCOMC__)
    #define magic_inline static __inline
    #define magic_noinline
#else
    #define magic_inline static inline
    #define magic_noinline
#endif
#endif // magic_inline

// compiler
#ifndef prh_magic_compiler
#ifdef _MSC_VER
    #define magic_cl_msc 1
#elif defined(__GNUC__)
    #define magic_cl_gnu 1
#endif
#define prh_magic_compiler
#endif

// fastcall convension
#ifndef prh_magic_fastcall
#if defined(magic_cl_msc)
    #if defined(magic_arch_x64)
        #define magic_naked_fastcall(ret) __declspec(naked) ret
        #define magic_fastcall(ret) ret
        #define magic_fastcall_typedef(ret, name) typedef ret (*name)
    #elif defined(magic_arch_x86)
        #define magic_naked_fastcall(ret) __declspec(naked) ret __fastcall
        #define magic_fastcall(ret) ret __fastcall
        #define magic_fastcall_typedef(ret, name) typedef ret (__fastcall *name)
    #endif
    #define magic_asm_begin() __asm {
    #define magic_asm_end() }
#elif defined(magic_cl_gnu)
    #if defined(magic_arch_x64)
        #define magic_naked_fastcall(ret) __attribute__((naked)) ret
        #define magic_fastcall(ret) ret
        #define magic_fastcall_typedef(ret, name) typedef ret (*name)
    #elif defined(magic_arch_x86)
        #define magic_naked_fastcall(ret) __attribute__((naked,fastcall)) ret
        #define magic_fastcall(ret) __attribute__((fastcall)) ret
        #define magic_fastcall_typedef(ret, name) typedef ret (__attribute__((fastcall)) *name)
    #endif
    #define magic_asm_begin() __asm__ (
    #define magic_asm_end() );
#endif
#define prh_magic_fastcall
#endif // prh_magic_fastcall

#ifndef magic_debug
#ifdef NDEBUG
    #define magic_debug 0
#else
    #define magic_debug 1
#endif
#endif

#ifndef magic_unused
#define magic_unused(a) ((void)(a))
#endif

#ifndef magic_align
#define magic_ispow2(x) (((x) & ((x) - 1)) == 0) // power of 2 or zero
#define magic_align(n) (((n) + sizeof(void *) - 1) & (~(sizeof(void *) - 1)))
#define magic_align_16_byte(n) (((unsigned prh_intp)(n) + 15) & (~(unsigned prh_intp)15))
#endif

#ifndef PRH_ARRAY_LEN
#include <assert.h>
#define PRH_ARRAY_LEN(a) (sizeof(a)/sizeof((a)[0]))
#define PRH_ARRAY_GET(a, i) ((assert((unsigned prh_intp)(i) < PRH_ARRAY_LEN(a))), (a)[i])
#ifdef PRH_STRIP_PREFIX
    #define ARRAY_LEN           PRH_ARRAY_LEN
    #define ARRAY_GET           PRH_ARRAY_GET
#endif
#endif

#ifndef PRH_MEM_ALLOC
#include <stdlib.h>
#define PRH_MEM_ALLOC malloc
// void *malloc(size_t size);
// the newly allocated block of memory is not initialized, remaining with indeterminate values.
// if size == 0 { may or may not return null, but the returned pointer shall not be dereferenced }
// if fails to allocate the requested block of memory, a null pointer is returned.
#endif

#ifndef PRH_MEM_REALLOC
#include <stdlib.h>
#define PRH_MEM_REALLOC realloc
// void *realloc(void *ptr, size_t size);
// if ptr == NULL { return malloc(size) }
// if size == 0 { may be free(ptr) or depends on library implementation }
// if size > ptr old size { may return the new location and the newer portion is indeterminate }
// if fails to allocate the requested block of memory, null is returned and ptr remain unchanged.
#endif

#ifndef PRH_MEM_FREE
#include <stdlib.h>
#define PRH_MEM_FREE free
// void free(void *ptr);
// if ptr == NULL { the function does nothing }
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#if !defined(__cplusplus) && !defined(bool) && !defined(PRH_NO_BOOL)
    #define PRH_NO_BOOL
    typedef unsigned char bool;
    #define true (0 == 0)
    #define false (0 != 0)
#endif

#ifndef prh_static_must
    #define prh_static_must(const_expr) typedef int prh_must_static_meet[(const_expr) ? 1 : -1]
#ifdef PRH_STRIP_PREFIX
    #define static_must         prh_static_must
#endif
#endif

#ifndef prh_primary_types
    typedef unsigned char prh_byte;
    #define prh_int1 signed char
    #define prh_int2 short
    #define prh_int4 int
    #define prh_int8 long long
#if magic_arch_bits == 32
    #define prh_intp prh_int4
#elif magic_arch_bits == 64
    #define prh_intp prh_int8
#else
    #error unsupported architecture
#endif
prh_static_must(sizeof(bool) == 1);
prh_static_must(sizeof(prh_byte) == 1);
prh_static_must(sizeof(prh_int1) == 1);
prh_static_must(sizeof(prh_int2) == 2);
prh_static_must(sizeof(unsigned prh_int2) == 2);
prh_static_must(sizeof(prh_int4) == 4);
prh_static_must(sizeof(unsigned prh_int4) == 4);
prh_static_must(sizeof(prh_int8) == 8);
prh_static_must(sizeof(unsigned prh_int8) == 8);
prh_static_must(sizeof(prh_intp) == sizeof(void *));
prh_static_must(sizeof(unsigned prh_intp) == sizeof(void *));
prh_static_must(sizeof(int) == sizeof(prh_int4));
prh_static_must(sizeof(unsigned) == sizeof(int));
#ifdef PRH_STRIP_PREFIX
    typedef unsigned char byte;         // byte - 1-byte unsigned
    #define int1            prh_int1    // int1 - 1-byte signed
    #define int2            prh_int2    // int2 - 2-byte signed, unsigned int2 - 2-byte unsigned
    #define int4            prh_int4    // int4 - 4-byte signed, unsigned int4 - 4-byte unsigned
    #define int8            prh_int8    // int8 - 8-byte signed, unsigned int8 - 8-byte unsigned
    #define intp            prh_intp    // intp - p-byte signed, unsigned intp - p-byte unsigned
#endif
#define prh_primary_types
#endif // prh_primary_types

struct coro;

typedef struct {
    void *address;
} coro_struct;

magic_fastcall(bool) prh_impl_asm_coro_yield(struct coro *coro, struct coro *next);
struct coro *prh_impl_coro_next_resume(struct coro *coro);

magic_inline void coroutine_yield(struct coro *coro)
{
    prh_impl_asm_coro_yield(coro, prh_impl_coro_next_resume(coro));
}

#define magic_coroproc(ret) magic_fastcall(ret)
magic_fastcall_typedef(void, coroproc)(struct coro *);

int coroutine_self(struct coro *coro);
void *coroutine_userdata(struct coro *coro);

int coroutine_alloc_size(int maxcoros);
coro_struct coroutine_init_inplace(void *addr, int maxcoros); // addr shall be previously zero initialized
coro_struct coroutine_init(int maxcoros);

void coroutine_create(coro_struct main, coroproc proc, int stack_size, void *userdata);
void *coroutine_ext_create(coro_struct main, coroproc proc, int stack_size, int userdata_bytes);
struct coro *coroutine_get(coro_struct main, int index);

bool coroutine_start_cycle(coro_struct main);
bool coroutine_start(coro_struct main, int index);
void coroutine_reload(coro_struct main, int index, coroproc proc);
void coroutine_ext_reload(coro_struct main, int index, coroproc proc, void *userdata);
void coroutine_finish(coro_struct *main);

typedef struct {
    void *impl;
    void *a[magic_arch_bits == 64 ? 2 : 3];
} solo_struct;

void solo_create(solo_struct *main, coroproc proc, int stack_size, void *userdata);
void *solo_ext_create(solo_struct *main, coroproc proc, int stack_size, int maxudsize);
bool solo_start(solo_struct *main);
void solo_reload(solo_struct *main, coroproc proc);
void solo_ext_reload(solo_struct *main, coroproc proc, void *userdata);
void solo_finish(solo_struct *main);

#ifdef __cplusplus
}
#endif
#endif // prh_magic_coroutine_h

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2025 Swdayu <swdayu**qq**com>
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
