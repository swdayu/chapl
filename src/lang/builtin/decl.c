#include "internal/decl.h"

__DECL_THREAD Error g_tl_error;

#if CONFIG_RT_ERROR_STRING
const byte* g_builtin_errors[BUILTIN_NUM_ERRORS] = {
    "SUCCESS",
    "ERROR",
#define ERROR_MAPPING(ID, STR) STR,
#include "builtin/error.h"
#undef ERROR_MAPPING
};
#endif

#if CONFIG_RT_FILE_STRING
static const byte* g_file_strings[] = {
#define FILE_MAPPING(ID, STR, LEVEL) STR,
#include "internal/init.h"
#undef FILE_MAPPING
};
#endif

void assertfault_(uint16 file, int line)
{
#if CONFIG_RT_FILE_STRING
    printf("assert fault: Ln%d %s\n", line, g_file_strings[file]);
#else
    printf("assert fault: Ln%d %02x\n", line, file);
#endif
    exit(1);
}

void logtrace0_(Error err, uint32 file_err, uint32 line)
{
    const byte* err_str = nil;
    strid_t file = LOG_FILE(file_err);

#if CONFIG_RT_FILE_STRING
    if (err_str) {
        printf("[%c] %s Ln%04d: %s\n", LOG_CHAR(line), g_file_strings[file], LOG_LINE(line), err_str);
    } else {
        printf("[%c] %s Ln%04d: %02x\n", LOG_CHAR(line), g_file_strings[file], LOG_LINE(line), (uint32)err);
    }
#else
    if (err_str) {
        printf("[%c] %02x Ln%04d: %s\n", LOG_CHAR(line), file, LOG_LINE(line), err_str);
    } else {
        printf("[%c] %02x Ln%04d: %02x\n", LOG_CHAR(line), file, LOG_LINE(line), (uint32)err);
    }
#endif
}

void logtracen_(Error err, uint32 file_err, uint32 argn_line, ...)
{

}

typedef struct {
    byte* a;
} bufhead_t;

Uint *bufhead_init(void *b, Uint cap, Uint N)
{
    byte *p = (byte *)malloc(cap);
    if (p) {
        ((bufhead_t *)b)->a = p;
    } else {
        memset(b, 0, N);
    }
    return (Uint *)p;
}

void bufhead_free(void *b, Uint N)
{
    void *p = ((bufhead_t *)b)->a;
    if (p) {
        free(p);
    }
    memset(b, 0, N);
}

bool buffix_init(buffix_t *b, Uint size)
{
    Uint *p = bufhead_init(b, sizeof(Uint) + size, sizeof(buffix_t));
    if (p) {
        *p = size;
        b->a = (byte *)(p + 1);
        b->cur = b->a;
        return true;
    }
    return false;
}

void buffix_free(buffix_t *b)
{
    if (b->a) {
        free(buffix_realaddr(b));
        b->a = 0;
    }
    b->cur = 0;
}

bool buffer_init(buffer_t *b, Uint cap)
{
    if (bufhead_init(b, cap, sizeof(buffer_t))) {
        b->cap = cap;
        b->len = 0;
        return true;
    }
    return false;
}

void buffer_free(buffer_t *b)
{
    bufhead_free(b, sizeof(buffer_t));
}

void buffer_reset(buffer_t *b)
{
    b->len = 0;
}

void buffer_push(buffer_t *b, const byte* a, Uint n)
{
    Uint len2;
    void *p;
    if (!b->a || !a || !n) {
        return;
    }
    len2 = b->len + n;
    if (len2 <= b->len) {
        return;
    }
    if (b->cap < len2) {
        // void* realloc (void* ptr, size_t size);
        //  返回的指针可能跟传入的 ptr 不同，因为可能移动内存位置
        //  如果移动了位置，旧的内容会拷贝到新的空间中，扩大的空间内容是不确定的
        //  如果传入的 ptr 为空，相当于 malloc(size)
        //  如果传入的 size 为零，相当于 free(ptr)，此时返回的指针可能为空，也可能指向不能解引用的内存位置
        //  当 size 不为零的情况下，如果返回空指针表示分配失败，ptr 指向的旧空间仍然有效
        p = realloc(b->a, len2);
        if (!p) {
            return;
        }
        b->a = (byte *)p;
        b->cap = len2;
    }
    memcpy(b->a + b->len, a, n);
}

alist_hash_t *alist_hash_init(Uint len)
{
    Uint alloc;
    alist_hash_t *p;
    if (len < 2) {
        len = 2;
    }
    alloc = sizeof(alist_hash_t) + len * sizeof(snode_t);
    p = (alist_hash_t *)malloc(alloc);
    if (p) {
        memset(p, 0, alloc);
        p->len = len; // len 必须是 2 的幂
    } else {
        p->len = 0;
    }
    return p;
}

void alist_hash_free(alist_hash_t *a, free_t func)
{
    if (!a) return;
    Uint i = 0;
    Uint n = a->len + 1;
    snode_t *head = (snode_t *)(a + 1);
    snode_t *p = 0;
    for (; i < n; i += 1) {
        while (head->next) {
            p = head->next;
            head->next = p->next;
            if (func) {
                func(p + 1);
            }
            free(p);
        }
        head += 1;
    }
    free(a);
}

byte *alit_hash_push(alist_hash_t *a, uint32 hash, equal_t eq, const void *cmp_para, Uint obj_bytes, bool *exist)
{
    snode_t *head = (snode_t *)(a + 1);
    snode_t *node = 0;
    hash &= (a->len - 1);
    head += hash;
    if (exist) *exist = 0;
label_loop:
    if (!head->next) {
        Uint alloc = sizeof(snode_t) + obj_bytes;
        node = (snode_t *)malloc(alloc);
        if (node) {
            memset(node, 0, alloc);
            head->next = node;
            return (byte *)(node + 1);
        }
        return 0;
    }
    if (eq((byte *)(head->next + 1), cmp_para)) {
        if (exist) *exist = 1;
        return (byte *)(head->next + 1);
    }
    head = head->next;
    goto label_loop;
}

byte *alist_hash_find(alist_hash_t *a, uint32 hash, equal_t eq, const void *cmp_para)
{
    snode_t *head = (snode_t *)(a + 1);
    snode_t *node = 0;
    hash &= (a->len - 1);
    head += hash;
label_loop:
    if (!head->next) {
        return 0;
    }
    if (eq((byte *)(head->next + 1), cmp_para)) {
        return (byte *)(head->next + 1);
    }
    head = head->next;
    goto label_loop;
}
