/**
 * @file formatting.c
 * @brief
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


/**
 * Analyzes how many chars are necessary to allocate the first line of the
 * pretty-printing version of a long roman numeral without security checks.
 *
 * It has to be used after the roman numeral has been already checked with
 * numerus_count_roman_chars() to prevent any errors.
 */
static size_t expected_length_of_overlined(const char* numeral)
{
    uint8_t index;
    int8_t first_underscore_index = -1;
    int8_t second_underscore_index = -1;
    size_t overlined_length = 0;

    for (index = 0; index < NUMERUS_MAX_EXTENDED_LENGTH; index++)
    {
        if (numeral[index] == '\0')
        {
            break;
        }
        if (numeral[index] == '_')
        {
            if (first_underscore_index == -1)
            {
                first_underscore_index = index;
            }
            else
            {
                // TODO Does not check if the second underscore actually exists!
                second_underscore_index = index;
            }
        }
        else
        {
            overlined_length++;
        }
    }
    /* The following line also includes a space for the line feed '\n'. */
    overlined_length += second_underscore_index - first_underscore_index;
    return overlined_length;
}


/**
 * Allocates a string with a prettier representation of a long roman numeral
 * with actual overlining.
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

    status = prepare_for_analysis(&numeral);
    if (status != NUMERUS_OK)
    {
        return status;
    }
    size_t
            numeral_length =
            strnlen(numeral, NUMERUS_MAX_EXTENDED_LENGTH + 1) + 1;
    if (numeral_length > NUMERUS_MAX_EXTENDED_LENGTH)
    {
        status = NUMERUS_ERROR_TOO_LONG_EXTENDED_NUMERAL;
        return status;
    }
    if (contains_extended_characters(numeral))
    { // TODO should check that there are actually two underscores?
        status = obtain_numeral_buffer(p_formatted,
                // TODO check this cast
                                       (uint8_t) expected_length_of_overlined(
                                               numeral));
        char* unformatted_start = numeral;
        char* formatted = *p_formatted;;

        /* Skip minus sign */
        if (*numeral == '-')
        {
            *(formatted++) = ' ';
            numeral++;
        }

        /* Write the overline */
        numeral++; /* Skip first underscore */
        while (*numeral != '_')
        {
            *(formatted++) = '_';
            numeral++;
        }
        *(formatted++) = '\n';

        /* Copy the numeral in the second line without underscores. */
        numeral = unformatted_start;
        while (*numeral != '\0')
        {
            if (*numeral == '_')
            {
                numeral++;
            }
            else
            {
                *(formatted++) = *numeral;
                numeral++;
            }
        }
        *formatted = '\0';
        return status;
    }
    else
    {
        /* Just a basic numeral. */
        status = obtain_numeral_buffer(p_formatted, NUMERUS_MAX_BASIC_LENGTH);
        if (status == NUMERUS_OK)
        {
            strncpy(*p_formatted, numeral, NUMERUS_MAX_BASIC_LENGTH);
        }
        return status;
    }
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
