/**
 * @file numerus_utils.c
 * @brief Numerus constants and utility functions for roman numerals manipulation.
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
#include "numerus_internal.h"



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
 * Verifies if the passed roman numeral is (-)NUMERUS_ZERO, case insensitive.
 *
 * @param *roman string containing a roman numeral to check if it is
 * NUMERUS_ZERO
 * @returns int 1 if the string is (-)NUMERUS_ZERO or 0 if it's not
 */
short numerus_is_zero(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
    if (*errcode != NUMERUS_OK) {
        return false;
    }
    return _num_is_zero(roman);
}

/**
 * Verifies if the passed roman numeral is a long numeral, outside
 * [NUMERUS_MIN_NONLONG_FLOAT_VALUE, NUMERUS_MAX_NONLONG_FLOAT_VALUE].
 *
 * Does **not** perform a syntax check. Any string starting with "-_" or "-"
 * would return a true result.
 *
 * @param *roman string containing a roman numeral to check if it is a long
 * roman numeral
 * @returns int 1 if the string is a long roman numeral or 0 if it's not
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

short numerus_sign(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
    if (*errcode != NUMERUS_OK) {
        return false;
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
 * Calculates the number of roman characters in the roman numeral.
 *
 * It's case insensitive. Includes the minus `-` and any of the following
 * characters `MDCLXVI`. Exludes underscroes `_` and the null terminator. It
 * does not perform a syntax check, but it stops at NUMERAL_MAX_LONG_LENGTH
 * characters. If the string is longer, returns -1. If any non-roman character
 * is found in the string, returns -2.
 *
 * @param *roman string containing a roman numeral to count the roman chars of
 * @returns short number of roman characters in a roman numeral, -1 if the
 * string is too long for a roman numeral, -2 if any non roman character is
 * found.
 */
short numerus_numeral_length(char *roman, int *errcode) {
    _num_headtrim_check_numeral(&roman, &errcode);
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
                numerus_error_code = NUMERUS_ERROR_ILLEGAL_CHARACTER;
                *errcode = NUMERUS_ERROR_ILLEGAL_CHARACTER;
                return -3;
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
    short frac_part_bigger;
    long int_part_bigger = numerus_roman_to_int_and_frac_part(roman_bigger, &frac_part_bigger, errcode);
    if (*errcode != NUMERUS_OK) {
        numerus_error_code = *errcode;
        return 0;
    }
    short frac_part_smaller;
    long int_part_smaller = numerus_roman_to_int_and_frac_part(roman_smaller, &frac_part_smaller, errcode);
    if (*errcode != NUMERUS_OK) {
        numerus_error_code = *errcode;
        return 0;
    }
    if (int_part_bigger > int_part_smaller) {
        return 1;
    } else if (int_part_bigger < int_part_smaller) {
        return -1;
    } else { // equal int parts
        if (frac_part_bigger > frac_part_smaller) {
            return 1;
        } else if (frac_part_bigger < frac_part_smaller) {
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
    short frac_part;
    long int_part = numerus_double_to_parts(double_value, &frac_part);
    char *pretty_value = malloc(17);
    if (pretty_value == NULL) {
        numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
        return NULL;
    }
    if (frac_part == 0) {
        snprintf(pretty_value, 17, "%ld", int_part);
    } else if (shorten) {
        snprintf(pretty_value, 17, "%ld, %s", int_part,
                 numerus_shorten_fraction(frac_part));
    } else {
        snprintf(pretty_value, 17, "%ld, %d/12", int_part, frac_part);
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


/* pass it values in [0, 1[ to round to the nearest twelfth. Returns the numerator from 0 to 11 */
static short _num_extract_twelfth(double value) {
    value = round(value * 12) / 12; /* Round to nearest twelfth */
    value = round(value * 12); /* Get numerator of that twelfth */
    return (short) ABS(value);
}


double numerus_parts_to_double(long int_part, short frac_part) {
    return (double) (int_part) + frac_part/12.0;
}

long numerus_double_to_parts(double value, short *frac_part) {
    short zero_frac_part = 0;
    if (frac_part == NULL) {
        frac_part = &zero_frac_part;
    }
    double double_int_part;
    double double_frac_part = modf(value, &double_int_part);
    *frac_part = _num_extract_twelfth(double_frac_part);
    return (long) double_int_part;;
}