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
#include <stdlib.h> // For malloc()
#include <string.h>
#include <ctype.h> // toupper()
#include <math.h> // NAN, round
#include <inttypes.h> // To format integers // todo optional iff stdio used
#include <stdio.h> // For sprintf()   // todo make it optional

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
/**
 * The maximum length a basic roman numeral string may have.
 *
 * It includes the null terminator `\0`.
 *
 * That is the length of the roman numeral `-MMMDCCCLXXXVIII\0`
 * with value 3888.
 */
#define NUMERUS_BASIC_MAX_LEN (17U)
#define NUMERUS_EXTENDED_MAX_LEN (37U)
#define NUMERUS_EXTENDED_OVERLINED_MAX_LEN (53U)
#define NUMERUS_ZERO_ROMAN_LEN (6U)

typedef enum
{
    NUMERUS_OK = 0,
    NUMERUS_ERR_NULL_NUMERAL,
    NUMERUS_ERR_NULL_FRACTION,
    NUMERUS_ERR_NULL_DOUBLE,
    NUMERUS_ERR_NULL_INT,
    NUMERUS_ERR_VALUE_OUT_OF_RANGE,
    NUMERUS_ERR_NOT_FINITE_DOUBLE,
    NUMERUS_ERR_PARSING_EMPTY_NUMERAL,
    NUMERUS_ERR_PARSING_INVALID_SYNTAX,
    NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM,
    NUMERUS_ERR_PARSING_M_AFTER_VINCULUM,
    NUMERUS_ERR_NULL_FORMATTED,
    NUMERUS_ERR_MALLOC_FAILURE,
} numerus_err_t;

extern const char NUMERUS_ZERO_ROMAN[NUMERUS_ZERO_ROMAN_LEN];

typedef struct
{
    int32_t int_part;
    int32_t twelfths;
} numerus_frac_t;


//  --------------- From roman numeral to int/double/fraction ---------------
numerus_err_t numerus_roman_to_int(int32_t* value, const char* numeral);
numerus_err_t numerus_roman_extended_to_fraction(
        numerus_frac_t* fraction,
        const char* numeral);
numerus_err_t
numerus_roman_extended_to_double(double* real, const char* numeral);

//  --------------- From int/double/fraction to roman numeral ---------------
numerus_err_t
numerus_to_roman(char* numeral, int_fast32_t value);

numerus_err_t
numerus_to_roman_extended_fraction(
        char* numeral, numerus_frac_t fraction);

numerus_err_t
numerus_to_roman_extended_double(char* numeral, double value);


//  --------------- From int/double/fraction to roman numeral with allocation
numerus_err_t
numerus_to_roman_alloc(char** numeral, int_fast32_t value);

numerus_err_t
numerus_to_roman_extended_fraction_alloc(
        char** numeral, numerus_frac_t fraction);

numerus_err_t
numerus_to_roman_extended_double_alloc(char** numeral, double value);


// --------------- Utilities on roman numerals ---------------
bool numerus_is_zero(const char* numeral);
int_fast8_t numerus_sign(const char* numeral);


// --------------- Utilities on fractions ---------------
numerus_err_t
numerus_simplify_fraction(numerus_frac_t* fraction);
numerus_err_t numerus_fraction_to_double(
        double* result,
        numerus_frac_t fraction);
numerus_err_t numerus_double_to_fraction(
        numerus_frac_t* fraction,
        double real);


// --------------- Formatting ---------------
numerus_err_t numerus_fmt_fraction(
        char* formatted, numerus_frac_t fraction);
numerus_err_t numerus_fmt_overlined_wineol(
        char* formatted, const char* numeral);
numerus_err_t numerus_fmt_overlined_unixeol(
        char* formatted, const char* numeral);

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_H_ */
