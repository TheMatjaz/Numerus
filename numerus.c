/**
 * @file numerus.c
 * @brief Numerus constants and functions for roman numerals conversion.
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 */

#include <math.h>     /* For `round()`  */
#include <ctype.h>    /* For `upcase()` */
#include <stdio.h>    /* To  `fprintf()` to `stderr` */
#include <stdlib.h>   /* For `malloc()` */
#include <string.h>   /* For `strcmp()` */
#include <stdbool.h>  /* To use booleans `true` and `false` */
#include "numerus.h"




/*  -+-+-+-+-+-+-+-+-+-+-+-+-+-{   CONSTANTS   }-+-+-+-+-+-+-+-+-+-+-+-+-+-  */


/**
 * Maximum value a long roman numeral (with '_') may have.
 */
const long int NUMERUS_MAX_LONG_VALUE = 3999999;


const double NUMERUS_MAX_VALUE = NUMERUS_MAX_LONG_VALUE + 11.0 / 12.0;


/**
 * Minimum value a long a roman numeral (with '_') may have.
 */
const long int NUMERUS_MIN_LONG_VALUE = -NUMERUS_MAX_LONG_VALUE;


const double NUMERUS_MIN_VALUE = -NUMERUS_MAX_VALUE;


/**
 * Maximum value a short roman numeral (without '_') may have.
 */
const short int NUMERUS_MAX_SHORT_VALUE = 3999;


/**
 * Minimum value a short roman numeral (without '_') may have.
 */
const short int NUMERUS_MIN_SHORT_VALUE = -NUMERUS_MAX_SHORT_VALUE;


/**
 * Roman numeral of value 0 (zero).
 */
const char *NUMERUS_ZERO = "NULLA";


/**
 * Maximum length of a float roman numeral string including the null terminator.
 *
 * The roman numeral `"-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS....."`
 * (value: -3888888 - 11+12) + `\0` is a string long 36+1 = 37 chars.
 */
const short int NUMERUS_MAX_LENGTH = 37;


/**
 * Maximum length of a long non-float roman numeral string including the null
 * terminator.
 *
 * The roman numeral `"-_MMMDCCCLXXXVIII_DCCCLXXXVIII"`
 * (value: -3888888) + `\0` is a string long 30+1 = 31 chars.
 */
const short int NUMERUS_MAX_LONG_LENGTH = 31;


/**
 * Maximum length of a short non-long non-float roman numeral string including
 * the null terminator.
 *
 * The roman numeral `"-MMMDCCCLXXXVIII"`
 * (value: -3888) + `\0` is a string long 16+1 = 17 chars.
 */
const short int NUMERUS_MAX_SHORT_LENGTH = 17;




/*  -+-+-+-+-+-+-+-+-{   VARIABLES and DATA STRUCTURES   }-+-+-+-+-+-+-+-+-  */


/**
 * Global error code variable to store any errors during conversions.
 *
 * It may contain any of the NUMERUS_ERROR_* error codes or NUMERUS_OK.
 */
int numerus_error_code = NUMERUS_OK;


/**
 * Buffer where the strings with roman numerals are build an then copied from.
 *
 * This buffer is as long as the longest roman numeral. The usage of this buffer
 * allows one conversion at the time but is more memory efficient since the
 * roman numerals have variable length and can be returned as a string copied
 * from the buffer with just the right amount of space allocated.
 */
static char _num_numeral_build_buffer[NUMERUS_MAX_LENGTH];


/**
 * Struct containing a basic roman char, its double value and the maximum
 * consecutive repetition of it that a roman numeral may have.
 *
 * It's used to create the _NUM_DICTIONARY dictionary and used by conversion
 * functions. The "roman chars" as called in this library are strings of 1 or 2
 * chars that have a specific a priori known value.
 */
struct _num_dictionary_char {
    const double value;
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
    { 1000.0, "M" ,  3 },
    {  900.0, "CM",  1 },
    {  500.0, "D" ,  1 },
    {  400.0, "CD",  1 },
    {  100.0, "C" ,  3 },
    {   90.0, "XC",  1 },
    {   50.0, "L" ,  1 },
    {   40.0, "XL",  1 },
    {   10.0, "X" ,  3 },
    {    9.0, "IX",  1 },
    {    5.0, "V" ,  1 },
    {    4.0, "IV",  1 },
    {    1.0, "I" ,  3 },
    {    0.5, "S" ,  1 },
    { 1/12.0, "." ,  5 },
    {    0.0, NULL,  0 }
};




/*  -+-+-+-+-+-+-+-+-+-+-{   CONVERSION FUNCTIONS   }-+-+-+-+-+-+-+-+-+-+-  */


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
 * numeral is confronted with, the value, sign, length of the numeral, if it has
 * underscore and counts the number of consecutive repetitions a single roman
 * char has.
 *
 * @see numerus_roman_to_value()
 */
struct _num_numeral_parser_data {
    char *current_numeral_position;
    const struct _num_dictionary_char *current_dictionary_char;
    bool numeral_is_long;
    short numeral_length;
    short numeral_sign;
    double numeral_value;
    short char_repetitions;
};


/**
 * Initializer of the _num_numeral_parser_data data structure.
 *
 * Sets the fields to be ready to start the conversion from roman to value.
 *
 * @see numerus_roman_to_value()
 */
static void _num_init_parser_data(struct _num_numeral_parser_data *parser_data,
                                  char *roman) {

    parser_data->current_numeral_position = roman;
    parser_data->current_dictionary_char = &_NUM_DICTIONARY[0];
    parser_data->numeral_is_long = false;
    parser_data->numeral_length = -1;
    parser_data->numeral_sign = 1;
    parser_data->numeral_value = 0.0;
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
 * which is used by numerus_roman_to_value()
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
 * @returns result code as an error or NUMERUS_OK. The computation result is
 * stored in the passed _num_numeral_parser_data
 */
static int _num_compare_numeral_position_with_dictionary(
        struct _num_numeral_parser_data *parser_data) {
    short num_of_matching_chars = _num_string_begins_with(
            parser_data->current_numeral_position,
            parser_data->current_dictionary_char->characters);
    if (num_of_matching_chars > 0) {
        parser_data->char_repetitions++;
        if (parser_data->char_repetitions >
            parser_data->current_dictionary_char->max_repetitions) {
            return NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS;
        }
        parser_data->current_numeral_position += num_of_matching_chars;
        parser_data->numeral_value += parser_data->current_dictionary_char->value;
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
 *
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

static int _num_parse_part_after_underscores(
        struct _num_numeral_parser_data *parser_data) {
    char *stop_chars;
    if (parser_data->numeral_is_long) {
        stop_chars = "M_-";
    } else {
        stop_chars = "_-";
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
            return NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART;
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


int numerus_roman_to_value(char *roman, double *value) {
    if (numerus_is_zero(roman)) {
        *value = 0.0;
        return NUMERUS_OK;
    }
    short length = 0;
    int response_code = numerus_numeral_length(roman, &length);
    if (response_code != NUMERUS_OK) {
        return response_code;
    }
    struct _num_numeral_parser_data parser_data;
    _num_init_parser_data(&parser_data, roman);
    if (*parser_data.current_numeral_position == '-') {
        parser_data.numeral_sign = -1;
        parser_data.current_numeral_position++;
        parser_data.numeral_length++;
    }
    if (*parser_data.current_numeral_position == '_') {
        parser_data.current_numeral_position++;
        parser_data.numeral_length++;
        parser_data.numeral_is_long = 1;
    }
    if (parser_data.numeral_is_long) {
        response_code = _num_parse_part_in_underscores(&parser_data);
        if (response_code != NUMERUS_OK) {
            return response_code;
        }
        parser_data.current_numeral_position++; // skip second underscore
        parser_data.numeral_value *= 1000;
        // reset back to "CM", "M" is excluded for long numerals
        parser_data.current_dictionary_char = &_NUM_DICTIONARY[1];
        parser_data.char_repetitions = 0;
    }
    response_code = _num_parse_part_after_underscores(&parser_data);
    if (response_code != NUMERUS_OK) {
        return response_code;
    }
    *value = parser_data.numeral_sign * parser_data.numeral_value;
    return NUMERUS_OK;
}


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
static char *_num_short_to_roman(long int arabic, char *roman_string) {
    const struct _num_dictionary_char *current_roman_char = &_NUM_DICTIONARY[0];
    while (arabic > 0) {
        while (arabic >= current_roman_char->value) {
            roman_string = _num_copy_char_from_dictionary(
                    current_roman_char->characters, roman_string);
            arabic -= current_roman_char->value;
        }
        current_roman_char++;
    }
    return roman_string;
}

/* Just to be able to call it with more details and parameters
 * returns \0 position in the buffer (the start is easy to find, it's the buffer start itself) */
static char *_num_long_to_roman(long int arabic, int copy_out_of_buffer) {
    /* Out of range check */
    if (arabic < NUMERUS_MIN_LONG_VALUE || arabic > NUMERUS_MAX_LONG_VALUE) {
        numerus_error_code = NUMERUS_ERROR_OUT_OF_RANGE;
        fprintf(stderr,
                "Roman conversion error: short int %li out of range [%li, %li]\n",
                arabic, NUMERUS_MIN_LONG_VALUE, NUMERUS_MAX_LONG_VALUE);
        return NULL;
    }

    /* Create pointer to the building buffer */
    char *roman_string = &_num_numeral_build_buffer[0];

    /* Save sign or return NUMERUS_ZERO for 0 */
    if (arabic < 0) {
        arabic *= -1;
        *(roman_string++) = '-';
    } else if (arabic == 0) {
        /* Return writable copy of NUMERUS_ZERO */
        char *zero_string = malloc(strlen(NUMERUS_ZERO) + 1);
        strcpy(zero_string, NUMERUS_ZERO);
        return zero_string;
    }

    /* Create part between underscores */
    if (arabic > NUMERUS_MAX_SHORT_VALUE) {
        *(roman_string++) = '_';
        roman_string = _num_short_to_roman(arabic / 1000, roman_string);
        arabic -= (arabic / 1000) *
                  1000; /* Keep just the 3 right-most digits because of the integer division */
        *(roman_string++) = '_';
    }
    /* Create part after underscores */
    roman_string = _num_short_to_roman(arabic, roman_string);
    *roman_string = '\0';

    /* Copy out of the buffer and return it */
    if (copy_out_of_buffer) {
        char *returnable_roman_string =
                malloc(roman_string - _num_numeral_build_buffer);
        strcpy(returnable_roman_string, _num_numeral_build_buffer);
        return returnable_roman_string;
    } else {
        return roman_string;
    }
}

/**
 * Converts a short int to a roman numeral.
 *
 * It allocates a string with the roman numerals long just as required and
 * returns a pointer to it.  If the short is outside of [NUMERUS_MIN_LONG_VALUE,
 * NUMERUS_MAX_LONG_VALUE], the conversion is impossible.
 *
 * @returns pointer to a string containing the roman numeral, NULL if the short
 * is out of range.
 */
char *numerus_long_to_roman(long int arabic) {
    return _num_long_to_roman(arabic, true);
}

static double _num_round_to_nearest_12th(double decimal) {
    if (decimal < 0) {
        decimal *= -1;
    }
    decimal = round(decimal * 12) / 12; /* Round to nearest twelfth */
    return decimal;
}

/* pass it values in [0, 1[ to round to the nearest twelfth. Returns the numerator from 0 to 11 */
static short _num_nearest_12th_numerator(double decimal) {
    decimal = _num_round_to_nearest_12th(decimal);
    decimal = round(decimal * 12);
    return (short) decimal;
}

static void _num_append_decimal_part_to_numeral(double decimal_part,
                                                char *roman,
                                                short insert_minus) {
    if (decimal_part < 0) {
        decimal_part *= -1;
        if (insert_minus != 0) {
            *(roman++) = '-';
        }
    }
    short twelfth = _num_nearest_12th_numerator(decimal_part);
    if (twelfth != 0) { /* It's not just a long */
        if (twelfth > 5) { /* At least one half */
            *(roman++) = 'S';
            twelfth -= 6;
        }
        while (twelfth > 0) {
            *(roman++) = '.';
            twelfth--;
        }
    }
    *roman = '\0';
}

char *numerus_double_to_roman(double value) {
    /* Extract integer and decimal part from parameter */
    double integer_part_double;
    double decimal_part = modf(value, &integer_part_double);
    long integer_part = (long) integer_part_double;
    char *roman;
    char *returnable_roman_string;

    if (decimal_part == 0.0) {
        /* It's just a long */
        return _num_long_to_roman(integer_part, true);
    }

    if (integer_part != 0) { /* Example 2.7 or -2.7 */
        roman = _num_long_to_roman(integer_part, 0);
        if (roman == NULL) {
            return NULL;
        }
        _num_append_decimal_part_to_numeral(decimal_part, roman + strlen(roman),
                                            0);
        /* Copy from build buffer */
        returnable_roman_string = malloc(roman - _num_numeral_build_buffer);
        strcpy(returnable_roman_string, _num_numeral_build_buffer);
    } else { /* Example 0.7 or -0.7 */
        roman = malloc(8); /* '-' + 'S' + 5x '.' + '\0' */
        _num_append_decimal_part_to_numeral(decimal_part, roman, 1);
        /* Copy the values in smaller string */
        returnable_roman_string = malloc(strlen(roman));
        strcpy(returnable_roman_string, roman);
        free(roman);
    }
    return returnable_roman_string;
}





/**
 * Converts a roman numeral to a short int.
 *
 * It is case insensitive and accepts negative roman numerals. If the numeral
 * cannot be converted, it means it has wrong syntax. In that case a value
 * bigger than NUMERUS_MAX_LONG_VALUE is returned and the error code
 * NUMERUS_ERROR_WRONG_SYNTAX is stored in numerus_error_code.
 *
 * @param *roman string with a roman numeral
 * @returns short value of the roman numeral or a value bigger than 
 * NUMERUS_MAX_LONG_VALUE in case of error
 */

