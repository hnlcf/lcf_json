//
// Created by nim on 2022/3/26.
//

#ifndef LCF_JSON_H_INCLUDE
#define LCF_JSON_H_INCLUDE

#include <stddef.h>

#define lj_memcpy(dst, src, len)                                                                   \
    do {                                                                                           \
        memcpy(dst, src, len);                                                                     \
    } while (0)

#define lj_init(val)                                                                               \
    do {                                                                                           \
        (val)->type = LJ_NULL;                                                                     \
    } while (0)

#define lj_malloc(type, len) (type) malloc((len) + 1)
#define lj_set_null(val)     lj_free(val)

typedef unsigned char u_char;
typedef intptr_t      lj_int_t;
typedef uintptr_t     lj_uint_t;


typedef enum lcf_json_bool_e         lj_bool_t;
typedef enum lcf_json_type_e         lj_type_t;
typedef enum lcf_json_parse_status_e lj_parse_status_t;
typedef struct lcf_json_value_s      lj_value_t;
typedef struct lcf_json_string_s     lj_string_t;
typedef struct lcf_json_context_s    lj_context_t;

enum lcf_json_bool_e { false = 0, true = 1 };

enum lcf_json_type_e { LJ_NULL, LJ_OBJECT, LJ_ARRAY, LJ_TRUE, LJ_FALSE, LJ_NUMBER, LJ_STRING };

enum lcf_json_parse_status_e {
    LJ_PARSE_OK = 0,
    LJ_PARSE_EXPECT_VALUE,       // ws
    LJ_PARSE_ROOT_NOT_SINGULAR,  // ws value ws value
    LJ_PARSE_INVALID_VALUE,      // other errors
    LJ_PARSE_NUMBER_TOO_BIG
};

struct lcf_json_string_s {
    u_char *data;
    size_t  len;
};

struct lcf_json_value_s {
    union {
        double      n;
        lj_string_t str;
    };
    lj_type_t type;
};

struct lcf_json_context_s {
    const char *json;
    char *      stack;
    size_t      size;
    size_t      top;
};

/**
 * Parse the string literal of Json.
 * @param val json value
 * @param json the json string
 * @return parse status
 */
int lj_parse(lj_value_t *val, const char *json);

void lj_free(lj_value_t *val);

lj_type_t lj_get_type(const lj_value_t *val);

lj_bool_t lj_get_boolean(const lj_value_t *val);

double lj_get_number(const lj_value_t *val);

lj_string_t lj_get_string(const lj_value_t *val);

void lj_set_boolean(const lj_value_t *val, lj_bool_t b);

void lj_set_number(const lj_value_t *val, double n);

void lj_set_string(lj_value_t *val, const char *str, size_t len);

#endif  // LCF_JSON_H_INCLUDE
