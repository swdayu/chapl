#ifndef CHAPL_INTERNAL_DECL_H
#define CHAPL_INTERNAL_DECL_H

#ifndef CONFIG_MODULE_VER
#define CONFIG_MODULE_VER 0
#endif

typedef enum {
#define FILE_MAPPING(ID, STR) ID,
#include "internal/init.h"
} file_module_enum_t;

#include "builtin/decl.h"
#endif /* CHAPL_INTERNAL_DECL_H */
