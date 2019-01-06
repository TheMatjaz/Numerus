/**
 * @file
 * @brief Implementation of conversion functions from roman numeral
 * to value.
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


numerus_status_t numerus_basic_numeral_to_int(
        const char* numeral, int16_t* value);
numerus_status_t numerus_extended_numeral_to_int(
        const char* numeral, int32_t* integer_part, int8_t* twelfths);
numerus_status_t numerus_extended_numeral_to_double(
        const char* numeral, double* value);

/**
 * Checks if two strings match in the the next 1 or 2 characters, returning the
 * length of the match.
 *
 * For internal use only. It is case INsensitive.
 *
 * @param *to_be_compared the current position in the string to check against
 * the pattern
 * @param *pattern the position in the string  from the dictionary containing
 * the 1 or 2 characters that may be in to_be_compared
 * @returns length of the match as uint8_t: 0 if they don't match or 1 or 2
 * if they match for that number of characters.
 */
static uint8_t string_begins_with(
        char* to_be_compared,
        const char* pattern)
{

    size_t pattern_length = strlen(pattern);
    /* Compare the first `pattern_length` characters */
    if (strncasecmp(to_be_compared, pattern, pattern_length) == 0)
    {
        return (uint8_t) pattern_length;
    }
    else
    {
        return 0;
    }
}


/**
 * Initializer of the numeral_parser_data data structure.
 *
 * Sets the fields to be ready to start the conversion from roman to value.
 */
static void init_parser_data(
        struct numeral_parser_data* parser_data,
        char* roman)
{

    parser_data->current_numeral_position = roman;
    parser_data->current_dictionary_char = &DICTIONARY[DICTIONARY_INDEX_FOR_M];
    parser_data->numeral_is_long = false;
    parser_data->numeral_sign = 1;
    parser_data->int_part = 0;
    parser_data->twelfths = 0;
    parser_data->char_repetitions = 0;
}


/**
 * Checks if the char appears in the string.
 *
 * @param current char to search in the string
 * @param *terminating_chars the string to be searched in
 * @returns `true` if the char `current` is `\0` or appears in the string
 * `*terminating_chars`. `false` otherwise.
 */
static bool char_is_in_string(char current, char* terminating_chars)
{

    if (current == '\0')
    {
        return true;
    }
    while (*terminating_chars != '\0')
    {
        if (current == *terminating_chars)
        {
            return true;
        }
        else
        {
            terminating_chars++;
        }
    }
    return false;
}


/**
 * Moves the pointer from the current dictionary character to the next that has
 * max_repetitions more than 1.
 *
 * As an exception to this rule, if the dictionary character at the start of
 * the skipping is not "V" or "L" or "D", another character is skipped, since
 * those three are the only ones that can have an "I" or "X" or "C" appended
 * to them.
 *
 * This forces just one of the characters that exclude each other. An example
 * written as regex: (CM)|(CD)|(D?C{0,3}).
 *
 * The computation result is stored in the passed numeral_parser_data.
 *
 * @param *parser_data of the numeral that is currently being converted to value
 */
static void skip_to_next_non_unique_dictionary_char(
        struct numeral_parser_data* parser_data)
{

    bool current_char_is_multiple_of_five =
            strlen(parser_data->current_dictionary_char->characters) == 1;
    while (parser_data->current_dictionary_char->max_repetitions == 1)
    {
        parser_data->current_dictionary_char++;
        parser_data->char_repetitions = 0;
    }
    if (!current_char_is_multiple_of_five)
    {
        parser_data->current_dictionary_char++;
    }
}


/**
 * Parses one single roman character of a roman numeral, comparing it to the
 * dictionary.
 *
 * Confronts the currently pointed position of the roman numeral with the
 * currently pointed roman character in the dictionary. If they match, advances
 * to the next character in the numeral, up to a maximum number of repetitions.
 * If they don't, advances to the next dictionary roman char - it has to be
 * called again to perform the check.
 *
 * The computation result is stored in the passed numeral_parser_data.
 *
 * @param *parser_data of the numeral that is currently being converted to value
 * @returns result code as an error or NUMERUS_OK.
 */
static int8_t compare_numeral_position_with_dictionary(
        struct numeral_parser_data* parser_data)
{

    uint8_t num_of_matching_chars = string_begins_with(
            parser_data->current_numeral_position,
            parser_data->current_dictionary_char->characters);

    if (num_of_matching_chars > 0)
    {
        /* Chars match */
        parser_data->char_repetitions++;
        if (parser_data->char_repetitions >
            parser_data->current_dictionary_char->max_repetitions)
        {
            return NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS;
        }
        parser_data->current_numeral_position += num_of_matching_chars;
        if (*(parser_data->current_dictionary_char->characters) == 'S'
            || *(parser_data->current_dictionary_char->characters) == '.')
        {
            /* Add to decimal part value */
            parser_data->twelfths
                    +=
                    parser_data->current_dictionary_char->value;
        }
        else
        {
            /* Add to integer part value */
            parser_data->int_part
                    +=
                    parser_data->current_dictionary_char->value;
        }
        skip_to_next_non_unique_dictionary_char(parser_data);
    }
    else
    {
        /* Chars don't match */
        parser_data->char_repetitions = 0;
        parser_data->current_dictionary_char++;
        if (parser_data->current_dictionary_char->max_repetitions == 0)
        {
            return NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE;
        }
    }
    return NUMERUS_OK;
}


/**
 * Parses the numeral part between underscores.
 *
 * Parses each position in the numeral with until a stopping character
 * is found (one of "_Ss.-\0"). If an illegal stopping character is found,
 * a specific error code is returned.
 *
 * The computation result is stored in the passed numeral_parser_data.
 *
 * @param *parser_data of the numeral that is currently being converted to value
 * @returns result code as an error or NUMERUS_OK.
 */
static int8_t parse_part_in_underscores(
        struct numeral_parser_data* parser_data)
{

    while (!char_is_in_string(*(parser_data->current_numeral_position),
                              "_Ss.-"))
    {
        int8_t result_code = compare_numeral_position_with_dictionary(
                parser_data);
        if (result_code != NUMERUS_OK)
        {
            return result_code;
        }
    }
    if (*(parser_data->current_numeral_position) == '\0')
    {
        return NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE;
    }
    if (char_is_in_string(*(parser_data->current_numeral_position),
                          "sS."))
    {
        return NUMERUS_ERROR_DECIMALS_IN_LONG_PART;
    }
    if (*(parser_data->current_numeral_position) == '-')
    {
        return NUMERUS_ERROR_ILLEGAL_MINUS_POSITION;
    }
    return NUMERUS_OK;
}


/**
 * Parses the numeral part after the second underscore, if any, or the whole
 * numeral, if it's not a long numeral.
 *
 * Parses each position in the numeral until a stopping character is found
 * (one of "M_Ss.-\0"). If an illegal stopping character is found, a
 * specific error code is returned. "M" is an illegal character in the part
 * after underscores only in long numerals.
 *
 * The computation result is stored in the passed numeral_parser_data.
 *
 * @param *parser_data of the numeral that is currently being converted to value
 * @returns result code as an error or NUMERUS_OK.
 */
static int8_t parse_part_after_underscores(
        struct numeral_parser_data* parser_data)
{

    char* stop_chars;
    if (parser_data->numeral_is_long)
    {
        stop_chars = "Ss.M_-";
    }
    else
    {
        stop_chars = "Ss._-";
    }
    while (!char_is_in_string(*(parser_data->current_numeral_position),
                              stop_chars))
    {
        int8_t result_code = compare_numeral_position_with_dictionary(
                parser_data);
        if (result_code != NUMERUS_OK)
        {
            return result_code;
        }
    }
    if (*(parser_data->current_numeral_position) == '_')
    {
        if (parser_data->numeral_is_long)
        {
            return NUMERUS_ERROR_TOO_MANY_UNDERSCORES;
        }
        else
        {
            return NUMERUS_ERROR_ILLEGAL_FIRST_UNDERSCORE_POSITION;
        }
    }
    if (*(parser_data->current_numeral_position) == 'M')
    {
        return NUMERUS_ERROR_M_IN_SHORT_PART;
    }
    if (*(parser_data->current_numeral_position) == '-')
    {
        return NUMERUS_ERROR_ILLEGAL_MINUS_POSITION;
    }
    return NUMERUS_OK;
}


/**
 * Parses the numeral part with decimal characters, if any.
 *
 * Parses each position in the numeral until a stopping character is found
 * (one of "_-\0"). If an illegal stopping character is found, a
 * specific error code is returned.
 *
 * The computation result is stored in the passed numeral_parser_data.
 *
 * @param *parser_data of the numeral that is currently being converted to value
 * @returns result code as an error or NUMERUS_OK.
 */
static int8_t parse_decimal_part(
        struct numeral_parser_data* parser_data)
{

    while (!char_is_in_string(*(parser_data->current_numeral_position),
                              "_-"))
    {
        int8_t result_code = compare_numeral_position_with_dictionary(
                parser_data);
        if (result_code != NUMERUS_OK)
        {
            return result_code;
        }
    }
    if (*(parser_data->current_numeral_position) == '_')
    {
        if (parser_data->numeral_is_long)
        {
            return NUMERUS_ERROR_TOO_MANY_UNDERSCORES;
        }
        else
        {
            return NUMERUS_ERROR_ILLEGAL_FIRST_UNDERSCORE_POSITION;
        }
    }
    if (*(parser_data->current_numeral_position) == '-')
    {
        return NUMERUS_ERROR_ILLEGAL_MINUS_POSITION;
    }
    return NUMERUS_OK;
}


/**
 * Converts a roman numeral to its value expressed as a double.
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
 * than NUMERUS_OK, an error occurred during the conversion and the returned
 * value is outside the possible range of values of roman numerals.
 * The error code may help find the specific error.
 *
 * @param *roman string with a roman numeral
 * @param *errcode int8_t where to store the conversion status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns double value of the roman numeral or a value outside the the
 * possible range of values when an error occurs.
 */
double numerus_roman_to_double(char* roman, int8_t* errcode)
{
    int32_t int_part;
    uint8_t twelfths;
    int_part =
            numerus_roman_to_int_part_and_twelfths(roman, &twelfths, errcode);
    return numerus_parts_to_double(int_part, twelfths);
}


/**
 * Converts a roman numeral to its value floored and expressed as long integer.
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
 * than NUMERUS_OK, an error occurred during the conversion and the returned
 * value is outside the possible range of values of roman numerals.
 * The error code may help find the specific error.
 *
 * @param *roman string with a roman numeral
 * @param *errcode int8_t where to store the conversion status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns long value of the roman numeral or a value outside the the
 * possible range of values when an error occurs.
 */
int32_t numerus_roman_to_int(char* roman, int8_t* errcode)
{
    return numerus_roman_to_int_part_and_twelfths(roman, 0, errcode);
}


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
 * than NUMERUS_OK, an error occurred during the conversion and the returned
 * value is outside the possible range of values of roman numerals.
 * The error code may help find the specific error.
 *
 * The number of twelfths is stored in the passed parameter, while the integer
 * part is returned directly.
 *
 * @param *roman string with a roman numeral
 * @param *errcode int where to store the conversion status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @param *twelfths number of twelfths from 0 to 11. NULL is interpreted as 0
 * twelfths.
 * @returns int32_t as the integer part of the value of the roman numeral or a
 * value outside the the possible range of values when an error occurs.
 */
int32_t numerus_roman_to_int_part_and_twelfths(
        char* roman, uint8_t* twelfths,
        int8_t* errcode)
{
    /* Prepare variables */
    int32_t int_part;
    int8_t response_code;
    uint8_t zero_twelfths = 0;
    if (twelfths == NULL)
    {
        twelfths = &zero_twelfths;
    }
    if (errcode == NULL)
    {
        errcode = &numerus_error_code;
    }
    struct numeral_parser_data parser_data;
    init_parser_data(&parser_data, roman);

    /* Check for illegal symbols or length */
    numerus_count_roman_chars(roman, &response_code);
    if (response_code != NUMERUS_OK)
    {
        numerus_error_code = response_code;
        *errcode = response_code;
        return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
    }

    /* Skip initial whitespace */
    while (isspace(*roman))
    {
        roman++;
    }

    /* Conversion if NUMERUS_NULLA */
    if (is_zero(roman))
    {
        int_part = 0;
        *twelfths = 0;
        numerus_error_code = NUMERUS_OK;
        *errcode = NUMERUS_OK;
        return int_part;
    }

    /* Conversion of other cases */
    if (*parser_data.current_numeral_position == '-')
    {
        parser_data.numeral_sign = -1;
        parser_data.current_numeral_position++;
    }
    if (*parser_data.current_numeral_position == '_')
    {
        parser_data.current_numeral_position++;
        parser_data.numeral_is_long = 1;
    }
    if (parser_data.numeral_is_long)
    {
        response_code = parse_part_in_underscores(&parser_data);
        if (response_code != NUMERUS_OK)
        {
            numerus_error_code = response_code;
            *errcode = response_code;
            return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
        }
        parser_data.current_numeral_position++; /* Skip second underscore */
        parser_data.int_part *= 1000;
        parser_data.current_dictionary_char = &_NUM_DICTIONARY[1];
        parser_data.char_repetitions = 0;
    }
    response_code = parse_part_after_underscores(&parser_data);
    if (response_code != NUMERUS_OK)
    {
        numerus_error_code = response_code;
        *errcode = response_code;
        return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
    }
    response_code = parse_decimal_part(&parser_data);
    if (response_code != NUMERUS_OK)
    {
        numerus_error_code = response_code;
        *errcode = response_code;
        return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
    }
    int_part = parser_data.numeral_sign * parser_data.int_part;
    *twelfths = parser_data.numeral_sign * parser_data.twelfths;
    numerus_error_code = NUMERUS_OK;
    *errcode = NUMERUS_OK;
    return int_part;
}
