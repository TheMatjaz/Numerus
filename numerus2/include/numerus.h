/**
 * @file
 * @brief Numerus roman numerals library API.
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NMRS_H_
#define NMRS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Version of the Numerus library using semantic versioning.
 *
 * <https://semver.org/spec/v2.0.0.html>
 */
#define NMRS_VERSION "v3.0.0"

#define NMRS_UNIX_EOL 0U
#define NMRS_WINDOWS_EOL 1U

/**
 * Smallest negative value of a basic roman numeral, without Numerus
 * extensions.
 */
extern const int16_t NMRS_MIN_BASIC_VALUE;

/**
 * Biggest positive value of a basic roman numeral, without Numerus
 * extensions.
 */
extern const int16_t NMRS_MAX_BASIC_VALUE;

/**
 * Smallest negative value of an extended roman numeral.
 *
 * That is the smallest number with a vinculum and the fractional part.
 */
extern const float NMRS_MIN_EXTENDED_VALUE;

/**
 * Biggest positive value of an extended roman numeral.
 *
 * That is the biggest number with a vinculum and the fractional part.
 */
extern const float NMRS_MAX_EXTENDED_VALUE;

/**
 * Smallest negative value the integer part of an extended roman numeral.
 *
 * That is the smallest number with a vinculum but without the fractional part.
 */
extern const int32_t NMRS_MIN_EXTENDED_VALUE_INT_PART;

/**
 * Biggest positive value the integer part of an extended roman numeral.
 *
 * That is the biggest number with a vinculum but without the fractional part.
 */
extern const int32_t NMRS_MAX_EXTENDED_VALUE_INT_PART;

/**
 * The maximum length a basic roman numeral string may hav.
 *
 * It includes the null terminator `\0`.
 *
 * That is the length of the roman numeral `-MMMDCCCLXXXVIII\0`
 * with value 3888.
 */
extern const uint8_t NMRS_MAX_BASIC_LENGTH;

/**
 * The maximum length an extended roman numeral string may have
 *
 * It includes the null terminator `\0`.
 *
 * That is the length of the roman numeral
 * `-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....\0`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
extern const uint8_t NMRS_MAX_EXTENDED_LENGTH;

/**
 * The maximum length an overlined extended roman numeral string may have.
 *
 * It includes the null terminator `\0`.
 *
 * This is the length of the maximum string that numerus_overline() may
 * output.
 *
 * Specifically it is the length of the roman numeral
 * @verbatim
 *  _______________\r\n
 * -MMMDCCCLXXXVIIIDCCCLXXXVIIIS.....\0
 * @endverbatim
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
extern const uint8_t NMRS_MAX_EXTENDED_OVERLINED_LENGTH;

/**
 * The roman numeral of value 0 (zero).
 *
 * Both for positive and negative zero.
 */
extern const char* const NMRS_ZERO_NUMERAL;

/**
 * The error code returned by any Numerus function.
 *
 * As common practice, the value of 0 (false) indicates no error.
 * The values fit within a `uint8_t`.
 */
typedef enum
{
    /**
     * Successful operation.
     */
    NMRS_OK = 0U,

    /**
     * An unknown or unspecified error happened.
     *
     * The error did not match any other error scenario Numerus is prepared to
     * handle.
     */
    NMRS_ERROR_GENERIC,

    /**
     * The roman numeral contains mispositioned characters.
     *
     * This is the most generic error for a syntax failure of the roman
     * numeral. It usually means that a repeatable roman character is repeated
     * non-consecutively (like `MCIM`) or that a non-repeatable roman
     * character has been repeated (like `DD`).
     */
    NMRS_ERROR_ILLEGAL_CHAR_SEQUENCE,

    /**
     * The value to be converted to basic roman numeral is out of range.
     *
     * The value should be within
     * [#NMRS_MIN_BASIC_VALUE, #NMRS_MAX_BASIC_VALUE].
     */
    NMRS_ERROR_BASIC_VALUE_OUT_OF_RANGE,

    /**
     * The value to be converted to extended roman numeral is out of range.
     *
     * The value should be within
     * [#NMRS_MIN_EXTENDED_VALUE, #NMRS_MAX_EXTENDED_VALUE].
     */
    NMRS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE,

    /**
     * The value to be converted to extended roman numeral must not be
     * +/- Infinity or NaN.
     *
     * At the same time, the value should be within
     * [#NMRS_MIN_EXTENDED_VALUE, #NMRS_MAX_EXTENDED_VALUE].
     */
    NMRS_ERROR_DOUBLE_VALUE_IS_NOT_FINITE,

    /**
     * The basic roman numeral contains a character that is not accepted.
     *
     * The only allowed characters are: `MDCLXVI-` both lowercase and
     * uppercase. The only exception is the #NMRS_ZERO_STRING string.
     */
    NMRS_ERROR_ILLEGAL_BASIC_CHARACTER,

    /**
     * The extended roman numeral contains a character that is not accepted.
     *
     * The only allowed characters are: `MDCLXVIS_.-` both lowercase and
     * uppercase. The only exception is the #NMRS_ZERO_STRING string.
     */
    NMRS_ERROR_ILLEGAL_EXTENDED_CHARACTER,

    /**
     * The basic roman numeral is too long to be syntactically correct.
     *
     * The max allowed length of a basic roman numeral is
     * #NMRS_MAX_BASIC_LENGTH.
     */
    NMRS_ERROR_TOO_LONG_BASIC_NUMERAL,

    /**
     * The extended roman numeral is too long to be syntactically correct.
     *
     * The max allowed length of an extended roman numeral is
     * #NMRS_MAX_BASIC_LENGTH.
     */
    NMRS_ERROR_TOO_LONG_EXTENDED_NUMERAL,

    /**
     * The roman numeral contains too many consecutive repetitions of a
     * repeatable character.
     *
     * Characters that are allowed to be repeated multiple times are `MCXI.`.
     * They have 3 maximum consecutive repetitions each, the dot `.` has 5.
     */
    NMRS_ERROR_TOO_MANY_REPEATED_CHARS,

    /**
     * The extended roman numeral contains only one underscore.
     *
     * A roman numeral may contain exactly 0 or 2 underscores, the
     * latter case being an extended numeral.
     */
    NMRS_ERROR_MISSING_SECOND_UNDERSCORE,

    /**
     * The extended roman numeral contains more than two underscores.
     *
     * A roman numeral may contain exactly 0 or 2 underscores, the
     * latter case being an extended numeral.
     */
    NMRS_ERROR_TOO_MANY_UNDERSCORES,

    /**
     * The extended roman numeral contains the first underscore not at the
     * numeral start.
     *
     * The first underscore has to be placed at the first position of the
     * extended numeral, after the minus in case there is one.
     */
    NMRS_ERROR_ILLEGAL_FIRST_UNDERSCORE_POSITION,

    /**
     * The extended roman numeral contains decimal characters not
     * at the numeral end.
     *
     * Decimal characters `S.` are allowed only at the end of the numeral.
     */
    NMRS_ERROR_FRACTIONAL_CHARS_NOT_AT_END,
    NMRS_ERROR_FRACTIONAL_CHARS_BETWEEN_UNDERSCORES,


    /**
     * The roman numeral contains a misplaced minus `-` or more than one.
     *
     * A roman numeral must contain exactly 0 or 1 minus symbol. If existing,
     * it must be at the very first position of the numeral.
     */
            NMRS_ERROR_ILLEGAL_MINUS_POSITION,

    /**
     * The extended roman numeral contains an `M` not between underscores.
     *
     * An extended roman numeral may contain an `M` only between the
     * underscores, if they are used. Values bigger than 1000 (`M`) are
     * represented between underscores, making an `M` character after the
     * underscores illogical.
     */
            NMRS_ERROR_M_AFTER_UNDERSCORES,

    /**
     * Heap memory allocation failure.
     *
     * The #numerus_malloc function failed to allocate memory.
     * The operation could not be completed.
     */
            NMRS_ERROR_ALLOC_FAIL,

    /**
     * The pointer to the roman numeral string is NULL.
     */
            NMRS_ERROR_NULL_NUMERAL,

    /**
     * The pointer to a number is NULL.
     */
            NMRS_ERROR_NULL_NUMBER,

    /**
     * The roman numeral string is empty or filled with only whitespace.
     *
     * Insert some non-whitespace characters in the string.
     */
            NMRS_ERROR_EMPTY_NUMERAL,
} nmrs_err_t;
i

typedef struct
{
    int32_t int_part : 22;
    int32_t twelfths : 4;
    // This kind of struct fits in 4 bytes and does not require padding.
} nmrs_value_t;

/* CONVERSIONS FROM VALUE TO NUMERAL */
/**
 * Converts an integer to a basic roman numeral.
 *
 * Accepts any integer within
 * [#NMRS_MIN_BASIC_VALUE, #NMRS_MAX_BASIC_VALUE] and generates
 * a basic numeral without any Numerus extensions representing the same value.
 *
 * The function can either allocate the string dynamically or fill a provided
 * buffer, based on the \p numeral parameter.
 *
 * @anchor numeral_allocation
 * @remark **Important!**
 * The \p numeral parameter will contain the generated numeral at execution
 * end, but also allows the user to decide who allocates the buffer for the
 * numeral to be written in. Either way it's up to the user to deallocate
 * that memory.
 *
 * - if \p numeral is a NULL pointer, the function returns an error;
 *
 *   \code{C}
 *   numerus_status_t status;
 *
 *   status = numerus_int_to_basic_roman(42, NULL);
 *   // status == NMRS_ERROR_NULL_NUMERAL
 *   \endcode
 *
 *
 * - if \p numeral points to a NULL pointer, a string will be allocated on the
 *   heap memory using #numerus_malloc and returned through the same parameter.
 *   It's **up to the user to free it!** It stays unchanged in case of errors.
 *   The memory allocation procedure will allocate exactly the space required
 *   to fit the numeral, including its null terminator '\0'.
 *
 *   \code{C}
 *   char* numeral = NULL;
 *   numerus_status_t status;
 *
 *   status = numerus_int_to_basic_roman(42, &numeral);
 *   // status == NMRS_OK
 *   // numeral points to "XLII\0", which was allocated in the function.
 *
 *   status = numerus_int_to_basic_roman(983492875, &numeral);
 *   // status == NMRS_ERROR_BASIC_VALUE_OUT_OF_RANGE
 *   // numeral still points to "XLII\0".
 *
 *   numerus_free(numeral);
 *   // The memory location of the "XLII\0" string is released.
 *   \endcode
 *
 *
 * - if \p numeral points to a non-NULL location, that will be considered as
 *   the buffer to fill with the numeral. The buffer will be **ASSUMED** to
 *   have #NMRS_MAX_BASIC_LENGTH size for basic numerals an
 *   #NUMERAL_MAX_EXTENDED_LENGTH for extended numerals. The buffer will be
 *   set to an empty string in case of error. In both cases (error or not)
 *   the bytes after the null terminator `\0` are unchanged.
 *
 *   \code{C}
 *   char numeral_buffer[NMRS_MAX_BASIC_LENGTH];
 *   numerus_status_t status;
 i*
 *   status = numerus_int_to_basic_roman(42, &numeral_buffer);
 *   // status == NMRS_OK
 *   // numeral_buffer contains "XLII\0".
 *
 *   status = numerus_int_to_basic_roman(983492875, &numeral_buffer);
 *   // status == NMRS_ERROR_BASIC_VALUE_OUT_OF_RANGE
 *   // numeral_buffer contains "\0".
 *   \endcode
 *
 * @param[in] value the integer value to be converted. Can be negative.
 * @param[in, out] p_numeral pointer to the generated numeral string. Check its
 *        **remarks**, as it can be either allocated by the user or by
 *        this function.
 * @return status code, indicating operation success or failure.
 * @see numerus_int_to_extended_roman
 * @see numerus_double_to_extended_roman
 */
nmrs_err_t nmrs_int_to_basic_numeral(int16_t value, char** numeral);

nmrs_int_to_basic_numeral_a(int16_t value, void* numeral);

/**
 * Converts an integer and a fractional part (in twelfths) to an extended
 * roman numeral.
 *
 * Accepts any integer part within
 * [#NMRS_MIN_EXTENDED_VALUE_INT_PART, #NMRS_MAX_EXTENDED_VALUE_INT_PART]
 * and any fractional part as long as the combined value is within
 * [#NMRS_MIN_EXTENDED_VALUE, #NMRS_MAX_EXTENDED_VALUE]. Generates
 * an extended numeral with the Numerus extensions representing the same
 * combined value.
 *
 * @remark Same **important** remarks apply as for the
 * numerus_int_to_basic_roman() function regarding the usage of the \p numeral
 * parameter.
 *
 * @param[in] integer_part the integer part of the value of the
 *        to-be extended roman numeral. Can be negative.
 * @param[in] twelfths the fractional part to sum to the \p integer_part.
 *        Can be negative.
 * @param[in, out] p_numeral pointer to the generated numeral string. Check its
 *        **remarks**, as it can be either allocated by the user or by
 *        this function. It's up to the user to free this string.
 * @return status code, indicating operation success or failure.
 * @see numerus_int_to_basic_roman
 * @see numerus_double_to_extended_roman
 */
nmrs_err_t nmrs_int_to_extended_numeral(nmrs_value_t valu, char* p_numeral);

/**
 * Converts a double-precision floating point value to an extended
 * roman numeral.
 *
 * Accepts any double within
 * [#NMRS_MIN_EXTENDED_VALUE, #NMRS_MAX_EXTENDED_VALUE] and generates
 * an extended numeral with the Numerus extensions representing the same value.
 *
 * @remark Same **important** remarks apply as for the
 * numerus_int_to_basic_roman() function regarding the usage of the \p numeral
 * parameter.
 *
 * @param[in] double_value the value of the numeral. Can be negative.
 *        Must be finite.
 * @param[in, out] p_numeral pointer to the generated numeral string. Check its
 *        **remarks**, as it can be either allocated by the user or by
 *        this function. It's up to the user to free this string.
 * @return status code, indicating operation success or failure.
 * @see numerus_int_to_basic_roman
 * @see numerus_int_to_extended_roman
 */
nmrs_err_t nmrs_float_to_extended_numeral(
        float value, char* p_numeral);

/* CONVERSIONS FROM NUMERAL TO VALUE */
nmrs_err_t nmrs_basic_numeral_to_int(
        const char* numeral, int16_t* p_value);

/**
 * Converts a roman numeral to its value expressed as pair of its integer part
 * and number of twelfths.
 *
 * Accepts many variations of roman numerals:
 *
 * - it's case INsensitive
 * - accepts negative roman numerals (with leading minus '-')
 * - accepts long roman numerals (with character between underscores to denote
 *   the part that has a value multiplied by 1000)
 * - accepts decimal value of the roman numerals, those are twelfths (with
 *   the characters 'S' and dot '.')
 * - all combinations of the above
 *
 * The parsing status of the roman numeral (any kind of wrong syntax)
 * is stored in the errcode passed as parameter, which can be NULL to ignore
 * the error, although it's not recommended. If the the error code is different
 * than NMRS_OK, an error occurred during the conversion and the returned
 * value is outside the possible range of values of roman numerals.
 * The error code may help find the specific error.
 *
 * The number of twelfths is stored in the passed parameter, while the integer
 * part is returned directly.
 *
 * @param *roman string with a roman numeral
 * @param *errcode int where to store the conversion status: NMRS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @param *twelfths number of twelfths from 0 to 11. NULL is interpreted as 0
 * twelfths.
 * @returns int32_t as the integer part of the value of the roman numeral or a
 * value outside the the possible range of values when an error occurs.
 */
nmrs_err_t nmrs_extended_numeral_to_int(
        const char* numeral, nmrs_value_t* value);
nmrs_err_t nmrs_extended_numeral_to_float(
        const char* numeral, float* value);


/* FRACTIONAL FORMAT MANAGEMENT */
/**
 * Converts a value expressed as integer part and twelfths to a double.
 *
 * Returns an error if the summed value \p integer_part + \p twelfths
 * is not in the range
 * [#NMRS_MIN_EXTENDED_VALUE, #NMRS_MAX_EXTENDED_VALUE].
 *
 * @param *integer_part long integer part of the value.
 * @param *twelfths fractional part of the value in twelfths.
 * @returns status code, indicating operation success or failure.
 */
nmrs_err_t nmrs_int_parts_to_float(
        nrms_value_t value, float* result);

/**
 * Splits a double value to its integer part and a number of twelfths.
 *
 * The twelfths are obtained by rounding the double value to the nearest
 * twelfth multiple.
 *
 * Accepts any finite double within
 * [#NMRS_MIN_EXTENDED_VALUE -1.0/24, #NMRS_MAX_EXTENDED_VALUE +1.0/24].
 * The range is extended by half a twelfth on each side as the values
 * get rounded.
 *
 * @examples
 * - `10.0` becomes `10 and 0/12 = 10.0`
 * - `-2.8` becomes `-2 and -10/12 = -2.8333`
 * - `11.1` becomes `11 and 1/12 = 11.08333`
 * - `7.9` becomes `7 and 11/12 = 7.91666`
 *
 * @param value to convert to integer part and twelfths. Must be finite.
 * @param *integer_part long integer part of the value.
 * @param *twelfths fractional part of the value in twelfths.
 * @returns status code, indicating operation success or failure.
 */
nmrs_err_t nmrs_float_to_int_parts(
        float value, nmrs_value_t* result);

/**
 * Enforce twelfths to be in [-11, 11] and to match sign of the integer part.
 *
 * It's useful when the twelfths are more than 11 (or less than -11 if
 * negative). The transformation is performed so that the parts will carry the
 * same sign at the end.
 *
 * Returns an error if the summed value \p integer_part + \p twelfths
 * is not in the range
 * [#NMRS_MIN_EXTENDED_VALUE, #NMRS_MAX_EXTENDED_VALUE].
 *
 * In any case, the summed value \p integer_part + \p twelfths will be the
 * same before and after the computation, just simplified mathematically.
 *
 * @examples
 * - `-3 and 2/12` becomes `-2 and -10/12`
 * - `10 and 13/12` becomes `11 and 1/12`
 * - `10 and -25/12` becomes `7 and 11/12`
 * - `28 and 1/12` stays unaltered
 * - `0 and -3/12` stays unaltered
 *
 * @param *integer_part long integer part of the value.
 * @param *twelfths fractional part of the value in twelfths.
 * @returns status code, indicating operation success or failure.
 */
nmrs_err_t nmrs_simplify_twelfths(
        nmrs_value_t* value);


/* ANALYSIS OF NUMERAL STRINGS */
nmrs_err_t nmrs_is_zero(
        const char* numeral, bool* p_result);
nmrs_err_t nmrs_sign(
        const char* numeral, int8_t* p_result);
nmrs_err_t nmrs_is_basic_numeral(
        const char* numeral, bool* p_result);
nmrs_err_t nmrs_count_roman_chars(
        const char* numeral, uint8_t* p_result);


/* NUMERAL FORMATTING */
// Pass the string two times to overwrite it??
// Max result size #NMRS_MAX_EXTENDED_OVERLINED_LENGTH

/**
 * Converts the numeral to a printable representation with actual overlining.
 *
 * Generates a two lined string by overlining the part between underscores.
 * The end-of-line character is defined by \p windows_end_of_line as boolean.
 * In case of no overlining, it provides a copy of the original numeral.
 * Ignores any heading whitespace.
 *
 * It's up to the user to free the overlined string.
 *
 * @examples
 * - `-_CXX_VIII` becomes ` ___\n-CXXVIII` or ` ___\r\n-CXXVIII`
 * - `VIII` becomes `VIII`
 *
 * The printed strings look like this:
 *
 * @verbatim
 *                  ___
 * -_CXX_VIII  =>  -CXXVIII
 * VIII        =>   VIII
 * @endverbatim
 *
 * @remark Same **important** remarks apply as for the
 * numerus_int_to_basic_roman() function regarding the usage of the
 * \p p_formatted parameter.
 *
 * @param[in] numeral string to convert to an overlined version.
 *            The original is unchanged.
 * @param[in, out] pointer to the generated numeral string. Check its
 *        **remarks**, as it can be either allocated by the user or by
 *        this function. It's up to the user to free this string.
 * @param windows_end_of_line true sets the EOL characters to "\r\n"
 *        (Windows/DOS style), false to "\n" (Unix style).
 * @return status code, indicating operation success or failure.
 * @see numerus_int_to_basic_roman
 */
nmrs_err_t nmrs_overline(
        const char* numeral, char* p_formatted, bool windows_end_of_line);

nmrs_err_t nmrs_int_parts_to_string(
        nmrs_value_t* value, char* p_formatted);
nmrs_err_t nmrs_double_as_int_parts_string(
        float value, char* p_formatted);

#define NMRS_HAS_MALLOC
#ifdef NMRS_HAS_MALLOC
/**
 * Heap memory allocation function.
 *
 * Replace it with any preferred allocator.
 */
#define nmrs_malloc malloc

/**
 * Heap memory allocation function with zeroing-out.
 *
 * Replace it with any preferred allocator that clear the allocated space.
 */
#define nmrs_calloc calloc

/**
 * Heap memory freeing function.
 *
 * Replace it with any preferred allocation release function.
 */
#define nmrs_free free
#endif  /* NMRS_HAS_MALLOC */

#ifdef __cplusplus
}
#endif

#endif  /* NMRS_H_ */
