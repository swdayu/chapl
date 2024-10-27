#define __CURR_FILE__ STRID_CHCC_CIFA
#include "internal/decl.h"
#include "chcc/cifa.h"

#define FILEBUF_HEAD_BYTE_CNT 7 // 能向后回退字节数，不小于一次 f->c_len 的长度
#define FILEBUF_TAIL_BYTE_CNT 1 // EOB

bufile_t *bufile_new(const char *filename, string_t s, Int filebufsize)
{
    bufile_t *f;
    buffix_t *b;
    int fd;
    Int cap = (filebufsize > 0) ? filebufsize : 1;
    if (filename) {
        if (strcmp(filename, "-") == 0) {
            fd = 0; // stdin
        } else {
            fd = open(filename, O_RDONLY | O_BINARY);
            if (fd < 0) return nil;
        }
    } else {
        cap = ((Int)s.len > cap) ? s.len : cap;
        fd = -1;
    }
    cap += FILEBUF_HEAD_BYTE_CNT + FILEBUF_TAIL_BYTE_CNT;
    f = (bufile_t *)malloc(sizeof(bufile_t) + cap);
    if (f) {
        memset(f, 0, sizeof(bufile_t));
        f->fd = fd;
        b = &f->b;
        buffer_init(&f->cpstr, 0);
        buffix_init_inplace(b, cap);
        if (filename) {
            b->cur += FILEBUF_HEAD_BYTE_CNT;
            f->cur_c = *(b->cur) = CIFA_CH_EOB;
            f->c_len = f->c_cnt = 1;
            f->line_num = 1;
            f->col_bytes = 1;
            f->col_chars = 1;
        } else {
            bufile_load_string(f, s);
        }
    }
    return f;
}

void bufile_load_string(bufile_t *f, string_t s)
{
    buffix_t *b = &f->b;
    byte *arr = buffix_data(b);
    Int len = (Int)b->cap - (FILEBUF_HEAD_BYTE_CNT + FILEBUF_TAIL_BYTE_CNT);
    if (f->fd != -1) {
        return;
    }
    if ((Int)s.len < len) {
        len = (Int)s.len;
    }
    b->cur = arr + FILEBUF_HEAD_BYTE_CNT;
    if (len > 0) {
        memcpy(b->cur, s.a, len);
    } else {
        len = 0;
    }
    *(b->cur + len) = CIFA_CH_EOF;
    b->cur -= 1; // 将cur设置到前一个字符
    f->cur_c = *(b->cur) = CIFA_CH_EOB;
    f->c_len = f->c_cnt = 1;
    f->line_num = 1;
    f->col_bytes = 1;
    f->col_chars = 1;
}

void bufile_delete(bufile_t *f)
{
    if (f) {
        if (f->fd > 0) {
            close(f->fd);
            f->fd = -1;
        }
        buffer_free(&f->cpstr);
        free(f);
    }
}

void cfys_reset(cfys_t *cf, bufile_t *f, rune c)
{
    memset(cf, 0, sizeof(cfys_t));
    cf->val.i = c;
    cf->cf_line = f->line_num;
    cf->cf_col = f->col_chars - f->c_cnt + 1;
}

void cfsym_free(void *object)
{
    cfsym_t *sym = (cfsym_t *)object;
    string_free(&sym->ident);
}

bool cfsym_eq(const void *object, const void *cmp_para)
{
    cfsym_t *sym = (cfsym_t *)object;
    string_t *str = (string_t *)cmp_para;
    return (sym->ident.len == str->len) && (memcmp(sym->ident.a, str->a, str->len) == 0);
}

#define IDENT_HASH_INIT 1
#define IDENT_HASH_SIZE (8*1024) // 必须是2的幂
#define IDENT_ARRAY_EXPAND 512
#define CFSTR_ALLOC_EXPAND 128

uint32 ident_hash_func(uint32 h, rune c)
{
    return ((h) + ((h) << 5) + ((h) >> 27) + (c));
}

cfsym_t *cifa_ident_create(cfst_t *cfst, uint32 hash, bool calc, string_t ident)
{
    array_ex_t *arry_ident = cfst->arry_ident.a;
    bhash_t *hash_ident = cfst->hash_ident.a;
    cfsym_t *sym;
    Uint i = 0;
    bool exist = false;

    if (calc) {
        hash = IDENT_HASH_INIT;
        for (; i < ident.len; i++) {
            hash = ident_hash_func(hash, ident.a[i]);
        }
    }

    // 将标识符符号创建到符号哈希表中
    sym = (cfsym_t *)bhash_push(hash_ident, hash, cfsym_eq, &ident, sizeof(cfsym_t), &exist);
    if (!exist) {
        // 该符号在哈希表中不存在，返回的是新创建的符号，这里初始化这个符号
        string_move_init(&sym->ident, &ident);
        // 将符号添加到符号数组中，并初始化该符号在数组中的序号
        sym->ident_id = ((uint32)CIFA_ID_START) + 1 + array_ex_len(arry_ident);
        array_ex_push(&cfst->arry_ident, (byte *)sym, IDENT_ARRAY_EXPAND);
    }

#if __CHCC_DEBUG__
    printf("ident %08x exist %d id %08x [%d] %s\n", hash, exist, sym->ident_id, sym->ident.len, sym->ident.a);
#endif

    return sym;
}

void cifa_end(cfys_t *curcf, cfid_t cfid, Error error)
{
    curcf->cfid = cfid;
    if (error) {
        curcf->error = error;
    }
}

void cifa_ident_end(cfst_t *cfst, cfsym_t *sym)
{
    cfys_t *curcf = &cfst->curcf;
    curcf->val.sym = sym;
    curcf->s = sym->ident;
    cifa_end(curcf, sym->ident_id, nil);
}

typedef struct {
    uint64 i;
    uint64 f;
    uint32 exp;
    uint8 is_float: 1;
    uint8 f_digit: 1;
    uint8 exp_neg: 1;
} numval_t;

void cifa_numlit_end(cfst_t *cfst, const numval_t *val)
{
    cfys_t *curcf = &cfst->curcf;
    cfid_t cfid;
    if (val->is_float) {
        cfid = CIFA_CF_FLOAT_LIT;
    } else {
        cfid = CIFA_CF_INT_LIT;
        curcf->val.i = val->i;
    }
    curcf->s = string_ref_buffer(&cfst->f->cpstr); // 数值字面量后缀
    cifa_end(curcf, cfid, nil);
}

void cifa_nil_end(cfst_t *cfst)
{
    cfys_t *curcf = &cfst->curcf;
    curcf->val.i = 0;
    cifa_end(curcf, CIFA_CF_NIL_LIT, nil);
}

void cifa_bool_end(cfst_t *cfst, bool a)
{
    cfys_t *curcf = &cfst->curcf;
    curcf->val.i = a;
    cifa_end(curcf, CIFA_CF_BOOL_LIT, nil);
}

void cifa_cmmt_end(cfst_t *cfst, rune cmmt, Error error)
{
    cfys_t *curcf = &cfst->curcf;
    curcf->s = string_ref_buffer(&cfst->f->cpstr);
    cifa_end(curcf, CIFA_CF_COMMENT, error);
}

void cifa_runelit_end(cfst_t *cfst, rune c, Error error)
{
    cfys_t *curcf = &cfst->curcf;
    curcf->val.i = c;
    cifa_end(curcf, CIFA_CF_RUNE_LIT, error);
}

void cifa_strlit_end(cfst_t *cfst, rune quote, Error error)
{
    cfys_t *curcf = &cfst->curcf;
    curcf->s = string_ref_buffer(&cfst->f->cpstr);
    cifa_end(curcf, CIFA_CF_STR_LIT, error);
}

static const byte predecl_ident_g[] = {
#define PREDECL(id, ...) __VA_ARGS__, 0x00,
#include "chcc/decl.h"
    0x00
};

void cfst_init(cfst_t *cfst, bufile_t *f)
{
    const byte *p, *c;
    cfsym_t *sym = 0;

    memset(cfst, 0, sizeof(cfst_t));

    cfst->f = f;
    bhash_init(&cfst->hash_ident, IDENT_HASH_SIZE);
    array_ex_init(&cfst->arry_ident, sizeof(cfsym_t*), IDENT_ARRAY_EXPAND);

    // 创建预定义的标识符
    p = predecl_ident_g;
    while (*p) {
        c = p;
        while (*c) ++c;
        sym = cifa_ident_create(cfst, 0, true, string_create(p, c - p, 0));
        p = c + 1;
    }

    cfst->user_ident_start = sym->ident_id + 1;
}

void cfst_free(cfst_t *cfst)
{
    bhash_free(&cfst->hash_ident, cfsym_free);
    array_ex_free(&cfst->arry_ident);
}

void more(bufile_t *f)
{
    int len = 0;
    buffix_t *b = &f->b;
    byte *arr = buffix_data(b);
    Uint cap = buffix_cap(b);
    if (cap > (FILEBUF_HEAD_BYTE_CNT + FILEBUF_TAIL_BYTE_CNT)) {
        len = read(f->fd, arr + FILEBUF_HEAD_BYTE_CNT,
            cap - (FILEBUF_HEAD_BYTE_CNT + FILEBUF_TAIL_BYTE_CNT));
    }
    if (len > 0) {
        arr[FILEBUF_HEAD_BYTE_CNT+len] = CIFA_CH_EOB;
    } else {
        arr[FILEBUF_HEAD_BYTE_CNT] = CIFA_CH_EOF;
    }
    b->cur = arr + FILEBUF_HEAD_BYTE_CNT;
}

#define cur_c_last_byte(f) ((f)->b.cur)
#define cur_c_begin_ptr(f) ((f)->b.cur - (f)->c_len + 1) // 保证 c_len 大于 0

rune next_c(bufile_t *f, bool cpstr)
{
    buffix_t *b = &f->b;
    byte *arr = buffix_data(b);
    byte *curstr = f->curstr;
    rune c = *cur_c_last_byte(f);
    if (c == CIFA_CH_EOF) {
        arr[FILEBUF_HEAD_BYTE_CNT] = CIFA_CH_EOF;
        b->cur = arr + FILEBUF_HEAD_BYTE_CNT;
    } else {
        if (c == CIFA_CH_EOB && f->fd >= 0) {
label_read_more:
            if (cpstr) {
                buffer_push(&f->cpstr, curstr, cur_c_begin_ptr(f)-curstr, CFSTR_ALLOC_EXPAND);
            }
            more(f);
            f->curstr = b->cur;
        } else {
            b->cur += 1; // b->cur 指向当前字符最后一个字节，前进一步是下一个字符
            if (*b->cur == CIFA_CH_EOB) {
                goto label_read_more; // next_c 不会返回 CIFA_CH_EOB
            }
        }
    }
    // 开始解析 b->cur 指向的字符，待办：处理 utf-8 字符
    f->c_len = f->c_cnt = 1;
    return (f->cur_c = *b->cur);
}

#define CIFA_HEX_DIGIT 0x01
#define CIFA_BIN_DIGIT '1'
#define CIFA_DEC_DIGIT '9'

static bool cifa_digit(rune c)
{
    return (c >= '0' && c <= '9');
}

static bool cifa_hex_digit(rune c)
{
    return (c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'F') ||
        (c >= 'a' && c <= 'f');
}

bool cifa_num_digit(rune c, byte digtp)
{
    if (digtp == CIFA_HEX_DIGIT) {
        return cifa_hex_digit(c);
    }
    return c >= '0' && c <= digtp;
}

byte cifa_dig_val(rune c)
{
    if (c >= 'A' && c <= 'F') {
        return 10 + (byte)(c - 'A');
    } else if (c >= 'a' && c <= 'f') {
        return 10 + (byte)(c - 'a');
    } else {
        return (byte)c - '0';
    }
}

static bool cifa_ident_begin(rune c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c == '_');
}

static bool cifa_ident_cont(rune c)
{
    return cifa_ident_begin(c) || cifa_digit(c);
}

static Uint newline(bufile_t *f, rune c, bool *rdnext);
static Uint identifier(cfst_t *cfst, rune c, bool *rdnext);
static Uint comment(cfst_t *cfst, rune c, bool *rdnext);
static Uint num_lit(cfst_t *cfst, rune c, bool *rdnext);
static Uint str_lit(cfst_t *cfst, rune quote, bool *rdnext);
static Uint rune_lit(cfst_t *cfst, bool *rdnext);

// 解析当前词法
void curr(cfst_t *cfst)
{
    byte b128[128] = {CHAR_128_BYTE_ARRAY_G};
    rune c, c2, c3;
    byte c_class;
    cfid_t cfid;
    Uint colrd;
    bufile_t *f = cfst->f;
    cfys_t *curcf = &cfst->curcf;
    bool skipcf, rdnext;

label_loop:
    c = f->cur_c;
    if (c < 0x80) {
        c_class = b128[c];
    } else {
        c_class = CHAR_CLASS_OTHERBYTE;
    }

#if __CHCC_DEBUG__
    printf("line %04d col %03d %c %02x class %02x\n", f->line_num, f->col_chars,
        (c_class == CHAR_CLASS_OTHERBYTE || c_class == CHAR_CLASS_NEWLINE) ? ' ' : (byte)c, c, c_class);
#endif

    if (c == CIFA_CH_EOB) {
        c = next_c(f, false);
        goto label_loop;
    }

    cfys_reset(curcf, f, c);
    skipcf = false;
    rdnext = true;
    colrd = f->c_cnt;
    cfid = c;

    if (c == CIFA_CH_EOF) {
        cifa_end(curcf, CIFA_CH_EOF, nil);
        return;
    }

    switch (c_class) {
    case CHAR_CLASS_OTHERBYTE:  // 0x01
        if (c < 0x80) {
            skipcf = true; // 当空白字符忽略
        } else {
            // 待办：处理utf8字符
        }
        break;
    case CHAR_CLASS_DIGIT:      // 0x02 0 ~ 9
        colrd = num_lit(cfst, c, &rdnext);
        goto label_cifa_parsed;
    case CHAR_CLASS_IDENT:      // 0x03 A ~ Z a ~ z _
        colrd = identifier(cfst, c, &rdnext);
        goto label_cifa_parsed;
    case CHAR_CLASS_SINGLE:     // 0x05 # $ @ ( ) [ ] { } , ; 反斜杠
        if (c == '\\') {
            skipcf = true; // 忽略反斜杠字符
        }
        break;
    case CHAR_CLASS_NEWLINE:    // 0x06 \r \r\n \n
        skipcf = true;
        newline(f, c, &rdnext);
        colrd = 0;
        goto label_cifa_parsed;
    case CHAR_CLASS_QUOTE:      // 0x0B
        // '    CIFA_CH_RUNE_Q
        // "    CIFA_CH_STR_Q
        // `    CIFA_CH_RSTR_Q
        if (c == CIFA_CH_RUNE_Q) {
            colrd = rune_lit(cfst, &rdnext);
        } else {
            colrd = str_lit(cfst, c, &rdnext);
        }
        goto label_cifa_parsed;
    case CHAR_CLASS_DOT: // 0x0C
        // .    CIFA_CH_DOT
        // ..   CIFA_OP_2DOT
        // ...  CIFA_OP_3DOT
        // .1   numeric lit 为了清晰性不支持以点号开始的浮点数
        c2 = next_c(f, false);
        if (c2 == '.') {
            c3 = next_c(f, false);
            if (c3 == '.') {
                cfid = CIFA_OP_3DOT;
                colrd = 3;
            } else {
                rdnext = false;
                cfid = CIFA_OP_2DOT;
                colrd = 2;
            }
        } else {
            rdnext = false; // CIFA_CH_DOT
        }
        break;
    case CHAR_CLASS_QMARK: // 0x0E ? ?= ?>
    case CHAR_CLASS_TILDE: // 0x0F ~ ~= ~>
        cfid = 0;
        break;
    // CHAR_CLASS_EQ_FLAG 0x10 ~ 0x1F:
    case CHAR_CLASS_SLASH: // 0x13
        // /    CIFA_CH_DIV
        // /=   CIFA_OP_DIV_ASSIGN
        // //   CIFA_OP_DOUBLE_SLASH
        // /*   CIFA_OP_SLASH_ASTER
        c2 = next_c(f, false);
        if (c2 == '/' || c2 == '*') {
            colrd = comment(cfst, c2, &rdnext);
            goto label_cifa_parsed;
        } else {
            if (c2 == '=') {
                cfid = CIFA_OP_DIV_ASSIGN;
                colrd = 2;
            } else {
                rdnext = false; // CIFA_CH_DIV
            }
        }
        break;
    case CHAR_CLASS_ASTER: // 0x14
        // *    CIFA_CH_MUL     CIFA_CH_DREF        CIFA_CH_POINTER
        // *=   CIFA_OP_MUL_ASSIGN
        // */   CIFA_OP_ASTER_SLASH
        // **   CIFA_OP_POW     CIFA_OP_POINPOINTER
        // **=  CIFA_OP_POW_ASSIGN
        c2 = next_c(f, false);
        if (c2 == '*') {
            c3 = next_c(f, true);
            if (c3 == '=') {
                cfid = CIFA_OP_POW_ASSIGN;
                colrd = 3;
            } else {
                rdnext = false;
                cfid = CIFA_OP_POW;
                colrd = 2;
            }
        } else if (c2 == '/') {
            cfid = CIFA_OP_ASTER_SLASH;
            colrd = 2; // 未配对的块注释，待办：报错
        } else {
            if (c2 == '=') {
                cfid = CIFA_OP_MUL_ASSIGN;
                colrd = 2;
            } else {
                rdnext = false; // CIFA_CH_MUL
            }
        }
        break;
    default:
        if (c_class & CHAR_CLASS_EQ_FLAG) {
            // 0x15 ~ 0x1F
            // CHAR_CLASS_LT        <       CIFA_OP_LT
            //                      <=      CIFA_OP_LE
            //                      <<      CIFA_OP_LSH
            //                      <<=     CIFA_OP_LSH_ASSIGN
            // CHAR_CLASS_GT        >       CIFA_OP_GT
            //                      >=      CIFA_OP_GE
            //                      >>      CIFA_OP_RSH
            //                      >>=     CIFA_OP_RSH_ASSIGN
            // CHAR_CLASS_COLON     :       CIFA_OP_COLON
            //                      :=      CIFA_OP_INI_ASSIGN
            // CHAR_CLASS_EMARK     !       CIFA_OP_NOT
            //                      !=      CIFA_OP_NE
            // CHAR_CLASS_PLUS      +       CIFA_OP_ADD
            //                      +=      CIFA_OP_ADD_ASSIGN
            // CHAR_CLASS_MINUS     -       CIFA_OP_SUB
            //                      -=      CIFA_OP_SUB_ASSIGN
            //                      ->      CIFA_OP_ARROW
            // CHAR_CLASS_PERCENT   %       CIFA_OP_MOD
            //                      %=      CIFA_OP_MOD_ASSIGN
            // CHAR_CLASS_AND       &       CIFA_OP_AND
            //                      &=      CIFA_OP_AND_ASSIGN
            //                      &&      CIFA_OP_LAND
            // CHAR_CLASS_VERTBAR   |       CIFA_OP_BOR
            //                      |=      CIFA_OP_BOR_ASSIGN
            //                      ||      CIFA_OP_LOR
            // CHAR_CLASS_CARET     ^       CIFA_OP_XOR     CIFA_OP_COMPL
            //                      ^=      CIFA_OP_XOR_ASSIGN
            // CHAR_CLASS_EQUAL     =       CIFA_OP_ASSIGN
            //                      ==      CIFA_OP_EQ
            c2 = next_c(f, false);
            if (c2 == '=') {
                cfid = c + CIFA_OP_BASE;
                colrd = 2;
                break;
            }
            if (c == '-') {
                // ->   CIFA_OP_ARROW
                if (c2 == '>') {
                    cfid = CIFA_OP_ARROW;
                    colrd = 2;
                    break;
                }
            }
            if (c == '&' || c == '|') {
                // &&   CIFA_OP_LAND
                // ||   CIFA_OP_LOR
                if (c2 == c) {
                    cfid = (CIFA_OP_BASE+CIFA_OP_AND_OFF(c));
                    colrd = 2;
                    break;
                }
            } else if (c == '<' || c == '>') {
                // <<   CIFA_OP_LSH
                // <<=  CIFA_OP_LSH_ASSIGN
                // >>   CIFA_OP_RSH
                // >>=  CIFA_OP_RSH_ASSIGN
                if (c2 == c) {
                    c3 = next_c(f, false);
                    if (c3 == '=') {
                        cfid = (CIFA_OP_BASE+c-CIFA_OP_SHASS_OFF);
                        colrd = 3;
                    } else {
                        rdnext = false;
                        cfid = (CIFA_OP_BASE+c-CIFA_OP_SHIFT_OFF);
                        colrd = 2;
                    }
                    break;
                }
            }
            rdnext = false;
        }
        break;
    }

    if (!skipcf) {
        cifa_end(curcf, cfid, nil);
    }

label_cifa_parsed:
    f->col_chars += colrd;
    if (rdnext) {
        next_c(f, false);
    }
    if (skipcf) {
        goto label_loop;
    }
}

static Uint newline(bufile_t *f, rune c, bool *rdnext)
{
    // EOL - Windows (CR LF)    \r\n    0x0d 0x0a
    //     - Unix (LF)          \n      0x0a
    //     - Macintosh (CR)     \r      0x0d
    Uint colrd = 1;
    if (c == '\r')
    {
        c = next_c(f, false);
        if (c == '\n') {
            // Windows \r\n
            colrd = 2;
        } else {
            // Macintosh \r
            *rdnext = false;
        }
    }
    ++f->line_num;
    f->col_chars = 1;
    f->col_bytes = 1;
    return colrd;
}

static Uint identifier(cfst_t *cfst, rune c, bool *rdnext)
{
    uint32 h = IDENT_HASH_INIT;
    bufile_t *f = cfst->f;
    buffer_t *cpstr = &f->cpstr;
    cfsym_t *cfsym;
    Uint colrd;
    buffer_clear(cpstr);
    f->curstr = cur_c_begin_ptr(f); // 当前字符是标识符的开始
    h = ident_hash_func(h, c);
    for (; ;) {
        c = next_c(f, true);
        if (cifa_ident_cont(c)) {
            h = ident_hash_func(h, c);
        } else {
            *rdnext = false;
            buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
            break; // 标识符读取完毕
        }
    }
    colrd = cpstr->len;
    if (cpstr->len == 3 && memcmp(cpstr->a, "nil", 3) == 0) {
        cifa_nil_end(cfst);
    } else if (cpstr->len == 4 && memcmp(cpstr->a, "true", 4) == 0) {
        cifa_bool_end(cfst, true);
    } else if (cpstr->len == 5 && memcmp(cpstr->a, "false", 5) == 0) {
        cifa_bool_end(cfst, false);
    } else {
        cfsym = cifa_ident_create(cfst, h, false, string_ref_buffer(cpstr));
        cifa_ident_end(cfst, cfsym);
    }
    return colrd;
}

static Uint comment(cfst_t *cfst, rune c, bool *rdnext)
{
    // //   CIFA_OP_DOUBLE_SLASH
    // /*   CIFA_OP_SLASH_ASTER
    // */   CIFA_OP_ASTER_SLASH
    bufile_t *f = cfst->f;
    buffer_t *cpstr = &f->cpstr;
    Uint colrd = 2; // // or /*
    Error error = nil;
    buffer_clear(cpstr);
    f->curstr = f->b.cur + 1; // 注释从下一个字符开始
    if (c == '/') {
        for (; ;) {
            c = next_c(f, true);
            if (c == CIFA_CH_EOF || c == CIFA_CH_RETURN || c == CIFA_CH_NEWLINE) {
                buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
                *rdnext = false;
                break; // 行注释读取完毕
            }
        }
        colrd += cpstr->len;
        cifa_cmmt_end(cfst, CIFA_OP_DOUBLE_SLASH, nil);
    } else {
        rune c2;
        bool readnext = true;
        for (; ;) {
            if (readnext) {
                c = next_c(f, true);
            }
            readnext = true;
            if (c == CIFA_CH_EOF || c == CIFA_CH_RETURN || c == CIFA_CH_NEWLINE) {
                buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
                if (c == CIFA_CH_EOF) {
                    error = ERROR_CMMT_NOT_CLOSED;
                    log_error_2(error, f->line_num, f->col_chars + colrd);
                    break; // 块注释语法错误，缺少块注释结束词法
                } else {
                    newline(f, c, &readnext);
                    colrd = 0;
                    buffer_put(cpstr, '\n', CFSTR_ALLOC_EXPAND);
                    if (readnext) {
                        f->curstr = f->b.cur + 1;
                    } else {
                        f->curstr = cur_c_begin_ptr(f);
                        c = f->cur_c;
                    }
                }
            } else if (c == '*') {
                c2 = next_c(f, true);
                if (c2 == '/') {
                    buffer_push(cpstr, f->curstr, f->b.cur + 1 - f->curstr, CFSTR_ALLOC_EXPAND);
                    buffer_pop(cpstr, 2); // */ 不属于注释内容
                    colrd += 2; // */
                    break; // 块注释读取完毕
                } else {
                    colrd += 1;
                    readnext = false;
                    c = c2;
                }
            } else {
                colrd += f->c_cnt;
            }
        }
        cifa_cmmt_end(cfst, CIFA_OP_SLASH_ASTER, error);
    }
    return colrd;
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
// dec_exp = ("p" | "P") ["+" | "-"] dec_digits .
// dec_float_lit =
//      dec_lit "." dec_digits |
//      dec_lit "." dec_digits dec_exp |
//      dec_lit "." dec_exp |
//      dec_lit dec_exp .
// bin_float_lit =
//      bin_lit "." bin_digits |
//      bin_lit "." bin_digits dec_exp |
//      bin_lit "." dec_exp |
//      bin_lit dec_exp .
// hex_float_lit =
//      hex_lit "." hex_digits |
//      hex_lit "." hex_digits dec_exp |
//      hex_lit "." dec_exp |
//      hex_lit dec_exp .

static Uint num_tail(cfst_t *cfst, rune c)
{
    bufile_t *f = cfst->f;
    buffer_t *cpstr = &f->cpstr;
    Uint colrd = 0;
    while (cifa_digit(c)) { // 忽略掉数字，例如非二进制数字
        colrd += 1;
        c = next_c(f, false);
    }
    buffer_clear(cpstr);
    f->curstr = cur_c_begin_ptr(f); // 当前字符是标识符的开始
    if (cifa_ident_begin(c)) {
        for (; ;) {
            c = next_c(f, true);
            if (!cifa_ident_cont(c)) {
                buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
                break; // 标识符读取完毕
            }
        }
    }
    return colrd + cpstr->len;
}

static Uint exp_part(cfst_t *cfst, numval_t *out)
{
    bufile_t *f = cfst->f;
    uint32 exp = 0;
    Uint colrd = 0;
    rune c = next_c(f, false);
    bool neg = false;
    out->is_float = 1;
    if (c == '+' || c == '-') {
        colrd += 1;
        neg = (c == '-');
        c = next_c(f, false);
    }
    for (; ;) {
        if (cifa_digit(c)) {
            exp = exp * 10 + (c - '0');
        } else if (c != '_') {
            break; // 指数读取完毕
        }
        colrd += 1;
        c = next_c(f, false);
    }
    out->exp = exp;
    out->exp_neg = neg;
    colrd += num_tail(cfst, c);
    return colrd;
}

static Uint frag_part(cfst_t *cfst, byte digtp, numval_t *out)
{
    bufile_t *f = cfst->f;
    uint64 fpart = 0;
    Uint colrd = 0;
    byte shf = 0;
    byte digval;
    rune c = next_c(f, false);
    out->is_float = 1;
    out->f_digit = digtp;
    for (; ;) {
        if (cifa_num_digit(c, digtp)) {
            digval = cifa_dig_val(c);
            if (shf < 64) {
                fpart |= (digval << shf);
                if (digtp == CIFA_BIN_DIGIT) {
                    shf += 1;
                } else {
                    shf += 4;
                }
            }
        } else if (c == 'p' || c == 'P') {
            colrd += 1;
            out->f = fpart;
            colrd += exp_part(cfst, out);
            return colrd;
        } else if (c != '_') {
            break; // 小数读取完毕
        }
        colrd += 1;
        c = next_c(f, false);
    }
    out->f = fpart;
    colrd += num_tail(cfst, c);
    return colrd;
}

static Uint int_part(cfst_t *cfst, rune c, numval_t *out)
{
    bufile_t *f = cfst->f;
    uint64 i = 0;
    Uint colrd = 0;
    byte digval;
    byte digtp;
    if (c == '0') {
        colrd += 1;
        c = next_c(f, false);
        if (c == '_' || c == '.' || c == 'p' || c == 'P' || cifa_digit(c)) {
            digtp = CIFA_DEC_DIGIT; // dec_lit
            goto label_parse_digit;
        } else if (c == 'b' || c == 'B') {
            colrd += 1;
            c = next_c(f, false);
            digtp = CIFA_BIN_DIGIT; // bin_lit
            goto label_parse_digit;
        } else if (c == 'x' || c == 'X') {
            colrd += 1;
            c = next_c(f, false);
            digtp = CIFA_HEX_DIGIT; // hex_lit
            goto label_parse_digit;
        }
        // 整数读取完毕
    } else {
        digtp = CIFA_DEC_DIGIT; // dec_lit
label_parse_digit:
        for (; ;) {
            if (cifa_num_digit(c, digtp)) {
                digval = cifa_dig_val(c);
                if (digtp == CIFA_HEX_DIGIT) {
                    i = (i << 4) + digval;
                } else if (digtp == CIFA_BIN_DIGIT) {
                    i = (i << 1) + digval;
                } else {
                    i = i * 10 + digval;
                }
            } else if (c == '.') {
                colrd += 1;
                out->i = i;
                colrd += frag_part(cfst, digtp, out);
                return colrd;
            } else if (c == 'p' || c == 'P') {
                colrd += 1;
                out->i = i;
                out->f = 0;
                colrd += exp_part(cfst, out);
                return colrd;
            } else if (c != '_') {
                break; // 整数读取完毕
            }
            colrd += 1;
            c = next_c(f, false);
        }
    }
    out->i = i;
    colrd += num_tail(cfst, c);
    return colrd;
}

static Uint num_lit(cfst_t *cfst, rune c, bool *rdnext)
{
    numval_t v = {0};
    Uint colrd = 0;
    colrd = int_part(cfst, c, &v);
    cifa_numlit_end(cfst, &v);
    *rdnext = false;
    return colrd;
}

#define CIFA_MAX_UNICODE 0x0010FFFF
#define CIFA_INVALID_UTF 0xFFFFFFFF

static Uint unicode_to_utf8(uint32 u, byte *p)
{
    // 00000000 … 0000007F  -> 0xxxxxxx
    // 00000080 … 000007FF  -> 110xxxxx 10xxxxxx
    // 00000800 … 0000FFFF  -> 1110xxxx 10xxxxxx 10xxxxxx
    // 00010000 … 0010FFFF  -> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
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
        *p++ = 0xF0 | ((u >> 18) & 0x07);
        *p++ = 0x80 | ((u >> 12) & 0x3F);
        *p++ = 0x80 | ((u >> 6) & 0x3F);
        *p = 0x80 | (u & 0x3F);
        return 4;
    }
    p[0] = p[1] = p[2] = p[3] = CIFA_CH_EOF;
    return 4;
}

static uint32 utf8_to_unicode(const byte *p)
{
    uint32 u = CIFA_INVALID_UTF;
    byte c = *p;
    if (c < 0x80) {
        return c;
    }
    if (c <= 0xDF) {
        return ((c & 0x1F) << 6) | ((*++p) & 0x3F);
    }
    if (c <= 0xEF) {
        return ((c & 0x0F) << 12) | (((*++p) & 0x3F) << 6) | ((*p) & 0x3F);
    }
    if (c <= 0xF7) {
        u = ((c & 0x07) << 18) | (((*++p) & 0x3F) << 12) |
            (((*++p) & 0x3F) << 6) | ((*p) & 0x3F);
    }
    return (u > CIFA_MAX_UNICODE) ? CIFA_INVALID_UTF : u;
}

typedef struct {
    rune u;
    Uint esclen;
    Uint colrd;
    Error error;
} escchar_t;

static escchar_t escchar(cfst_t *cfst, rune quote, Uint colrd, bool *rdnext)
{
    // 转义字符：
    // \a \b \f \n \r \t \v \\ \' \"
    // \0 … \9
    // \x00 … \xff
    // \u0000 … /uffff
    // \U00000000 … /Uffffffff
    // 其他转移字符报编译错误
    escchar_t r = {.u = 0, .esclen = 1, .colrd = colrd + 2, .error = nil};
    bufile_t *f = cfst->f;
    Uint i = 0;
    rune c;
    bool readnext = true;
    c = next_c(f, false);
    if (c == 'a') {
        r.u = '\a';
    } else if (c == 'b') {
        r.u = '\b';
    } else if (c == 'f') {
        r.u = '\f';
    } else if (c == 'n') {
        r.u = '\n';
    } else if (c == 'r') {
        r.u = '\r';
    } else if (c == 't') {
        r.u = '\t';
    } else if (c == 'v') {
        r.u = '\v';
    } else if (c == '\\' || c == '\'' || c == '\"') {
        r.u = c;
    } else if (cifa_digit(c)) {
        r.u = c - '0';
    } else if (c == 'x') {
        r.esclen = 2;
label_read_hex:
        for (; i < r.esclen; ++i) {
            if (readnext) {
                c = next_c(f, false);
            }
            if (cifa_hex_digit(c)) {
                r.u = (r.u << 4) | cifa_dig_val(c);
                r.colrd += 1;
                continue;
            }
            r.u = (r.u << 4); // 将不识别的X当成数字0
            if (c == CIFA_CH_EOF || c == CIFA_CH_RETURN || c == CIFA_CH_NEWLINE || c == CIFA_CH_BSLASH || c == quote) {
                readnext = false;
                if (i == (r.esclen - 1)) {
                    *rdnext = false;
                    r.error = ERROR_INVALID_HEXNUMB;
                }
            } else {
                r.colrd += f->c_cnt;
                r.error = ERROR_INVALID_HEXCHAR;
                log_error_3(r.error, f->line_num, f->col_chars + r.colrd, c);
            }
        }
        if (r.u > CIFA_MAX_UNICODE) {
            r.error = ERROR_INVALID_UNICODE;
            c = r.u;
        }
    } else if (c == 'u') {
        r.esclen = 4;
        goto label_read_hex;
    } else if (c == 'U') {
        r.esclen = 8;
        goto label_read_hex;
    } else {
        r.error = ERROR_INVALID_ESCCHAR;
        r.u = CIFA_CH_SPACE; // 忽略当前不识别的转义字符，并解析成空格
    }
    if (r.error && r.error != ERROR_INVALID_HEXCHAR) {
        log_error_3(r.error, f->line_num, f->col_chars + r.colrd, c);
    }
    return r;
}

static Uint rune_lit(cfst_t *cfst, bool *rdnext)
{
    // '    CIFA_CH_RUNE_Q
    bufile_t *f = cfst->f;
    rune c, prev = 0;
    escchar_t r = {.u = 0, .colrd = 1};
    Error error = nil;
    bool readnext = true;
    c = next_c(f, false);
    if (c == CIFA_CH_BSLASH) {
        r = escchar(cfst, CIFA_CH_RUNE_Q, r.colrd, &readnext);
        if (r.error) {
            error = r.error;
        }
        if (!readnext) {
            if (f->cur_c == CIFA_CH_RUNE_Q) {
                goto label_rune_quote;
            } else if (f->cur_c == CIFA_CH_BSLASH) {
                goto label_skip_remain;
            } else {
                goto label_end_early;
            }
        }
        goto label_end_quote;
    } else if (c == CIFA_CH_RETURN || c == CIFA_CH_NEWLINE || c == CIFA_CH_EOF) {
        error = ERROR_MISS_CLOSE_QUOTE;
label_end_early:
        *rdnext = false;
        // 遇到结束引号前到达行尾或文件尾，字符字面量读取完毕
    } else if (c == CIFA_CH_RUNE_Q) {
        error = ERROR_EMPTY_RUNE_LIT;
label_rune_quote:
        r.colrd += 1;
        // 空字符字面量
    } else {
        r.colrd += f->c_cnt;
        r.u = c;
label_end_quote:
        c = next_c(f, false);
        if (c == CIFA_CH_RUNE_Q) {
            r.colrd += 1;
        } else {
            error = ERROR_MULT_CHAR_EXIST;
        }
    }
    if (error) {
        log_error_3(error, f->line_num, f->col_chars + r.colrd, r.u);
    }
    if (error == ERROR_MULT_CHAR_EXIST) {
        if (c == CIFA_CH_RETURN || c == CIFA_CH_NEWLINE || c == CIFA_CH_EOF) {
            error = ERROR_MISS_CLOSE_QUOTE;
        }
label_skip_remain:
        for (; ;) { // 跳过字符字面量中多余的字符
            if (c == CIFA_CH_RETURN || c == CIFA_CH_NEWLINE || c == CIFA_CH_EOF) {
                *rdnext = false;
                break; // 遇到行尾或文件尾结束
            } else if (c == CIFA_CH_RUNE_Q && prev != CIFA_CH_BSLASH) {
                r.colrd += 1;
                break; // 遇到真正的单引号结束
            }
            r.colrd += f->c_cnt;
            prev = c;
            c = next_c(f, false);
        }
    }
    cifa_runelit_end(cfst, r.u, error);
    return r.colrd;
}

static Uint str_lit(cfst_t *cfst, rune quote, bool *rdnext)
{
    // "    CIFA_CH_STR_Q
    // `    CIFA_CH_RSTR_Q
    bufile_t *f = cfst->f;
    buffer_t *cpstr = &f->cpstr;
    Uint colrd = 1; // quote
    byte utf8[8];
    Uint ulen;
    escchar_t r;
    Error error = nil;
    bool readnext = true;
    rune c = 0;
    buffer_clear(cpstr);
    f->curstr = f->b.cur + 1; // 从下一个字符开始
    for (; ;) {
        if (readnext) {
            c = next_c(f, false);
        }
        readnext = true;
        if (c == CIFA_CH_BSLASH) {
            if (quote == CIFA_CH_RSTR_Q) {
                colrd += 1;
            } else {
                buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
                r = escchar(cfst, quote, colrd, &readnext);
                colrd = r.colrd;
                if (r.error) {
                    error = r.error;
                }
                if (r.esclen <= 2) {
                    buffer_put(cpstr, (byte)r.u, CFSTR_ALLOC_EXPAND);
                } else {
                    ulen = unicode_to_utf8(r.u, utf8);
                    buffer_push(cpstr, utf8, ulen, CFSTR_ALLOC_EXPAND);
                }
                if (readnext) {
                    f->curstr = f->b.cur + 1;
                } else {
                    f->curstr = cur_c_begin_ptr(f);
                    c = f->cur_c;
                }
            }
        } else if (c == CIFA_CH_RETURN || c == CIFA_CH_NEWLINE) {
            if (quote == CIFA_CH_RSTR_Q) {
                buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
                newline(f, c, &readnext);
                colrd = 0;
                buffer_put(cpstr, '\n', CFSTR_ALLOC_EXPAND);
                if (readnext) {
                    f->curstr = f->b.cur + 1;
                } else {
                    f->curstr = cur_c_begin_ptr(f);
                    c = f->cur_c;
                }
            } else {
                buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
                error = ERROR_MISS_CLOSE_QUOTE;
                *rdnext = false;
                break; // 遇到结束引号前到达行尾，字符串读取完毕
            }
        } else if (c == CIFA_CH_EOF) {
            buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
            error = ERROR_MISS_CLOSE_QUOTE;
            break; // 遇到结束引号前到达文件尾，字符串读取完毕
        } else {
            colrd += f->c_cnt;
            if (c == quote) {
                buffer_push(cpstr, f->curstr, cur_c_begin_ptr(f) - f->curstr, CFSTR_ALLOC_EXPAND);
                break; // 字符串读取完毕
            }
        }
    }
    if (error) {
        log_error_3(error, f->line_num, f->col_chars + colrd, c);
    }
    cifa_strlit_end(cfst, quote, error);
    return colrd;
}
