/**
 * @file
 * Numerus: roman numerals library API.
 *
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
#include <inttypes.h> // To format integers // todo optional iff stdio used
#include <string.h>
#include <ctype.h> // toupper()
#include <math.h> // NAN, round
#include <stdio.h> // For sprintf()   // todo make it optional


#if NUMERUS_HAS_MALLOC
#include <stdlib.h>
#endif

/** Version of the Numerus library using semantic versioning. */
#define NUMERUS_VERSION "v3.0.0"

//  ----- Extremes of the values represented by roman numerals ----------------
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

//  ----- Maximum string sizes a.k.a. buffer sizes ----------------------------
/**
 * Maximum length a basic roman numeral string may have, including the
 * null terminator, thus useful for buffer sizes.
 *
 * That is the length of the roman numeral `-MMMDCCCLXXXVIII\0`
 * with value 3888.
 */
#define NUMERUS_BASIC_MAX_LEN_WITH_TERM (16U + 1U)

/**
 * Maximum length an extended roman numeral string may have, including
 * the null terminator, thus useful for buffer sizes.
 *
 * That is the length of the roman numeral
 * `-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....\0`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
#define NUMERUS_EXTENDED_MAX_LEN_WITH_TERM (36U + 1U)

/**
 * Maximum length an extended roman numeral formatted by numerus_fmt_overline()
 * may have, including the null terminator, thus useful for buffer sizes.
 *
 * This is the length of the string
 * ` _______________\r\n-MMMDCCCLXXXVIIIDCCCLXXXVIIIS.....\0`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
#define NUMERUS_EXTENDED_OVERLINED_MAX_LEN_WITH_TERM (52U + 1U)

/**
 * Maximum length a #numerus_frac_t fraction formatted by
 * numerus_fmt_fraction() may have, including the null terminator,
 * thus useful for buffer sizes.
 *
 * This is the length of the string
 * `-12345, -11/12\0` (or any other 5-digit integer part)
 */
#define NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM (14U + 1U)

/**
 * Exact length of the roman numeral with value 0 (zero) #NUMERUS_ZERO_ROMAN
 * including the null terminator, thus useful for buffer sizes.
 */
#define NUMERUS_ZERO_ROMAN_LEN_WITH_TERM (5U + 1U)

/** Roman numeral with value 0 (zero).
 * Length without terminator: #NUMERUS_ZERO_ROMAN_LEN_NO_TERM.
 * Length with terminator: #NUMERUS_ZERO_ROMAN_LEN_WITH_TERM.
 */
extern const char NUMERUS_ZERO_ROMAN[NUMERUS_ZERO_ROMAN_LEN_WITH_TERM];


// ----- Error codes and data types -------------------------------------------
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

// Todo doxygen
typedef struct numerus_frac
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


//  ----- From int/double/fraction to roman numeral with heap-allocation ------
#if NUMERUS_HAS_MALLOC
numerus_err_t
numerus_to_roman_alloc(char** numeral, int_fast32_t value);

numerus_err_t
numerus_to_roman_extended_fraction_alloc(
        char** numeral, numerus_frac_t fraction);

numerus_err_t
numerus_to_roman_extended_double_alloc(char** numeral, double value);
#endif  /* NUMERUS_HAS_MALLOC */


// ----- Utilities on roman numerals ------------------------------------------
bool numerus_is_zero(const char* numeral);
int_fast8_t numerus_sign(const char* numeral);


// ----- Utilities on fractions -----------------------------------------------
numerus_err_t
numerus_simplify_fraction(numerus_frac_t* fraction);
numerus_err_t numerus_fraction_to_double(
        double* result,
        numerus_frac_t fraction);
numerus_err_t numerus_double_to_fraction(
        numerus_frac_t* fraction,
        double real);


// ---- Formatting ------------------------------------------------------------
/**
 * Formats the #numerus_frac_t fraction as a human-readable string.
 *
 * Examples:
 * - `{1, 0/12}` -> "1", simpler form when no twelfths
 * - `{1, 1/12}` -> "1, 1/12", twelfths after the comma
 * - `{1, 2/12}` -> "1, 1/6", twelfths are simplified
 * - `{-10, -9/12}` -> "-10, -3/4", negative numbers supported
 * - `{-10, +14/12} == {-9, +2/12} == {-8, -10/12}` -> "-8, -5/6",
 *   the whole fraction is simplified
 *
 * @param [out] formatted buffer of
 *     #NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM bytes where to write
 *     the fraction formatted to string
 * @param [in] fraction to convert to string
 * @retval #NUMERUS_OK on success
 * @retval #NUMERUS_NULL_FORMATTED when \p formatted is NULL
 * @retval #NUMERUS_ERR_VALUE_OUT_OF_RANGE when \p fraction is out of the
 *     allowed range of [#NUMERUS_EXTENDED_MIN, #NUMERUS_EXTENDED_MAX].
 */
numerus_err_t numerus_fmt_fraction(
        char* formatted, numerus_frac_t fraction);

/**
 * Formats a roman numeral as two lines, using underscores in the first
 * to overline the vinculum part of the numeral, and the numeral itself
 * in the second line.
 *
 * In case of no overlining, it provides a copy of the original numeral.
 * Does no checks whether the roman numeral uses the proper syntax and thus
 * correctly represents a valid value.
 *
 * The end-of-line (EOL) terminator chan be chosed to be Windows-style
 * "\r\n" (carriage return + line feed) or Unix-style "\n" (line feed only)
 * with the \p use_windows_eol parameter.
 *
 * Examples:
 * - `-_CXX_VIII` becomes ` ___\r\n-CXXVIII` or ` ___\n-CXXVIII`
 * - `VIII` becomes `VIII`
 *
 * The printed strings look like this:
 *
 *                      ___
 *     -_CXX_VIII  =>  -CXXVIII
 *     VIII        =>   VIII
 *
 * @param [out] formatted buffer of
 *     #NUMERUS_EXTENDED_OVERLINED_MAX_LEN_WITH_TERM bytes where to write
 *     the overlined numeral
 * @param [in] numeral to overline
 * @param [in] use_windows_eol when true, the EOL terminator will be "\r\n"
 *             otherwise is "\n"
 * @retval #NUMERUS_OK on success
 * @retval #NUMERUS_NULL_FORMATTED when \p formatted is NULL
 * @retval #NUMERUS_NULL_NUMERAL when \p numeral is NULL
 * @retval #NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM when \p numeral
 *         is missing the end of the vinculum, the second '_'.
 */
numerus_err_t numerus_fmt_overlined(
        char* formatted, const char* numeral, bool use_windows_eol);

#if NUMERUS_HAS_MALLOC
/**
 * Formats the #numerus_frac_t fraction as a human-readable heap-allocated
 * string.
 *
 * @copydetails numerus_fmt_fraction
 * @retval #NUMERUS_ERR_MALLOC_FAILURE when strdup() fails to allocated heap
 *         memory
 */
numerus_err_t numerus_fmt_fraction_alloc(
        char** formatted, numerus_frac_t fraction);

/**
 * Formats a roman numeral as two heap-allocated lines, using underscores in
 * the first to overline the vinculum part of the numeral, and the numeral
 * itself in the second line.
 *
 * @copydetails numerus_fmt_overlined
 * @retval #NUMERUS_ERR_MALLOC_FAILURE when strdup() fails to allocated heap
 *         memory
 */
numerus_err_t numerus_fmt_overlined_alloc(
        char** formatted, const char* numeral, bool use_windows_eol);

#endif /* NUMERUS_HAS_MALLOC */

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_H_ */
