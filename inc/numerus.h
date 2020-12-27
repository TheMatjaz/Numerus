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

/** Largest value (int16) a basic roman numeral can represent. */
#define NUMERUS_BASIC_MAX (+3999)
/** Smallest value (int16) a basic roman numeral can represent. */
#define NUMERUS_BASIC_MIN (-3999)
/** Largest integer value (int32) an extended roman numeral can represent. */
#define NUMERUS_EXTENDED_INT_MAX (+3999999L)
/** Smallest integer value (int32) an extended roman numeral can represent. */
#define NUMERUS_EXTENDED_INT_MIN (-3999999L)
/** Largest real number (double) an extended roman numeral can represent. */
#define NUMERUS_EXTENDED_MAX (+3999999.9166666665)
/** Smallest real number (double) an extended roman numeral can represent. */
#define NUMERUS_EXTENDED_MIN (-3999999.9166666665)

//  --------------- Buffer sizes and maximum string sizes ---------------
/**
 * Maximum length a basic roman numeral string may have, excluding the
 * null terminator, thus comparable with strlen().
 *
 * That is the length of the roman numeral `-MMMDCCCLXXXVIII`
 * with value 3888.
 */
#define NUMERUS_BASIC_MAX_LEN_NO_TERM (16U)

/**
 * Maximum length a basic roman numeral string may have, including the
 * null terminator, thus useful for buffer sizes.
 *
 * That is the length of the roman numeral `-MMMDCCCLXXXVIII\0`
 * with value 3888.
 */
#define NUMERUS_BASIC_MAX_LEN_WITH_TERM (NUMERUS_BASIC_MAX_LEN_NO_TERM + 1U)

/**
 * Maximum length an extended roman numeral string may have, excluding
 * the null terminator, thus comparable with strlen().
 *
 * That is the length of the roman numeral
 * `-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
#define NUMERUS_EXTENDED_MAX_LEN_NO_TERM (36U)

/**
 * Maximum length an extended roman numeral string may have, including
 * the null terminator, thus useful for buffer sizes.
 *
 * That is the length of the roman numeral
 * `-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....\0`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
#define NUMERUS_EXTENDED_MAX_LEN_WITH_TERM \
    (NUMERUS_EXTENDED_MAX_LEN_NO_TERM + 1U)

/**
 * Maximum length an extended roman numeral formatted by numerus_fmt_overline()
 * may have, excluding the null terminator, thus comparable with strlen().
 *
 * This is the length of the string
 * ` _______________\r\n-MMMDCCCLXXXVIIIDCCCLXXXVIIIS.....`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
#define NUMERUS_EXTENDED_OVERLINED_MAX_LEN_NO_TERM (52U)

/**
 * Maximum length an extended roman numeral formatted by numerus_fmt_overline()
 * may have, including the null terminator, thus useful for buffer sizes.
 *
 * This is the length of the string
 * ` _______________\r\n-MMMDCCCLXXXVIIIDCCCLXXXVIIIS.....\0`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
#define NUMERUS_EXTENDED_OVERLINED_MAX_LEN_WITH_TERM \
    (NUMERUS_EXTENDED_OVERLINED_MAX_LEN_NO_TERM + 1U)

/**
 * Exact length of the roman numeral with value 0 (zero) #NUMERUS_ZERO_ROMAN
 * excluding the null terminator, thus comparable with strlen().
 */
#define NUMERUS_ZERO_ROMAN_LEN_NO_TERM (5U)

/**
 * Exact length of the roman numeral with value 0 (zero) #NUMERUS_ZERO_ROMAN
 * including the null terminator, thus useful for buffer sizes.
 */
#define NUMERUS_ZERO_ROMAN_LEN_WITH_TERM (NUMERUS_ZERO_ROMAN_LEN_NO_TERM + 1U)

/** Roman numeral with value 0 (zero).
 * Length without terminator: #NUMERUS_ZERO_ROMAN_LEN_NO_TERM.
 * Length with terminator: #NUMERUS_ZERO_ROMAN_LEN_WITH_TERM.
 */
extern const char NUMERUS_ZERO_ROMAN[NUMERUS_ZERO_ROMAN_LEN_WITH_TERM];


/** Error codes returned by Numerus functions. */
typedef enum
{
    NUMERUS_OK = 0, ///< Successful completion
    NUMERUS_ERR_NULL_NUMERAL, ///< Roman numeral must be not-NULL
    NUMERUS_ERR_NULL_FRACTION, ///< Fraction #numerus_frac_t must be not-NULL
    NUMERUS_ERR_NULL_DOUBLE, ///< Double value must be not-NULL
    NUMERUS_ERR_NULL_INT,  ///< Integer value must be not-NULL
    NUMERUS_ERR_NULL_FORMATTED, ///< Formatted string must be not-NULL
    NUMERUS_ERR_VALUE_OUT_OF_RANGE, ///< Too large/small value to convert
    NUMERUS_ERR_NOT_FINITE_DOUBLE, ///< Cannot operate on non finite double
    NUMERUS_ERR_PARSING_EMPTY_NUMERAL, ///< Roman numeral is empty/only spaces
    NUMERUS_ERR_PARSING_INVALID_SYNTAX, ///< Illegal roman char or order  // TODO link to syntax file
    NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM, ///< Missing end of vinculum
    /** The 'M' roman char is not allowed after the vinculum's end */
    NUMERUS_ERR_PARSING_M_AFTER_VINCULUM,
    NUMERUS_ERR_MALLOC_FAILURE, ///< Heap memory allocation failed
} numerus_err_t;


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
/**
 *
 * @param [out] formatted
 * @param [in] fraction
 * @retval
 */
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
