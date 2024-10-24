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
#define true 1
#define false 0
typedef uint8 bool;
typedef uint8 byte;
typedef uint32 rune;
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
        __CURR_FILE__ = FILE_ID, \
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

#define X_FILE(err) (__CURR_FILE__ << 16)
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

typedef struct {
    snode_t *top;
} stack_t;

typedef void (*free_t)(void *object);
byte *stack_push(stack_t *s, Int obj_bytes);
bool stack_pop(stack_t *s, free_t func);
void stack_free(stack_t *s, free_t func);
inline void stack_init(stack_t *s) { s->top = 0; }
inline bool stack_empty(stack_t *s) { return !s->top; }
inline byte *stack_top(stack_t *s) { return stack_empty(s) ? 0 : (byte*)(s->top+1); }

typedef struct {
    Uint len;
} bhash_t;

typedef struct {
    bhash_t *a;
} bhash2_t;

typedef bool (*equal_t)(const void *object, const void *cmp_para);
bool bhash_init(bhash2_t *p, Int len); // len是2的幂
void bhash_free(bhash2_t *p, free_t func);
byte *bhash_push(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, Int obj_bytes, bool *exist);
byte *bhash_find(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para);

#endif /* CHAPL_BUILTIN_DECL_H */
