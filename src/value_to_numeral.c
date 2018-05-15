/**
 * @file value_to_numeral.c
 * @brief Implementation of conversion functions from value
 * to roman numeral.
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


static bool handle_basic_corner_cases(
        int32_t value,
        char** p_numeral,
        numerus_status_t* p_status);
static bool handle_extended_corner_case(
        int32_t* p_integer_part, int8_t* p_twelfths, char** p_numeral,
        numerus_status_t* p_status);
static uint8_t convert_integer_part(
        int32_t integer_part, char* const numeral_in_progress);
static uint8_t value_to_numeral_section(
        int32_t value,
        char* const numeral,
        uint8_t current_dict_char_index);


numerus_status_t numerus_int_to_basic_numeral(
        int16_t value, char** p_numeral)
{
    numerus_status_t status;
    bool was_corner_case;

    was_corner_case = handle_basic_corner_cases(value, p_numeral, &status);
    if (!was_corner_case)
    {
        char numeral_in_progress[NUMERUS_MAX_BASIC_LENGTH];
        uint8_t numeral_length = 0;

        if (value < 0)
        {
            numeral_in_progress[numeral_length++] = '-';
            value = ABS(value);
        }
        numeral_length += value_to_numeral_section(
                value, &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_M);
        numeral_in_progress[numeral_length++] = '\0';
        status = obtain_numeral_buffer(p_numeral, numeral_length);
        if (status == NUMERUS_OK)
        {
            strncpy(*p_numeral, numeral_in_progress, numeral_length);
            status = NUMERUS_OK;
        }
    }
    return status;
}

// Returns true if it was a corner case

/**
 * @internal
 * Handles the corner cases for integer to basic numeral conversion.
 *
 * This includes handling of the cases:
 * - NULL pointers
 * - value out of range
 * - value==0, which sets the status to NUMERUS_OK and generates the numeral
 *
 * @param value to be converted
 * @param p_numeral pointer to pointer to the numeral-to-be memory location
 * @param p_status pointer to the numerus_status_t to update
 * @return true if it was actually a corner case and thus was handled.
 *         The status code will contain what happened. False if it was a
 *         normal case.
 */
static bool handle_basic_corner_cases(
        const int32_t value,
        char** const p_numeral,
        numerus_status_t* const p_status)
{
    bool is_corner_case = true;

    if (p_numeral == NULL)
    {
        *p_status = NUMERUS_ERROR_NULL_NUMERAL;
    }
    else if (value < NUMERUS_MIN_BASIC_VALUE
             || value > NUMERUS_MAX_BASIC_VALUE)
    {
        *p_status = NUMERUS_ERROR_BASIC_VALUE_OUT_OF_RANGE;
    }
    else if (value == 0)
    {
        *p_status = obtain_numeral_buffer(p_numeral, ZERO_NUMERAL_SIZE);
        if (*p_status == NUMERUS_OK)
        {
            strncpy(*p_numeral, NUMERUS_ZERO_NUMERAL, ZERO_NUMERAL_SIZE);
        }
    }
    else
    {
        *p_status = NUMERUS_OK;
        is_corner_case = false;
    }
    return is_corner_case;
}


numerus_status_t numerus_double_to_extended_numeral(
        const double double_value, char** p_numeral)
{
    int32_t int_part;
    int8_t twelfths;
    numerus_status_t status;

    status = numerus_double_to_int_parts(double_value, &int_part, &twelfths);
    if (status == NUMERUS_OK)
    {
        status = numerus_int_to_extended_numeral(
                int_part, twelfths, p_numeral);
    }
    return status;
}


numerus_status_t numerus_int_to_extended_numeral(
        int32_t integer_part, int8_t twelfths, char** p_numeral)
{
    numerus_status_t status;
    bool was_corner_case;

    was_corner_case = handle_extended_corner_case(
            &integer_part, &twelfths, p_numeral, &status);
    if (!was_corner_case)
    {
        char numeral_in_progress[NUMERUS_MAX_EXTENDED_LENGTH];
        uint8_t numeral_length = 0;

        if (integer_part < 0 || (integer_part == 0 && twelfths < 0))
        {
            numeral_in_progress[numeral_length++] = '-';
            integer_part = ABS(integer_part);
            twelfths = ABS(twelfths);
        }
        numeral_length += convert_integer_part(
                &numeral_in_progress[numeral_length], integer_part);
        /* Decimal part, starting with "S" char */
        numeral_length += value_to_numeral_section(
                twelfths,
                &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_S);
        numeral_in_progress[numeral_length++] = '\0';
        status = obtain_numeral_buffer(p_numeral, numeral_length);
        if (status == NUMERUS_OK)
        {
            strncpy(*p_numeral, numeral_in_progress, numeral_length);
        }
    }
    return status;
}

/**
 * @internal
 * Handles the corner cases for integer to extended numeral conversion.
 *
 * This includes handling of the cases:
 * - simplification of integer part and twelfths
 * - NULL pointers
 * - value out of range
 * - value==0, which sets the status to NUMERUS_OK and generates the numeral
 *
 * @param p_integer_part value to be converted
 * @param p_twelfths to be added to the \p integer_part
 * @param p_numeral pointer to pointer to the numeral-to-be memory location
 * @param p_status pointer to the numerus_status_t to update
 * @return true if it was actually a corner case and thus was handled.
 *         The status code will contain what happened. False if it was a
 *         normal case.
 */
static bool handle_extended_corner_case(
        int32_t* const p_integer_part, int8_t* const p_twelfths,
        char** p_numeral, numerus_status_t* p_status)
{
    bool is_corner_case = true;

    *p_status = numerus_simplify_twelfths(p_integer_part, p_twelfths);
    if (*p_status == NUMERUS_OK)
    {
        if (*p_integer_part < NUMERUS_MIN_EXTENDED_VALUE_INT_PART
            || *p_integer_part > NUMERUS_MAX_EXTENDED_VALUE_INT_PART)
        {
            *p_status = NUMERUS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
        }
        else if (*p_integer_part == 0 && *p_twelfths == 0)
        {
            *p_status = obtain_numeral_buffer(p_numeral, ZERO_NUMERAL_SIZE);
            if (*p_status == NUMERUS_OK)
            {
                strncpy(*p_numeral, NUMERUS_ZERO_NUMERAL, ZERO_NUMERAL_SIZE);
            }
        }
        else
        {
            is_corner_case = false;
        }
    }
    return is_corner_case;
}


/**
 * @internal
 * Converts the integer part of the value into the appropriate part of the
 * extended numeral.
 *
 * Afterwards only the fractional part in twelfths needs to be processed.
 *
 * @param integer_part the value to convert
 * @param numeral_in_progress the location where to write the numeral
 * @return amount of characters written
 */
static uint8_t convert_integer_part(
        int32_t integer_part, char* const numeral_in_progress)
{
    uint8_t numeral_length = 0;

    if (integer_part > NUMERUS_MAX_BASIC_VALUE)
    {
        /* Part between underscores */
        numeral_in_progress[numeral_length++] = '_';
        numeral_length += value_to_numeral_section(
                integer_part / VINCULUMN_VALUE_MULTIPLER,
                &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_M);
        integer_part -= (integer_part / VINCULUMN_VALUE_MULTIPLER)
                        * VINCULUMN_VALUE_MULTIPLER; /* Remove 3 left-most digits */
        numeral_in_progress[numeral_length++] = '_';
        /* Part after underscores without "M" char, start with "CM" */
        numeral_length += value_to_numeral_section(
                integer_part,
                &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_CM);
    }
    else
    {
        /* No underscores needed, so starting with "M" char */
        numeral_length += value_to_numeral_section(
                integer_part,
                &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_M);
    }
    return numeral_length;
}


/**
 * Converts a value to a small part of a roman numeral
 *
 * Appends the generated numeral to the passed position in the roman string,
 * already allocated. Starts comparing the values with the specified dictionary
 * char index. The latter is used to skip some forbidden characters.
 *
 * @param value the value to convert
 * @param numeral the location where to write the numeral
 * @param current_dict_char_index index of the dictionary to specify the
 * starting dictionary entry to compare the characters with
 * @returns amount of characters written
 */
static uint8_t value_to_numeral_section(
        int32_t value,
        char* const numeral,
        uint8_t current_dict_char_index)
{
    uint8_t written_characters = 0;

    while (value > 0)
    {
        while (value >= DICTIONARY[current_dict_char_index].value)
        {
            numeral[written_characters] =
                    DICTIONARY[current_dict_char_index].character_1;
            written_characters++;
            if (DICTIONARY[current_dict_char_index].character_2 != '\0')
            {
                numeral[written_characters] =
                        DICTIONARY[current_dict_char_index].character_2;
                written_characters++;
            }
            value -= DICTIONARY[current_dict_char_index].value;
        }
        current_dict_char_index++;
    }
    return written_characters;
}
