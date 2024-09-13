#include "internal/decl.h"
extern void test_builtin(void);

int main(int argc, char **argv)
{
    test_builtin();
    printf("TEST PASS\n");
    return 0;
}
