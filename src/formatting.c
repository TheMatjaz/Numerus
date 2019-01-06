/**
 * @file
 * @brief Implementations of numeral formatting functions.
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include <stdio.h>
#include "internal.h"


static numerus_status_t expected_length_of_overlined(
        const char* numeral,
        uint8_t* numeral_length,
        uint8_t* additional_size);
static uint8_t digits_of_integer_part(int32_t value);
static uint8_t digits_of_twelfths(uint8_t value);
static void shorten_fraction(int8_t* numerator, int8_t* denominator);
static int8_t greatest_common_divisor(int8_t a, int8_t b);


numerus_status_t numerus_overline(
        const char* numeral, char** const p_formatted,
        const bool windows_end_of_line)
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
        if (windows_end_of_line)
        {
            (*p_formatted)[formatted_index++] = '\r';
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


numerus_status_t numerus_double_as_int_parts_string(
        const double value, char** const p_formatted)
{
    int32_t int_part;
    int8_t twelfths;
    numerus_status_t status;

    status = numerus_double_to_int_parts(value, &int_part, &twelfths);
    if (status == NUMERUS_OK)
    {
        status = numerus_int_parts_to_string(int_part, twelfths, p_formatted);
    }
    return status;
}

numerus_status_t numerus_int_parts_to_string(
        int32_t int_part, int8_t twelfths, char** const p_formatted)
{
    numerus_status_t status;

    status = numerus_simplify_twelfths(&int_part, &twelfths);
    if (status != NUMERUS_OK)
    {
        return status;
    }
    char build_buffer[NUMERUS_MAX_FORMATTED_PARTS_LENGTH];
    uint8_t written_in_buffer;
    if (twelfths == 0)
    {
        written_in_buffer = sprintf(build_buffer, "%d", int_part);
    }
    else
    {
        int8_t denominator = 12;
        shorten_fraction(&twelfths, &denominator);
        written_in_buffer = sprintf(
                build_buffer, "%d, %d/%d", int_part, twelfths, denominator);
    }
    written_in_buffer++;  /* sprintf return value does not count '\0'. */
    status = obtain_numeral_buffer(p_formatted, written_in_buffer);
    if (status == NUMERUS_OK)
    {
        strncpy(*p_formatted, build_buffer, written_in_buffer);
    }
    return status;
}


static uint8_t digits_of_integer_part(int32_t value)
{
    // Inspired by https://stackoverflow.com/a/6655759
    uint8_t digits = 1;

    if (value < 0)
    {
        digits++; /* Minus sign. */
        value = -value;
    }
    if (value >= 10000)
    {
        digits += 4;
        value /= 10000;
    }
    if (value >= 100)
    {
        digits += 2;
        value /= 100;
    }
    if (value >= 10)
    {
        digits += 1;
    }
    return digits;
}

static uint8_t digits_of_twelfths(uint8_t value)
{
    // Inspired by https://stackoverflow.com/a/6655759
    uint8_t digits = 1;

    if (value >= 10)
    {
        digits += 1;
    }
    return digits;
}

static void shorten_fraction(int8_t* numerator, int8_t* denominator)
{
    int8_t gcd = greatest_common_divisor(*numerator, *denominator);

    *numerator /= gcd;
    *denominator /= gcd;
}

static int8_t greatest_common_divisor(int8_t a, int8_t b)
{
    int8_t temp;

    while (b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
