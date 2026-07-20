
#define PRH_FILE_INCLUDE
#define PRH_FILE_IMPLEMENTATION
#define PRH_TEST_IMPLEMENTATION
#include "prh_include.h"

#if defined(prh_plat_windows)
#define PRH_PLAT_WINDOWS_TEST
#endif

#if defined(PRH_PLAT_WINDOWS_TEST)
void prh_impl_windows_test(void) {
#if defined(WIN32)
    printf("WIN32 defined\n");
#endif
#if defined(WIN64)
    printf("WIN64 defined\n");
#endif
#if defined(_WIN32)
    printf("_WIN32 defined %d\n", _WIN32);
#endif
#if defined(_WIN64)
    printf("_WIN64 defined %d\n", _WIN64);
#endif
#if defined(WINVER)
    printf("WINVER %04x\n", WINVER);
#endif
#if defined(_WIN32_WINNT)
    printf("_WIN32_WINNT %04x\n", _WIN32_WINNT);
#endif
#if defined(NTDDI_VERSION)
    printf("NTDDI_VERSION %08x\n", NTDDI_VERSION);
#endif
#if defined(WINVER_MAXVER)
    printf("WINVER_MAXVER %04x\n", WINVER_MAXVER);
#endif
#if defined(WDK_NTDDI_VERSION)
    printf("WDK_NTDDI_VERSION %08x\n", WDK_NTDDI_VERSION);
#endif
    printf("size_t %zd-byte\n", sizeof(size_t));
    printf("ptrdiff_t %zd-byte\n", sizeof(ptrdiff_t));
    printf("time_t %zd-byte\n", sizeof(time_t));
    printf("uintptr_t %zd-byte\n", sizeof(uintptr_t));
    printf("void* %zd-byte\n", sizeof(void *));
    printf("PVOID %zd-byte\n", sizeof(PVOID));
    printf("ULONG_PTR %zd-byte\n", sizeof(ULONG_PTR));
    printf("HANDLE %zd-byte\n", sizeof(HANDLE));
    printf("HMODULE %zd-byte \n", sizeof(HMODULE));
    printf("INVALID_HANDLE_VALUE %zd\n", (size_t)INVALID_HANDLE_VALUE);
    printf("BOOL %zd-byte\n", sizeof(BOOL));
    printf("TRUE %d FALSE %d\n", TRUE, FALSE);
    printf("UINT %zd-byte\n", sizeof(UINT));
    printf("MAX_PATH %d\n", MAX_PATH);
    printf("prh_memory_page_size %d\n", prh_memory_page_size);
    const prh_windows_sysconf *s = prh_windows_get_sysconf();
#ifdef PRH_FILE_INCLUDE
    prh_windows_file_sysconf();
    printf("sectors_per_cluster %d\n", s->sectors_per_cluster);
    printf("bytes_per_sector %d\n", s->bytes_per_sector);
    printf("bytes_per_logical_sector %d\n", s->bytes_per_logical_sector);
    printf("bytes_per_physical_sector %d\n", s->bytes_per_physical_sector);
    printf("absolute_path_max_chars %d\n", s->absolute_path_max_chars);
    printf("relative_path_max_chars %d\n", s->relative_path_max_chars);
    printf("path_component_max_chars %d\n", s->path_component_max_chars);
    prh_real_assert(s->bytes_per_logical_sector != 0 && s->bytes_per_logical_sector == s->bytes_per_sector);
    prh_real_assert(s->bytes_per_physical_sector != 0 && s->bytes_per_physical_sector >= s->bytes_per_logical_sector);
    prh_real_assert(prh_memory_page_size >= s->bytes_per_physical_sector);
#endif
#ifdef PRH_THRD_INCLUDE
    printf("CRITICAL_SECTION %zd-byte\n", sizeof(CRITICAL_SECTION));
    printf("CONDITION_VARIABLE %zd-byte\n", sizeof(CONDITION_VARIABLE));
    printf("MMSYSERR_NOERROR %d\n", MMSYSERR_NOERROR);
    printf("MAXIMUM_SUSPEND_COUNT %d\n", MAXIMUM_SUSPEND_COUNT);
    printf("MAXIMUM_WAIT_OBJECTS %d\n", MAXIMUM_WAIT_OBJECTS);
    printf("MAXIMUM_PROCESSORS %d\n", MAXIMUM_PROCESSORS);
    printf("MEMORY_ALLOCATION_ALIGNMENT %d\n", MEMORY_ALLOCATION_ALIGNMENT);
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    int arch = info.wProcessorArchitecture;
    if (arch == PROCESSOR_ARCHITECTURE_INTEL) {
        printf("PROCESSOR_ARCHITECTURE_INTEL X86\n");
    } else if (arch == PROCESSOR_ARCHITECTURE_AMD64) {
        printf("PROCESSOR_ARCHITECTURE_AMD64 X64\n");
    } else if (arch == PROCESSOR_ARCHITECTURE_ARM) {
        printf("PROCESSOR_ARCHITECTURE_ARM\n");
    } else if (arch == PROCESSOR_ARCHITECTURE_ARM64) {
        printf("PROCESSOR_ARCHITECTURE_ARM64\n");
    } else {
        printf("PROCESSOR_ARCHITECTURE_UNKNOWN\n");
    }
    printf("SYSTEM_INFO dwActiveProcessorMask %p\n", (void *)info.dwActiveProcessorMask);
    printf("SYSTEM_INFO dwNumberOfProcessors %d\n", (int)info.dwNumberOfProcessors);
    printf("SYSTEM_INFO lpMinimumApplicationAddress %p\n", (void *)info.lpMinimumApplicationAddress);
    printf("SYSTEM_INFO lpMaximumApplicationAddress %p\n", (void *)info.lpMaximumApplicationAddress);
    prh_sysinfo sys_info;
    prh_system_info(&sys_info);
    printf("page size %d %dKB\n", sys_info.page_size, sys_info.page_size/1024);
    printf("vmem unit %d %dKB\n", sys_info.vmem_unit, sys_info.vmem_unit/1024);
    printf("cache line size %d\n", sys_info.cache_line_size);
    printf("active processor count %d\n", sys_info.processor_count);
#if defined(CREATE_WAITABLE_TIMER_HIGH_RESOLUTION)
    printf("CREATE_WAITABLE_TIMER_HIGH_RESOLUTION is defined\n");
#endif
    printf("prh_mutex %d-byte\n", (int)sizeof(prh_mutex));
    printf("prh_thrd_cond %d-byte\n", (int)sizeof(prh_thrd_cond));
    printf("prh_cond_sleep %d-byte\n", (int)sizeof(prh_cond_sleep));
#endif // PRH_THRD_INCLUDE

#ifdef PRH_SOCK_INCLUDE
    printf("SOCKET %d-byte\n", (int)sizeof(SOCKET));
    printf("INVALID_SOCKET %p\n", (void *)INVALID_SOCKET);
    printf("struct sockaddr_in %d-byte\n", (int)sizeof(struct sockaddr_in));
    printf("struct sockaddr_in6 %d-byte\n", (int)sizeof(struct sockaddr_in6));
    printf("RIO_MAX_CQ_SIZE %d (%08x)\n", RIO_MAX_CQ_SIZE, RIO_MAX_CQ_SIZE);
    prh_impl_wsaenumprotocol();
#endif // PRH_SOCK_INCLUDE
}
#endif // PRH_PLAT_WINDOWS_TEST

void prh_bit_operation_test(void) {
    prh_real_assert(prh_reg_lower_most_bit_position(0x00) == 0);
    prh_real_assert(prh_reg_lower_most_bit_position(0x01) == 0);
    prh_real_assert(prh_reg_lower_most_bit_position(0x07) == 0);
    prh_real_assert(prh_reg_lower_most_bit_position(0x09) == 0);
    prh_real_assert(prh_reg_lower_most_bit_position(0x02) == 1);
    prh_real_assert(prh_reg_lower_most_bit_position(0x06) == 1);
    prh_real_assert(prh_reg_lower_most_bit_position(0x04) == 2);
    prh_real_assert(prh_reg_lower_most_bit_position(0x0C) == 2);
    prh_real_assert(prh_reg_lower_most_bit_position(0x08) == 3);
    prh_real_assert(prh_reg_lower_most_bit_position(0x10) == 4);
    prh_real_assert(prh_reg_lower_most_bit_position(0x20) == 5);
    prh_real_assert(prh_reg_lower_most_bit_position(0x40) == 6);
    prh_real_assert(prh_reg_lower_most_bit_position(0x80) == 7);

    prh_real_assert(prh_reg_higher_most_bit_position(0x00) == 0);
    prh_real_assert(prh_reg_higher_most_bit_position(0x01) == 0);
    prh_real_assert(prh_reg_higher_most_bit_position(0x02) == 1);
    prh_real_assert(prh_reg_higher_most_bit_position(0x04) == 2);
    prh_real_assert(prh_reg_higher_most_bit_position(0x06) == 2);
    prh_real_assert(prh_reg_higher_most_bit_position(0x07) == 2);
    prh_real_assert(prh_reg_higher_most_bit_position(0x08) == 3);
    prh_real_assert(prh_reg_higher_most_bit_position(0x09) == 3);
    prh_real_assert(prh_reg_higher_most_bit_position(0x0C) == 3);
    prh_real_assert(prh_reg_higher_most_bit_position(0x10) == 4);
    prh_real_assert(prh_reg_higher_most_bit_position(0x20) == 5);
    prh_real_assert(prh_reg_higher_most_bit_position(0x40) == 6);
    prh_real_assert(prh_reg_higher_most_bit_position(0x80) == 7);
}

void prh_bsearch_test(void) {
                // 0  1  2  3
    prh_r32 a[] = {2, 3, 5, 7};
    prh_r32 b[] = {2, 3, 5};
    prh_reg i;

    prh_bsearch_first_less_equal(i, 1, a, prh_arrlen(a)); prh_real_assert(i == 0);
    prh_bsearch_first_less_equal(i, 2, a, prh_arrlen(a)); prh_real_assert(i == 0);
    prh_bsearch_first_less_equal(i, 3, a, prh_arrlen(a)); prh_real_assert(i == 1);
    prh_bsearch_first_less_equal(i, 4, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_bsearch_first_less_equal(i, 5, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_bsearch_first_less_equal(i, 6, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_bsearch_first_less_equal(i, 7, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_bsearch_first_less_equal(i, 8, a, prh_arrlen(a)); prh_real_assert(i == 4);
    prh_bsearch_first_less_equal(i, 1, b, prh_arrlen(b)); prh_real_assert(i == 0);
    prh_bsearch_first_less_equal(i, 2, b, prh_arrlen(b)); prh_real_assert(i == 0);
    prh_bsearch_first_less_equal(i, 3, b, prh_arrlen(b)); prh_real_assert(i == 1);
    prh_bsearch_first_less_equal(i, 4, b, prh_arrlen(b)); prh_real_assert(i == 2);
    prh_bsearch_first_less_equal(i, 5, b, prh_arrlen(b)); prh_real_assert(i == 2);
    prh_bsearch_first_less_equal(i, 6, b, prh_arrlen(b)); prh_real_assert(i == 3);

    prh_bsearch_last_greater_equal(i, 1, a, prh_arrlen(a)); prh_real_assert(i == -1);
    prh_bsearch_last_greater_equal(i, 2, a, prh_arrlen(a)); prh_real_assert(i == 0);
    prh_bsearch_last_greater_equal(i, 3, a, prh_arrlen(a)); prh_real_assert(i == 1);
    prh_bsearch_last_greater_equal(i, 4, a, prh_arrlen(a)); prh_real_assert(i == 1);
    prh_bsearch_last_greater_equal(i, 5, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_bsearch_last_greater_equal(i, 6, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_bsearch_last_greater_equal(i, 7, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_bsearch_last_greater_equal(i, 8, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_bsearch_last_greater_equal(i, 1, b, prh_arrlen(b)); prh_real_assert(i == -1);
    prh_bsearch_last_greater_equal(i, 2, b, prh_arrlen(b)); prh_real_assert(i == 0);
    prh_bsearch_last_greater_equal(i, 3, b, prh_arrlen(b)); prh_real_assert(i == 1);
    prh_bsearch_last_greater_equal(i, 4, b, prh_arrlen(b)); prh_real_assert(i == 1);
    prh_bsearch_last_greater_equal(i, 5, b, prh_arrlen(b)); prh_real_assert(i == 2);
    prh_bsearch_last_greater_equal(i, 6, b, prh_arrlen(b)); prh_real_assert(i == 2);

    prh_fast_bsearch_first_less_equal(i, 1, a, prh_arrlen(a)); prh_real_assert(i == 0);
    prh_fast_bsearch_first_less_equal(i, 2, a, prh_arrlen(a)); prh_real_assert(i == 0);
    prh_fast_bsearch_first_less_equal(i, 3, a, prh_arrlen(a)); prh_real_assert(i == 1);
    prh_fast_bsearch_first_less_equal(i, 4, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_fast_bsearch_first_less_equal(i, 5, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_fast_bsearch_first_less_equal(i, 6, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_fast_bsearch_first_less_equal(i, 7, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_fast_bsearch_first_less_equal(i, 8, a, prh_arrlen(a)); prh_real_assert(i == 4);
    prh_fast_bsearch_first_less_equal(i, 1, b, prh_arrlen(b)); prh_real_assert(i == 0);
    prh_fast_bsearch_first_less_equal(i, 2, b, prh_arrlen(b)); prh_real_assert(i == 0);
    prh_fast_bsearch_first_less_equal(i, 3, b, prh_arrlen(b)); prh_real_assert(i == 1);
    prh_fast_bsearch_first_less_equal(i, 4, b, prh_arrlen(b)); prh_real_assert(i == 2);
    prh_fast_bsearch_first_less_equal(i, 5, b, prh_arrlen(b)); prh_real_assert(i == 2);
    prh_fast_bsearch_first_less_equal(i, 6, b, prh_arrlen(b)); prh_real_assert(i == 3);

    prh_fast_bsearch_last_greater_equal(i, 1, a, prh_arrlen(a)); prh_real_assert(i == -1);
    prh_fast_bsearch_last_greater_equal(i, 2, a, prh_arrlen(a)); prh_real_assert(i == 0);
    prh_fast_bsearch_last_greater_equal(i, 3, a, prh_arrlen(a)); prh_real_assert(i == 1);
    prh_fast_bsearch_last_greater_equal(i, 4, a, prh_arrlen(a)); prh_real_assert(i == 1);
    prh_fast_bsearch_last_greater_equal(i, 5, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_fast_bsearch_last_greater_equal(i, 6, a, prh_arrlen(a)); prh_real_assert(i == 2);
    prh_fast_bsearch_last_greater_equal(i, 7, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_fast_bsearch_last_greater_equal(i, 8, a, prh_arrlen(a)); prh_real_assert(i == 3);
    prh_fast_bsearch_last_greater_equal(i, 1, b, prh_arrlen(b)); prh_real_assert(i == -1);
    prh_fast_bsearch_last_greater_equal(i, 2, b, prh_arrlen(b)); prh_real_assert(i == 0);
    prh_fast_bsearch_last_greater_equal(i, 3, b, prh_arrlen(b)); prh_real_assert(i == 1);
    prh_fast_bsearch_last_greater_equal(i, 4, b, prh_arrlen(b)); prh_real_assert(i == 1);
    prh_fast_bsearch_last_greater_equal(i, 5, b, prh_arrlen(b)); prh_real_assert(i == 2);
    prh_fast_bsearch_last_greater_equal(i, 6, b, prh_arrlen(b)); prh_real_assert(i == 2);

                // 0  1  2  3
    prh_r32 c[] = {7, 5, 3, 2};
    prh_r32 d[] = {7, 5, 3};

    prh_descending_bsearch_last_less_equal(i, 8, c, prh_arrlen(c)); prh_real_assert(i == -1);
    prh_descending_bsearch_last_less_equal(i, 7, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_descending_bsearch_last_less_equal(i, 6, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_descending_bsearch_last_less_equal(i, 5, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_descending_bsearch_last_less_equal(i, 4, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_descending_bsearch_last_less_equal(i, 3, c, prh_arrlen(c)); prh_real_assert(i == 2);
    prh_descending_bsearch_last_less_equal(i, 2, c, prh_arrlen(c)); prh_real_assert(i == 3);
    prh_descending_bsearch_last_less_equal(i, 1, c, prh_arrlen(c)); prh_real_assert(i == 3);
    prh_descending_bsearch_last_less_equal(i, 8, d, prh_arrlen(d)); prh_real_assert(i == -1);
    prh_descending_bsearch_last_less_equal(i, 7, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_descending_bsearch_last_less_equal(i, 6, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_descending_bsearch_last_less_equal(i, 5, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_descending_bsearch_last_less_equal(i, 4, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_descending_bsearch_last_less_equal(i, 3, d, prh_arrlen(d)); prh_real_assert(i == 2);
    prh_descending_bsearch_last_less_equal(i, 2, d, prh_arrlen(d)); prh_real_assert(i == 2);

    prh_descending_bsearch_first_greater_equal(i, 8, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_descending_bsearch_first_greater_equal(i, 7, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_descending_bsearch_first_greater_equal(i, 6, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_descending_bsearch_first_greater_equal(i, 5, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_descending_bsearch_first_greater_equal(i, 4, c, prh_arrlen(c)); prh_real_assert(i == 2);
    prh_descending_bsearch_first_greater_equal(i, 3, c, prh_arrlen(c)); prh_real_assert(i == 2);
    prh_descending_bsearch_first_greater_equal(i, 2, c, prh_arrlen(c)); prh_real_assert(i == 3);
    prh_descending_bsearch_first_greater_equal(i, 1, c, prh_arrlen(c)); prh_real_assert(i == 4);
    prh_descending_bsearch_first_greater_equal(i, 8, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_descending_bsearch_first_greater_equal(i, 7, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_descending_bsearch_first_greater_equal(i, 6, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_descending_bsearch_first_greater_equal(i, 5, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_descending_bsearch_first_greater_equal(i, 4, d, prh_arrlen(d)); prh_real_assert(i == 2);
    prh_descending_bsearch_first_greater_equal(i, 3, d, prh_arrlen(d)); prh_real_assert(i == 2);
    prh_descending_bsearch_first_greater_equal(i, 2, d, prh_arrlen(d)); prh_real_assert(i == 3);

    prh_fast_descending_bsearch_last_less_equal(i, 8, c, prh_arrlen(c)); prh_real_assert(i == -1);
    prh_fast_descending_bsearch_last_less_equal(i, 7, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_last_less_equal(i, 6, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_last_less_equal(i, 5, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_last_less_equal(i, 4, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_last_less_equal(i, 3, c, prh_arrlen(c)); prh_real_assert(i == 2);
    prh_fast_descending_bsearch_last_less_equal(i, 2, c, prh_arrlen(c)); prh_real_assert(i == 3);
    prh_fast_descending_bsearch_last_less_equal(i, 1, c, prh_arrlen(c)); prh_real_assert(i == 3);
    prh_fast_descending_bsearch_last_less_equal(i, 8, d, prh_arrlen(d)); prh_real_assert(i == -1);
    prh_fast_descending_bsearch_last_less_equal(i, 7, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_last_less_equal(i, 6, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_last_less_equal(i, 5, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_last_less_equal(i, 4, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_last_less_equal(i, 3, d, prh_arrlen(d)); prh_real_assert(i == 2);
    prh_fast_descending_bsearch_last_less_equal(i, 2, d, prh_arrlen(d)); prh_real_assert(i == 2);

    prh_fast_descending_bsearch_first_greater_equal(i, 8, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_first_greater_equal(i, 7, c, prh_arrlen(c)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_first_greater_equal(i, 6, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_first_greater_equal(i, 5, c, prh_arrlen(c)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_first_greater_equal(i, 4, c, prh_arrlen(c)); prh_real_assert(i == 2);
    prh_fast_descending_bsearch_first_greater_equal(i, 3, c, prh_arrlen(c)); prh_real_assert(i == 2);
    prh_fast_descending_bsearch_first_greater_equal(i, 2, c, prh_arrlen(c)); prh_real_assert(i == 3);
    prh_fast_descending_bsearch_first_greater_equal(i, 1, c, prh_arrlen(c)); prh_real_assert(i == 4);
    prh_fast_descending_bsearch_first_greater_equal(i, 8, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_first_greater_equal(i, 7, d, prh_arrlen(d)); prh_real_assert(i == 0);
    prh_fast_descending_bsearch_first_greater_equal(i, 6, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_first_greater_equal(i, 5, d, prh_arrlen(d)); prh_real_assert(i == 1);
    prh_fast_descending_bsearch_first_greater_equal(i, 4, d, prh_arrlen(d)); prh_real_assert(i == 2);
    prh_fast_descending_bsearch_first_greater_equal(i, 3, d, prh_arrlen(d)); prh_real_assert(i == 2);
    prh_fast_descending_bsearch_first_greater_equal(i, 2, d, prh_arrlen(d)); prh_real_assert(i == 3);
}
