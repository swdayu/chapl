#include "internal/decl.h"

__DECL_THREAD Error error_g;

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

void assertfault_(uint16 file, int line)
{
#if CONFIG_RT_FILE_STRING
    printf("assert fault: Ln%d %s\n", line, file_strings_g[file]);
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
        printf("[%c] %s Ln%04d: %s\n", LOG_CHAR(line), file_strings_g[file], LOG_LINE(line), err_str);
    } else {
        printf("[%c] %s Ln%04d: %02x\n", LOG_CHAR(line), file_strings_g[file], LOG_LINE(line), (uint32)err);
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

static array_t *arrayinit_(array2_t *a, Uint elt_bytes, Uint elt_count, Uint N, Uint elf_off)
{
    Uint bytes;
    array_t *p;
    elt_bytes = elt_bytes ? elt_bytes : 1;
    bytes = elt_bytes * elt_count;
    bytes = bytes ? bytes : elt_bytes;
    p = (array_t *)malloc(N + bytes);
    if (p) {
        memset(p, 0, N);
        p->cap = elt_count;
        a->a = p;
        if (elf_off) {
            ((Uint *)p)[elf_off] = elt_bytes;
            *(((Uint*)(((byte*)p)+N))-1) = elt_bytes;
        }
    } else {
        a->a = 0;
    }
    return p;
}

bool arrfix_init(arrfix2_t *a, Uint len)
{
    return arrayinit_((array2_t *)a, 1, len, sizeof(arrfix_t), 0) != 0;
}

bool arrfix_ex_init(arrfix2_ex_t *p, Uint elt_bytes, Uint elt_count)
{
    return arrayinit_((array2_t *)p, elt_bytes, elt_count, sizeof(arrfix_ex_t), 1) != 0;
}

bool array_init(array2_t *a, Uint cap)
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

static bool arraypush_(array2_t *a2, const byte* data, Uint elt_count, Uint expand, Uint elt_bytes, Uint N)
{
    array_t *a = a2->a;
    array_t *p;
    Uint bytes = elt_count * elt_bytes;
    Uint cap_bytes, len_bytes, len2_bytes, len2;
    if (!a || !data || !bytes) {
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
        expand = (expand ? (len2 + expand) : (len2 * 2));
        p = (array_t *)realloc(a, N + expand * elt_bytes);
        if (!p) {
            return false;
        }
        p->cap = expand;
        a = a2->a = p;
    }
    memcpy(array_end(a), data, bytes);
    a->len = len2;
    return true;
}

bool array_push(array2_t *a2, const byte* data, Uint len, Uint expand)
{
    return arraypush_(a2, data, len, expand, 1, sizeof(array_t));
}

bool array_ex_init(array2_ex_t *a, Uint elt_bytes, Uint elt_count)
{
    return arrayinit_((array2_t *)a, elt_bytes, elt_count, sizeof(array_ex_t), 2) != 0;
}

bool array_ex_push(array2_ex_t *a, const byte* data, Uint expand)
{
    return arraypush_((array2_t *)a, data, 1, expand, a->a->elt, sizeof(array_ex_t));
}

bool buffix_init(buffix2_t *b, Uint cap)
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

byte *bufferinit_(buffer_head_t *b, Uint cap, bool cap_filed_uint, Uint N)
{
    byte *p;
    cap = cap ? cap : 1;
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

void bufferfree_(buffer_head_t *b, Uint N)
{
    if (b->a) free(b->a);
    memset(b, 0, N);
}

bool buffer_init(buffer_t *b, Uint cap)
{
    return bufferinit_((buffer_head_t *)b, cap, true, sizeof(buffer_t)) != 0;
}

void buffer_free(buffer_t *b)
{
    bufferfree_(b, sizeof(buffer_t));
}

bool buffer_push(buffer_t *b, const byte* a, Uint n, Uint expand)
{
    Uint len2;
    void *p;
    if (!b->a || !a || !n) {
        return false;
    }
    len2 = b->len + n;
    if (len2 <= b->len) {
        return false;
    }
    if (b->cap < len2) {
        // void* realloc (void* ptr, size_t size);
        //  返回的指针可能跟传入的 ptr 不同，因为可能移动内存位置
        //  如果移动了位置，旧的内容会拷贝到新的空间中，扩大的空间内容是不确定的
        //  如果传入的 ptr 为空，相当于 malloc(size)
        //  如果传入的 size 为零，相当于 free(ptr)，此时返回的指针可能为空，也可能指向不能解引用的内存位置
        //  当 size 不为零的情况下，如果返回空指针表示分配失败，ptr 指向的旧空间仍然有效
        expand = (expand ? (len2 + expand) : (len2 * 2));
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

bool bhash_init(bhash2_t *p, Uint len)
{
    Uint alloc;
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
    Uint i = 0;
    Uint n = a->len + 1;
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

byte *bhash_push(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para, Uint obj_bytes, bool *exist)
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

byte *bhash_find(bhash_t *a, uint32 hash, equal_t eq, const void *cmp_para)
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
