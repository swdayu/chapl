#ifndef CHAPL_LANG_CHCC_CIFA_H
#define CHAPL_LANG_CHCC_CIFA_H
#include "builtin/decl.h"

// 词法元素包括：
// 1. 操作符（operator），一元操作符包括 + - ^ ! * &
// 2. 关键字（keyword）
// 3. 内置预声明（predeclared identifier）
// 4. 注释和空白（comment）
// 5. 字面量（literal）
// 6. 标识符（identifier）

// 字符分类
#define CHAR_CLASS_OTHERBYTE    0x01    // 其他字节
#define CHAR_CLASS_DIGIT        0x02    // 0 ~ 9
#define CHAR_CLASS_IDENT        0x03    // A ~ Z a ~ z _
#define CHAR_CLASS_SINGLE       0x05    // # $ @ ( ) [ ] { } , ; 反斜杠
#define CHAR_CLASS_NEWLINE      0x06    // \r \r\n \n
#define CHAR_CLASS_QUOTE        0x0B    // ' ''' " `
#define CHAR_CLASS_DOT          0x0C    // . .. ... .1
#define CHAR_CLASS_QMARK        0x0E    // ? ?: ?>
#define CHAR_CLASS_TILDE        0x0F    // ~ ~= ~>
#define CHAR_CLASS_EQ_FLAG      0x10    // 0x10 ~ 0x1F
#define CHAR_CLASS_SLASH        0x13    // / /= // /* />
#define CHAR_CLASS_ASTER        0x14    // * *= */ ** **=
#define CHAR_CLASS_LT           0x15    // < <= << <<= </ <- <% <| <~ <! <?
#define CHAR_CLASS_GT           0x16    // > >= >> >>=
#define CHAR_CLASS_COLON        0x17    // : :=
#define CHAR_CLASS_EMARK        0x18    // ! != !>
#define CHAR_CLASS_PLUS         0x19    // + +=
#define CHAR_CLASS_MINUS        0x1A    // - -= ->
#define CHAR_CLASS_PERCENT      0x1B    // % %= %>
#define CHAR_CLASS_AND          0x1C    // & &= &&
#define CHAR_CLASS_VERTBAR      0x1D    // | |= || |>
#define CHAR_CLASS_CARET        0x1E    // ^ ^=
#define CHAR_CLASS_EQUAL        0x1F    // = ==

#define CHAR_128_BYTE_ARRAY_G \
           /* 0           1           2           3           4           5           6           7 */      \
           /* 8           9           A           B           C           D           E           F */      \
    /*00*/ /*00*/0x01, /*01*/0x01, /*02*/0x01, /*03*/0x01, /*04*/0x01, /*05*/0x01, /*06*/0x01, /*\a*/0x01,  \
    /*08*/ /*\b*/0x01, /*\t*/0x01, /*\n*/0x06, /*\v*/0x01, /*\f*/0x01, /*\r*/0x06, /*0E*/0x01, /*0F*/0x01,  \
    /*10*/ /*10*/0x01, /*11*/0x01, /*12*/0x01, /*13*/0x01, /*14*/0x01, /*15*/0x01, /*16*/0x01, /*17*/0x01,  \
    /*18*/ /*18*/0x01, /*19*/0x01, /*1A*/0x01, /*1B*/0x01, /*1C*/0x01, /*1D*/0x01, /*1E*/0x01, /*1F*/0x01,  \
    /*20*/ /*20*/0x01, /*!!*/0x18, /*""*/0x0B, /*##*/0x05, /*$$*/0x05, /*%%*/0x1B, /*&&*/0x1C, /*''*/0x0B,  \
    /*20*/ /*((*/0x05, /*))*/0x05, /****/0x14, /*++*/0x19, /*,,*/0x05, /*--*/0x1A, /*..*/0x0C, /*//*/0x13,  \
    /*30*/ /*00*/0x02, /*11*/0x02, /*22*/0x02, /*33*/0x02, /*44*/0x02, /*55*/0x02, /*66*/0x02, /*77*/0x02,  \
    /*30*/ /*88*/0x02, /*99*/0x02, /*::*/0x17, /*;;*/0x05, /*<<*/0x15, /*==*/0x1F, /*>>*/0x16, /*??*/0x0E,  \
    /*40*/ /*@@*/0x05, /*AA*/0x03, /*BB*/0x03, /*CC*/0x03, /*DD*/0x03, /*EE*/0x03, /*FF*/0x03, /*GG*/0x03,  \
    /*40*/ /*HH*/0x03, /*II*/0x03, /*JJ*/0x03, /*KK*/0x03, /*LL*/0x03, /*MM*/0x03, /*NN*/0x03, /*OO*/0x03,  \
    /*50*/ /*PP*/0x03, /*QQ*/0x03, /*RR*/0x03, /*SS*/0x03, /*TT*/0x03, /*UU*/0x03, /*VV*/0x03, /*WW*/0x03,  \
    /*50*/ /*XX*/0x03, /*YY*/0x03, /*ZZ*/0x03, /*[[*/0x05, /*\\*/0x05, /*]]*/0x05, /*^^*/0x1E, /*__*/0x03,  \
    /*60*/ /*``*/0x0B, /*aa*/0x03, /*bb*/0x03, /*cc*/0x03, /*dd*/0x03, /*ee*/0x03, /*ff*/0x03, /*gg*/0x03,  \
    /*60*/ /*hh*/0x03, /*ii*/0x03, /*jj*/0x03, /*kk*/0x03, /*ll*/0x03, /*mm*/0x03, /*nn*/0x03, /*oo*/0x03,  \
    /*70*/ /*pp*/0x03, /*qq*/0x03, /*rr*/0x03, /*ss*/0x03, /*tt*/0x03, /*uu*/0x03, /*vv*/0x03, /*ww*/0x03,  \
    /*70*/ /*xx*/0x03, /*yy*/0x03, /*zz*/0x03, /*{{*/0x05, /*||*/0x1D, /*}}*/0x05, /*~~*/0x0F, /*7F*/0x01

// 单字符词法
#define CIFA_CH_NULL            '\0'    // 00
#define CIFA_CH_AUDIBLE_BELL    '\a'    // 07
#define CIFA_CH_BACKSPACE       '\b'    // 08
#define CIFA_CH_TAB             '\t'    // 09   horizontal tabulation (ht)
#define CIFA_CH_NEWLINE         '\n'    // 0A   line feed (lf), new line (nl), end of line (eol)
#define CIFA_CH_VT              '\v'    // 0B   vertical tabulation (vt)
#define CIFA_CH_FORMFEED        '\f'    // 0C   form feed (ff), new page
#define CIFA_CH_RETURN          '\r'    // 0D   carriage return (cr)
#define CIFA_CH_ESC             '\x1b'  // 1B   escape
#define CIFA_CH_SPACE           ' '     // 20
#define CIFA_CH_EMARK           '!'     // 21
#define CIFA_CH_NOT             '!'     // 21   逻辑非（一元操作）
#define CIFA_CH_DQUOTE          '\"'    // 22
#define CIFA_CH_STR_Q           '\"'    // 22   字符串字面量开始和结束
#define CIFA_CH_HASH            '#'     // 23
#define CIFA_CH_TYPEPREFIX      '#'     // 23   类型名称前缀
#define CIFA_CH_DOLLAR          '$'     // 24
#define CIFA_CH_PERCENT         '%'     // 25
#define CIFA_CH_MOD             '%'     // 25   取模
#define CIFA_CH_AMPERSAND       '&'     // 26
#define CIFA_CH_AND             '&'     // 26   位与
#define CIFA_CH_SQUOTE          '\''    // 27
#define CIFA_CH_RUNE_Q          '\''    // 27   字符字面量开始和结束
#define CIFA_CH_OPEN_PAREN      '('     // 28
#define CIFA_CH_CLOSE_PAREN     ')'     // 29
#define CIFA_CH_ASTER           '*'     // 2A
#define CIFA_CH_MUL             '*'     // 2A   乘法
#define CIFA_CH_PLUS            '+'     // 2B
#define CIFA_CH_ADD             '+'     // 2B   加法
#define CIFA_CH_COMMA           ','     // 2C
#define CIFA_CH_MINUS           '-'     // 2D
#define CIFA_CH_SUB             '-'     // 2D   减法
#define CIFA_CH_DOT             '.'     // 2E   成员选择或导入所有标识符
#define CIFA_CH_SLASH           '/'     // 2F
#define CIFA_CH_DIV             '/'     // 2F   除法
#define CIFA_CH_DIGIT_0         '0'     // 30
#define CIFA_CH_DIGIT_9         '9'     // 39
#define CIFA_CH_COLON           ':'     // 3A   标签、复合赋值、case/default、切片
#define CIFA_CH_SEMICOLON       ';'     // 3B
#define CIFA_CH_LT              '<'     // 3C   小于
#define CIFA_CH_EQUAL           '='     // 3D
#define CIFA_CH_ASSIGN          '='     // 3D   赋值
#define CIFA_CH_GT              '>'     // 3E   大于
#define CIFA_CH_QMARK           '?'     // 3F
#define CIFA_CH_ATSIGN          '@'     // 40
#define CIFA_CH_ATTRIBUTE       '@'     // 40   属性
#define CIFA_CH_UPPER_A         'A'     // 41
#define CIFA_CH_UPPER_Z         'Z'     // 5A
#define CIFA_CH_OPEN_SQUARE     '['     // 5B
#define CIFA_CH_BSLASH          '\\'    // 5C
#define CIFA_CH_CLOSE_SQUARE    ']'     // 5D
#define CIFA_CH_CARET           '^'     // 5E
#define CIFA_CH_XOR             '^'     // 5E   异或
#define CIFA_CH_UNDERSCORE      '_'     // 5F
#define CIFA_CH_BQUOTE          '`'     // 60
#define CIFA_CH_RSTR_Q          '`'     // 60   原始串字面量开始和结束
#define CIFA_CH_LOWER_A         'a'     // 61
#define CIFA_CH_LOWER_Z         'z'     // 7A
#define CIFA_CH_OPEN_CURLY      '{'     // 7B
#define CIFA_CH_VERTBAR         '|'     // 7C
#define CIFA_CH_BOR             '|'     // 7C   位或
#define CIFA_CH_CLOSE_CURLY     '}'     // 7D
#define CIFA_CH_TILDE           '~'     // 7E   连接泛型实参（类型实参）
#define CIFA_CH_DEL             '\x7f'  // 7F   delete
#define CIFA_CH_EOB             0xFE
#define CIFA_CH_EOF             0xFF

// 多字符词法
#define CIFA_OP_SHIFT_OFF           (CIFA_CH_LT-1)
#define CIFA_OP_SHASS_OFF           (CIFA_CH_LT-4)
#define CIFA_OP_AND_OFF(c)          ((((c)-CIFA_CH_AND)>>6)+7)
#define CIFA_OP_BASE                                                       0xFE000000
#define CIFA_OP_LSH         (CIFA_OP_BASE+CIFA_CH_LT-CIFA_OP_SHIFT_OFF) // 0xFE000001       <<      左移
#define CIFA_OP_RSH         (CIFA_OP_BASE+CIFA_CH_GT-CIFA_OP_SHIFT_OFF) // 0xFE000003       >>      右移
#define CIFA_OP_LSH_ASSIGN  (CIFA_OP_BASE+CIFA_CH_LT-CIFA_OP_SHASS_OFF) // 0xFE000004       <<=     左移并赋值
#define CIFA_OP_RSH_ASSIGN  (CIFA_OP_BASE+CIFA_CH_GT-CIFA_OP_SHASS_OFF) // 0xFE000006       >>=     右移并赋值
#define CIFA_OP_LAND        (CIFA_OP_BASE+CIFA_OP_AND_OFF(CIFA_CH_AND)) // 0xFE000007       &&      逻辑与
#define CIFA_OP_LOR         (CIFA_OP_BASE+CIFA_OP_AND_OFF(CIFA_CH_BOR)) // 0xFE000008       ||      逻辑或
#define CIFA_OP_POW                                                        0xFE00000A   //  **      取幂
#define CIFA_OP_POW_ASSIGN                                                 0xFE00000B   //  **=     取幂并赋值
#define CIFA_OP_DOUBLE_SLASH                                               0xFE00000C   //  //      行注释开始
#define CIFA_OP_SLASH_ASTER                                                0xFE00000D   //  /*      块注释开始
#define CIFA_OP_ASTER_SLASH                                                0xFE00000E   //  */      块注释结束
#define CIFA_OP_LSTR_Q                                                     0xFE00000F   //  '''     长字符串字面量开始
#define CIFA_OP_2DOT                                                       0xFE000010   //  ..      未定义
#define CIFA_OP_3DOT                                                       0xFE000011   //  ...     可变个数参数或元素
#define CIFA_OP_ARROW                                                      0xFE000012   //  ->      成员选择
#define CIFA_OP_RREF                                                       0xFE000013   //  &&      右值引用
#define CIFA_OP_REF                                                        0xFE000014   //  &       引用
#define CIFA_OP_ADDR                                                       0xFE000015   //  &       取地址（一元操作）
#define CIFA_OP_COMPL                                                      0xFE000016   //  ^       位反（一元操作）
#define CIFA_OP_DREF                                                       0xFE000017   //  *       解引用（一元操作）
#define CIFA_OP_POINTER                                                    0xFE000018   //  *       类型指针（Type*）
#define CIFA_OP_POINPOINTER                                                0xFE000019   //  **      指针的指针（Type**）
#define CIFA_OP_NE                  (CIFA_OP_BASE+CIFA_CH_NOT)          // 0xFE000021       !=      不等于
#define CIFA_OP_MOD_ASSIGN          (CIFA_OP_BASE+CIFA_CH_MOD)          // 0xFE000025       %=      取模并赋值
#define CIFA_OP_AND_ASSIGN          (CIFA_OP_BASE+CIFA_CH_AND)          // 0xFE000026       &=      位与并赋值
#define CIFA_OP_MUL_ASSIGN          (CIFA_OP_BASE+CIFA_CH_MUL)          // 0xFE00002A       *=      乘法并赋值
#define CIFA_OP_ADD_ASSIGN          (CIFA_OP_BASE+CIFA_CH_ADD)          // 0xFE00002B       +=      加法并赋值
#define CIFA_OP_SUB_ASSIGN          (CIFA_OP_BASE+CIFA_CH_SUB)          // 0xFE00002D       -=      减法并赋值
#define CIFA_OP_DIV_ASSIGN          (CIFA_OP_BASE+CIFA_CH_DIV)          // 0xFE00002F       /=      除法并赋值
#define CIFA_OP_LE                  (CIFA_OP_BASE+CIFA_CH_LT)           // 0xFE00003C       <=      小于等于
#define CIFA_OP_EQ                  (CIFA_OP_BASE+CIFA_CH_ASSIGN)       // 0xFE00003D       ==      等于
#define CIFA_OP_GE                  (CIFA_OP_BASE+CIFA_CH_GT)           // 0xFE00003E       >=      大于等于
#define CIFA_OP_INI_ASSIGN          (CIFA_OP_BASE+CIFA_CH_COLON)        // 0xFE00003A       :=      初始化赋值
#define CIFA_OP_XOR_ASSIGN          (CIFA_OP_BASE+CIFA_CH_XOR)          // 0xFE00005E       ^=      异或并赋值
#define CIFA_OP_BOR_ASSIGN          (CIFA_OP_BASE+CIFA_CH_BOR)          // 0xFE00007C       |=      位或并赋值
#define CIFA_OP_LAST                                                       0xFE000080

// 注释、字面量
#define CIFA_CF_START       0xFE000080
#define CIFA_CF_COMMENT     0xFE000081
#define CIFA_CF_BLOCK_CMMT  0xFE000091
#define CIFA_CF_STR_LIT     0xFE000082
#define CIFA_CF_RSTR_LIT    0xFE000092
#define CIFA_CF_RUNE_LIT    0xFE0000A2
#define CIFA_CF_INT_LIT     0xFE000084
#define CIFA_CF_FLOAT_LIT   0xFE000094
#define CIFA_CF_BOOL_LIT    0xFE0000A4
#define CIFA_CF_NIL_LIT     0xFE0000B4
#define CIFA_CF_LAST        0xFE0000FF

// 标识符
#define CIFA_ID_LAST    0xFFFFFFFF // 可最多表示 0x01FFFF00 个标识符（33,554,176），三千多万个
enum chcc_predecl_ident {
    CIFA_ID_START = 0xFE0000FF,
#define PREDECL(id, ...) id,
#include "chcc/decl.h"
};

typedef struct {
    int fd;
    Uint c_cnt;     // 词法前缀包含的字符个数
    Uint c_len;     // 词法前缀的字节长度，可能包含多个字符
    rune cur_c;     // 词法前缀经过解析后，用最多4字节长的utf8字符表示，如果词法前缀包含多个字符使用CIFA_OP_XXX形式表示
    Uint line_num;  // 当前词法前缀所在行
    Uint col_chars; // 当前词法前缀最后一个字符所在列（以字符为单位）
    Uint col_bytes; // 当前词法前缀最后一个字节所在列（以字节为单位）
    byte *curstr;   // 当前字符串的起始位置
    buffer_t cpstr; // 词法解析过程中复制词法字符串
    buffix_t b;     // 必须为最后一个字段，b.cur 指向当前词法前缀的最后一个字节
} bufile_t;

#define FILEBUF_DEFAULT_SIZE 8096
bufile_t *bufile_new(const char *filename, string_t s, Int filebufsize);
void bufile_load_string(bufile_t *f, string_t s);
void bufile_delete(bufile_t *p);

typedef uint32 cfid_t;

typedef struct {
    cfid_t ident_id;
    string_t ident;
} cfsym_t;

typedef union {
    uint64 i;
    float64 f;
    cfsym_t *sym;
} cfval_t;

typedef struct {
    cfval_t val;
    string_t s;
    cfid_t cfid;
    Uint cf_line;
    Uint cf_col;
    Error error;
} cfys_t;

typedef struct {
    bufile_t *f;
    cfys_t curcf;
    Uint user_ident_start;
    bhash2_t hash_ident;
    array2_ex_t arry_ident;
} cfst_t;

void cfst_init(cfst_t *cfst, bufile_t *f);
void cfst_free(cfst_t *cfst);
void curr(cfst_t *cfst);

#define ERROR_CMMT_NOT_CLOSED   0xE0
#define ERROR_INVALID_ESCCHAR   0xE1
#define ERROR_INVALID_HEXCHAR   0xE2
#define ERROR_INVALID_HEXNUMB   0xE3
#define ERROR_INVALID_UNICODE   0xE4
#define ERROR_MISS_CLOSE_QUOTE  0xE5
#define ERROR_EMPTY_RUNE_LIT    0xE6
#define ERROR_MULT_CHAR_EXIST   0xE7

#define __CHCC_DEBUG__ 1

#endif /* CHAPL_LANG_CHCC_CIFA_H */
