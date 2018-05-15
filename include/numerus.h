/**
 * @file numerus.h
 * @brief Numerus roman numerals library API.
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NUMERUS_NUMERUS3_H_
#define NUMERUS_NUMERUS3_H_

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
 * Version of this header file using semantic versioning.
 *
 * <https://semver.org/spec/v2.0.0.html>
 */
#define NUMERUS_API_VERSION "v3.0.0"

/**
 * Heap memory allocation function.
 */
#define numerus_malloc malloc

/**
 * Heap memory allocation function with zeroing-out.
 */
#define numerus_calloc calloc

/**
 * Heap memory freeing function.
 */
#define numerus_free free

/**
 * Smallest negative value of a basic roman numeral, without Numerus
 * extensions.
 */
extern const int16_t NUMERUS_MIN_BASIC_VALUE;

/**
 * Biggest positive value of a basic roman numeral, without Numerus
 * extensions.
 */
extern const int16_t NUMERUS_MAX_BASIC_VALUE;

/**
 * Smallest negative value of an extended roman numeral.
 *
 * That is the smallest number with a vinculum and the fractional part.
 */
extern const double NUMERUS_MIN_EXTENDED_VALUE;

/**
 * Biggest positive value of an extended roman numeral.
 *
 * That is the biggest number with a vinculum and the fractional part.
 */
extern const double NUMERUS_MAX_EXTENDED_VALUE;

/**
 * Smallest negative value the integer part of an extended roman numeral.
 *
 * That is the smallest number with a vinculum but without the fractional part.
 */
extern const int32_t NUMERUS_MIN_EXTENDED_VALUE_INT_PART;

/**
 * Biggest positive value the integer part of an extended roman numeral.
 *
 * That is the biggest number with a vinculum but without the fractional part.
 */
extern const int32_t NUMERUS_MAX_EXTENDED_VALUE_INT_PART;

/**
 * The maximum length a basic roman numeral string may hav.
 *
 * It includes the null terminator `\0`.
 *
 * That is the length of the roman numeral `-MMMDCCCLXXXVIII\0`
 * with value 3888.
 */
extern const uint8_t NUMERUS_MAX_BASIC_LENGTH;

/**
 * The maximum length an extended roman numeral string may have
 *
 * It includes the null terminator `\0`.
 *
 * That is the length of the roman numeral
 * `-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....\0`
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
extern const uint8_t NUMERUS_MAX_EXTENDED_LENGTH;

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
 *  _______________\n
 * -MMMDCCCLXXXVIIIDCCCLXXXVIIIS.....\0
 * @endverbatim
 * with value (-3888888 and -11/12) = -3888888.9166666665.
 */
extern const uint8_t NUMERUS_MAX_EXTENDED_OVERLINED_LENGTH;

/**
 * The roman numeral of value 0 (zero).
 *
 * Both for positive and negative zero.
 */
extern const char* const NUMERUS_ZERO_NUMERAL;


/**
 * The status code returned by any Numerus function.
 *
 * As common practice, the value of 0 (false) indicates no error.
 * The values fit within a `uint8_t`.
 */
typedef enum
{
    /**
     * Successful operation.
     */
            NUMERUS_OK = 0U,

    /**
     * An unknown or unspecified error happened.
     *
     * The error did not match any other error scenario Numerus is prepared to
     * handle.
     */
            NUMERUS_ERROR_GENERIC,

    /**
     * The roman numeral contains mispositioned characters.
     *
     * This is the most generic error for a syntax failure of the roman
     * numeral. It usually means that a repeatable roman character is repeated
     * non-consecutively (like `MCIM`) or that a non-repeatable roman
     * character has been repeated (like `DD`).
     */
            NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE,

    /**
     * The value to be converted to basic roman numeral is out of range.
     *
     * The value should be within
     * [#NUMERUS_MIN_BASIC_VALUE, #NUMERUS_MAX_BASIC_VALUE].
     */
            NUMERUS_ERROR_BASIC_VALUE_OUT_OF_RANGE,

    /**
     * The value to be converted to extended roman numeral is out of range.
     *
     * The value should be within
     * [#NUMERUS_MIN_EXTENDED_VALUE, #NUMERUS_MAX_EXTENDED_VALUE].
     */
            NUMERUS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE,

    /**
     * The value to be converted to extended roman numeral must not be
     * +/- Infinity or NaN.
     *
     * At the same time, the value should be within
     * [#NUMERUS_MIN_EXTENDED_VALUE, #NUMERUS_MAX_EXTENDED_VALUE].
     */
            NUMERUS_ERROR_DOUBLE_VALUE_IS_NOT_FINITE,

    /**
     * The basic roman numeral contains a character that is not accepted.
     *
     * The only allowed characters are: `MDCLXVI-` both lowercase and
     * uppercase. The only exception is the #NUMERUS_ZERO_STRING string.
     */
            NUMERUS_ERROR_ILLEGAL_BASIC_CHARACTER,

    /**
     * The extended roman numeral contains a character that is not accepted.
     *
     * The only allowed characters are: `MDCLXVIS_.-` both lowercase and
     * uppercase. The only exception is the #NUMERUS_ZERO_STRING string.
     */
            NUMERUS_ERROR_ILLEGAL_EXTENDED_CHARACTER,

    /**
     * The basic roman numeral is too long to be syntactically correct.
     *
     * The max allowed length of a basic roman numeral is
     * #NUMERUS_MAX_BASIC_LENGTH.
     */
            NUMERUS_ERROR_TOO_LONG_BASIC_NUMERAL,

    /**
     * The extended roman numeral is too long to be syntactically correct.
     *
     * The max allowed length of an extended roman numeral is
     * #NUMERUS_MAX_BASIC_LENGTH.
     */
            NUMERUS_ERROR_TOO_LONG_EXTENDED_NUMERAL,

    /**
     * The roman numeral contains too many consecutive repetitions of a
     * repeatable character.
     *
     * Characters that are allowed to be repeated multiple times are `MCXI.`.
     * They have 3 maximum consecutive repetitions each, the dot `.` has 5.
     */
            NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS,

    /**
     * The extended roman numeral contains only one underscore.
     *
     * A roman numeral may contain exactly 0 or 2 underscores, the
     * latter case being an extended numeral.
     */
            NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE,

    /**
     * The extended roman numeral contains more than two underscores.
     *
     * A roman numeral may contain exactly 0 or 2 underscores, the
     * latter case being an extended numeral.
     */
            NUMERUS_ERROR_TOO_MANY_UNDERSCORES,

    /**
     * The extended roman numeral contains the first underscore not at the
     * numeral start.
     *
     * The first underscore has to be placed at the first position of the
     * extended numeral, after the minus in case there is one.
     */
            NUMERUS_ERROR_ILLEGAL_FIRST_UNDERSCORE_POSITION,

    /**
     * The extended roman numeral contains decimal characters not
     * at the numeral end.
     *
     * Decimal characters `S.` are allowed only at the end of the numeral.
     */
            NUMERUS_ERROR_FRACTIONAL_CHARS_NOT_AT_END,
            NUMERUS_ERROR_FRACTIONAL_CHARS_BETWEEN_UNDERSCORES,


    /**
     * The roman numeral contains a misplaced minus `-` or more than one.
     *
     * A roman numeral must contain exactly 0 or 1 minus symbol. If existing,
     * it must be at the very first position of the numeral.
     */
            NUMERUS_ERROR_ILLEGAL_MINUS_POSITION,

    /**
     * The extended roman numeral contains an `M` not between underscores.
     *
     * An extended roman numeral may contain an `M` only between the
     * underscores, if they are used. Values bigger than 1000 (`M`) are
     * represented between underscores, making an `M` character after the
     * underscores illogical.
     */
            NUMERUS_ERROR_M_AFTER_UNDERSCORES,

    /**
     * Heap memory allocation failure.
     *
     * The #numerus_malloc function failed to allocate memory.
     * The operation could not be completed.
     */
            NUMERUS_ERROR_ALLOC_FAIL,

    /**
     * The pointer to the roman numeral string is NULL.
     */
            NUMERUS_ERROR_NULL_NUMERAL,

    /**
     * The pointer to a number is NULL.
     */
            NUMERUS_ERROR_NULL_NUMBER,

    /**
     * The roman numeral string is empty or filled with only whitespace.
     *
     * Insert some non-whitespace characters in the string.
     */
            NUMERUS_ERROR_EMPTY_NUMERAL,
} numerus_status_t;


/**
 * Converts an integer to a basic roman numeral.
 *
 * Accepts any integer within
 * [#NUMERUS_MIN_BASIC_VALUE, #NUMERUS_MAX_BASIC_VALUE] and generates
 * a basic numeral without any Numerus extensions representing the same value.
 *
 * The function can either allocate the string dynamically or fill a provided
 * buffer, based on the \p numeral parameter.
 *
 * @anchor numeral_allocation
 * @remark **Important!**
 *         The \p numeral parameter will contain the generated numeral
 *         at execution and, but also allows the user to decide who allocates
 *         the buffer for the numeral to be written in.
 *
 * - if \p numeral is a NULL pointer, the function returns an error;
 *
 *   \code{C}
 *   numerus_status_t status;
 *
 *   status = numerus_int_to_basic_roman(42, NULL);
 *   // status == NUMERUS_ERROR_NULL_NUMERAL
 *   \endcode
 *
 *
 * - if \p numeral points to a NULL pointer, a string will be allocated on the
 *   heap memory using #numerus_malloc and returned through the same parameter.
 *   It's **up to the user to free it!** It stays unchanged in case of errors.
 *   The memory allocation procedure will allocate exactly the space required
 *   to fit the numeral.
 *
 *   \code{C}
 *   char* numeral = NULL;
 *   numerus_status_t status;
 *
 *   status = numerus_int_to_basic_roman(42, &numeral);
 *   // status == NUMERUS_OK
 *   // numeral points to "XLII\0", which was allocated in the function.
 *
 *   status = numerus_int_to_basic_roman(983492875, &numeral);
 *   // status == NUMERUS_ERROR_BASIC_VALUE_OUT_OF_RANGE
 *   // numeral still points to "XLII\0".
 *
 *   numerus_free(numeral);
 *   // The "XLII\0" string is freed.
 *   \endcode
 *
 *
 * - if \p numeral points to a non-NULL location, that will be considered as
 *   the buffer to fill with the numeral. The buffer will be **ASSUMED** to
 *   have #NUMERUS_MAX_BASIC_LENGTH size for basic numerals an
 *   #NUMERAL_MAX_EXTENDED_LENGTH for extended numerals. The buffer will be
 *   set to an empty string in case of error. In both cases (error or not)
 *   the bytes after the null terminator `\0` are unchanged.
 *
 *   \code{C}
 *   char numeral_buffer[NUMERUS_MAX_BASIC_LENGTH];
 *   numerus_status_t status;
 *
 *   status = numerus_int_to_basic_roman(42, &numeral_buffer);
 *   // status == NUMERUS_OK
 *   // numeral_buffer contains "XLII\0".
 *
 *   status = numerus_int_to_basic_roman(983492875, &numeral_buffer);
 *   // status == NUMERUS_ERROR_BASIC_VALUE_OUT_OF_RANGE
 *   // numeral_buffer contains "\0".
 *   \endcode
 *
 * @param[in] value the integer value of the numeral. Can be negative.
 * @param[in, out] p_numeral pointer to the generated numeral string. Check its
 *        **remarks**, as it can be either allocated by the user or by
 *        this function. It's up to the user to free this string.
 * @return status code, indicating operation success or failure.
 * @see numerus_int_to_extended_roman
 * @see numerus_double_to_extended_roman
 */
numerus_status_t numerus_int_to_basic_numeral(
        int16_t value, char** p_numeral);

/**
 * Converts an integer and a fractional part (in twelfths) to an extended
 * roman numeral.
 *
 * Accepts any integer part within
 * [#NUMERUS_MIN_EXTENDED_VALUE_INT_PART, #NUMERUS_MAX_EXTENDED_VALUE_INT_PART]
 * and any fractional part as long as the combined value is within
 * [#NUMERUS_MIN_EXTENDED_VALUE, #NUMERUS_MAX_EXTENDED_VALUE]. Generates
 * an extended numeral with the Numerus extensions representing the same
 * combined value.
 *
 * @remark Same **important** remarks apply as for the
 * numerus_int_to_basic_roman() function regarding the usage of the \p numeral
 * parameter.
 *
 * @param[in] integer_part the integer part of the value of the
 *        extended roman numeral. Can be negative.
 * @param[in] twelfths the fractional part to sum to the \p integer_part.
 *        Can be negative.
 * @param[in, out] p_numeral pointer to the generated numeral string. Check its
 *        **remarks**, as it can be either allocated by the user or by
 *        this function. It's up to the user to free this string.
 * @return status code, indicating operation success or failure.
 * @see numerus_int_to_basic_roman
 * @see numerus_double_to_extended_roman
 */
numerus_status_t numerus_int_to_extended_numeral(
        int32_t integer_part, int8_t twelfths, char** p_numeral);

/**
 * Converts a double-precision floating point value to an extended
 * roman numeral.
 *
 * Accepts any double within
 * [#NUMERUS_MIN_EXTENDED_VALUE, #NUMERUS_MAX_EXTENDED_VALUE] and generates
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
numerus_status_t numerus_double_to_extended_numeral(
        double double_value, char** p_numeral);


/* Conversion function from roman numeral to value */
numerus_status_t numerus_basic_numeral_to_int(
        const char* numeral, int16_t* value);
numerus_status_t numerus_extended_numeral_to_int(
        const char* numeral, int32_t* integer_part, int8_t* twelfths);
numerus_status_t numerus_extended_numeral_to_double(
        const char* numeral, double* value);


/* Functions to manage twelfths */
numerus_status_t numerus_int_parts_to_double(
        int32_t int_part, uint8_t twelfths, double* result);
numerus_status_t numerus_double_to_int_parts(
        double value, int32_t* int_part, int8_t* twelfths);
numerus_status_t numerus_simplify_twelfths(
        int32_t* int_part, int8_t* twelfths);


/* Numeral analysis functions */
numerus_status_t numerus_is_zero(
        const char* numeral, bool* p_result);
numerus_status_t numerus_sign(
        const char* numeral, int8_t* p_result);
numerus_status_t numerus_is_basic_numeral(
        const char* numeral, bool* p_result);
numerus_status_t numerus_count_roman_chars(
        const char* numeral, uint8_t* p_result);


/* Output formatting functions */
// Pass the string two times to overwrite it??
// Max result size #NUMERUS_MAX_EXTENDED_OVERLINED_LENGTH
numerus_status_t numerus_overline(
        const char* numeral, char** p_formatted);


#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_NUMERUS3_H_ */
