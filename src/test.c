//
// Created by nim on 2022/3/26.
//
#include "lcf_json.h"
#include "test_macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TEST_NUMBER_FLAG
#define TEST_NUMBER_FLAG 1
#endif

static int main_ret   = 0;
static int test_count = 0;
static int test_pass  = 0;

static void test_parse_null();
static void test_parse_true();
static void test_parse_false();
static void test_parse_number();
static void test_parse_expect_value();
static void test_parse_invalid_value();
static void test_parse_root_not_singular();
static void test_parse_number_too_big();
static void test_access_string();
static void test_parse();

int
main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}

static void
test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
    test_access_string();
}

static void
test_parse_null() {
    lj_value_t v;
    v.type = LJ_TRUE;
    EXPECT_EQ_INT(LJ_PARSE_OK, lj_parse(&v, "null"));
    EXPECT_EQ_INT(LJ_NULL, lj_get_type(&v));
}

static void
test_parse_true() {
    lj_value_t v;
    v.type = LJ_FALSE;
    EXPECT_EQ_INT(LJ_PARSE_OK, lj_parse(&v, "true"));
    EXPECT_EQ_INT(LJ_TRUE, lj_get_type(&v));
}

static void
test_parse_false() {
    lj_value_t v;
    v.type = LJ_TRUE;
    EXPECT_EQ_INT(LJ_PARSE_OK, lj_parse(&v, "false"));
    EXPECT_EQ_INT(LJ_FALSE, lj_get_type(&v));
}

static void
test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

    TEST_NUMBER(1.0000000000000002, "1.0000000000000002");           /* the smallest number > 1 */
    TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308"); /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER(2.2250738585072014e-308,
                "2.2250738585072014e-308"); /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308"); /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

static void
test_parse_expect_value() {
    TEST_ERROR(LJ_PARSE_EXPECT_VALUE, "");
    TEST_ERROR(LJ_PARSE_EXPECT_VALUE, " ");
}

static void
test_parse_invalid_value() {
    /* invalid string */
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "?");

#if TEST_NUMBER_FLAG
    /* invalid number */
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(LJ_PARSE_INVALID_VALUE, "nan");
#endif
}

static void
test_parse_root_not_singular() {
    TEST_ERROR(LJ_PARSE_ROOT_NOT_SINGULAR, "null x");

#if TEST_NUMBER_FLAG
    /* invalid number */
    TEST_ERROR(LJ_PARSE_ROOT_NOT_SINGULAR,
               "0123"); /* after zero should be '.' , 'E' , 'e' or nothing */
    TEST_ERROR(LJ_PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(LJ_PARSE_ROOT_NOT_SINGULAR, "0x123");
#endif
}

static void
test_parse_number_too_big() {
#if TEST_NUMBER_FLAG
    TEST_ERROR(LJ_PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(LJ_PARSE_NUMBER_TOO_BIG, "-1e309");
#endif
}

static void
test_access_string() {
    lj_value_t  v;
    lj_string_t str;
    lj_init(&v);

    lj_set_string(&v, "", 0);
    str = lj_get_string(&v);
    EXPECT_EQ_STRING("", str.data, str.len);

    lj_set_string(&v, "Hello", 5);
    str = lj_get_string(&v);
    EXPECT_EQ_STRING("Hello", str.data, str.len);

    lj_free(&v);
}