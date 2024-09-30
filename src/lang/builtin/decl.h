#ifndef CHAPL_BUILTIN_DECL_H
#define CHAPL_BUILTIN_DECL_H
#include <stdio.h>
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

#undef nil
#undef true
#undef false
#undef bool
#undef byte
#undef rune
#undef strid_t
#undef Error

#define nil 0
#define true (0 == 0)
#define false (0 != 0)
typedef uint8 bool;
typedef uint8 byte;
typedef int32 rune;
typedef uint16 strid_t;
typedef uintptr Error;

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

#define DEBUG_MODULE(FILE_ID) typedef enum { \
        __CURR_FILE__ = (FILE_ID) << 16, \
        __LOG_LEVEL__ = FILE_ID##_LOG_LEVEL, \
    } debug_module_enum_t;

#undef lang_assert
#undef debug_assert

#define lang_assert(e) ((e) ? ((void)0) : assertfault_(__CURR_FILE__, __LINE__))

#ifdef CONFIG_DEBUG
#define debug_assert(e) lang_assert(e)
#else
#define debug_assert(e)
#endif

void assertfault_(uint16 file, int line);
void logtrace0_(Error err, uint32 file_err, uint32 line);
void logtracen_(Error err, uint32 file_err, uint32 argn_line, ...);

#define LOG_LEVEL_F 0x00000000 // fatal panic
#define LOG_LEVEL_M 0x01000000 // main
#define LOG_LEVEL_E 0x02000000 // error
#define LOG_LEVEL_I 0x03000000 // information
#define LOG_LEVEL_D 0x04000000 // only for debug, not in release

#define X_FILE(err) __CURR_FILE__
#define X_LINE(level, argn) (argn << 27) | level | __LINE__

#define LOG_FILE(file) ((strid_t)((file) >> 16))
#define LOG_ARGN(line) ((line) >> 27)
#define LOG_LEVEL(line) (((line) >> 24) & 0x7)
#define LOG_LINE(line) ((line) & 0x00FFFFFF)
#define LOG_CHAR(line) "FmEID"[LOG_LEVEL(line)]

#define log_fatal(err) logtrace0_((err), X_FILE(err), LOG_LEVEL_F|__LINE__)
#define log_fatal_x(err, n, ...) logtracen_((err), X_FILE(err), X_LINE(LOG_LEVEL_F, (n)), __VA_ARGS__)

#if (__LOG_LEVEL__ >= LOG_LEVEL_M)
#define log_main(err) logtrace0_((err), X_FILE(err), LOG_LEVEL_M|__LINE__)
#define log_main_x(err, n, ...) logtracen_((err), X_FILE(err), X_LINE(LOG_LEVEL_M, (n)), __VA_ARGS__)
#else
#define log_main(err)
#define log_main_x(err, n, ...)
#endif

#if (__LOG_LEVEL__ >= LOG_LEVEL_E)
#define log_error(err) logtrace0_((err), X_FILE(err), LOG_LEVEL_E|__LINE__)
#define log_error_x(err, n, ...) logtracen_((err), X_FILE(err), X_LINE(LOG_LEVEL_E, (n)), __VA_ARGS__)
#else
#define log_error(err)
#define log_error_x(err, n, ...)
#endif

#if (__LOG_LEVEL__ >= LOG_LEVEL_I)
#define log_info(err) logtrace0_((err), X_FILE(err), LOG_LEVEL_I|__LINE__)
#define log_info_x(err, n, ...) logtracen_((err), X_FILE(err), X_LINE(LOG_LEVEL_I, (n)), __VA_ARGS__)
#else
#define log_info(err)
#define log_info_x(err, n, ...)
#endif

#if (__LOG_LEVEL__ >= LOG_LEVEL_D) && defined(CONFIG_DEBUG)
#define log_debug(err) logtrace0_((err), X_FILE(err), LOG_LEVEL_D|__LINE__)
#define log_debug_x(err, n, ...) logtracen_((err), X_FILE(err), X_LINE(LOG_LEVEL_D, (n)), __VA_ARGS__)
#else
#define log_debug(err)
#define log_debug_x(err, n, ...)
#endif

#undef ERROR
#undef BUILTIN_NUM_ERRORS
#undef BUILTIN_LAST_ERROR

typedef enum {
    ERROR = 1,
#define ERROR_MAPPING(ID, STR) ERROR_##ID,
#include "builtin/error.h"
#undef ERROR_MAPPING
    BUILTIN_NUM_ERRORS,
    BUILTIN_LAST_ERROR = 0xFF,
} builtin_error_enum_t;

#if CONFIG_RT_ERROR_STRING
extern const byte* g_builtin_errors[BUILTIN_NUM_ERRORS];
#define builtin_error(id) ((Error)g_builtin_errors[id])
#else
#define builtin_error(id) (((id) > ERROR) ? (((id) << 1) | ERROR) : (id))
#endif

extern __DECL_THREAD Error g_tl_error;

#undef set_last_error
#undef get_last_error

#define set_last_error(n) g_tl_error = (n)
#define get_last_error() g_tl_error

// 将 p 开始的 len 个字节写入到 obj 中，返回成功写入的字节数（0 <= n <= len），如果
// len > 0 而 n < len，表示在写完之前被某个错误提前打断，此时必须返回一个非 nil 值的
// error。
typedef Int (*Writer)(void *obj, const byte *p, Int len);

// 将 p 开始的 len 个字节写入到 obj 偏移 off 字节处，返回写入的字节数（0 <= n <= len），
// 如果 len > 0 而 n < len，表示在写完之前被某个错误提前打断，此时必须返回一个非 nil
// 值的 error。目标对象 obj 在偏移 off 字节写入数据后，其数据流位置必须还原到偏移之前
// 的原位置。
typedef Int (*WriterAt)(void *obj, Int off, const byte *p, Int len);

// 将数据写入到 wr 中，直到没有更多的数据，或者因为错误提前结束；返回成功写入的字节数，
// 如果遇到错误通过 err 返回。
typedef Int (*WriterTo)(void *obj, Writer wr);

typedef struct {
    Uint len;
    byte* s;
} string_t;

typedef struct {
    Uint len;
} arrfix_t;

typedef struct {
    Uint len;
    byte* a;
    Uint cap;
} buffer_t;

typedef struct {
    uint16 len;
    uint16 cap;
    byte* a;
} buff16_t;

typedef struct {
    Uint len;
    byte* a;
    Uint cap;
    Uint start;
} deffer_t;

typedef struct {
    uint16 len;
    uint16 cap;
    uint16 start;
    uint16 extra;
    byte* a;
} deff16_t;

typedef struct {
    Uint len;
    Uint cap;
} array_t;

typedef struct {
    uint16 len;
    uint16 cap;
} arr16_t;

typedef struct {
    Uint len;
    Uint cap;
    Uint start;
} dearr_t;

typedef struct {
    uint16 len;
    uint16 cap;
    uint16 start;
    uint16 extra;
} dea16_t;

typedef struct single_link_node {
    struct single_link_node *next;
} single_link_node_t;

typedef struct {
    single_link_node_t head;
    single_link_node_t *tail;
} single_link_list_t;

typedef struct link_node {
    struct link_node *next;
    struct link_node *prev;
} link_node_t;

typedef struct link_list {
    link_node_t head;
} link_list_t;

#endif /* CHAPL_BUILTIN_DECL_H */
