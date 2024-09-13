#ifndef CHAPL_INTERNAL_DECL_H
#define CHAPL_INTERNAL_DECL_H
#include "builtin/decl.h"

#ifndef CONFIG_MODULE_VER
#define CONFIG_MODULE_VER 0
#endif

typedef enum {
#define FILE_MAPPING(ID, STR, LEVEL) ID,
#include "internal/init.h"
#undef FILE_MAPPING
} file_enum_t;

typedef enum {
#define FILE_MAPPING(ID, STR, LEVEL) ID##_LOG_LEVEL = LEVEL,
#include "internal/init.h"
#undef FILE_MAPPING
} log_level_enum_t;

#endif /* CHAPL_INTERNAL_DECL_H */
