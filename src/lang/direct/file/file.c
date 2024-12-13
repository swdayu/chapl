#include "direct/file.h"

#if defined(__MSC__)
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/
// https://learn.microsoft.com/en-us/windows/win32/api/fileapifromapp/
// https://learn.microsoft.com/en-us/windows/win32/FileIO/file-systems
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/low-level-i-o

// 低级 IO（low-level I/O）调用操作系统来提供比流式 IO（stream I/O）更低级别的操作，
// 低级 IO 不会对数据进行缓冲或格式化。这些低级函数可以使用以下预定义的文件描述符来访问
// 程序启动时打开的标准流：
//
//      标准流      文件描述符
//      stdin           0
//      stdout          1
//      stderr          2
//
// 当发生错误时，低级 IO 会设置 errno 全局变量。只有当程序需要使用在 stdio.h 中定义的
// 常量时（例如 EOF），才需要在使用低级函数时包含 stdio.h 头文件。以下是低级 IO 函数的
// 原型，它们都定义在 <io.h> 头文件中：
//
// int _close(int fd);
//      返回0表示成功，返回-1表示失败；
//
// int _commit(int fd);
//
// int _creat(const char *filename, int pmode);
// int _wcreat(const wchar_t *filename, int pmode);
//
// int _dup(int fd);
// int _dup2(int fd1, int fd2);
//
// int _eof(int fd);
//
// long _lseek(int fd, long offset, int origin);
// __int64 _lseeki64(int fd, __int64 offset, int origin);
//
// int _open(const char *filename, int oflag [, int pmode]);
// int _wopen(const wchar_t *filename, int oflag [, int pmode]);
// int _sopen(const char *filename, int oflag, int shflag [, int pmode]);
// int _wsopen(const wchar_t *filename, int oflag, int shflag [, int pmode]);
// errno_t _sopen_s(int* pfh, const char *filename, int oflag, int shflag, int pmode);
// errno_t _wsopen_s(int* pfh, const wchar_t *filename, int oflag, int shflag, int pmode);
//
// int _read(int const fd, void *const buffer, unsigned const buffer_size);
// int _write(int fd, const void *buffer, unsigned int count);
//
// long _tell(int handle);
// __int64 _telli64(int handle);
//
// int _umask(int pmode);
// errno_t _umask_s(int mode, int* poldmode);
//
// int _open_osfhandle(intptr_t osfhandle, int flags);
//      Win32 函数 CreateFile 创建的 HANDLE 可以通过该函数转换成文件描述符；
//      成功返回文件描述符，否则返回-1表示失败；
//      (intptr_t)(HANDLE)osfhandle，操作系统文件句柄；
//      flags 定义在 <fcntl.h> 头文件中，可以使用以下一些标志：
//      - _O_APPEND：在每次些操作之前将文件位置设置到文件尾部
//      - _O_RDONLY：只读模式打开文件
//      - _O_TEXT：ANSI 文本模式打开文件
//      - _O_WTEXT，Unicode（UTF-16）模式打开文件
#elif defined(__GCC__)
// open, openat, creat - 打开并且可能创建一个文件
// POSIZ.1-2008
// libc, -lc
// #include <fcntl.h>
// int open(const char *pathname, int flags, ... /* mode_t mode */);
//      该系统调用打开一个文件，如果文件不存在并且置了O_CREATE会创建这个文件
//      返回对应文件的文件描述符，是一个非负小整数，指向进程打开文件描述符表的元素
//      如果发生错误，返回-1并设置错误码errno
//      flags 必须包含 O_RDONLY O_WRONLY O_RDWR 中的一个
//      O_NOATIME - 不修改最后访问时间节省磁盘操作，不是所有的系统都支持
//      O_DIRECT - 如果应用程序使用字节的文件缓冲，使用该标志可以直接使用用户空间缓冲
//      O_DIRECT 对用户空间缓存的长度、地址位置，以及文件I/O的位置偏移有限制：
//      1. 对于未对齐的地址，各系统处理不同，可能报错EINVAL，可能回头使用自己的内存页面缓存
//      2. Linux 6.1 之后，支持使用 statx(2) STATX_DIOALIGN 查询一个文件的地址对齐要求
//      3. 否则在 Linux 2.4 上，基于块设备的多数文件系统要求文件偏移、长度、内存地址必
//         须是文件系统块结构体的整数倍，一般是4096字节
//      4. 在 Linux 2.6.0 上，这个值放松到块设备的逻辑块大小，一般为512字节
//      5. 块设备的逻辑块大小，可以使用 ioctl(2) BLKSSZGET 操作确定
#include <fcntl.h>
#include <unistd.h>
Uint fileread_(int fd, Uint len, byte *out)
{
    if (len > SSIZE_MAX || len > 0x7ffff000) {
        return 0;
    }
    return (Int)read(fd, out, len);
}
#endif

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
    Int len = (Int)b->cap - BUF_HEAD_BYTE_CNT;
    if ((Int)s.len < len) {
        len = (Int)s.len;
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

void file_reopen(file_t *f, const char *filename, uint32 mode)
{
    int32 fd = 0; // stdin;
    int flags = 0;
    fileclose_(f);
    f->b.cur = ((byte *)(f + 1)) + BUF_HEAD_BYTE_CNT;
    if (filename && *filename) {
        if (mode == 'r') {
            flags = O_RDONLY;
        } else if (mode == 'a') {
            flags = O_WRONLY|O_APPEND;
        } else if (mode == 'u') {
            flags = O_RDWR;
        }
        if (!flags) {
            return;
        }
        fd = fileopen_(filename, flags|O_BINARY);
        if (fd < 0) {
            return;
        }
    }
    f->fd = fd;
    f->len = BUF_HEAD_BYTE_CNT;
    f->real_eof = 0;
}

file_t *fileopen_(const byte *filename, Int strlen, int32 bufsize, uint32 mode)
{
    file_t *f;
    Int cap = (bufsize > 0) ? bufsize : DEFAULT_BUFF_SIZE;
    if (!mode) {
        cap = (strlen > cap) ? strlen : cap;
    }
    cap += BUF_HEAD_BYTE_CNT;
    if (cap > FILE_BUF_MAX_SIZE) {
        return null;
    }
    f = (file_t *)malloc(sizeof(file_t) + cap);
    if (!f) {
        return null;
    }
    f->fd = -1;
    buffix_init_inplace(&f->b, cap);
    if (mode) {
        file_reopen(f, filename, mode);
    } else {
        file_reload(f, strflen(filename, strlen));
    }
    return f;
}

file_t *file_load(string_t s, int32 bufsize)
{
    return fileopen_(s.a, s.len, bufsize, 0);
}

file_t *file_open(const char *filename, uint32 mode, int32 bufsize)
{
    return fileopen_(filename, 0, bufsize, mode);
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
    Uint cap = buffix_cap(b);
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
    len = fileread_(f->fd, b->cur, bflen);
    if (len <= 0 || len > bflen) {
        return;
    }
    f->len += len;
    if (len == bflen) {
        f->real_eof = 0; // 文件还有剩余内容可读
    }
}

int file_get_ex(file_t *f, void (*cp)(void *p, const byte *e), void *p)
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
        fileblock_(f, cp, p);
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
