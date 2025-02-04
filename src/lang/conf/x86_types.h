#ifndef CHAPL_X86_TYPES_H
#define CHAPL_X86_TYPES_H

#undef __ARCH_32BIT__
#undef __ARCH_64BIT__
#undef __ARCH_BITS__

#define __ARCH_64BIT__ 0
#define __ARCH_32BIT__ 32
#define __ARCH_BITS__ 32

#undef __LIT_ENDIAN__
#undef __BIG_ENDIAN__

#define __LIT_ENDIAN__ 1
#define __BIG_ENDIAN__ 0

#undef int8
#undef uint8

typedef signed char int8;
typedef unsigned char uint8;

#undef int16
#undef uint16

typedef signed short int16;
typedef unsigned short uint16;

#undef int32
#undef uint32

typedef signed int int32;
typedef unsigned int uint32;

#undef int64
#undef uint64

typedef signed long long int64;
typedef unsigned long long uint64;

#undef intptr
#undef uintptr

typedef int32 intptr;
typedef uint32 uintptr;

#undef LANG_INTPTR_MIN
#undef LANG_INTPTR_MAX
#undef LANG_INTPTR_UMAX

#define LANG_INTPTR_MIN LANG_INT32_MIN
#define LANG_INTPTR_MAX LANG_INT32_MAX
#define LANG_INTPTR_UMAX LANG_INT32_UMAX

#endif /* CHAPL_X86_TYPES_H */
