#ifndef CHAPL_DIRECT_FILE_H
#define CHAPL_DIRECT_FILE_H
#include "builtin/file.h"
#ifdef __cplusplus
extern "C" {
#endif

intd_t fileopen_(const char *name, uint32 mode);
uint32 fileread_(intd_t fd, uint32 len, byte *out);

#ifdef __cplusplus
}
#endif
#endif /* CHAPL_DIRECT_FILE_H */
