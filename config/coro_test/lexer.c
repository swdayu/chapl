#include "coro.h"

#define CORO_STACK_SIZE 8*1024

typedef enum {
    TOK_EOF,
    TOK_INT = 0x100,
    TOK_OPER = 0x200,
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
    int value;
} Token;

typedef struct {
    CoroCont cont;
    Coro *coro;
    Token oper;
    Token value;
} Context;

static const char *parse_int(const char *expr, int *out)
{
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

__magic_coro_api(void) lexer(Coro *coro)
{
    int ch;
    const char *expr = (const char *)coro->para;
    Token *token = (Token *)coro->retp;
    if (!expr) goto label_return;
    while ((ch = *expr++)) {
        if (ch == ' ' || ch == '\t') {
            continue; // skip space
        } else if (ch >= '0' && ch <= '9') {
            token->kind = TOK_INT;
            expr = parse_int(expr-1, &token->value);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            token->kind = TOK_OPER + ch;
            token->value = (ch == '+' || ch == '-') ? PRIO_ADD : PRIO_MUL;
        } else {
            break;
        }
        coroutine_yield(coro);
        token = (Token *)coro->retp;
    }
label_return:
    token->kind = TOK_EOF;
}

#define INT_TAG "TOK_INT: "
#define OPR_TAG "TOK_OPR: "
#define SUM_TAG "       : "

bool token(Context *ctx, TokenKind kind)
{
    Token *token = (kind == TOK_INT) ? &ctx->value : &ctx->oper;
    while (coroutine_yield_manual(ctx->cont, ctx->coro, (uptr)token) && (token->kind & kind)) {
        if (kind == TOK_INT) {
            printf(INT_TAG"%d\n", token->value);
        } else {
            printf(OPR_TAG"%c\n", (token->kind & 0xff));
        }
        return true;
    }
    token->kind = TOK_EOF;
    token->value = 0;
    return false;
}

void caculate(int left, TokenKind oper, Token *right)
{
    int result;
    if (oper == TOK_ADD) {
        result = left + right->value;
        printf(SUM_TAG"%d + %d = %d\n", left, right->value, result);
    } else if (oper == TOK_SUB) {
        result = left - right->value;
        printf(SUM_TAG"%d - %d = %d\n", left, right->value, result);
    } else if (oper == TOK_MUL) {
        result = left * right->value;
        printf(SUM_TAG"%d * %d = %d\n", left, right->value, result);
    } else if (oper == TOK_DIV) {
        if (right->value == 0) {
            result = left;
            printf(SUM_TAG"%d / %d = divided by zero error!\n", left, right->value);
        } else {
            result = left / right->value;
            printf(SUM_TAG"%d / %d = %d\n", left, right->value, result);
        }
    } else {
        result = 0xa5a5a5a5;
    }
    right->value = result;
}

void evaluate(Context *ctx, Token perv_oper)
{
    Token left, curr_oper;
    if (!token(ctx, TOK_OPER)) {
        return;
    }
    while (ctx->oper.value > perv_oper.value) {
        left = ctx->value;
        curr_oper = ctx->oper;
        if (!token(ctx, TOK_INT)) {
            return;
        }
        evaluate(ctx, curr_oper);
        if (ctx->value.kind != TOK_INT) {
            return;
        }
        caculate(left.value, curr_oper.kind, &ctx->value);
    }
}

void test_lexer(const char *expr)
{
    CoroCont cont = coroutine_init(0, 1);
    Coro *coro = coroutine_create(cont, lexer, CORO_STACK_SIZE, (uptr)expr);
    Context ctx = {cont, coro, {TOK_EOF, -1}};
    if (expr) {
        printf("expr: %s\n", expr);
    }
    if (token(&ctx, TOK_INT)) {
        evaluate(&ctx, ctx.oper);
    }
    printf("Quit!\n");
    coroutine_finish(&cont);
}
