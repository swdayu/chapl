#define __CURR_FILE__ STRID_CHCC_YUFA
#include "internal/decl.h"
#include "chcc/chcc.h"

void cifa_init(chcc_t *cc, bufile_t *f);
bool type_lit(chcc_t *cc, cfsym_t *named, tpcur_t *out);

sym_t *sym_push_it(chcc_t *cc, stack_t *s, struct stack_it *type_sym, cfsym_t *named)
{
    sym_t *sym = (sym_t *)stack_it_get(type_sym);
    cfid_t anon_id = named ? 0 : (CIFA_ID_ANON_START + cc->anon_id++);
    if (anon_id >= CIFA_ID_ANON_LAST) {
        cc->yferr = ERROR_TOO_MANY_ANON_SYM;
        return null;
    }
    sym->id = named ? named->id : anon_id;
    sym->scope = cc->scope;
    if (named) {
        stack_t *refs;
        uintptr *addr;
        if (sym->type_kind) {
            refs = &named->u.named_type;
        } else if (sym->is_field) {
            refs = &named->u.field_sym;
        } else if (sym->is_func) {
            refs = &named->u.func_sym;
        } else if (sym->is_method) {
            refs = &named->u.method;
        } else {
            refs = &named->u.ident_sym;
        }
        addr = (uintptr *)stack_push(refs, sizeof(sym_t *));
        if (!addr) {
            cc->yferr = ERROR_OUT_OF_MEMORY;
            return null;
        }
        *addr = (uintptr)sym;
    }
    stack_push_it(s, type_sym);
    return sym;
}

sym_t *sym_push(chcc_t *cc, cfsym_t *named, uint32 scope, stack_t *s, Int objsize, stack_t *refs)
{
    sym_t *sym;
    uintptr *addr;
    cfid_t anon_id = named ? 0 : (CIFA_ID_ANON_START + cc->anon_id++);
    if (anon_id >= CIFA_ID_ANON_LAST) {
        cc->yferr = ERROR_TOO_MANY_ANON_SYM;
        return null;
    }
    sym = (sym_t *)stack_push(s, objsize);
    if (!sym) {
        goto label_oom;
    }
    if (named) {
        sym->id = named->id;
    } else {
        sym->id = anon_id;
    }
    sym->scope = scope;
    if (refs) {
        addr = (uintptr *)stack_push(refs, sizeof(sym_t *));
        if (!addr) {
            stack_pop(s, null);
            goto label_oom;
        }
        *addr = (uintptr)sym;
    }
    return sym;
label_oom:
    cc->yferr = ERROR_OUT_OF_MEMORY;
    return null;
}

#define STACK_ONE_SYMBOL ((struct stack_it *)(Uint)1)
#define STACK_ALL_SYMBOL null

void sym_pop(chcc_t *cc, stack_t *s, struct stack_it *b, bool keep_in_s)
{
    sym_t *sym;
    cfsym_t *cfsym;
    struct stack_it *p = stack_begin(s);
    while (p && p != b) {
        sym = (sym_t *)stack_it_get(p);
        cfsym = cfsym_get(cc, sym->id);
        if (cfsym) {
            if (sym->type_kind) {
                stack_pop(&cfsym->u.named_type, 0);
            } else if (sym->is_field) {
                stack_pop(&cfsym->u.field_sym, 0);
            } else {
                stack_pop(&cfsym->u.ident_sym, 0);
            }
        }
        p = stack_next(p);
        // p必须在行之前更新
        if (!keep_in_s) {
            stack_pop(s, 0);
        }
        if (b == STACK_ONE_SYMBOL) {
            break; // 只释放一个符号
        }
    }
}

sym_t *find_type(cfsym_t *type_sym)
{
    // 最内层的局部作用域中的名字总是优先于外层的同名符号
    if (!type_sym) return 0;
    return (sym_t *)stack_top(&type_sym->u.named_type);
}

func_t *find_func(cfsym_t *name)
{
    return (func_t *)stack_top(&name->u.func_sym);
}

struct stack_before_it *find_func_before(cfsym_t *name)
{
    if (find_func(name)) {
        return stack_before_begin(&name->u.func_sym);
    }
    return null;
}

struct stack_before_it *find_method_before(cfsym_t *name)
{
    stack_t *s = &name->u.method;
    snode_t *b = (snode_t *)stack_before_begin(s);
    func_t *f;
    while (b->next) { // 查找对应名称的方法
        f = (func_t *)(b->next + 1);
        if (f->sym.id == name->id) {
            return (struct stack_before_it *)b;
        }
        b = b->next;
    }
    return null;
}

func_t *find_method(cfsym_t *name)
{
    struct stack_before_it *b = find_method_before(name);
    return b ? (func_t *)stack_before_it_get(b) : null;
}

sym_t *sym_type_redefined(cfsym_t *type_sym, uint32 scope)
{
    sym_t *t = find_type(type_sym);
    return (t && t->scope == scope) ? t : 0;
}

field_t *sym_field_redefined(cfsym_t *field_sym, uint32 scope)
{
    if (!field_sym) return 0;
    field_t *field = (field_t *)stack_top(&field_sym->u.field_sym);
    return (field && field->sym.scope == scope) ? field : 0;
}

void basic_type_decl(chcc_t *cc, cfid_t id, Uint size, uint8 align)
{
    stack_t *s = &cc->symbol;
    cfsym_t *sym;
    sym_t *type;

    sym = cfsym_get(cc, id);

    type = (sym_t *)sym_push(cc, sym, 0, s, sizeof(sym_t), &sym->u.named_type);
    type->type_kind = SYM_TYPE_BASIC;
    type->align = align;
    type->size = size;
}

void chcc_init(chcc_t *cc, bufile_t *f)
{
    stack_t *s = &cc->symbol;

    memset(cc, 0, sizeof(chcc_t));

    cifa_init(cc, f);

    basic_type_decl(cc, CIFA_ID_INT8, 1, 0); // 1 = 1 << 0, 2 = 1 << 1, 4 = 1 << 2, 8 = 1 << 3
    basic_type_decl(cc, CIFA_ID_INT16, 2, 1);
    basic_type_decl(cc, CIFA_ID_INT32, 4, 2);
    basic_type_decl(cc, CIFA_ID_INT64, 8, 3);

    basic_type_decl(cc, CIFA_ID_UINT8, 1, 0);
    basic_type_decl(cc, CIFA_ID_UINT16, 2, 1);
    basic_type_decl(cc, CIFA_ID_UINT32, 4, 2);
    basic_type_decl(cc, CIFA_ID_UINT64, 8, 3);

#if __ARCH_64BIT__
    basic_type_decl(cc, CIFA_ID_INT, 8, 3);
    basic_type_decl(cc, CIFA_ID_UINT, 8, 3);
    basic_type_decl(cc, CIFA_ID_STRING, 16, 3);
#else
    basic_type_decl(cc, CIFA_ID_INT, 4, 2);
    basic_type_decl(cc, CIFA_ID_UINT, 4, 2);
    basic_type_decl(cc, CIFA_ID_STRING, 8, 2);
#endif

#ifdef CONFIG_32BIT_FLOAT
    basic_type_decl(cc, CIFA_ID_FLOAT, 4, 2);
    basic_type_decl(cc, CIFA_ID_COMPLEX, 8, 2);
#else
    basic_type_decl(cc, CIFA_ID_FLOAT, 8, 3);
    basic_type_decl(cc, CIFA_ID_COMPLEX, 16, 3);
#endif

    basic_type_decl(cc, CIFA_ID_FLOAT16, 2, 1);
    basic_type_decl(cc, CIFA_ID_FLOAT32, 4, 2);
    basic_type_decl(cc, CIFA_ID_FLOAT64, 8, 3);

    basic_type_decl(cc, CIFA_ID_COMPLEX16, 4, 1);
    basic_type_decl(cc, CIFA_ID_COMPLEX32, 8, 2);
    basic_type_decl(cc, CIFA_ID_COMPLEX64, 16, 3);

    basic_type_decl(cc, CIFA_ID_BOOL, 1, 0);
    basic_type_decl(cc, CIFA_ID_BYTE, 1, 0);
    basic_type_decl(cc, CIFA_ID_RUNE, 4, 2);
    basic_type_decl(cc, CIFA_ID_ERROR, 4, 2);

    stack_push(s, sizeof(sym_t));
    cc->global = stack_begin(s); // 压入一个哑符号作为全局符号的顶部


}

// 当前代码包可能有的标识符名称
// 1. 全局名称
//      包名
//      常量名
//      类型名
//      函数或方法名
// 2. 结构体内部成员名称
//      局部变量名
// 3. 函数或方法内部名称
//      标签名
//      局部变量名
//      匿名结构体
//      匿名接口
//      匿名函数
//      匿名闭包
//
// 代码包重名标识符的处理
// 1. 类型名
//      类型名必然都是全局名称
//      类型名不能重名
//      类型名于包名、常量、函数、变量的名称形式都有所区别
//      局部的匿名类型本来没有名字，且只在局部使用，因此无关紧要
// 2. 结构体内局部变量名
//      局部变量名只在对应的作用域内有效
//      也只保存在对应的局部作用域中
//

cfsym_t *ident_(chcc_t *cc, bool type_ident)
{
    cfys_t *cf = &cc->curcf;
    cfsym_t *sym;
    if (cf->cfid < CIFA_ID_START) {
        return null;
    }
    if (type_ident && !cf->cftype) {
        return null;
    }
    sym = cf->val.sym;
    curr(cc);
    return sym;
}

cfsym_t *ident(chcc_t *cc)
{
    return ident_(cc, false);
}

cfsym_t *type_ident(chcc_t *cc)
{
    return ident_(cc, true);
}

void skip(chcc_t *cc, cfid_t c)
{
    curr(cc);
}

Uint aligned_offset(Uint addr, Uint align)
{
    align = (1 << align) - 1;
    return (addr + align) & (~align);
}

field_t *sym_create_field(chcc_t *cc, stack_t *s, cfsym_t *field_name, uint32 scope)
{
    field_t *field;
    stack_t *refs = field_name ? &field_name->u.field_sym : null;
    if (sym_field_redefined(field_name, scope)) {
        cc->yferr = ERROR_FIELD_REDEFINED;
        return null;
    }
    field = (field_t *)sym_push(cc, field_name, scope, s, sizeof(field_t), refs);
    if (!field) {
        return null;
    }
    field->sym.is_field = 1;
    return field;
}

bool check_struct_embed_field_name_conflict(struct_t *s)
{
    return false;
}

void sym_free(void *object)
{
    sym_t *sym = (sym_t *)object;
    sym;
}

void tpcur_free(tpcur_t *tpcur)
{
    stack_delete_it(tpcur->type_struct, sym_free);
}

// struct_type = "struct" "{" { field_decl [";"] } "}" .
// field_decl = embed_type "." |
//              unsigned_type ":" ident dec_lit { ident dec_lit } |
//              type_var_list .
// embed_type = type_ident | type_ident "&" .
// unsigned_type = "byte" | "uint" | "uint8" | "uint16" | "uint32" | "uint64" .
// type_var_list = type_lit ident ( { ident } | { "|" ident } ) .
// type_lit = type_ident | type_ident "&" | struct_type | interface_type | func_ptr .
bool struct_decl(chcc_t *cc, cfsym_t *named, tpcur_t *out)
{
    cfys_t *cf = &cc->curcf;
    struct stack_it *struct_sym;
    tpcur_t field_type = {0};
    uint32 max_align = 0;
    Uint prevoff = 0;
    field_t *first_field;
    field_t *prev;
    field_t *field;
    sym_t *t;
    cfsym_t *type_ident;
    cfsym_t *ident;
    struct_t *ps;
    Uint bfoff;
    Uint bfsize;
    uint8 dyn = 0;
    bool alias_field = false;
    bool embed_field_exist = false;

    if (cf->cfid != CIFA_CH_OPEN_CURLY) {
        cc->yferr = ERROR_NO_OPEN_CURLY;
        return null;
    }

    skip(cc, CIFA_CH_OPEN_CURLY);

    // 结构体本身属于当前作用域
    struct_sym = stack_new_it(sizeof(struct_t));
    if (!struct_sym) {
        return false;
    }

    ps = (struct_t *)stack_it_get(struct_sym);
    ps->sym.type_kind = SYM_TYPE_STRUCT;

    // 开始进入结构体内部作用域，解析结构体内部成员
    cc->scope++;

    // 结构体可以出现在：
    //  1. 全局类型定义，scope为0
    //  2. 结构体内部匿名结构体作为类型声明，scope为结构体所在作用域加1
    //  3. 函数签名中出现的匿名结构体，scope为函数所在作用域加1
    //  4. 函数体内部出现的匿名结构体，scope为函数所在作用域加1
    for (; ;) {
        stack_delete_it(field_type.type_struct, sym_free);
        memset(&field_type, 0, sizeof(field_type));
        if (!type_lit(cc, null, &field_type)) {
            if (cf->cfid == CIFA_CH_CLOSE_CURLY) {
                cc->yferr = null;
                skip(cc, CIFA_CH_CLOSE_CURLY);
                break;
            }
            goto label_error;
        }

        // 成功读取到下一个合法的成员的类型
        prev = (field_t *)stack_top(&ps->field);
        if (prev && prev->sym.dyn) {
            cc->yferr = ERROR_DYN_TYPE_FIELD_POS;
            goto label_error;
        }

        type_ident = field_type.type_ident;
        t = field_type.t;

        if (cf->cfid == CIFA_CH_DOT) { // 内嵌
            skip(cc, CIFA_CH_DOT);
            if (!type_ident) {
                cc->yferr = ERROR_EMBED_TYPE_INVALID;
                goto label_error;
            }
            field = sym_create_field(cc, &ps->field, type_ident, cc->scope);
            if (!field) {
                goto label_error;
            }
            field->type = t;
            field->type_ident = type_ident;
            field->embed = 1;
            if (field_type.tpref) { // 内嵌类型引用，此时tpcur->t可能还不存在
                field->tpref = 1;
                field->sym.align = ALIGNOF_POINTER;
                field->sym.size = SIZE_OF_POINTER;
            } else { // 内嵌类型本身，此时保证tpcur->t一定已经存在
                field->sym.dyn = t->dyn;
                field->sym.align = t->align;
                field->sym.size = t->size;
            }
            field->sym.off = aligned_offset(prevoff, field->sym.align);
            prevoff = field->sym.off + field->sym.size;
            embed_field_exist = true;
        } else if (cf->cfid == CIFA_CH_COLON) { // 位域
            skip(cc, CIFA_CH_COLON);
            if (type_ident->id >= CIFA_ID_UINT && type_ident->id <= CIFA_ID_UINT64) {
                if (!cf->cfident) {
                    cc->yferr = ERROR_INVALID_BITFIELD_NAME;
                    goto label_error;
                }
                bfoff = 0;
                for (; ;) {
                    ident = cf->val.sym;
                    skip(cc, ident->id);
                    if (cf->cfid != CIFA_CF_INT_LIT || cf->num_base != CIFA_DEC_LIT) {
                        cc->yferr = ERROR_BITFIELD_SIZE_NOT_DEC;
                        goto label_error;
                    }
                    bfsize = (uint8)cf->val.i;
                    skip(cc, CIFA_CF_INT_LIT);
                    if (bfoff + bfsize > t->size * 8) {
                        cc->yferr = ERROR_INVALID_BITFIELD_SIZE;
                        log_error_s_3(ERROR_INVALID_BITFIELD_SIZE, ident->s, bfoff, bfsize, t->size * 8);
                        goto label_error;
                    }
                    field = sym_create_field(cc, &ps->field, ident, cc->scope);
                    if (!field) {
                        goto label_error;
                    }
                    field->sym.align = t->align;
                    field->sym.off = aligned_offset(prevoff, t->align);
                    field->sym.size = t->size;
                    field->sym.bf_off = (uint8)bfoff;
                    field->sym.bf_size = (uint8)bfsize;
                    field->sym.bitfield = 1;
                    field->type = t;
                    field->type_ident = ident;
                    bfoff += bfsize;
                    if (!cf->cfident) {
                        break;
                    }
                }
                prevoff = field->sym.off + field->sym.size;
            } else {
                cc->yferr = ERROR_INVALID_BITFIELD_TYPE;
                goto label_error;
            }
        } else {
            if (!type_ident) {
                cc->yferr = ERROR_INVALID_FIELD_TYPE;
                goto label_error;
            }
            if (!cf->cfident) {
                cc->yferr = ERROR_INVALID_FIELD_NAME;
                goto label_error;
            }
            first_field = null;
            alias_field = false;
            for (; ;) {
                ident = cf->val.sym;
                skip(cc, ident->id);
                field = sym_create_field(cc, &ps->field, ident, cc->scope);
                if (!field) {
                    goto label_error;
                }
                field->type = t;
                field->type_ident = ident;
                if (field_type.tpref) {
                    field->tpref = 1;
                    field->sym.align = ALIGNOF_POINTER;
                    field->sym.size = SIZE_OF_POINTER;
                } else {
                    field->sym.dyn = t->dyn;
                    field->sym.align = t->align;
                    field->sym.size = t->size;
                }
                if (alias_field) {
                    field->sym.off = first_field->sym.off;
                    field->alias = 1;
                    if (cf->cfid != CIFA_CH_VERTBAR) {
                        break; // 成员别名列表结束
                    }
                    skip(cc, CIFA_CH_VERTBAR);
                } else {
                    field->sym.off = aligned_offset(prevoff, field->sym.align);
                    prevoff = field->sym.off + field->sym.size;
                }
                if (first_field == null) {
                    first_field = field;
                    if (cf->cfid == CIFA_CH_VERTBAR) {
                        skip(cc, CIFA_CH_VERTBAR);
                        alias_field = true;
                    }
                }
                if (!cf->cfident) {
                    if (alias_field) {
                        cc->yferr = ERROR_MISSING_ALIAS_FIELD;
                        goto label_error;
                    }
                    break; // 成名声明列表结束
                }
            }
        }
        if (field->sym.dyn) {
            dyn = 1;
        }
        if (field->sym.align > max_align) {
            max_align = field->sym.align;
        }
    }

    if (embed_field_exist && !check_struct_embed_field_name_conflict(ps)) {
        goto label_error;
    }

    ps->sym.dyn = dyn;
    ps->sym.align = max_align;
    ps->sym.size = aligned_offset(prevoff, max_align);

    // 离开结构体作用域，释放所有成员符号的引用
    sym_pop(cc, &ps->field, STACK_ALL_SYMBOL, true);
    out->type_struct = struct_sym;
    out->t = &ps->sym;
    stack_delete_it(field_type.type_struct, sym_free);
    cc->scope--;
    return true;

label_error:
    sym_pop(cc, &ps->field, STACK_ALL_SYMBOL, false);
    stack_delete_it(field_type.type_struct, sym_free);
    stack_delete_it(struct_sym, sym_free);
    cc->scope--;
    return false;
}

// func_type = "func" [ name_sign ] "(" [ func_parm ] ")" [ func_attr ] [ "{" func_body "}" ] .
// name_sign = [ type_ident | closure_capture ] [ func_name ] .
// func_parm = param_list [ return [ rparm_list ] ] .
// func_attr = @ident { @ident } .
// func_body = statement_list .
// param_list = { type_lit [ { ident } ] } [ type_lit ... | type ... ].
// rparm_list = { type_lit [ { ident } ] } .

void sym_func_free(func_t *f)
{
    sym_free(&f->sym);
}

field_t *sym_create_param(chcc_t *cc, stack_t *s, cfsym_t *name, tpcur_t *tpcur, Uint prevoff)
{
    field_t *field;
    sym_t *t = tpcur->t;
    if (!tpcur->tpref && t->dyn) {
        cc->yferr = ERROR_DYN_TYPE_CANT_BE_PARAM;
        return null;
    }
    field = sym_create_field(cc, s, name, cc->scope);
    if (!field) {
        return null;
    }
    field->type = t;
    field->type_ident = tpcur->type_ident;
    if (tpcur->tpref) {
        field->tpref = 1;
        field->sym.align = ALIGNOF_POINTER;
        field->sym.size = SIZE_OF_POINTER;
    } else {
        field->sym.align = t->align;
        field->sym.size = t->size;
    }
    field->sym.off = aligned_offset(prevoff, field->sym.align);
    field->param = 1;
}

bool sym_func_decl_match(func_t *a, func_t *b)
{
    field_t *field_a;
    field_t *field_b;
    struct stack_it *ait = stack_begin(&a->field);
    struct stack_it *bit = stack_begin(&b->field);
    if (a->param_align != b->param_align || a->rparm_align != b->rparm_align ||
        a->param_size != b->param_size || a->rparm_size != b->rparm_size) {
        return false;
    }
    while (ait != null) {
        if (bit == null) {
            return false;
        }
        field_a = (field_t *)stack_it_get(ait);
        field_b = (field_t *)stack_it_get(bit);
        if (field_a->type_ident != field_b->type_ident ||
            field_a->tpref != field_b->tpref ||
            field_a->param != field_b->param ||
            field_a->rparm != field_b->rparm ||
            field_a->vaarg != field_b->vaarg ||
            field_a->sym.align != field_b->sym.align ||
            field_a->sym.off != field_b->sym.off ||
            field_a->sym.size != field_b->sym.size) {
            return false;
        }
        ait = stack_next(ait);
        bit = stack_next(bit);
    }
    return true;
}

bool func_decl(chcc_t *cc, cfsym_t *named, tpcur_t *out)
{
    cfys_t *cf = &cc->curcf;
    struct stack_it *func_sym;
    tpcur_t field_type = {0};
    tpcur_t addr_type = {0};
    sym_t *receiver = null;
    cfsym_t *recv_name = null;
    cfsym_t *func_name = null;
    field_t *field;
    func_t *pf;
    cfsym_t *ident;
    uint32 max_align;
    Uint prevoff;
    bool miss_param_name = false;

    // 方法所属类型
    if (cf->cftype) {
        if (cf->reserved) {
            cc->yferr = ERROR_INVALID_RECEIVER_TYPE;
            return false;
        }
        recv_name = cf->val.sym;
        receiver = find_type(recv_name);
        if (!receiver) {
            cc->yferr = ERROR_RECEIVER_TYPE_UNFOUND;
            return false;
        }
        skip(cc, cf->cfid);
    }

    // 函数名称
    if (cf->cfident) {
        func_name = cf->val.sym;
        skip(cc, cf->cfid);
    }

    if (cf->cfid != CIFA_CH_OPEN_PAREN) {
        cc->yferr = ERROR_MISSING_OPEN_PAREN;
        return false;
    }

    skip(cc, CIFA_CH_OPEN_PAREN);

    func_sym = stack_new_it(sizeof(func_t));
    if (!func_sym) {
        return false;
    }

    pf = (func_t *)stack_it_get(func_sym);
    pf->sym.type_kind = SYM_TYPE_FUNCTION;
    pf->receiver = receiver;
    pf->func_name = func_name;

    // 进入函数作用域，函数参数属于函数作用域
    cc->scope++;

    addr_type.type_ident = cfsym_get(cc, CIFA_ID_UINT);
    addr_type.t = find_type(addr_type.type_ident);

    prevoff = 0; // 函数返回地址
    field = sym_create_param(cc, &pf->field, null, &addr_type, prevoff);
    if (!field) {
        goto label_error;
    }

    prevoff = SIZE_OF_POINTER; // 返回值的地址
    field = sym_create_param(cc, &pf->field, null, &addr_type, prevoff);
    if (!field) {
        goto label_error;
    }

    prevoff += SIZE_OF_POINTER;
    max_align = ALIGNOF_POINTER;

    if (receiver) { // type指针参数
        addr_type.type_ident = recv_name;
        addr_type.t = receiver;
        addr_type.tpref = 1;
        field = sym_create_param(cc, &pf->field, cfsym_get(cc, CIFA_ID_TYPE), &addr_type, prevoff);
        if (!field) {
            goto label_error;
        }
        prevoff += SIZE_OF_POINTER;
    }

    for (; ;) {
        stack_delete_it(field_type.type_struct, sym_free);
        memset(&field_type, 0, sizeof(field_type));
        if (!type_lit(cc, null, &field_type)) {
            if (cf->cfid == CIFA_CH_CLOSE_PAREN) {
                cc->yferr = null;
label_close_paren:
                skip(cc, CIFA_CH_CLOSE_PAREN);
                if (pf->param == null) {
                    pf->param = stack_begin(&pf->field);
                    pf->param_align = max_align;
                    pf->param_size = aligned_offset(prevoff, max_align);
                } else {
                    pf->rparm = stack_begin(&pf->field);
                    pf->rparm_align = max_align;
                    pf->rparm_size = aligned_offset(prevoff, max_align);
                }
                break; // 遇到结束小括号，整个参数和返回值解析完毕
            }
            if (cf->cfid == CIFA_ID_RETURN) {
                cc->yferr = null;
label_return_param:
                pf->param = stack_begin(&pf->field);
                pf->param_align = max_align;
                pf->param_size = aligned_offset(prevoff, max_align);
                prevoff = 0;
                max_align = 0;
                skip(cc, CIFA_ID_RETURN);
                continue;
            } else if (cf->cfid == CIFA_ID_TYPE) {
                cc->yferr = null;
label_basic_va_arg:
                if (pf->param == null) { // 有签名的返回地址等内容保证遇到return后不会为null
                    skip(cc, CIFA_ID_TYPE);
                    if (cf->cfid == CIFA_OP_3DOT) {
                        skip(cc, CIFA_OP_3DOT);
                        field = sym_create_field(cc, &pf->field, null, cc->scope);
                        if (!field) {
                            goto label_error;
                        }
                        field->type = null;
                        field->sym.align = BASIC_MAX_ALIGN;
                        field->sym.off = aligned_offset(prevoff, BASIC_MAX_ALIGN);
                        field->param = 1;
label_shall_last_param:
                        field->vaarg = 1;
                        field->sym.dyn = 1;
                        if (field->sym.align > max_align) {
                            max_align = field->sym.align;
                        }
                        if (cf->cfid == CIFA_ID_RETURN) {
                            goto label_return_param;
                        } else if (cf->cfid == CIFA_CH_CLOSE_PAREN) {
                            goto label_close_paren;
                        } else {
                            cc->yferr = ERROR_VAARG_SHALL_LAST_PARAM;
                        }
                    } else {
                        cc->yferr = ERROR_MISS_BASIC_VAARG_3DOT;
                    }
                }
            }
            goto label_error;
        }
        if (!field_type.type_ident) {
            cc->yferr = ERROR_INVALID_PARAM_TYPE;
            goto label_error;
        }
        if (!cf->cfident) {
            ident = null;
            if (pf->param == null) {
                miss_param_name = true;
            }
            field = sym_create_param(cc, &pf->field, ident, &field_type, prevoff);
            if (!field) {
                goto label_error;
            }
            prevoff = field->sym.off + field->sym.size;
            if (pf->param == null) {
                if (cf->cfid == CIFA_OP_3DOT) {
                    skip(cc, CIFA_OP_3DOT);
                    goto label_shall_last_param;
                }
            } else {
                field->rparm = 1;
            }
            goto label_continue;
        }
        for (; ;) {
            if (cf->reserved) {
                if (pf->param == null) {
                    if (cf->cfid == CIFA_ID_RETURN) {
                        goto label_return_param;
                    } else if (cf->cfid == CIFA_ID_TYPE) {
                        goto label_basic_va_arg;
                    }
                }
                cc->yferr = ERROR_INVALID_PARAM_NAME;
                goto label_error;
            }
            ident = cf->val.sym;
            skip(cc, ident->id);
            field = sym_create_param(cc, &pf->field, ident, &field_type, prevoff);
            if (!field) {
                goto label_error;
            }
            prevoff = field->sym.off + field->sym.size;
            if (pf->param != null) {
                field->rparm = 1;
            }
label_continue:
            if (field->sym.align > max_align) {
                max_align = field->sym.align;
            }
            if (!cf->cfident) {
                break; // 一个相同类型的参数列表结束
            }
        }
    }

    // func 函数声明存在的场景
    // 1. 全局作用域类型定义语句，定义一个函数类型，不能有函数体，不需添加函数原型，只需定义这个函数类型
    // 2. 全局作用域声明一个函数原型，需要添加原型到全局作用域，没有函数体
    // 3. 全局作用域定义一个函数，需要将原型添加到全局作用域，且包含函数体
    // 4. 函数体内定义一个局部函数对象，不能只是一个原型声明，必须定义函数体
    if (cf->cfid == CIFA_CH_OPEN_CURLY) {
        if (named) { // 类型定义语句定义函数类型不能包含函数体
            cc->yferr = ERROR_TYPE_CANT_CONTAIN_BODY;
            goto label_error;
        }
        if (miss_param_name) {
            cc->yferr = ERROR_FUNC_MISS_PARAM_NAME;
            goto label_error;
        }
        pf->have_body = 1;
    } else {
        sym_pop(cc, &pf->field, STACK_ALL_SYMBOL, true);
    }

    stack_delete_it(field_type.type_struct, sym_free);
    out->type_struct = func_sym;
    out->t = &pf->sym;
    cc->scope--;
    return true;

label_error:
    sym_pop(cc, &pf->field, STACK_ALL_SYMBOL, false);
    stack_delete_it(field_type.type_struct, sym_free);
    stack_delete_it(func_sym, sym_free);
    cc->scope--;
    return false;
}

bool type_lit(chcc_t *cc, cfsym_t *named, tpcur_t *out)
{
    bool ret = false;
    cfys_t *cf = &cc->curcf;
    sym_t *t;
    cfsym_t *b;

    b = ident(cc);
    if (!b) {
        cc->yferr = ERROR_INVALID_TYPE_LIT;
        return false;
    }

    if (b->id == CIFA_ID_STRUCT) {
        ret = struct_decl(cc, named, out);
    } else if (b->id == CIFA_ID_INTERFACE) {
        // interface_decl();
    } else if (b->id == CIFA_ID_FUNC) {
        ret = func_decl(cc, named, out);
    } else {
        if (b->id < cc->user_id_start) {
            cc->yferr = ERROR_NAME_RESERVED;
            return false;
        }
        if (!cf->cftype) {
            cc->yferr = ERROR_INVALID_TYPE_NAME;
            return false;
        }
        t = find_type(b); // 变量或成员声明语句，返回找到的类型
        if (cf->cfid == CIFA_CH_ASTER) {
            skip(cc, CIFA_CH_ASTER);
            out->tpref = 1;
        } else if (!t) {
            cc->yferr = ERROR_TYPE_NOT_DEFINED;
            log_error_s_2(ERROR_TYPE_NOT_DEFINED, b->s, cf->cf_line, cf->cf_col);
            return false;
        }
        out->type_ident = b;
        out->t = t;
        ret = true;
    }

    return ret;
}

void func_gen(chcc_t *cc, func_t *f)
{
    // 进入函数体
    skip(cc, CIFA_CH_OPEN_CURLY);

}

void func_def(chcc_t *cc)
{
    tpcur_t func_type = {0};
    sym_t *receiver;
    cfsym_t *name_added_to;
    struct stack_before_it *b;
    func_t *exist;
    func_t *f;
    if (!func_decl(cc, null, &func_type)) {
        return;
    }
    f = (func_t *)func_type.t;
    if (!f->func_name) { // 全局作用域声明函数原型或定义函数，必须带有函数名
        cc->yferr = ERROR_GLOBAL_FUNC_MISS_NAME;
        goto label_return;
    }
    f->sym.type_kind = 0;
    receiver = f->receiver;
    if (receiver) {
        f->sym.is_method = 1;
        name_added_to = cfsym_get(cc, receiver->id);
        b = find_method_before(name_added_to);
    } else {
        f->sym.is_func = 1;
        name_added_to = f->func_name;
        b = find_func_before(name_added_to);
    }
    exist = (func_t *)stack_before_it_get(b);
    if (exist) {
        if (!sym_func_decl_match(f, exist)) {
            cc->yferr = ERROR_FUNC_DECL_NOT_MATCH;
            goto label_return;
        }
        // 使用已经存在的函数声明或定义
        if (exist->have_body) {
            if (f->have_body) {
                cc->yferr = ERROR_FUNC_IS_REDEFINED;
            } else {
                // f 仅仅是一个函数声明，因为已经存在，不需要保存 f
            }
            goto label_return;
        } else {
            // 使用f替换掉exist
            stack_replace(b, func_type.type_struct, sym_free);
            func_type.type_struct = null;
        }
    } else {
        // 创建新的函数声明
        if (sym_push_it(cc, &cc->symbol, func_type.type_struct, name_added_to)) {
            func_type.type_struct = null;
        } else {
            goto label_return;
        }
    }
    if (f->have_body) {
        if (exist && exist->have_body) {
            cc->yferr = ERROR_FUNC_IS_REDEFINED;
            goto label_return;
        }
        func_gen(cc, f);
        return;
    }
label_return:
    stack_delete_it(func_type.type_struct, sym_free);
}

// 类型定义语句，只能出现在全局作用域
//  type Type TypeIdent // 不支持基于已存在类型创建新的类型
//  type Type struct {...}
//  type Type interface {...}
//  type Type func (...)
void type_def(chcc_t *cc)
{
    cfys_t *cf = &cc->curcf;
    tpcur_t tpcur = {0};
    cfsym_t *named;

    named = type_ident(cc);
    if (!named) {
        cc->yferr = ERROR_INVALID_TYPE_NAME;
        return;
    }
    if (named->id < cc->user_id_start) {
        cc->yferr = ERROR_NAME_RESERVED;
        return;
    }

    if (sym_type_redefined(named, cc->scope)) {
        cc->yferr = ERROR_TYPE_REDEFINED;
        log_error_s_2(ERROR_TYPE_REDEFINED, named->s, cf->cf_line, cf->cf_col);
        return;
    }

    if (!type_lit(cc, named, &tpcur)) {
        return;
    }

    if (!tpcur.type_struct) {
        //  type Type TypeIdent // 不支持基于已存在类型创建新的类型
        cc->yferr = ERROR_TYPE_LIT_NOT_SUPPORT;
        return;
    }

    if (!sym_push_it(cc, &cc->symbol, tpcur.type_struct, named)) {
        stack_delete_it(tpcur.type_struct, sym_free);
    }
}

void statement(chcc_t *cc)
{
    cfys_t *cf = &cc->curcf;
    cfsym_t *a;

    for (; ;) {
        cc->yferr = null;

        a = ident(cc);
        if (!a) {
            cc->yferr = ERROR_IS_NOT_IDENT;
            goto label_return;
        }

        if (a->id == CIFA_ID_TYPE) {
            type_def(cc);
        } else if (a->id == CIFA_ID_FUNC) {
            func_def(cc);
        }

        if (cc->yferr) {
            goto label_return;
        }
    }

    return;
label_return:
    log_error_3(cc->yferr, cf->cf_line, cf->cf_col, cf->cfid);
}
