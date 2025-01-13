#ifndef CHAPL_BUILTIN_TYPE_H
#define CHAPL_BUILTIN_TYPE_H
#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __ARCH_X86__
#include "conf/x86_types.h"
#elif __ARCH_X64__
#include "conf/x64_types.h"
#else
#include "conf/pretypes.genfile"
#endif

#define null 0

#ifndef __cplusplus
#define true 1
#define false 0
typedef uint8 bool;
#endif

typedef uint8 byte;
typedef uint32 rune; // 保存的是unicode代码点
typedef uint16 strid_t;
typedef uint16 errot;
typedef int32 intd_t;   // 默认整数类型
typedef uint32 uintd_t;
typedef intptr int96;   // 机器字长类型
typedef uintptr uint96;

const static int8 LANG_INT8_MIN = -127-1; /* 128 0x80 */
const static int8 LANG_INT8_MAX = 0x7f; /* 127 */
const static uint8 LANG_INT8_UMAX = 0xff; /* 255 */

const static int16 LANG_INT16_MIN = -32767-1; /* 32768 0x8000 */
const static int16 LANG_INT16_MAX = 0x7fff; /* 32767 */
const static uint16 LANG_INT16_UMAX = 0xffff; /* 65535 */

const static int32 LANG_INT32_MIN = -2147483647-1; /* 2147483648 0x80000000 */
const static int32 LANG_INT32_MAX = 0x7fffffff; /* 2147483647 */
const static uint32 LANG_INT32_UMAX = 0xffffffff; /* 4294967295 */

const static int64 LANG_INT64_MIN = -9223372036854775807LL-1; /* 9223372036854775808 0x8000000000000000 */
const static int64 LANG_INT64_MAX = 0x7fffffffffffffffLL; /* 9223372036854775807 */
const static uint64 LANG_INT64_UMAX = 0xffffffffffffffffULL; /* 18446744073709551615 */

#undef LANG_INT_MIN
#undef LANG_INT_MAX
#undef LANG_INT_UMAX
#undef LANG_INTW_MIN
#undef LANG_INTW_MAX
#undef LANG_INTW_UMAX

#define LANG_INT_MIN LANG_INT32_MIN
#define LANG_INT_MAX LANG_INT32_MAX
#define LANG_INT_UMAX LANG_INT32_UMAX

#define LANG_INT96_MIN LANG_INTPTR_MIN
#define LANG_INT96_MAX LANG_INTPTR_MAX
#define LANG_INT96_UMAX LANG_INTPTR_UMAX

#undef floatd_t
#undef float32
#undef float64

typedef float float32;
typedef double float64;
typedef float64 floatd_t; // 默认浮点类型

typedef struct {
    byte* a;    // 必须是第一个字段
    uint96 len: __ARCH_BITS__ - 1;
    uint96 dyn: 1;
} string_t;

bool string_init(string_t *s, const byte *a, int96 len, bool alloc);
void string_move_init(string_t *s, string_t *from);
void string_move(string_t *to, string_t *from);
string_t string_create(const byte *a, int96 len, bool alloc);
string_t string_move_create(string_t *from);
void string_free(string_t *s);
void assertfaults_(uint16 file, uint32 argn_line, string_t s, ...);
void logtraces_(errot err, uint32 file_err, uint32 argn_line, string_t s, ...);
inline string_t strnull() { string_t s = {0,0,0}; return s; }
inline string_t strfend(const byte *f, const byte *e) { return string_create(f, e - f, 0); }
inline string_t strflen(const byte *f, int96 len) { return string_create(f, len, 0); }
inline string_t strfrom(const char *s) { string_t a = {(byte*)s, s ? strlen(s) : 0, 0}; return a; }
inline bool string_empty(string_t *s) { return !s->len; }
inline uint96 string_len(string_t *s) { return s->len; }
inline byte *string_data(string_t *s) { return s->a; }

typedef struct {
    uint96 len;
} strfix_t;

typedef struct {
    strfix_t *a;
} strfix2_t;

bool strfix_init(strfix2_t *s, int96 len);
void strfix_free(strfix2_t *s);
inline uint96 strfix_len(strfix_t *a) { return a->len; }
inline byte *strfix_data(strfix_t *a) { return (byte*)(a+1); }

typedef struct {
    uint96 len;
} arrfix_t;

typedef struct {
    arrfix_t *a;
} arrfix2_t;

bool arrfix_init(arrfix2_t *a, int96 len);
inline uint96 arrfix_len(arrfix_t *a) { return a->len; }
inline byte *arrfix_data(arrfix_t *a) { return (byte *)(a + 1); }

typedef struct {
    uint16 len;
} arrfix_16_t;

typedef struct {
    uint8 len;
} arrfix_8_t;

typedef struct {
    uint96 len;   // 必须是第一个字段
    uint96 elt;   // 必须是第二个字段
} arrfix_ex_t;

typedef struct {
    arrfix_ex_t *a;
} arrfix2_ex_t; // elt版本的一个缺点是指针步进的计数不是乘以一个常量而是一个运行时值

bool arrfix_ex_init(arrfix2_ex_t *p, int96 elt_bytes, int96 elt_count);
inline uint96 arrfix_ex_len(arrfix_ex_t *a) { return a->len; } // 元素个数
inline byte *arrfix_ex_data(arrfix_ex_t *a) { return (byte*)(a+1); } // 数组数据
inline byte *arrfix_ex_at(arrfix_ex_t *a, int96 i) { return (arrfix_ex_data(a) + i * a->elt); }
inline byte *arrfix_ex_at_n(arrfix_ex_t *a, int96 i, int96 N) { return (arrfix_ex_data(a) + i * N); }

typedef struct {
    uint16 len;
    uint16 elt;
} arrfix_e16_t;

typedef struct {
    uint8 len;
    uint8 elt;
} arrfix_e8_t;

typedef struct {
    uint96 cap;   // 必须是第一个字段
    uint96 len;   // 必须是第二个字段
} array_t;

typedef struct {
    array_t *a;
} array2_t;

bool array_init(array2_t *a, int96 cap);
void array_free(array2_t *a);
bool array_push(array2_t *a, const byte* p, int96 len, int96 expand);
inline void arrfix_free(arrfix2_t *a) { array_free((array2_t *)a); }
inline void arrfix_ex_free(arrfix2_ex_t *a) { array_free((array2_t *)a); }
inline bool array_push_s(array2_t *a, const string_t *s, int96 expand) { return array_push(a, s->a, s->len, expand); }
inline void array_clear(array_t *a) { a->len = 0; }
inline uint96 array_cap(array_t *a) { return a->cap; }
inline uint96 array_len(array_t *a) { return a->len; }
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
    uint96 cap;   // 必须是第一个字段
    uint96 len;   // 必须是第二个字段
    uint96 elt;
} array_ex_t;

typedef struct {
    array_ex_t *a;
} array2_ex_t;

bool array_ex_init(array2_ex_t *a, int96 elt_bytes, int96 elt_count);
bool array_ex_push(array2_ex_t *a, const byte* p, int96 expand);
inline void array_ex_free(array2_ex_t *a) { array_free((array2_t *)a); }
inline void array_ex_clear(array_ex_t *a) { a->len = 0; }
inline uint96 array_ex_cap(array_ex_t *a) { return a->cap; }
inline uint96 array_ex_len(array_ex_t *a) { return a->len; }
inline bool array_ex_empty(array_ex_t *a) { return !array_ex_len(a); }
inline byte *array_ex_data(array_ex_t *a) { return (byte *)(a + 1); }
inline byte *array_ex_end(array_ex_t *a) { return array_ex_data(a) + a->len * a->elt; }
inline byte *array_ex_at(array_ex_t *a, int96 i) { return array_ex_data(a) + i * a->elt; }
inline byte *array_ex_at_n(array_ex_t *a, int96 i, int96 N) { return array_ex_data(a) + i * N; }

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
    uint96 cap;
    uint96 len;
    uint96 start;
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
    uint96 cap;
    uint96 len;
    uint96 start;
    uint96 elt;
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
    uint96 cap;   // 必须是第一个字段
    byte *cur;
} buffix_t;

typedef struct {
    buffix_t *a;
} buffix2_t;

bool buffix_init(buffix2_t *b, int96 cap);
inline void buffix_free(buffix2_t *b) { array_free((array2_t *)b); }
inline void buffix_init_inplace(buffix_t *a, int96 cap) { a->cap = cap; a->cur = (byte*)(a + 1); }
inline uint96 buffix_cap(buffix_t *a) { return a->cap; }
inline byte *buffix_data(buffix_t *a) { return (byte *)(a + 1); }
inline byte *buffix_cur(buffix_t *a) { return a->cur; }

typedef struct {
    byte* a;    // 必须是第一个字段
    uint96 cap;   // 必须是第二个字段
    uint96 len;
} buffer_t;

bool buffer_init(buffer_t *b, int96 cap);
void buffer_move_init(buffer_t *b, buffer_t *from);
void buffer_move(buffer_t *to, buffer_t *from);
string_t buffer_move_to_string(buffer_t *b);
void buffer_free(buffer_t *b);
bool buffer_push(buffer_t *b, const byte* a, int96 n, int96 expand);
bool buffer_put(buffer_t *b, byte a, int96 expand);
void buffer_pop(buffer_t *b, int96 n);
inline string_t string_ref_buffer(buffer_t *b) { string_t s = {b->a, b->len, 0}; return s; }
inline void buffer_clear(buffer_t *b) { b->len = 0; }
inline uint96 buffer_cap(buffer_t *b) { return b->cap; }
inline uint96 buffer_len(buffer_t *b) { return b->len; }
inline byte *buffer_data(buffer_t *b) { return b->a; }
inline bool buffer_empty(buffer_t *b) { return !buffer_len(b); }
inline bool buffer_eq(buffer_t *b, const byte *a, int96 len) { return ((a != 0) && ((int96)buffer_len(b) == len) && (memcmp(buffer_data(b), a, len) == 0)); }
inline bool buffer_eq_s(buffer_t *b, const string_t *s) { return buffer_eq(b, s->a, s->len); }

typedef struct {
    byte* a;    // 必须是第一个字段
    uint16 cap; // 必须是第二个字段
    uint16 len;
} buffer_16_t;

typedef struct {
    byte* a;    // 必须是第一个字段
    uint96 cap;   // 必须是第二个字段
    uint96 len;
    uint96 elt;
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
    uint96 cap;   // 必须是第二个字段
    uint96 len;
    uint96 start;
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
    uint96 cap;   // 必须是第二个字段
    uint96 len;
    uint96 start;
    uint96 elt;
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

typedef struct {  // 置零初始化
    snode_t head;
    snode_t *tail;
} slist_t;

struct slist_it;
typedef void (*free_t)(void *object);
byte *slist_push_front(slist_t *l, int96 obj_bytes);
byte *slist_push_back(slist_t *l, int96 obj_bytes);
byte *slist_front(slist_t *l);
byte *slist_back(slist_t *l);
byte *slist_pop_front_node(slist_t *l);
bool slist_pop_front(slist_t *l, free_t func);
void slist_free(slist_t *l, free_t func);
inline struct slist_it *slist_begin(slist_t *l) { return (struct slist_it *)(l ? l->head.next : 0); }
inline struct slist_it *slist_end(slist_t *l) { return 0; }
inline struct slist_it *slist_next(struct slist_it *p) { return (struct slist_it *)(((snode_t *)p)->next); }
inline byte *slist_it_get(struct slist_it *p) { return (p ? ((byte *)p) + sizeof(snode_t*) : 0); }

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

byte *stack_push(stack_t *s, int96 obj_bytes);
byte *stack_push_it(stack_t *s, struct stack_it *it);
byte *stack_push_node(stack_t *s, const byte *node);
byte *stack_insert_after(struct stack_it *p, int96 obj_bytes);
byte *stack_insert(struct stack_before_it *p, int96 obj_bytes);
byte *stack_replace(struct stack_before_it *p, struct stack_it *node, free_t func);

bool stack_pop(stack_t *s, free_t func);
void stack_free(stack_t *s, free_t func);
struct stack_it *stack_new_it(int96 obj_bytes);
byte *stack_new_node(int96 obj_bytes);
void stack_free_node(const byte *node);
void stack_delete_it(struct stack_it *it, free_t func);
inline bool stack_empty(stack_t *s) { return (!s || !s->top); }
inline byte *stack_top(stack_t *s) { return (byte*)(s ? s->top + 1 : 0); }
inline struct stack_it *stack_begin(stack_t *s) { return (struct stack_it *)(s ? s->top : 0); }
inline struct stack_before_it *stack_before_begin(stack_t *s) { return (struct stack_before_it *)s; }
inline struct stack_it *stack_end(stack_t *s) { return 0; }
inline struct stack_it *stack_next(struct stack_it *p) { return (struct stack_it *)(((snode_t *)p)->next); }
inline struct stack_before_it *stack_before_next(struct stack_before_it *p) { return (struct stack_before_it *)(((snode_t *)p)->next); }
inline byte *stack_node_next(const byte *node) { snode_t *p = ((snode_t *)(node - sizeof(snode_t)))->next; return p ? (byte *)(p + 1) : 0; }
inline bool stack_in_range(struct stack_before_it *b, struct stack_it *it) { return (struct stack_it *)(((snode_t *)b)->next) != it; }
inline byte *stack_it_get(struct stack_it *p) { return p ? ((byte *)p) + sizeof(snode_t*) : 0; }
inline byte *stack_before_it_get(struct stack_before_it *p) { return p ? stack_it_get((struct stack_it *)(((snode_t *)p)->next)) : null; }

typedef struct {
    uint96 len;
} bhash_t;

typedef struct {
    bhash_t *a;
} bhash2_t;

typedef struct {
    snode_t *node;
} bhash_node_t;

typedef bool (*equal_t)(const void *object, const void *cmp_para);
bool bhash_init(bhash2_t *p, int96 len); // len是2的幂
void bhash_free(bhash2_t *p, free_t func);
byte *bhash_push(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, int96 obj_bytes, bool *exist);
byte *bhash_find(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para);
byte *bhash_find_x(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, bhash_node_t *node);
byte *bhash_push_x(bhash_node_t node, int96 obj_bytes);

// 将 p 开始的 len 个字节写入到 obj 中，返回成功写入的字节数（0 <= n <= len），如果
// len > 0 而 n < len，表示在写完之前被某个错误提前打断，此时必须返回一个非 null 值的
// error。
typedef int96 (*writer_t)(void *obj, const byte *p, int96 len);

// 将 p 开始的 len 个字节写入到 obj 偏移 off 字节处，返回写入的字节数（0 <= n <= len），
// 如果 len > 0 而 n < len，表示在写完之前被某个错误提前打断，此时必须返回一个非 null
// 值的 error。目标对象 obj 在偏移 off 字节写入数据后，其数据流位置必须还原到偏移之前
// 的原位置。
typedef int96 (*writerat_t)(void *obj, int96 off, const byte *p, int96 len);

// 将数据写入到 wr 中，直到没有更多的数据，或者因为错误提前结束；返回成功写入的字节数，
// 如果遇到错误通过 err 返回。
typedef int96 (*writerto_t)(void *obj, writer_t wr);

#ifdef __cplusplus
}
#endif
#endif /* CHAPL_BUILTIN_TYPE_H */
