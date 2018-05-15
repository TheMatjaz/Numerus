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


static uint8_t cleaned_value_to_basic_numeral(
        int32_t value, char* numeral_part, uint8_t dictionary_start_char);
static uint8_t copy_char_from_dictionary(
        const dictionary_char_t* source, char* destination);

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

    status = obtain_numeral_buffer(p_numeral, NUMERUS_MAX_BASIC_LENGTH);
    if (status != NUMERUS_OK)
    {
        return status;
    }
    if (value < NUMERUS_MIN_BASIC_VALUE || value > NUMERUS_MAX_BASIC_VALUE)
    {
        return NUMERUS_ERROR_BASIC_VALUE_OUT_OF_RANGE;
    }

    if (value == 0)
    {
        /* Return writable copy of NUMERUS_ZERO on the heap */
        strncpy(*p_numeral, NUMERUS_ZERO_NUMERAL, ZERO_NUMERAL_SIZE);
        return NUMERUS_OK;
    }
    else if (value < 0)
    {
        **p_numeral = '-';
        (*p_numeral)++;
        value = ABS(value);
    }
    cleaned_value_to_basic_numeral(value, *p_numeral, DICTIONARY_INDEX_FOR_M);
    (*p_numeral)++;
    **p_numeral = '\0';
    return NUMERUS_OK;
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
    /* ALGORITHM
    sing
    + ('_' + value_to_basic(integer_part/1000) + '_') if ABS(value) > NUMERUS_MAX_BASIC_VALUE
    + value_to_basic(integer_part - integer_part/1000)
    + decimal_part(twelfths)
     */
    numerus_status_t status;

    status = obtain_numeral_buffer(p_numeral, NUMERUS_MAX_EXTENDED_LENGTH);
    if (status != NUMERUS_OK)
    {
        return status;
    }
    status = numerus_simplify_twelfths(&integer_part, &twelfths);
    if (status != NUMERUS_OK)
    {
        return status;
    }
    if (integer_part < NUMERUS_MIN_EXTENDED_VALUE_INT_PART
        || integer_part > NUMERUS_MAX_EXTENDED_VALUE_INT_PART)
    {
        return NUMERUS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
    }

    if (integer_part == 0 && twelfths == 0)
    {
        /* Return writable copy of NUMERUS_ZERO on the heap */
        strncpy(*p_numeral, NUMERUS_ZERO_NUMERAL, ZERO_NUMERAL_SIZE);
        return NUMERUS_OK;
    }
    else if (integer_part < 0 || (integer_part == 0 && twelfths < 0))
    {
        **p_numeral = '-';
        (*p_numeral)++;
        integer_part = ABS(integer_part);
        twelfths = ABS(twelfths);
    }

    /* Create part between underscores */
    if (integer_part > NUMERUS_MAX_BASIC_VALUE)
    {
        /* Underscores are needed */
        **p_numeral = '_';
        (*p_numeral)++;
        cleaned_value_to_basic_numeral(integer_part / 1000, *p_numeral, DICTIONARY_INDEX_FOR_M);
        integer_part -= (integer_part / 1000) * 1000; /* Remove 3 left-most digits */
        **p_numeral = '_';
        (*p_numeral)++;
        /* Part after underscores without "M" char, start with "CM" */
        cleaned_value_to_basic_numeral(integer_part, *p_numeral, DICTIONARY_INDEX_FOR_CM);
    }
    else
    {
        /* No underscores needed, so starting with "M" char */
        cleaned_value_to_basic_numeral(integer_part, *p_numeral, DICTIONARY_INDEX_FOR_M);
    }
    /* Decimal part, starting with "S" char */
    cleaned_value_to_basic_numeral(twelfths, *p_numeral, DICTIONARY_INDEX_FOR_S);
    **p_numeral = '\0';
    (*p_numeral)++;
    return NUMERUS_OK;
}


/**
 * Copies a string of 1 or 2 characters.
 *
 * Copies a character from the source to the destination. If there is another
 * character after that in the source, copies that as well.
 *
 * @param *source the string of 1-2 characters to copy
 * @param *destination the string, already allocated, to copy the *source into
 * @returns amount of characters that were copied
 */
static uint8_t copy_char_from_dictionary(
        const dictionary_char_t* source,
        char* destination)
{
    *destination = source->character_1;
    if (source->character_2 != '\0')
    {
        destination++;
        *destination = source->character_2;
        return 2;
    }
    else
    {
        return 1;
    }
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
        int32_t value, char* const numeral_part,
        uint8_t dictionary_start_char)
{
    const dictionary_char_t* current_dictionary_char
            = &DICTIONARY[dictionary_start_char];
    uint8_t written_characters;

    written_characters = 0;
    while (value > 0)
    {
        while (value >= current_dictionary_char->value)
        {
            numeral_part[written_characters] =
                    current_dictionary_char->character_1;
            written_characters++;
            if (current_dictionary_char->character_2 != '\0')
            {
                numeral_part[written_characters] =
                        current_dictionary_char->character_2;
                written_characters++;
            }
            value -= current_dictionary_char->value;
        }
        current_dictionary_char++;
    }
    return written_characters;
}
