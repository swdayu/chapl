#include "coro.h"

#define CORO_STACK_SIZE 1024

#define TOK_EOF (-1)
#define TOK_OP  1
#define TOK_INT 2

typedef struct {
    int kind;
    int value;
} Token;

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
    Token *token = (Token *)coro->para;
    const char *expr = (const char *)coro->retp;
    if (!expr) goto label_return;
    while ((ch = *expr++)) {
        if (ch == ' ' || ch == '\t') {
            continue; // skip space
        } else if (ch >= '0' && ch <= '9') {
            token->kind = TOK_INT;
            expr = parse_int(expr-1, &token->value);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            token->kind = TOK_OP;
            token->value = ch;
        } else {
            break;
        }
        coroutine_yield(coro);
    }
label_return:
    token->kind = TOK_EOF;
}

#define INT_TAG "TOK_INT: "
#define SUM_TAG "       : "

void test_lexer(const char *expr)
{
    int op = 0, prev = 0, curr;
    Token token = {TOK_EOF};
    CoroCont t = coroutine_init(0, 1);
    Coro *coro = coroutine_create(t, lexer, CORO_STACK_SIZE, (uptr)&token);
    if (expr) printf("expr: %s\n", expr);
    while (coroutine_yield_manual(t, coro, (uptr)expr)) {
        switch (token.kind) {
        case TOK_OP:
            op = token.value;
            break;
        case TOK_INT:
            curr = token.value;
            printf(INT_TAG"%d\n", curr);
            if (op == '+') {
                curr = prev + curr;
                printf(SUM_TAG"%d + %d = %d\n", prev, token.value, curr);
            } else if (op == '-') {
                curr = prev - curr;
                printf(SUM_TAG"%d - %d = %d\n", prev, token.value, curr);
            } else if (op == '*') {
                curr = prev * curr;
                printf(SUM_TAG"%d * %d = %d\n", prev, token.value, curr);
            } else if (op == '/') {
                if (curr == 0) {
                    curr = prev;
                    printf(SUM_TAG"%d / %d = divided by zero error!\n", prev, token.value);
                } else {
                    curr = prev / curr;
                    printf(SUM_TAG"%d / %d = %d\n", prev, token.value, curr);
                }
            }
            prev = curr;
            op = TOK_EOF;
            break;
        default:
            printf("Quit!\n");
            break;
        }
    }
    coroutine_finish(&t);
}
