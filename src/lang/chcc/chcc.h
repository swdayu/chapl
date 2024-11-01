#ifndef CHAPL_LANG_CHCC_H
#define CHAPL_LANG_CHCC_H
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
#define CHAR_CLASS_UPPER        0x03    // A ~ Z
#define CHAR_CLASS_LOWER        0x04    // a ~ z
#define CHAR_CLASS_UNDERSCORE   0x05    // _
#define CHAR_CLASS_NEWLINE      0x06    // \r \r\n \n
#define CHAR_CLASS_SINGLE       0x07    // # $ @ ( ) [ ] { } , ; 反斜杠
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
    /*20*/ /*20*/0x01, /*!!*/0x18, /*""*/0x0B, /*##*/0x07, /*$$*/0x07, /*%%*/0x1B, /*&&*/0x1C, /*''*/0x0B,  \
    /*20*/ /*((*/0x07, /*))*/0x07, /****/0x14, /*++*/0x19, /*,,*/0x07, /*--*/0x1A, /*..*/0x0C, /*//*/0x13,  \
    /*30*/ /*00*/0x02, /*11*/0x02, /*22*/0x02, /*33*/0x02, /*44*/0x02, /*55*/0x02, /*66*/0x02, /*77*/0x02,  \
    /*30*/ /*88*/0x02, /*99*/0x02, /*::*/0x17, /*;;*/0x07, /*<<*/0x15, /*==*/0x1F, /*>>*/0x16, /*??*/0x0E,  \
    /*40*/ /*@@*/0x07, /*AA*/0x03, /*BB*/0x03, /*CC*/0x03, /*DD*/0x03, /*EE*/0x03, /*FF*/0x03, /*GG*/0x03,  \
    /*40*/ /*HH*/0x03, /*II*/0x03, /*JJ*/0x03, /*KK*/0x03, /*LL*/0x03, /*MM*/0x03, /*NN*/0x03, /*OO*/0x03,  \
    /*50*/ /*PP*/0x03, /*QQ*/0x03, /*RR*/0x03, /*SS*/0x03, /*TT*/0x03, /*UU*/0x03, /*VV*/0x03, /*WW*/0x03,  \
    /*50*/ /*XX*/0x03, /*YY*/0x03, /*ZZ*/0x03, /*[[*/0x07, /*\\*/0x07, /*]]*/0x07, /*^^*/0x1E, /*__*/0x05,  \
    /*60*/ /*``*/0x0B, /*aa*/0x04, /*bb*/0x04, /*cc*/0x04, /*dd*/0x04, /*ee*/0x04, /*ff*/0x04, /*gg*/0x04,  \
    /*60*/ /*hh*/0x04, /*ii*/0x04, /*jj*/0x04, /*kk*/0x04, /*ll*/0x04, /*mm*/0x04, /*nn*/0x04, /*oo*/0x04,  \
    /*70*/ /*pp*/0x04, /*qq*/0x04, /*rr*/0x04, /*ss*/0x04, /*tt*/0x04, /*uu*/0x04, /*vv*/0x04, /*ww*/0x04,  \
    /*70*/ /*xx*/0x04, /*yy*/0x04, /*zz*/0x04, /*{{*/0x07, /*||*/0x1D, /*}}*/0x07, /*~~*/0x0F, /*7F*/0x01

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
#define CIFA_OP_BASE                                                       0xF8000000
#define CIFA_OP_LSH         (CIFA_OP_BASE+CIFA_CH_LT-CIFA_OP_SHIFT_OFF) // 0xF8000001       <<      左移
#define CIFA_OP_RSH         (CIFA_OP_BASE+CIFA_CH_GT-CIFA_OP_SHIFT_OFF) // 0xF8000003       >>      右移
#define CIFA_OP_LSH_ASSIGN  (CIFA_OP_BASE+CIFA_CH_LT-CIFA_OP_SHASS_OFF) // 0xF8000004       <<=     左移并赋值
#define CIFA_OP_RSH_ASSIGN  (CIFA_OP_BASE+CIFA_CH_GT-CIFA_OP_SHASS_OFF) // 0xF8000006       >>=     右移并赋值
#define CIFA_OP_LAND        (CIFA_OP_BASE+CIFA_OP_AND_OFF(CIFA_CH_AND)) // 0xF8000007       &&      逻辑与
#define CIFA_OP_LOR         (CIFA_OP_BASE+CIFA_OP_AND_OFF(CIFA_CH_BOR)) // 0xF8000008       ||      逻辑或
#define CIFA_OP_POW                                                        0xF800000A   //  **      取幂
#define CIFA_OP_POW_ASSIGN                                                 0xF800000B   //  **=     取幂并赋值
#define CIFA_OP_DOUBLE_SLASH                                               0xF800000C   //  //      行注释开始
#define CIFA_OP_SLASH_ASTER                                                0xF800000D   //  /*      块注释开始
#define CIFA_OP_ASTER_SLASH                                                0xF800000E   //  */      块注释结束
#define CIFA_OP_LSTR_Q                                                     0xF800000F   //  '''     长字符串字面量开始
#define CIFA_OP_2DOT                                                       0xF8000010   //  ..      未定义
#define CIFA_OP_3DOT                                                       0xF8000011   //  ...     可变个数参数或元素
#define CIFA_OP_ARROW                                                      0xF8000012   //  ->      成员选择
#define CIFA_OP_RREF                                                       0xF8000013   //  &&      右值引用
#define CIFA_OP_REF                                                        0xF8000014   //  &       引用
#define CIFA_OP_ADDR                                                       0xF8000015   //  &       取地址（一元操作）
#define CIFA_OP_COMPL                                                      0xF8000016   //  ^       位反（一元操作）
#define CIFA_OP_DREF                                                       0xF8000017   //  *       解引用（一元操作）
#define CIFA_OP_POINTER                                                    0xF8000018   //  *       类型指针（Type*）
#define CIFA_OP_POINPOINTER                                                0xF8000019   //  **      指针的指针（Type**）
#define CIFA_OP_NE                  (CIFA_OP_BASE+CIFA_CH_NOT)          // 0xF8000021       !=      不等于
#define CIFA_OP_MOD_ASSIGN          (CIFA_OP_BASE+CIFA_CH_MOD)          // 0xF8000025       %=      取模并赋值
#define CIFA_OP_AND_ASSIGN          (CIFA_OP_BASE+CIFA_CH_AND)          // 0xF8000026       &=      位与并赋值
#define CIFA_OP_MUL_ASSIGN          (CIFA_OP_BASE+CIFA_CH_MUL)          // 0xF800002A       *=      乘法并赋值
#define CIFA_OP_ADD_ASSIGN          (CIFA_OP_BASE+CIFA_CH_ADD)          // 0xF800002B       +=      加法并赋值
#define CIFA_OP_SUB_ASSIGN          (CIFA_OP_BASE+CIFA_CH_SUB)          // 0xF800002D       -=      减法并赋值
#define CIFA_OP_DIV_ASSIGN          (CIFA_OP_BASE+CIFA_CH_DIV)          // 0xF800002F       /=      除法并赋值
#define CIFA_OP_LE                  (CIFA_OP_BASE+CIFA_CH_LT)           // 0xF800003C       <=      小于等于
#define CIFA_OP_EQ                  (CIFA_OP_BASE+CIFA_CH_ASSIGN)       // 0xF800003D       ==      等于
#define CIFA_OP_GE                  (CIFA_OP_BASE+CIFA_CH_GT)           // 0xF800003E       >=      大于等于
#define CIFA_OP_INI_ASSIGN          (CIFA_OP_BASE+CIFA_CH_COLON)        // 0xF800003A       :=      初始化赋值
#define CIFA_OP_XOR_ASSIGN          (CIFA_OP_BASE+CIFA_CH_XOR)          // 0xF800005E       ^=      异或并赋值
#define CIFA_OP_BOR_ASSIGN          (CIFA_OP_BASE+CIFA_CH_BOR)          // 0xF800007C       |=      位或并赋值
#define CIFA_OP_LAST                                                       0xF8000080

// 注释、字面量
#define CIFA_CF_START       0xF8000080
#define CIFA_CF_COMMENT     0xF8000081
#define CIFA_CF_BLOCK_CMMT  0xF8000091
#define CIFA_CF_STR_LIT     0xF8000082
#define CIFA_CF_RSTR_LIT    0xF8000092
#define CIFA_CF_RUNE_LIT    0xF80000A2
#define CIFA_CF_INT_LIT     0xF8000084
#define CIFA_CF_FLOAT_LIT   0xF8000094
#define CIFA_CF_BOOL_LIT    0xF80000A4
#define CIFA_CF_NIL_LIT     0xF80000B4
#define CIFA_CF_LAST        0xF80000FF

// 匿名符号
#define CIFA_ID_ANON_START  0xF8000100
#define CIFA_ID_ANON_LAST   0xFBFFFFFF // 最多可表示 0x03FFFF00 个匿名符号（67,108,608），六千多万个

// 标识符
#define CIFA_ID_START   0xFC000000
#define CIFA_ID_LAST    0xFFFFFFFF // 最多可表示 0x04000000 个标识符（67,108,864），六千多万个
enum chcc_predecl_ident {
    CIFA_ID_BEFORE_START = 0xFBFFFFFF,
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
    buffix_t b;     // 必须为最后一个字段，b.cur 指向当前词法前缀的最后一个字节
} bufile_t;

#define FILEBUF_DEFAULT_SIZE 8096
bufile_t *bufile_new(const char *filename, string_t s, Int filebufsize);
void bufile_load_string(bufile_t *f, string_t s);
void bufile_delete(bufile_t *p);

typedef uint32 cfid_t;

// 标识符可以命名：
// 1. 包名
// 2. 标签
// 3. 类型和类型别名
// 4. 类型约束
// 5. 类型参数
// 6. 函数和方法
// 7. 变量名
// 8. 常量

#define SCOPE_GLOBAL 0
#define SCOPE_STRUCT_FIELD 1
#define SCOPE_LOCAL_START 2

typedef struct {
    cfid_t ident; // 0表示匿名符号
    uint32 scope; // 符号所属所用域
} sym_t;

#define SYMBOL_IS_TYPE  0x10
#define SYM_IS_TYPE_REF 0x08
#define SYM_TYPE_BASIC  0x11
#define SYM_TYPE_STRUCT 0x12
#define SYM_TYPE_IF     0x13
#define SYM_TYPE_FPTR   0x14
#define SYM_TYPE_FOBJ   0x15

typedef struct {
    cfid_t id;
    uint32 is_type: 5;
    uint32 is_const: 1;
    uint32 is_ident: 1;
    uint32 is_label: 1;
    uint32 is_variable: 1;
    uint32 is_func: 1;
    uint32 is_method: 1;
    string_t s;
    union {
        sym_t *sym_type; // 当名称作为类型名使用时
        stack_t sym_ident;
    } u;
} cfsym_t;

typedef union {
    uint64 i;
    float64 f;
    cfsym_t *sym;
    Uint slen;
} cfval_t;

typedef struct {
    cfval_t val;
    string_t s;
    cfid_t cfid;
    Uint cf_line;
    Uint cf_col;
} cfys_t;

typedef struct {
    sym_t sym;
    uint32 kind: 5;
    uint32 dyn: 1; // 类型大小不固定
    uint32 align: 4; // (1 << align)
    Uint size;
    Uint off;
} type_t;

typedef struct {
    type_t type;
    cfid_t ident;
} var_t; // 符号是类型变量

typedef struct {
    type_t type;
    cfid_t ident;
    cfval_t val;
} const_t;

typedef struct {
    type_t type; // type.ident值为0时是匿名结构体
    stack_t field;
} struct_t;

typedef struct {
    bufile_t *f;
    cfys_t curcf;
    buffer_t cpstr;
    Uint user_id_start;
    Uint stack_init_len;
    bhash2_t hash_ident;
    array2_ex_t arry_ident;
    Error error;
    uint32 scope_seed;
    uint32 anon_id_seed;
    struct stack_it *global; // 全局符号栈顶
    struct stack_it *field; // 结构体类型声明时的成员栈顶
    stack_t symbol; // 当前语法解析时的符号栈
} chcc_t;

void chcc_init(chcc_t *cc, bufile_t *f);
void chcc_free(chcc_t *cc);
void curr(chcc_t *cc);
cfsym_t *cfsym(chcc_t *cc, cfid_t id);

#define ERROR_CMMT_NOT_CLOSED   0xE0
#define ERROR_INVALID_ESCCHAR   0xE1
#define ERROR_INVALID_HEXCHAR   0xE2
#define ERROR_INVALID_HEXNUMB   0xE3
#define ERROR_INVALID_UNICODE   0xE4
#define ERROR_MISS_CLOSE_QUOTE  0xE5
#define ERROR_EMPTY_RUNE_LIT    0xE6
#define ERROR_MULT_CHAR_EXIST   0xE7
#define ERROR_IS_NOT_IDENT      0xEA
#define ERROR_IS_NOT_TYPE       0xEB
#define ERROR_IS_SAME_TYPE      0xEC
#define ERROR_NAME_RESERVED     0xED

#define __CHCC_DEBUG__ 1

#endif /* CHAPL_LANG_CHCC_H */
