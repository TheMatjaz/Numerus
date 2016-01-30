/**
 * @file numerus_utils.c
 * @brief Numerus constants and utility functions for roman numerals manipulation.
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 */

#include <math.h>     /* For `round()`, `floor()`  */
#include <ctype.h>    /* For `isspace()`    */
#include <stdio.h>    /* For `snprintf()`   */
#include <stdlib.h>   /* For `malloc()`     */
#include <string.h>   /* For `strcasecmp()` */
#include <stdbool.h>  /* To use booleans `true` and `false` */
#include "numerus_internal.h"


/**
 * Verifies if the roman numeral is of value 0 (zero) without security checks.
 *
 * Ignores any leading minus. It's case INsensitive.
 *
 * @param *roman string containing the roman numeral.
 * @returns short as boolean: true if the numeral is NUMERUS_ZERO, false
 * otherwise.
 */
short _num_is_zero(char *roman) {
    if (*roman == '-') {
        roman++;
    }
    if (strcasecmp(roman, NUMERUS_ZERO) != 0) {
        return false;
    } else {
        return true;
    };
}


/**
 * Performs a security check and headtrimming of the roman numeral.
 *
 * Modifies the passed values themselves. Verifies if the *roman numeral is
 * NULL, if the *errcode is NULL, skips any heading whitespace of the roman
 * numeral, verifies if the roman numeral is empty. Otherwise stores NUMERUS_OK
 * in the *errcode.
 *
 * It is used as a preliminary securit check by other functions
 *
 * @param **roman string containing the roman numeral.
 * @param **errcode int where to store the analysis status: NUMERUS_OK or any
 * other error.
 * @returns void as the result is stored in **errcode.
 */
void _num_headtrim_check_numeral(char **roman, int **errcode) {
    if (*errcode == NULL) {
        *errcode = &numerus_error_code;
    }
    if (*roman == NULL) {
        numerus_error_code = NUMERUS_ERROR_NULL_ROMAN;
        **errcode = NUMERUS_ERROR_NULL_ROMAN;
        return;
    }
    while (isspace(**roman)) {
        (*roman)++;
    }
    if (**roman == '\0') {
        numerus_error_code = NUMERUS_ERROR_EMPTY_ROMAN;
        **errcode = NUMERUS_ERROR_EMPTY_ROMAN;
        return;
    }
    numerus_error_code = NUMERUS_OK;
    **errcode = NUMERUS_OK;
}


/**
 * Verifies if the roman numeral is of value 0 (zero).
 *
 * Ignores any leading minus. It's case INsensitive.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occured during the
 * analysis and the returned value is false. The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns short as boolean: true if the numeral is NUMERUS_ZERO, false
 * otherwise.
 */
short numerus_is_zero(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
    if (*errcode != NUMERUS_OK) {
        return false;
    }
    return _num_is_zero(roman);
}


/**
 * Verifies if the passed roman numeral is a long roman numeral (if contains a
 * correct number of underscores).
 *
 * Does **not** perform a syntax check or a value check, just searches for
 * underscores. Zero underscores means it's not a long roman numeral, two means
 * it is. Other numbers of underscores result in an error different than
 * NUMERUS_OK.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occured during the
 * analysis and the returned value is false. The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns short as boolean: true if the numeral is long, false
 * otherwise.
 */
short numerus_is_long_numeral(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
    if (*errcode != NUMERUS_OK) {
        return false;
    }
    short i = 0;
    short underscores_found = 0;
    while (*roman != '\0') {
        if (i > NUMERUS_MAX_LENGTH) {
            numerus_error_code = NUMERUS_ERROR_TOO_LONG_NUMERAL;
            *errcode = NUMERUS_ERROR_TOO_LONG_NUMERAL;
            return false;
        }
        if (*roman == '_') {
            underscores_found++;
        }
        i++;
        roman++;
    }
    if (underscores_found == 2) {
        numerus_error_code = NUMERUS_OK;
        *errcode = NUMERUS_OK;
        return true;
    } else if (underscores_found == 0){
        numerus_error_code = NUMERUS_OK;
        *errcode = NUMERUS_OK;
        return false;
    } else if (underscores_found == 1){
        numerus_error_code = NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE;
        *errcode = NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE;
        return false;
    } else {
        numerus_error_code = NUMERUS_ERROR_UNDERSCORE_AFTER_LONG_PART;
        *errcode = NUMERUS_ERROR_UNDERSCORE_AFTER_LONG_PART;
        return false;
    }
}


// Checks for an S or . and returns the index of the first found, else -1
/**
 * Verifies if the passed roman numeral is a float roman numeral (if contains
 * decimal characters 'S' and dot '.').
 *
 * Does **not** perform a syntax check or a value check, just searches for 'S'
 * and dot '.'. It's case INsensitive.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occured during the
 * analysis and the returned value is false. The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns short as boolean: true if the numeral contains 'S' or dot '.', false
 * otherwise.
 */
short numerus_is_float_numeral(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
    if (*errcode != NUMERUS_OK) {
        return false;
    }
    short i = 0;
    while (*roman != '\0') {
        if (i > NUMERUS_MAX_LENGTH) {
            numerus_error_code = NUMERUS_ERROR_TOO_LONG_NUMERAL;
            *errcode = NUMERUS_ERROR_TOO_LONG_NUMERAL;
            return false;
        }
        if (*roman == 'S' || *roman == 's' || *roman == '.') {
            numerus_error_code = NUMERUS_OK;
            *errcode = NUMERUS_OK;
            return i;
        } else {
            i++;
        }
        roman++;
    }
    numerus_error_code = NUMERUS_OK;
    *errcode = NUMERUS_OK;
    return false;
}


/**
 * Returns the sign of the roman numeral.
 *
 * Does **not** perform a syntax check or a value check, just searches for
 * the sign. If the numeral has value zero, returns 0; if the numeral has
 * a leading minus '-', returns -1; otherwise +1.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occured during the
 * analysis and the returned value is 0 (zero). The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns short with the sign of the roman numeral: 0 if has value zero, -1
 * if negative, +1 if positive.
 */
short numerus_sign(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
    if (*errcode != NUMERUS_OK) {
        return 0;
    }
    if (_num_is_zero(roman)) {
        return 0;
    }
    if (*roman == '-') {
        return -1;
    } else {
        return 1;
    }
}


/**
 * Returns the number of roman characters in the roman numeral.
 *
 * Does **not** perform a syntax check or a value check, but **does** check for
 * illegal characters. The length value does not count the underscores or
 * whitespace.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the the
 * error code is different than NUMERUS_OK, an error occured during the
 * analysis and the returned value is negative. The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns short with the number of roman characters excluding underscores.
 */
    _num_headtrim_check_numeral(&roman, &errcode);
short numerus_count_roman_chars(char *roman, int *errcode) {
    if (*errcode != NUMERUS_OK) {
        numerus_error_code = *errcode;
        return -1;
    }
    if (_num_is_zero(roman)) {
        numerus_error_code = NUMERUS_OK;
        *errcode = NUMERUS_OK;
        return (short) strlen(NUMERUS_ZERO);
    }
    short i = 0;
    while (*roman != '\0') {
        if (i > NUMERUS_MAX_LENGTH) {
            numerus_error_code = NUMERUS_ERROR_TOO_LONG_NUMERAL;
            *errcode = NUMERUS_ERROR_TOO_LONG_NUMERAL;
            return -2;
        }
        switch (toupper(*roman)) {
            case '_': {
                roman++; // ignore underscores
                break;
            }
            case '-':
            case 'M':
            case 'D':
            case 'C':
            case 'L':
            case 'X':
            case 'V':
            case 'I':
            case 'S':
            case '.': {
                roman++;
                i++; // count every other roman char
                break;
            }
            default: {
                if (isspace(*roman)) {
                    numerus_error_code = NUMERUS_ERROR_WHITESPACE_CHARACTER;
                    *errcode = NUMERUS_ERROR_WHITESPACE_CHARACTER;
                    return -3;
                } else {
                    numerus_error_code = NUMERUS_ERROR_ILLEGAL_CHARACTER;
                    *errcode = NUMERUS_ERROR_ILLEGAL_CHARACTER;
                    return -4;
                }
            }
        }
    }
    numerus_error_code = NUMERUS_OK;
    *errcode = NUMERUS_OK;
    return i;
}


/**
 * Compares the value of two roman numerals, emulating the operator '>'.
 *
 * @param *roman_bigger string with a roman numeral to compare with the second
 * parameter
 * @param *roman_smaller string with a roman numeral to compare with the first
 * parameter
 * @returns 1 if the first parameter is bigger, 0 if they are equal, -1 if the
 * second is bigger. Returns NUMERUS_ERROR_CANNOT_COMPARE if at least one of the
 * two numerals has wrong syntax and cannot be compared.
 */
short numerus_compare_value(char *roman_bigger, char *roman_smaller, int *errcode) {
    if (errcode == NULL) {
        errcode = &numerus_error_code;
    }
    short twelfths_bigger;
    long int_part_bigger = numerus_roman_to_int_part_and_twelfths(roman_bigger,
                                                                  &twelfths_bigger,
                                                                  errcode);
    if (*errcode != NUMERUS_OK) {
        numerus_error_code = *errcode;
        return 0;
    }
    short twelfths_smaller;
    long int_part_smaller = numerus_roman_to_int_part_and_twelfths(
            roman_smaller, &twelfths_smaller, errcode);
    if (*errcode != NUMERUS_OK) {
        numerus_error_code = *errcode;
        return 0;
    }
    if (int_part_bigger > int_part_smaller) {
        return 1;
    } else if (int_part_bigger < int_part_smaller) {
        return -1;
    } else { // equal int parts
        if (twelfths_bigger > twelfths_smaller) {
            return 1;
        } else if (twelfths_bigger < twelfths_smaller) {
            return -1;
        } else { // equal frac parts
            return 0;
        }
    }
}


static size_t _num_pretty_print_malloc_size(char *roman) {
    size_t alloc_size = 0;
    while (*roman != '\0' && !isspace(*roman)) {
        if (*roman != '_') {
            alloc_size++;
        }
        if (alloc_size > NUMERUS_MAX_LENGTH * 2) {
            return 0;
        }
        roman++;
    }
    alloc_size++; /* For '\n' at end of first line with underscores "____" */
    return alloc_size;
}

/* TODO: USE THE LENGTH MEASURING FUNCTION TO FIND ANY ILLEGAL CHARACTERS? */

char *numerus_pretty_print_long_numerals(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
    if (*errcode != NUMERUS_OK) {
        return false;
    }
    if (numerus_is_long_numeral(roman, errcode)) {
        char *roman_start = roman;
        char *pretty_roman = malloc(_num_pretty_print_malloc_size(roman));
        if (pretty_roman == NULL) {
            numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
            *errcode = NUMERUS_ERROR_MALLOC_FAIL;
            return NULL;
        }
        char *pretty_roman_start = pretty_roman;

        /* Skip minus sign */
        if (*roman == '-') {
            *(pretty_roman++) = ' ';
            roman++;
        }

        /* Write the overline */
        roman++; /* Skip first underscore */
        while (*roman != '_') {
            *(pretty_roman++) = '_';
            roman++;
        }
        roman++; /* Skip second underscore */
        *(pretty_roman++) = '\n';

        /* Copy the numeral in the second line */
        roman = roman_start;
        while (*roman != '\0') {
            if (*roman == '_') { // TODO remove this if, should still work
                roman++;
            } else {
                *(pretty_roman++) = *roman;
                roman++;
            }
        }
        *pretty_roman = '\0';
        return pretty_roman_start;
    } else {
        return roman;
    }
}

char *numerus_shorten_fraction(short twelfth) {
    if (twelfth < 0) {
        switch (twelfth) {
            case -11:
                return "-11/12";
            case -10:
                return "-5/6";
            case -9:
                return "-3/4";
            case -8:
                return "-2/3";
            case -7:
                return "-7/12";
            case -6:
                return "-1/2";
            case -5:
                return "-5/12";
            case -4:
                return "-1/3";
            case -3:
                return "-1/4";
            case -2:
                return "-1/6";
            case -1:
                return "-1/12";
            default:
                return NULL;
        }
    } else {
        switch (twelfth) {
            case 0:
                return "0/12";
            case 1:
                return "1/12";
            case 2:
                return "1/6";
            case 3:
                return "1/4";
            case 4:
                return "1/3";
            case 5:
                return "5/12";
            case 6:
                return "1/2";
            case 7:
                return "7/12";
            case 8:
                return "2/3";
            case 9:
                return "3/4";
            case 10:
                return "5/6";
            case 11:
                return "11/12";
            default:
                return NULL;
        }
    }
}

char *numerus_pretty_print_float_value(double double_value, int shorten) {
    short twelfths;
    long int_part = numerus_double_to_parts(double_value, &twelfths);
    char *pretty_value = malloc(17);
    if (pretty_value == NULL) {
        numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
        return NULL;
    }
    if (twelfths == 0) {
        snprintf(pretty_value, 17, "%ld", int_part);
    } else if (shorten) {
        snprintf(pretty_value, 17, "%ld, %s", int_part,
                 numerus_shorten_fraction(twelfths));
    } else {
        snprintf(pretty_value, 17, "%ld, %d/12", int_part, twelfths);
    }
    return pretty_value;
}

struct _num_error_codes {
    const int code;
    const char *message;
};

struct _num_error_codes _NUM_ERROR_CODES[] = {
    {NUMERUS_ERROR_VALUE_OUT_OF_RANGE,
            "The value to be converted to roman is out of conversion range."},
    {NUMERUS_ERROR_ILLEGAL_CHARACTER,
            "The roman numeral contains a character that is not part of the syntax of roman numerals."},
    {NUMERUS_ERROR_TOO_LONG_NUMERAL,
            "The roman numeral is too long to be syntactically correct."},
    {NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS,
            "The roman numeral contains too many consecutive repetitions of a repeatable character."},
    {NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE,
            "The roman numeral contains mispositioned characters."},
    {NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE,
            "The roman numeral contains one underscore but not the second one."},
    {NUMERUS_ERROR_UNDERSCORE_AFTER_LONG_PART,
            "The long roman numeral contains one underscore after the second one."},
    {NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG,
            "The non-long roman numeral contains one underscore."},
    {NUMERUS_ERROR_DECIMALS_IN_LONG_PART,
            "The long roman numeral contains decimal characters \"Ss.\" in the long part."},
    {NUMERUS_ERROR_ILLEGAL_MINUS,
            "The roman numeral contains a misplaced minus '-' or more than one."},
    {NUMERUS_ERROR_M_IN_SHORT_PART,
            "The long roman numeral contains an 'M' character after the long part."},
    {NUMERUS_ERROR_MALLOC_FAIL,
            "Heap memory allocation failure."},
    {NUMERUS_ERROR_NULL_ROMAN,
            "The pointer to the roman numeral string is NULL."},
    {NUMERUS_ERROR_EMPTY_ROMAN,
            "The roman numeral string is empty or filled with whitespace."},
    {NUMERUS_ERROR_WHITESPACE_CHARACTER,
            "The roman numeral string contains whitespace characters, even at the end."},
    {NUMERUS_OK,
            "Everything went all right."},
    {NUMERUS_ERROR_GENERIC,
            "An unknown or unspecified error happened."}
};

const char *numerus_explain_error(int error_code) {
    const struct _num_error_codes *current_code = &_NUM_ERROR_CODES[0];
    while (current_code->code != 0) {
        if (error_code == current_code->code) {
            return current_code->message;
        } else {
            current_code++;
        }
    }
    return "An unknown or unspecified error happened.";
}


double numerus_parts_to_double(long int_part, short twelfths) {
    return (double) (int_part) + twelfths / 12.0;
}

long numerus_double_to_parts(double value, short *twelfths) {
    short zero_twelfths = 0;
    if (twelfths == NULL) {
        twelfths = &zero_twelfths;
    }
    long int_part = (long) floor(value);
    value -= int_part; /* Get only decimal part */
    value = round(value * 12) / 12; /* Round to nearest twelfth */
    value = round(value * 12); /* Get numerator of that twelfth */
    *twelfths = (short) value;
    return int_part;
}