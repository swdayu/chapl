#ifndef CHAPL_DIRECT_WAPI_H
#define CHAPL_DIRECT_WAPI_H

// 微软 C 运行时库
//
// 通用 C 运行时库（UCRT，Universal C Runtime Library）支持 C++ 兼容所需的大部分 C 标
// 准库，它实现了 C99（ISO/IEC 9899:1999），还实现了 POSIX.1（ISO/IEC 9945-1:1996），
// 但它并不完全符号任何特定的 POSIX 标准。UCRT 还实现了一些微软特定的不属于标准的函数和
// 宏。
//
// 在全局命名空间中，以单个下划线开头的函数名是 C++ 标准为实现保留的。POSIX 函数和微软
// 特定函数都在全局命名空间中，但不属于标准 C 运行时库的一部分。这就是为什么微软推荐这些
// 名称使用下划线版本的原因。为了可移植性，UCRT 也支持默认名称，但微软 C++ 编译器在编译
// 这些名称时会报警告信息。只有默认名称是被标记为过时的，而下划线版本不是。要抑制警告信
// 息，在包含 POSIX 名称的头文件之前定义 _CRT_NONSTDC_NO_WARNINGS 宏。因为 C 标准不允
// 许在头文件中使用非标准名称，所以默认 /std:c11 和 /std:c17 不会暴露 POSIX 函数、类型、
// 宏的默认名称。如果需要这些名称，需要定义 _CRT_DECLARE_NONSTDC_NAMES 来暴露它们。

// export original POSIX names and suppress deprecation warnings.
#define _CRT_DECLARE_NONSTDC_NAMES 1
#define _CRT_NONSTDC_NO_WARNINGS 1

// define WIN32_LEAN_AND_MEAN to exclude apis such as Cryptography, DDE, RPC,
// Shell, and Windows Sockets.
#define WIN32_LEAN_AND_MEAN 1

// when you define STRICT you enable features that require more care in declaring
// and using types. this helps you write more portable code. this extra care will
// also reduce your debuging time.
#define STRICT 1

// suppress deprecation warnings for the older less secure functions.
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>

#endif /* CHAPL_DIRECT_WAPI_H */
