/**
 * @file value_to_numeral.c
 * @brief
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 *
 * This file contains implementations of the conversion functions from value
 * to roman numeral only..
 */

#include "internal.h"
#include "dictionary.h"


static bool handle_basic_corner_cases(
        int32_t value,
        char** p_numeral,
        numerus_status_t* p_status);
static bool handle_extended_corner_case(
        int32_t integer_part, int8_t twelfths, char** p_numeral,
        numerus_status_t* p_status);
static uint8_t convert_integer_part(
        char* numeral_in_progress, int32_t integer_part);
static uint8_t cleaned_value_to_basic_numeral(
        int32_t value,
        char* numeral,
        uint8_t current_dict_char_index);

/**
 * Converts an int32_t integer value to a roman numeral with its value.
 *
 * Accepts any long within
 * [NUMERUS_MAX_LONG_NONFLOAT_VALUE, NUMERUS_MIN_LONG_NONFLOAT_VALUE].
 *
 * Remember to free() the roman numeral when it's not useful anymore.
 *
 * The conversion status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occurred during the
 * conversion and the returned string is NULL. The error code may help find the
 * specific error.
 *
 * @param int_value int32_t to be converted to roman numeral.
 * @param *errcode int8_t where to store the conversion status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns char* a string containing the roman numeral or NULL when an error
 * occurs.
 */
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
        numeral_length += cleaned_value_to_basic_numeral(
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

/**
 * Converts a double value to a roman numeral with its value.
 *
 * Accepts any long within [NUMERUS_MAX_VALUE, NUMERUS_MIN_VALUE]. The decimal
 * part of the value is also converted.
 *
 * Remember to free() the roman numeral when it's not useful anymore.
 *
 * The conversion status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occurred during the
 * conversion and the returned string is NULL. The error code may help find the
 * specific error.
 *
 * @param double_value double precision floating point value to be converted to
 * roman numeral.
 * @param *errcode int where to store the conversion status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns char* a string containing the roman numeral or NULL when an error
 * occurs.
 */
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


/**
 * Converts an integer value and a number of twelfths to a roman numeral with
 * their sum as value.
 *
 * Accepts any pair of integer value and twelfths so that their sum is within
 * [NUMERUS_MAX_VALUE, NUMERUS_MIN_VALUE].
 *
 * Remember to free() the roman numeral when it's not useful anymore.
 *
 * The conversion status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occurred during the
 * conversion and the returned string is NULL. The error code may help find the
 * specific error.
 *
 * @param int_part int32_t part of a value to be added to the twelfths
 * and converted to roman numeral.
 * @param twelfths uint8_t as number of twelfths (1/12) to be added to the
 * integer part and converted to roman numeral.
 * @param *errcode int8_t where to store the conversion status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns char* a string containing the roman numeral or NULL when an error
 * occurs.
 */
numerus_status_t numerus_int_to_extended_numeral(
        int32_t integer_part, int8_t twelfths, char** p_numeral)
{
    numerus_status_t status;
    bool was_corner_case;

    was_corner_case = handle_extended_corner_case(
            integer_part, twelfths, p_numeral, &status);
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
        numeral_length += cleaned_value_to_basic_numeral(
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

static bool handle_extended_corner_case(
        int32_t integer_part, int8_t twelfths, char** p_numeral,
        numerus_status_t* p_status)
{
    bool is_corner_case = true;

    *p_status = numerus_simplify_twelfths(&integer_part, &twelfths);
    if (*p_status == NUMERUS_OK)
    {
        if (integer_part < NUMERUS_MIN_EXTENDED_VALUE_INT_PART
            || integer_part > NUMERUS_MAX_EXTENDED_VALUE_INT_PART)
        {
            *p_status = NUMERUS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
        }
        else if (integer_part == 0 && twelfths == 0)
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


static uint8_t convert_integer_part(
        char* numeral_in_progress, int32_t integer_part)
{
    uint8_t numeral_length = 0;

    if (integer_part > NUMERUS_MAX_BASIC_VALUE)
    {
        /* Part between underscores */
        numeral_in_progress[numeral_length++] = '_';
        numeral_length += cleaned_value_to_basic_numeral(
                integer_part / VINCULUMN_VALUE_MULTIPLER,
                &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_M);
        integer_part -= (integer_part / VINCULUMN_VALUE_MULTIPLER)
                        * VINCULUMN_VALUE_MULTIPLER; /* Remove 3 left-most digits */
        numeral_in_progress[numeral_length++] = '_';
        /* Part after underscores without "M" char, start with "CM" */
        numeral_length += cleaned_value_to_basic_numeral(
                integer_part,
                &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_CM);
    }
    else
    {
        /* No underscores needed, so starting with "M" char */
        numeral_length += cleaned_value_to_basic_numeral(
                integer_part,
                &numeral_in_progress[numeral_length],
                DICTIONARY_INDEX_FOR_M);
    }
    return numeral_length;
}


/**
 * Converts a value to a part of a roman numeral for the part between
 * underscores or the part after them.
 *
 * Appends the generated numeral to the passed position in the roman string,
 * already allocated. Starts comparing the values with the specified dictionary
 * char.
 *
 * @param value int32_t to be converted to a roman numeral
 * @param *roman string where to add the generated roman numeral
 * @param dictionary_start_char uint8_t index of the dictionary to specify the
 * starting dictionary entry to compare the characters with
 * @returns position after the inserted string
 */
static uint8_t cleaned_value_to_basic_numeral(
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
