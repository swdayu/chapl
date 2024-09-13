#include "internal/decl.h"
DEBUG_MODULE(STRID_TEST_BUILTIN)

void test_builtin(void)
{
    lang_assert(nil == 0);
    lang_assert(ERROR == 1);
    lang_assert(BUILTIN_LAST_ERROR & 1);
}
