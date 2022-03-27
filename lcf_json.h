//
// Created by nim on 2022/3/26.
//

#ifndef INCLUDE_LCF_JSON_H
#define INCLUDE_LCF_JSON_H

typedef enum lcf_json_type_e         lj_type_t;
typedef enum lcf_json_parse_status_e lj_parse_status_t;
typedef struct lcf_json_value_s      lj_value_t;
typedef struct lcf_json_context_s    lj_context_t;

enum lcf_json_type_e { LJ_NULL, LJ_OBJECT, LJ_ARRAY, LJ_TRUE, LJ_FALSE, LJ_NUMBER, LJ_STRING };

enum lcf_json_parse_status_e {
    LJ_PARSE_OK = 0,
    LJ_PARSE_EXPECT_VALUE,       // ws
    LJ_PARSE_ROOT_NOT_SINGULAR,  // ws value ws value
    LJ_PARSE_INVALID_VALUE,      // other errors
    LJ_PARSE_NUMBER_TOO_BIG
};

struct lcf_json_value_s {
    double    n;
    lj_type_t type;
};

struct lcf_json_context_s {
    const char *json;
};

/**
 * Parse the string literal of Json.
 * @param val json value
 * @param json the json string
 * @return parse status
 */
int lj_parse(lj_value_t *val, const char *json);

lj_type_t lj_get_type(const lj_value_t *val);

double lj_get_number(const lj_value_t *val);

#endif  // INCLUDE_LCF_JSON_H
