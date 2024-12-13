#ifndef CHAPL_DIRECT_FILE_H
#define CHAPL_DIRECT_FILE_H
#include "builtin/decl.h"

typedef struct {
    int32 fd;
    uint32 len: 31;
    uint32 real_eof: 1;
    int32 eof_cnt;
    buffix_t b; // 必须为最后一个字段，b.cur 指向当前词法前缀的最后一个字节
} file_t;

file_t *file_open(const char *filename, uint32 mode, int32 bufsize);
file_t *file_load(string_t s, int32 bufsize);
void file_reopen(file_t *f, const char *filename, uint32 mode);
void file_reload(file_t *f, string_t s);
void file_close(file_t *f);
int file_get(file_t *f);
int file_get_ex(file_t *f, void (*cp)(void *p, const byte *e), void *p);
bool file_unget(file_t *f);
bool file_unget_ex(file_t *f, int32 n);

#endif /* CHAPL_DIRECT_FILE_H */
