#ifndef CHAPL_DIRECT_FILE_H
#define CHAPL_DIRECT_FILE_H
#include "builtin/file.h"
int_t fileopen_(const char *name, uint32 mode);
uint32 fileread_(uint32 fd, uint32 len, byte *out);
#endif /* CHAPL_DIRECT_FILE_H */
