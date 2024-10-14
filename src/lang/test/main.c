#include "internal/decl.h"
void test_decl(void);
void test_chcc(void);

int main(int argc, char **argv)
{
    test_decl();
    test_chcc();
    printf("TEST PASS\n");
    return 0;
}
