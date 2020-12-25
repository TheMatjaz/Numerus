/**
 * @file
 * @brief
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NUMERUS_H_
#define NUMERUS_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // toupper()
#include <math.h> // NAN, round

/**
 * Version of the Numerus library using semantic versioning.
 *
 * <https://semver.org/spec/v2.0.0.html>
 */
#define NUMERUS_VERSION "v3.0.0"

#define NUMERUS_BASIC_MAX (+3999)
#define NUMERUS_BASIC_MIN (-3999)
#define NUMERUS_EXTENDED_INT_MAX (+3999999L)
#define NUMERUS_EXTENDED_INT_MIN (-3999999L)
#define NUMERUS_EXTENDED_MAX (+3999999.9166666665)
#define NUMERUS_EXTENDED_MIN (-3999999.9166666665)
#define NUMERUS_BASIC_MAX_LEN (17U)
#define NUMERUS_EXTENDED_MAX_LEN (37U)
#define NUMERUS_EXTENDED_OVERLINED_MAX_LEN (53U)
#define NUMERUS_ZERO_ROMAN_LEN (6U)

typedef enum
{
    NUMERUS_OK = 0,
    NUMERUS_ERR_NULL_NUMERAL,
    NUMERUS_ERR_VALUE_OUT_OF_RANGE,
    NUMERUS_ERR_NULL_FRACTION,
    NUMERUS_ERR_NULL_DOUBLE,
    NUMERUS_ERR_NOT_FINITE_DOUBLE,
} numerus_err_t;

extern const char* const NUMERUS_ZERO_ROMAN;

typedef char roman_t;

typedef struct
{
    int32_t int_part;
    int32_t twelfths;
} numerus_frac_t;

numerus_err_t
numerus_to_roman(char* numeral, int_fast16_t value);

numerus_err_t
numerus_to_roman_extended_twelfts(
        char* numeral, int32_t int_part, int8_t twelfts);

numerus_err_t
numerus_to_roman_extended_double(char* numeral, double value);


bool numerus_is_zero(const char* numeral);
int_fast8_t numerus_sign(const char* numeral);
bool numerus_is_valid(const char* numeral);


// --------------- Fractions ---------------
numerus_err_t
numerus_simplify_fraction(numerus_frac_t* fraction);
numerus_err_t numerus_fraction_to_double(
        double* result,
        const numerus_frac_t* fraction);
double numerus_fraction_to_double_v2(const numerus_frac_t* fraction);
numerus_err_t numerus_double_to_fraction(
        numerus_frac_t* fraction,
        double real);


#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_H_ */
