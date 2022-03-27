//
// Created by nim on 2022/3/26.
//

#include "lcf_json.h"
#include <assert.h> /* assert() */
#include <stdlib.h> /* NULL, strtod() */

#define EXPECT(ctx, ch)                                                                            \
    do {                                                                                           \
        assert(*(ctx)->json == (ch));                                                              \
        (ctx)->json++;                                                                             \
    } while (0)

#define IS_DECIMAL_DIGIT(ch)  ((ch) >= '0' && (ch) <= '9')
#define IS_NON_ZERO_DIGIT(ch) ((ch) >= '1' && (ch) <= '9')

// Parse Whitespace
// type:
//  - ' '   space
//  - '\t'  horizontal tab
//  - '\n'  carriage return
//  - '\r'  linefeed
static void
lj_parse_whitespace(lj_context_t *ctx) {
    const char *p = ctx->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        p++;
    }
    ctx->json = p;
}

static int
lj_parse_null(lj_context_t *ctx, lj_value_t *val) {
    EXPECT(ctx, 'n');
    if (ctx->json[0] != 'u' || ctx->json[1] != 'l' || ctx->json[2] != 'l') {
        return LJ_PARSE_INVALID_VALUE;
    }
    ctx->json += 3;
    val->type = LJ_NULL;
    return LJ_PARSE_OK;
}

static int
lj_parse_true(lj_context_t *ctx, lj_value_t *val) {
    EXPECT(ctx, 't');
    if (ctx->json[0] != 'r' || ctx->json[1] != 'u' || ctx->json[2] != 'e') {
        return LJ_PARSE_INVALID_VALUE;
    }
    ctx->json += 3;
    val->type = LJ_TRUE;
    return LJ_PARSE_OK;
}

static int
lj_parse_false(lj_context_t *ctx, lj_value_t *val) {
    EXPECT(ctx, 'f');
    if (ctx->json[0] != 'a' || ctx->json[1] != 'l' || ctx->json[2] != 's' || ctx->json[3] != 'e') {
        return LJ_PARSE_INVALID_VALUE;
    }
    ctx->json += 4;
    val->type = LJ_FALSE;
    return LJ_PARSE_OK;
}

static int
lj_parse_number(lj_context_t *ctx, lj_value_t *val) {
    char *end;
    val->n = strtod(ctx->json, &end);

    if (ctx->json == end) {
        return LJ_PARSE_INVALID_VALUE;
    }
    ctx->json = end;
    val->type = LJ_NUMBER;
    return LJ_PARSE_OK;
}

static int
lj_parse_value(lj_context_t *ctx, lj_value_t *val) {
    switch (*ctx->json) {
        case 'n':
            return lj_parse_null(ctx, val);
        case 't':
            return lj_parse_true(ctx, val);
        case 'f':
            return lj_parse_false(ctx, val);
        case '\0':
            return LJ_PARSE_EXPECT_VALUE;
        default:
            return lj_parse_number(ctx, val);
    }
}

int
lj_parse(lj_value_t *val, const char *json) {
    assert(val != NULL);

    int          ret;
    lj_context_t ctx;
    ctx.json  = json;
    val->type = LJ_NULL;

    lj_parse_whitespace(&ctx);
    ret = lj_parse_value(&ctx, val);

    if (ret == LJ_PARSE_OK) {
        lj_parse_whitespace(&ctx);
        if (ctx.json[0] != '\0') {
            ret = LJ_PARSE_ROOT_NOT_SINGULAR;
        }
    }

    return ret;
}

lj_type_t
lj_get_type(const lj_value_t *val) {
    assert(val != NULL);
    return val->type;
}

double
lj_get_number(const lj_value_t *val) {
    assert(val != NULL && val->type == LJ_NUMBER);
    return val->n;
}