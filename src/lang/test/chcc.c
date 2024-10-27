#define __CURR_FILE__ STRID_TEST_CHCC
#include "internal/decl.h"
#include "chcc/cifa.h"

#define cifa_assert(ln, col, c) curr(&cfst); \
    lang_assert_2(cf->cf_line == ln && cf->cf_col == col && cf->cfid == c, cf->cf_col, cf->cfid)

#define cifa_cmm_assert(ln, col, c, str) curr(&cfst); \
    lang_assert_s_2(cf->cf_line == ln && cf->cf_col == col && cf->cfid == c && cf->s.len == strlen(str) && \
    (cf->s.len ? (memcmp(cf->s.a, str, cf->s.len) == 0) : true), cf->s, cf->cf_col, cf->cfid)

#define cifa_rune_assert(ln, col, cur_c, e) curr(&cfst); \
    lang_assert_3(cf->cf_line == ln && cf->cf_col == col && cf->cfid == CIFA_CF_RUNE_LIT && \
    cf->val.i == cur_c && cf->error == e, cf->cf_col, cf->val.i, cf->error)

#define cifa_str_assert(ln, col, str, e) curr(&cfst); \
    lang_assert_s_2(cf->cf_line == ln && cf->cf_col == col && cf->cfid == CIFA_CF_STR_LIT && cf->s.len == strlen(str) && \
    (cf->s.len ? (memcmp(cf->s.a, str, cf->s.len) == 0) : true) && cf->error == e, cf->s, cf->cf_col, cf->error)

#define cifa_int_assert(ln, col, c, v) curr(&cfst); \
    lang_assert_3(cf->cf_line == ln && cf->cf_col == col && cf->cfid == c && cf->val.i == v, \
    cf->cf_col, cf->cfid, cf->val.i)

#define cifa_ident_assert(ln, col, id) curr(&cfst); \
    lang_assert_s_2(cf->cf_line == ln && cf->cf_col == col && cf->cfid == id, cf->s, cf->cf_col, cf->cfid)

void test_chcc(void)
{
    cfst_t cfst;
    bufile_t *f;
    cfys_t *cf = &cfst.curcf;

    f = bufile_new(NULL, nullstr(), FILEBUF_DEFAULT_SIZE);
    lang_assert(f != nil);

    cfst_init(&cfst, f);

    lang_assert(CIFA_OP_BASE == 0xFE000000);
    lang_assert(CIFA_OP_LSH == 0xFE000001);
    lang_assert(CIFA_OP_RSH == 0xFE000003);
    lang_assert(CIFA_OP_LSH_ASSIGN == 0xFE000004);
    lang_assert(CIFA_OP_RSH_ASSIGN == 0xFE000006);
    lang_assert(CIFA_OP_LAND == 0xFE000007);
    lang_assert(CIFA_OP_LOR == 0xFE000008);
    lang_assert(CIFA_OP_NE == 0xFE000021);
    lang_assert(CIFA_OP_MOD_ASSIGN == 0xFE000025);
    lang_assert(CIFA_OP_AND_ASSIGN == 0xFE000026);
    lang_assert(CIFA_OP_MUL_ASSIGN == 0xFE00002A);
    lang_assert(CIFA_OP_ADD_ASSIGN == 0xFE00002B);
    lang_assert(CIFA_OP_SUB_ASSIGN == 0xFE00002D);
    lang_assert(CIFA_OP_DIV_ASSIGN == 0xFE00002F);
    lang_assert(CIFA_OP_LE == 0xFE00003C);
    lang_assert(CIFA_OP_EQ == 0xFE00003D);
    lang_assert(CIFA_OP_GE == 0xFE00003E);
    lang_assert(CIFA_OP_INI_ASSIGN == 0xFE00003A);
    lang_assert(CIFA_OP_XOR_ASSIGN == 0xFE00005E);
    lang_assert(CIFA_OP_BOR_ASSIGN == 0xFE00007C);
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

    bufile_load_string(f, strfrom(
        "\t \\ \r"              //  1 空白
        " \n"                   //  2 换行
        "\r\n"                  //  3 换行
        "@()[]{},;\n"           //  4 操作符
        "@ () [] {} , ;\n"      //  5
        " . ... -> && || \n"    //  6
        ".->&&||...@)[},\n"     //  7
        "=<>:!+-*%/&|^<=>=\n"   //  8
        "==!=+=-=*=/=%=&=|=\n"  //  9
        "^=<<>><<=>>=<<<>>>\n"  // 10
    ));

    // "@()[]{},;\n"
    cifa_assert(4, 1, CIFA_CH_ATSIGN);
    cifa_assert(4, 2, CIFA_CH_OPEN_PAREN);
    cifa_assert(4, 3, CIFA_CH_CLOSE_PAREN);
    cifa_assert(4, 4, CIFA_CH_OPEN_SQUARE);
    cifa_assert(4, 5, CIFA_CH_CLOSE_SQUARE);
    cifa_assert(4, 6, CIFA_CH_OPEN_CURLY);
    cifa_assert(4, 7, CIFA_CH_CLOSE_CURLY);
    cifa_assert(4, 8, CIFA_CH_COMMA);
    cifa_assert(4, 9, CIFA_CH_SEMICOLON);

    // "@ () [] {} , ;\n"
    cifa_assert(5, 1, CIFA_CH_ATSIGN);
    cifa_assert(5, 3, CIFA_CH_OPEN_PAREN);
    cifa_assert(5, 4, CIFA_CH_CLOSE_PAREN);
    cifa_assert(5, 6, CIFA_CH_OPEN_SQUARE);
    cifa_assert(5, 7, CIFA_CH_CLOSE_SQUARE);
    cifa_assert(5, 9, CIFA_CH_OPEN_CURLY);
    cifa_assert(5, 10, CIFA_CH_CLOSE_CURLY);
    cifa_assert(5, 12, CIFA_CH_COMMA);
    cifa_assert(5, 14, CIFA_CH_SEMICOLON);

    // " . ... -> && || \n"
    cifa_assert(6, 2, CIFA_CH_DOT);
    cifa_assert(6, 4, CIFA_OP_3DOT);
    cifa_assert(6, 8, CIFA_OP_ARROW);
    cifa_assert(6, 11, CIFA_OP_LAND);
    cifa_assert(6, 14, CIFA_OP_LOR);

    // ".->&&||...@)[},\n"
    cifa_assert(7, 1, CIFA_CH_DOT);
    cifa_assert(7, 2, CIFA_OP_ARROW);
    cifa_assert(7, 4, CIFA_OP_LAND);
    cifa_assert(7, 6, CIFA_OP_LOR);
    cifa_assert(7, 8, CIFA_OP_3DOT);
    cifa_assert(7, 11, CIFA_CH_ATSIGN);
    cifa_assert(7, 12, CIFA_CH_CLOSE_PAREN);
    cifa_assert(7, 13, CIFA_CH_OPEN_SQUARE);
    cifa_assert(7, 14, CIFA_CH_CLOSE_CURLY);
    cifa_assert(7, 15, CIFA_CH_COMMA);

    // "=<>:!+-*%/&|^<=>=\n"
    cifa_assert(8, 1, CIFA_CH_EQUAL);
    cifa_assert(8, 2, CIFA_CH_LT);
    cifa_assert(8, 3, CIFA_CH_GT);
    cifa_assert(8, 4, CIFA_CH_COLON);
    cifa_assert(8, 5, CIFA_CH_NOT);
    cifa_assert(8, 6, CIFA_CH_PLUS);
    cifa_assert(8, 7, CIFA_CH_MINUS);
    cifa_assert(8, 8, CIFA_CH_MUL);
    cifa_assert(8, 9, CIFA_CH_MOD);
    cifa_assert(8, 10, CIFA_CH_DIV);
    cifa_assert(8, 11, CIFA_CH_AND);
    cifa_assert(8, 12, CIFA_CH_BOR);
    cifa_assert(8, 13, CIFA_CH_XOR);
    cifa_assert(8, 14, CIFA_OP_LE);
    cifa_assert(8, 16, CIFA_OP_GE);

    // "==!=+=-=*=/=%=&=|=\n"
    cifa_assert(9, 1, CIFA_OP_EQ);
    cifa_assert(9, 3, CIFA_OP_NE);
    cifa_assert(9, 5, CIFA_OP_ADD_ASSIGN);
    cifa_assert(9, 7, CIFA_OP_SUB_ASSIGN);
    cifa_assert(9, 9, CIFA_OP_MUL_ASSIGN);
    cifa_assert(9, 11, CIFA_OP_DIV_ASSIGN);
    cifa_assert(9, 13, CIFA_OP_MOD_ASSIGN);
    cifa_assert(9, 15, CIFA_OP_AND_ASSIGN);
    cifa_assert(9, 17, CIFA_OP_BOR_ASSIGN);

    // "^=<<>><<=>>=<<<>>>\n"
    cifa_assert(10, 1, CIFA_OP_XOR_ASSIGN);
    cifa_assert(10, 3, CIFA_OP_LSH);
    cifa_assert(10, 5, CIFA_OP_RSH);
    cifa_assert(10, 7, CIFA_OP_LSH_ASSIGN);
    cifa_assert(10, 10, CIFA_OP_RSH_ASSIGN);
    cifa_assert(10, 13, CIFA_OP_LSH);
    cifa_assert(10, 15, CIFA_CH_LT);
    cifa_assert(10, 16, CIFA_OP_RSH);
    cifa_assert(10, 18, CIFA_CH_GT);

    bufile_load_string(f, strfrom(
        "//\n"              //  1 注释
        "// \n"             //  2
        " //a\n"            //  3
        " //a \n"           //  4
        " //abc\n"          //  5
        "/**/ &\n"          //  6
        " /**/\n"           //  7
        "/*a*/ &\n"         //  8
        "/**a*/&\n"         //  9
        "/***a*/&\n"        // 10
        "/*abc*/&\n"        // 11
        "/**abc*/&\n"       // 12
        "/***abc*/&\n"      // 13
    ));

    cifa_cmm_assert(1, 1, CIFA_CF_COMMENT, "");
    cifa_cmm_assert(2, 1, CIFA_CF_COMMENT, " ");
    cifa_cmm_assert(3, 2, CIFA_CF_COMMENT, "a");
    cifa_cmm_assert(4, 2, CIFA_CF_COMMENT, "a ");
    cifa_cmm_assert(5, 2, CIFA_CF_COMMENT, "abc");
    cifa_cmm_assert(6, 1, CIFA_CF_COMMENT, ""); cifa_assert(6, 6, CIFA_CH_AND);
    cifa_cmm_assert(7, 2, CIFA_CF_COMMENT, "");
    cifa_cmm_assert(8, 1, CIFA_CF_COMMENT, "a"); cifa_assert(8, 7, CIFA_CH_AND);
    cifa_cmm_assert(9, 1, CIFA_CF_COMMENT, "*a"); cifa_assert(9, 7, CIFA_CH_AND);
    cifa_cmm_assert(10, 1, CIFA_CF_COMMENT, "**a"); cifa_assert(10, 8, CIFA_CH_AND);
    cifa_cmm_assert(11, 1, CIFA_CF_COMMENT, "abc"); cifa_assert(11, 8, CIFA_CH_AND);
    cifa_cmm_assert(12, 1, CIFA_CF_COMMENT, "*abc"); cifa_assert(12, 9, CIFA_CH_AND);
    cifa_cmm_assert(13, 1, CIFA_CF_COMMENT, "**abc"); cifa_assert(13, 10, CIFA_CH_AND);

    bufile_load_string(f, strfrom(
        "''\n"                              //  1 字符字面量
        "'' \n"                             //  2
        " ''\n"                             //  3
        " '' &\n"                           //  4
        "'a'\n"                             //  5
        "'a' &\n"                           //  6
        " 'a'\n"                            //  7
        " 'a' &\n"                          //  8
        "'\\a''\\b''\\f' '\\n' '\\r' \n"    //  9
        " '\\t''\\v''\\\\''\\'''\\\"'\n"    // 10
        "'\\0' '\\1' '\\2' '\\3' '\\4'\n"   // 11
        " '\\5''\\6''\\7''\\8' '\\9' \n"    // 12
        "'\\x12''\\u1234''\\U000A5678' \n"  // 13
        " '\\x12' '\\u1234' '\\U000A5678'\n"// 14
        "'\\y''\\x1''\\xz1''\\UFFFFFFFF'\n" // 15
        " '\\z' '\\x1z' '\\u1' '\\u1z2' \n" // 16
        "'\n"                               // 17
        "'a\n"                              // 18
        "'\\t\n"                            // 19
        "'\\x1\n"                           // 20
        "'\\u1\n"                           // 21
        "'\\z\n"                            // 22
        "'ab''a\\'''abc\n"                  // 23
        "'\\tb''\\t\\'' '\\t\\'\n"          // 24
        "'\\x12z''\\x345\n"                 // 25
        "'\\zab''\\zab\n"                   // 26
        "'\\x1\\'' '\\u12\\\n"              // 27
    ));

    cifa_rune_assert(1, 1, 0x00, ERROR_EMPTY_RUNE_LIT);
    cifa_rune_assert(2, 1, 0x00, ERROR_EMPTY_RUNE_LIT);
    cifa_rune_assert(3, 2, 0x00, ERROR_EMPTY_RUNE_LIT);
    cifa_rune_assert(4, 2, 0x00, ERROR_EMPTY_RUNE_LIT); cifa_assert(4, 5, CIFA_CH_AND);
    cifa_rune_assert(5, 1, 'a', nil);
    cifa_rune_assert(6, 1, 'a', nil); cifa_assert(6, 5, CIFA_CH_AND);
    cifa_rune_assert(7, 2, 'a', nil);
    cifa_rune_assert(8, 2, 'a', nil); cifa_assert(8, 6, CIFA_CH_AND);
    cifa_rune_assert(9, 1, '\a', nil);
    cifa_rune_assert(9, 5, '\b', nil);
    cifa_rune_assert(9, 9, '\f', nil);
    cifa_rune_assert(9, 14, '\n', nil);
    cifa_rune_assert(9, 19, '\r', nil);
    cifa_rune_assert(10, 2, '\t', nil);
    cifa_rune_assert(10, 6, '\v', nil);
    cifa_rune_assert(10, 10, '\\', nil);
    cifa_rune_assert(10, 14, '\'', nil);
    cifa_rune_assert(10, 18, '\"', nil);
    // "'\\0' '\\1' '\\2' '\\3' '\\4'\n"   // 11
    cifa_rune_assert(11, 1, 0, nil);
    cifa_rune_assert(11, 6, 1, nil);
    cifa_rune_assert(11, 11, 2, nil);
    cifa_rune_assert(11, 16, 3, nil);
    cifa_rune_assert(11, 21, 4, nil);
    // " '\\5''\\6''\\7''\\8' '\\9' \n"    // 12
    cifa_rune_assert(12, 2, 5, nil);
    cifa_rune_assert(12, 6, 6, nil);
    cifa_rune_assert(12, 10, 7, nil);
    cifa_rune_assert(12, 14, 8, nil);
    cifa_rune_assert(12, 19, 9, nil);
    // "'\\x12''\\u1234''\\U000A5678' \n"  // 13
    cifa_rune_assert(13, 1, 0x12, nil);
    cifa_rune_assert(13, 7, 0x1234, nil);
    cifa_rune_assert(13, 15, 0x0A5678, nil);
    // " '\\x12' '\\u1234' '\\U000A5678'\n"// 14
    cifa_rune_assert(14, 2, 0x12, nil);
    cifa_rune_assert(14, 9, 0x1234, nil);
    cifa_rune_assert(14, 18, 0x0A5678, nil);
    // "'\\y''\\x1''\\xz1''\\UFFFFFFFF'\n" // 15
    cifa_rune_assert(15, 1, ' ', ERROR_INVALID_ESCCHAR);
    cifa_rune_assert(15, 5, 0x10, ERROR_INVALID_HEXNUMB);
    cifa_rune_assert(15, 10, 0x01, ERROR_INVALID_HEXCHAR);
    cifa_rune_assert(15, 16, 0xffffffff, ERROR_INVALID_UNICODE);
    // " '\\z' '\\x1z' '\\u1' '\\u1z2' \n" // 16
    cifa_rune_assert(16, 2, ' ', ERROR_INVALID_ESCCHAR);
    cifa_rune_assert(16, 7, 0x10, ERROR_INVALID_HEXCHAR);
    cifa_rune_assert(16, 14, 0x1000, ERROR_INVALID_HEXNUMB);
    cifa_rune_assert(16, 20, 0x1020, ERROR_INVALID_HEXNUMB);
    // "'\n"                               // 17
    // "'a\n"                              // 18
    // "'\\t\n"                            // 19
    // "'\\x1\n"                           // 20
    // "'\\u1\n"                           // 21
    // "'\\z\n"                            // 22
    // "'ab''a\\'''abc\n"                  // 23
    // "'\\tb''\\t\\'' '\\t\\'\n"          // 24
    // "'\\x12z''\\x345\n"                 // 25
    // "'\\zab''\\zab\n"                   // 26
    // "'\\x1\\'' '\\u12\\\n"              // 27
    cifa_rune_assert(17, 1, 0x00, ERROR_MISS_CLOSE_QUOTE);
    cifa_rune_assert(18, 1, 'a', ERROR_MISS_CLOSE_QUOTE);
    cifa_rune_assert(19, 1, '\t', ERROR_MISS_CLOSE_QUOTE);
    cifa_rune_assert(20, 1, 0x10, ERROR_INVALID_HEXNUMB);
    cifa_rune_assert(21, 1, 0x1000, ERROR_INVALID_HEXNUMB);
    cifa_rune_assert(22, 1, ' ', ERROR_MISS_CLOSE_QUOTE);
    cifa_rune_assert(23, 1, 'a', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(23, 5, 'a', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(23, 10, 'a', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(24, 1, '\t', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(24, 6, '\t', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(24, 13, '\t', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(25, 1, 0x12, ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(25, 8, 0x34, ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(26, 1, ' ', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(26, 7, ' ', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(27, 1, 0x10, ERROR_INVALID_HEXNUMB);
    cifa_rune_assert(27, 9, 0x1200, ERROR_INVALID_HEXNUMB);

    bufile_load_string(f, strfrom(
        "\"\" ``\n"                             //  1 字符串字面量
        "\"\"`` \n"                             //  2
        " \"\" ``\n"                            //  3
        " \"\n"                                 //  4
        "\"a\" `a`\n"                           //  5
        "\"ab\"`ab`\n"                          //  6
        "\"abc\n"                               //  7
        "\"\\a\\b\\f\\n\\r\"\n"                 //  8
        "\"\\t\\v\\\\\\\'\\\"\"\n"              //  9
        "\"\\8\\1\\2\\3\\4\"\n"                 // 10
        "\"\\5\\6\\7\\8\\9\"\n"                 // 11
        "\"\\x12\\u03FF\\y\\x1\\xz1\\x1z\"\n"   // 12
        "`\r\n"                                 // 13
        "``a\\a\\b\\c\\d\r"                     // 14
        "``a\\t\\0\\x12\\u1FFF`\n"              // 15
    ));

    cifa_str_assert(1, 1, "", nil);
    cifa_str_assert(1, 4, "", nil);
    cifa_str_assert(2, 1, "", nil);
    cifa_str_assert(2, 3, "", nil);
    cifa_str_assert(3, 2, "", nil);
    cifa_str_assert(3, 5, "", nil);
    cifa_str_assert(4, 2, "", ERROR_MISS_CLOSE_QUOTE);
    cifa_str_assert(5, 1, "a", nil);
    cifa_str_assert(5, 5, "a", nil);
    cifa_str_assert(6, 1, "ab", nil);
    cifa_str_assert(6, 5, "ab", nil);
    cifa_str_assert(7, 1, "abc", ERROR_MISS_CLOSE_QUOTE);
    cifa_str_assert(8, 1, "\a\b\f\n\r", nil);
    cifa_str_assert(9, 1, "\t\v\\\'\"", nil);
    cifa_str_assert(10, 1, "\x08\x01\x02\x03\x04", nil);
    cifa_str_assert(11, 1, "\x05\x06\x07\x08\x09", nil);
    cifa_str_assert(12, 1, "\x12\xCF\xBF \x10\x01\x10", ERROR_INVALID_HEXCHAR);
    cifa_str_assert(13, 1, "\n", nil);
    cifa_str_assert(14, 2, "a\\a\\b\\c\\d\n", nil);
    cifa_str_assert(15, 2, "a\\t\\0\\x12\\u1FFF", nil);

    bufile_load_string(f, strfrom(
        "true false nil\n"              // 1 整数字面量
        "0 0_ 0__0 0_1__ 001 12 345\n"  // 2
        "0b_0_1_2345_ab_ 0B_ 0b__ab \n" // 3
        "0x_0__12abefz1_ 0X_ 0x__zz \n" // 4
        "0B1011_0111 0b1010_ 0X7FEE_\n" // 5
    ));

    cifa_int_assert(1, 1, CIFA_CF_BOOL_LIT, true);
    cifa_int_assert(1, 6, CIFA_CF_BOOL_LIT, false);
    cifa_int_assert(1, 12, CIFA_CF_NIL_LIT, 0);
    cifa_int_assert(2, 1, CIFA_CF_INT_LIT, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 3, CIFA_CF_INT_LIT, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 6, CIFA_CF_INT_LIT, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 11, CIFA_CF_INT_LIT, 1); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 17, CIFA_CF_INT_LIT, 1); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 21, CIFA_CF_INT_LIT, 12); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 24, CIFA_CF_INT_LIT, 345); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(3, 1, CIFA_CF_INT_LIT, 1); lang_assert_s(memcmp(cf->s.a, "_ab_", 4) == 0, cf->s);
    cifa_int_assert(3, 17, CIFA_CF_INT_LIT, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(3, 21, CIFA_CF_INT_LIT, 0); lang_assert_s(memcmp(cf->s.a, "ab", 2) == 0, cf->s);
    cifa_int_assert(4, 1, CIFA_CF_INT_LIT, 0x12abef); lang_assert_s(memcmp(cf->s.a, "z1_", 3) == 0, cf->s);
    cifa_int_assert(4, 17, CIFA_CF_INT_LIT, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(4, 21, CIFA_CF_INT_LIT, 0); lang_assert_s(memcmp(cf->s.a, "zz", 2) == 0, cf->s);
    cifa_int_assert(5, 1, CIFA_CF_INT_LIT, 0xB7); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(5, 13, CIFA_CF_INT_LIT, 0x0A); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(5, 21, CIFA_CF_INT_LIT, 0x7FEE); lang_assert_1(cf->s.len == 0, cf->s.len);

    bufile_load_string(f, strfrom(
        "if for int float\n"            // 1 标识符
        " _0aA_9_ _aA_0 abc AB\n"       // 2
        "_ a b C Z\n"                   // 3
    ));

    cifa_ident_assert(1, 1, CIFA_ID_IF); lang_assert_s(memcmp(cf->s.a, "if", 2) == 0, cf->s);
    cifa_ident_assert(1, 4, CIFA_ID_FOR); lang_assert_s(memcmp(cf->s.a, "for", 3) == 0, cf->s);
    cifa_ident_assert(1, 8, CIFA_ID_INT); lang_assert_s(memcmp(cf->s.a, "int", 3) == 0, cf->s);
    cifa_ident_assert(1, 12, CIFA_ID_FLOAT); lang_assert_s(memcmp(cf->s.a, "float", 5) == 0, cf->s);
    cifa_ident_assert(2, 2, cfst.user_ident_start+0); lang_assert_s(memcmp(cf->s.a, "_0aA_9_", 7) == 0, cf->s);
    cifa_ident_assert(2, 10, cfst.user_ident_start+1); lang_assert_s(memcmp(cf->s.a, "_aA_0", 5) == 0, cf->s);
    cifa_ident_assert(2, 16, cfst.user_ident_start+2); lang_assert_s(memcmp(cf->s.a, "abc", 3) == 0, cf->s);
    cifa_ident_assert(2, 20, cfst.user_ident_start+3); lang_assert_s(memcmp(cf->s.a, "AB", 2) == 0, cf->s);
    cifa_ident_assert(3, 1, cfst.user_ident_start+4); lang_assert_s(memcmp(cf->s.a, "_", 1) == 0, cf->s);
    cifa_ident_assert(3, 3, cfst.user_ident_start+5); lang_assert_s(memcmp(cf->s.a, "a", 1) == 0, cf->s);
    cifa_ident_assert(3, 5, cfst.user_ident_start+6); lang_assert_s(memcmp(cf->s.a, "b", 1) == 0, cf->s);
    cifa_ident_assert(3, 7, cfst.user_ident_start+7); lang_assert_s(memcmp(cf->s.a, "C", 1) == 0, cf->s);
    cifa_ident_assert(3, 9, cfst.user_ident_start+8); lang_assert_s(memcmp(cf->s.a, "Z", 1) == 0, cf->s);

    bufile_delete(f);
    cfst_free(&cfst);
}
