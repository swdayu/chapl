#include "builtin/decl.h"
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
int_t fileopen_(const char *name, uint32 mode)
{
    int flags = 0;
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
    return open(name, flags|O_BINARY);
}

uint32 fileread_(uint32 fd, uint32 len, byte *out)
{
    if (len > 0x7ffff000) {
        return 0;
    }
    return read(fd, out, len);
}
