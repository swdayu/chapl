#define PRH_CORO_INCLUDE
#define PRH_CORO_STRIP_PREFIX
#include "prh_include.h"
#include <stdio.h>

#define CORO_STACK_SIZE (128*3)

typedef enum {
    TOK_EOF,
    TOK_INT,
    TOK_L_PAREN = '(',
    TOK_R_PAREN = ')',
    TOK_OPER = 0x100,
    TOK_ADD = TOK_OPER + '+',
    TOK_SUB = TOK_OPER + '-',
    TOK_MUL = TOK_OPER + '*',
    TOK_DIV = TOK_OPER + '/',
} TokenKind;

typedef enum {
    PRIO_ADD,
    PRIO_MUL,
} Priority;

typedef struct {
    TokenKind kind;
    union { int value; int prio; } u;
} Token;

typedef struct {
    const char *expr;
    coro_struct_t *main;
    Token oper;
    Token value;
    Token *token;
} Context;

static const char *parse_int(const char *expr, int *out) {
    int ch, value = 0;
    for (; (ch = *expr); expr += 1) {
        if (ch >= '0' && ch <= '9') {
            value = value * 10 + (ch - '0');
        } else {
            break;
        }
    }
    *out = value;
    return expr;
}

coro_proc_func lexer(coro_t *coro) {
    int ch;
    Context *ctx = (Context *)coro_get_data(coro);
    const char *expr = ctx->expr;
    Token *token = ctx->token;
    if (!expr) goto label_return;
    while ((ch = *expr++)) {
        if (ch == ' ' || ch == '\t') {
            continue; // skip space
        } else if (ch >= '0' && ch <= '9') {
            token->kind = TOK_INT;
            expr = parse_int(expr-1, &token->u.value);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            token->kind = TOK_OPER + ch;
            token->u.prio = (ch == '+' || ch == '-') ? PRIO_ADD : PRIO_MUL;
        } else if (ch == '(' || ch == ')') {
            token->kind = ch;
        } else {
            break;
        }
        coro_yield(coro);
        token = ctx->token;
    }
label_return:
    token->kind = TOK_EOF;
}

#define INT_TAG " TOK_INT: "
#define OPR_TAG " TOK_OPR: "
#define SUM_TAG "        : "

void eval(Context *ctx, Token perv_oper);

bool token(Context *ctx, TokenKind kind) {
    Token *t = (kind == TOK_OPER) ? &ctx->oper : &ctx->value;
    ctx->token = t;
    while (coro_start(ctx->main, 1)) {
        if (kind == TOK_OPER) {
            if (t->kind & TOK_OPER) {
                printf(OPR_TAG"%c\n", (t->kind & 0xff));
                return true;
            }
            if (t->kind == TOK_R_PAREN) {
                printf(OPR_TAG")\n");
                t->u.prio = -1; // r paren is the lowest operator
                return true;
            }
            if (t->kind == TOK_INT) {
                printf(OPR_TAG"skip %d\n", t->u.value);
            } else {
                printf(OPR_TAG"skip %c\n", t->kind);
            }
        } else {
            if (t->kind == TOK_INT) {
                printf(INT_TAG"%d\n", t->u.value);
                return true;
            }
            if (t->kind == TOK_L_PAREN) {
                printf(INT_TAG"(\n");
                if (!token(ctx, TOK_INT)) break;
                eval(ctx, (Token){TOK_EOF, {-1}});
                return true;
            }
            if (t->kind & TOK_OPER) {
                printf(INT_TAG"skip %c\n", (t->kind & 0xff));
            } else {
                printf(INT_TAG"skip %c\n", t->kind);
            }
        }
    }
    t->kind = TOK_EOF;
    t->u.prio = -1; // treat eof as the lowest operator
    return false;
}

void calc(int left, TokenKind oper, Token *right) {
    int result = 0;
    if (oper == TOK_ADD) {
        result = left + right->u.value;
        printf(SUM_TAG"%d + %d = %d\n", left, right->u.value, result);
    } else if (oper == TOK_SUB) {
        result = left - right->u.value;
        printf(SUM_TAG"%d - %d = %d\n", left, right->u.value, result);
    } else if (oper == TOK_MUL) {
        result = left * right->u.value;
        printf(SUM_TAG"%d * %d = %d\n", left, right->u.value, result);
    } else if (oper == TOK_DIV) {
        if (right->u.value == 0) {
            result = left;
            printf(SUM_TAG"%d / %d = divided by zero error!\n", left, right->u.value);
        } else {
            result = left / right->u.value;
            printf(SUM_TAG"%d / %d = %d\n", left, right->u.value, result);
        }
    }
    right->u.value = result;
}

void eval(Context *ctx, Token perv_oper) {
    Token left, curr_oper;
    if (!token(ctx, TOK_OPER)) {
        return;
    }
    while (ctx->oper.u.prio > perv_oper.u.prio) {
        left = ctx->value;
        curr_oper = ctx->oper;
        if (!token(ctx, TOK_INT)) return;
        eval(ctx, curr_oper);
        if (ctx->value.kind != TOK_INT) return;
        calc(left.u.value, curr_oper.kind, &ctx->value);
    }
}

void test_lexer(const char *expr) {
    coro_struct_t *s = coro_init(70, 1);
    Context ctx = {expr, s};
    coro_create(s, lexer, CORO_STACK_SIZE, &ctx);
    if (expr) {
        printf("    expr: %s\n", expr);
    }
    if (token(&ctx, TOK_INT)) {
        eval(&ctx, (Token){TOK_EOF, {-1}});
    }
    printf("Quit!\n");
    coro_finish(&s);
}
