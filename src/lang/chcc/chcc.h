#ifndef CHAPL_LANG_CHCC_H
#define CHAPL_LANG_CHCC_H
#include "builtin/decl.h"
#include "direct/file.h"

#define __CHCC_DEBUG__ 1

// 词法元素包括：
// 1. 操作符（operator）
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
#define CHAR_CLASS_OPERATOR     0x10    // 操作符和标点
#define CHAR_CLASS_VERTBAR      0x10    // | ||
#define CHAR_CLASS_CARET        0x13    // ^
#define CHAR_CLASS_BSLASH       0x15    // \ \\ \= \\=
#define CHAR_CLASS_GT           0x19    // >
#define CHAR_CLASS_EQUAL        0x1d    // =
#define CHAR_CLASS_LT           0x1f    // <
#define CHAR_CLASS_COLON        0x25    // :
#define CHAR_CLASS_SLASH        0x26    // /
#define CHAR_CLASS_MDOT         0x28    // .
#define CHAR_CLASS_MINUS        0x29    // -
#define CHAR_CLASS_PLUS         0x2e    // +
#define CHAR_CLASS_ASTER        0x30    // *
#define CHAR_CLASS_AND          0x34    // &
#define CHAR_CLASS_PERCENT      0x37    // %
#define CHAR_CLASS_EMARK        0x3b    // !

#define CHAR_128_BYTE_ARRAY_G \
           /* 0           1           2           3           4           5           6           7 */      \
           /* 8           9           A           B           C           D           E           F */      \
    /*00*/ /*00*/0x01, /*01*/0x01, /*02*/0x01, /*03*/0x01, /*04*/0x01, /*05*/0x01, /*06*/0x01, /*\a*/0x01,  \
    /*08*/ /*\b*/0x01, /*\t*/0x01, /*\n*/0x01, /*\v*/0x01, /*\f*/0x01, /*\r*/0x01, /*0E*/0x01, /*0F*/0x01,  \
    /*10*/ /*10*/0x01, /*11*/0x01, /*12*/0x01, /*13*/0x01, /*14*/0x01, /*15*/0x01, /*16*/0x01, /*17*/0x01,  \
    /*18*/ /*18*/0x01, /*19*/0x01, /*1A*/0x01, /*1B*/0x01, /*1C*/0x01, /*1D*/0x01, /*1E*/0x01, /*1F*/0x01,  \
    /*20*/ /*20*/0x01, /*!!*/0x3b, /*""*/0x08, /*##*/0x09, /*$$*/0x09, /*%%*/0x37, /*&&*/0x34, /*''*/0x08,  \
    /*20*/ /*((*/0x09, /*))*/0x09, /****/0x30, /*++*/0x2e, /*,,*/0x09, /*--*/0x29, /*..*/0x06, /*//*/0x26,  \
    /*30*/ /*00*/0x02, /*11*/0x02, /*22*/0x02, /*33*/0x02, /*44*/0x02, /*55*/0x02, /*66*/0x02, /*77*/0x02,  \
    /*30*/ /*88*/0x02, /*99*/0x02, /*::*/0x25, /*;;*/0x09, /*<<*/0x1f, /*==*/0x1d, /*>>*/0x19, /*??*/0x0E,  \
    /*40*/ /*@@*/0x09, /*AA*/0x03, /*BB*/0x03, /*CC*/0x03, /*DD*/0x03, /*EE*/0x03, /*FF*/0x03, /*GG*/0x03,  \
    /*40*/ /*HH*/0x03, /*II*/0x03, /*JJ*/0x03, /*KK*/0x03, /*LL*/0x03, /*MM*/0x03, /*NN*/0x03, /*OO*/0x03,  \
    /*50*/ /*PP*/0x03, /*QQ*/0x03, /*RR*/0x03, /*SS*/0x03, /*TT*/0x03, /*UU*/0x03, /*VV*/0x03, /*WW*/0x03,  \
    /*50*/ /*XX*/0x03, /*YY*/0x03, /*ZZ*/0x03, /*[[*/0x09, /*\\*/0x15, /*]]*/0x09, /*^^*/0x13, /*__*/0x05,  \
    /*60*/ /*``*/0x08, /*aa*/0x04, /*bb*/0x04, /*cc*/0x04, /*dd*/0x04, /*ee*/0x04, /*ff*/0x04, /*gg*/0x04,  \
    /*60*/ /*hh*/0x04, /*ii*/0x04, /*jj*/0x04, /*kk*/0x04, /*ll*/0x04, /*mm*/0x04, /*nn*/0x04, /*oo*/0x04,  \
    /*70*/ /*pp*/0x04, /*qq*/0x04, /*rr*/0x04, /*ss*/0x04, /*tt*/0x04, /*uu*/0x04, /*vv*/0x04, /*ww*/0x04,  \
    /*70*/ /*xx*/0x04, /*yy*/0x04, /*zz*/0x04, /*{{*/0x09, /*||*/0x10, /*}}*/0x09, /*~~*/0x0F, /*7F*/0x01

// 优先级（从高到低）
// 9 从左向右结合：() [] . -> (type){list}
// 8 从右向左结合：+ - ! ^ * & (type) 一元操作
// 7 从左向右结合：* ** \ \\ % %% <-- --> & ^
// 6 从左向右结合：+ - |
// 5 从左向右结合：< << <= <<= > >> >= >>=
// 4 从左向右结合：== !=
// 3 从左向右结合：&&
// 2 从左向右结合：||
// 1 从右向左结合：= := += -= &= |= ^= *= **= \= \\= %= %%= <--= -->=
// 单字符操作符：每个操作符都对应实际的机器指令
#define CIFA_OP_PLUS        CHAR_PLUS
#define CIFA_OP_MINUS       CHAR_MINUS
#define CIFA_OP_NOT         CHAR_NOT
#define CIFA_OP_COMPL       CHAR_CARET
#define CIFA_OP_DREF        CHAR_ASTER
#define CIFA_OP_ADDR        CHAR_AMPERSAND
#define CIFA_OP_MUL         CHAR_ASTER
#define CIFA_OP_DIV         CHAR_BSLASH
#define CIFA_OP_MOD         CHAR_PERCENT
#define CIFA_OP_AND         CHAR_AMPERSAND
#define CIFA_OP_XOR         CHAR_XOR
#define CIFA_OP_ADD         CHAR_ADD
#define CIFA_OP_SUB         CHAR_SUB
#define CIFA_OP_BOR         CHAR_BOR
#define CIFA_OP_LT          CHAR_LT
#define CIFA_OP_GT          CHAR_GT
#define CIFA_OP_ASSIGN      CHAR_ASSIGN
// 多字符操作符：每个操作符都对应实际的机器指令
#define CIFA_OP_UMUL        0x81    // **   无符号乘法
#define CIFA_OP_UDIV        0x82    // \\   无符号除法
#define CIFA_OP_UMOD        0x83    // %%   无符号取模
#define CIFA_OP_LSH         0x84    // <--  左移
#define CIFA_OP_RSH         0x85    // -->  右移
#define CIFA_OP_ULT         0x86    // <<   无符号小于
#define CIFA_OP_UGT         0x87    // >>   无符号大于
#define CIFA_OP_LE          0x88    // <=   小于等于
#define CIFA_OP_GE          0x89    // >=   大于等于
#define CIFA_OP_ULE         0x8a    // <<=  无符号小于等于
#define CIFA_OP_UGE         0x8b    // >>=  无符号大于等于
#define CIFA_OP_EQ          0x8c    // ==   等于
#define CIFA_OP_NE          0x8d    // !=   不等于
#define CIFA_OP_LAND        0x8e    // &&   逻辑与
#define CIFA_OP_LOR         0x8f    // ||   逻辑或
#define CIFA_OP_INIT_ASSIGN 0x90    // :=   初始化赋值
#define CIFA_OP_ADD_ASSIGN  0x91    // +=   加法并赋值
#define CIFA_OP_SUB_ASSIGN  0x92    // -=   减法并赋值
#define CIFA_OP_AND_ASSIGN  0x93    // &=   位与并赋值
#define CIFA_OP_BOR_ASSIGN  0x94    // |=   位或并赋值
#define CIFA_OP_XOR_ASSIGN  0x95    // ^=   异或并赋值
#define CIFA_OP_MUL_ASSIGN  0x96    // *=   乘法并赋值
#define CIFA_OP_UMUL_ASSIGN 0x97    // **=  无符号乘法并赋值
#define CIFA_OP_DIV_ASSIGN  0x98    // \=   除法并赋值
#define CIFA_OP_UDIV_ASSIGN 0x99    // \\=  无符号除法并赋值
#define CIFA_OP_MOD_ASSIGN  0x9a    // %=   取模并赋值
#define CIFA_OP_UMOD_ASSIGN 0x9b    // %%=  无符号取模并赋值
#define CIFA_OP_LSH_ASSIGN  0x9c    // <--= 左移并赋值
#define CIFA_OP_RSH_ASSIGN  0x9d    // -->= 右移并赋值
// 多字符标点
#define CIFA_PT_ARROW       0xa1    // ->
#define CIFA_PT_LINE_CMMT   0xa2    // //
#define CIFA_PT_BLOCK_CMMT  0xa3    // /*
#define CIFA_PT_3DOT        0xa4    // ...
#define CIFA_OPER_PUNCT     0xc0
// 词法字面量
#define CIFA_TYPE_NUMERIC   0xc1
#define CIFA_TYPE_STRING    0xc2
#define CIFA_TYPE_COMMENT   0xc3
// 标识符
#define CIFA_IDENT_START    0x100
enum chcc_predecl_ident {
    CIFA_TYPE_IDENT = 0xff,
#define PREDECL(id, ...) id,
#include "chcc/decl.h"
};
// 匿名标识符
#define CIFA_ANON_IDENT 0x80000000

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
    uint64 c;
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

// 1. 操作符和标点
//      cfid < 0xc0
//      oper > 0 操作符，表示优先级
//      oper = 0 标点
//      val.c 对应机器指令
// 2. 数值字面量
//      cfid = 0xc0
//      isbool: 1   val.c   布尔常量
//      isnull: 1   val.c   空值常量
//      ischar: 1   val.c   字符常量
//      iserr: 1    val.c   错误代码
//      isint: 1    val.i   整数常量    base    基数    cf->s   临时后缀字符串
//      isfloat:    val.f   浮点常量    base    基数    cf->s   临时后缀字符串
// 3. 字符串字面量
//      cfid = 0xc1
//      cf->s   临时字符串值
// 4. 注释
//      cfid = 0xc2
//      bcmmt: 1 块注释
//      bcmmt: 0 行注释
//      cf->s   临时注释字符串
// 3. 标识符
//      cfid > 0xff
//      cfid 标识符数组的索引
//      val.sym 指向标识符符号
//      pdid: 1 语言预定义标识符
//      tpid: 1 类型标识符
//      csid: 1 常量标识符
//      ident: 1 非类型非常量标识符
// 4. 其中语言预定义以及非地址标识符范围
//      cfid [0x100, 0x200)
// 5. 地址标识符范围
//      cfid [0x200, ...)
typedef struct {
    cfval_t val;
    string_t s;
    cfid_t cfid;
    uint8 oper;
    uint8 base;
    uint16 pdid: 1;
    uint16 tpid: 1;
    uint16 csid: 1;
    uint16 ident: 1;
    uint16 isbool: 1;
    uint16 isnull: 1;
    uint16 ischar: 1;
    uint16 iserr: 1;
    uint16 isint: 1;
    uint16 isfloat: 1;
    uint16 bcmmt: 1;
    Error error;
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
    byte prior;
    byte cfid;
    byte len;
    byte op[4];
    uint32 inst;
} ops_t;

#define CHCC_MAX_ERRORS 8

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
    Error *error;
    Error errst[CHCC_MAX_ERRORS];
    Error yferr;
} chcc_t;

void chcc_init(chcc_t *cc, file_t *f);
void chcc_free(chcc_t *cc);
void curr(chcc_t *cc);
ident_t *ident_get(chcc_t *cc, cfid_t id);

typedef enum {
    ERROR_CMMT_NOT_CLOSED = 0xE00,
    ERROR_INVALID_ESCCHAR,
    ERROR_INVALID_HEXCHAR,
    ERROR_INVALID_HEXNUMB,
    ERROR_INVALID_UNICODE,
    ERROR_INVALID_UTF8_BYTE,
    ERROR_INVALID_TYPE_NAME,
    ERROR_INVALID_FIELD_TYPE,
    ERROR_INVALID_FIELD_NAME,
    ERROR_INVALID_BITFIELD_NAME,
    ERROR_INVALID_TYPE_LIT,
    ERROR_INVALID_BITFIELD_TYPE,
    ERROR_INVALID_BITFIELD_SIZE,
    ERROR_INVALID_RECEIVER_TYPE,
    ERROR_INVALID_PARAM_TYPE,
    ERROR_INVALID_PARAM_NAME,
    ERROR_MISS_CLOSE_QUOTE,
    ERROR_EMPTY_RUNE_LIT,
    ERROR_MULT_CHAR_EXIST,
    ERROR_IS_NOT_IDENT,
    ERROR_CANT_DEF_SAME_TYPE,
    ERROR_NAME_RESERVED,
    ERROR_NO_OPEN_CURLY,
    ERROR_NO_CLOSE_CURLY,
    ERROR_TYPE_REDEFINED,
    ERROR_FIELD_REDEFINED,
    ERROR_EMBED_TYPE_INVALID,
    ERROR_TYPE_NOT_DEFINED,
    ERROR_TYPE_LIT_NOT_SUPPORT,
    ERROR_DYN_TYPE_FIELD_POS,
    ERROR_BITFIELD_SIZE_NOT_DEC,
    ERROR_MISSING_ALIAS_FIELD,
    ERROR_RECEIVER_TYPE_UNFOUND,
    ERROR_MISSING_OPEN_PAREN,
    ERROR_MISS_BASIC_VAARG_3DOT,
    ERROR_VAARG_SHALL_LAST_PARAM,
    ERROR_DYN_TYPE_CANT_BE_PARAM,
    ERROR_TOO_MANY_ANON_SYM,
    ERROR_TYPE_CANT_CONTAIN_BODY,
    ERROR_GLOBAL_FUNC_MISS_NAME,
    ERROR_FUNC_DECL_NOT_MATCH,
    ERROR_FUNC_MISS_PARAM_NAME,
    ERROR_FUNC_IS_REDEFINED,
};

#endif /* CHAPL_LANG_CHCC_H */
