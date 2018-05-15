/**
 * @file formatting.c
 * @brief Implementations of numeral formatting functions.
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


static numerus_status_t expected_length_of_overlined(
        const char* numeral,
        uint8_t* numeral_length,
        uint8_t* additional_size);


/**
 * Allocates a string with a prettier representation of a long roman numeral
 * with actual overlining.
 *
 * Ignores any heading whitespace.
 *
 * In case of no overlining, it provides a copy of the original numeral,
 * with the same parameter usage as in value-to-numeral conversion functions.
 *
 * Generates a two lined string (newline character is '\n') by overlining the
 * part between underscores. The string is just copied if the roman numeral is
 * not long.
 *
 * Remember to free() the pretty-printed roman numeral when it's not useful
 * anymore and (depending on your necessity) also the roman numeral itself.
 *
 * Example:
 *
 * <pre>
 *                  ___
 * -_CXX_VIII  =>  -CXXVIII
 * VIII        =>   VIII
 * </pre>
 *
 * The prettifying process status is stored in the errcode passed as parameter,
 * which can be NULL to ignore the error, although it's not recommended. If the
 * error code is different than NUMERUS_OK, an error occurred during the
 * prettifying process and the returned string is NULL. The error code may help
 * find the specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param status int where to store the comparison status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns char* allocated string with the prettier version of the roman
 * numeral or NULL if malloc() fails.
 */

numerus_status_t numerus_overline(
        const char* numeral, char** const p_formatted)
{
    numerus_status_t status;
    uint8_t numeral_length;
    uint8_t additional_size;

    status = prepare_for_analysis(&numeral);
    if (status != NUMERUS_OK)
    {
        return status;
    }
    status = expected_length_of_overlined(
            numeral, &numeral_length, &additional_size);
    if (status != NUMERUS_OK)
    {
        return status;
    }
    if (additional_size > 0)
    {
        /* Needs to be overlined. */
        status = obtain_numeral_buffer(p_formatted,
                                       numeral_length + additional_size);
        if (status != NUMERUS_OK)
        {
            return status;
        }
        uint8_t formatted_index = 0;
        uint8_t numeral_index = 0;

        /* Skip minus sign */
        if (numeral[numeral_index] == '-')
        {
            (*p_formatted)[formatted_index++] = ' ';
            numeral_index++;
        }

        /* Write the overline */
        numeral_index++; /* Skip first underscore */
        while (numeral[numeral_index] != '_')
        {
            (*p_formatted)[formatted_index++] = '_';
            numeral_index++;
        }
        (*p_formatted)[formatted_index++] = '\n';

        /* Copy the numeral in the second line without underscores. */
        numeral_index = 0;
        while (numeral[numeral_index] != '\0')
        {
            if (numeral[numeral_index] != '_')
            {
                (*p_formatted)[formatted_index++] = numeral[numeral_index];
            }
            numeral_index++;
        }
        (*p_formatted)[formatted_index] = '\0';
    }
    else
    {
        /* Just a basic numeral, no overlining needed */
        status = obtain_numeral_buffer(p_formatted, numeral_length);
        if (status == NUMERUS_OK)
        {
            strncpy(*p_formatted, numeral, numeral_length);
        }
    }
    return status;
}


/**
 * @internal
 * Analyzes how much additional space would the numeral need after conversion
 * to overlined
 *
 * Including the newline character '\n', the null terminator '\0' and
 * an initial space for an optional minus '-'.
 *
 * If the numeral does not have exactly two underscores (includes the case
 * if it's just a basic numeral), it returns the same size of the passed
 * numeral.
 *
 * @param[in] numeral the numeral to analyze
 * @param[out] numeral_length size of the unchanged \p numeral, including
 *             its null terminator '\0'.
 * @param[out] additional_size additional size that the overlined form would
 *             need. Sum it up with \p numeral_length to obtain the total
 *             allocation size. It's 0 in case no overlining is required
 *             or possible.
 * @returns status code, indicating operation success or any numeral format
 *          errors that prevent the overlining.
 */
static numerus_status_t expected_length_of_overlined(
        const char* const numeral,
        uint8_t* const numeral_length,
        uint8_t* const additional_size)
{
    uint8_t overlined_length = 0;
    uint8_t first_underscore_index = NUMERUS_MAX_EXTENDED_LENGTH;
    uint8_t index = 0;
    numerus_status_t status = NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE;

    while (numeral[index] != '\0' && index < NUMERUS_MAX_EXTENDED_LENGTH)
    {
        if (numeral[index] == '_')
        {
            if (first_underscore_index == NUMERUS_MAX_EXTENDED_LENGTH)
            {
                first_underscore_index = index;
            }
            else
            {
                /* The following line also includes a byte for the line
                 * feed '\n'. */
                overlined_length += index - first_underscore_index;
                status = NUMERUS_OK;
                break;
            }
        }
        else
        {
            overlined_length++;
        }
        index++;
    };
    while (numeral[index] != '\0' && index < NUMERUS_MAX_EXTENDED_LENGTH)
    {
        if (numeral[index] == '_')
        {
            status = NUMERUS_ERROR_TOO_MANY_UNDERSCORES;
            break;
        }
        index++;
    };
    if (index >= NUMERUS_MAX_EXTENDED_LENGTH)
    {
        status = NUMERUS_ERROR_TOO_LONG_EXTENDED_NUMERAL;
    }
    *numeral_length = ++index;
    if (status == NUMERUS_OK)
    {
        *additional_size = overlined_length;
    }
    else
    {
        *additional_size = 0;
    }
    return status;
}


char* numerus_create_pretty_value_as_double(double double_value)
{
    uint8_t twelfths;
    int32_t int_part = numerus_double_to_parts(double_value, &twelfths);
    return numerus_create_pretty_value_as_parts(int_part, twelfths);
}


/**
 * Allocates a string with a prettier representation of a value as an integer
 * and a number of twelfths, with the twelfths shortened.
 *
 * Remember to free() the pretty-printed value when it's not useful anymore.
 * If a malloc() error occurs during the operation, the returned value is NULL.
 *
 * Example: `-3, 2` (= -3 + 2/12) becomes "-2, -5/6" (= -2 -10/12).
 *
 * @param int_part long integer part of a value to be added to the twelfths
 * and converted to a pretty string.
 * @param twelfths short integer as number of twelfths (1/12) to be added to the
 * integer part and converted to a pretty string.
 * @returns char* allocated string with the prettier version of the value or
 * NULL if malloc() fails.
 */
char* numerus_create_pretty_value_as_parts(int32_t int_part, uint8_t twelfths)
{
    char* pretty_value;
    if (twelfths == 0)
    {
        size_t needed_space = snprintf(NULL, 0, "%d", int_part);
        pretty_value = malloc(needed_space + 1); /* +1 for '\0' */
        if (pretty_value == NULL)
        {
            numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
            return NULL;
        }
        sprintf(pretty_value, "%d", int_part);
    }
    else
    {
        numerus_shorten_and_same_sign_to_parts(&int_part, &twelfths);
        /* Shorten twelfth fraction */
        int8_t gcd = _num_greatest_common_divisor(twelfths, 12);
        size_t
                needed_space =
                snprintf(NULL, 0, "%d, %d/%d", int_part, twelfths / gcd,
                         12 / gcd);
        pretty_value = malloc(needed_space + 1); /* +1 for '\0' */
        if (pretty_value == NULL)
        {
            numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
            return NULL;
        }
        sprintf(pretty_value, "%d, %d/%d", int_part, twelfths / gcd, 12 / gcd);
    }
    return pretty_value;
}
