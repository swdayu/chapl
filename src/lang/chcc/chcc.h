#ifndef CHAPL_LANG_CHCC_H
#define CHAPL_LANG_CHCC_H
#include "builtin/decl.h"
#include "direct/file.h"

#define __CHCC_DEBUG__ 1

// 词法元素包括：
// 1. 操作符（operator），一元操作符包括 + - ^ ! * &
// 2. 关键字（keyword）
// 3. 内置预声明（predeclared identifier）
// 4. 注释和空白（comment）
// 5. 字面量（literal）
// 6. 标识符（identifier）

// 字符分类
#define CHAR_CLASS_BLANK        0x01    // 空白字符
#define CHAR_CLASS_DIGIT        0x02    // 0 ~ 9
#define CHAR_CLASS_UPPER        0x03    // A ~ Z
#define CHAR_CLASS_LOWER        0x04    // a ~ z
#define CHAR_CLASS_UNDERSCORE   0x05    // _
#define CHAR_CLASS_DOT          0x06    // . .. ... .1
#define CHAR_CLASS_QUOTE        0x08    // ' ''' " `
#define CHAR_CLASS_SINGLE       0x09    // # $ @ ( ) [ ] { } , ;
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
    /*08*/ /*\b*/0x01, /*\t*/0x01, /*\n*/0x01, /*\v*/0x01, /*\f*/0x01, /*\r*/0x01, /*0E*/0x01, /*0F*/0x01,  \
    /*10*/ /*10*/0x01, /*11*/0x01, /*12*/0x01, /*13*/0x01, /*14*/0x01, /*15*/0x01, /*16*/0x01, /*17*/0x01,  \
    /*18*/ /*18*/0x01, /*19*/0x01, /*1A*/0x01, /*1B*/0x01, /*1C*/0x01, /*1D*/0x01, /*1E*/0x01, /*1F*/0x01,  \
    /*20*/ /*20*/0x01, /*!!*/0x18, /*""*/0x08, /*##*/0x09, /*$$*/0x09, /*%%*/0x1B, /*&&*/0x1C, /*''*/0x08,  \
    /*20*/ /*((*/0x09, /*))*/0x09, /****/0x14, /*++*/0x19, /*,,*/0x09, /*--*/0x1A, /*..*/0x06, /*//*/0x13,  \
    /*30*/ /*00*/0x02, /*11*/0x02, /*22*/0x02, /*33*/0x02, /*44*/0x02, /*55*/0x02, /*66*/0x02, /*77*/0x02,  \
    /*30*/ /*88*/0x02, /*99*/0x02, /*::*/0x17, /*;;*/0x09, /*<<*/0x15, /*==*/0x1F, /*>>*/0x16, /*??*/0x0E,  \
    /*40*/ /*@@*/0x09, /*AA*/0x03, /*BB*/0x03, /*CC*/0x03, /*DD*/0x03, /*EE*/0x03, /*FF*/0x03, /*GG*/0x03,  \
    /*40*/ /*HH*/0x03, /*II*/0x03, /*JJ*/0x03, /*KK*/0x03, /*LL*/0x03, /*MM*/0x03, /*NN*/0x03, /*OO*/0x03,  \
    /*50*/ /*PP*/0x03, /*QQ*/0x03, /*RR*/0x03, /*SS*/0x03, /*TT*/0x03, /*UU*/0x03, /*VV*/0x03, /*WW*/0x03,  \
    /*50*/ /*XX*/0x03, /*YY*/0x03, /*ZZ*/0x03, /*[[*/0x09, /*\\*/0x01, /*]]*/0x09, /*^^*/0x1E, /*__*/0x05,  \
    /*60*/ /*``*/0x08, /*aa*/0x04, /*bb*/0x04, /*cc*/0x04, /*dd*/0x04, /*ee*/0x04, /*ff*/0x04, /*gg*/0x04,  \
    /*60*/ /*hh*/0x04, /*ii*/0x04, /*jj*/0x04, /*kk*/0x04, /*ll*/0x04, /*mm*/0x04, /*nn*/0x04, /*oo*/0x04,  \
    /*70*/ /*pp*/0x04, /*qq*/0x04, /*rr*/0x04, /*ss*/0x04, /*tt*/0x04, /*uu*/0x04, /*vv*/0x04, /*ww*/0x04,  \
    /*70*/ /*xx*/0x04, /*yy*/0x04, /*zz*/0x04, /*{{*/0x09, /*||*/0x1D, /*}}*/0x09, /*~~*/0x0F, /*7F*/0x01

// 多字符词法
#define CIFA_OP_SHIFT_OFF           (CHAR_LT-1)
#define CIFA_OP_SHASS_OFF           (CHAR_LT-4)
#define CIFA_OP_AND_OFF(c)          ((((c)-CHAR_AND)>>6)+7)
#define CIFA_OP_BASE                                                       0xF8000000
#define CIFA_OP_LSH         (CIFA_OP_BASE+CHAR_LT-CIFA_OP_SHIFT_OFF) // 0xF8000001       <<      左移
#define CIFA_OP_RSH         (CIFA_OP_BASE+CHAR_GT-CIFA_OP_SHIFT_OFF) // 0xF8000003       >>      右移
#define CIFA_OP_LSH_ASSIGN  (CIFA_OP_BASE+CHAR_LT-CIFA_OP_SHASS_OFF) // 0xF8000004       <<=     左移并赋值
#define CIFA_OP_RSH_ASSIGN  (CIFA_OP_BASE+CHAR_GT-CIFA_OP_SHASS_OFF) // 0xF8000006       >>=     右移并赋值
#define CIFA_OP_LAND        (CIFA_OP_BASE+CIFA_OP_AND_OFF(CHAR_AND)) // 0xF8000007       &&      逻辑与
#define CIFA_OP_LOR         (CIFA_OP_BASE+CIFA_OP_AND_OFF(CHAR_BOR)) // 0xF8000008       ||      逻辑或
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
#define CIFA_OP_NE                  (CIFA_OP_BASE+CHAR_NOT)          // 0xF8000021       !=      不等于
#define CIFA_OP_MOD_ASSIGN          (CIFA_OP_BASE+CHAR_MOD)          // 0xF8000025       %=      取模并赋值
#define CIFA_OP_AND_ASSIGN          (CIFA_OP_BASE+CHAR_AND)          // 0xF8000026       &=      位与并赋值
#define CIFA_OP_MUL_ASSIGN          (CIFA_OP_BASE+CHAR_MUL)          // 0xF800002A       *=      乘法并赋值
#define CIFA_OP_ADD_ASSIGN          (CIFA_OP_BASE+CHAR_ADD)          // 0xF800002B       +=      加法并赋值
#define CIFA_OP_SUB_ASSIGN          (CIFA_OP_BASE+CHAR_SUB)          // 0xF800002D       -=      减法并赋值
#define CIFA_OP_DIV_ASSIGN          (CIFA_OP_BASE+CHAR_DIV)          // 0xF800002F       /=      除法并赋值
#define CIFA_OP_LE                  (CIFA_OP_BASE+CHAR_LT)           // 0xF800003C       <=      小于等于
#define CIFA_OP_EQ                  (CIFA_OP_BASE+CHAR_ASSIGN)       // 0xF800003D       ==      等于
#define CIFA_OP_GE                  (CIFA_OP_BASE+CHAR_GT)           // 0xF800003E       >=      大于等于
#define CIFA_OP_INI_ASSIGN          (CIFA_OP_BASE+CHAR_COLON)        // 0xF800003A       :=      初始化赋值
#define CIFA_OP_XOR_ASSIGN          (CIFA_OP_BASE+CHAR_XOR)          // 0xF800005E       ^=      异或并赋值
#define CIFA_OP_BOR_ASSIGN          (CIFA_OP_BASE+CHAR_BOR)          // 0xF800007C       |=      位或并赋值
#define CIFA_OP_LAST                                                       0xF8000080

// 注释、字面量
#define CIFA_CF_START       0xF8000080
#define CIFA_CF_COMMENT     0xF8000081
#define CIFA_CF_BLOCK_CMMT  0xF8000082
#define CIFA_CF_STR_LIT     0xF8000083
#define CIFA_CF_RSTR_LIT    0xF8000084
#define CIFA_CF_RUNE_LIT    0xF8000085
#define CIFA_CF_NULL_LIT    0xF8000086
#define CIFA_CF_BOOL_LIT    0xF8000087
#define CIFA_CF_INT_LIT     0xF8000088
#define CIFA_CF_FLOAT_LIT   0xF8000089
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

#if __ARCH_64BIT__
#define SIZE_OF_POINTER 8
#define ALIGNOF_POINTER 3 // (1 << 3) = 8
#else
#define SIZE_OF_POINTER 4
#define ALIGNOF_POINTER 2 // (1 << 2) = 4
#endif

#define BASIC_MAX_ALIGN 3 // (1 << 3) = 8

// 标识符可以命名：
// 1. 包名
// 2. 标签
// 3. 类型和类型别名
// 4. 类型约束
// 5. 类型参数
// 6. 函数和方法
// 7. 变量名
// 8. 常量

#define SYM_TYPE_BASIC      1
#define SYM_TYPE_STRUCT     2
#define SYM_TYPE_INTERFACE  3
#define SYM_TYPE_FUNCTION   4
#define SYM_TYPE_COMPOSITE  6

typedef uint32 cfid_t;

typedef struct {
    cfid_t id; // 0表示匿名符号
    uint32 scope; // 符号所属所用域
    uint32 type_kind: 4;
    uint32 is_package: 1;
    uint32 is_label: 1;
    uint32 is_field: 1;
    uint32 bitfield: 1;
    uint32 is_param: 1;
    uint32 localvar: 1;
    uint32 is_const: 1;
    uint32 is_func: 1; // 无函数体的是类型，有函数体的不是类型，而是函数类型的一个实例
    uint32 is_method: 1;
    uint32 dyn: 1; // 类型大小不固定
    uint32 align: 4; // (1 << align)
    uint8 bf_off;
    uint8 bf_size;
    Uint off;
    Uint size;
} sym_t;

typedef struct {
    cfid_t id;
    string_t s;
    union {
        stack_t named_type; // 符号作为命名类型使用
        stack_t field_sym;  // 符号当作成员或参数或局部变量使用
        stack_t func_sym;   // 一个符号可以有以自己为名称的函数
        stack_t method;     // 一个符号可以有自己的多个方法
        stack_t ident_sym;
    } u;
} ident_t;

typedef union {
    uint64 i;
    float64 f;
    float32 f32;
    ident_t *sym;
    byte jmp[2];
    uint16 cmp[2];
    string_t sval;
} cfval_t;

typedef struct {
    uint32 attr;    // 值的属性标志
    uint16 reg;     // 寄存器以及处理器状态
    uint16 reg2;    // 第二个寄存器
    sym_t *type;    // 变量或常量涉及的类型
    sym_t *sym;     // 值对应的变量标识符符号
    cfval_t c;      // 常量的值
} yfvar_t;

#define CIFA_DEC_LIT 1
#define CIFA_BIN_LIT 2
#define CIFA_HEX_LIT 3

typedef struct {
    cfval_t val;
    string_t s;
    cfid_t cfid;
    uint8 istype: 1; // 类型标识符
    uint8 iscons: 1; // 常量标识符
    uint8 isiden: 1; // 不是类型和常量的标识符
    uint8 ispred: 1; // 语言预声明标识符
    uint8 base;      // 2/10/16
    Uint line;
    Uint cols;
} cifa_t;

typedef struct {
    sym_t sym;
    sym_t *type; // 未声明类型的引用，或基本类型变长参数，对应的type都是null
    ident_t *type_ident;
    uint32 tpref: 1;
    uint32 embed: 1;
    uint32 alias: 1; // 成员别名
    uint32 param: 1; // 函数参数或返回值
    uint32 vaarg: 1; // 存在一个变长参数
    uint32 rparm: 1;
    uint32 locvar: 1;
} field_t;

typedef struct {
    sym_t sym;
    sym_t *type;
    cfval_t val;
} const_t;

typedef struct {
    sym_t sym;
    stack_t field;
} struct_t;

typedef struct {
    sym_t sym;
    sym_t *receiver;
    ident_t *func_name;
    uint32 param_align: 4;
    uint32 rparm_align: 4;
    uint32 local_align: 4;
    uint32 func_attr: 4;
    uint32 have_body: 1;
    Uint param_size;
    Uint rparm_size;
    Uint local_size;
    struct stack_it *param;
    struct stack_it *rparm;
    struct stack_it *local;
    stack_t field;
} func_t;

typedef struct {
    struct stack_it *type_struct; // 解析创建的结构体、接口、函数类型
    ident_t *type_ident; // 或者是一个类型标识符
    sym_t *t; // 类型的语法定义
    uint32 tpref: 1;
} tpcur_t;

typedef struct {
    byte c;
    byte d;
    uint16 n;
} esc_t;

typedef struct {
    int32 prior;
    cfid_t cfid;
    uint32 inst;
} cfop_t;

typedef struct {
    uint32 len;
    byte op[4];
    cfop_t val;
} ops_t;

typedef struct {
    file_t *f;
    byte *b128;
    esc_t *esc;
    ops_t *ops;
    buffer_t s;
    cifa_t cf;
    rune c;
    bool unicode;
    bool copied;
    byte *start;
    Uint line;  // 当前词法前缀所在行
    Uint cols;  // 当前词法前缀所在字符列
    uint32 user_id_start;
    bhash2_t hash_ident;
    array2_ex_t arry_ident;
    uint32 scope;
    uint32 anon_id;
    struct stack_it *global; // 全局符号栈顶
    stack_t symbol; // 当前语法解析时的符号栈
    yfvar_t *vtop;
    yfvar_t *vstk;
    Error chcc_error;
    Error error;
    Error yferr;
} chcc_t;

void chcc_init(chcc_t *cc, file_t *f);
void chcc_free(chcc_t *cc);
void curr(chcc_t *cc);
ident_t *ident_get(chcc_t *cc, cfid_t id);

#define ERROR_CMMT_NOT_CLOSED           0xE00
#define ERROR_INVALID_ESCCHAR           0xE01
#define ERROR_INVALID_HEXCHAR           0xE02
#define ERROR_INVALID_HEXNUMB           0xE03
#define ERROR_INVALID_UNICODE           0xE04
#define ERROR_MISS_CLOSE_QUOTE          0xE05
#define ERROR_EMPTY_RUNE_LIT            0xE06
#define ERROR_MULT_CHAR_EXIST           0xE07
#define ERROR_IS_NOT_IDENT              0xE10
#define ERROR_INVALID_TYPE_NAME         0xE11
#define ERROR_CANT_DEF_SAME_TYPE        0xE12
#define ERROR_NAME_RESERVED             0xE13
#define ERROR_NO_OPEN_CURLY             0xE14
#define ERROR_NO_CLOSE_CURLY            0xE15
#define ERROR_TYPE_REDEFINED            0xE16
#define ERROR_FIELD_REDEFINED           0xE17
#define ERROR_INVALID_FIELD_TYPE        0xE18
#define ERROR_INVALID_FIELD_NAME        0xE19
#define ERROR_INVALID_BITFIELD_NAME     0xE1A
#define ERROR_EMBED_TYPE_INVALID        0xE1B
#define ERROR_TYPE_NOT_DEFINED          0xE1C
#define ERROR_INVALID_TYPE_LIT          0xE1D
#define ERROR_TYPE_LIT_NOT_SUPPORT      0xE1E
#define ERROR_DYN_TYPE_FIELD_POS        0xE1F
#define ERROR_INVALID_BITFIELD_TYPE     0xE20
#define ERROR_INVALID_BITFIELD_SIZE     0xE21
#define ERROR_BITFIELD_SIZE_NOT_DEC     0xE22
#define ERROR_MISSING_ALIAS_FIELD       0xE23
#define ERROR_INVALID_RECEIVER_TYPE     0xE24
#define ERROR_RECEIVER_TYPE_UNFOUND     0xE25
#define ERROR_MISSING_OPEN_PAREN        0xE26
#define ERROR_INVALID_PARAM_TYPE        0xE27
#define ERROR_INVALID_PARAM_NAME        0xE28
#define ERROR_MISS_BASIC_VAARG_3DOT     0xE29
#define ERROR_VAARG_SHALL_LAST_PARAM    0xE2A
#define ERROR_DYN_TYPE_CANT_BE_PARAM    0xE2B
#define ERROR_TOO_MANY_ANON_SYM         0xE2C
#define ERROR_TYPE_CANT_CONTAIN_BODY    0xE2D
#define ERROR_GLOBAL_FUNC_MISS_NAME     0xE2E
#define ERROR_FUNC_DECL_NOT_MATCH       0xE2F
#define ERROR_FUNC_MISS_PARAM_NAME      0xE30
#define ERROR_FUNC_IS_REDEFINED         0xE31

#endif /* CHAPL_LANG_CHCC_H */
