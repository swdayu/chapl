#define __CURR_FILE__ STRID_CHCC_CIFA
#include "internal/decl.h"
#include "chcc/chcc.h"
#include "chcc/gabi.h"

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

typedef struct {
    string_t *s;
    string_t *s2;
} identeq_t;

bool ident_eq(const void *object, const void *para)
{
    ident_t *sym = (ident_t *)object;
    identeq_t *p = (identeq_t *)para;
    string_t *s = p->s;
    string_t *s2 = p->s2;
    if (sym->s.len != s->len + (s2 ? s2->len : 0)) {
        return false;
    }
    if (memcmp(sym->s.a, s->a, s->len) != 0) {
        return false;
    }
    if (s2 && memcmp(sym->s.a + s->len, s2->a, s2->len) != 0) {
        return false;
    }
    return true;
}

ident_t *findhashident(hashident_t *a, string_t s, uint32 hash)
{
    identeq_t param = {&s};
    return (ident_t *)bhash_find(&a->hash_ident, hash, ident_eq, &param);
}

ident_t *pushhashident_x(hashident_t *a, string_t s, string_t s2, uint32 hash, bool calc)
{
    array_ex_t *arry_ident = a->arry_ident.a;
    bhash_t *hash_ident = a->hash_ident.a;
    identeq_t param = {&s, string_empty(&s2) ? null : &s2};
    bhash_node_t node;
    ident_t *d;
    intd_t i = 0;
    if (calc) {
        hash = IDENT_HASH_INIT;
        for (i = 0; i < s.len; i++) {
            hash = ident_hash(hash, s.a[i]);
        }
        for (i = 0; i < s2.len; i++) {
            hash = ident_hash(hash, s2.a[i]);
        }
    }
    if ((d = (ident_t *)bhash_find_x(hash_ident, hash, ident_eq, &param, &node))) {
        return d; // 同名标识符在哈希表中已经存在
    }
    d = (ident_t *)bhash_push_x(node, sizeof(ident_t) + s.len + s2.len);
    if (!d) {
        log_error_s(ERROR_HASH_IDENT_PUSH_FAILED, s);
        return null;
    }
    memcpy((byte *)(d + 1), s.a, s.len); // 哈希表新创建的符号，这里初始化这个符号
    if (s2.len) { memcpy((byte *)(d + 1) + s.len, s2.a, s2.len); }
    string_init(&d->s, (byte *)(d + 1), s.len + s2.len, false);
    if (memchr(d->s.a, '~', d->s.len)) { d->haspkgprefix = true; }
    d->id = (CIFA_IDENT_START) + array_ex_len(arry_ident); // 将符号添加到符号数组中，并初始化该符号在数组中的序号
    if (!array_ex_push(&a->arry_ident, (byte *)d, IDENT_ARRAY_EXPAND)) {
        log_error_s(ERROR_ARRAY_IDENT_PUSH_FAILED, s);
    }
#if __CHCC_DEBUG__
    printf("ident %08x id %08x [%d] %s\n", hash, d->id, d->s.len, d->s.a);
#endif
    return d;
}

ident_t *pushhashident(hashident_t *a, string_t name, uint32 hash, bool calc)
{
    return pushhashident_x(a, name, strnull(), hash, calc);
}

static bool type_ident(const byte *p, const byte *e)
{
    int96 len = e - p;
    byte c = *p++;
    bool s = (c == '_'); // 首字母下划线
    bool u = (c >= 'A' && c <= 'Z'); // 首字母大写
    bool t = ((*(e-2) == '_') && (*(e-1) == 't')); // 以_t结尾
    bool l = false; // 第二个字符开始包含小写或包含数字
    while (p < e) {
        c = *p++;
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            l = true;
            break;
        }
    }
    return (u && (len == 1 || l)) || (len > 2 && !s && t);
}

static bool cons_ident(const byte *p, const byte *e)
{
    byte c = *p;
    int n = 0; // 大写个数
    bool u = true; // 全部大写（不包含小写字符）
    bool s = (c == '_'); // 首字母下划线
    while (p < e) {
        c = *p++;
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
    if (digtp == CHAR_HEX_DIGIT) {
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

static uint96 utf2unc(const byte *p, const byte *e, rune *out)
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

void cifa_b128(prearr_t *p)
{
    byte a[128] = {CHAR_128_BYTE_ARRAY_G};
    p->b128 = (byte *)malloc(sizeof(a));
    memcpy(p->b128, a, sizeof(a));
}

void cifa_esc(prearr_t *p)
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
    p->esc = (esc_t *)malloc(sizeof(a));
    memcpy(p->esc, a, sizeof(a));
}

void cifa_ops(prearr_t *p)
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
        {1, CIFA_OP_BOR_ASSIGN,     2, 0, {'|', '='},              },  // 0x7C     0x00
        {2, CIFA_OP_LOR,            2, 0, {'|', '|'},              },  //          0x01
        {6, CIFA_OP_BOR,            1, 0, {'|'},                   },  //          0x02
        {1, CIFA_OP_XOR_ASSIGN,     2, 0, {'^', '='},              },  // 0x5E     0x03
        {7, CIFA_OP_XOR,            1, 1, {'^'},                   },  //          0x04
        {1, CIFA_OP_UDIV_ASSIGN,    3, 0, {'\\', '\\', '='},       },  // 0x5C     0x05
        {7, CIFA_OP_UDIV,           2, 0, {'\\', '\\'},            },  //          0x06
        {1, CIFA_OP_DIV_ASSIGN,     2, 0, {'\\', '='},             },  //          0x07
        {7, CIFA_OP_DIV,            1, 0, {'\\'},                  },  //          0x08
        {5, CIFA_OP_UGE,            3, 0, {'>', '>', '='},         },  // 0x3E     0x09
        {5, CIFA_OP_UGT,            2, 0, {'>', '>'},              },  //          0x0a
        {5, CIFA_OP_GE,             2, 0, {'>', '='},              },  //          0x0b
        {5, CIFA_OP_GT,             1, 0, {'>'},                   },  //          0x0c
        {4, CIFA_OP_EQ,             2, 0, {'=', '='},              },  // 0x3D     0x0d
        {1, CIFA_OP_ASSIGN,         1, 0, {'='},                   },  //          0x0e
        {1, CIFA_OP_LSH_ASSIGN,     4, 0, {'<', '-', '-', '='},    },  // 0x3C     0x0f
        {7, CIFA_OP_LSH,            3, 0, {'<', '-', '-'},         },  //          0x10
        {5, CIFA_OP_ULE,            3, 0, {'<', '<', '='},         },  //          0x11
        {5, CIFA_OP_ULT,            2, 0, {'<', '<'},              },  //          0x12
        {5, CIFA_OP_LE,             2, 0, {'<', '='},              },  //          0x13
        {5, CIFA_OP_LT,             1, 0, {'<'},                   },  //          0x14
        {1, CIFA_OP_INIT_ASSIGN,    2, 0, {':', '='},              },  // 0x3A     0x15
        {0, CIFA_PT_LINE_CMMT,      2, 0, {'/', '/'},              },  // 0x2F     0x16
        {0, CIFA_PT_BLOCK_CMMT,     2, 0, {'/', '*'},              },  //          0x17
        {0, CIFA_PT_3DOT,           3, 0, {'.', '.', '.'},         },  // 0x2E     0x18
        {1, CIFA_OP_RSH_ASSIGN,     4, 0, {'-', '-', '>', '='},    },  // 0x2D     0x19
        {7, CIFA_OP_RSH,            3, 0, {'-', '-', '>'},         },  //          0x1a
        {1, CIFA_OP_SUB_ASSIGN,     2, 0, {'-', '='},              },  //          0x1b
        {0, CIFA_PT_ARROW,          2, 0, {'-', '>'},              },  //          0x1c
        {6, CIFA_OP_SUB,            1, 1, {'-'},                   },  //          0x1d
        {1, CIFA_OP_ADD_ASSIGN,     2, 0, {'+', '='},              },  // 0x2B     0x1e
        {6, CIFA_OP_ADD,            1, 1, {'+'},                   },  //          0x1f
        {1, CIFA_OP_UMUL_ASSIGN,    3, 0, {'*', '*', '='},         },  // 0x2A     0x20
        {7, CIFA_OP_UMUL,           2, 0, {'*', '*'},              },  //          0x21
        {1, CIFA_OP_MUL_ASSIGN,     2, 0, {'*', '='},              },  //          0x22
        {7, CIFA_OP_MUL,            1, 1, {'*'},                   },  //          0x23
        {1, CIFA_OP_AND_ASSIGN,     2, 0, {'&', '='},              },  // 0x26     0x24
        {3, CIFA_OP_LAND,           2, 0, {'&', '&'},              },  //          0x25
        {7, CIFA_OP_AND,            1, 1, {'&'},                   },  //          0x26
        {1, CIFA_OP_UMOD_ASSIGN,    3, 0, {'%', '%', '='},         },  // 0x25     0x27
        {7, CIFA_OP_UMOD,           2, 0, {'%', '%'},              },  //          0x28
        {1, CIFA_OP_MOD_ASSIGN,     2, 0, {'%', '='},              },  //          0x29
        {7, CIFA_OP_MOD,            1, 0, {'%'},                   },  //          0x2a
        {4, CIFA_OP_NE,             2, 0, {'!', '='},              },  // 0x21     0x2b
        {8, CIFA_OP_NOT,            1, 1, {'!'},                   },  //          0x2c
        {0, 0,                      0,                          }   // 0x00
    };
    p->ops = (ops_t *)malloc(sizeof(a));
    memcpy(p->ops, a, sizeof(a));
}

void rch(bufile_t *top)
{
    top->c = file_get(top->f);
}

void rch_ex(bufile_t *top, void (*f)(void *p, const byte *e))
{
    top->c = file_get_ex(top->f, f, top);
}

void un_rch(bufile_t *top)
{
    file_unget(top->f);
}

void un_rch_ex(bufile_t *top, int32 n)
{
    file_unget_ex(top->f, n);
}

void utf(bufile_t *top)
{
    rune c = top->c;
    utf_t u;
    int i = 0;

    top->cf.unicode = false;

    if (c == CHAR_EOF || (u = utfhead(c)).l == 0) {
        return;
    }

    top->cf.unicode = true;

    for (; i < u.l; i += 1) {
        rch(top);
        if (!utftail(&u.c, top->c)) {
            un_rch(top);
            break;
        }
    }

    if (i == u.l && u.c > u.min) {
        top->c = u.c;
        return;
    }

    log_error_5(ERROR_INVALID_UTF8_BYTE, top->line, top->cols, u.c, u.l, i);
    top->c = CHAR_INVALID_UTF;
}

void err(bufile_t *top, errot error, uint32 n)
{
    top->haserr = true;
    log_error_3(error, top->line, top->cols, n);
}

void errs(bufile_t *top, errot error, string_t s, uint32 n)
{
    top->haserr = true;
    log_error_s_3(error, s, top->line, top->cols, n);
}

void ferr(bufile_t *top, errot error, uint32 n)
{
    err(top, error, n);
    exit(1);
}

void ferrs(bufile_t *top, errot error, string_t s, uint32 n)
{
    errs(top, error, s, n);
    exit(1);
}

void cifa_end(bufile_t *top, cfid_t cfid, errot error)
{
    cifa_t *cf = &top->cf;
    cf->cfid = cfid;
    if (error) {
        top->haserr = true;
        top->error = error;
    }
}

void oper_end(bufile_t *top, ops_t *op)
{
    // cfid < 0xc0
    // oper > 0 操作符，oper->prior 表示优先级，oper->inst 表示指令
    // oper = 0 标点
    cifa_t *cf = &top->cf;
    cf->optr = op;
    cf->oper = op->prior;
    cifa_end(top, op->cfid, 0);
}

static void newline(bufile_t *top, rune c)
{
    // EOL - Windows (CR LF)    \r\n    0x0d 0x0a
    //     - Unix (LF)          \n      0x0a
    //     - Macintosh (CR)     \r      0x0d
    if (c == '\r') {
        rch(top);
        if (top->c == '\n') {
            // Windows \r\n
        } else {
            // Macintosh \r
            un_rch(top);
        }
    }
    top->line ++;
    top->cols = 1;
}

void cmmt_end(bufile_t *top, cfid_t cfid)
{
    // iscmm: 1
    // cf->s   临时注释字符串
    cifa_t *cf = &top->cf;
    cf->iscmm = 1;
    cf->val.str = string_ref_buffer(&top->s);
    cifa_end(top, cfid, 0);
}

void cpstr(void *p, const byte *e)
{
    bufile_t *top = (bufile_t *)p;
    file_t *f = top->f;
    buffer_t *b = &top->s;
    byte *s = top->start;
    buffer_push(b, s, e - s, CFSTR_ALLOC_EXPAND);
    top->start = f->b.cur;
}

void un_cpstr(bufile_t *top, uint32 n) {
    buffer_t *b = &top->s;
    if (b->len > n) {
        b->len -= n;
    }
}

// sign = ("+" | "-") .
// digit = ("0" … "9") .
// dec_digit = "_" | ("0" … "9") .
// bin_digit = "_" | ("0" … "1") .
// hex_digit = "_" | ("0" … "9") | ("A" … "Z") | ("a" … "z") .
//
// dec_digits = dec_digit { dec_digit } .
// bin_digits = bin_digit { bin_digit } .
// hex_digits = hex_digit { hex_digit } .
//
// int_lit = [ sign ] (dec_lit | bin_lit | hex_lit) .
// dec_lit = digit { dec_digit } .
// bin_lit = "0" ("b" | "B") bin_digits .
// hex_lit = "0" ("x" | "X") hex_digits .
//
// float_lit = [ sign ] (dec_float_lit | bin_float_lit | hex_float_lit) .
// dec_exp = ("p" | "P") ["+" | "-"] dec_digits .
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

typedef struct {
    uint64 i;
    uint64 f;
    uint32 exp;
    uint8 isfloat: 1;
    uint8 exp_neg: 1;
    uint8 int_neg: 1;
    uint8 base;
    uint32 tlen;
    const byte *tail;
} numval_t;

void rune_end(bufile_t *top, rune c, errot error)
{
    cifa_t *cf = &top->cf;
    cf->islit = 1;
    cf->val.c = c;
    cifa_end(top, (c <= 0xff) ? CIFA_ID_BYTE : CIFA_ID_RUNE, error);
}

void numlit_end(bufile_t *top, const numval_t *val)
{
    // cfid = 0xc0
    // isbool: 1   val.c   布尔常量
    // isnull: 1   val.c   空值常量
    // ischar: 1   val.c   字符常量
    // iserr: 1    val.c   错误代码
    // isint: 1    val.i   整数常量    base    基数    cf->s   临时后缀字符串
    // isfloat:    val.f   浮点常量    base    基数    cf->s   临时后缀字符串
    cifa_t *cf = &top->cf;
    cfid_t cfid = CIFA_ID_INT;
    if (val->isfloat) {
        cf->isfloat = 1;
        cfid = CIFA_ID_FLOAT;
    } else {
        cf->isint = 1;
        if (val->i > LANG_INT32_UMAX) {
            cfid = CIFA_ID_INT64;
            cf->val.i64 = val->i;
        } else {
            cf->val.c = (uint32)val->i;
        }
    }
    cf->islit = 1;
    cf->numbase = val->base;
    cf->s = strflen(val->tail, val->tlen); // 数值字面量后缀
    cifa_end(top, cfid, 0);
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
        if (!ident_cont(c) && c != '~') {
            --s;
            break;
        }
    }
    v->tlen = s - v->tail;
}

void exppart(const byte *s, const byte *e, numval_t *v)
{
    uint32 exp = 0;
    byte c = *s;
    v->isfloat = 1;
    if (s < e && (c == '+' || c == '-')) {
        v->exp_neg = (c == '-');
        ++s;
    }
    while (s < e) {
        c = *s++;
        if (digit(c)) { // 指数总是十进制数
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
    if (c == '+' || c == '-') {
        v->int_neg = (c == '-');
        c = *s++;
    }
    if (c == '0') {
        if (s >= e) {
            goto label_dec;
        } else {
            c = *s++;
            if (c == 'b' || c == 'B') {
                digtp = CHAR_BIN_DIGIT; // bin_lit
            } else if (c == 'x' || c == 'X') {
                digtp = CHAR_HEX_DIGIT; // hex_lit
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
            if (digtp == CHAR_HEX_DIGIT) { // 如果数据过大高位被截去只留低位
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

uint96 numlit(bufile_t *top, string_t s)
{
    numval_t v = {0};
    intpart(s.a, s.a + s.len, &v);
    numlit_end(top, &v);
    return s.len;
}

void ident_end(bufile_t *top, string_t s, uint32 hash, uint32 pkhash, uint32 pknm_len)
{
    // cfid = CIFA_TYPE_IDENT
    // cf->s 临时标识符名称
    // val.c 标识符字符串的哈希值
    // ident.haspknm: 1 包名引用的符号，pknm_len 包名的长度
    // ident.istype: 1 类型名
    // ident.isconst: 1 常量名
    // ident.isvar: 1 变量名
    cifa_t *cf = &top->cf;
    cf->s = s;
    cf->val.c = hash;
    cf->pkhash = pkhash;
    cf->pknm_len = pknm_len;
    if (pknm_len) {
        cf->haspknm = 1;
    }
    if (type_ident(s.a + pknm_len, s.a + s.len)) {
        cf->istype = 1;
    } else if (cons_ident(s.a + pknm_len, s.a + s.len)) {
        cf->isconst = 1;
    } else {
        cf->isvar = 1;
    }
    cifa_end(top, CIFA_TYPE_IDENT, 0);
}

static uint96 idnum(bufile_t *top, bool num)
{
    const byte *b128 = top->a.b128;
    errot error = null;
    uint32 h = IDENT_HASH_INIT;
    buffer_t *s = &top->s;
    buffix_t *b = &top->f->b;
    ident_t *sym;
    string_t d;
    uint32 pkhash;
    uint32 pknm_len = 1;
    byte t, e = CHAR_CLASS_IDENT;
    rune c = top->c;

    if (num) {
        e = CHAR_CLASS_SIGN;
    } else {
        h = ident_hash(h, c);
    }

    buffer_clear(s);
    top->start = b->cur - 1;

    for (; ;) {
        rch_ex(top, cpstr);
        c = top->c;
        if (c < 0x80 && (t = b128[c]) >= CHAR_CLASS_DIGIT && t <= e) {
            if (!num) {
                h = ident_hash(h, c);
                pknm_len += 1;
            }
        } else {
            break;
        }
    }

    if (num || c != '~') {
        pknm_len = 0;
        goto label_finish;
    }
    pknm_len += 1; // 包名长度包含最后的~字符
    pkhash = h = ident_hash(h, c);
    rch_ex(top, cpstr);
    c = top->c;
    if (c >= 0x80 || b128[c] != CHAR_CLASS_IDENT) {
        error = ERROR_MISSING_EXPORT_SYM;
        log_error_3(error, top->line, top->cols + pknm_len, c);
        goto label_finish;
    }
    h = ident_hash(h, c); // 标识符第一个字符
    for (; ;) {
        rch_ex(top, cpstr);
        c = top->c;
        if (c < 0x80 && ((t = b128[c]) == CHAR_CLASS_DIGIT || t == CHAR_CLASS_IDENT)) {
            h = ident_hash(h, c);
        } else {
            break;
        }
    }

label_finish:
    un_rch(top); // 以上算法总会预读一个字符
    un_cpstr(b, 1);
    if (s->len) {
        buffer_push(s, top->start, b->cur - top->start, CFSTR_ALLOC_EXPAND);
        d = strflen(s->a, s->len);
    } else {
        d = strfend(top->start, b->cur);
    }

    if (num) {
        numlit(top, d);
    } else if (error) {
        cifa_end(top, c, error);
    } else {
        ident_end(top, d, h, pkhash, pknm_len);
    }

    return d.len;
}

static uint96 comment(bufile_t *top, cfid_t c)
{
    buffix_t *b = &top->f->b;
    buffer_t *s = &top->s;
    uint96 nch = 2; // // or /*
    buffer_clear(s);
    top->start = b->cur; // 注释从下一个字符开始
    if (c == CIFA_PT_LINE_CMMT) {
        for (; ;) {
            rch_ex(top, cpstr);
            c = top->c;
            if (c == CHAR_EOF) {
                break;
            }
            if (c == CHAR_RETURN || c == CHAR_NEWLINE) {
                un_rch(top);
                un_cpstr(top, 1);
                buffer_push(s, top->start, b->cur - top->start, CFSTR_ALLOC_EXPAND);
                break;
            }
        }
        nch += s->len;
        cmmt_end(top, CIFA_PT_LINE_CMMT);
    } else {
        for (; ;) {
            rch_ex(top, cpstr);
            c = top->c;
            if (c == CHAR_EOF) {
                break;
            }
            if (c == CHAR_RETURN || c == CHAR_NEWLINE) {
                buffer_push(s, top->start, b->cur - 1 - top->start, CFSTR_ALLOC_EXPAND);
                buffer_put(s, '\n', CFSTR_ALLOC_EXPAND);
                newline(top, c);
                nch = 0;
                top->start = b->cur - 1;
            } else {
                nch += 1;
                if (c != '*') {
                    continue;
                }
                rch_ex(top, cpstr);
                if (top->c == '/') {
                    buffer_push(s, top->start, b->cur - 2 - top->start, CFSTR_ALLOC_EXPAND); // */ 不属于注释内容
                    nch += 1; // */
                    break; // 块注释读取完毕
                }
                un_rch(top);
                un_cpstr(top, 1);
            }
        }
        cmmt_end(top, CIFA_PT_BLOCK_CMMT);
    }
    return nch;
}

static uint96 esc(bufile_t *top, rune quote, errot *out)
{
    const esc_t *a = top->a.esc;
    errot error = null;
    uint96 i, nch = 1;
    rune u = 0;
    rune c;
    rch(top);
    c = top->c;
    for (; a->c && c <= a->c; a += 1) {
        if (c < a->c) continue;
        u = a->d;
        for (i = 0; i < a->n; ++i) { // 开始处理 \xff \uffff \Uffffffff
            rch(top);
            c = top->c;
            if (c == quote || c == CHAR_BSLASH || c == CHAR_EOF || c == CHAR_RETURN || c == CHAR_NEWLINE) {
                un_rch(top); // 遇到结束字符回退并退出
                u <<= 4 * (a->n - i); // 剩余的字符都看出是0
                error = ERROR_INVALID_HEXNUMB;
                goto label_finish;
            } else {
                if (hex_digit(c)) {
                    u = (u << 4) | dig_value(c);
                } else {
                    u = u << 4; // 未知的字符当成字符0
                    error = ERROR_INVALID_HEXCHAR;
                }
                nch += 1;
            }
        }
        goto label_finish;
    }
    error = ERROR_INVALID_ESCCHAR;
    if (c == CHAR_EOF || c == CHAR_RETURN || c == CHAR_NEWLINE) {
        un_rch(top); // 特殊的不识别转义字符需要回退
        nch = 0;
    }
    u = CHAR_SPACE; // 不识别转义字符解析成空格
label_finish:
    if (error) {
        log_error_3(error, top->line, top->cols + nch, c);
        if (out) *out = error;
    }
    top->c = u;
    top->cf.unicode = (a->n >= 4);
    return nch;
}

static int a_rune(bufile_t *top, errot *error)
{
    rune c;
    rch(top);
    c = top->c;
    if (c == CHAR_SQUOTE) {
        return -1; // 读取结束
    }
    if (c == CHAR_RETURN || c == CHAR_NEWLINE || c == CHAR_EOF) {
        un_rch(top);
        return 0; // 读取结束
    }
    if (c == CHAR_BSLASH) {
        return 1 + esc(top, CHAR_SQUOTE, error);
    }
    utf(top); // 字符是一个数值类型，如果是utf8需要将其转换成对应的数值代码点
    return 1;
}

static uint96 runelit(bufile_t *top)
{
    errot error = null;
    uint96 nch = 1;
    rune u = 0;
    int len = a_rune(top, &error);
    if (len <= 0) {
        nch += (-len);
        error = (len < 0) ? ERROR_EMPTY_RUNE_LIT : ERROR_MISS_CLOSE_QUOTE;
    } else {
        nch += len;
        u = top->c;
        len = a_rune(top, null);
        if (len == 0) {
            if (!error) { // 最先出现的错误优先
                error = ERROR_MISS_CLOSE_QUOTE;
            }
        } else {
            while (len > 0) { // 跳过字符字面量中多余的字符
                nch += len;
                len = a_rune(top, null);
                if (!error) { // 最先出现的错误优先
                    error = ERROR_MULT_CHAR_EXIST;
                }
            }
            nch += (-len);
        }
    }
    rune_end(top, u, error);
    return nch;
}

static uint96 strlit(bufile_t *top, rune quote)
{
    buffix_t *b = &top->f->b;
    buffer_t *s = &top->s;
    const byte *pend;
    uint96 nch = 1; // quote
    errot error = null;
    bool rawstr = false;
    byte utf8[4];
    byte len;
    rune c = 0;
    if (quote == CHAR_SQUOTE) {
        return runelit(top);
    } else if (quote == CHAR_BQUOTE) {
        rawstr = true;
    }
    buffer_clear(s);
    top->start = b->cur;
    for (; ;) {
        rch_ex(top, cpstr);
        c = top->c;
        if (c == CHAR_RETURN || c == CHAR_NEWLINE) {
            if (rawstr) {
                buffer_push(s, top->start, b->cur - 1 - top->start, CFSTR_ALLOC_EXPAND);
                newline(top, c);
                buffer_put(s, '\n', CFSTR_ALLOC_EXPAND);
                top->start = b->cur;
                nch = 0;
            } else {
                un_rch(top);
                un_cpstr(top, 1);
                error = ERROR_MISS_CLOSE_QUOTE;
                pend = b->cur;
                break; // 字符串读取完毕
            }
        } else if (c == CHAR_EOF) {
            error = ERROR_MISS_CLOSE_QUOTE;
            goto label_copied; // 遇到结束引号前到达文件尾，字符串读取完毕
        } else if (c == CHAR_BSLASH && !rawstr) {
            buffer_push(s, top->start, b->cur - 1 - top->start, CFSTR_ALLOC_EXPAND);
            nch += 1 + esc(top, CHAR_DQUOTE, &error);
            if (top->cf.unicode) { // 代码点转换成utf8字符串字节流
                len = unc2utf(top->c, utf8);
                buffer_push(s, utf8, len, CFSTR_ALLOC_EXPAND);
            } else {
                buffer_put(s, (byte)top->c, CFSTR_ALLOC_EXPAND);
            }
            top->start = b->cur;
        } else {
            nch += 1;
            if (c == quote) {
                pend = b->cur - 1;
                break; // 字符串读取完毕
            }
        }
    }
    if (s->len) {
        buffer_push(s, top->start, pend - top->start, CFSTR_ALLOC_EXPAND);
label_copied:
        top->cf.val.str = string_ref_buffer(&top->s);
    } else {
        top->cf.val.str = strflen(top->start, pend - top->start);
    }
    top->cf.islit = 1;
    top->cf.isstr = 1;
    cifa_end(top, CIFA_ID_STRING, error);
    return nch;
}

ops_t *oper(bufile_t *top, rune c, byte idx)
{
    ops_t *a = top->a.ops + idx;
    int i = 0;
    for (; a->len && a->op[0] == c; a += 1, i = 0) {
        for (; ;) {
            if (a->len == ++i) {
                return a;
            }
            rch(top);
            if (top->c != a->op[i]) {
                un_rch_ex(top, i); // 如果不匹配回退除第一个字符外的所有字符
                break;
            }
        }
    }
    return null;
}

bool ident(bufile_t *top)
{
    cifa_t *cf = &top->cf;
    hashident_t *a = top->a.hash;
    string_t pknm;
    ident_t *real_pknm;
    ident_t *ident;
    symb_t *symb;
    if (cf->haspknm) { // 该标识符包含包名前缀
        pknm = strflen(cf->s.a, cf->pknm_len); // 获取该包名前缀
        real_pknm = findhashident(a, pknm, cf->pkhash); // 从哈希表中查找这个包名
        if (!real_pknm || !real_pknm->pknm) { // 未找到对应的标识符
            err(top, ERROR_PACKAGE_NOT_FOUND, 0);
            return false;
        }
        if (real_pknm == real_pknm->pknm) { // val.c 保存了整个字符串的哈希值
            ident = pushhashident(a, cf->s, cf->val.c, false);
        } else {
            real_pknm = real_pknm->pknm; // 用这个真实的包名查找该标识符，如果找到直接使用这个标识符，否则创建
            ident = pushhashident_x(a, real_pknm->s, strfend(cf->s.a + cf->pknm_len, cf->s.a + cf->s.len), 0, true);
        }
    } else {
        ident = pushhashident(a, cf->s, cf->val.c, false);
    }
    if (!ident) {
        return true;
    }
    // 在当前作用域下，标识符创建的符号可能有另外一个真实的名称，当前的标识符只是它的别名
    ident = getrealident(ident);
    if (ident->id < CIFA_ID_LANG_PKG) {
        cf->isvar = 0;
        cf->defvar = 0;
        cf->refvar = 0;
        cf->predecl = 1;
        if (ident->id <= CIFA_ID_FUNC) {
            cf->keyword = 1; // 关键字不能作任何其他用
        } else if (ident->id == CIFA_ID_NULL) {
            cf->cfid = CIFA_ID_NULL_TYPE;
            cf->islit = 1;
            cf->isint = 1;
            cf->val.c = 0;
            return true;
        } else if (ident->id == CIFA_ID_TRUE || ident->id == CIFA_ID_FALSE) {
            cf->cfid = CIFA_ID_BOOL;
            cf->val.c = (ident->id == CIFA_ID_TRUE);
            cf->islit = 1;
            cf->isint = 1;
            return true;
        } else if (ident->id >= CIFA_ID_INT && ident->id <= CIFA_ID_STRING) {
            cf->istype = 1;
            cf->reftype = 1; // 该名称是引用预定义类型
        } else if (ident->id >= CIFA_ID_PRINT) {
            cf->isvar = 1;
            cf->refvar = 1; // 该名称是引用预定义变量
        }
    }
    if (ident->haspkgprefix) {
        if (cf->istype) {
            cf->deftype = 0;
            cf->reftype = 1;
        }
        if (cf->isconst) {
            cf->defconst = 0;
            cf->refconst = 1;
        }
        if (cf->isvar) {
            cf->defvar = 0;
            cf->refvar = 1;
        }
    } else {
        if (cf->istype && !cf->reftype) {
            if (ident->defsym) {
                cf->reftype = 1; // 该类型名称已经定义
            } else {
                cf->deftype = 1;
            }
        }
        if (cf->isconst) {
            if (ident->defsym) {
                cf->refconst = 1; // 该常量名称已经定义
            } else {
                cf->defconst = 1;
            }
        }
        if (cf->isvar && !cf->refvar) {
            if (ident->defsym) {
                cf->refvar = 1; // 该变量名称已经定义
            } else {
                cf->defvar = 1;
            }
        }
    }
    cf->ident = ident;
    cf->cfid = ident->id;
    return true;
}

void crlf(bufile_t *top)
{
    rune c;
    uint8 t;
label_cont:
    c = top->c;
    if (c < 0x80 && (t = top->a.b128[c]) == CHAR_CLASS_BLANK) {
        if (c == CHAR_NEWLINE || c == CHAR_RETURN) {
            newline(top, c);
            top->cols = 0;
            top->c = ';'; // 将换行当成分号
            return;
        }
        top->cols += 1;
        rch(top);
        goto label_cont;
    }
}

void cur(bufile_t *top) // 解析当前词法
{
    cifa_t *cf = &top->cf;
    ops_t *op;
    rune c;
    uint8 t;
    uint96 nch;
label_cont:
    c = top->c;
    memset(cf, 0, sizeof(cifa_t));
    cf->line = top->line;
    cf->cols = top->cols;
    if (c == CHAR_EOF) {
        cifa_end(top, CHAR_EOF, 0);
        return;
    }
    if (c >= 0x80) {
        utf(top); // 解析并跳过utf8字符
        top->cols += 1;
        rch(top); // 读取下一个字符
        goto label_cont;
    }
    t = top->a.b128[c];
#if __CHCC_DEBUG__
    printf("line %04d col %03d %c %02x t %02x\n", top->line, top->cols, (t == CHAR_CLASS_BLANK) ? ' ' : (byte)c, c, t);
#endif
    if (t == CHAR_CLASS_BLANK) {
        if (c == CHAR_NEWLINE || c == CHAR_RETURN) {
            newline(top, c);
        } else {
            top->cols += 1;
        }
        rch(top);
        goto label_cont;
    }
    if (t == CHAR_CLASS_DIGIT || t == CHAR_CLASS_IDENT) {
        nch = idnum(top, (t == CHAR_CLASS_DIGIT));
    } else if (t == CHAR_CLASS_QUOTE) {
        nch = strlit(top, c);
    } else {
        nch = 1;
        if (t == CHAR_CLASS_SIGN) {
            t = (c == '+') ? CHAR_CLASS_PLUS : CHAR_CLASS_MINUS;
        }
        if (t == CHAR_CLASS_DOT) {
            t = CHAR_CLASS_MDOT;
        }
        if (t >= CHAR_CLASS_OPERATOR) {
            op = oper(top, c, t - CHAR_CLASS_OPERATOR);
        } else {
            goto label_punct;
        }
        if (op) {
            nch = op->len;
            if (!op->prior) {
                c = op->cfid;
                goto label_punct;
            }
            oper_end(top, op);
        } else {
label_punct:
            if (c == CIFA_PT_LINE_CMMT || c == CIFA_PT_BLOCK_CMMT) {
                nch = comment(top, c);
            } else {
                cifa_end(top, c, 0);
            }
        }
    }
    top->cols += nch;
    rch(top);
    if (cf->cfid == CIFA_TYPE_IDENT) {
        if (!ident(top)) {
            goto label_cont;
        }
    }
}

void start(chcc_t *cc)
{
    bufile_t *top = cc->top;
    top->error = null;
    top->haserr = false;
    top->line = 1;
    top->cols = 1;
    rch(top);
}

void next(chcc_t *cc)
{
    bufile_t *top = cc->top;
    cifa_t *cf = &top->cf; // cur 只更新 top->cf
    cur(top);
    if (cf->cfid == '@') {
        cur(top);
        if (cf->defvar) {
            cf->isvar = 0;
            cf->defvar = 0;
            cf->isattr = 1;
        } else {
            err(top, ERROR_INVALID_ATTR_NAME, 0);
        }
    }
    cc->cf = *cf;
}

void skip(chcc_t *cc, cfid_t id)
{
    cifa_t *cf = &cc->cf;
    if (cf->cfid != id) {
        err(cc->top, ERROR_SKIP_MATCH_FAILED, cf->cfid);
    }
    next(cc);
}

// 词法标识符会标记名称是类型名、常量名、包名、还是其他名称，当创建或引用对应标识符时，需要考虑是否要添加包名。
// 当创建一个全局标识符时：
//      1. 其标识符可能是类名、函数名、或包名
//      2. 该标识符必须使用包名前缀来创建其语法符号
//      3. 如果对应的符号已经存在，则是重复定义
//      4. 同一个包名只需导入其中所有全局导出符号一次，当导入包的别名时只需指向真实的包
// 当创建一个局部标识符时：
//      1. 标识符可以是结构体成员，结构体内嵌套的匿名结构体成员
//      2. 还可以是函数声明时的参数或返回值参数名称
//      3. 还可以是函数体内的局部变量声明
//      4. 还可以创建匿名的局部类型、匿名局部函数，局部函数包含函数指针类型声明或函数定义
//      4. 这些局部标识符都使用其本身的名称，不需要加包名
//      5. 一为了简化和统一化处理，当前作用域中的局部名称不能与任何其他名称重名
//      6. 局部名称也不可能与全局名称重名，因为全局名称包括包名都有 pkg~ 前缀，而局部名称不可能包含 ~ 字符
//      7. 统一化处理的原因是，结构体内嵌套的命名结构体不能与外部重名，标签名在一个函数体内也不能重名
//      8. 二是避免局部变量对前一层次同名变量的覆盖，导致程序可读性、歧义性、清晰性问题
// 当引用一个全局标识符时：
//      1. 通过包名语法引用的标识符相当于引用全局符号，因为导出符号都必定是全局的
//      2. 通过包名引用的标识符，必须使用包的真实名称前缀查找这个符号，不能使用任何别名
//      3. 不带包名对一个全局名称的引用，必须使用对应包的前缀查找这个符号
//      4. 除了类型指针、函数调用外，其他情况如果找不到这个引用的全局符号，需要报错
// 当引用一个局部标识符时：
//      1. 不需要添加包名前缀，必须要找到对应的局部符号，其 scope 的值必须大于 0
//      2. 可能需要对局部类型和局部函数做某些特殊处理
//
// 语法符号都保存在自己对应的作用域中，并随着作用域的切换动态变化，已经退出的作用域会释放其中的符号。哈希表中
// 的标识符会指向同名的语法符号，以方便检查语法符号是否存在重复定义。符号都保存在作用域中，作用域中的符号可以
// 是变量也可以是类型。值栈只能引用作用域中的符号，而且值栈只能引用变量符号，当退出作用域时对应的值栈也需要恢
// 复到原始状态。

void pushfile_(chcc_t *cc, file_t *f, bool dont_change_file_line)
{
    bufile_t *prev = cc->top;
    bufile_t *cur;
    if (!f) {
        return;
    }
    cur = (bufile_t *)stack_push(&cc->fstk, sizeof(bufile_t));
    cur->a = cc->prearr;
    cur->f = f;
    cc->top = cur;
    if (prev && dont_change_file_line) {
        cur->line = prev->line;
        cur->cols = prev->cols;
    }
    start(cc);
}

void pushstrtofile(chcc_t *cc, string_t s, bool dont_change_file_line)
{
    pushfile_(cc, file_load(s, 0), dont_change_file_line);
}

void pushfile(chcc_t *cc, const char *filename) // filename "-" 可以从标准输入读取
{
    pushfile_(cc, file_open(filename, 'r', 0), false);
}

void filestackfree(void *object)
{
    bufile_t *cur = (bufile_t *)object;
    buffer_free(&cur->s);
    file_close(cur->f);
}

void popfile(chcc_t *cc)
{
    stack_pop(&cc->fstk, filestackfree);
    cc->top = (bufile_t *)stack_top(&cc->fstk);
}

void replacefile_(chcc_t *cc, file_t *f)
{
    bufile_t *cur;
    stack_pop(&cc->fstk, filestackfree);
    if (f) {
        cur = (bufile_t *)stack_push(&cc->fstk, sizeof(bufile_t));
        cur->f = f;
        start(cc);
    }
}

void replacestrtofile(chcc_t *cc, string_t s)
{
    replacefile_(cc, file_load(s, 0));
}

void replacefile(chcc_t *cc, const char *filename)
{
    replacefile_(cc, file_open(filename, 'r', 0));
}

ident_t *findident(chcc_t *cc, cfid_t cfid)
{
    if (cfid < CIFA_IDENT_START || cfid >= CIFA_ANON_IDENT) {
        return null;
    }
    return (ident_t *)array_ex_at_n(cc->ident.arry_ident.a, cfid-CIFA_IDENT_START, sizeof(ident_t *));
}

ident_t *getrealident(ident_t *ident)
{   // 在当前作用域下，标识符创建的符号可能有另外一个真实的名称，当前的标识符只是它的别名
    symb_t *symb = getscopesym(ident);
    return symb ? symb->real : ident;
}

ident_t *findrealident(chcc_t *cc, cfid_t cfid)
{
    return getrealident(findident(cc, cfid));
}

ident_t *getpkgident(chcc_t *cc, cfid_t cfid)
{
    ident_t *ident = findident(cc, cfid);
    if (!ident) { return null; }
    return ident->pknm;
}

scope_t *curscope(chcc_t *cc)
{
    return (scope_t *)stack_top(&cc->scope);
}

bool pushscopesym(chcc_t *cc, symb_t *def_symb)
{
    ident_t *named = def_symb->name; // 全局符号必须是命名符号，定义符号defsym一定不是以包名前缀开头
    ident_t *gname;
    bool global = !cc->local;
    if (global) {
        if (named->glosym) {
            goto label_dup_defined;
        }
        gname = pushhashident_x(cc, cc->pknm->s, named->s, 0, true);
        if (!gname) { // 可以在已经定义局部符号的情况下定义一个pkg~name全局符号，但是局部符号会覆盖全局符号
            return false;
        }
        gname->defsym = gname->glosym = def_symb;
        if (def_symb->isbtype && named->id == CIFA_ID_ALIAS_NULL) {
            // null 定义的是 null~null 不能关联到 type~null
        } else {
            named->glosym = def_symb;
        }
        def_symb->real = gname;
        def_symb->cfid = gname->id;
    } else if (named) {
        if (named->defsym) {
label_dup_defined:
            errs(cc, ERROR_SYMB_DUP_DEFINED, named->s, 0);
            return false;
        }
        named->defsym = def_symb;
        def_symb->real = named;
        def_symb->cfid = named->id;
    } else {
        def_symb->cfid = cc->anon_id++;
    }
    stack_push_node(cc->sstk, (byte *)def_symb);
    return true;
}

symb_t *getscopesym(ident_t *ident)
{
    if (!ident) {
        return null;
    }
    if (ident->defsym) {
        return ident->defsym;
    }
    if (ident->glosym) {
        return ident->glosym;
    }
    return null;
}

symb_t *findscopesym(chcc_t *cc, cfid_t cfid)
{
    return getscopesym(findident(cc, cfid));
}

void freescopesym(symb_t *symb, bool global)
{
    ident_t *named = symb->name; // ??? symb->real 需要释放吗？
    if (global) {
        named->defsym = named->glosym = null;
    } else if (named) {
        named->defsym = null;
    }
}

void scopefree(void *object)
{
    scope_t *s = (scope_t *)object;
    symb_t *symb;
    while ((symb = stack_top(&s->symb))) {
        freescope_sym(symb, !s->local);
        stack_pop(&s->symb, null);
    }
}

void enterscope(chcc_t *cc)
{
    scope_t *prev = curscope(cc);
    scope_t *s = (scope_t *)stack_push(&cc->scope, sizeof(scope_t));
    s->local = prev->local + 1;
    cc->sstk = &s->symb;
    cc->local = s->local;
}

void leavescope(chcc_t *cc)
{
    scope_t *s;
    stack_pop(&cc->scope, scopefree);
    s = curscope(cc);
    cc->sstk = &s->symb;
    cc->local = s->local;
}

void popscopesym(chcc_t *cc, symb_t *last_symb, bool free_last_symb)
{
    stack_t *s;
    symb_t *symb;
    bool finish = false;
    if (!last_symb) { return; }
    for (; ;) {
        s = cc->sstk;
        while ((symb = stack_top(s))) {
            if (symb == last_symb) {
                if (!free_last_symb) { return; }
                finish = true;
            }
            freescopesym(symb, !cc->local);
            stack_pop(s, null);
            if (finish) { return; }
        }
        leavescope(cc);
    }
}

void pkgpredecl(chcc_t *cc, cfid_t cfid)
{
    ident_t *name = findident(cc, cfid);
    name->pknm = name;
}

void cstpredecl(chcc_t *cc, cfid_t cfid)
{
    ident_t *name = findident(cc, cfid);
    symb_t *symb;
    if (!name || cfid < CIFA_ID_ALIAS_NULL || cfid > CIFA_ID_ALIAS_FALSE) {
        return;
    }
    symb = (symb_t *)stack_new_node(sizeof(symb_t));
    symb->name = name;
    symb->isconst = 1;
    if (cfid == CIFA_ID_ALIAS_NULL) {
        if (cc->expose_prenull) {
            name->defsym = symb;
        }
    } else {
        if (cc->expose_prebool) {
            name->defsym = symb; 
        }
    }
    if (!pushscopesym(cc, symb)) {
        stack_free_node((byte *)symb);
    }
}

void btypedecl(chcc_t *cc, cfid_t cfid, uint32 size, byte align)
{
    symb_t *symb;
    ident_t *name = findident(cc, cfid);
    if (!name || cfid < CIFA_ID_ALIAS_INT || cfid > CIFA_ID_ALIAS_STRING) {
        return;
    }
    symb = (symb_t *)stack_new_node(sizeof(symb_t));
    symb->name = name;
    symb->size = size;
    symb->align = align;
    symb->istype = 1;
    symb->isbtype = 1;
    if (cfid != CIFA_ID_ALIAS_NULL && cc->expose_pretype) {
        name->defsym = symb; // null 定义的是 null~null 不能关联到 type~null
    }
    if (!pushscopesym(cc, symb)) {
        stack_free_node((byte *)symb);
    }
}

void basicdecl(chcc_t *cc)
{
    // 预定义包名
    pkgpredecl(cc, CIFA_ID_LANG_PKG);
    pkgpredecl(cc, CIFA_ID_TYPE_PKG);
    pkgpredecl(cc, CIFA_ID_BOOL_PKG);
    pkgpredecl(cc, CIFA_ID_NULL_PKG);
    pkgpredecl(cc, CIFA_ID_XDECL_PKG);

    // 基本类型
    cc->pknm = getpkgident(cc, CIFA_ID_TYPE_PKG);
    btypedecl(cc, CIFA_ID_ALIAS_INT, 4, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT8, 1, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT16, 2, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT32, 4, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT64, 8, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT128, 16, ALIGNOF_128BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT256, 32, ALIGNOF_256BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT512, 64, ALIGNOF_512BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_FLOAT, 8, ALIGNOF_64BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_FLOAT16, 2, ALIGNOF_16BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_FLOAT32, 4, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_FLOAT64, 8, ALIGNOF_64BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_COMPLEX, 2*8, ALIGNOF_64BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_COMPLEX16, 2*2, ALIGNOF_16BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_COMPLEX32, 2*4, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_COMPLEX64, 2*8, ALIGNOF_64BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_BOOL, 1, ALIGNOF_8BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_NULL, 1, ALIGNOF_8BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_BYTE, 1, ALIGNOF_8BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_RUNE, 4, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_INT96, SIZE_OF_POINTER, ALIGNOF_POINTER);
    btypedecl(cc, CIFA_ID_ALIAS_ERROT, 4, ALIGNOF_32BITS_TYPE);
    btypedecl(cc, CIFA_ID_ALIAS_STRING, 2*SIZE_OF_POINTER, ALIGNOF_POINTER);

    // 预定义常量
    cc->pknm = getpkgident(cc, CIFA_ID_NULL_PKG);
    cstpredecl(cc, CIFA_ID_ALIAS_NULL);
    cc->pknm = getpkgident(cc, CIFA_ID_BOOL_PKG);
    cstpredecl(cc, CIFA_ID_ALIAS_TRUE);
    cstpredecl(cc, CIFA_ID_ALIAS_FALSE);
}

void vstackinit(chcc_t *cc)
{
    cc->vtop = (synval_t *)stack_push(&cc->vstack, sizeof(synval_t));
}

synval_t *vtopvalid(chcc_t *cc, synval_t *prev)
{
    bufile_t *top = cc->top;
    synval_t *vtop = cc->vtop;
    if (top->haserr) {
        return null;
    }
    if ((synval_t *)stack_node_next((byte *)prev) != vtop) {
        err(top, ERROR_INVALID_VSTACK_TOP, 0);
        return null;
    }
    return vtop;
}

synval_t *vtop_valid_const(chcc_t *cc, synval_t *prev)
{
    bufile_t *top = cc->top;
    synval_t *vtop = cc->vtop;
    if (!vtopvalid(cc, prev)) {
        return null;
    }
    if (!vtop->symb.isconst) {
        err(top, ERROR_ISNOT_CONST_EXPR, 0);
        return null;
    }
    return vtop;
}

void vpush(chcc_t *cc, synval_t *node)
{
    stack_push_node(&cc->vstack, (byte *)node);
    cc->vtop = node;
}

void vpop(chcc_t *cc)
{
    stack_pop(&cc->vstack, null);
}

synval_t *vi(chcc_t *cc, uint32 i, symb_t *tsym)
{
    synval_t *synv = (synval_t *)stack_new_node(sizeof(synval_t));
    synv->val.c = i;
    synv->refs = tsym;
    synv->symb.isconst = 1;
    synv->symb.isbtype = 1;
    synv->symb.btype_i = 1;
    vpush(cc, synv);
    return synv;
}

synval_t *vi64(chcc_t *cc, uint64 i)
{
    synval_t *synv = (synval_t *)stack_new_node(sizeof(synval_t));
    synv->val.i64 = i;
    synv->refs = findscopesym(cc, CIFA_ID_INT64);
    synv->symb.isconst = 1;
    synv->symb.isbtype = 1;
    synv->symb.btype_i = 1;
    vpush(cc, synv);
    return synv;
}

synval_t *vf(chcc_t *cc, float f, symb_t *tsym)
{
    synval_t *synv = (synval_t *)stack_new_node(sizeof(synval_t));
    synv->val.f = f;
    synv->refs = tsym;
    synv->symb.isconst = 1;
    synv->symb.isbtype = 1;
    synv->symb.btype_f = 1;
    vpush(cc, synv);
    return synv;
}

synval_t *vstr(chcc_t *cc, string_t s)
{
    synval_t *synv = (synval_t *)stack_new_node(sizeof(synval_t) + s.len);
    if (s.len) {
        memcpy(synv + 1, s.a, s.len);
        synv->val.str = strflen((byte *)(synv + 1), s.len);
    }
    synv->refs = findscopesym(cc, CIFA_ID_STRING);
    synv->symb.isconst = 1;
    synv->symb.isbtype = 1;
    synv->symb.btype_s = 1;
    vpush(cc, synv);
    return synv;
}

void vlit_suffix(chcc_t *cc, synval_t *synv) // 1024phy~kg 12cm "abc"tag
{
    cifa_t *cf = &cc->cf;
    ident_t *suffix;
    symb_t *fsym;
    if (!cf->s.len) {
        return;
    }
    suffix = pushhashident(cc, cf->s, 0, true);
    if (!suffix) {
        return;
    }
    fsym = getscopesym(suffix);
    if (!fsym || !fsym->isfvar) {
        errs(cc->top, ERROR_INVALID_LIT_SUFFIX, cf->s, 0);
        return;
    }
    synv->post = fsym;
}

void vlit(chcc_t *cc)
{
    // 字面量（islit 不为 0）
    // isint: 1  val.c/i64   整数常量    numbase 基数   cf->s   临时后缀字符串      cfid = CIFA_ID_INT/INT64/BOOL/BYTE/RUNE/ERROR/NULL_TYPE
    // isfloat: 1  val.f   浮点常量    numbase 基数     cf->s   临时后缀字符串      cfid = CIFA_ID_FLOAT
    // 布尔常量    val.c   cfid = CIFA_ID_BOOL
    // 空值常量    val.c   cfid = CIFA_ID_NULL_TYPE
    // 字符常量    val.c   cfid = CIFA_ID_RUNE
    // 错误代码    val.c   cfid = CIFA_ID_ERROR
    // 字符串  isstr: 1    cfid = CIFA_ID_STRING       cf->val.str   临时字符串值
    cifa_t *cf = &cc->cf;
    synval_t *synv;
    if (cf->isstr) {
        synv = vstr(cc, cf->val.str);
    } else if (cf->isfloat) {
        synv = vf(cc, cf->val.f, findscopesym(cc, cf->cfid));
    } else if (cf->cfid == CIFA_ID_INT64) {
        synv = vi64(cc, cf->val.i64);
    } else {
        synv = vi(cc, cf->val.c, findscopesym(cc, cf->cfid));
    }
    vlit_suffix(cc, synv);
}

void vcst(chcc_t *cc, csym_t *csym, ident_t *ident) // 常量标识符
{
    bufile_t *top = cc->top;
    synval_t *synv;
    if (!csym) {
        errs(top, ERROR_SYMB_NOT_DEFINED, ident->s, 0);
        return;
    }
    if (!csym->symb.isconst) {
        errs(top, ERROR_INVALID_CONST_SYMB, ident->s, 0);
        return;
    }
    synv = (synval_t *)stack_new_node(sizeof(synval_t));
    synv->symb = csym->symb;
    synv->val = csym->val;
    synv->refs = csym->refs;
    vpush(cc, synv);
}

void venm(chcc_t *cc, csym_t *csym, ident_t *ident) // 常量枚举类型标识符
{
    csym_t *dest;
    struct slist_it* it;
    for (it = slist_begin(&csym->list); it; it = slist_next(it)) {
        dest = (csym_t *)slist_it_value(it);
        if (dest->symb.name == ident) {
            vcst(cc, dest, ident);
            return;
        }
    }
    errs(cc->top, ERROR_CONST_ENUM_NOT_FOUND, ident->s, 0);
}

void vvar(chcc_t *cc, ident_t *ident)
{
    synval_t *synv;
    symb_t *symb = getscopesym(ident);
    if (!symb || !symb->isvar) { // 变量也可能保存常量值
        errs(cc->top, ERROR_INVALID_VAR_SYMB, ident->s, 0);
        return;
    }
    synv = (synval_t *)stack_new_node(sizeof(synval_t));
    synv->symb = *symb;
    synv->refv = (vsym_t *)symb;
    synv->refs = ((vsym_t *)symb)->refs;
    vpush(cc, synv);
}

void vret(chcc_t *cc, fsym_t *f)
{
    cifa_t *cf = &cc->cf;
    bool rval = false;
    crlf(cc);
    next(cc);
    if (cf->cfid != ';') {
        if (!expr(cc, f, false)) {
            return;
        }
        rval = true;
    }
    // f->radr = (int96 *)gjmp(cc, (byte *)f->radr); // 跳转到函数返回之前
}

void gcvt(chcc_t *cc, vsym_t *v, vsym_t *t)
{

}

void setsymbptr(symb_t *symb)
{
    symb->size = SIZE_OF_POINTER;
    symb->align = ALIGNOF_POINTER;
    symb->ptrvar = 1;
}

void setsymbtype(symb_t *symb, symb_t *t)
{
    symb->size = t->size;
    symb->align = t->align;
    symb->ptrvar = 0;
}

bool setvarptr(chcc_t *cc, vsym_t *v, symb_t *t)
{
    v->refs = t;
    v->symb.size = SIZE_OF_POINTER;
    v->symb.align = ALIGNOF_POINTER;
    v->symb.ptrvar = 1;
    if (t && (t->isftype || t->isitype)) {
        ferr(cc->top, ERROR_INVALID_POINTER_TYPE, 0);
        return false;
    }
    return true;
}

bool setvartype(chcc_t *cc, vsym_t *v, symb_t *t)
{
    v->refs = t;
    v->symb.size = t->size;
    v->symb.align = t->align;
    v->symb.ptrvar = 0;
    return true;
}

void g2(vsym_t *a, vsym_t *b, uint32 op)
{

}

bool g1(chcc_t *cc, cfid_t op, vsym_t *tsym)
{
    bufile_t *f = cc->top;
    cifa_t *cf = &cc->cf;
    synval_t *top = cc->vtop;
    if (op == '(') {
        gcvt(cc, top, tsym);
    } else if (op == '*') {
        // top 应该是一个指针类型，如果是变量则它的地址已经加载到%eax中
        if (!top->symb.ptrvar) {
            ferr(f, ERROR_DREF_OP_WITH_UNPTR, 0);
            return false;
        }
        if (top->symb.ptrder) {
            err(f, ERROR_PTR_ALREADY_DEREFED, 0);
            return false;
        }
        setsymbtype(&top->symb, top->refs);
        top->symb.ptrder = 1;
        if (cf->cfid == '=') { // 赋值 *top = expr
            
        } else { // 读取 *top
            // mov (%eax),%eax 读取内存地址内容到%eax
            gldr();
        }
    } else if (op == '&') {
        // 将局部变量或全局变量的地址加载到%eax
        if (top->symb.ptrvar || top->symb.ptrder) {
            ferr(f, ERROR_CANT_GET_PTR_ADDR, 0); // 已经是指针或者已经解引用不能再取指针
            return false;
        }
        setsymbptr(cc, top);
        if (top->symb.isconst) {
            // 常量没有地址不能加载，只变成指针类型，后面只能被解引用之后才能再使用
        } else if (top->symb.isvar) {
            glea(top, 0); // 加载有效地址到%eax，lea EA,%eax
        } else {
            ferrs(f, ERROR_CANT_GET_VARADDR, top->symb.name, 0);
            return false;
        }
    } else if (op == '+') { // + - ^ !

    } else if (op == '-') {

    } else if (op == '^') {

    } else if (op == '!') {

    } else {
        ferr(top, ERROR_INVALID_UNARY_OPER, op);
    }
}

bool unary(chcc_t *cc, fsym_t *f, uintd_t begin_with_paren)
{
    cifa_t *cf = &cc->cf;
    cfid_t id = cf->cfid;
    ident_t *ident = cf->ident;
    symb_t *symb;
    vsym_t vsym;
    if (ident) {
        if (cf->cfid == CIFA_ID_RETURN) {
            vret(cc, f);
        } else if (cf->cfid == CIFA_ID_CONST) {
            if (cc->const_index) { // 在常量声明语句内部
                vi(cc, cc->const_index - 1, findscopesym(cc, CIFA_ID_INT));
            } else {
                decl(cc, f, null);
            }
        } else if (cf->istype) { // 类型标识符
            symb = getscopesym(ident);
            if (symb && symb->iscenum) {
                next(cc);
                skip(cc, '.');
                if (!cf->ident || !cf->defconst) {
                    errs(cc->top, ERROR_INVALID_CONST_MEMBER, ident->s, 0);
                    return false;
                }
                venm(cc, (csym_t *)symb, cf->ident);
            } else {
                errs(cc->top, ERROR_INVALID_TYPE_SYMB, ident->s, 0);
                return false;
            }
        } else if (cf->isconst) { // 常量标识符，常量不分配内存空间
            vcst(cc, (csym_t *)getscopesym(ident), ident);
        } else if (cf->isvar) { // 变量标识符，变量会分配内存空间
            next(cc);
            if (cf->cfid == CIFA_OP_INIT_ASSIGN) {
                decl(cc, f, ident);
            } else {
                vvar(cc, f, ident);
            }
            g1(cc, '@', null);
        } else {
            goto label_invalid;
        }
    } else if (cf->islit) {
        vlit(cc);
        buffer_free(&cc->top->s);
    } else if (cf->iscmm) {
        buffer_free(&cc->top->s);
        next(cc); // 忽略注释
        unary(cc, f, (begin_with_paren & 0x10));
    } else if (id == '(') {
        skip(cc, '(');
        if (reftype(cc, &vsym)) {
            skip(cc, ')');
            if (cf->cfid == '{') {
                initializer(cc, &vsym);
            } else {
                unary(cc, f, (begin_with_paren & 0x10));
                g1(cc, '(', &vsym);
            }
        } else {
            expr(cc, f, (begin_with_paren & 0x10) | 0x01);
            skip(cc, ')');
        }
    } else if (cf->optr) {
        if (cf->optr->unary) {
            next(cc);
            unary(cc, f, (begin_with_paren & 0x10));
            g1(cc, id, null);
        } else {
            ferr(cc->top, ERROR_INVALID_UNARY_OPER, id);
        }
    } else {
label_invalid:
        return false;
    }
    for (; ;) {
        id = cf->cfid;
        if (id == '.') {

        } else if (id == '[') {

        } else if (id == '(') {

        } else {
            break;
        }
    }
}

// a || b + c * d
// cf --> a ||              : expr_logic(1)
// cf --> a || b            : expr_logic(1) --> unary() --> vtop记录b的地址
// cf --> a || b +          : expr_logic(1) --> vtop记录b的地址，加号优先级大于||递归调用expr_infix(2)先计算加号
// cf --> a || b + c        : expr_logic(1) --> expr_infix(2)
// cf --> a || b + c *      : expr_logic(1) --> expr_infix(2) 乘法优先级大于加号，递归调用expr_infix(+1)先计算乘法
// cf --> a || b + c * d    : expr_logic(1) --> expr_infix(2) --> expr_infix(+1)
// cf --> a || b + cd       : expr_logic(1) --> expr_infix(2)
// cf --> a || bcd          : expr_logic(1)
void expr_logic(chcc_t *cc, fsym_t *f, ops_t *op)
{
    cifa_t *cf = &cc->cf;
    cfid_t oper = op->cfid;
    bool jmp_when_true = (oper == CIFA_OP_LOR);
    byte *a = 0;
    for (; ;) {
        a = gjcc(cc, jmp_when_true, a); // 对于||如果为真跳转，对于&&如果为假跳转，跳转到grel
        skip(cc, oper);
        unary(cc, f, 0x10);
        if (cf->oper > op->prior) {
            expr_infix(cc, f, 0x10, op->prior + 1);
        }
        if (cf->cfid != oper) {
            break;
        }
    }
    if (a) {
        a = gjcc(cc, jmp_when_true, a); // 对于||如果为真跳转，对于&&如果为假跳转，跳转到grel
        gi(cc, !jmp_when_true);         // 否则对于||结果%eax为假或继续下一个条件，对于&&结果%eax为真或继续下一个条件
        gjmp(cc, (byte *)5);            // 跳过以下5个字节代码
        grel(cc->text, (int96 *)a);    // 修正跳转地址，跳转到此处下一行立即数结果
        gi(cc, jmp_when_true);          // 对于||结果%eax为真，对于&&结果%eax为假
    }
}

// a + b - c || d || e && f * g
// cf --> a                 : unary()       --> vtop记录符号a的地址
// cf --> a +               : expr_infix(1) --> unary()
// cf --> a + b             :               --> unary() --> vtop记录符号b的地址
// cf --> a + b -           :               --> vtop记录符号b的地址 --> 减号的优先级不大于加号，计算gop(+, a, b)，减号大于||的优先级继续循环
// cf --> ab - c            :               --> unary() --> vtop记录符号c的地址
// cf --> ab - c ||         :               --> vtop记录符号c的地址，||的优先级不大于减号，可以计算减号，gop(-, ab, c)，||大于等于||的优先级继续循环
// cf --> abc || d          :               --> unary() --> vtop记录符号d的地址
// cf --> abc || d &&       :               --> vtop记录符号d的地址，&&的优先级大于||，递归调用expr_infix(2)
// cf --> abc || d && f     :               --> expr_infix(2) --> unary() --> vtop记录符号f的地址
// cf --> abc || d && f *   :               --> expr_infix(2) --> vtop记录符号f的地址，乘号优先级大于&&，递归嗲用expr_infix(3)
// cf --> abc || d && f * g :               --> expr_infix(2) --> expr_infix(3) --> unary() --> vtop记录g的地址，未读到操作符计算gop(*, f, g)
// cf --> abc || d && fg    :               --> expr_infix(2) --> 乘法计算完毕，计算gop(&&, d, fg)
// cf --> abc || dfg        :               --> &&计算完毕，计算gop(||, abc, dfg)
void expr_infix(chcc_t *cc, fsym_t *f, uintd_t begin_with_paren, uint32 prior)
{
    cifa_t *cf = &cc->cf;
    ops_t *op;
    while (cf->oper >= prior) {
        op = cf->optr;
        prior = cf->oper;
        if (cf->cfid == CIFA_OP_LOR || cf->cfid == CIFA_OP_LAND) {
            expr_logic(cc, f, op);
        } else {
            next(cc);
            unary(cc, f, 0x10 | begin_with_paren);
            if (cf->oper > prior) {
                expr_infix(cc, f, 0x10 | begin_with_paren, prior + 1);
            }
            gop(op);
        }
    }
}

bool assign_op(cfid_t cfid)
{
    return cfid == CIFA_OP_ASSIGN || (cfid > CIFA_OP_INIT_ASSIGN && cfid < CIFA_OP_END_ASSIGN);
}

bool middle_assign_shall_quote_in_paren(uintd_t begin_with_paren)
{
    return !(begin_with_paren & 0x10) || (begin_with_paren & 0x01);
}

bool islvalue(chcc_t *cc, vsym_t *v)
{
    if (!v->symb.islval) {
        err(cc->top, ERROR_SHALL_BE_LVALUE, 0);
        return false;
    }
    return true;
}

void expr_assign(chcc_t *cc, fsym_t *f, uintd_t begin_with_paren)
{
    cifa_t *cf = &cc->cf;
    vsym_t *vtop = cc->vtop;
    unary(cc, f, begin_with_paren);
    expr_infix(cc, f, begin_with_paren, 2);
    if (!(vtop = vtopvalid(cc, vtop))) {
        return;
    }
    if (assign_op(cf->cfid)) {
        ops_t *oper = cf->optr;
        if (!islvalue(cc, vtop)) {
            return;
        }
        next(cc);
        if (cf->cfid == CIFA_OP_ASSIGN) {
            expr_assign(cc, f, false); // 单纯赋值
        } else { // 处理常量表达式，变量也可能赋值常量
            vdup(cc);
            expr_assign(cc, f, false);
            gop(cc, oper); // 先二元操作再赋值
        }
        gmov(cc); // 赋值，栈顶赋值给次顶
    }
}

bool expr(chcc_t *cc, fsym_t *f, uintd_t begin_with_paren)
{
    cifa_t *cf = &cc->cf;
    vsym_t *vtop = cc->vtop;
    expr_assign(cc, f, begin_with_paren);
    if (!(vtop = vtopvalid(cc, vtop))) {
        return false;
    }
    while (cf->cfid == ',') {
        vpop(cc);
        next(cc);
        expr_assign(cc, f, 0x10 | begin_with_paren);
        if (!(vtop = vtopvalid(cc, vtop))) {
            return false;
        }
    }
    return true;
}

bool block(chcc_t *cc, fsym_t *f, int96 *b) // 语句块
{
    cifa_t *cf = &cc->cf;
    cfid_t cfid = cf->cfid;
    byte *text = cc->text;
    uint32 loc = f->loc;
    bool succ = false;
    int96 *a, *n;
    if (cfid == CIFA_ID_IF) {
        // ifstmt = "if" { expr ";" } expr block [ "else" (ifstmt | block) ] .
        next(cc);
label_if_expr:
        if (!expr(cc, f, false)) {
            return false;
        }
        if (cf->cfid == ';') {
            vpop(cc);
            next(cc);
            goto label_if_expr;
        }
        if (cf->cfid != '{') {
            vpop(cc);
            goto label_false;
        }
        a = gjmp_z(cc, 0);  // if 0 需要跳转的地址
        if (!block(cc, f, b)) {
            goto label_false;
        }
        f->loc = loc;
        if (cf->cfid == CIFA_ID_ELSE) {
            next(cc);
            if (cf->cfid != '{' && cf->cfid != CIFA_ID_IF) {
                goto label_false;
            }
            n = gjmp(cc, 0);        // 这条指令只有上面的if为1时才执行，这里需要跳过整个else块
            grel(cc->text, a);      // 到这里是整个if块的结束，写入if正确的跳转地址
            if (!block(cc, f, b)) {    // else 语句块
                goto label_false;
            }
            f->loc = loc;
            grel(cc->text, n);      // 这里是else块的结束，写入else正确的跳转地址
        } else {
            grel(cc->text, a); // 如果没有else则结束if，写入if正确的跳转地址
        }
    } else if (cfid == CIFA_ID_FOR) {

    } else if (cfid == '{') {
        enterscope(cc);
        next(cc);
        while (cf->cfid != '}') {
            block(cc, f, b);
        }
        skip(cc, '}');
        leavescope(cc);
    } else if (cfid == CIFA_ID_GOTO) {

    } else if (cfid == CIFA_ID_BREAK) {

    } else if (cfid == CIFA_ID_CONTINUE) {

    } else if (cfid == CIFA_ID_DEFER) {

    } else if (cfid != ';') { // 解析一个表达式
        if (!expr(cc, f, false)) {
            goto label_false;
        }
    }
    return true;
label_false:
    cc->text = text;
    f->loc = loc;
    return false;
}

fsym_t *fsymalloc(void)
{
    return stack_new_node(sizeof(fsym_t));
}

void fsyminit(fsym_t *f, int96 addr)
{
    f->v.symb.istype = 1;
    f->v.symb.isftype = 1;
    if (addr) {
        f->v.symb.isvar = 1;
        f->v.symb.isfvar = 1;
    }
    if (f->recv) {
        f->v.symb.align = ALIGNOF_POINTER;
        f->v.symb.size = sizeof(fobj_t);
    } else {
        f->v.symb.align = ALIGNOF_POINTER;
        f->v.symb.size = SIZE_OF_POINTER;
    }
    f->v.refs = &f->v.symb;
    f->v.addr = addr;
}

void fsymfree(fsym_t *f)
{
    slist_free(&f->para, null);
    slist_free(&f->retp, null);
    stack_free_node((byte *)f);
}

bool reftype(chcc_t *cc, vsym_t *v) // 使用类型
{
    // 必须以类型名或struct/interface/func开头
    cifa_t *cf = &cc->cf;
    ident_t *name = cf->ident;
    symb_t *tsym = getscopesym(name);
    if (!cf->istype) {
        return false;
    }
    next(cc);
    if (cf->cfid == '*') {
        next(cc);
        if (!setvarptr(cc, v, tsym)) {
            return false;
        }
    } else {
        if (!tsym) {
            errs(cc->top, ERROR_TYPE_NOT_DEFINED, name->s, 0);
            return false;
        }
        setvartype(cc, v, tsym);
    }
    return true;
}

uint32 pushvar(slist_t *l, vsym_t *v, ident_t *name, uint32 offset)
{
    v->symb.name = name;
    v->addr = round_up(offset, v->symb.align);
    *(vsym_t *)slist_push_back(l, sizeof(vsym_t)) = *v;
    return v->addr + v->symb.size;
}

bool typevar(chcc_t *cc, slist_t *l, rune term, rune t2) // 类型变量列表
{
    cifa_t *cf = &cc->cf;
    vsym_t *t = null;
    bool prev_is_type = false;
    uint32 offset = 0;
    vsym_t vsym;
label_loop:
    if (cf->cfid == term || (t2 && cf->cfid == t2)) {
        if (prev_is_type) {
            pushvar(l, t, null, offset);
        }
        return true;
    }
    if (reftype(cc, &vsym)) {
        if (prev_is_type) {
            offset = pushvar(l, t, null, offset);
        }
        t = &vsym;
        prev_is_type = true;
    } else {
        if (!cf->defvar) {
            return false;
        }
        if (!t) {
            errs(cc, ERROR_VAR_WITHOUT_TYPE, cf->ident->s, 0);
            next(cc);
            return false;
        }
        offset = pushvar(l, t, cf->ident, offset);
        prev_is_type = false;
    }
    next(cc);
    goto label_loop;
}

fsym_t *func_syn(chcc_t *cc, ident_t *dest)
{
    cifa_t *cf = &cc->cf;
    struct slist_it *it;
    fsym_t *f = fsymalloc();
    uint32 len = 0;
    vsym_t *v;
    next(cc);
    if (cf->istype) {
        f->recv = cf->ident;
        next(cc);
    }
    if (cf->defvar) {
        f->v.symb.name = cf->ident;
        next(cc);
    }
    skip(cc, '(');
    if (!typevar(cc, &f->para, CIFA_ID_RETURN, ')')) {
        err(cc->top, ERROR_FUNC_INVALID_PARAM, cf->cfid);
    }
    if (cf->cfid == CIFA_ID_RETURN) {
        next(cc);
        if (!typevar(cc, &f->retp, ')', 0)) {
            err(cc->top, ERROR_FUNC_INVALID_RPARA, cf->cfid);
        }
    }
    skip(cc, ')');
    while (cf->isattr) {
        // f->v.symb.flags |= cf->attr;
        next(cc);
    }
    f->v.symb.body = (cf->cfid == '{');
    if (cc->top->haserr) {
        fsymfree(f);
        return null;
    }
    for (it = slist_begin(&f->para); it != slist_end(&f->para); it = slist_next(it)) {
        v = (vsym_t *)slist_it_get(it);
        v->addr += 8; // 函数参数在函数返回地址以及返回值地址之后
        if (v->symb.size < SIZE_OF_POINTER) {
            v->symb.size = SIZE_OF_POINTER;
        }
        len += v->symb.size;
    }
    f->plen = 8 + len;
    len = 0;
    for (it = slist_begin(&f->retp); it != slist_end(&f->para); it = slist_next(it)) {
        v = (vsym_t *)slist_it_get(it);
        len += v->symb.size;
    }
    f->rlen = len;
    if (cc->local) {
        f->dest = dest;
    }
    return f;
}

bool func_gen(chcc_t *cc, fsym_t *f)
{
    ident_t *name = f->v.symb.name;
    ident_t *fglo = null;
    uint32 loc;
    // 必须先创建函数符号，因为可以递归调用
#if 0
    if (cc->local) {
        f->v.symb.name = f->dest; // 局部函数总使用赋值目标变量名为名称
    }
#endif
    if (!name) {
        err(cc, ERROR_GLOBAL_FUNC_NONAME, 0);
        goto label_false;
    }
    if (!pushscopesym(cc, &f->v.symb)) {
        goto label_false;
    }
    cc->text = round_up_addr(cc->text, sizeof(uint96)-1);
    fsyminit(f, (int96)cc->text);
    // 进入函数作用域并生成代码
    loc = genter(cc, f);
    if (!block(cc, loc, 0)) {
        goto label_false;
    }
    gret(cc, f);
    f->clen = cc->text - f->v.addr;
    popscopesym(cc, &f->v.symb, false);
    return true;
label_false:
    cc->text = f->v.addr;
    popscopesym(cc, &f->v.symb, true);
    return false;
}

bool func_type_decl(chcc_t *cc, fsym_t *f)
{

}

csym_t *csymalloc(void)
{
    return (csym_t *)stack_new_node(sizeof(csym_t));
}

void csymfree(csym_t *csym)
{
    slist_free(&csym->cstval, null);
    stack_free_node((byte *)csym);
}

void cstvaladd(csym_t *csym, ident_t *name, vsym_t *v)
{
    vsym_t *vsym = (vsym_t *)slist_push_back(&csym->cstval, sizeof(vsym_t));
    *vsym = *v;
    vsym->symb.name = name;
}

bool get_cst_expr(chcc_t *cc, string_t *out)
{
    bufile_t *top = cc->top;
    buffer_t *s = &top->s;
    buffix_t *b = &top->f->b;
    uint32 paren = 1;
    rune c;
    buffer_clear(s);
    top->start = b->cur - 1; // 包含开始'('
    for (; ;) {
        rch_ex(cc, cpstr);
        c = top->c;
        if (c == '(') {
            paren += 1;
        } else if (c == ')') {
            paren -= 1;
            if (paren == 0) {
                break;
            }
        } else if (c == CHAR_NEWLINE || c == CHAR_RETURN) {
            newline(cc, c);
        } else if (c == CHAR_EOF) {
            return false;
        }
    }
    if (s->len) {
        buffer_push(s, top->start, b->cur - top->start, CFSTR_ALLOC_EXPAND);
        *out = strflen(s->a, s->len);
    } else {
        *out = strfend(top->start, b->cur);
    }
    top->cols += out->len - 1; // 开始'('已经计算
    return true;
}

csym_t *cst_syn(chcc_t *cc, fsym_t *f)
{
    cifa_t *cf = &cc->cf;
    csym_t *csym = csymalloc(cc);
    vsym_t *vtop = cc->vtop;
    string_t cstexp = findident(cc, CIFA_ID_CONST)->s;
    symb_t *tsym;
    ident_t *name;
    next(cc);
    if (cf->defconst) {
        name = cf->ident;
        next(cc);
        if (cf->cfid != '=') {
            err(cc->top, ERROR_CONST_DECL_MISSING_EQ, 0);
            goto label_false;
        }
        next(cc);
        expr(cc, f, 0x10);
        if (!(vtop = vtop_valid_const(cc, vtop))) {
            goto label_false;
        }
        csym->v.refs = vtop->refs; // 常量的类型
        cstvaladd(csym, name, vtop);
        vpop(cc);
    } else if (cf->deftype) {
        csym->v.symb.name = cf->ident; // 常量枚举类型名称
        next(cc);
        if (cf->cfid != '{') {
            err(cc->top, ERROR_CONST_TYPE_MISSING_CURLY, 0);
            goto label_false;
        }
        goto label_const_list;
    } else if (cf->cfid == '{') {
label_const_list:
        next(cc);
        tsym = getscopesym(cf->ident);
        if (tsym && tsym->btype_i) {
            csym->v.refs = tsym;
            next(cc);
        } else {
            csym->v.refs = findscopesym(cc, CIFA_ID_INT);
        }
        cc->const_index = 1;
        while (cf->defconst) {
            name = cf->ident;
            next(cc);
            if (cf->cfid == '(') {
                if (!get_cst_expr(cc, &cstexp)) {
                    err(cc->top, ERROR_INVALID_CONST_EXPR, 0);
                    goto label_false;
                }
                skip(cc, ')');
            }
            pushstrtofile(cc, cstexp, true);
            expr(cc, f, 0x10);
            if (!(vtop = vtop_valid_const(cc, vtop))) {
                popfile(cc);
                goto label_false;
            }
            gcast(cc, tsym);
            cstvaladd(csym, name, vtop);
            vpop(cc);
            popfile(cc);
            cc->const_index += 1;
        }
        cc->const_index = 0;
        if (cf->cfid != '}') {
            err(cc->top, ERROR_INVALID_CONST_NAME, 0);
            goto label_false;
        }
        skip(cc, '}');
    } else {
        err(cc->top, ERROR_INVALID_CONST_SYNTAX, cf->cfid);
        goto label_false;
    }
    return csym;
label_false:
    cc->const_index = 0;
    csymfree(csym);
    return null;
}

void cst_gen(chcc_t *cc, csym_t *csym)
{
    struct slist_it *it;
    bool haserr = false;
    vsym_t *first = null;
    vsym_t *v; // 局部常量只能在函数内局部使用，全局常量如果没有声明为私有则会被导出
    if (!csym) { return; }
    if (csym->v.symb.name) { // 声明这个常量枚举类型
        csym->v.symb.istype = 1;
        csym->v.symb.iscenum = 1;
        if (!pushscopesym(cc, &csym->v.symb)) {
            csymfree(csym);
        }
    } else { // 声明列表中所有的常量
        while ((v = (vsym_t *)slist_pop_front_node(&csym->cstval))) {
            v->symb.isconst = 1;
            if (pushscopesym(cc, &v->symb)) {
                if (!first) { first = v; }
            } else {
                haserr = true;
                break;
            }
        }
        if (haserr && first) {
            popscopesym(cc, &first->symb, true);
        }
        csymfree(csym);
    }
}

vsym_t *vsymalloc(void)
{
    return (vsym_t *)stack_new_node(sizeof(vsym_t));
}

void vsymfree(vsym_t *v)
{
    stack_free_node((byte *)v);
}

symb_t *decl(chcc_t *cc, fsym_t *f, ident_t *dest) // 类型声明，函数声明，变量声明、标签声明
{
    cifa_t *cf = &cc->cf;
    cfid_t cfid = cf->cfid;
    bool succ = false;
    if (cfid == CIFA_ID_PACKAGE) {

    } else if (cfid == CIFA_ID_IMPORT) {

    } else if (cfid == CIFA_ID_CONST) {
        csym_t *csym = cst_syn(cc, f);
        cst_gen(cc, f, csym);
        return null;
    } else if (cfid == CIFA_ID_TYPE) {

    } else if (cfid == CIFA_ID_STRUCT) {

    } else if (cfid == CIFA_ID_INTERFACE) {

    } else if (cfid == CIFA_ID_FUNC) {
        fsym_t *fsym = func_syn(cc, dest);
        if (!fsym) {
            return null;
        }
        if (fsym->v.symb.body) {
            succ = func_gen(cc, fsym);
        } else {
            succ = func_type_decl(cc, fsym);
        }
        if (!succ) {
            fsymfree(fsym);
            return null;
        }
        return &fsym->v.symb;
    } else if (cfid == CIFA_OP_INIT_ASSIGN) {
        vsym_t *vsym = vsymalloc();
        vsym_t *vtop;
        if (!vsym) {
            return null;
        }
        vsym->symb.name = dest;
        vsym->symb.isvar = 1;
        vsym->symb.islval = 1;
        if (!pushscopesym(cc, &vsym->symb)) {
            vsymfree(vsym);
            return null;
        }
        vpush(cc, vsym);
        vtop = vsym;
        expr_assign(cc, f, 0x10);
        if (!(vtop = vtopvalid(cc, vtop))) {
            popscopesym(cc, vsym, true);
            return false;
        }
        vsym->addr = round_up(f->loc, vtop->symb.align);
        f->loc += vtop->symb.size;
        gmov(cc); // 赋值，栈顶赋值给次顶
        return &vsym->symb;
    }
}

void scopeinit(chcc_t *cc)
{
    cc->gsym = (scope_t *)stack_push(&cc->scope, sizeof(scope_t));
    cc->sstk = &cc->gsym->symb;
    cc->local = 0;
}

void chccinit(chcc_t *cc)
{
    const byte *p, *c;
    ident_t *sym = 0;
    prearr_t *prearr = &cc->prearr;
    hashident_t *a = &cc->ident;
    byte predecl_ident[] = {
#define PREDECL(id, ...) __VA_ARGS__, 0x00,
#include "chcc/decl.h"
        0x00
    };

    memset(cc, 0, sizeof(chcc_t));

    cifa_b128(prearr);
    cifa_esc(prearr);
    cifa_ops(prearr);
    prearr->hash = a;

    bhash_init(&a->hash_ident, IDENT_HASH_SIZE);
    array_ex_init(&a->arry_ident, sizeof(ident_t*), IDENT_ARRAY_EXPAND);

    p = predecl_ident; // 创建预定义的标识符
    while (*p) {
        c = p;
        while (*c) ++c;
        sym = pushhashident(a, strfend(p, c), 0, true);
        p = c + 1;
    }

    cc->user_id_start = sym->id + 1;
    cc->anon_id = CIFA_ANON_IDENT;

    scopeinit(cc);
    vstackinit(cc);

    // 添加预声明名称，例如：type~int 以及别名 int 需要自动添加到哈希表和标识符数组，
    // 并且 int 需要指向 type~int；null~null bool~true 以及别名 null/true/false 需
    // 要自动添加到哈希表和标识符数组，并且后者指向前者
    basicdecl(cc);
}

void chccfree(chcc_t *cc)
{
    prearr_t *a = &cc->prearr;
    hashident_t *h = a->hash;
    bhash_free(&h->hash_ident, ident_free);
    array_ex_free(&h->arry_ident);
    stack_free(&cc->fstk, filestackfree);
    stack_free(&cc->scope, scopefree);
    stack_free(&cc->vstack, null);
    free(a->ops);
    free(a->esc);
    free(a->b128);
}
