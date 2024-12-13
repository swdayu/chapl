#define __CURR_FILE__ STRID_CHCC_CIFA
#include "internal/decl.h"
#include "chcc/chcc.h"

#define IDENT_HASH_INIT 1
#define IDENT_HASH_SIZE (8*1024) // 必须是2的幂
#define IDENT_ARRAY_EXPAND 512
#define CFSTR_ALLOC_EXPAND 128

uint32 ident_hash(uint32 h, rune c)
{
    return ((h) + ((h) << 5) + ((h) >> 27) + (c));
}

void ident_free(void *object)
{
    ident_t *sym = (ident_t *)object;
    string_free(&sym->s);
}

bool ident_eq(const void *object, const void *cmp_para)
{
    ident_t *sym = (ident_t *)object;
    string_t *str = (string_t *)cmp_para;
    return (sym->s.len == str->len) && (memcmp(sym->s.a, str->a, str->len) == 0);
}

ident_t *ident_create(chcc_t *cc, uint32 hash, bool calc, string_t ident)
{
    array_ex_t *arry_ident = cc->arry_ident.a;
    bhash_t *hash_ident = cc->hash_ident.a;
    ident_t *sym;
    Uint i = 0;
    bool exist = false;

    if (calc) {
        hash = IDENT_HASH_INIT;
        for (; i < ident.len; i++) {
            hash = ident_hash(hash, ident.a[i]);
        }
    }

    // 将标识符符号创建到符号哈希表中
    sym = (ident_t *)bhash_push(hash_ident, hash, ident_eq, &ident, sizeof(ident_t) + ident.len, &exist);
    if (!exist) {
        // 该符号在哈希表中不存在，返回的是新创建的符号，这里初始化这个符号
        memcpy((byte *)(sym + 1), ident.a, ident.len);
        string_init(&sym->s, (byte *)(sym + 1), ident.len, false);
        // 将符号添加到符号数组中，并初始化该符号在数组中的序号
        sym->id = (CIFA_IDENT_START) + array_ex_len(arry_ident);
        array_ex_push(&cc->arry_ident, (byte *)sym, IDENT_ARRAY_EXPAND);
    }

#if __CHCC_DEBUG__
    printf("ident %08x exist %d id %08x [%d] %s\n", hash, exist, sym->id, sym->s.len, sym->s.a);
#endif

    return sym;
}

ident_t *ident_get(chcc_t *st, cfid_t id)
{
    if (id < CIFA_IDENT_START) {
        return 0;
    }
    return (ident_t *)array_ex_at_n(st->arry_ident.a, id-CIFA_IDENT_START, sizeof(ident_t*));
}

bool type_ident(const byte *s, const byte *e)
{
    Int len = e - s;
    byte c = *s++;
    bool s = (c == '_'); // 首字母下划线
    bool u = (c >= 'A' && c <= 'Z'); // 首字母大写
    bool t = ((*(e-2) == '_') && (*(e-1) == 't')); // 以_t结尾
    bool l = false; // 第二个字符开始包含小写或包含数字
    while (s < e) {
        c = *s++;
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            l = true;
            break;
        }
    }
    return (u && (len == 1 || l)) || (len > 2 && !s && t);
}

bool cons_ident(const byte *s, const byte *e)
{
    byte c = *s;
    int n = 0; // 大写个数
    bool u = true; // 全部大写（不包含小写字符）
    bool s = (c == '_'); // 首字母下划线
    while (s < e) {
        c = *s++;
        if (c >= 'A' && c <= 'Z') {
            ++n;
        } else if (c >= 'a' && c <= 'z') {
            u = false;
            break;
        }
    }
    return u && (n >= 2 || (n >= 1 && s));
}

static bool digit(rune c)
{
    return (c >= '0' && c <= '9');
}

static bool hex_digit(rune c)
{
    return (c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'F') ||
        (c >= 'a' && c <= 'f');
}

static bool ident_begin(rune c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c == '_');
}

static bool ident_cont(rune c)
{
    return ident_begin(c) || digit(c);
}

bool num_digit(rune c, byte digtp)
{
    if (digtp == CHAR_BIN_DIGIT) {
        return hex_digit(c);
    }
    return c >= '0' && c <= digtp;
}

byte dig_value(rune c)
{
    if (c >= 'A' && c <= 'F') {
        return 10 + (byte)(c - 'A');
    } else if (c >= 'a' && c <= 'f') {
        return 10 + (byte)(c - 'a');
    } else {
        return (byte)c - '0';
    }
}

// 00000000 … 0000007F  -> 0xxxxxxx                             => 0x0000_007F
// 00000080 … 000007FF  -> 110xxxxx 10xxxxxx                    => 0x0000_07FF
// 00000800 … 0000FFFF  -> 1110xxxx 10xxxxxx 10xxxxxx           => 0x0000_FFFF
// 00010000 … 0010FFFF  -> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  => 0x001F_FFFF
// utf8-1 = (0x00 … 0x7F)
// utf8-2 = (0xC2 … 0xDF) uft8-tail
// utf8-3 = 0xE0 (0xA0 … 0xBF) utf8-tail |
//          0xED (0x80 … 0x9F) utf8-tail |
//          (0xE1 … 0xEC) utf8-tail utf8-tail |
//          (0xEE … 0xEF) utf8-tail utf8-tail
// utf8-4 = 0xF0 (0x90 … 0xBF) utf8-tail utf8-tail |
//          0xF4 (0x80 … 0x8F) utf8-tail utf8-tail |
//          (0xF1 … 0xF3) utf8-tail utf8-tail utf8-tail
// utf8-tail = (0x80 … 0xBF)

static byte unc2utf(uint32 u, byte *p)
{
    if (u < 0x80) {
        *p = (byte)u;
        return 1;
    }
    if (u < 0x800) {
        *p++ = 0xc0 | ((u >> 6) & 0x1F);
        *p = 0x80 | (u & 0x3F);
        return 2;
    }
    if (u < 0x10000) {
        *p++ = 0xE0 | ((u >> 12) & 0x0F);
        *p++ = 0x80 | ((u >> 6) & 0x3F);
        *p = 0x80 | (u & 0x3F);
        return 3;
    }
    if (u < 0x110000) {
label_4_byte:
        *p++ = 0xF0 | ((u >> 18) & 0x07);
        *p++ = 0x80 | ((u >> 12) & 0x3F);
        *p++ = 0x80 | ((u >> 6) & 0x3F);
        *p = 0x80 | (u & 0x3F);
        return 4;
    }
    u = CHAR_INVALID_UTF;
    goto label_4_byte;
}

typedef struct {
    rune c; // 第一个字节包含的有效比特值
    byte l; // 后面跟随几个utf尾部字节
    rune min; // 表示的unicode必须大于这个值
} utf_t;

utf_t utfhead(rune c)
{
    utf_t u;
    if (c < 0xc2 || c > 0xf4) {
        u.c = c;
        u.l = 0;
    } else if (c <= 0xdf) {
        u.c = c & 0x1f;
        u.l = 1;
        u.min = 0x7f;
    } else if (c <= 0xef) {
        u.c = c & 0xff;
        u.l = 2;
        u.min = 0x7ff;
    } else {
        u.c = c & 0x7;
        u.l = 3;
        u.min = 0xffff;
    }
    return u;
}

bool utftail(rune *u, rune c)
{
    if (c >= 0x80 && c <= 0xbf) {
        *u = (*u << 6) | (c & 0x3f);
        return true;
    }
    return false;
}

static Uint utf2unc(const byte *p, const byte *e, rune *out)
{
    const byte *s = p;
    utf_t u = utfhead(*p++);
    int i = 0;
    if (p > e) {
        return 0;
    }
    if (!u.l) {
        *out = u.c;
        return 1;
    }
    for (; i < u.l && p < e; i += 1, p += 1) {
        if (!utftail(&u.c, *p)) {
            break;
        }
    }
    if (i != u.l || u.c <= u.min) {
        log_error_3(ERROR_INVALID_UTF8_BYTE, u.c, u.l, i);
        u.c = CHAR_INVALID_UTF;
    }
    *out = u.c;
    return p - s;
}

void cifa_b128(chcc_t *cc)
{
    byte a[128] = {CHAR_128_BYTE_ARRAY_G};
    cc->b128 = (byte *)malloc(sizeof(a));
    memcpy(cc->b128, a, sizeof(a));
}

void cifa_esc(chcc_t *cc)
{
    // 转义字符：
    // \a \b \f \n \r \t \v \\ \' \"
    // \0 … \9
    // \x00 … \xff
    // \u0000 … /uffff
    // \U00000000 … /Uffffffff
    esc_t a[] = { // 大到小排序
        {'x',      0, 2},
        {'v',   '\v', 0},
        {'u',      0, 4},
        {'t',   '\t', 0},
        {'r',   '\r', 0},
        {'n',   '\n', 0},
        {'f',   '\f', 0},
        {'b',   '\b', 0},
        {'a',   '\a', 0},
        {'\\',  '\\', 0},
        {'U',      0, 8},
        {'9', '\x09', 0},
        {'8', '\x08', 0},
        {'7', '\x07', 0},
        {'6', '\x06', 0},
        {'5', '\x05', 0},
        {'4', '\x04', 0},
        {'3', '\x03', 0},
        {'2', '\x02', 0},
        {'1', '\x01', 0},
        {'0', '\x00', 0},
        {'\'',  '\'', 0},
        {'\"',  '\"', 0},
        {  0,      0, 0},
    };
    cc->esc = (esc_t *)malloc(sizeof(a));
    memcpy(cc->esc, a, sizeof(a));
}

void cifa_ops(chcc_t *cc)
{
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
    ops_t a[] = { // 从大到小排列
        {1, CIFA_OP_BOR_ASSIGN,     2, {'|', '='},              },  // 0x7C     0x00
        {2, CIFA_OP_LOR,            2, {'|', '|'},              },  //          0x01
        {6, CIFA_OP_BOR,            1, {'|'},                   },  //          0x02
        {1, CIFA_OP_XOR_ASSIGN,     2, {'^', '='},              },  // 0x5E     0x03
        {7, CIFA_OP_XOR,            1, {'^'},                   },  //          0x04
        {1, CIFA_OP_UDIV_ASSIGN,    3, {'\\', '\\', '='},       },  // 0x5C     0x05
        {7, CIFA_OP_UDIV,           2, {'\\', '\\'},            },  //          0x06
        {1, CIFA_OP_DIV_ASSIGN,     2, {'\\', '='},             },  //          0x07
        {7, CIFA_OP_DIV,            1, {'\\'},                  },  //          0x08
        {5, CIFA_OP_UGE,            3, {'>', '>', '='},         },  // 0x3E     0x09
        {5, CIFA_OP_UGT,            2, {'>', '>'},              },  //          0x0a
        {5, CIFA_OP_GE,             2, {'>', '='},              },  //          0x0b
        {5, CIFA_OP_GT,             1, {'>'},                   },  //          0x0c
        {4, CIFA_OP_EQ,             2, {'=', '='},              },  // 0x3D     0x0d
        {1, CIFA_OP_ASSIGN,         1, {'='},                   },  //          0x0e
        {1, CIFA_OP_LSH_ASSIGN,     4, {'<', '-', '-', '='},    },  // 0x3C     0x0f
        {7, CIFA_OP_LSH,            3, {'<', '-', '-'},         },  //          0x10
        {5, CIFA_OP_ULE,            3, {'<', '<', '='},         },  //          0x11
        {5, CIFA_OP_ULT,            2, {'<', '<'},              },  //          0x12
        {5, CIFA_OP_LE,             2, {'<', '='},              },  //          0x13
        {5, CIFA_OP_LT,             1, {'<'},                   },  //          0x14
        {1, CIFA_OP_INIT_ASSIGN,    2, {':', '='},              },  // 0x3A     0x15
        {0, CIFA_PT_LINE_CMMT,      2, {'/', '/'},              },  // 0x2F     0x16
        {0, CIFA_PT_BLOCK_CMMT,     2, {'/', '*'},              },  //          0x17
        {0, CIFA_PT_3DOT,           3, {'.', '.', '.'},         },  // 0x2E     0x18
        {1, CIFA_OP_RSH_ASSIGN,     4, {'-', '-', '>', '='},    },  // 0x2D     0x19
        {7, CIFA_OP_RSH,            3, {'-', '-', '>'},         },  //          0x1a
        {1, CIFA_OP_SUB_ASSIGN,     2, {'-', '='},              },  //          0x1b
        {0, CIFA_PT_ARROW,          2, {'-', '>'},              },  //          0x1c
        {6, CIFA_OP_SUB,            1, {'-'},                   },  //          0x1d
        {1, CIFA_OP_ADD_ASSIGN,     2, {'+', '='},              },  // 0x2B     0x1e
        {6, CIFA_OP_ADD,            1, {'+'},                   },  //          0x1f
        {1, CIFA_OP_UMUL_ASSIGN,    3, {'*', '*', '='},         },  // 0x2A     0x20
        {7, CIFA_OP_UMUL,           2, {'*', '*'},              },  //          0x21
        {1, CIFA_OP_MUL_ASSIGN,     2, {'*', '='},              },  //          0x22
        {7, CIFA_OP_MUL,            1, {'*'},                   },  //          0x23
        {1, CIFA_OP_AND_ASSIGN,     2, {'&', '='},              },  // 0x26     0x24
        {3, CIFA_OP_LAND,           2, {'&', '&'},              },  //          0x25
        {7, CIFA_OP_AND,            1, {'&'},                   },  //          0x26
        {1, CIFA_OP_UMOD_ASSIGN,    3, {'%', '%', '='},         },  // 0x25     0x27
        {7, CIFA_OP_UMOD,           2, {'%', '%'},              },  //          0x28
        {1, CIFA_OP_MOD_ASSIGN,     2, {'%', '='},              },  //          0x29
        {7, CIFA_OP_MOD,            1, {'%'},                   },  //          0x2a
        {4, CIFA_OP_NE,             2, {'!', '='},              },  // 0x21     0x2b
        {8, CIFA_OP_NOT,            1, {'!'},                   },  //          0x2c
        {0, 0,                      0,                          }   // 0x00
    };
    cc->ops = (ops_t *)malloc(sizeof(a));
    memcpy(cc->ops, a, sizeof(a));
}

void cifa_init(chcc_t *cc, file_t *f)
{
    byte predecl_ident[] = {
#define PREDECL(id, ...) __VA_ARGS__, 0x00,
#include "chcc/decl.h"
        0x00
    };
    const byte *p, *c;
    ident_t *sym = 0;

    cc->f = f;
    cifa_b128(cc);
    cifa_esc(cc);
    cifa_ops(cc);

    buffer_init(&cc->s, 0);
    bhash_init(&cc->hash_ident, IDENT_HASH_SIZE);
    array_ex_init(&cc->arry_ident, sizeof(ident_t*), IDENT_ARRAY_EXPAND);

    // 创建预定义的标识符
    p = predecl_ident;
    while (*p) {
        c = p;
        while (*c) ++c;
        sym = ident_create(cc, 0, true, string_create(p, c - p, 0));
        p = c + 1;
    }

    cc->user_id_start = sym->id + 1;
    cc->error = cc->errst;
    cc->errst[0] = 0;
}

void chcc_free(chcc_t *cc)
{
    buffer_free(&cc->s);
    bhash_free(&cc->hash_ident, ident_free);
    array_ex_free(&cc->arry_ident);
    stack_free(&cc->symbol, 0);
    free(cc->ops);
    free(cc->esc);
    free(cc->b128);
}

void rch(chcc_t *cc)
{
    cc->c = file_get(cc->f);
}

void rch_ex(chcc_t *cc, void (*f)(void))
{
    cc->c = file_get_ex(cc->f, f, cc);
}

void un_rch(chcc_t *cc)
{
    file_unget(cc->f);
}

void un_rch_ex(chcc_t *cc, int32 n)
{
    file_unget_ex(cc->f, n);
}

void utf(chcc_t *cc)
{
    rune c = cc->c;
    utf_t u;
    int i = 0;

    cc->unicode = false;

    if (c == CHAR_EOF || (u = utfhead(c)).l == 0) {
        return;
    }

    cc->unicode = true;

    for (; i < u.l; i += 1) {
        rch(cc);
        if (!utftail(&u.c, cc->c)) {
            un_rch(cc);
            break;
        }
    }

    if (i == u.l && u.c > u.min) {
        cc->c = u.c;
        return;
    }

    log_error_5(ERROR_INVALID_UTF8_BYTE, cc->line, cc->cols, u.c, u.l, i);
    cc->c = CHAR_INVALID_UTF;
}

void cifa_end(chcc_t *cc, cfid_t cfid, Error error)
{
    cifa_t *cf = &cc->cf;
    cf->cfid = cfid;
    if (error) {
        if (cc->error < cc->errst + CHCC_MAX_ERRORS) {
            *cc->error++ = error;
        }
        cf->error = error;
    }
}

void oper_end(chcc_t *cc, ops_t *op)
{
    // cfid < 0xc0
    // oper > 0 操作符，表示优先级
    // oper = 0 标点
    // val.c 对应机器指令
    cifa_t *cf = &cc->cf;
    cf->oper = op->prior;
    cf->val.c = op->inst;
    cifa_end(cc, op->cfid, 0);
}

static void newline(chcc_t *cc, rune c)
{
    // EOL - Windows (CR LF)    \r\n    0x0d 0x0a
    //     - Unix (LF)          \n      0x0a
    //     - Macintosh (CR)     \r      0x0d
    if (c == '\r') {
        rch(cc);
        if (cc->c == '\n') {
            // Windows \r\n
        } else {
            // Macintosh \r
            un_rch(cc);
        }
    }
    cc->line ++;
    cc->cols = 1;
}

void cmmt_end(chcc_t *cc, bool bcmmt)
{
    // cfid = 0xc2
    // bcmmt: 1 块注释
    // bcmmt: 0 行注释
    // cf->s   临时注释字符串
    cifa_t *cf = &cc->cf;
    cf->bcmmt = bcmmt;
    cf->s = string_ref_buffer(&cc->s);
    cifa_end(cc, CIFA_TYPE_COMMENT, 0);
}

void cpstr(void *p, const byte *e)
{
    chcc_t *cc = (chcc_t *)p;
    file_t *f = cc->f;
    buffer_t *b = &cc->s;
    byte *s = cc->start;
    buffer_push(b, s, e - s, CFSTR_ALLOC_EXPAND);
    cc->copied = true;
    cc->start = f->b.cur;
}

// digit = ("0" … "9") .
// dec_digit = "_" | ("0" … "9") .
// bin_digit = "_" | ("0" … "1") .
// hex_digit = "_" | ("0" … "9") | ("A" … "Z") | ("a" … "z") .
//
// dec_digits = dec_digit { dec_digit } .
// bin_digits = bin_digit { bin_digit } .
// hex_digits = hex_digit { hex_digit } .
//
// int_lit = dec_lit | bin_lit | hex_lit .
// dec_lit = digit { dec_digit } .
// bin_lit = "0" ("b" | "B") bin_digits .
// hex_lit = "0" ("x" | "X") hex_digits .
//
// float_lit = dec_float_lit | bin_float_lit | hex_float_lit .
// dec_exp = ("p" | "P") dec_digits .
// dec_float_lit =      // 10 ^ exp
//      dec_lit "." dec_digits |
//      dec_lit "." dec_digits dec_exp |
//      dec_lit "." dec_exp |
//      dec_lit dec_exp .
// bin_float_lit =      // 2 ^ exp
//      bin_lit "." bin_digits |
//      bin_lit "." bin_digits dec_exp |
//      bin_lit "." dec_exp |
//      bin_lit dec_exp .
// hex_float_lit =      // 2 ^ exp
//      hex_lit "." hex_digits |
//      hex_lit "." hex_digits dec_exp |
//      hex_lit "." dec_exp |
//      hex_lit dec_exp .
// 3.14 314P2 0.314p1

typedef struct {
    uint64 i;
    uint64 f;
    uint32 exp;
    uint8 isfloat: 1;
    uint8 exp_neg: 1;
    uint8 base;
    uint32 tlen;
    const byte *tail;
} numval_t;

Uint bool_end(chcc_t *cc, bool a)
{
    cifa_t *cf = &cc->cf;
    cf->isbool = 1;
    cf->val.c = a;
    cifa_end(cc, CIFA_TYPE_NUMERIC, 0);
    return 4 + (a == true);
}

Uint null_end(chcc_t *cc)
{
    cifa_t *cf = &cc->cf;
    cf->isnull = 1;
    cf->val.c = 0;
    cifa_end(cc, CIFA_TYPE_NUMERIC, 0);
    return 4;
}

void char_end(chcc_t *cc, rune c, Error error)
{
    cifa_t *cf = &cc->cf;
    cf->ischar = 1;
    cf->val.c = c;
    cifa_end(cc, CIFA_TYPE_NUMERIC, error);
}

void numlit_end(chcc_t *cc, const numval_t *val)
{
    // cfid = 0xc0
    // isbool: 1   val.c   布尔常量
    // isnull: 1   val.c   空值常量
    // ischar: 1   val.c   字符常量
    // iserr: 1    val.c   错误代码
    // isint: 1    val.c   整数常量    base    基数    cf->s   临时后缀字符串
    // isfloat:    val.f   浮点常量    base    基数    cf->s   临时后缀字符串
    cifa_t *cf = &cc->cf;
    if (val->isfloat) {
        cf->isfloat = 1;
    } else {
        cf->isint = 1;
        cf->val.c = val->i;
    }
    cf->base = val->base;
    cf->s = strflen(val->tail, val->tlen); // 数值字面量后缀
    cifa_end(cc, CIFA_TYPE_NUMERIC, 0);
}

void numtail(const byte *s, const byte *e, numval_t *v)
{
    byte c;
    while (s < e) {
        c = *s++;
        if (ident_begin(c)) { // 找到尾部标识符的开始
            v->tail = s - 1;
            goto label_tail;
        }
    }
    return;
label_tail:
    while (s < e) {
        c = *s++;
        if (!ident_cont(c)) {
            --s;
            break;
        }
    }
    v->tlen = s - v->tail;
}

void exppart(const byte *s, const byte *e, numval_t *v)
{
    uint32 exp = 0;
    byte c;
    v->isfloat = 1;
    while (s < e) {
        c = *s++;
        if (digit(c)) {
            exp = exp * 10 + (c - '0');
        } else if (c != '_') {
            --s;
            break; // 指数读取完毕
        }
    }
    v->exp = exp;
    numtail(s, e, v);
}

void fragpart(const byte *s, const byte *e, byte digtp, numval_t *v)
{
    byte c;
    byte shf = 0;
    byte digval;
    uint64 fpart = 0;
    v->isfloat = 1;
    while (s < e) {
        c = *s++;
        if (num_digit(c, digtp)) {
            digval = dig_value(c);
            if (shf < 64) {     // 如果数据过长只留小数点后的高位
                fpart |= (digval << shf);
                if (digtp == CHAR_BIN_DIGIT) {
                    shf += 1;   // 小数点后第一个数字放在值的最低位
                } else {
                    shf += 4;   // 十进制和十六进制都占4bit
                }
            }
        } else if (c == 'p' || c == 'P') {
            v->f = fpart;
            v->exp_neg = (c == 'P');
            exppart(s, e, v);
            return;
        } else if (c != '_') {
            --s;
            break; // 小数读取完毕
        }
    }
    v->f = fpart;
    numtail(s, e, v);
}

void intpart(const byte *s, const byte *e, numval_t *v)
{
    byte digtp;
    byte digval;
    byte c = *s++;
    uint64 i = 0;
    if (c == '0') {
        if (s >= e) {
            goto label_dec;
        } else {
            c = *s++;
            if (c == 'b' || c == 'B') {
                digtp = CHAR_BIN_DIGIT; // bin_lit
            } else if (c == 'x' || c == 'X') {
                digtp = CHAR_BIN_DIGIT; // hex_lit
            } else {
                goto label_dec;
            }
        }
    } else if (c >= '1' && c <= '9') {
label_dec:
        digtp = CHAR_DEC_DIGIT; // dec_lit
        --s;
    } else {
        return;
    }
    v->base = digtp;
    while (s < e) {
        c = *s++;
        if (num_digit(c, digtp)) {
            digval = dig_value(c);
            if (digtp == CHAR_BIN_DIGIT) { // 如果数据过大高位被截去只留低位
                i = (i << 4) + digval;
            } else if (digtp == CHAR_BIN_DIGIT) {
                i = (i << 1) + digval;
            } else {
                i = i * 10 + digval;
            }
        } else if (c == '.') {
            v->i = i;
            fragpart(s, e, digtp, v);
            return;
        } else if (c == 'p' || c == 'P') {
            v->i = i;
            v->f = 0;
            v->exp_neg = (c == 'P');
            exppart(s, e, v);
            return;
        } else if (c != '_') {
            --s;
            break; // 整数读取完毕
        }
    }
    v->i = i;
    numtail(s, e, v);
}

Uint numlit(chcc_t *cc, string_t *s)
{
    numval_t v = {0};
    intpart(s->a, s->a + s->len, &v);
    numlit_end(cc, &v);
    return s->len;
}

void ident_end(chcc_t *cc, ident_t *sym)
{
    // cfid > 0xff
    // cfid 标识符数组的索引
    // val.sym 指向标识符符号
    // pdid: 1 语言预定义标识符
    // tpid: 1 类型标识符
    // csid: 1 常量标识符
    // ident: 1 非类型非常量标识符
    cifa_t *cf = &cc->cf;
    string_t *s = &sym->s;
    cf->val.sym = sym;
    cf->s = *s;
    if (sym->id >= CIFA_ID_INT && sym->id <= CIFA_ID_STRING) {
        cf->tpid = 1;
    } else if (type_ident(s->a, s->a + s->len)) {
        cf->tpid = 1;
    } else if (cons_ident(s->a, s->a + s->len)) {
        cf->csid = 1;
    } else {
        cf->ident = 1;
    }
    if (sym->id < cc->user_id_start) {
        cf->pdid = 1;
    }
    cifa_end(cc, sym->id, 0);
}

static Uint idnum(chcc_t *cc, bool num)
{
    uint32 h = IDENT_HASH_INIT;
    buffer_t *s = &cc->s;
    buffix_t *b = &cc->f->b;
    ident_t *sym;
    string_t d;
    byte e_class = CHAR_CLASS_UNDERSCORE;
    byte c_class;
    rune c = cc->c;

    if (num) {
        e_class = CHAR_CLASS_DOT;
    } else {
        h = ident_hash(h, c);
    }

    buffer_clear(s);
    cc->start = b->cur - 1;
    cc->copied = false;

    for (; ;) {
        rch_ex(cc, cpstr);
        c = cc->c;
        if (c < 0x80 && (c_class = cc->b128[c]) >= CHAR_CLASS_DIGIT && c_class <= e_class) {
            if (!num) {
                h = ident_hash(h, c);
            }
        } else {
            un_rch(cc);
            break;
        }
    }

    if (s->len) {
        buffer_push(s, cc->start, b->cur - cc->start, CFSTR_ALLOC_EXPAND);
        d = strflen(s->a, s->len);
    } else {
        d = strflen(cc->start, b->cur);
    }

    if (num) {
        return numlit(cc, &d);
    }

    if (s->len == 4) {
        if (memcmp(s->a, "null", 4) == 0) {
            return null_end(cc);
        } else if (memcmp(s->a, "true", 4) == 0) {
            return bool_end(cc, true);
        }
    } else if (s->len == 5 && memcmp(s->a, "false", 5) == 0) {
        return bool_end(cc, false);
    }

    sym = ident_create(cc, h, false, d);
    ident_end(cc, sym);
    return d.len;
}

static Uint comment(chcc_t *cc, cfid_t c)
{
    buffix_t *b = &cc->f->b;
    buffer_t *s = &cc->s;
    Uint nch = 2; // // or /*
    buffer_clear(s);
    cc->start = b->cur; // 注释从下一个字符开始
    if (c == CIFA_PT_LINE_CMMT) {
        for (; ;) {
            rch_ex(cc, cpstr);
            c = cc->c;
            if (c == CHAR_EOF) {
                break;
            }
            if (c == CHAR_RETURN || c == CHAR_NEWLINE) {
                un_rch(cc);
                buffer_push(s, cc->start, b->cur - cc->start, CFSTR_ALLOC_EXPAND);
                break;
            }
        }
        nch += s->len;
        cmmt_end(cc, false);
    } else {
        for (; ;) {
            rch_ex(cc, cpstr);
            c = cc->c;
            if (c == CHAR_EOF) {
                break;
            }
            if (c == CHAR_RETURN || c == CHAR_NEWLINE) {
                buffer_push(s, cc->start, b->cur - 1 - cc->start, CFSTR_ALLOC_EXPAND);
                buffer_put(s, '\n', CFSTR_ALLOC_EXPAND);
                newline(cc, c);
                nch = 0;
                cc->start = b->cur - 1;
            } else if (c == '*') {
                rch_ex(cc, cpstr);
                if (cc->c == '/') {
                    buffer_push(s, cc->start, b->cur - 2 - cc->start, CFSTR_ALLOC_EXPAND); // */ 不属于注释内容
                    nch += 2; // */
                    break; // 块注释读取完毕
                }
                un_rch(cc);
            } else {
                nch += 1;
            }
        }
        cmmt_end(cc, true);
    }
    return nch;
}

static Uint esc(chcc_t *cc, rune quote)
{
    esc_t *a = cc->esc;
    Error error = null;
    Uint i, nch = 1;
    rune u = 0;
    rune c;
    rch(cc);
    c = cc->c;
    for (; a->c && c <= a->c; a += 1) {
        if (c < a->c) continue;
        u = a->d;
        for (i = 0; i < a->n; ++i) { // 开始处理 \xff \uffff \Uffffffff
            rch(cc);
            c = cc->c;
            if (hex_digit(c)) {
                u = (u << 4) | dig_value(c);
                nch += 1;
            } else {
                un_rch(cc); // 遇到不识别的字符则回退并退出
                error = ERROR_INVALID_HEXCHAR;
                goto label_finish;
            }
        }
        goto label_finish;
    }
    error = ERROR_INVALID_ESCCHAR;
    if (c == CHAR_EOF || c == CHAR_RETURN || c == CHAR_NEWLINE) {
        un_rch(cc); // 特殊的不识别转义字符需要回退
        nch = 0;
    }
    u = CHAR_SPACE; // 不识别转义字符解析成空格
label_finish:
    if (error) {
        log_error_3(error, cc->line, cc->cols + nch, c);
    }
    cc->c = u;
    cc->unicode = (a->n >= 4);
    return nch;
}

static int achar(chcc_t *cc)
{
    rune c;
    rch(cc);
    c = cc->c;
    if (c == CHAR_SQUOTE) {
        return -1; // 读取结束
    }
    if (c == CHAR_RETURN || c == CHAR_NEWLINE || c == CHAR_EOF) {
        un_rch(cc);
        return 0; // 读取结束
    }
    if (c == CHAR_BSLASH) {
        return 1 + esc(cc, CHAR_SQUOTE);
    }
    utf(cc); // 字符是一个数值类型，如果是utf8需要将其转换成对应的数值代码点
    return 1;
}

static Uint charlit(chcc_t *cc)
{
    Error error = null;
    Uint nch = 1;
    rune u = 0;
    int len = achar(cc);
    if (len <= 0) {
        nch += (-len);
        error = (len < 0) ? ERROR_EMPTY_RUNE_LIT : ERROR_MISS_CLOSE_QUOTE;
    } else {
        nch += len;
        u = cc->c;
        while ((len = achar(cc)) > 0) { // 跳过字符字面量中多余的字符
            error = ERROR_MULT_CHAR_EXIST;
            nch += len;
        }
        nch += (-len);
    }
    char_end(cc, u, error);
    return nch;
}

static Uint strlit(chcc_t *cc, rune quote)
{
    buffix_t *b = &cc->f->b;
    buffer_t *s = &cc->s;
    Uint nch = 1; // quote
    Error error = null;
    bool rawstr = false;
    byte utf8[4];
    byte len;
    rune c = 0;
    if (quote == CHAR_SQUOTE) {
        return charlit(cc);
    } else if (quote == CHAR_BQUOTE) {
        rawstr = true;
    }
    buffer_clear(s);
    cc->start = b->cur;
    cc->copied = false;
    for (; ;) {
        rch_ex(cc, cpstr);
        c = cc->c;
        if (c == CHAR_RETURN || c == CHAR_NEWLINE) {
            if (rawstr) {
                buffer_push(s, cc->start, b->cur - 1 - cc->start, CFSTR_ALLOC_EXPAND);
                newline(cc, c);
                buffer_put(s, '\n', CFSTR_ALLOC_EXPAND);
                cc->start = b->cur;
                nch = 0;
            } else {
                un_rch(cc);
                error = ERROR_MISS_CLOSE_QUOTE;
                break; // 字符串读取完毕
            }
        } else if (c == CHAR_EOF) {
            error = ERROR_MISS_CLOSE_QUOTE;
            goto label_copied; // 遇到结束引号前到达文件尾，字符串读取完毕
        } else if (c == CHAR_BSLASH && !rawstr) {
            buffer_push(s, cc->start, b->cur - 1 - cc->start, CFSTR_ALLOC_EXPAND);
            nch += esc(cc, CHAR_DQUOTE);
            if (cc->unicode) { // 代码点转换成utf8字符串字节流
                len = unc2utf(cc->c, utf8);
                buffer_push(s, utf8, len, CFSTR_ALLOC_EXPAND);
            } else {
                buffer_put(s, (byte)cc->c, CFSTR_ALLOC_EXPAND);
            }
            cc->start = b->cur;
        } else {
            nch += 1;
            if (c == quote) {
                break; // 字符串读取完毕
            }
        }
    }
    if (cc->copied) {
        buffer_push(s, cc->start, b->cur - 1 - cc->start, CFSTR_ALLOC_EXPAND);
label_copied:
        cc->cf.s = string_ref_buffer(&cc->s);
    } else {
        cc->cf.s = strflen(cc->start, b->cur - 1 - cc->start);
    }
    cifa_end(cc, CIFA_TYPE_STRING, error);
    return nch;
}

ops_t *oper(chcc_t *cc, rune c, byte idx)
{
    ops_t *a = cc->ops + idx;
    int i = 0;
    for (; a->len && a->op[0] == c; a += 1, i = 0) {
        for (; ;) {
            if (a->len == ++i) {
                return a;
            }
            rch(cc);
            if (cc->c != a->op[i]) {
                un_rch_ex(cc, i); // 如果不匹配回退除第一个字符外的所有字符
                break;
            }
        }
    }
}

void cur(chcc_t *cc) // 解析当前词法
{
    cifa_t *cf = &cc->cf;
    ops_t *op;
    rune c;
    uint8 t;
    Uint nch;
    for (; ;) {
        c = cc->c;
        memset(cf, 0, sizeof(cifa_t));
        cf->line = cc->line;
        cf->cols = cc->cols;
        if (c == CHAR_EOF) {
            cifa_end(cc, CHAR_EOF, 0);
            return;
        }
        if (c >= 0x80) {
            utf(cc); // 解析并跳过utf8字符
            cc->cols += 1;
            rch(cc); // 读取下一个字符
            continue;
        }
        t = cc->b128[c];
#if __CHCC_DEBUG__
        printf("line %04d col %03d %c %02x t %02x\n", cc->line, cc->cols, (t == CHAR_CLASS_BLANK) ? ' ' : (byte)c, c, t);
#endif
        if (t == CHAR_CLASS_BLANK) {
            if (c == CHAR_NEWLINE || c == CHAR_RETURN) {
                newline(cc, c);
            } else {
                cc->cols += 1;
            }
            rch(cc);
            continue;
        }
        if (t >= CHAR_CLASS_DIGIT && t <= CHAR_CLASS_UNDERSCORE) {
            nch = idnum(cc, (t == CHAR_CLASS_DIGIT));
        } else if (t == CHAR_CLASS_QUOTE) {
            nch = strlit(cc, c);
        } else {
            nch = 1;
            t = (t == CHAR_CLASS_DOT) ? CHAR_CLASS_MDOT : t;
            if (t >= CHAR_CLASS_OPERATOR) {
                op = oper(cc, c, t - CHAR_CLASS_OPERATOR);
            } else {
                goto label_punct;
            }
            if (op) {
                nch = op->len;
                if (!op->prior) {
                    c = op->cfid;
                    goto label_punct;
                }
                oper_end(cc, op);
            } else {
    label_punct:
                if (c == CIFA_PT_LINE_CMMT || c == CIFA_PT_BLOCK_CMMT) {
                    nch = comment(cc, c);
                } else {
                    cifa_end(cc, c, 0);
                }
            }
        }
        cc->cols += nch;
        rch(cc);
    }
}
