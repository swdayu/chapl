#include "direct/file.h"

#define BUF_HEAD_BYTE_CNT 32 // 能向后回退的字节数
#define FILE_BUF_MAX_SIZE 0x7ffff000
#define DEFAULT_BUFF_SIZE 512

void fileclose_(file_t *f)
{
    if (f->fd >= 0) {
        close(f->fd);
        f->fd = -1;
    }
    f->len = 0;
    f->real_eof = 1;
}

void file_reload(file_t *f, string_t s)
{
    buffix_t *b = &f->b;
    byte *arr = buffix_data(b);
    int96 len = (int96)b->cap - BUF_HEAD_BYTE_CNT;
    if ((int96)s.len < len) {
        len = (int96)s.len;
    }
    fileclose_(f);
    b->cur = arr + BUF_HEAD_BYTE_CNT;
    if (len > 0) {
        memcpy(b->cur, s.a, len);
    } else {
        len = 0;
    }
    f->len = BUF_HEAD_BYTE_CNT + len;
    f->real_eof = 0;
}

void file_reopen(file_t *f, const char *name, uint32 mode)
{
    intd_t fd;
    fileclose_(f);
    f->b.cur = ((byte *)(f + 1)) + BUF_HEAD_BYTE_CNT;
    if (*name == '-') {
        if (mode == 'r') {
            fd = 0; // stdin
        } else {
            fd = 1; // stdout
        }
    } else {
        fd = fileopen_(name, mode);
        if (fd < 0) {
            return;
        }
    }
    f->fd = fd;
    f->len = BUF_HEAD_BYTE_CNT;
    f->real_eof = 0;
}

file_t *fileopenx_(const byte *filename, int96 strlen, int32 bufsize, uint32 mode)
{
    file_t *f;
    int96 cap = (bufsize > 0) ? bufsize : DEFAULT_BUFF_SIZE;
    if (!mode) {
        cap = (strlen > cap) ? strlen : cap;
    }
    cap += BUF_HEAD_BYTE_CNT;
    if (cap > FILE_BUF_MAX_SIZE) {
        return null;
    }
    if (mode && (!filename || !filename[0])) {
        return null;
    }
    f = (file_t *)malloc(sizeof(file_t) + cap);
    if (!f) {
        return null;
    }
    f->fd = -1;
    buffix_init_inplace(&f->b, cap);
    if (mode) { // 通过文件打开模式是否为空来区分加载的是文件还是字符串
        file_reopen(f, (char *)filename, mode);
    } else {
        file_reload(f, strflen(filename, strlen));
    }
    return f;
}

file_t *file_load(string_t s, int32 bufsize)
{
    return fileopenx_(s.a, s.len, bufsize, 0);
}

file_t *file_open(const char *filename, uint32 mode, int32 bufsize)
{
    return fileopenx_((byte *)filename, 0, bufsize, mode);
}

void file_close(file_t *f)
{
    if (!f) {
        return;
    }
    fileclose_(f);
    free(f);
}

void fileblock_(file_t *f, void (*cp)(void *p, const byte *e), void *p)
{
    int32 len, bflen;
    buffix_t *b = &f->b;
    byte *arr = buffix_data(b);
    byte *cur = b->cur;
    uint96 cap = buffix_cap(b);
    f->real_eof = 1;
    if (f->fd < 0) {
        return;
    }
    bflen = cap - BUF_HEAD_BYTE_CNT;
    if (bflen <= 0) {
        return;
    }
    // 为保证unget成功，必须将对应长度内容拷贝到头部
    memmove(arr, cur - BUF_HEAD_BYTE_CNT, BUF_HEAD_BYTE_CNT);
    b->cur = arr + BUF_HEAD_BYTE_CNT;
    f->len = BUF_HEAD_BYTE_CNT;
    if (cp) {
        cp(p, cur);
    }
    len = fileread_(f->fd, bflen, b->cur);
    if (len <= 0 || len > bflen) {
        return;
    }
    f->len += len;
    if (len == bflen) {
        f->real_eof = 0; // 文件还有剩余内容可读
    }
}

int file_get_ex(file_t *f, void (*cp)(void *p, const byte *e), void *arg)
{
    buffix_t *b = &f->b;
    byte *arr = buffix_data(b);
    if (f->real_eof) {
        byte *p = b->cur + f->eof_cnt - 1;
        if (f->eof_cnt++ <= 0 && p >= arr) {
            return *p;
        }
        return CHAR_EOF;
    }
    if (b->cur >= arr + f->len) {
        fileblock_(f, cp, arg);
    }
    if (f->real_eof) {
        f->eof_cnt = 1;
        return CHAR_EOF;
    }
    return *b->cur++;
}

int file_get(file_t *f) // 读取一个字节，成功返回非负字符，失败返回-1
{
    return file_get_ex(f, null, null);
}

bool file_unget_ex(file_t *f, int32 n)
{
    buffix_t *b = &f->b;
    byte *arr = buffix_data(b);
    if (n <= 0) {
        return true;
    }
    if (f->real_eof) {
        f->eof_cnt -= n;
        return true;
    }
    if (b->cur >= arr + n) {
        b->cur -= n;
        return true;
    }
    return false;
}

bool file_unget(file_t *f)
{
    return file_unget_ex(f, 1);
}
