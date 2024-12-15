#define __CURR_FILE__ STRID_TEST_CHCC
#include "internal/decl.h"
#include "chcc/chcc.h"

#define cifa_assert(ln, col, c) scn(&cc); \
    lang_assert_2(cf->line == ln && cf->cols == col && cf->cfid == c, cf->cols, cf->cfid)

#define cifa_cmm_assert(ln, col, c, str) scn(&cc); \
    lang_assert_s_2(cf->line == ln && cf->cols == col && cf->cfid == c && cf->s.len == strlen(str) && \
    (cf->s.len ? (memcmp(cf->s.a, str, cf->s.len) == 0) : true), cf->s, cf->cols, cf->cfid)

#define cifa_rune_assert(ln, col, cur_c, e) scn(&cc); \
    lang_assert_3(cf->line == ln && cf->cols == col && cf->ischar && \
    cf->val.c == cur_c && cf->error == e, cf->cols, cf->val.c, cf->error)

#define cifa_str_assert(ln, col, str, e) scn(&cc); \
    lang_assert_s_2(cf->line == ln && cf->cols == col && cf->cfid == CIFA_TYPE_STRING && cf->s.len == strlen(str) && \
    (cf->s.len ? (memcmp(cf->s.a, str, cf->s.len) == 0) : true) && cf->error == e, cf->s, cf->cols, cf->error)

#define cifa_int_assert(ln, col, t, v) scn(&cc); \
    lang_assert_3(cf->line == ln && cf->cols == col && cf->cfid == t && cf->val.i == v, \
    cf->cols, cf->cfid, cf->val.i)

#define cifa_ident_assert(ln, col, id) scn(&cc); \
    lang_assert_s_2(cf->line == ln && cf->cols == col && cf->cfid == id, cf->s, cf->cols, cf->cfid)

#define cifa_is_basic_type(c) \
    lang_assert_3((c) >= CIFA_ID_INT && (c) <= CIFA_ID_STRING, CIFA_ID_INT, CIFA_ID_STRING, (c))

void test_chcc(void)
{
    chcc_t cc;
    file_t *f = file_load(strnull(), 0);
    cifa_t *cf = &cc.cf;

    lang_assert(f != null);

    chcc_init(&cc, f);

    cifa_is_basic_type(CIFA_ID_INT);
    cifa_is_basic_type(CIFA_ID_BYTE);
    cifa_is_basic_type(CIFA_ID_SHORT);
    cifa_is_basic_type(CIFA_ID_LONG);
    cifa_is_basic_type(CIFA_ID_ARCH);
    cifa_is_basic_type(CIFA_ID_XMM);
    cifa_is_basic_type(CIFA_ID_YMM);
    cifa_is_basic_type(CIFA_ID_ZMM);
    cifa_is_basic_type(CIFA_ID_FLOAT);
    cifa_is_basic_type(CIFA_ID_FLOAT16);
    cifa_is_basic_type(CIFA_ID_FLOAT32);
    cifa_is_basic_type(CIFA_ID_FLOAT64);
    cifa_is_basic_type(CIFA_ID_COMPLEX);
    cifa_is_basic_type(CIFA_ID_COMPLEX16);
    cifa_is_basic_type(CIFA_ID_COMPLEX32);
    cifa_is_basic_type(CIFA_ID_COMPLEX64);
    cifa_is_basic_type(CIFA_ID_BOOL);
    cifa_is_basic_type(CIFA_ID_RUNE);
    cifa_is_basic_type(CIFA_ID_ERROR);
    cifa_is_basic_type(CIFA_ID_STRING);

    file_reload(f, strfrom(
        "\t \\ \r"              //  1 空白
        " \n"                   //  2 换行
        "\r\n"                  //  3 换行
        "@()[]{},;\n"           //  4 操作符
        "@ () [] {} , ;\n"      //  5
        " . ... -> && || \n"    //  6
        ".->&&||...@)[},\n"     //  7
        "=<>:!+-*%/&|^<=>=\n"   //  8
        "==!=+=-=*=\\=%=&=|=\n" //  9
        "^=<<>><<=>>=<<<>>>\n"  // 10
    ));

    // "@()[]{},;\n"
    chcc_start(&cc);
    cifa_assert(1, 3, CIFA_OP_DIV);
    cifa_assert(4, 1, CHAR_ATSIGN);
    cifa_assert(4, 2, CHAR_OPEN_PAREN);
    cifa_assert(4, 3, CHAR_CLOSE_PAREN);
    cifa_assert(4, 4, CHAR_OPEN_SQUARE);
    cifa_assert(4, 5, CHAR_CLOSE_SQUARE);
    cifa_assert(4, 6, CHAR_OPEN_CURLY);
    cifa_assert(4, 7, CHAR_CLOSE_CURLY);
    cifa_assert(4, 8, CHAR_COMMA);
    cifa_assert(4, 9, CHAR_SEMICOLON);

    // "@ () [] {} , ;\n"
    cifa_assert(5, 1, CHAR_ATSIGN);
    cifa_assert(5, 3, CHAR_OPEN_PAREN);
    cifa_assert(5, 4, CHAR_CLOSE_PAREN);
    cifa_assert(5, 6, CHAR_OPEN_SQUARE);
    cifa_assert(5, 7, CHAR_CLOSE_SQUARE);
    cifa_assert(5, 9, CHAR_OPEN_CURLY);
    cifa_assert(5, 10, CHAR_CLOSE_CURLY);
    cifa_assert(5, 12, CHAR_COMMA);
    cifa_assert(5, 14, CHAR_SEMICOLON);

    // " . ... -> && || \n"
    cifa_assert(6, 2, CHAR_DOT);
    cifa_assert(6, 4, CIFA_PT_3DOT);
    cifa_assert(6, 8, CIFA_PT_ARROW);
    cifa_assert(6, 11, CIFA_OP_LAND);
    cifa_assert(6, 14, CIFA_OP_LOR);

    // ".->&&||...@)[},\n"
    cifa_assert(7, 1, CHAR_DOT);
    cifa_assert(7, 2, CIFA_PT_ARROW);
    cifa_assert(7, 4, CIFA_OP_LAND);
    cifa_assert(7, 6, CIFA_OP_LOR);
    cifa_assert(7, 8, CIFA_PT_3DOT);
    cifa_assert(7, 11, CHAR_ATSIGN);
    cifa_assert(7, 12, CHAR_CLOSE_PAREN);
    cifa_assert(7, 13, CHAR_OPEN_SQUARE);
    cifa_assert(7, 14, CHAR_CLOSE_CURLY);
    cifa_assert(7, 15, CHAR_COMMA);

    // "=<>:!+-*%/&|^<=>=\n"
    cifa_assert(8, 1, CHAR_EQUAL);
    cifa_assert(8, 2, CHAR_LT);
    cifa_assert(8, 3, CHAR_GT);
    cifa_assert(8, 4, CHAR_COLON);
    cifa_assert(8, 5, CHAR_NOT);
    cifa_assert(8, 6, CHAR_PLUS);
    cifa_assert(8, 7, CHAR_MINUS);
    cifa_assert(8, 8, CHAR_ASTER);
    cifa_assert(8, 9, CHAR_PERCENT);
    cifa_assert(8, 10, CHAR_SLASH);
    cifa_assert(8, 11, CHAR_AND);
    cifa_assert(8, 12, CHAR_BOR);
    cifa_assert(8, 13, CHAR_XOR);
    cifa_assert(8, 14, CIFA_OP_LE);
    cifa_assert(8, 16, CIFA_OP_GE);

    // "==!=+=-=*=\\=%=&=|=\n"
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
    cifa_assert(10, 3, CIFA_OP_ULT);
    cifa_assert(10, 5, CIFA_OP_UGT);
    cifa_assert(10, 7, CIFA_OP_ULE);
    cifa_assert(10, 10, CIFA_OP_UGE);
    cifa_assert(10, 13, CIFA_OP_ULT);
    cifa_assert(10, 15, CHAR_LT);
    cifa_assert(10, 16, CIFA_OP_UGT);
    cifa_assert(10, 18, CHAR_GT);

    file_reload(f, strfrom(
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

    chcc_start(&cc);
    cifa_cmm_assert(1, 1, CIFA_TYPE_COMMENT, "");
    cifa_cmm_assert(2, 1, CIFA_TYPE_COMMENT, " ");
    cifa_cmm_assert(3, 2, CIFA_TYPE_COMMENT, "a");
    cifa_cmm_assert(4, 2, CIFA_TYPE_COMMENT, "a ");
    cifa_cmm_assert(5, 2, CIFA_TYPE_COMMENT, "abc");
    cifa_cmm_assert(6, 1, CIFA_TYPE_COMMENT, ""); cifa_assert(6, 6, CHAR_AND);
    cifa_cmm_assert(7, 2, CIFA_TYPE_COMMENT, "");
    cifa_cmm_assert(8, 1, CIFA_TYPE_COMMENT, "a"); cifa_assert(8, 7, CHAR_AND);
    cifa_cmm_assert(9, 1, CIFA_TYPE_COMMENT, "*a"); cifa_assert(9, 7, CHAR_AND);
    cifa_cmm_assert(10, 1, CIFA_TYPE_COMMENT, "**a"); cifa_assert(10, 8, CHAR_AND);
    cifa_cmm_assert(11, 1, CIFA_TYPE_COMMENT, "abc"); cifa_assert(11, 8, CHAR_AND);
    cifa_cmm_assert(12, 1, CIFA_TYPE_COMMENT, "*abc"); cifa_assert(12, 9, CHAR_AND);
    cifa_cmm_assert(13, 1, CIFA_TYPE_COMMENT, "**abc"); cifa_assert(13, 10, CHAR_AND);

    file_reload(f, strfrom(
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

    chcc_start(&cc);
    cifa_rune_assert(1, 1, 0x00, ERROR_EMPTY_RUNE_LIT);
    cifa_rune_assert(2, 1, 0x00, ERROR_EMPTY_RUNE_LIT);
    cifa_rune_assert(3, 2, 0x00, ERROR_EMPTY_RUNE_LIT);
    cifa_rune_assert(4, 2, 0x00, ERROR_EMPTY_RUNE_LIT); cifa_assert(4, 5, CHAR_AND);
    cifa_rune_assert(5, 1, 'a', null);
    cifa_rune_assert(6, 1, 'a', null); cifa_assert(6, 5, CHAR_AND);
    cifa_rune_assert(7, 2, 'a', null);
    cifa_rune_assert(8, 2, 'a', null); cifa_assert(8, 6, CHAR_AND);
    cifa_rune_assert(9, 1, '\a', null);
    cifa_rune_assert(9, 5, '\b', null);
    cifa_rune_assert(9, 9, '\f', null);
    cifa_rune_assert(9, 14, '\n', null);
    cifa_rune_assert(9, 19, '\r', null);
    cifa_rune_assert(10, 2, '\t', null);
    cifa_rune_assert(10, 6, '\v', null);
    cifa_rune_assert(10, 10, '\\', null);
    cifa_rune_assert(10, 14, '\'', null);
    cifa_rune_assert(10, 18, '\"', null);
    // "'\\0' '\\1' '\\2' '\\3' '\\4'\n"   // 11
    cifa_rune_assert(11, 1, 0, null);
    cifa_rune_assert(11, 6, 1, null);
    cifa_rune_assert(11, 11, 2, null);
    cifa_rune_assert(11, 16, 3, null);
    cifa_rune_assert(11, 21, 4, null);
    // " '\\5''\\6''\\7''\\8' '\\9' \n"    // 12
    cifa_rune_assert(12, 2, 5, null);
    cifa_rune_assert(12, 6, 6, null);
    cifa_rune_assert(12, 10, 7, null);
    cifa_rune_assert(12, 14, 8, null);
    cifa_rune_assert(12, 19, 9, null);
    // "'\\x12''\\u1234''\\U000A5678' \n"  // 13
    cifa_rune_assert(13, 1, 0x12, null);
    cifa_rune_assert(13, 7, 0x1234, null);
    cifa_rune_assert(13, 15, 0x0A5678, null);
    // " '\\x12' '\\u1234' '\\U000A5678'\n"// 14
    cifa_rune_assert(14, 2, 0x12, null);
    cifa_rune_assert(14, 9, 0x1234, null);
    cifa_rune_assert(14, 18, 0x0A5678, null);
    // "'\\y''\\x1''\\xz1''\\UFFFFFFFF'\n" // 15
    cifa_rune_assert(15, 1, ' ', ERROR_INVALID_ESCCHAR);
    cifa_rune_assert(15, 5, 0x10, ERROR_INVALID_HEXNUMB);
    cifa_rune_assert(15, 10, 0x01, ERROR_INVALID_HEXCHAR);
    cifa_rune_assert(15, 16, 0xffffffff, 0);
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
    cifa_rune_assert(22, 1, ' ', ERROR_INVALID_ESCCHAR);
    cifa_rune_assert(23, 1, 'a', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(23, 5, 'a', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(23, 10, 'a', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(24, 1, '\t', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(24, 6, '\t', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(24, 13, '\t', ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(25, 1, 0x12, ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(25, 8, 0x34, ERROR_MULT_CHAR_EXIST);
    cifa_rune_assert(26, 1, ' ', ERROR_INVALID_ESCCHAR);
    cifa_rune_assert(26, 7, ' ', ERROR_INVALID_ESCCHAR);
    cifa_rune_assert(27, 1, 0x10, ERROR_INVALID_HEXNUMB);
    cifa_rune_assert(27, 9, 0x1200, ERROR_INVALID_HEXNUMB);

    file_reload(f, strfrom(
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

    chcc_start(&cc);
    cifa_str_assert(1, 1, "", null);
    cifa_str_assert(1, 4, "", null);
    cifa_str_assert(2, 1, "", null);
    cifa_str_assert(2, 3, "", null);
    cifa_str_assert(3, 2, "", null);
    cifa_str_assert(3, 5, "", null);
    cifa_str_assert(4, 2, "", ERROR_MISS_CLOSE_QUOTE);
    cifa_str_assert(5, 1, "a", null);
    cifa_str_assert(5, 5, "a", null);
    cifa_str_assert(6, 1, "ab", null);
    cifa_str_assert(6, 5, "ab", null);
    cifa_str_assert(7, 1, "abc", ERROR_MISS_CLOSE_QUOTE);
    cifa_str_assert(8, 1, "\a\b\f\n\r", null);
    cifa_str_assert(9, 1, "\t\v\\\'\"", null);
    cifa_str_assert(10, 1, "\x08\x01\x02\x03\x04", null);
    cifa_str_assert(11, 1, "\x05\x06\x07\x08\x09", null);
    cifa_str_assert(12, 1, "\x12\xCF\xBF \x10\x01\x10", ERROR_INVALID_HEXCHAR);
    cifa_str_assert(13, 1, "\n", null);
    cifa_str_assert(14, 2, "a\\a\\b\\c\\d\n", null);
    cifa_str_assert(15, 2, "a\\t\\0\\x12\\u1FFF", null);

    file_reload(f, strfrom(
        "true false null\n"              // 1 整数字面量
        "0 0_ 0__0 0_1__ 001 12 345\n"  // 2
        "0b_0_1_2345_ab_ 0B_ 0b__ab \n" // 3
        "0x_0__12abefz1_ 0X_ 0x__zz \n" // 4
        "0B1011_0111 0b1010_ 0X7FEE_\n" // 5
    ));

    chcc_start(&cc);
    cifa_int_assert(1, 1, CIFA_TYPE_NUMERIC, true);
    cifa_int_assert(1, 6, CIFA_TYPE_NUMERIC, false);
    cifa_int_assert(1, 12, CIFA_TYPE_NUMERIC, 0);
    cifa_int_assert(2, 1, CIFA_TYPE_NUMERIC, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 3, CIFA_TYPE_NUMERIC, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 6, CIFA_TYPE_NUMERIC, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 11, CIFA_TYPE_NUMERIC, 1); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 17, CIFA_TYPE_NUMERIC, 1); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 21, CIFA_TYPE_NUMERIC, 12); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(2, 24, CIFA_TYPE_NUMERIC, 345); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(3, 1, CIFA_TYPE_NUMERIC, 1); lang_assert_s(memcmp(cf->s.a, "_ab_", 4) == 0, cf->s);
    cifa_int_assert(3, 17, CIFA_TYPE_NUMERIC, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(3, 21, CIFA_TYPE_NUMERIC, 0); lang_assert_s(memcmp(cf->s.a, "ab", 2) == 0, cf->s);
    cifa_int_assert(4, 1, CIFA_TYPE_NUMERIC, 0x12abef); lang_assert_s(memcmp(cf->s.a, "z1_", 3) == 0, cf->s);
    cifa_int_assert(4, 17, CIFA_TYPE_NUMERIC, 0); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(4, 21, CIFA_TYPE_NUMERIC, 0); lang_assert_s(memcmp(cf->s.a, "zz", 2) == 0, cf->s);
    cifa_int_assert(5, 1, CIFA_TYPE_NUMERIC, 0xB7); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(5, 13, CIFA_TYPE_NUMERIC, 0x0A); lang_assert_1(cf->s.len == 0, cf->s.len);
    cifa_int_assert(5, 21, CIFA_TYPE_NUMERIC, 0x7FEE); lang_assert_1(cf->s.len == 0, cf->s.len);

    file_reload(f, strfrom(
        "if for int float\n"            // 1 标识符
        " _0aA_9_ _aA_0 abc AB\n"       // 2
        "_ a b C Z\n"                   // 3
    ));

    chcc_start(&cc);
    cifa_ident_assert(1, 1, CIFA_ID_IF); lang_assert_s(memcmp(cf->s.a, "if", 2) == 0, cf->s);
    cifa_ident_assert(1, 4, CIFA_ID_FOR); lang_assert_s(memcmp(cf->s.a, "for", 3) == 0, cf->s);
    cifa_ident_assert(1, 8, CIFA_ID_INT); lang_assert_s(memcmp(cf->s.a, "int", 3) == 0, cf->s);
    cifa_ident_assert(1, 12, CIFA_ID_FLOAT); lang_assert_s(memcmp(cf->s.a, "float", 5) == 0, cf->s);
    cifa_ident_assert(2, 2, cc.user_id_start+0); lang_assert_s(memcmp(cf->s.a, "_0aA_9_", 7) == 0, cf->s);
    cifa_ident_assert(2, 10, cc.user_id_start+1); lang_assert_s(memcmp(cf->s.a, "_aA_0", 5) == 0, cf->s);
    cifa_ident_assert(2, 16, cc.user_id_start+2); lang_assert_s(memcmp(cf->s.a, "abc", 3) == 0, cf->s);
    cifa_ident_assert(2, 20, cc.user_id_start+3); lang_assert_s(memcmp(cf->s.a, "AB", 2) == 0, cf->s);
    cifa_ident_assert(3, 1, cc.user_id_start+4); lang_assert_s(memcmp(cf->s.a, "_", 1) == 0, cf->s);
    cifa_ident_assert(3, 3, cc.user_id_start+5); lang_assert_s(memcmp(cf->s.a, "a", 1) == 0, cf->s);
    cifa_ident_assert(3, 5, cc.user_id_start+6); lang_assert_s(memcmp(cf->s.a, "b", 1) == 0, cf->s);
    cifa_ident_assert(3, 7, cc.user_id_start+7); lang_assert_s(memcmp(cf->s.a, "C", 1) == 0, cf->s);
    cifa_ident_assert(3, 9, cc.user_id_start+8); lang_assert_s(memcmp(cf->s.a, "Z", 1) == 0, cf->s);

    file_close(f);
    chcc_free(&cc);
}
