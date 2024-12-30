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
#define CHAR_CLASS_IDENT        0x03    // A ~ Z a ~ z _
#define CHAR_CLASS_DOT          0x04    // . .. ... .1
#define CHAR_CLASS_SIGN         0x05    // + -
#define CHAR_CLASS_LETTER       0x06    // A ~ Z a ~ z
#define CHAR_CLASS_UPPER        0x07    // A ~ Z
#define CHAR_CLASS_LOWER        0x08    // a ~ z
#define CHAR_CLASS_UNDERSCORE   0x09    // _
#define CHAR_CLASS_QUOTE        0x0a    // ' ''' " `
#define CHAR_CLASS_SINGLE       0x0b    // # $ @ ( ) [ ] { } , ;
#define CHAR_CLASS_QMARK        0x0e    // ? ?: ?>
#define CHAR_CLASS_TILDE        0x0f    // ~ ~= ~>
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
    /*20*/ /*20*/0x01, /*!!*/0x3b, /*""*/0x0a, /*##*/0x0b, /*$$*/0x0b, /*%%*/0x37, /*&&*/0x34, /*''*/0x0a,  \
    /*20*/ /*((*/0x0b, /*))*/0x0b, /****/0x30, /*++*/0x05, /*,,*/0x0b, /*--*/0x05, /*..*/0x04, /*//*/0x26,  \
    /*30*/ /*00*/0x02, /*11*/0x02, /*22*/0x02, /*33*/0x02, /*44*/0x02, /*55*/0x02, /*66*/0x02, /*77*/0x02,  \
    /*30*/ /*88*/0x02, /*99*/0x02, /*::*/0x25, /*;;*/0x0b, /*<<*/0x1f, /*==*/0x1d, /*>>*/0x19, /*??*/0x0e,  \
    /*40*/ /*@@*/0x0b, /*AA*/0x03, /*BB*/0x03, /*CC*/0x03, /*DD*/0x03, /*EE*/0x03, /*FF*/0x03, /*GG*/0x03,  \
    /*40*/ /*HH*/0x03, /*II*/0x03, /*JJ*/0x03, /*KK*/0x03, /*LL*/0x03, /*MM*/0x03, /*NN*/0x03, /*OO*/0x03,  \
    /*50*/ /*PP*/0x03, /*QQ*/0x03, /*RR*/0x03, /*SS*/0x03, /*TT*/0x03, /*UU*/0x03, /*VV*/0x03, /*WW*/0x03,  \
    /*50*/ /*XX*/0x03, /*YY*/0x03, /*ZZ*/0x03, /*[[*/0x0b, /*\\*/0x15, /*]]*/0x0b, /*^^*/0x13, /*__*/0x03,  \
    /*60*/ /*``*/0x0a, /*aa*/0x03, /*bb*/0x03, /*cc*/0x03, /*dd*/0x03, /*ee*/0x03, /*ff*/0x03, /*gg*/0x03,  \
    /*60*/ /*hh*/0x03, /*ii*/0x03, /*jj*/0x03, /*kk*/0x03, /*ll*/0x03, /*mm*/0x03, /*nn*/0x03, /*oo*/0x03,  \
    /*70*/ /*pp*/0x03, /*qq*/0x03, /*rr*/0x03, /*ss*/0x03, /*tt*/0x03, /*uu*/0x03, /*vv*/0x03, /*ww*/0x03,  \
    /*70*/ /*xx*/0x03, /*yy*/0x03, /*zz*/0x03, /*{{*/0x0b, /*||*/0x10, /*}}*/0x0b, /*~~*/0x0f, /*7F*/0x01

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
// 标识符
#define CIFA_IDENT_START    0x100
enum chcc_predecl_ident {
    CIFA_TYPE_IDENT = 0xff,
#define PREDECL(id, ...) id,
#include "chcc/decl.h"
};
// 匿名标识符
#define CIFA_ANON_IDENT 0x80000000
#define CIFA_ANON_LAST  0xf0000000

#if __ARCH_64BIT__
#define SIZE_OF_POINTER 8
#define ALIGNOF_POINTER ALIGNOF_64BITS_TYPE
#else
#define SIZE_OF_POINTER 4
#define ALIGNOF_POINTER ALIGNOF_32BITS_TYPE
#endif

#define BASIC_MAX_ALIGN ALIGNOF_64BITS_TYPE
#define UNFORCED_CONST_FLOAT 0xfffffffe
#define UNFORCED_CONST_INT 0xffffffff

#define ALIGNOF_TYPE_8          1
#define ALIGNOF_TYPE_16         2
#define ALIGNOF_TYPE_32         4
#define ALIGNOF_TYPE_64         8
#define ALIGNOF_TYPE_128        16
#define ALIGNOF_TYPE_256        32
#define ALIGNOF_TYPE_512        64

#define ALIGNOF_LOG2_8          0   // 2 ^ 0 = 1    (1 << align)
#define ALIGNOF_LOG2_16         1   // 2 ^ 1 = 2
#define ALIGNOF_LOG2_32         2   // 2 ^ 2 = 4
#define ALIGNOF_LOG2_64         3   // 2 ^ 3 = 8
#define ALIGNOF_LOG2_128        4   // 2 ^ 4 = 16
#define ALIGNOF_LOG2_256        5   // 2 ^ 5 = 32
#define ALIGNOF_LOG2_512        6   // 2 ^ 6 = 64

#define ALIGNOF_POW2_SUB1_8     0
#define ALIGNOF_POW2_SUB1_16    1
#define ALIGNOF_POW2_SUB1_32    3
#define ALIGNOF_POW2_SUB1_64    7
#define ALIGNOF_POW2_SUB1_128   15
#define ALIGNOF_POW2_SUB1_256   31
#define ALIGNOF_POW2_SUB1_512   63

#define ALIGNOF_8BITS_TYPE      ALIGNOF_POW2_SUB1_8
#define ALIGNOF_16BITS_TYPE     ALIGNOF_POW2_SUB1_16
#define ALIGNOF_32BITS_TYPE     ALIGNOF_POW2_SUB1_32
#define ALIGNOF_64BITS_TYPE     ALIGNOF_POW2_SUB1_64
#define ALIGNOF_128BITS_TYPE    ALIGNOF_POW2_SUB1_128
#define ALIGNOF_256BITS_TYPE    ALIGNOF_POW2_SUB1_256
#define ALIGNOF_512BITS_TYPE    ALIGNOF_POW2_SUB1_512

// 标识符可以命名：
// 1. 包名
// 2. 标签
// 3. 类型和类型别名
// 4. 类型约束
// 5. 类型参数
// 6. 函数和方法
// 7. 变量名
// 8. 常量

typedef uint32 cfid_t;

typedef struct {
    cfid_t cfid; // 可能匿名也可能命名
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
    uint32 off;
    uint32 size;
} sym_t;

struct symb_t;
struct vsym_t;

typedef struct ident_t {
    cfid_t id;
    string_t s;
    bool haspkgprefix;
    struct ident_t *pknm;   // 这个标识符是包名或包的别名，这里指向真实的包名称，如果为空则表示不是包名
    struct symb_t *defsym;
    struct symb_t *glosym;
} ident_t;

typedef union {
    uint32 c;
    uint64 i64;
    float64 f;
    float32 f32;
    string_t str;
    byte jmp[2];
    uint16 cmp[2];
} cstval_t;

typedef struct symb_t {
    ident_t *name;      // 命名符号名称，如果为空则为匿名类型
    ident_t *real;      // 符号实际的名称，当前的符号name只是一个别名，只能给全局名称创建别名
    cfid_t cfid;        // 符号真实名称id，或匿名id
    uint32 size;        // 类型或变量的大小
    uint32 align: 8;    // 类型或变量的对齐
    uint32 szdyn: 1;    // 大小不是固定大小，运行时才确定
    uint32 istype: 1;   // 该符号是一个类型
    uint32 isstype: 1;  // 是一个结构体类型
    uint32 isitype: 1;  // 是一个接口类型
    uint32 isftype: 1;  // 是一个函数类型，底层表示是一个指针或者函数对象
    uint32 isbtype: 1;  // 是一个基本类型
    uint32 btype_i: 1;  // 整数基本类型
    uint32 btype_f: 1;  // 浮点基本类型
    uint32 btype_s: 1;  // 字符串基本类型
    uint32 isconst: 1;  // 该符号是一个常量
    uint32 isvar: 1;    // 该符号是一个变量
    uint32 isfvar: 1;   // 是一个可调用变量
    uint32 ptrvar: 1;   // 是一个可解引用变量
    uint32 body: 1;
} symb_t;

typedef struct vsym_t {
    symb_t symb;
    intv_t addr;    // 变量地址（包括函数地址、标签地址）
    intv_t *usel;   // 使用变量的地址列表，所有使用的地方都需要写入变量的地址
    symb_t *refs;   // 涉及的类型符号，即变量的类型
    cstval_t val;   // 该符号表示一个常量
} vsym_t; // 变量符号

typedef struct {
    vsym_t v;    // 函数其实是对应函数类型的一个对象
    ident_t *dest; // 赋值目标变量名称
    ident_t *recv;
    slist_t para;   // 包含vsym_t
    slist_t retp;   // 包含vsym_t
    uint32 plen;
    uint32 rlen;
    uint32 clen;
    uint32 loc;
    intv_t *radr;
} fsym_t;

typedef struct {
    uintv_t object;
    uintv_t method;
} fobj_t;

// 1. 操作符和标点
//      cfid < CIFA_OPER_PUNCT（0xc0）
//      oper > 0 操作符，表示优先级
//      oper = 0 标点
//      optr     指向操作符信息
// 2. 字面量
//      islit: 1
//      isint: 1    val.i   整数常量    numbase 基数    cf->s   临时后缀字符串      cfid = CIFA_ID_INT/INT64/BOOL/BYTE/RUNE/ERROR/NULL_TYPE
//      isfloat: 1  val.f   浮点常量    numbase 基数    cf->s   临时后缀字符串      cfid = CIFA_ID_FLOAT
//      布尔常量    val.c   cfid = CIFA_ID_BOOL
//      空值常量    val.c   cfid = CIFA_ID_NULL_TYPE
//      字符常量    val.c   cfid = CIFA_ID_RUNE
//      错误代码    val.c   cfid = CIFA_ID_ERROR
//      字符串  islit: 1    cfid = CIFA_ID_STRING       cf->s   临时字符串值
// 3. 注释
//      cfid = CIFA_PT_LINE_CMMT 或 CIFA_PT_BLOCK_CMMT
//      iscmm: 1
//      cf->s   临时注释字符串
// 4. 标识符
//      cfid = 创建之前 CIFA_TYPE_IDENT，创建之后 >= CIFA_IDENT_START
//      cf->s 临时标识符名称
//      val.c 标识符字符串的哈希值
//      ident.haspknm: 1 包名引用的符号，pknm_len 包名的长度
//      ident.keyword: 1 语言关键字
//      ident.predecl: 1 语言预声明名称
//      ident.istype: 1 类型名
//      ident.isconst: 1 常量名
//      ident.isvar: 1 变量名
// 5. 其中语言预定义以及非地址标识符范围
//      cfid [0x100, 0x200)
// 6. 地址标识符范围
//      cfid [0x200, ...)
typedef struct {
    cstval_t val;
    string_t s;
    ident_t *ident;
    cfid_t cfid;
    uint32 pkhash;
    uint32 pknm_len;
    uint16 attr;
    byte numbase;
    // 操作符和标点（cfid < CIFA_OPER_PUNCT）
    byte oper;
    ops_t *optr;
    // 注释（iscmm 不为 0，cfid = CIFA_PT_LINE_CMMT/CIFA_PT_BLOCK_CMMT）
    uint16 iscmm: 1;
    // 字面量（islit 不为 0）
    uint16 islit: 1;
    uint16 isint: 1;
    uint16 isfloat: 1;
    uint16 isstr: 1;
    // 语言预声明名称（predecl 不为 0，ident 不为空）
    uint16 predecl: 1;  // 语言预声明名称
    uint16 keyword: 1;  // 语言关键字
    // 标识符（ident 不为空）
    uint16 isattr: 1;   // 属性名称
    uint16 haspknm: 1;  // 标识符有包名前缀
    uint16 istype: 1;   // 类型名
    uint16 deftype: 1;  // 可用于定义新类型的类型名
    uint16 reftype: 1;  // 引用已定义类型的类型名
    uint16 isconst: 1;  // 常量名
    uint16 defconst: 1; // 可用于定义新常量的常量名
    uint16 refconst: 1; // 引用已定义常量的常量名
    uint16 isvar: 1;    // 变量名
    uint16 defvar: 1;   // 可用于定义新变量的变量名
    uint16 refvar: 1;   // 引用已定义变量的变量名
    Error error;
    Uint line;
    Uint cols;
} cifa_t;

typedef struct {
    sym_t sym;
    sym_t *type; // 未声明类型的引用，或基本类型变长参数，对应的type都是null
    ident_t *tpid;
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
    ident_t *tpid; // 或者是一个类型标识符
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

typedef struct {
    uint32 local;
    stack_t symb; // 保存本作用域定义的符号
} scope_t;

typedef struct {
    vsym_t v;
} synval_t;

typedef struct {
    file_t *f;
    byte *b128;
    esc_t *esc;
    ops_t *ops;
    buffer_t s;
    cifa_t cf;
    rune c;
    bool unicode;
    bool haserr;
    byte *start;
    Uint line;  // 当前词法前缀所在行
    Uint cols;  // 当前词法前缀所在字符列
    uint32 user_id_start;
    bhash2_t hash_ident;
    array2_ex_t arry_ident;
    byte *text_section;
    byte *text; // 代码段
    byte *data_section;
    byte *data; // 数据段
    byte *rodata_section;
    byte *rodata; // 只读数据段
    ident_t *pknm; // 当前代码包名称
    uint32 anon_id;
    stack_t vstack;
    vsym_t *vtop;
    scope_t *gsym; // 全局符号
    stack_t *sstk; // 当前作用域符号栈
    uint32 local;
    stack_t scope;
    bool expose_pretype;
    bool expose_prenull;
    bool expose_prebool;
} chcc_t;

void chcc_init(chcc_t *cc, file_t *f);
void chcc_free(chcc_t *cc);
void chcc_start(chcc_t *cc);
void rch(chcc_t *cc);
void next(chcc_t *cc);
symb_t *getscopesym(ident_t *ident);
symb_t *findscopesym(chcc_t *cc, cfid_t cfid);
ident_t *getrealident(ident_t *ident);

enum {
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
    ERROR_INVALID_POINTER_TYPE,
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
    ERROR_IDENT_NOT_PKGNAME,
    ERROR_PKG_REF_WITHOUT_IDENT,
    ERROR_MISSING_EXPORT_SYM,
    ERROR_PACKAGE_NOT_FOUND,
    ERROR_HASH_IDENT_PUSH_FAILED,
    ERROR_ARRAY_IDENT_PUSH_FAILED,
    ERROR_SKIP_MATCH_FAILED,
    ERROR_FUNC_INVALID_PARAM,
    ERROR_FUNC_INVALID_RPARA,
    ERROR_INVALID_ATTR_NAME,
    ERROR_GLOBAL_FUNC_NONAME,
    ERROR_VAR_WITHOUT_TYPE,
    ERROR_SYMB_DUP_DEFINED,
    ERROR_VSTACK_OVERFLOW,
    ERROR_UNKNOWN_OPERATOR,
};

#endif /* CHAPL_LANG_CHCC_H */
