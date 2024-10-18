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

// 词法分类
#define CIFA_CLASS_OTHERBYTE    0x01    // 其他字节
#define CIFA_CLASS_DIGIT        0x02    // 0 ~ 9
#define CIFA_CLASS_UPPER        0x03    // A ~ Z
#define CIFA_CLASS_LOWER        0x04    // a ~ z
#define CIFA_CLASS_UNDERSCORE   0x05    // _
#define CIFA_CLASS_SPEC_SIGN    0x06    // # $ @
#define CIFA_CLASS_PAREN        0x07    // ( )
#define CIFA_CLASS_SQUARE       0x08    // [ ]
#define CIFA_CLASS_CURLY        0x09    // { }
#define CIFA_CLASS_NEWLINE      0x0A    // \r \r\n \n
#define CIFA_CLASS_QUOTE        0x0B    // ' ''' " `
#define CIFA_CLASS_DOT          0x0C    // . .. ... .1
#define CIFA_CLASS_DELIM        0x0D    // , ;
#define CIFA_CLASS_QMARK        0x0E    // ? ?: ?>
#define CIFA_CLASS_TILDE        0x0F    // ~ ~= ~>
#define CIFA_CLASS_EQ_FLAG      0x10    // 0x10 ~ 0x1F
#define CIFA_CLASS_SLASH        0x13    // / /= // /* />
#define CIFA_CLASS_ASTER        0x14    // * *= ** **= */
#define CIFA_CLASS_LT           0x15    // < <= << <<= </ <- <% <| <~ <! <?
#define CIFA_CLASS_GT           0x16    // > >= >> >>=
#define CIFA_CLASS_COLON        0x17    // : :=
#define CIFA_CLASS_EMARK        0x18    // ! != !>
#define CIFA_CLASS_PLUS         0x19    // + +=
#define CIFA_CLASS_MINUS        0x1A    // - -= ->
#define CIFA_CLASS_PERCENT      0x1B    // % %= %>
#define CIFA_CLASS_AND          0x1C    // & &= &&
#define CIFA_CLASS_VERTBAR      0x1D    // | |= || |>
#define CIFA_CLASS_CARET        0x1E    // ^ ^=
#define CIFA_CLASS_EQUAL        0x1F    // = ==
#define CIFA_CLASS_DOT_DIGIT    0x20    // .1

#define CIFA_128_BYTE_ARRAY_G \
           /* 0           1           2           3           4           5           6           7 */      \
           /* 8           9           A           B           C           D           E           F */      \
    /*00*/ /*00*/0x01, /*01*/0x01, /*02*/0x01, /*03*/0x01, /*04*/0x01, /*05*/0x01, /*06*/0x01, /*07*/0x01,  \
    /*08*/ /*08*/0x01, /*09*/0x01, /*\n*/0x0A, /*0B*/0x01, /*0C*/0x01, /*\r*/0x0A, /*0E*/0x01, /*0F*/0x01,  \
    /*10*/ /*10*/0x01, /*11*/0x01, /*12*/0x01, /*13*/0x01, /*14*/0x01, /*15*/0x01, /*16*/0x01, /*17*/0x01,  \
    /*18*/ /*18*/0x01, /*19*/0x01, /*1A*/0x01, /*1B*/0x01, /*1C*/0x01, /*1D*/0x01, /*1E*/0x01, /*1F*/0x01,  \
    /*20*/ /*20*/0x01, /*!!*/0x18, /*""*/0x0B, /*##*/0x06, /*$$*/0x06, /*%%*/0x1B, /*&&*/0x1C, /*''*/0x0B,  \
    /*20*/ /*((*/0x07, /*))*/0x07, /****/0x14, /*++*/0x19, /*,,*/0x0D, /*--*/0x1A, /*..*/0x0C, /*//*/0x13,  \
    /*30*/ /*00*/0x02, /*11*/0x02, /*22*/0x02, /*33*/0x02, /*44*/0x02, /*55*/0x02, /*66*/0x02, /*77*/0x02,  \
    /*30*/ /*88*/0x02, /*99*/0x02, /*::*/0x17, /*;;*/0x0D, /*<<*/0x15, /*==*/0x1F, /*>>*/0x16, /*??*/0x0E,  \
    /*40*/ /*@@*/0x06, /*AA*/0x03, /*BB*/0x03, /*CC*/0x03, /*DD*/0x03, /*EE*/0x03, /*FF*/0x03, /*GG*/0x03,  \
    /*40*/ /*HH*/0x03, /*II*/0x03, /*JJ*/0x03, /*KK*/0x03, /*LL*/0x03, /*MM*/0x03, /*NN*/0x03, /*OO*/0x03,  \
    /*50*/ /*PP*/0x03, /*QQ*/0x03, /*RR*/0x03, /*SS*/0x03, /*TT*/0x03, /*UU*/0x03, /*VV*/0x03, /*WW*/0x03,  \
    /*50*/ /*XX*/0x03, /*YY*/0x03, /*ZZ*/0x03, /*[[*/0x08, /*\\*/0x01, /*]]*/0x08, /*^^*/0x1E, /*__*/0x05,  \
    /*60*/ /*``*/0x0B, /*aa*/0x04, /*bb*/0x04, /*cc*/0x04, /*dd*/0x04, /*ee*/0x04, /*ff*/0x04, /*gg*/0x04,  \
    /*60*/ /*hh*/0x04, /*ii*/0x04, /*jj*/0x04, /*kk*/0x04, /*ll*/0x04, /*mm*/0x04, /*nn*/0x04, /*oo*/0x04,  \
    /*70*/ /*pp*/0x04, /*qq*/0x04, /*rr*/0x04, /*ss*/0x04, /*tt*/0x04, /*uu*/0x04, /*vv*/0x04, /*ww*/0x04,  \
    /*70*/ /*xx*/0x04, /*yy*/0x04, /*zz*/0x04, /*{{*/0x09, /*||*/0x1D, /*}}*/0x09, /*~~*/0x0F, /*7F*/0x01

// 单字节词法
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

// 多字节词法
#define CIFA_CF_SHIFT_OFF           (CIFA_CH_LT-1)
#define CIFA_CF_SHASS_OFF           (CIFA_CH_LT-4)
#define CIFA_CF_AND_OFF(c)          ((((c)-CIFA_CH_AND)>>6)+7)
#define CIFA_CF_BASE                                                       0xFE000000
#define CIFA_CF_LSH         (CIFA_CF_BASE+CIFA_CH_LT-CIFA_CF_SHIFT_OFF) // 0xFE000001       <<      左移
#define CIFA_CF_RSH         (CIFA_CF_BASE+CIFA_CH_GT-CIFA_CF_SHIFT_OFF) // 0xFE000003       >>      右移
#define CIFA_CF_LSH_ASSIGN  (CIFA_CF_BASE+CIFA_CH_LT-CIFA_CF_SHASS_OFF) // 0xFE000004       <<=     左移并赋值
#define CIFA_CF_RSH_ASSIGN  (CIFA_CF_BASE+CIFA_CH_GT-CIFA_CF_SHASS_OFF) // 0xFE000006       >>=     右移并赋值
#define CIFA_CF_LAND        (CIFA_CF_BASE+CIFA_CF_AND_OFF(CIFA_CH_AND)) // 0xFE000007       &&      逻辑与
#define CIFA_CF_LOR         (CIFA_CF_BASE+CIFA_CF_AND_OFF(CIFA_CH_BOR)) // 0xFE000008       ||      逻辑或
#define CIFA_CF_POW                                                        0xFE00000A   //  **      取幂
#define CIFA_CF_POW_ASSIGN                                                 0xFE00000B   //  **=     取幂并赋值
#define CIFA_CF_LINE_CMMT                                                  0xFE00000C   //  //      行注释开始
#define CIFA_CF_BLOCK_CMMT                                                 0xFE00000D   //  /*      块注释开始
#define CIFA_CF_END_BLOCK_CMMT                                             0xFE00000E   //  */      块注释结束
#define CIFA_CF_LSTR_Q                                                     0xFE00000F   //  '''     长字符串字面量开始
#define CIFA_CF_2DOT                                                       0xFE000010   //  ..      未定义
#define CIFA_CF_3DOT                                                       0xFE000011   //  ...     可变个数参数或元素
#define CIFA_CF_ARROW                                                      0xFE000012   //  ->      成员选择
#define CIFA_CF_RREF                                                       0xFE000013   //  &&      右值引用
#define CIFA_CF_REF                                                        0xFE000014   //  &       引用
#define CIFA_CF_ADDR                                                       0xFE000015   //  &       取地址（一元操作）
#define CIFA_CF_COMPL                                                      0xFE000016   //  ^       位反（一元操作）
#define CIFA_CF_DREF                                                       0xFE000017   //  *       解引用（一元操作）
#define CIFA_CF_POINTER                                                    0xFE000018   //  *       类型指针（Type*）
#define CIFA_CF_NE                  (CIFA_CF_BASE+CIFA_CH_NOT)          // 0xFE000021       !=      不等于
#define CIFA_CF_MOD_ASSIGN          (CIFA_CF_BASE+CIFA_CH_MOD)          // 0xFE000025       %=      取模并赋值
#define CIFA_CF_AND_ASSIGN          (CIFA_CF_BASE+CIFA_CH_AND)          // 0xFE000026       &=      位与并赋值
#define CIFA_CF_MUL_ASSIGN          (CIFA_CF_BASE+CIFA_CH_MUL)          // 0xFE00002A       *=      乘法并赋值
#define CIFA_CF_ADD_ASSIGN          (CIFA_CF_BASE+CIFA_CH_ADD)          // 0xFE00002B       +=      加法并赋值
#define CIFA_CF_SUB_ASSIGN          (CIFA_CF_BASE+CIFA_CH_SUB)          // 0xFE00002D       -=      减法并赋值
#define CIFA_CF_DIV_ASSIGN          (CIFA_CF_BASE+CIFA_CH_DIV)          // 0xFE00002F       /=      除法并赋值
#define CIFA_CF_LE                  (CIFA_CF_BASE+CIFA_CH_LT)           // 0xFE00003C       <=      小于等于
#define CIFA_CF_EQ                  (CIFA_CF_BASE+CIFA_CH_ASSIGN)       // 0xFE00003D       ==      等于
#define CIFA_CF_GE                  (CIFA_CF_BASE+CIFA_CH_GT)           // 0xFE00003E       >=      大于等于
#define CIFA_CF_INI_ASSIGN          (CIFA_CF_BASE+CIFA_CH_COLON)        // 0xFE00003A       :=      初始化赋值
#define CIFA_CF_XOR_ASSIGN          (CIFA_CF_BASE+CIFA_CH_XOR)          // 0xFE00005E       ^=      异或并赋值
#define CIFA_CF_BOR_ASSIGN          (CIFA_CF_BASE+CIFA_CH_BOR)          // 0xFE00007C       |=      位或并赋值

// 字面量
#define CIFA_LT_START       0xFE0000A0
#define CIFA_LT_LAST        0xFE0000FF

// 标识符
#define CIFA_ID_START       0xFE000100
#define CIFA_ID_LAST        0xFFFFFFFF // 可最多表示 0x01FFFF00 个标识符（33,554,176），三千多万个
enum chcc_predecl_ident {
    CIFA_ID_BEFORE_START = CIFA_ID_START - 1,
#define PREDECL(id, ...) id,
#include "chcc/decl.h"
};

typedef struct {
    int fd;
    uint8 b_rst: 1; // b.a 数组是否被重置用来加载后续内容块
    uint8 c_cnt: 4; // 词法前缀包含的字符个数
    uint8 c_class;  // 词法前缀的类别
    uint8 c_len;    // 词法前缀的字节长度，可能包含多个字符
    rune cur_c;     // 词法前缀经过解析后，用最多4字节长的utf8字符表示，如果词法前缀包含多个字符使用CIFA_CF_XXX形式表示
    Uint line_num;  // 当前词法前缀所在行
    Uint col_chars; // 当前词法前缀最后一个字符所在列（以字符为单位）
    Uint col_bytes; // 当前词法前缀最后一个字节所在列（以字节为单位）
    buffix_t b;     // 必须为最后一个字段，b.cur 指向当前词法前缀的最后一个字节
} bufile_t;

bufile_t *bufile_new(const char *filename, const byte *str, Uint len);
void bufile_delete(bufile_t *p);

typedef int32 cfid_t;

typedef struct {
    cfid_t ident_id;
    Uint ident_len;
} cfsym_t;

typedef union {
    uint64 i;
    float32 f32;
    float64 f64;
    buffer_t s;
} cfval_t;

typedef struct {
    cfid_t cfid;
    uint8 cfclass;
    uint8 valid;
    Uint cf_line;
    Uint cf_col;
    cfsym_t *sym;
    cfval_t val;
} cfres_t;

typedef struct {
    bufile_t *f;
    buffer_t cpstr;
    cfres_t curcf;
    Uint user_ident_start;
    bhash2_t hash_ident;
    array2_ex_t arry_ident;
} cfstate_t;

void cifa_init(cfstate_t *cfst, bufile_t *f);
void cifa_free(cfstate_t *cfst);

#endif /* CHAPL_LANG_CHCC_CIFA_H */
