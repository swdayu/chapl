#include "internal/decl.h"
#include "chcc/cifa.h"
DEBUG_MODULE(STRID_TEST_CHCC)

void test_chcc(void)
{
    lang_assert(CIFA_CF_BASE == 0xFE000000);
    lang_assert(CIFA_CF_LSH == 0xFE000001);
    lang_assert(CIFA_CF_RSH == 0xFE000003);
    lang_assert(CIFA_CF_LSH_ASSIGN == 0xFE000004);
    lang_assert(CIFA_CF_RSH_ASSIGN == 0xFE000006);
    lang_assert(CIFA_CF_LAND == 0xFE000007);
    lang_assert(CIFA_CF_LOR == 0xFE000008);
    lang_assert(CIFA_CF_NE == 0xFE000021);
    lang_assert(CIFA_CF_MOD_ASSIGN == 0xFE000025);
    lang_assert(CIFA_CF_AND_ASSIGN == 0xFE000026);
    lang_assert(CIFA_CF_MUL_ASSIGN == 0xFE00002A);
    lang_assert(CIFA_CF_ADD_ASSIGN == 0xFE00002B);
    lang_assert(CIFA_CF_SUB_ASSIGN == 0xFE00002D);
    lang_assert(CIFA_CF_DIV_ASSIGN == 0xFE00002F);
    lang_assert(CIFA_CF_LE == 0xFE00003C);
    lang_assert(CIFA_CF_EQ == 0xFE00003D);
    lang_assert(CIFA_CF_GE == 0xFE00003E);
    lang_assert(CIFA_CF_INI_ASSIGN == 0xFE00003A);
    lang_assert(CIFA_CF_XOR_ASSIGN == 0xFE00005E);
    lang_assert(CIFA_CF_BOR_ASSIGN == 0xFE00007C);
    lang_assert(CIFA_CH_NOT == 0x21);
    lang_assert(CIFA_CH_MOD == 0x25);
    lang_assert(CIFA_CH_AND == 0x26);
    lang_assert(CIFA_CH_MUL == 0x2A);
    lang_assert(CIFA_CH_ADD == 0x2B);
    lang_assert(CIFA_CH_SUB == 0x2D);
    lang_assert(CIFA_CH_DIV == 0x2F);
    lang_assert(CIFA_CH_LT == 0x3C);
    lang_assert(CIFA_CH_ASSIGN == 0x3D);
    lang_assert(CIFA_CH_GT == 0x3E);
    lang_assert(CIFA_CH_COLON == 0x3A);
    lang_assert(CIFA_CH_XOR == 0x5E);
    lang_assert(CIFA_CH_BOR == 0x7C);
}
