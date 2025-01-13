#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>

typedef union {
    unsigned char a[sizeof(int)];
    unsigned int n;
} ByteOrder;

int main(int argc, char *argv[])
{
    ByteOrder order;
    int size_char = (int)(sizeof(char));
    int size_short = (int)(sizeof(short));
    int size_int = (int)(sizeof(int));
    int size_long = (int)(sizeof(long));
    int size_long_long = (int)(sizeof(long long));
    int size_ptr = (int)(sizeof(void*));

    printf("#ifndef CHAPL_ABI_PRETYPES_H\n");
    printf("#define CHAPL_ABI_PRETYPES_H\n");

    printf("/* char %d-byte */\n", (int)(sizeof(char)));
    printf("/* short %d-byte */\n", (int)(sizeof(short)));
    printf("/* int %d-byte */\n", (int)(sizeof(int)));
    printf("/* long %d-byte */\n", (int)(sizeof(long)));
    printf("/* long long %d-byte */\n", (int)(sizeof(long long)));
    printf("/* void* %d-byte */\n", (int)(sizeof(void*)));
    printf("/* size_t %d-byte */\n", (int)(sizeof(size_t)));
    printf("/* time_t %d-byte */\n", (int)(sizeof(time_t)));
    printf("/* clock_t %d-byte */\n", (int)(sizeof(clock_t)));
    printf("/* ptrdiff_t %d-byte */\n", (int)(sizeof(ptrdiff_t)));
    printf("/* jmp_buf %d-byte */\n", (int)(sizeof(jmp_buf)));
    printf("/* CLOCKS_PER_SEC %d */\n", (int)CLOCKS_PER_SEC);
    printf("/* BUFSIZ %d */\n", BUFSIZ);
    printf("/* EOF %d */\n", EOF);
    printf("/* FILENAME_MAX %d */\n", FILENAME_MAX);
    printf("/* FOPEN_MAX %d */\n", FOPEN_MAX);
    printf("/* TMP_MAX %d */\n", TMP_MAX);
    printf("/* L_tmpnam %d */\n", L_tmpnam);
    printf("/* RAND_MAX %d */\n", RAND_MAX);
    printf("\n");

    printf("#undef __ARCH_32BIT__\n");
    printf("#undef __ARCH_64BIT__\n");
    printf("#undef __ARCH_BITS__\n");
    printf("\n");

    if (size_ptr == 4)
    {
        printf("#define __ARCH_32BIT__ 32\n");
        printf("#define __ARCH_64BIT__ 0\n");
        printf("#define __ARCH_BITS__ 32\n");
    }
    else if (size_ptr == 8)
    {
        printf("#define __ARCH_64BIT__ 64\n");
        printf("#define __ARCH_32BIT__ 0\n");
        printf("#define __ARCH_BITS__ 64\n");
    }
    else
    {
        printf("#error __ARCH_BITS__ invalid\n");
    }
    printf("\n");

    order.n = 0xabcd;
    printf("#undef __LIT_ENDIAN__\n");
    printf("#undef __BIG_ENDIAN__\n");
    printf("\n");

    if (order.a[0] == 0xcd && order.a[1] == 0xab)
    {
        printf("#define __LIT_ENDIAN__ 1\n");
        printf("#define __BIG_ENDIAN__ 0\n");
    }
    else
    {
        printf("#define __LIT_ENDIAN__ 0\n");
        printf("#define __BIG_ENDIAN__ 1\n");
    }
    printf("\n");

    printf("#undef int8\n");
    printf("#undef uint8\n");
    printf("\n");

    if (size_char == 1)
    {
        printf("typedef signed char int8;\n");
        printf("typedef unsigned char uint8;\n");
    }
    else
    {
        printf("#error int8 not defined\n");
    }
    printf("\n");

    printf("#undef int16\n");
    printf("#undef uint16\n");
    printf("\n");

    if (size_short == 2)
    {
        printf("typedef signed short int16;\n");
        printf("typedef unsigned short uint16;\n");
    }
    else if (size_int == 2)
    {
        printf("typedef signed int int16;\n");
        printf("typedef unsigned int uint16;\n");
    }
    else
    {
        printf("#error int16 not defined\n");
    }
    printf("\n");

    printf("#undef int32\n");
    printf("#undef uint32\n");
    printf("\n");

    if (size_int == 4)
    {
        printf("typedef signed int int32;\n");
        printf("typedef unsigned int uint32;\n");
    }
    else if (size_long == 4)
    {
        printf("typedef signed long int32;\n");
        printf("typedef unsigned long uint32;\n");
    }
    else
    {
        printf("#error int32 not defined\n");
    }
    printf("\n");

    printf("#undef int64\n");
    printf("#undef uint64\n");
    printf("\n");

    if (size_long_long == 8)
    {
        printf("typedef signed long long int64;\n");
        printf("typedef unsigned long long uint64;\n");
    }
    else
    {
        printf("#error int64 not defined\n");
    }
    printf("\n");

    printf("#undef intptr\n");
    printf("#undef uintptr\n");
    printf("\n");

    if (size_ptr == 4)
    {
        printf("typedef int32 intptr;\n");
        printf("typedef uint32 uintptr;\n");
    }
    else if (size_ptr == 8)
    {
        printf("typedef int64 intptr;\n");
        printf("typedef uint64 uintptr;\n");
    }
    else
    {
        printf("#error intptr not defined\n");
    }
    printf("\n");

    printf("#undef LANG_INTPTR_MIN\n");
    printf("#undef LANG_INTPTR_MAX\n");
    printf("#undef LANG_INTPTR_UMAX\n");
    printf("\n");

    if (size_ptr == 4)
    {
        printf("#define LANG_INTPTR_MIN LANG_INT32_MIN\n");
        printf("#define LANG_INTPTR_MAX LANG_INT32_MAX\n");
        printf("#define LANG_INTPTR_UMAX LANG_INT32_UMAX\n");
    }
    else if (size_ptr == 8)
    {
        printf("#define LANG_INTPTR_MIN LANG_INT64_MIN\n");
        printf("#define LANG_INTPTR_MAX LANG_INT64_MAX\n");
        printf("#define LANG_INTPTR_UMAX LANG_INT64_UMAX\n");
    }
    else
    {
        printf("#error LANG_INTPTR_MAX not defined\n");
    }
    printf("\n");

    printf("#endif /* CHAPL_ABI_PRETYPES_H */\n");
    return 0;
}
