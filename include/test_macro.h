#ifndef TEST_MACRO_H_INCLUDE
#define TEST_MACRO_H_INCLUDE

#define EXPECT_EQ_BASE(equality, expect, actual, format)                                           \
    do {                                                                                           \
        test_count++;                                                                              \
        if (equality)                                                                              \
            test_pass++;                                                                           \
        else {                                                                                     \
            fprintf(stderr,                                                                        \
                    "%s:%d: expect: " format " actual: " format "\n",                              \
                    __FILE__,                                                                      \
                    __LINE__,                                                                      \
                    expect,                                                                        \
                    actual);                                                                       \
            main_ret = 1;                                                                          \
        }                                                                                          \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%lf")

#define EXPECT_EQ_STRING(expect_str, actual_str, actual_len)                                       \
    EXPECT_EQ_BASE(sizeof(expect_str) - 1 == (actual_len) &&                                       \
                           memcmp(expect_str, actual_str, actual_len) == 0,                        \
                   expect_str,                                                                     \
                   actual_str,                                                                     \
                   "%s")

#define EXPECT_EQ_TRUE(actual) EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s")

#define EXPECT_EQ_FALSE(actual) EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s")

#define TEST_ERROR(error, json)                                                                    \
    do {                                                                                           \
        lj_value_t v;                                                                              \
        v.type = LJ_FALSE;                                                                         \
        EXPECT_EQ_INT(error, lj_parse(&v, json));                                                  \
        EXPECT_EQ_INT(LJ_NULL, lj_get_type(&v));                                                   \
    } while (0)

#define TEST_NUMBER(expect, json)                                                                  \
    do {                                                                                           \
        lj_value_t v;                                                                              \
        EXPECT_EQ_INT(LJ_PARSE_OK, lj_parse(&v, json));                                            \
        EXPECT_EQ_INT(LJ_NUMBER, lj_get_type(&v));                                                 \
        EXPECT_EQ_DOUBLE(expect, lj_get_number(&v));                                               \
    } while (0)

#endif  // TEST_MACRO_H_INCLUDE