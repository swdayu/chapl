#include "internal/decl.h"

__DECL_THREAD Error g_tl_error;

#if CONFIG_RT_ERROR_STRING
const byte* g_builtin_errors[BUILTIN_NUM_ERRORS] = {
    "SUCCESS",
    "ERROR",
#define ERROR_MAPPING(ID, STR) STR,
#include "builtin/error.h"
#undef ERROR_MAPPING
};
#endif

#if CONFIG_RT_FILE_STRING
static const byte* g_file_strings[] = {
#define FILE_MAPPING(ID, STR, LEVEL) STR,
#include "internal/init.h"
#undef FILE_MAPPING
};
#endif

void assertfault_(uint16 file, int line)
{
#if CONFIG_RT_FILE_STRING
    printf("assert fault: Ln%d %s\n", line, g_file_strings[file]);
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
        printf("[%c] %s Ln%04d: %s\n", LOG_CHAR(line), g_file_strings[file], LOG_LINE(line), err_str);
    } else {
        printf("[%c] %s Ln%04d: %02x\n", LOG_CHAR(line), g_file_strings[file], LOG_LINE(line), (uint32)err);
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
