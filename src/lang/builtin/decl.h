#ifndef CHAPL_BUILTIN_DECL_H
#define CHAPL_BUILTIN_DECL_H
#include "direct/file.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#undef __DECL_THREAD
#undef __DECL_PACKED

#if defined(__MSC__)
#define __DECL_THREAD __declspec(thread)
// pack takes effect at the first struct, union, or class declaration after the pragma is seen
#define __DECL_PACKED _Pragma("pack(1)") typedef struct
#elif defined(__GCC__)
#define __DECL_THREAD __thread
// an attribute specifier list may appear as part of a struct, union or enum specifier.
// it may go either immediately after the struct, union or enum keyword, or after the
// closing brace. the former syntax is preferred.
#define __DECL_PACKED typedef struct __attribute__ ((packed))
#else
#include <threads.h>
#define __DECL_THREAD thread_local
#define __DECL_PACKED _Pragma("pack(1)") typedef struct
#endif

#ifdef __TARGET_X86__
#include "abi/x86_abi.h"
#elif __TARGET_X64__
#include "abi/x64_abi.h"
#else
#include "conf/pretypes.genfile"
#endif

const static int8 LANG_MIN_INT8 = -127-1; /* 128 0x80 */
const static int8 LANG_MAX_INT8 = 0x7f; /* 127 */
const static uint8 LANG_MAX_UNT8 = 0xff; /* 255 */

const static int16 LANG_MIN_INT16 = -32767-1; /* 32768 0x8000 */
const static int16 LANG_MAX_INT16 = 0x7fff; /* 32767 */
const static uint16 LANG_MAX_UNT16 = 0xffff; /* 65535 */

const static int32 LANG_MIN_INT32 = -2147483647-1; /* 2147483648 0x80000000 */
const static int32 LANG_MAX_INT32 = 0x7fffffff; /* 2147483647 */
const static uint32 LANG_MAX_UNT32 = 0xffffffff; /* 4294967295 */

const static int64 LANG_MIN_INT64 = -9223372036854775807LL-1; /* 9223372036854775808 0x8000000000000000 */
const static int64 LANG_MAX_INT64 = 0x7fffffffffffffffLL; /* 9223372036854775807 */
const static uint64 LANG_MAX_UNT64 = 0xffffffffffffffffULL; /* 18446744073709551615 */

#undef LANG_MAX_BYTE
#undef LANG_MIN_INT
#undef LANG_MAX_INT
#undef LANG_MAX_UNT

#define LANG_MAX_BYTE LANG_MAX_UNT8
#define LANG_MIN_INT LANG_MIN_INTPTR
#define LANG_MAX_INT LANG_MAX_INTPTR
#define LANG_MAX_UNT LANG_MAX_UNTPTR

#undef null
#undef true
#undef false
#undef bool
#undef byte
#undef rune
#undef strid_t
#undef Error

#define null 0
#define true 1
#define false 0
typedef uint8 bool;
typedef uint8 byte;
typedef uint32 rune;
typedef uint16 strid_t;
typedef uint32 Error;

#undef Int
#undef Uint

typedef intptr Int;
typedef uintptr Uint;

#undef Float
#undef float32
#undef float64

typedef float float32;
typedef double float64;

#ifdef CONFIG_32BIT_FLOAT
typedef float32 Float;
#else
typedef float64 Float;
#endif

#define X_X_NAME(a, b) X_NAME(a, b)
#define X_X_MSTR(a) X_MSTR(a)
#define X_NAME(a, b) a##b
#define X_MSTR(a) #a

#define LOG_LEVEL_F 0x00000000 // fatal panic
#define LOG_LEVEL_M 0x01000000 // main
#define LOG_LEVEL_E 0x02000000 // error
#define LOG_LEVEL_I 0x03000000 // information
#define LOG_LEVEL_D 0x04000000 // only for debug, not in release

#undef lang_assert
#undef debug_assert

#ifdef CONFIG_DEBUG
#define debug_assert(e) lang_assert(e)
#else
#define debug_assert(e)
#endif

#define lang_assert(e) ((e) ? ((void)0) : assertfault_(__CURR_FILE__, __LINE__))
#define lang_assert_x(e, n, ...) ((e) ? ((void)0) : assertfaultx_(__CURR_FILE__, X_ARGN_LINE(n), ## __VA_ARGS__))
#define lang_assert_s_x(e, s, n, ...) ((e) ? ((void)0) : assertfaults_(__CURR_FILE__, X_ARGN_LINE(n), (s), ## __VA_ARGS__))
#define lang_assert_1(e, a) lang_assert_x((e), 1, (uint32)(a))
#define lang_assert_2(e, a, b) lang_assert_x((e), 2, (uint32)(a), (uint32)(b))
#define lang_assert_3(e, a, b, c) lang_assert_x((e), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define lang_assert_4(e, a, b, c, d) lang_assert_x((e), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define lang_assert_5(e, a, b, c, d, ee) lang_assert_x((e), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(ee))
#define lang_assert_s(e, s) lang_assert_s_x((e), (s), 0)
#define lang_assert_s_1(e, s, a) lang_assert_s_x((e), (s), 1, (uint32)(a))
#define lang_assert_s_2(e, s, a, b) lang_assert_s_x((e), (s), 2, (uint32)(a), (uint32)(b))
#define lang_assert_s_3(e, s, a, b, c) lang_assert_s_x((e), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define lang_assert_s_4(e, s, a, b, c, d) lang_assert_s_x((e), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define lang_assert_s_5(e, s, a, b, c, d, ee) lang_assert_s_x((e), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(ee))

void assertfault_(uint16 file, uint32 line);
void assertfaultx_(uint16 file, uint32 argn_line, ...);
void logtrace_(Error err, uint32 file_err, uint32 line);
void logtracex_(Error err, uint32 file_err, uint32 argn_line, ...);

#define X_FILE(err) (__CURR_FILE__ << 16)
#define X_ARGN_LINE(argn) (((argn) << 27) | __LINE__)
#define X_LINE(level, argn) (((argn) << 27) | (level) | __LINE__)

#define LOG_FILE(file) ((strid_t)((file) >> 16))
#define LOG_ARGN(line) ((line) >> 27)
#define LOG_LEVEL(line) (((line) >> 24) & 0x7)
#define LOG_LINE(line) ((line) & 0x00FFFFFF)
#define LOG_CHAR(line) "FmEID"[LOG_LEVEL(line)]

#define log_fatal(err) logtrace_((err), X_FILE(err), LOG_LEVEL_F|__LINE__)
#define log_fatal_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_F, (n)), __VA_ARGS__)
#define log_fatal_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_F, (n)), (s), ## __VA_ARGS__)

#define __LOG_LEVEL__ X_X_NAME(__CURR_FILE__, _LOG_LEVEL)

#if (__LOG_LEVEL__ == 'M' || __LOG_LEVEL__ == 'E' || __LOG_LEVEL__ == 'I' || __LOG_LEVEL__ == 'D')
#define log_main(err) logtrace_((err), X_FILE(err), LOG_LEVEL_M|__LINE__)
#define log_main_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_M, (n)), __VA_ARGS__)
#define log_main_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_M, (n)), (s), ## __VA_ARGS__)
#else
#define log_main(err)
#define log_main_x(err, n, ...)
#define log_main_s_x(err, s, n, ...)
#endif

#if (__LOG_LEVEL__ == 'E' || __LOG_LEVEL__ == 'I' || __LOG_LEVEL__ == 'D')
#define log_error(err) logtrace_((err), X_FILE(err), LOG_LEVEL_E|__LINE__)
#define log_error_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_E, (n)), __VA_ARGS__)
#define log_error_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_E, (n)), (s), ## __VA_ARGS__)
#else
#define log_error(err)
#define log_error_x(err, n, ...)
#define log_error_s_x(err, s, n, ...)
#endif

#if (__LOG_LEVEL__ == 'I' || __LOG_LEVEL__ == 'D')
#define log_info(err) logtrace_((err), X_FILE(err), LOG_LEVEL_I|__LINE__)
#define log_info_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_I, (n)), __VA_ARGS__)
#define log_info_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_I, (n)), (s), ## __VA_ARGS__)
#else
#define log_info(err)
#define log_info_x(err, n, ...)
#define log_info_s_x(err, s, n, ...)
#endif

#if (__LOG_LEVEL__ == 'D') && defined(CONFIG_DEBUG)
#define log_debug(err) logtrace_((err), X_FILE(err), LOG_LEVEL_D|__LINE__)
#define log_debug_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_D, (n)), __VA_ARGS__)
#define log_debug_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_D, (n)), (s), ## __VA_ARGS__)
#else
#define log_debug(err)
#define log_debug_x(err, n, ...)
#define log_debug_s_x(err, s, n, ...)
#endif

#define log_fatal_1(err, a) log_fatal_x((err), 1, (uint32)(a))
#define log_fatal_2(err, a, b) log_fatal_x((err), 2, (uint32)(a), (uint32)(b))
#define log_fatal_3(err, a, b, c) log_fatal_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_fatal_4(err, a, b, c, d) log_fatal_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_fatal_5(err, a, b, c, d, e) log_fatal_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_fatal_s(err, s) log_fatal_s_x((err), (s), 0)
#define log_fatal_s_1(err, s, a) log_fatal_s_x((err), (s), 1, (uint32)(a))
#define log_fatal_s_2(err, s, a, b) log_fatal_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_fatal_s_3(err, s, a, b, c) log_fatal_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_fatal_s_4(err, s, a, b, c, d) log_fatal_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_fatal_s_5(err, s, a, b, c, d, e) log_fatal_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_main_1(err, a) log_main_x((err), 1, (uint32)(a))
#define log_main_2(err, a, b) log_main_x((err), 2, (uint32)(a), (uint32)(b))
#define log_main_3(err, a, b, c) log_main_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_main_4(err, a, b, c, d) log_main_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_main_5(err, a, b, c, d, e) log_main_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_main_s(err, s, a) log_main_s_x((err), (s), 0)
#define log_main_s_1(err, s, a) log_main_s_x((err), (s), 1, (uint32)(a))
#define log_main_s_2(err, s, a, b) log_main_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_main_s_3(err, s, a, b, c) log_main_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_main_s_4(err, s, a, b, c, d) log_main_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_main_s_5(err, s, a, b, c, d, e) log_main_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_error_1(err, a) log_error_x((err), 1, (uint32)(a))
#define log_error_2(err, a, b) log_error_x((err), 2, (uint32)(a), (uint32)(b))
#define log_error_3(err, a, b, c) log_error_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_error_4(err, a, b, c, d) log_error_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_error_5(err, a, b, c, d, e) log_error_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_error_s(err, s, a) log_error_s_x((err), (s), 0)
#define log_error_s_1(err, s, a) log_error_s_x((err), (s), 1, (uint32)(a))
#define log_error_s_2(err, s, a, b) log_error_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_error_s_3(err, s, a, b, c) log_error_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_error_s_4(err, s, a, b, c, d) log_error_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_error_s_5(err, s, a, b, c, d, e) log_error_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_info_1(err, a) log_info_x((err), 1, (uint32)(a))
#define log_info_2(err, a, b) log_info_x((err), 2, (uint32)(a), (uint32)(b))
#define log_info_3(err, a, b, c) log_info_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_info_4(err, a, b, c, d) log_info_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_info_5(err, a, b, c, d, e) log_info_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_info_s(err, s, a) log_info_s_x((err), (s), 0)
#define log_info_s_1(err, s, a) log_info_s_x((err), (s), 1, (uint32)(a))
#define log_info_s_2(err, s, a, b) log_info_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_info_s_3(err, s, a, b, c) log_info_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_info_s_4(err, s, a, b, c, d) log_info_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_info_s_5(err, s, a, b, c, d, e) log_info_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_debug_1(err, a) log_debug_x((err), 1, (uint32)(a))
#define log_debug_2(err, a, b) log_debug_x((err), 2, (uint32)(a), (uint32)(b))
#define log_debug_3(err, a, b, c) log_debug_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_debug_4(err, a, b, c, d) log_debug_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_debug_5(err, a, b, c, d, e) log_debug_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_debug_s(err, s, a) log_debug_s_x((err), (s), 0)
#define log_debug_s_1(err, s, a) log_debug_s_x((err), (s), 1, (uint32)(a))
#define log_debug_s_2(err, s, a, b) log_debug_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_debug_s_3(err, s, a, b, c) log_debug_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_debug_s_4(err, s, a, b, c, d) log_debug_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_debug_s_5(err, s, a, b, c, d, e) log_debug_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#undef ERROR
#undef BUILTIN_NUM_ERRORS
#undef BUILTIN_LAST_ERROR

typedef enum {
    ERROR = 1,
#define ERROR_MAPPING(ID, STR) ERROR_##ID,
#include "builtin/error.h"
    BUILTIN_NUM_ERRORS,
    BUILTIN_LAST_ERROR = 0xFF,
} builtin_error_enum_t;

#if CONFIG_RT_ERROR_STRING
extern const byte* builtin_errors_g[BUILTIN_NUM_ERRORS];
#define builtin_error(id) ((Error)builtin_errors_g[id])
#else
#define builtin_error(id) (((id) > ERROR) ? (((id) << 1) | ERROR) : (id))
#endif

extern __DECL_THREAD Error error_g;

#undef set_last_error
#undef get_last_error

#define set_last_error(n) error_g = (n)
#define get_last_error() error_g

// 将 p 开始的 len 个字节写入到 obj 中，返回成功写入的字节数（0 <= n <= len），如果
// len > 0 而 n < len，表示在写完之前被某个错误提前打断，此时必须返回一个非 null 值的
// error。
typedef Int (*Writer)(void *obj, const byte *p, Int len);

// 将 p 开始的 len 个字节写入到 obj 偏移 off 字节处，返回写入的字节数（0 <= n <= len），
// 如果 len > 0 而 n < len，表示在写完之前被某个错误提前打断，此时必须返回一个非 null
// 值的 error。目标对象 obj 在偏移 off 字节写入数据后，其数据流位置必须还原到偏移之前
// 的原位置。
typedef Int (*WriterAt)(void *obj, Int off, const byte *p, Int len);

// 将数据写入到 wr 中，直到没有更多的数据，或者因为错误提前结束；返回成功写入的字节数，
// 如果遇到错误通过 err 返回。
typedef Int (*WriterTo)(void *obj, Writer wr);

typedef struct {
    byte* a;    // 必须是第一个字段
    Uint len: __ARCH_BITS__ - 1;
    Uint dyn: 1;
} string_t;

bool string_init(string_t *s, const byte *a, Int len, bool alloc);
void string_move_init(string_t *s, string_t *from);
void string_move(string_t *to, string_t *from);
string_t string_create(const byte *a, Int len, bool alloc);
string_t string_move_create(string_t *from);
void string_free(string_t *s);
void assertfaults_(uint16 file, uint32 argn_line, string_t s, ...);
void logtraces_(Error err, uint32 file_err, uint32 argn_line, string_t s, ...);
inline string_t strnull() { return (string_t){0,0,0}; }
inline string_t strfend(const byte *f, const byte *e) { return (string_t){f, e-f, 0}; }
inline string_t strfrom(const char *s) { return (string_t){(byte*)s, s ? strlen(s) : 0, 0}; }
inline bool string_empty(string_t *s) { return !s->len; }
inline Uint string_len(string_t *s) { return s->len; }
inline byte *string_data(string_t *s) { return s->a; }

typedef struct {
    Uint len;
} strfix_t;

typedef struct {
    strfix_t *a;
} strfix2_t;

bool strfix_init(strfix2_t *s, Int len);
void strfix_free(strfix2_t *s);
inline Uint strfix_len(strfix_t *a) { return a->len; }
inline byte *strfix_data(strfix_t *a) { return (byte*)(a+1); }

typedef struct {
    Uint len;
} arrfix_t;

typedef struct {
    arrfix_t *a;
} arrfix2_t;

bool arrfix_init(arrfix2_t *a, Int len);
inline Uint arrfix_len(arrfix_t *a) { return a->len; }
inline byte *arrfix_data(arrfix_t *a) { return (byte *)(a + 1); }

typedef struct {
    uint16 len;
} arrfix_16_t;

typedef struct {
    uint8 len;
} arrfix_8_t;

typedef struct {
    Uint len;   // 必须是第一个字段
    Uint elt;   // 必须是第二个字段
} arrfix_ex_t;

typedef struct {
    arrfix_ex_t *a;
} arrfix2_ex_t; // elt版本的一个缺点是指针步进的计数不是乘以一个常量而是一个运行时值

bool arrfix_ex_init(arrfix2_ex_t *p, Int elt_bytes, Int elt_count);
inline Uint arrfix_ex_len(arrfix_ex_t *a) { return a->len; } // 元素个数
inline byte *arrfix_ex_data(arrfix_ex_t *a) { return (byte*)(a+1); } // 数组数据
inline byte *arrfix_ex_at(arrfix_ex_t *a, Int i) { return (arrfix_ex_data(a) + i * a->elt); }
inline byte *arrfix_ex_at_n(arrfix_ex_t *a, Int i, Int N) { return (arrfix_ex_data(a) + i * N); }

typedef struct {
    uint16 len;
    uint16 elt;
} arrfix_e16_t;

typedef struct {
    uint8 len;
    uint8 elt;
} arrfix_e8_t;

typedef struct {
    Uint cap;   // 必须是第一个字段
    Uint len;   // 必须是第二个字段
} array_t;

typedef struct {
    array_t *a;
} array2_t;

bool array_init(array2_t *a, Int cap);
void array_free(array2_t *a);
bool array_push(array2_t *a, const byte* p, Int len, Int expand);
inline void arrfix_free(arrfix2_t *a) { array_free((array2_t *)a); }
inline void arrfix_ex_free(arrfix2_ex_t *a) { array_free((array2_t *)a); }
inline bool array_push_s(array2_t *a, const string_t *s, Int expand) { return array_push(a, s->a, s->len, expand); }
inline void array_clear(array_t *a) { a->len = 0; }
inline Uint array_cap(array_t *a) { return a->cap; }
inline Uint array_len(array_t *a) { return a->len; }
inline bool array_empty(array_t *a) { return !array_len(a); }
inline byte *array_data(array_t *a) { return (byte *)(a + 1); }
inline byte *array_end(array_t *a) { return array_data(a) + a->len; }

typedef struct {
    uint16 cap;
    uint16 len;
} array_16_t;

typedef struct {
    uint8 cap;
    uint8 len;
} array_8_t;

typedef struct {
    Uint cap;   // 必须是第一个字段
    Uint len;   // 必须是第二个字段
    Uint elt;
} array_ex_t;

typedef struct {
    array_ex_t *a;
} array2_ex_t;

bool array_ex_init(array2_ex_t *a, Int elt_bytes, Int elt_count);
bool array_ex_push(array2_ex_t *a, const byte* p, Int expand);
inline void array_ex_free(array2_ex_t *a) { array_free((array2_t *)a); }
inline void array_ex_clear(array_ex_t *a) { a->len = 0; }
inline Uint array_ex_cap(array_ex_t *a) { return a->cap; }
inline Uint array_ex_len(array_ex_t *a) { return a->len; }
inline bool array_ex_empty(array_ex_t *a) { return !array_ex_len(a); }
inline byte *array_ex_data(array_ex_t *a) { return (byte *)(a + 1); }
inline byte *array_ex_end(array_ex_t *a) { return array_ex_data(a) + a->len * a->elt; }
inline byte *array_ex_at(array_ex_t *a, Int i) { return array_ex_data(a) + i * a->elt; }
inline byte *array_ex_at_n(array_ex_t *a, Int i, Int N) { return array_ex_data(a) + i * N; }

typedef struct {
    uint16 cap;
    uint16 len;
    uint16 elt;
    uint16 extra;
} array_e16_t;

typedef struct {
    uint8 cap;
    uint8 len;
    uint8 elt;
    uint8 extra;
} array_e8_t;

typedef struct {
    Uint cap;
    Uint len;
    Uint start;
} dearr_t;

typedef struct {
    uint16 cap;
    uint16 len;
    uint16 start;
    uint16 extra;
} dearr_16_t;

typedef struct {
    uint8 cap;
    uint8 len;
    uint8 start;
    uint8 extra;
} dearr_8_t;

typedef struct {
    Uint cap;
    Uint len;
    Uint start;
    Uint elt;
} dearr_ex_t;

typedef struct {
    uint16 cap;
    uint16 len;
    uint16 start;
    uint16 elt;
} dearr_e16_t;

typedef struct {
    uint8 cap;
    uint8 len;
    uint8 start;
    uint8 elt;
} dearr_e8_t;

// buffix_t
//  维护一个固定长度的内存空间，只在初始化时分配一次
//  当前位置 cur 可以在数组范围内移动
typedef struct {
    Uint cap;   // 必须是第一个字段
    byte *cur;
} buffix_t;

typedef struct {
    buffix_t *a;
} buffix2_t;

bool buffix_init(buffix2_t *b, Int cap);
inline void buffix_free(buffix2_t *b) { array_free((array2_t *)b); }
inline void buffix_init_inplace(buffix_t *a, Int cap) { a->cap = cap; a->cur = (byte*)(a + 1); }
inline Uint buffix_cap(buffix_t *a) { return a->cap; }
inline byte *buffix_data(buffix_t *a) { return (byte *)(a + 1); }
inline byte *buffix_cur(buffix_t *a) { return a->cur; }

typedef struct {
    byte* a;    // 必须是第一个字段
    Uint cap;   // 必须是第二个字段
    Uint len;
} buffer_t;

bool buffer_init(buffer_t *b, Int cap);
void buffer_move_init(buffer_t *b, buffer_t *from);
void buffer_move(buffer_t *to, buffer_t *from);
string_t buffer_move_to_string(buffer_t *b);
void buffer_free(buffer_t *b);
bool buffer_push(buffer_t *b, const byte* a, Int n, Int expand);
bool buffer_put(buffer_t *b, byte a, Int expand);
void buffer_pop(buffer_t *b, Int n);
inline string_t string_ref_buffer(buffer_t *b) { return (string_t){b->a, b->len, 0}; }
inline void buffer_clear(buffer_t *b) { b->len = 0; }
inline Uint buffer_cap(buffer_t *b) { return b->cap; }
inline Uint buffer_len(buffer_t *b) { return b->len; }
inline byte *buffer_data(buffer_t *b) { return b->a; }
inline bool buffer_empty(buffer_t *b) { return !buffer_len(b); }
inline bool buffer_eq(buffer_t *b, const byte *a, Int len) { return ((a != 0) && ((Int)buffer_len(b) == len) && (memcmp(buffer_data(b), a, len) == 0)); }
inline bool buffer_eq_s(buffer_t *b, const string_t *s) { return buffer_eq(b, s->a, s->len); }

typedef struct {
    byte* a;    // 必须是第一个字段
    uint16 cap; // 必须是第二个字段
    uint16 len;
} buffer_16_t;

typedef struct {
    byte* a;    // 必须是第一个字段
    Uint cap;   // 必须是第二个字段
    Uint len;
    Uint elt;
} buffer_ex_t;

typedef struct {
    byte* a;    // 必须是第一个字段
    uint16 cap; // 必须是第二个字段
    uint16 len;
    uint16 elt;
    uint16 extra;
} buffer_e16_t;

typedef struct {
    byte* a;    // 必须是第一个字段
    Uint cap;   // 必须是第二个字段
    Uint len;
    Uint start;
} deffer_t;

typedef struct {
    byte* a;    // 必须是第一个字段
    uint16 cap; // 必须是第二个字段
    uint16 len;
    uint16 start;
    uint16 extra;
} deffer_16_t;

typedef struct {
    byte* a;    // 必须是第一个字段
    Uint cap;   // 必须是第二个字段
    Uint len;
    Uint start;
    Uint elt;
} deffer_ex_t;

typedef struct {
    byte* a;    // 必须是第一个字段
    uint16 cap; // 必须是第二个字段
    uint16 len;
    uint16 start;
    uint16 elt;
} deffer_e16_t;

typedef struct snode {
    struct snode *next;
} snode_t;

typedef struct {
    snode_t head;
    snode_t *tail;
} slist_t;

typedef struct node {
    struct node *next;
    struct node *prev;
} node_t;

typedef struct list {
    node_t head;
} list_t;

typedef struct { // 置零初始化
    snode_t *top;
} stack_t;

struct stack_it;
struct stack_before_it;
typedef void (*free_t)(void *object);
byte *stack_push(stack_t *s, Int obj_bytes);
byte *stack_push_it(stack_t *s, struct stack_it *it);
byte *stack_insert_after(struct stack_it *p, Int obj_bytes);
byte *stack_insert(struct stack_before_it *p, Int obj_bytes);
byte *stack_replace(struct stack_before_it *p, struct stack_it *node, free_t func);
bool stack_pop(stack_t *s, free_t func);
void stack_free(stack_t *s, free_t func);
struct stack_it *stack_new_it(Int obj_bytes);
void stack_delete_it(struct stack_it *it, free_t func);
inline bool stack_empty(stack_t *s) { return (!s || !s->top); }
inline byte *stack_top(stack_t *s) { return (byte*)(s ? s->top + 1 : 0); }
inline struct stack_it *stack_begin(stack_t *s) { return (struct stack_it *)(s ? s->top : 0); }
inline struct stack_before_it *stack_before_begin(stack_t *s) { return (struct stack_before_it *)s; }
inline struct stack_it *stack_end(stack_t *s) { return 0; }
inline struct stack_it *stack_next(struct stack_it *p) { return (struct stack_it *)(((snode_t *)p)->next); }
inline struct stack_before_it *stack_before_next(struct stack_before_it *p) { return (struct stack_before_it *)(((snode_t *)p)->next); }
inline bool stack_in_range(struct stack_before_it *b, struct stack_it *it) { return (struct stack_it *)(((snode_t *)b)->next) != it; }
inline byte *stack_it_get(struct stack_it *p) { return p ? ((byte *)p) + sizeof(snode_t*) : 0; }
inline byte *stack_before_it_get(struct stack_before_it *p) { return p ? stack_it_get((struct stack_it *)(((snode_t *)p)->next)) : null; }

typedef struct {
    Uint len;
} bhash_t;

typedef struct {
    bhash_t *a;
} bhash2_t;

typedef struct {
    snode_t *node;
} bhash_node_t;

typedef bool (*equal_t)(const void *object, const void *cmp_para);
bool bhash_init(bhash2_t *p, Int len); // len是2的幂
void bhash_free(bhash2_t *p, free_t func);
byte *bhash_push(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, Int obj_bytes, bool *exist);
byte *bhash_find(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para);
byte *bhash_find_x(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, bhash_node_t *node);
byte *bhash_push_x(bhash_node_t node, Int obj_bytes);

#if __LIT_ENDIAN__
inline uint16 host_16_to_le(uint16 n) { return n; }
inline uint32 host_24_to_le(uint32 n) { return n; }
inline uint32 host_32_to_le(uint32 n) { return n; }
inline uint64 host_64_to_le(uint32 n) { return n; }
inline uint16 le_16_to_host(uint16 n) { return n; }
inline uint32 le_24_to_host(uint32 n) { return n; }
inline uint32 le_32_to_host(uint32 n) { return n; }
inline uint64 le_64_to_host(uint64 n) { return n; }
inline uint16 host_16_to_be(uint16 n) { return ((n & 0xff) << 8) | (n >> 8); }
inline uint32 host_24_to_be(uint32 n) { return ((n & 0xff) << 16) | (n & 0xff00) | ((n & 0xff0000) >> 16); }
inline uint32 host_32_to_be(uint32 n) { return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24); }
inline uint64 host_64_to_be(uint32 n) { return (((uint64)host_32_to_be(n & 0xffffffff)) << 32) | host_32_to_be(n >> 32); }
inline uint16 be_16_to_host(uint16 n) { return host_16_to_be(n); }
inline uint32 be_24_to_host(uint32 n) { return host_24_to_be(n); }
inline uint32 be_32_to_host(uint32 n) { return host_32_to_be(n); }
inline uint64 be_64_to_host(uint64 n) { return host_64_to_be(n); }
#else
inline uint16 host_16_to_le(uint16 n) { return ((n & 0xff) << 8) | (n >> 8); }
inline uint32 host_24_to_le(uint32 n) { return ((n & 0xff) << 16) | (n & 0xff00) | ((n & 0xff0000) >> 16); }
inline uint32 host_32_to_le(uint32 n) { return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24); }
inline uint64 host_64_to_le(uint32 n) { return (((uint64)host_32_to_le(n & 0xffffffff)) << 32) | host_32_to_le(n >> 32); }
inline uint16 le_16_to_host(uint16 n) { return host_16_to_le(n); }
inline uint32 le_24_to_host(uint32 n) { return host_24_to_le(n); }
inline uint32 le_32_to_host(uint32 n) { return host_32_to_le(n); }
inline uint64 le_64_to_host(uint64 n) { return host_64_to_le(n); }
inline uint16 host_16_to_be(uint16 n) { return n; }
inline uint32 host_24_to_be(uint32 n) { return n; }
inline uint32 host_32_to_be(uint32 n) { return n; }
inline uint64 host_64_to_be(uint64 n) { return n; }
inline uint16 be_16_to_host(uint16 n) { return n; }
inline uint32 be_24_to_host(uint32 n) { return n; }
inline uint32 be_32_to_host(uint32 n) { return n; }
inline uint32 be_64_to_host(uint32 n) { return n; }
#endif

#define HOST_BYTE(a) ((byte)((a)&0xff))
#define HOST_16_LE_BYTES(a) HOST_BYTE(a), HOST_BYTE((a)>>8)
#define HOST_24_LE_BYTES(a) HOST_16_LE_BYTES(a), HOST_BYTE((a)>>16)
#define HOST_32_LE_BYTES(a) HOST_16_LE_BYTES(a), HOST_16_LE_BYTES((a)>>16)
#define HOST_64_LE_BYTES(a) HOST_32_LE_BYTES(a), HOST_32_LE_BYTES((a)>>32)
#define HOST_16_BE_BYTES(a) HOST_BYTE((a)>>8), HOST_BYTE(a)
#define HOST_24_BE_BYTES(a) HOST_BYTE((a)>>16), HOST_16_BE_BYTES(a)
#define HOST_32_BE_BYTES(a) HOST_16_BE_BYTES((a)>>16), HOST_16_BE_BYTES(a)
#define HOST_64_BE_BYTES(a) HOST_32_BE_BYTES((a)>>32), HOST_32_BE_BYTES(a)

inline void host_16_to_lp(uint16 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
}

inline void host_24_to_lp(uint32 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[2] = HOST_BYTE(n >> 16);
}

inline void host_32_to_lp(uint32 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[2] = HOST_BYTE(n >> 16);
    p[3] = HOST_BYTE(n >> 24);
}

inline void host_64_to_lp(uint64 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[2] = HOST_BYTE(n >> 16);
    p[3] = HOST_BYTE(n >> 24);
    p[4] = HOST_BYTE(n >> 32);
    p[5] = HOST_BYTE(n >> 40);
    p[6] = HOST_BYTE(n >> 48);
    p[7] = HOST_BYTE(n >> 56);
}

inline void host_16_to_bp(uint16 n, byte *p) {
    p[1] = HOST_BYTE(n);
    p[0] = HOST_BYTE(n >> 8);
}

inline void host_24_to_bp(uint32 n, byte *p) {
    p[2] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[0] = HOST_BYTE(n >> 16);
}

inline void host_32_to_bp(uint32 n, byte *p) {
    p[3] = HOST_BYTE(n);
    p[2] = HOST_BYTE(n >> 8);
    p[1] = HOST_BYTE(n >> 16);
    p[0] = HOST_BYTE(n >> 24);
}

inline void host_64_to_bp(uint64 n, byte *p) {
    p[7] = HOST_BYTE(n);
    p[6] = HOST_BYTE(n >> 8);
    p[5] = HOST_BYTE(n >> 16);
    p[4] = HOST_BYTE(n >> 24);
    p[3] = HOST_BYTE(n >> 32);
    p[2] = HOST_BYTE(n >> 40);
    p[1] = HOST_BYTE(n >> 48);
    p[0] = HOST_BYTE(n >> 56);
}

inline uint16 lp_16_to_host(byte *p) {
    return (((uint16)p[1]) << 8) | p[0];
}

inline uint32 lp_24_to_host(byte *p) {
    return (((uint32)p[2]) << 16) | (((uint32)p[1]) << 8) | p[0];
}

inline uint32 lp_32_to_host(byte *p) {
    return (((uint32)p[3]) << 24) | (((uint32)p[2]) << 16) | (((uint32)p[1]) << 8) | p[0];
}

inline uint64 lp_64_to_host(byte *p) {
    return
        (((uint64)p[7]) << 56) | (((uint64)p[6]) << 48) |
        (((uint64)p[5]) << 40) | (((uint64)p[4]) << 32) |
        (((uint64)p[3]) << 24) | (((uint64)p[2]) << 16) |
        (((uint64)p[1]) << 8)  | p[0];
}

inline uint16 bp_16_to_host(byte *p) {
    return (((uint16)p[0]) << 8) | p[1];
}

inline uint32 bp_24_to_host(byte *p) {
    return (((uint32)p[0]) << 16) | (((uint32)p[1]) << 8) | p[2];
}

inline uint32 bp_32_to_host(byte *p) {
    return (((uint32)p[0]) << 24) | (((uint32)p[1]) << 16) | (((uint32)p[2]) << 8) | p[3];
}

inline uint64 bp_64_to_host(byte *p) {
    return
        (((uint64)p[0]) << 56) | (((uint64)p[1]) << 48) |
        (((uint64)p[2]) << 40) | (((uint64)p[3]) << 32) |
        (((uint64)p[4]) << 24) | (((uint64)p[5]) << 16) |
        (((uint64)p[6]) << 8)  | p[7];
}

// (~pow_2_sub_1) 相当于 (-pow_2)
#define ROUND_POW2(T, n, pow_2_sub_1) (((n) + (pow_2_sub_1)) & (~((T)(pow_2_sub_1))))
#define ROUND_PAD(n, pow_2_sub_1) ((((pow_2_sub_1) + 1) - ((n) & (pow_2_sub_1))) & (pow_2_sub_1))
inline uint8 round_up_uint8(uint8 n, uint8 a) { return ROUND_POW2(uint8, n, a); }
inline uint16 round_up_uint16(uint16 n, uint16 a) { return ROUND_POW2(uint16, n, a); }
inline uint32 round_up_uint32(uint32 n, uint32 a) { return ROUND_POW2(uint32, n, a); }
inline uint64 round_up_uint64(uint64 n, uint64 a) { return ROUND_POW2(uint64, n, a); }

#endif /* CHAPL_BUILTIN_DECL_H */
