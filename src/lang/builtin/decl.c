#define __CURR_FILE__ STRID_LANG_DECL
#include "internal/decl.h"

#if CONFIG_RT_ERROR_STRING
const byte* builtin_errors_g[BUILTIN_NUM_ERRORS] = {
    "SUCCESS",
    "ERROR",
#define ERROR_MAPPING(ID, STR) STR,
#include "builtin/error.h"
#undef ERROR_MAPPING
};
#endif

#if CONFIG_RT_FILE_STRING
static const byte* file_strings_g[] = {
#define FILE_MAPPING(ID, STR, LEVEL) STR,
#include "internal/init.h"
#undef FILE_MAPPING
};
#endif

void assertfault_(uint16 file, uint32 line)
{
#if CONFIG_RT_FILE_STRING
    printf("[A] %s#%04d\n", file_strings_g[file], line);
#else
    printf("[A] %02x#%04d\n", file, line);
#endif
    exit(1);
}

void assertfaultx_(uint16 file, uint32 argn_line, ...)
{
    uint32 argn = LOG_ARGN(argn_line);
    uint32 a, i = 0;
    printf("[A] %02x#%04d", file, LOG_LINE(argn_line));
    va_list vl;
    va_start(vl, argn_line);
    for (; i < argn; ++i) {
        a = va_arg(vl, uint32);
        printf(" %02x", a);
    }
    va_end(vl);
    printf("\n");
    exit(1);
}

void assertfaults_(uint16 file, uint32 argn_line, string_t s, ...)
{
    uint32 argn = LOG_ARGN(argn_line);
    uint32 a, i = 0;
    printf("[A] %02x#%04d \"", file, LOG_LINE(argn_line));
    for (; i < s.len; ++i) {
        putchar(s.a[i]);
    }
    printf("\"");
    va_list vl;
    va_start(vl, s);
    for (i = 0; i < argn; ++i) {
        a = va_arg(vl, uint32);
        printf(" %02x", a);
    }
    va_end(vl);
    printf("\n");
    exit(1);
}

void logtrace_(Error err, uint32 file_err, uint32 line)
{
    const byte* err_str = null;
    strid_t file = LOG_FILE(file_err);

#if CONFIG_RT_FILE_STRING
    if (err_str) {
        printf("[%c] %s#%04d %s\n", LOG_CHAR(line), file_strings_g[file], LOG_LINE(line), err_str);
    } else {
        printf("[%c] %s#%04d %02X\n", LOG_CHAR(line), file_strings_g[file], LOG_LINE(line), (uint32)err);
    }
#else
    if (err_str) {
        printf("[%c] %02x#%04d %s\n", LOG_CHAR(line), file, LOG_LINE(line), err_str);
    } else {
        printf("[%c] %02x#%04d %02X\n", LOG_CHAR(line), file, LOG_LINE(line), (uint32)err);
    }
#endif
}

void logtracex_(Error err, uint32 file_err, uint32 argn_line, ...)
{
    uint32 argn = LOG_ARGN(argn_line);
    strid_t file = LOG_FILE(file_err);
    uint32 a, i = 0;
    printf("[%c] %02x#%04d %02X:", LOG_CHAR(argn_line), file, LOG_LINE(argn_line), (uint32)err);
    va_list vl;
    va_start(vl, argn_line);
    for (; i < argn; ++i) {
        a = va_arg(vl, uint32);
        printf(" %02x", a);
    }
    va_end(vl);
    printf("\n");
}

void logtraces_(Error err, uint32 file_err, uint32 argn_line, string_t s, ...)
{
    uint32 argn = LOG_ARGN(argn_line);
    strid_t file = LOG_FILE(file_err);
    uint32 a, i = 0;
    printf("[%c] %02x#%04d %02X:", LOG_CHAR(argn_line), file, LOG_LINE(argn_line), (uint32)err);
    printf(" \"");
    for (; i < s.len; ++i) {
        putchar(s.a[i]);
    }
    printf("\"");
    va_list vl;
    va_start(vl, s);
    for (i = 0; i < argn; ++i) {
        a = va_arg(vl, uint32);
        printf(" %02x", a);
    }
    va_end(vl);
    printf("\n");
}

bool string_init(string_t *s, const byte *a, Int len, bool alloc)
{
    byte *p = 0;
    len = (a && len > 0) ? len : 0;
    s->len = len;
    s->dyn = 0;
    if (len) {
        if (alloc) {
            if ((p = (byte *)malloc(len))) {
                memcpy(p, a, len);
                s->dyn = 1;
            } else {
                s->len = 0;
            }
        } else {
            p = (byte *)a;
        }
    }
    s->a = p;
    return (p != 0);
}

void string_move_init(string_t *s, string_t *from)
{
    *s = *from;
    memset(from, 0, sizeof(string_t));
}

void string_move(string_t *to, string_t *from)
{
    if (to->a && to->dyn) {
        free(to->a);
    }
    string_move_init(to, from);
}

string_t string_create(const byte *a, Int len, bool alloc)
{
    string_t s;
    string_init(&s, a, len, alloc);
    return s;
}

string_t string_move_create(string_t *from)
{
    string_t s;
    string_move_init(&s, from);
    return s;
}

void string_free(string_t *s)
{
    if (s->a && s->dyn) {
        free(s->a);
    }
    memset(s, 0, sizeof(string_t));
}

static array_t *arrayinit_(array2_t *a, Int elt_bytes, Int elt_count, Int N, Int elt_off)
{
    Int bytes;
    array_t *p;
    elt_bytes = (elt_bytes > 0) ? elt_bytes : 1;
    bytes = elt_bytes * elt_count;
    bytes = (bytes > 0) ? bytes : elt_bytes;
    p = (array_t *)malloc(N + bytes);
    if (p) {
        memset(p, 0, N);
        p->cap = elt_count;
        a->a = p;
        if (elt_off) {
            ((Uint *)p)[elt_off] = elt_bytes;
        }
    } else {
        a->a = 0;
    }
    return p;
}

bool arrfix_init(arrfix2_t *a, Int len)
{
    return arrayinit_((array2_t *)a, 1, len, sizeof(arrfix_t), 0) != 0;
}

bool arrfix_ex_init(arrfix2_ex_t *p, Int elt_bytes, Int elt_count)
{
    return arrayinit_((array2_t *)p, elt_bytes, elt_count, sizeof(arrfix_ex_t), 1) != 0;
}

bool array_init(array2_t *a, Int cap)
{
    return arrayinit_(a, 1, cap, sizeof(array_t), 0) != 0;
}

void array_free(array2_t *a)
{
    if (a->a) {
        free(a->a);
        a->a = 0;
    }
}

static bool arraypush_(array2_t *a2, const byte* data, Int elt_count, Int expand, Int elt_bytes, Int N)
{
    array_t *a = a2->a;
    array_t *p;
    Int bytes = elt_count * elt_bytes;
    Int cap_bytes, len_bytes, len2_bytes, len2;
    if (!a || !data || bytes <= 0) {
        return false;
    }
    cap_bytes = a->cap * elt_bytes;
    len_bytes = a->len * elt_bytes;
    len2_bytes = len_bytes + bytes;
    len2 = a->len + elt_count;
    if (len2_bytes <= len_bytes) {
        return false;
    }
    if (cap_bytes < len2_bytes) {
        expand = (expand > 0) ? (len2 + expand) : (len2 * 2);
        p = (array_t *)realloc(a, N + expand * elt_bytes);
        if (!p) {
            return false;
        }
        p->cap = expand;
        a = a2->a = p;
    }
    memcpy(((byte*)a) + N + a->len * elt_bytes, data, bytes);
    a->len = len2;
    return true;
}

bool array_push(array2_t *a2, const byte* data, Int len, Int expand)
{
    return arraypush_(a2, data, len, expand, 1, sizeof(array_t));
}

bool array_ex_init(array2_ex_t *a, Int elt_bytes, Int elt_count)
{
    return arrayinit_((array2_t *)a, elt_bytes, elt_count, sizeof(array_ex_t), 2) != 0;
}

bool array_ex_push(array2_ex_t *a, const byte* data, Int expand)
{
    return arraypush_((array2_t *)a, data, 1, expand, a->a->elt, sizeof(array_ex_t));
}

bool buffix_init(buffix2_t *b, Int cap)
{
    buffix_t *p = (buffix_t *)arrayinit_((array2_t *)b, 1, cap, sizeof(buffix_t), 0);
    if (p) {
        p->cur = buffix_data(p);
        return true;
    }
    return false;
}

typedef struct {
    byte *a;
} buffer_head_t;

byte *bufferinit_(buffer_head_t *b, Int cap, bool cap_filed_uint, Int N)
{
    byte *p;
    cap = (cap > 0) ? cap : 1;
    p = (byte *)malloc(cap);
    if (p) {
        b->a = p;
        b += 1;
        if (cap_filed_uint) {
            *((Uint *)b) = cap;
            N -= sizeof(byte *) + sizeof(Uint);
        } else {
            *((uint16 *)b) = (uint16)cap;
            N -= sizeof(byte *) + sizeof(uint16);
        }
    }
    memset(b, 0, N);
    return p;
}

void bufferfree_(buffer_head_t *b, Int N)
{
    if (b->a) {
        free(b->a);
        memset(b, 0, N);
    }
}

bool buffer_init(buffer_t *b, Int cap)
{
    return bufferinit_((buffer_head_t *)b, cap, true, sizeof(buffer_t)) != 0;
}

void buffer_move_init(buffer_t *b, buffer_t *from)
{
    *b = *from;
    memset(from, 0, sizeof(buffer_t));
}

void buffer_move(buffer_t *to, buffer_t *from)
{
    if (to->a) free(to->a);
    buffer_move_init(to, from);
}

string_t buffer_move_to_string(buffer_t *b)
{
    string_t d = {b->a, b->len, 1};
    memset(b, 0, sizeof(buffer_t));
    return d;
}

void buffer_free(buffer_t *b)
{
    bufferfree_((buffer_head_t *)b, sizeof(buffer_t));
}

bool buffer_push(buffer_t *b, const byte* a, Int n, Int expand)
{
    Int len2;
    void *p;
    if (!b->a && !buffer_init(b, n)) {
        return false;
    }
    if (!a || n <= 0) {
        return false;
    }
    len2 = b->len + n;
    if (len2 <= (Int)b->len) {
        return false;
    }
    if ((Int)b->cap < len2) {
        // void* realloc (void* ptr, size_t size);
        //  返回的指针可能跟传入的 ptr 不同，因为可能移动内存位置
        //  如果移动了位置，旧的内容会拷贝到新的空间中，扩大的空间内容是不确定的
        //  如果传入的 ptr 为空，相当于 malloc(size)
        //  如果传入的 size 为零，相当于 free(ptr)，此时返回的指针可能为空，也可能指向不能解引用的内存位置
        //  当 size 不为零的情况下，如果返回空指针表示分配失败，ptr 指向的旧空间仍然有效
        expand = (expand > 0) ? (len2 + expand) : (len2 * 2);
        p = realloc(b->a, expand);
        if (!p) {
            return false;
        }
        b->a = (byte *)p;
        b->cap = expand;
    }
    memcpy(b->a + b->len, a, n);
    b->len = len2;
    return true;
}

bool buffer_put(buffer_t *b, byte a, Int expand)
{
    return buffer_push(b, &a, 1, expand);
}

void buffer_pop(buffer_t *b, Int n)
{
    if (n > 0 && (Int)b->len >= n) {
        b->len -= n;
    }
}

byte *slist_front(slist_t *l)
{
    snode_t *p = l->head.next;
    return p ? (byte *)(p + 1) : 0;
}

byte *slist_back(slist_t *l)
{
    snode_t *p = l->tail;
    return p ? (byte *)(p + 1) : 0;
}

bool slist_pop_front(slist_t *l, free_t func)
{
    snode_t *p = l->head.next;
    if (p) {
        l->head.next = p->next;
        if (l->tail == p) {
            l->tail = null;
        }
        if (func) {
            func(p + 1);
        }
        free(p);
        return true;
    }
    return false;
}

void slist_free(slist_t *l, free_t func){
    snode_t *n;
    while ((n = l->head.next)) {
        l->head.next = n->next;
        if (func) {
            func(n + 1);
        }
        free(n);
    }
    l->tail = null;
}

struct stack_it *stack_new_it(Int obj_bytes)
{
    Int alloc = sizeof(snode_t) + (obj_bytes > 0 ? obj_bytes : 1);
    snode_t *p = (snode_t *)malloc(alloc);
    if (p) {
        memset(p, 0, alloc);
    }
    return (struct stack_it *)p;
}

snode_t *stackinsertafter_x_(struct stack_it *it, struct stack_it *node)
{
    snode_t *p = (snode_t *)it;
    snode_t *n = (snode_t *)node;
    if (n) {
        n->next = p->next;
        p->next = n;
        return n;
    }
    return 0;
}

byte *slist_push_front(slist_t *l, Int obj_bytes)
{
    snode_t *n = stackinsertafter_x_((struct stack_it *)l, stack_new_it(obj_bytes));
    if (l->tail == null) {
        l->tail = n;
    }
    return n ? (byte *)(n + 1) : 0;
}

byte *slist_push_back(slist_t *l, Int obj_bytes)
{
    struct stack_it *p = l->tail ? (struct stack_it *)l->tail : (struct stack_it *)l;
    snode_t *n = stackinsertafter_x_(p, stack_new_it(obj_bytes));
    l->tail = n;
    return n ? (byte *)(n + 1) : 0;
}

byte *stackinsertafter_(struct stack_it *it, struct stack_it *node)
{
    snode_t *n = stackinsertafter_x_(it, node);
    return n ? (byte *)(n + 1) : 0;
}

byte *stack_push_it(stack_t *s, struct stack_it *it)
{
    return stackinsertafter_((struct stack_it *)s, it);
}

byte *stack_push(stack_t *s, Int obj_bytes)
{
    return stack_push_it(s, stack_new_it(obj_bytes));
}

byte *stack_new_node(Int obj_bytes)
{
    snode_t *n = (snode_t *)stack_new_it(obj_bytes);
    return n ? (byte *)(n + 1) : 0;
}

void stack_push_node(stack_t *s, const byte *node)
{
    stackinsertafter_((struct stack_it *)s, (struct stack_it *)(node - sizeof(snode_t)));
}

void stack_delete_it(struct stack_it *it, free_t func)
{
    snode_t *p = (snode_t *)it;
    if (p) {
        if (func) {
            func(p + 1);
        }
        free(p);
    }
}

byte *stack_insert_after(struct stack_it *it, Int obj_bytes)
{
    Int alloc = sizeof(snode_t) + (obj_bytes > 0 ? obj_bytes : 1);
    snode_t *head = (snode_t *)it;
    snode_t *first = head->next;
    snode_t *p = (snode_t *)malloc(alloc);
    if (p) {
        memset(p, 0, alloc);
        p->next = first;
        head->next = p;
        return (byte *)(p + 1);
    }
    return 0;
}

byte *stack_insert(struct stack_before_it *p, Int obj_bytes)
{
    return stackinsertafter_((struct stack_it *)p, stack_new_it(obj_bytes));
}

byte *stack_replace(struct stack_before_it *p, struct stack_it *node, free_t func)
{
    snode_t *b = (snode_t *)p;
    snode_t *it = b->next;
    b->next = it->next;
    if (func) {
        func(it + 1);
    }
    free(it);
    return stackinsertafter_((struct stack_it *)b, node);
}

bool stack_pop(stack_t *s, free_t func)
{
    snode_t *p = s->top;
    if (p) {
        s->top = p->next;
        if (func) {
            func(p + 1);
        }
        free(p);
        return true;
    }
    return false;
}

void stack_free_node(const byte *node)
{
    if (node) {
        free(node - sizeof(snode_t));
    }
}

void stack_free(stack_t *s, free_t func)
{
    snode_t *p;
    while ((p = s->top)) {
        s->top = p->next;
        if (func) {
            func(p + 1);
        }
        free(p);
    }
}

bool bhash_init(bhash2_t *p, Int len)
{
    Int alloc;
    bhash_t *a;
    if (len < 2) {
        len = 2;
    }
    alloc = sizeof(bhash_t) + len * sizeof(snode_t);
    a = (bhash_t *)malloc(alloc);
    p->a = a;
    if (a) {
        memset(a, 0, alloc);
        a->len = len; // len 必须是 2 的幂
        return true;
    }
    a->len = 0;
    return false;
}

void bhash_free(bhash2_t *p, free_t func)
{
    bhash_t *a = p->a;
    if (!a) return;
    Int i = 0;
    Int n = a->len + 1;
    snode_t *head = (snode_t *)(a + 1);
    snode_t *node = 0;
    for (; i < n; i += 1) {
        while (head->next) {
            node = head->next;
            head->next = node->next;
            if (func) {
                func(node + 1);
            }
            free(node);
        }
        head += 1;
    }
    free(a);
}

byte *bhash_push(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, Int obj_bytes, bool *exist)
{
    snode_t *head = (snode_t *)(a + 1);
    hash &= (a->len - 1);
    head += hash;
    if (exist) *exist = 0;
label_loop:
    if (!head->next) {
        return bhash_push_x((bhash_node_t){head}, obj_bytes);
    }
    if (eq((byte *)(head->next + 1), cmp_para)) {
        if (exist) *exist = 1;
        return (byte *)(head->next + 1);
    }
    head = head->next;
    goto label_loop;
}

snode_t *bhashfind_(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para)
{
    snode_t *head = (snode_t *)(a + 1);
    hash &= (a->len - 1);
    head += hash;
label_loop:
    if (!head->next) {
        return head;
    }
    if (eq((byte *)(head->next + 1), cmp_para)) {
        return head;
    }
    head = head->next;
    goto label_loop;
}

byte *bhash_find(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para)
{
    snode_t *head = bhashfind_(a, hash, eq, cmp_para);
    return (byte *)(head->next ? (head->next + 1) : 0);
}

byte *bhash_find_x(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, bhash_node_t *node)
{
    snode_t *head = bhashfind_(a, hash, eq, cmp_para);
    node->node = head;
    return (byte *)(head->next ? (head->next + 1) : 0);
}

byte *bhash_push_x(bhash_node_t p, Int obj_bytes)
{
    snode_t *head = p.node;
    snode_t *node = 0;
    Int alloc = sizeof(snode_t) + (obj_bytes > 0 ? obj_bytes : 1);
    node = (snode_t *)malloc(alloc);
    if (node) {
        memset(node, 0, alloc);
        head->next = node;
        return (byte *)(node + 1);
    }
    return 0;
}
