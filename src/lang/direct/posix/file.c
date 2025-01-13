#include "builtin/decl.h"
#include <fcntl.h>
#include <unistd.h>
// open, openat, creat - 打开并且可能创建一个文件
// POSIX.1-2008
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
uint32 fileread_(uint32 fd, uint32 len, byte *out)
{
    if (len > SSIZE_MAX || len > 0x7ffff000) {
        return 0;
    }
    return (Int)read(fd, out, len);
}
