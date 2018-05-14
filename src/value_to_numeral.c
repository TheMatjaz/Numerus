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


void cleaned_value_to_basic_numeral(
        int32_t value, char* numeral_part,
        uint8_t dictionary_start_char);

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
        value *= -1;
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
        double double_value, char** numeral)
{
    uint8_t twelfths;
    int32_t int_part = numerus_double_to_parts(double_value, &twelfths);
    return numerus_int_with_twelfth_to_roman(int_part, twelfths, errcode);
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
        int32_t integer_part, int8_t twelfths, char** numeral)
{
    /* ALGORITHM
    sing
    + ('_' + value_to_basic(integer_part/1000) + '_') if ABS(value) > NUMERUS_MAX_BASIC_VALUE
    + value_to_basic(integer_part - integer_part/1000)
    + decimal_part(twelfths)
     */

    /* Prepare variables */
    numerus_simplify_twelfths(&int_part, &twelfths);
    double double_value = numerus_parts_to_double(int_part, twelfths);
    if (errcode == NULL)
    {
        errcode = &numerus_error_code;
    }

    /* Out of range check */
    if (double_value < NUMERUS_MIN_VALUE || double_value > NUMERUS_MAX_VALUE)
    {
        numerus_error_code = NUMERUS_ERROR_VALUE_OUT_OF_RANGE;
        *errcode = NUMERUS_ERROR_VALUE_OUT_OF_RANGE;
        return NULL;
    }

    /* Create pointer to the building buffer */
    char building_buffer[NUMERUS_MAX_LENGTH];
    char* roman_numeral = building_buffer;

    /* Save sign or return NUMERUS_ZERO for 0 */
    if (int_part == 0 && twelfths == 0)
    {
        /* Return writable copy of NUMERUS_ZERO on the heap */
        char* zero_string = malloc(strlen(NUMERUS_ZERO) + 1);
        if (zero_string == NULL)
        {
            numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
            *errcode = NUMERUS_ERROR_MALLOC_FAIL;
            return NULL;
        }
        strcpy(zero_string, NUMERUS_ZERO);
        return zero_string;
    }
    else if (int_part < 0 || (int_part == 0 && twelfths < 0))
    {
        int_part = ABS(int_part);
        twelfths = ABS(twelfths);
        double_value = ABS(double_value);
        *(roman_numeral++) = '-';
    }

    /* Create part between underscores */
    if (double_value > NUMERUS_MAX_NONLONG_FLOAT_VALUE)
    {
        /* Underscores are needed */
        *(roman_numeral++) = '_';
        roman_numeral = cleaned_value_to_basic_numeral(int_part / 1000,
                                                       roman_numeral, 0);
        int_part -= (int_part / 1000) * 1000; /* Remove 3 left-most digits */
        *(roman_numeral++) = '_';
        /* Part after underscores without "M" char, start with "CM" */
        roman_numeral =
                cleaned_value_to_basic_numeral(int_part, roman_numeral, 1);
    }
    else
    {
        /* No underscores needed, so starting with "M" char */
        roman_numeral =
                cleaned_value_to_basic_numeral(int_part, roman_numeral, 0);
    }
    /* Decimal part, starting with "S" char */
    roman_numeral =
            cleaned_value_to_basic_numeral(twelfths, roman_numeral, 13);
    *(roman_numeral++) = '\0';

    /* Copy out of the buffer and return it on the heap */
    char* returnable_roman_string =
            malloc(roman_numeral - building_buffer);
    if (returnable_roman_string == NULL)
    {
        numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
        *errcode = NUMERUS_ERROR_MALLOC_FAIL;
        return NULL;
    }
    strcpy(returnable_roman_string, building_buffer);
    numerus_error_code = NUMERUS_OK;
    *errcode = NUMERUS_OK;
    return returnable_roman_string;
}


/**
 * Copies a string of 1 or 2 characters.
 *
 * Copies a character from the source to the destination. If there is another
 * character after that in the source, copies that as well.
 *
 * @param *source the string of 1-2 characters to copy
 * @param *destination the string, already allocated, to copy the *source into
 * @returns the new position of the destination pointer after the characters
 * have been copied
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
void cleaned_value_to_basic_numeral(
        int32_t value, char* numeral_part,
        uint8_t dictionary_start_char)
{
    const dictionary_char_t* current_dictionary_char
            = &DICTIONARY[dictionary_start_char];
    while (value > 0)
    {
        while (value >= current_dictionary_char->value)
        {
            numeral_part += copy_char_from_dictionary(
                    current_dictionary_char, numeral_part);
            value -= current_dictionary_char->value;
        }
        current_dictionary_char++;
    }
}
