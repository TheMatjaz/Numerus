/**
 * @file numerus.c
 * @brief Numerus constants and functions for roman numerals conversion and 
 * manipulation.
 * @copyright Copyright © 2015, Matjaž Guštin <dev@matjaz.it> 
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under 
 * the BSD 3-clause license.
 *
 * Inspired by:
 * 
 * - http://stackoverflow.com/a/26723344/5292928
 * - http://stackoverflow.com/a/30816418/5292928
 */

#include <ctype.h>    /* For `upcase()` */
#include <stdio.h>    /* To `fprintf()` to `stderr` */
#include <stdlib.h>   /* For `malloc()` */
#include <string.h>   /* For `strcmp()` */
#include <regex.h>    /* To use regexes to match correct roman numeral syntax */
#include <sqlite3.h>  /* To export all roman numerals to an SQLite3 file */
#include <stdbool.h>  /* To use booleans `true` and `false` */
#include "numerus.h"


/**
 * Maximum value a long roman numeral (with '_') may have.
 */
const long int NUMERUS_MAX_LONG_VALUE = 3999999;
const double NUMERUS_MAX_DOUBLE_VALUE = NUMERUS_MAX_LONG_VALUE + 11/12;

/**
 * Minimum value a long a roman numeral (with '_') may have.
 */
const long int NUMERUS_MIN_LONG_VALUE = -NUMERUS_MAX_LONG_VALUE;
const double NUMERUS_MIN_DOUBLE_VALUE = -NUMERUS_MAX_DOUBLE_VALUE;

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
 * Maximum length of a double roman numeral string including the null terminator.
 *
 * The roman numeral `"-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS....."`
 * (value: -3888888 - 11+12) + `\0` is a string long 36+1 = 37 chars.
 */
const short int NUMERUS_MAX_DOUBLE_LENGTH = 37;

/**
 * Maximum length of a long roman numeral string including the null terminator.
 *
 * The roman numeral `"-_MMMDCCCLXXXVIII_DCCCLXXXVIII"` (value: -3888888) + `\0`
 * is a string long 30+1 = 31 chars.
 */
const short int NUMERUS_MAX_LONG_LENGTH = 31;

/**
 * Maximum length of a short roman numeral string including the null terminator.
 *
 * The roman numeral `"-MMMDCCCLXXXVIII"` (value: -3888) + `\0` is a string
 * long 16+1 = 17 chars.
 */
const short int NUMERUS_MAX_SHORT_LENGTH = 17;

/**
 * String containing a to-be-compiled regex matching any syntactically correct
 * roman numeral, including long roman numerals.
 *
 * The underscores are a notation used by Numerus to indicate so called "long roman numerals": the numbers
 * between them should be overlined (the line is called "vinculum") in other notations with graphical
 * capabilities, such as handwriting. The overlined characters have their value multiplied by 1000.
 */
const char *NUMERUS_LONG_SYNTAX_REGEX_STRING =
        "^-?((_M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})_)"
                "|M{0,3})(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$";

const char *NUMERUS_DOUBLE_SYNTAX_REGEX_STRING =
        "^-?((_M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})_)"
                "|M{0,3})(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})"
                "S?\\.{0,5}$";
/**
 * String containing a to-be-compiled regex matching only short syntactically correct
 * roman numerals.
 *
 * The so called "short roman numerals" don't have underscores and use normal syntax.
 */
const char *NUMERUS_SHORT_SYNTAX_REGEX_STRING =
        "^-?M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$";


static regex_t NUMERUS_DOUBLE_SYNTAX_REGEX;

/**
 * Compiled regex matching any syntactically correct roman numeral, including long numerals.
 *
 * Obtained by compiling NUMERUS_LONG_SYNTAX_REGEX_STRING.
 */
static regex_t NUMERUS_LONG_SYNTAX_REGEX;

/**
 * Compiled regex matching any syntactically correct short roman numeral.
 *
 * Obtained by compiling NUMERUS_SHORT_SYNTAX_REGEX_STRING.
 */
static regex_t NUMERUS_SHORT_SYNTAX_REGEX;

/**
 * Buffer where the strings with roman numerals are build an then copied from.
 *
 * This buffer is as long as the longest roman numeral. The usage of this buffer
 * allows one conversion at the time but is more memory efficient since the
 * roman numerals have variable length and can be returned as a string copied
 * from the buffer with just the right amount of space allocated.
 */
static char _num_numeral_build_buffer[NUMERUS_MAX_LONG_LENGTH];

/**
 * Global error code variable to store any errors during conversions.
 *
 * It may contain any of the NUMERUS_ERROR_* error codes or NUMERUS_OK.
 */
short int numerus_error_code = NUMERUS_OK;


/**
 * Struct containing a pair: basic roman char and its short integer value.
 *
 * It's used to create the _NUM_DICTIONARY dictionary and used by conversion
 * functions. The "roman chars" as called in this library are strings of 1 or 2
 * chars that have a specific a priori known value.
 */
struct _num_char_struct {
    const short int value;
    const char chars[3]; /* 1-2 chars + \0 = length 3 */
};

/**
 * Dictionary of basic roman chars and their values used by conversion
 * functions.
 */
static const struct _num_char_struct _NUM_DICTIONARY[] = {
    {1000, "M" },
    { 900, "CM"},
    { 500, "D" },
    { 400, "CD"},
    { 100, "C" },
    {  90, "XC"},
    {  50, "L" },
    {  40, "XL"},
    {  10, "X" },
    {   9, "IX"},
    {   5, "V" },
    {   4, "IV"},
    {   1, "I" }
};


/**
 * Verifies if the passed roman numeral is (-)NUMERUS_ZERO, case insensitive.
 *
 * @param *roman string containing a roman numeral to check if it is
 * NUMERUS_ZERO
 * @returns int 1 if the string is (-)NUMERUS_ZERO or 0 if it's not
 */
int numerus_is_zero(char *roman) {
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
 * Verifies if the passed roman numeral is a long numeral, outside
 * [NUMERUS_MIN_SHORT_VALUE, NUMERUS_MAX_SHORT_VALUE].
 *
 * Does **not** perform a syntax check. Any string starting with "-_" or "-"
 * would return a true result.
 *
 * @param *roman string containing a roman numeral to check if it is a long
 * roman numeral
 * @returns int 1 if the string is a long roman numeral or 0 if it's not
 */
int numerus_is_long_numeral(char *roman) {
    if (*roman == '_' || (*roman == '-' && *(roman+1) == '_')) {
        return true;
    } else {
        return false;
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
short numerus_numeral_length(char *roman) {
    if (numerus_is_zero(roman)) {
        return (short) strlen(NUMERUS_ZERO);
    }
    short i = 0;
    while (*roman != '\0') {
        if (i > NUMERUS_MAX_LONG_LENGTH) {
            return -1;
        }
        switch (toupper(*roman)) {
            case '_': {
                roman++;
                i++;
                break;
            }
            case '-':
            case 'M':
            case 'D':
            case 'C':
            case 'L':
            case 'X':
            case 'V':
            case 'I': {
                roman++;
                i++;
                break;
            }
            default: {
                return -2;
            }
        }
    }
    return i;
}

/**
 * Copies a string of 1 or 2 characters.
 *
 * Copies the character from the source to the destination. If there is another
 * character after that, that is not the null terminator, copies that as well.
 * Everything is performed without security checks for faster performance. This
 * function is used by numerus_short_to_roman() and it's meant to be used just on the
 * _num_char_struct in the dictionary _NUM_DICTIONARY.
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
 * Converts just the internal part of the underscores or the part after them. Stores it into *roman_string. Returns position after the inserted string.
 */
static char *_num_short_to_roman(long int arabic, char *roman_string) {
    const struct _num_char_struct *current_roman_char = &_NUM_DICTIONARY[0];
    while (arabic > 0) {
        while (arabic >= current_roman_char->value) {
            roman_string = _num_copy_char_from_dictionary(
                    current_roman_char->chars, roman_string);
            arabic -= current_roman_char->value;
        }
        current_roman_char++;
    }
    return roman_string;
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
        arabic -= (arabic / 1000) * 1000; /* Keep just the 3 right-most digits because of the integer division */
        *(roman_string++) = '_';
    }
    /* Create part after underscores */
    roman_string = _num_short_to_roman(arabic, roman_string);
    *roman_string = '\0';

    /* Copy out of the buffer and return it */
    char *returnable_roman_string =
            malloc(roman_string - _num_numeral_build_buffer);
    strcpy(returnable_roman_string, _num_numeral_build_buffer);
    return returnable_roman_string;
}

/**
 * Verifies if the passed string is a correct roman numeral.
 *
 * Performs syntax check of the passed roman numeral by checking it against a
 * regex compiled from NUMERUS_LONG_SYNTAX_REGEX_STRING. It is case insensitive. The
 * compilation is once for all subsequent calls of the function during
 * runtime. The regex compilation status is dropped since
 * NUMERUS_LONG_SYNTAX_REGEX_STRING is a correct hard coded constant.
 *
 * @param *roman string containing a roman numeral to check
 * @param is_short_numeral set it to non-zero to check the numeral only if it's a short numeral.
 * @returns int 1 if has correct roman syntax, 0 if it does not and  in case
 * of regex errors.
 */
int numerus_is_roman(char *roman, int is_short_numeral) {
    regex_t *regex_to_use;
    const char *regex_string_to_use;
    if (is_short_numeral != 0) {
        regex_to_use = &NUMERUS_SHORT_SYNTAX_REGEX;
        regex_string_to_use = NUMERUS_SHORT_SYNTAX_REGEX_STRING;
    } else {
        regex_to_use = &NUMERUS_LONG_SYNTAX_REGEX;
        regex_string_to_use = NUMERUS_LONG_SYNTAX_REGEX_STRING;
    }
    /* Compile the regex if it has not been done yet */
    if (regex_to_use->re_magic == 0) {
        /**
         * Flags in regcomp():
         * - REG_NOSUB:    does not save subexpressions (groups), only
         *                 reports the success or failure of compiling the regex
         * - REG_ICASE:    ignores the case, making the regex case insensitive
         * - REG_EXTENDED: uses the extended POSIX standard regular expressions,
         *                 which are required for the regex structure
         */
        regcomp(regex_to_use, regex_string_to_use,
                REG_NOSUB | REG_ICASE | REG_EXTENDED);
    }
    int match_result = regexec(regex_to_use, roman, 0, NULL, 0);
    if (match_result == 0) { /* Matches regex */
        return true;
    } else if (match_result == REG_NOMATCH) { /* Does not match regex */
        return false;
    } else { /* Other errors */
        char msgbuf[100];
        regerror(match_result, regex_to_use, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Roman syntax regex matching internal error.");
        return NUMERUS_ERROR_REGEXEC;
    }
}

/**
 * Checks if two strings match in the the next 1 or 2 characters.
 *
 * It is case insensitive. This functions is used by numerus_roman_to_short() and is
 * missing many security check since is internal and is meant to be called on
 * _NUM_DICTIONARY.
 *
 * @param *to_compare the current position in the string to check against the 
 * pattern
 * @param *pattern the position in the string containing the 1 or 2 characters 
 * that may be in *to_compare
 * @returns length of the match as short: 0 if they don't match or 1 or 2
 * if they match.
 */
static short int _num_begins_with(char *to_compare, const char *pattern) {
    size_t pattern_length = strlen(pattern);
    if (strncasecmp(to_compare, pattern, pattern_length) == 0) {
        /* Compare the first pattern_length characters */
        return (short) pattern_length;
    } else {
        return 0;
    }
}

static long _num_roman_to_short(char **roman) {
    long arabic = 0;
    const struct _num_char_struct *current_roman_char = &_NUM_DICTIONARY[0];
    while (**roman != '_' && **roman != '\0') {
        short matching_chars = _num_begins_with(*roman,
                                                current_roman_char->chars);
        if (matching_chars > 0) {
            *roman += matching_chars;
            arabic += current_roman_char->value;
        } else {
            current_roman_char++;
        }
    }
    return arabic;
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
long numerus_roman_to_long(char *roman) {
    /* Exclude nulla numerals */
    if (numerus_is_zero(roman)) {
        return 0;
    }

    /* Return an error if the roman is not syntactically correct */
    if (!numerus_is_roman(roman, 0)) {
        numerus_error_code = NUMERUS_ERROR_WRONG_SYNTAX;
        fprintf(stderr, "Roman conversion error: wrong syntax of numeral %s\n", roman);
        return NUMERUS_MAX_LONG_VALUE + 1;
    }

    /* Save sign */
    short int sign = 1;
    if (*roman == '-') {
        sign = -1;
        roman++;
    }

    /* Actual conversion */
    long arabic = 0;
    if (*roman == '_') {
        roman++; /* Skip '_' */
        arabic += _num_roman_to_short(&roman) * 1000;
        roman++; /* Skip '_' */
    }
    arabic += _num_roman_to_short(&roman);

    numerus_error_code = NUMERUS_OK;
    return sign * arabic;
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
int numerus_compare_value(char *roman_bigger, char *roman_smaller) {
    long value_bigger = numerus_roman_to_long(roman_bigger);
    long value_smaller = numerus_roman_to_long(roman_smaller);
    if (value_bigger > NUMERUS_MAX_LONG_VALUE || value_smaller >
                                                 NUMERUS_MAX_LONG_VALUE) {
        fprintf(stderr, "Roman comparition error: "
                        "cannot compare syntactically wrong numerals\n");
        numerus_error_code = NUMERUS_ERROR_CANNOT_COMPARE;
        return NUMERUS_ERROR_CANNOT_COMPARE;
    }
    if (value_bigger > value_smaller) {
        return 1;
    } else if (value_bigger == value_smaller) {
        return 0;
    } else {
        return -1;
    }
}

int numerus_export_to_csv(char *filename, long min_value, long max_value,
                          int numerals_first, char *separator, char *newline,
                          char *quotes) {
    if (filename == NULL) {
        filename = "/tmp/numerus.csv";
    }
    if (min_value < NUMERUS_MIN_LONG_VALUE || max_value > NUMERUS_MAX_LONG_VALUE) {
        return -1;
    }
    if (separator == NULL) {
        separator = ",";
    }
    if (newline == NULL) {
        newline = "\n";
    }
    if (quotes == NULL) {
        quotes = "\0";
    }

    FILE *csv_file = fopen(filename, "w");
    long int i;
    if (numerals_first == 0) {
        for (i = min_value; i <= max_value; i++) {
            fprintf(csv_file,
                    "%li%s%s%s%s%s",
                    i,
                    separator,
                    quotes,
                    numerus_long_to_roman(i),
                    quotes,
                    newline);
        }
    } else {
        for (i = min_value; i <= max_value; i++) {
            fprintf(csv_file,
                    "%s%s%s%s%li%s",
                    quotes,
                    numerus_long_to_roman(i),
                    quotes,
                    separator,
                    i,
                    newline);
        }
    }
    fclose(csv_file);
    return 0;
}

/**
 * Saves all roman numerals with their values to a SQLite3 file in a table
 * called "roman_numerals".
 *
 * The filename should be formatted as URI, see some [examples from the SQLite3
 * docs](https://www.sqlite.org/c3ref/open.html#urifilenameexamples). Errors are
 * printed to stderr.
 *
 * @param filename string with the SQLite3 file name to store into formatted as 
 * URI. If NULL, the file is saved to a file in the current directory named 
 * "numerus.db"
 * @returns response code as int: NUMERUS_OK if everything went OK, 
 * NUMERUS_ERROR_SQLITE if something went wrong.
 */
int numerus_export_to_sqlite3(char *filename, long min_value, long max_value) {
    if (filename == NULL) {
        filename = "file:numerus.db";
    }
    sqlite3 *db;
    char *err_msg;

    /* Open the database in read-write mode, create it if not exists yet */
    int resp_code = sqlite3_open_v2(filename, &db,
                                    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                    NULL);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: cannot open database %s, %s\n", filename,
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }

    /* Database settings for insecure but faster insertions */
    resp_code = sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL,
                             &err_msg);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n",
                sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }
    resp_code = sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL,
                             &err_msg);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n",
                sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }

    /* Create table */
    char *query_table = "DROP TABLE IF EXISTS roman_numerals; "
                        "CREATE TABLE roman_numerals "
                        "(value BIGINT, numeral TEXT);";
    resp_code = sqlite3_exec(db, query_table, 0, 0, &err_msg);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }

    /* Prepare statement */
    char *query = "INSERT INTO roman_numerals VALUES (@i, @s);";
    sqlite3_stmt *stmt;
    resp_code = sqlite3_prepare_v2(db, query, 150, &stmt, NULL);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }

    /* Start transaction */
    resp_code = sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &err_msg);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }

    /* Insert into SQLite table */
    long int i;
    printf("Insering into SQLite...\n");
    for (i = min_value; i <= max_value; i++) {
        char *roman = numerus_long_to_roman(i);
        /* Fill prepared statement */
        sqlite3_bind_int64(stmt, 1, i);
        sqlite3_bind_text(stmt, 2, roman, -1, SQLITE_TRANSIENT);

        /* Execute statement */
        resp_code = sqlite3_step(stmt);
        if (resp_code != SQLITE_OK) {
            fprintf(stderr, "SQLite error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return NUMERUS_ERROR_SQLITE;
        }

        /* Cleanup memory and prepared statement */
        free(roman);
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
    }

    /* Commit transaction */
    resp_code = sqlite3_exec(db, "END TRANSACTION", 0, 0, &err_msg);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }

    /* Add index on the value column */
    resp_code = sqlite3_exec(db, "CREATE INDEX 'idx_roman_value' "
                                 "ON roman_numerals (value);", 0, 0, &err_msg);
    if (resp_code != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NUMERUS_ERROR_SQLITE;
    }

    /* Cleanup and return */
    sqlite3_finalize(stmt);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NUMERUS_OK;
}

int _num_pretty_print_malloc_size(char *roman) {
    int alloc_size = 0;
    while (*roman != '\0') {
        if (*roman != '_') {
            alloc_size++;
        }
        roman++;
    }
    alloc_size++; /* For \n at end of "____" line */
    return alloc_size;
}

char *numerus_pretty_print_long_numerals(char *roman) {
    if (roman == NULL) {
        return NULL;
    }
    if (numerus_is_long_numeral(roman)) {
        char *roman_start = roman;
        char *pretty_roman = malloc(_num_pretty_print_malloc_size(roman));
        char *pretty_roman_start = pretty_roman;
        if (*roman == '-') {
            *(pretty_roman++) = ' ';
            roman++;
        }
        roman++; /* Skip first underscore */
        while (*roman != '_') {
            *(pretty_roman++) = '_';
            roman++;
        }
        roman++; /* Skip second underscore */
        *(pretty_roman++) = '\n';
        roman = roman_start;
        while (*roman != '\0') {
            if (*roman == '_') {
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
