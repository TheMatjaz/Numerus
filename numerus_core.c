/**
 * @file numerus_core.c
 * @brief Numerus constants and functions for roman numerals conversion.
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 */

#include <ctype.h>    /* For `isspace()` */
#include <stdlib.h>   /* For `malloc()` */
#include <string.h>   /* For `strlen()`, `strncasecmp()`, `strcpy()` */
#include <stdbool.h>  /* To use booleans `true` and `false` */
#include "numerus_internal.h"




/*  -+-+-+-+-+-+-+-+-+-+-+-+-+-{   CONSTANTS   }-+-+-+-+-+-+-+-+-+-+-+-+-+-  */


/**
 * The maximum value a roman numeral with underscores without decimals may have.
 */
const long NUMERUS_MAX_LONG_NONFLOAT_VALUE = 3999999;


/**
 * The maximum value a roman numeral may have.
 *
 * This is for a positive long and float roman numeral. It exceeds the actual
 * maximum value by 1/24 because conversion functions round the value to the
 * nearest 12th.
 */
const double NUMERUS_MAX_VALUE = NUMERUS_MAX_LONG_NONFLOAT_VALUE + 11.5 / 12.0;


/**
 * The minimum value a roman numeral with underscores without decimals may have.
 *
 * It's the opposite of NUMERUS_MAX_LONG_NONFLOAT_VALUE.s
 */
const long int NUMERUS_MIN_LONG_NONFLOAT_VALUE = -NUMERUS_MAX_LONG_NONFLOAT_VALUE;


/**
 * The minimum value a roman numeral may have.
 *
 * It's the opposite of NUMERUS_MAX_VALUE. This is for a negative long and float
 * roman numeral. It is below the actual maximum value by 1/24 because
 * conversion functions round the value to the nearest 12th.
 */
const double NUMERUS_MIN_VALUE = -NUMERUS_MAX_VALUE;


/**
 * The maximum value a roman numeral without underscores with decimals may have.
 *
 * If you floor() this value, you get the maximum value the standard roman
 * syntax allows.
 */
const double NUMERUS_MAX_NONLONG_FLOAT_VALUE = 3999 + 11.5 / 12.0;


/**
 * The minimum value a roman numeral without underscores with decimals may have.
 *
 * If you ceil() this value, you get the minimum value the standard roman
 * syntax allows.
 */
const double NUMERUS_MIN_NONLONG_FLOAT_VALUE = -NUMERUS_MAX_NONLONG_FLOAT_VALUE;


/**
 * The roman numeral of value 0 (zero).
 *
 * Both for positive and negative zero.
 */
const char *NUMERUS_ZERO = "NULLA";


/**
 * The maximum length a roman numeral string may have, including '\0'.
 *
 * The roman numeral `"-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS....."`
 * (value: -3888888, -11/12) + `\0` is a string long 36+1 = 37 chars.
 */
const short int NUMERUS_MAX_LENGTH = 37;



/*  -+-+-+-+-+-+-+-+-{   VARIABLES and DATA STRUCTURES   }-+-+-+-+-+-+-+-+-  */


/**
 * The elobal error code variable to store any errors during conversions.
 *
 * It may contain any of the NUMERUS_ERROR_* error codes or NUMERUS_OK.
 */
int numerus_error_code = NUMERUS_OK;


/*
 * Buffer where the strings with roman numerals are build an then copied from.
 *
 * This buffer is as long as the longest roman numeral. The usage of this buffer
 * allows one conversion at the time but is more memory efficient since the
 * roman numerals have variable length and can be returned as a string copied
 * from the buffer with just the right amount of space allocated.
 */


/**
 * Struct containing a basic roman char, its double value and the maximum
 * consecutive repetition of it that a roman numeral may have.
 *
 * It's used to create the _NUM_DICTIONARY dictionary and used by conversion
 * functions. The "roman chars" as called in this library are strings of 1 or 2
 * chars that have a specific a priori known value.
 */
struct _num_dictionary_char {
    const int value;
    const char *characters;
    const short max_repetitions;
};


/**
 * Dictionary of _num_dictionary_char used by conversion functions.
 *
 * The last value is a terminator, to be used by conversion function to
 * understand that the array has been parsed.
 */
static const struct _num_dictionary_char _NUM_DICTIONARY[] = {
    { 1000, "M" ,  3 }, // index: 0
    {  900, "CM",  1 }, // index: 1
    {  500, "D" ,  1 }, // index: 2
    {  400, "CD",  1 }, // index: 3
    {  100, "C" ,  3 }, // index: 4
    {   90, "XC",  1 }, // index: 5
    {   50, "L" ,  1 }, // index: 6
    {   40, "XL",  1 }, // index: 7
    {   10, "X" ,  3 }, // index: 8
    {    9, "IX",  1 }, // index: 9
    {    5, "V" ,  1 }, // index: 10
    {    4, "IV",  1 }, // index: 11
    {    1, "I" ,  3 }, // index: 12
    {    6, "S" ,  1 }, // index: 13
    {    1, "." ,  5 }, // index: 14
    {    0, NULL,  0 }  // index: 15
};


/*  -+-+-+-+-+-+-+-+-+-{   CONVERSION ROMAN -> VALUE   }-+-+-+-+-+-+-+-+-+-  */



/**
 * Checks if two strings match in the the next 1 or 2 characters.
 *
 * It is case insensitive. This functions is used by numerus_roman_to_short()
 * and is missing many security check since is internal and is meant to be
 * called on _NUM_DICTIONARY.
 *
 * @param *to_compare the current position in the string to check against the
 * pattern
 * @param *pattern the position in the string containing the 1 or 2 characters
 * that may be in *to_compare
 * @returns length of the match as short: 0 if they don't match or 1 or 2
 * if they match.
 */
static short _num_string_begins_with(char *to_be_compared,
                                     const char *pattern) {

    size_t pattern_length = strlen(pattern);
    if (strncasecmp(to_be_compared, pattern, pattern_length) == 0) {
        /* Compare the first pattern_length characters */
        return (short) pattern_length;
    } else {
        return 0;
    }
}


/**
 * Structure containing data about the numeral that is being converted to value.
 *
 * Used by `numerus_roman_to_value()` and other static functions. Contains info
 * about the currently parsed part of the numeral, the dictionary char that the
 * numeral is confronted with, the value, sign, if it has underscore and counts
 * the number of consecutive repetitions a single roman char has.
 *
 * @see numerus_roman_to_double()
 */
struct _num_numeral_parser_data {
    char *current_numeral_position;
    const struct _num_dictionary_char *current_dictionary_char;
    bool numeral_is_long;
    short numeral_sign;
    long int_part;
    short frac_part;
    short char_repetitions;
};


/**
 * Initializer of the _num_numeral_parser_data data structure.
 *
 * Sets the fields to be ready to start the conversion from roman to value.
 *
 * @see numerus_roman_to_double()
 */
static void _num_init_parser_data(struct _num_numeral_parser_data *parser_data,
                                  char *roman) {

    parser_data->current_numeral_position = roman;
    parser_data->current_dictionary_char = &_NUM_DICTIONARY[0];
    parser_data->numeral_is_long = false;
    parser_data->numeral_sign = 1;
    parser_data->int_part = 0;
    parser_data->frac_part = 0;
    parser_data->char_repetitions = 0;
}


/**
 * Finds if the passed char appears in the passed string.
 *
 * Used by _num_parse_part_after_underscores() and
 * _num_parse_part_in_underscores()
 *
 * @returns `true` if the char `current` is `\0` or appears in the string
 * `*terminating_chars`. `false` otherwise.
 */
static bool _num_char_is_in_string(char current, char *terminating_chars) {

    if (current == '\0') {
        return true;
    }
    while (*terminating_chars != '\0') {
        if (current == *terminating_chars) {
            return true;
        } else {
            terminating_chars++;
        }
    }
    return false;
}


/**
 * Moves the pointer to the current dictionary character to the next that has
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
 * Used by: _num_compare_numeral_position_with_dictionary()
 * which is used by numerus_roman_to_double()
 */
static void _num_skip_to_next_non_unique_dictionary_char(
        struct _num_numeral_parser_data *parser_data) {

    short current_char_is_multiple_of_five =
            strlen(parser_data->current_dictionary_char->characters) == 1;
    while (parser_data->current_dictionary_char->max_repetitions == 1) {
        parser_data->current_dictionary_char++;
        parser_data->char_repetitions = 0;
    }
    if (!current_char_is_multiple_of_five) {
        parser_data->current_dictionary_char++;
    }
}

/**
 * Parses one single roman character of a roman numeral.
 *
 * Confronts the currently pointed position of the roman numeral with the
 * currently pointed roman character in the dictionary. If they match, advances
 * to the next character in the numeral, up to a maximum number of repetitions.
 * If they don't, advances to the next dictionary roman char - it has to be
 * called again to perform the check.
 *
 * Used by: _num_parse_part_in_underscores() and
 * _num_parse_part_after_underscores() which are used by
 * numerus_roman_to_double()
 *
 * @returns result code as an error or NUMERUS_OK. The computation result is
 * stored in the passed _num_numeral_parser_data. Possible errors are
 * NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS or NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE
 */
static int _num_compare_numeral_position_with_dictionary(
        struct _num_numeral_parser_data *parser_data) {

    short num_of_matching_chars = _num_string_begins_with(
            parser_data->current_numeral_position,
            parser_data->current_dictionary_char->characters);
    if (num_of_matching_chars > 0) { /* chars match */
        parser_data->char_repetitions++;
        if (parser_data->char_repetitions >
            parser_data->current_dictionary_char->max_repetitions) {
            return NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS;
        }
        parser_data->current_numeral_position += num_of_matching_chars;
        if (*(parser_data->current_dictionary_char->characters) == 'S'
            || *(parser_data->current_dictionary_char->characters) == '.') {
            // add to decimal part value
            parser_data->frac_part += parser_data->current_dictionary_char->value;
        } else {
            // add to integer part value
            parser_data->int_part += parser_data->current_dictionary_char->value;
        }
        _num_skip_to_next_non_unique_dictionary_char(parser_data);
    } else { /* chars don't match */
        parser_data->char_repetitions = 0;
        parser_data->current_dictionary_char++;
        if (parser_data->current_dictionary_char->max_repetitions == 0) {
            return NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE;
        }
    }
    return NUMERUS_OK;
}


/**
 * Parses the numeral part between underscores.
 *
 * Parses each position in the numeral with
 * _num_compare_numeral_position_with_dictionary() until a stopping character
 * is found (one of "_Ss.-\0"). If an illegal stopping character is found,
 * a specific error code is returned.
 *
 * Used by: numerus_roman_to_double()
 *
 * @returns result code as an error or NUMERUS_OK. Possible errors are
 * NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE,
 * NUMERUS_ERROR_DECIMALS_IN_LONG_PART or NUMERUS_ERROR_ILLEGAL_MINUS.
 */
static int _num_parse_part_in_underscores(
        struct _num_numeral_parser_data *parser_data) {

    while (!_num_char_is_in_string(*(parser_data->current_numeral_position),
                                   "_Ss.-")) {
        int result_code = _num_compare_numeral_position_with_dictionary(
                parser_data);
        if (result_code != NUMERUS_OK) {
            return result_code;
        }
    }
    if (*(parser_data->current_numeral_position) == '\0') {
        return NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE;
    }
    if (_num_char_is_in_string(*(parser_data->current_numeral_position),
                               "sS.")) {
        return NUMERUS_ERROR_DECIMALS_IN_LONG_PART;
    }
    if (*(parser_data->current_numeral_position) == '-') {
        return NUMERUS_ERROR_ILLEGAL_MINUS;
    }
    return NUMERUS_OK;
}


/**
 * Parses the numeral part after the second underscore, if any, or the whole
 * numeral, if it's not a long numeral.
 *
 * Parses each position in the numeral with
 * _num_compare_numeral_position_with_dictionary() until a stopping character
 * is found (one of "M_Ss.-\0"). If an illegal stopping character is found,
 * a specific error code is returned. "M" is an illegal character in the part
 * after underscores only in long numerals.
 *
 * Used by: numerus_roman_to_double()
 *
 * @returns result code as an error or NUMERUS_OK. Possible errors are
 * NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART,
 * NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG,
 * NUMERUS_ERROR_M_IN_SHORT_PART or NUMERUS_ERROR_ILLEGAL_MINUS.
 */
static int _num_parse_part_after_underscores(
        struct _num_numeral_parser_data *parser_data) {

    char *stop_chars;
    if (parser_data->numeral_is_long) {
        stop_chars = "Ss.M_-";
    } else {
        stop_chars = "Ss._-";
    }
    while (!_num_char_is_in_string(*(parser_data->current_numeral_position),
                                   stop_chars)) {
        int result_code = _num_compare_numeral_position_with_dictionary(
                parser_data);
        if (result_code != NUMERUS_OK) {
            return result_code;
        }
    }
    if (*(parser_data->current_numeral_position) == '_') {
        if (parser_data->numeral_is_long) {
            return NUMERUS_ERROR_UNDERSCORE_AFTER_LONG_PART;
        } else {
            return NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG;
        }
    }
    if (*(parser_data->current_numeral_position) == 'M') {
        return NUMERUS_ERROR_M_IN_SHORT_PART;
    }
    if (*(parser_data->current_numeral_position) == '-') {
        return NUMERUS_ERROR_ILLEGAL_MINUS;
    }
    return NUMERUS_OK;
}


static int _num_parse_decimal_part(
        struct _num_numeral_parser_data *parser_data) {
    while (!_num_char_is_in_string(*(parser_data->current_numeral_position),
                                   "_-")) {
        int result_code = _num_compare_numeral_position_with_dictionary(
                parser_data);
        if (result_code != NUMERUS_OK) {
            return result_code;
        }
    }
    if (*(parser_data->current_numeral_position) == '_') {
        if (parser_data->numeral_is_long) {
            return NUMERUS_ERROR_UNDERSCORE_AFTER_LONG_PART;
        } else {
            return NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG;
        }
    }
    if (*(parser_data->current_numeral_position) == '-') {
        return NUMERUS_ERROR_ILLEGAL_MINUS;
    }
    return NUMERUS_OK;
}

/**
 * Converts a roman numeral to its value as a double.
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
 * The value of the numeral is stored into the referenced double passed as
 * parameter while the returning value of the functions is an error code. If
 * it is different than NUMERUS_OK, then the roman numeral has wrong syntax and
 * could not be parsed. The error code may help find the specific error.
 *
 * @param *roman string with a roman numeral
 * @param *value double where to store the value of the numeral
 * @returns int error code to indicate success (NUMERUS_OK) or failure
 * (NUMERUS_ERROR_*)
 */
double numerus_roman_to_double(char *roman, int *errcode) {
    long int_part;
    short frac_part;
    int_part = numerus_roman_to_int_and_frac_part(roman, &frac_part, errcode);
    return numerus_parts_to_double(int_part, frac_part);
}

long numerus_roman_to_int(char *roman, int *errcode) {
    return numerus_roman_to_int_and_frac_part(roman, 0, errcode);
}

long numerus_roman_to_int_and_frac_part(char *roman, short *frac_part, int *errcode) {
    long int_part;
    int response_code;
    short zero_frac_part = 0;
    if (frac_part == NULL) {
        frac_part = &zero_frac_part;
    }
    if (errcode == NULL) {
        errcode = &numerus_error_code;
    }
    struct _num_numeral_parser_data parser_data;
    _num_init_parser_data(&parser_data, roman);
    numerus_numeral_length(roman, &response_code);
    if (response_code != NUMERUS_OK) {
        numerus_error_code = response_code;
        *errcode = response_code;
        return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
    }
    while (isspace(*roman)) {
        roman++;
    }
    if (_num_is_zero(roman)) {
        int_part = 0;
        *frac_part = 0;
        numerus_error_code = NUMERUS_OK;
        *errcode = NUMERUS_OK;
        return int_part;
    }
    if (*parser_data.current_numeral_position == '-') {
        parser_data.numeral_sign = -1;
        parser_data.current_numeral_position++;
    }
    if (*parser_data.current_numeral_position == '_') {
        parser_data.current_numeral_position++;
        parser_data.numeral_is_long = 1;
    }
    if (parser_data.numeral_is_long) {
        response_code = _num_parse_part_in_underscores(&parser_data);
        if (response_code != NUMERUS_OK) {
            numerus_error_code = response_code;
            *errcode = response_code;
            return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
        }
        parser_data.current_numeral_position++; // skip second underscore
        parser_data.int_part *= 1000;
        // reset back to "CM", "M" is excluded for long numerals
        parser_data.current_dictionary_char = &_NUM_DICTIONARY[1];
        parser_data.char_repetitions = 0;
    }
    response_code = _num_parse_part_after_underscores(&parser_data);
    if (response_code != NUMERUS_OK) {
        numerus_error_code = response_code;
        *errcode = response_code;
        return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
    }
    response_code = _num_parse_decimal_part(&parser_data);
    if (response_code != NUMERUS_OK) {
        numerus_error_code = response_code;
        *errcode = response_code;
        return NUMERUS_MAX_LONG_NONFLOAT_VALUE + 10;
    }
    int_part = parser_data.numeral_sign * parser_data.int_part;
    *frac_part = parser_data.frac_part;
    numerus_error_code = NUMERUS_OK;
    *errcode = NUMERUS_OK;
    return int_part;
}



/*  -+-+-+-+-+-+-+-+-+-{   CONVERSION VALUE -> ROMAN   }-+-+-+-+-+-+-+-+-+-  */


/**
 * Copies a string of 1 or 2 characters.
 *
 * Copies the character from the source to the destination. If there is another
 * character after that, that is not the null terminator, copies that as well.
 * Everything is performed without security checks for faster performance. This
 * function is used by numerus_short_to_roman() and it's meant to be used just
 * on the _num_char_struct in the dictionary _NUM_DICTIONARY.
 *
 * @param *source the string of 1-2 characters to copy
 * @param *destination the string, already allocated, to copy the *source into
 * @returns the new position of the destination pointer after the characters 
 * have been copied
 */
static char *_num_copy_char_from_dictionary(const char *source,
                                            char *destination) {
    
    *destination = *(source++);
    if (*source != '\0') {
        *(++destination) = *source;
    }
    return ++destination;
}


/**
 * Converts just the internal part of the underscores or the part after them.
 *
 * Stores it into `*roman_string`.
 *
 * @returns position after the inserted string.
 */
static char *_num_value_part_to_roman(long value, char *roman, int dictionary_start_char) {
    const struct _num_dictionary_char *current_dictionary_char = &_NUM_DICTIONARY[dictionary_start_char];
    while (value > 0) {
        while (value >= current_dictionary_char->value) {
            roman = _num_copy_char_from_dictionary(
                    current_dictionary_char->characters, roman);
            value -= current_dictionary_char->value;
        }
        current_dictionary_char++;
    }
    return roman;
}

char *numerus_int_to_roman(long int_value, int *errcode) {
    return numerus_int_with_twelfth_to_roman(int_value, 0, errcode);
}

char *numerus_double_to_roman(double double_value, int *errcode) {
    short frac_part;
    long int_part = numerus_double_to_parts(double_value, &frac_part);
    return numerus_int_with_twelfth_to_roman(int_part, frac_part, errcode);
}

char *numerus_int_with_twelfth_to_roman(long int_part, short frac_part, int *errcode) {
    /* Prepare variables */
    frac_part = ABS(frac_part);
    double double_value = numerus_parts_to_double(int_part, frac_part);
    if (errcode == NULL) {
        errcode = &numerus_error_code;
    }

    /* Out of range check */
    if (double_value < NUMERUS_MIN_VALUE || double_value > NUMERUS_MAX_VALUE) {
        numerus_error_code = NUMERUS_ERROR_VALUE_OUT_OF_RANGE;
        *errcode = NUMERUS_ERROR_VALUE_OUT_OF_RANGE;
        return NULL;
    }

    /* Create pointer to the building buffer */
    char building_buffer[NUMERUS_MAX_LENGTH];
    char *roman_numeral = building_buffer;

    /* Save sign or return NUMERUS_ZERO for 0 */
    if (int_part == 0 && frac_part == 0) {
        /* Return writable copy of NUMERUS_ZERO on the heap */
        char *zero_string = malloc(strlen(NUMERUS_ZERO) + 1);
        strcpy(zero_string, NUMERUS_ZERO);
        return zero_string;
    } else if (int_part < 0) {
        int_part = ABS(int_part);
        double_value = ABS(double_value);
        *(roman_numeral++) = '-';
    }

    /* Create part between underscores */
    if (double_value > NUMERUS_MAX_NONLONG_FLOAT_VALUE) { /* Underscores are needed */
        *(roman_numeral++) = '_';
        roman_numeral = _num_value_part_to_roman(int_part / 1000, roman_numeral, 0); /* Integer cast to avoid decimals in part between underscores */
        int_part -= (int_part / 1000) * 1000; /* Remove the three left-most digits because of the integer division */
        *(roman_numeral++) = '_';
        /* Part after underscores without "M" char, start parsing the dictonary with "CM" */
        roman_numeral = _num_value_part_to_roman(int_part, roman_numeral, 1);
    } else {
        /* No underscores, so with "M" char */
        roman_numeral = _num_value_part_to_roman(int_part, roman_numeral, 0);
    }
    /* Decimal part, start parsing the dictonary with "S" */
    roman_numeral = _num_value_part_to_roman(frac_part, roman_numeral, 13);

    *(roman_numeral++) = '\0';

    /* Copy out of the buffer and return it on the heap */
    char *returnable_roman_string =
            malloc(roman_numeral - building_buffer);
    if (returnable_roman_string == NULL) {
        numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
        *errcode = NUMERUS_ERROR_MALLOC_FAIL;
        return NULL;
    }
    strcpy(returnable_roman_string, building_buffer);
    numerus_error_code = NUMERUS_OK;
    *errcode = NUMERUS_OK;
    return returnable_roman_string;
}
