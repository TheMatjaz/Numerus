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

/**
 * Minimum value a long a roman numeral (with '_') may have.
 */
const long int NUMERUS_MIN_LONG_VALUE = -3999999;

/**
 * Maximum value a short roman numeral (without '_') may have.
 */
const short int NUMERUS_MAX_SHORT_VALUE = 3999;

/**
 * Minimum value a short roman numeral (without '_') may have.
 */
const short int NUMERUS_MIN_SHORT_VALUE = -3999;

/**
 * Roman numeral of value 0 (zero).
 */
const char *NUMERUS_ZERO = "NULLA";

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

/**
 * String containing a to-be-compiled regex matching only short syntactically correct
 * roman numerals.
 *
 * The so called "short roman numerals" don't have underscores and use normal syntax.
 */
const char *NUMERUS_SHORT_SYNTAX_REGEX_STRING =
        "^-?M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$";

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
int numerus_roman_is_zero(char *roman) {
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

/* Excluding the null terminator */
short numerus_numeral_length(char *roman) {
    short i = 0;
    while (*roman != '\0') {
        if (i > NUMERUS_MAX_LENGTH) {
            return -1;
        }
        if (*roman == '_') {
            roman++;
        } else {
            i++;
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
 * Converts a short int to a roman numeral.
 *
 * It allocates a string with the roman numerals long just as required and
 * returns a pointer to it.  If the short is outside of [NUMERUS_MIN_LONG_VALUE,
 * NUMERUS_MAX_LONG_VALUE], the conversion is impossible.
 *
 * @returns pointer to a string containing the roman numeral, NULL if the short 
 * is out of range.
 */
char *numerus_int_to_roman(long int arabic) {
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


    /* Actual conversion comparing appending chars from _NUM_DICTIONARY */
    const struct _num_char_struct *current_roman_char = &_NUM_DICTIONARY[0];

    /* Treat big numerals */
    if (arabic > 3999) {
        *(roman_string++) = '_';
        while (arabic > 999) {
            while (arabic / 1000 >= current_roman_char->value) {
                roman_string = _num_copy_char_from_dictionary(
                        current_roman_char->chars, roman_string);
                arabic -= current_roman_char->value * 1000;
            }
            current_roman_char++;
        }
        *(roman_string++) = '_';
    }
    current_roman_char = &_NUM_DICTIONARY[0];
    while (arabic > 0) {
        while (arabic >= current_roman_char->value) {
            roman_string = _num_copy_char_from_dictionary(
                    current_roman_char->chars, roman_string);
            arabic -= current_roman_char->value;
        }
        current_roman_char++;
    }
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

/**
 * Converts a roman numeral to a short int.
 *
 * It is case insensitive and accepts negative roman numerals. If the numeral
 * cannot be converted, it means it has wrong syntax. In that case a value
 * bigger than NUMERUS_MAX_VALUE is returned and the error code
 * NUMERUS_ERROR_WRONG_SYNTAX is stored in numerus_error_code.
 *
 * @param *roman string with a roman numeral
 * @returns short value of the roman numeral or a value bigger than 
 * NUMERUS_MAX_VALUE in case of error
 */
short int numerus_roman_to_short(char *roman) {
    /* Exclude nulla numerals */
    if (numerus_roman_is_zero(roman)) {
        return 0;
    }

    /* Return an error if the roman is not syntactically correct */
    if (!numerus_is_roman(roman)) {
        numerus_error_code = NUMERUS_ERROR_WRONG_SYNTAX;
        fprintf(stderr, "Roman conversion error: wrong syntax of numeral %s\n", roman);
        return NUMERUS_MAX_VALUE + 1;
    }

    /* Save sign */
    short int sign = 1;
    if (*roman == '-') {
        sign = -1;
        roman++;
    }

    /* Actual conversion */
    short int arabic = 0;
    const struct _num_char_struct *current_roman_char = &_NUM_DICTIONARY[0];
    while (*roman != '\0') {
        short matching_chars = _num_begins_with(roman,
                                                current_roman_char->chars);
        if (matching_chars > 0) {
            roman += matching_chars;
            arabic += current_roman_char->value;
        } else {
            current_roman_char++;
        }
    }

    numerus_error_code = 0;
    return sign * arabic;
}

/**
 * Allocates all roman numerals and their values in memory for fast conversions
 * from value to roman numeral.
 *
 * It creates an array of pointers to strings, char*. Each char* points to the
 * roman numeral with the same value as the index of the char* in the array. The
 * returned pointer to the array points to the value 0.
 *
 * Example structure:
 *
 * \verbatim
 *  i  | p --> numeral
 * ----|---------------------
 * -1  | * --> "-I"
 *  0  | * --> "NULLA"
 *  1  | * --> "I"
 * \endverbatim
 *
 * Example usage:
 *
 * \code{.c}
 * char **all_romans = numerus_allocate_all_romans(1);
 * char *fortytwo = all_romans[42];
 * \endcode
 *
 * @param include_negatives set it to 0 to create the array from 0 to 
 * NUMERUS_MAX_VALUE or to anything else to create it from NUMERUS_MIN_VALUE to
 * NUMERUS_MAX_VALUE
 * @returns the address of the value 0 (which points to NUMERUS_ZERO) in the
 * array or NULL if malloc() fails to allocate the array
 */
char **numerus_allocate_all_romans(short int include_negatives) {
    short int num_of_romans = NUMERUS_MAX_VALUE;
    if (include_negatives != 0) {
        include_negatives = 1;
        num_of_romans *= 2;
    }
    num_of_romans += 1; /* Include NUMERUS_ZERO */

    /* Allocate an array of pointers to strings */
    char **all_roman_numerals = malloc(num_of_romans * sizeof(char *));
    if (all_roman_numerals == NULL) {
        numerus_error_code = NUMERUS_ERROR_ALLOCATE_ALL;
        fprintf(stderr, "All romans allocation error in malloc()\n");
        return NULL;
    }

    /* Fill the array */
    int index = 0;
    short i;
    if (include_negatives) {
        for (i = NUMERUS_MIN_VALUE; i < 0; i++) {
            all_roman_numerals[index++] = numerus_int_to_roman(i);
        }
    }
    for (i = 0; i <= NUMERUS_MAX_VALUE; i++) {
        all_roman_numerals[index++] = numerus_int_to_roman(i);
    }
    return &all_roman_numerals[0 + (include_negatives ? NUMERUS_MAX_VALUE : 0)];
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
    short int value_bigger = numerus_roman_to_short(roman_bigger);
    short int value_smaller = numerus_roman_to_short(roman_smaller);
    if (value_bigger > NUMERUS_MAX_VALUE || value_smaller > NUMERUS_MAX_VALUE) {
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

int numerus_export_all_to_csv(char *filename) {
    if (filename == NULL) {
        filename = "/tmp/numerus.csv";
    }
    FILE *csv = fopen(filename, "w");
    long int i;
    for (i = NUMERUS_MIN_VALUE; i <= NUMERUS_MAX_VALUE; i++) {
            fprintf(csv, "%li, %s\n", i, numerus_int_to_roman(i));
        }
    fclose(csv);
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
int numerus_export_all_to_sqlite3(char *filename) {
    if (filename == NULL) {
        filename = "file:numerus.db";
    }
    sqlite3 *db_connection;
    char *sqlite_error_msg = 0;

    /* Open the database in read-write mode, create it if not exists yet */
    int sqlite3_resp_code = sqlite3_open_v2(filename, &db_connection,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                            NULL);
    if (sqlite3_resp_code != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(
                db_connection));
        sqlite3_close(db_connection);
        return NUMERUS_ERROR_SQLITE;
    }

    /* OPTIMIZE */
    sqlite3_exec(db_connection, "PRAGMA synchronous = OFF", NULL, NULL, &sqlite_error_msg);
    sqlite3_exec(db_connection, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sqlite_error_msg);

    /* Create table query */
    char *query_table = "DROP TABLE IF EXISTS roman_numerals; "
                  "CREATE TABLE roman_numerals ("
                      "value BIGINT,"
                      "numeral TEXT"
                  ");";
    sqlite3_resp_code = sqlite3_exec(db_connection, query_table, 0, 0,
                                     &sqlite_error_msg);
    if (sqlite3_resp_code != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite_error_msg);
        sqlite3_free(sqlite_error_msg);
        sqlite3_free(query_table);
        sqlite3_close(db_connection);
        return NUMERUS_ERROR_SQLITE;
    }
    //free(query_table);
    /* Insert all roman numerals */

    /* Prepare statement */
    //char *query = malloc(150 * sizeof(char));
    char *query = "INSERT INTO roman_numerals VALUES (@i, @s);";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db_connection, query, 150, &stmt, NULL);

    /* Start transaction */
    sqlite3_resp_code = sqlite3_exec(db_connection, "BEGIN TRANSACTION", 0, 0,
                                     &sqlite_error_msg);
    long int i;
    for (i = NUMERUS_MIN_VALUE; i <= NUMERUS_MAX_VALUE; i++) {
        if (i % 100000 == 0) {
            printf("Inserting to SQLite: %5.2f%%\n", 100*(i-NUMERUS_MIN_VALUE)/(NUMERUS_MAX_VALUE*2.0+1));
        }
        char *roman = numerus_int_to_roman(i);
        /* Fill statement */
        sqlite3_bind_int64(stmt, 1, i);
        sqlite3_bind_text(stmt, 2, roman, -1, SQLITE_TRANSIENT);

        /* Execute statement */
        sqlite3_step(stmt);

        /* Cleanup memory */
        free(roman);
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_resp_code = sqlite3_exec(db_connection, "END TRANSACTION", 0, 0,
                                     &sqlite_error_msg);
    sqlite3_resp_code = sqlite3_exec(db_connection, "CREATE INDEX 'idx_roman_value' ON roman_numerals (value);", 0, 0,
                                     &sqlite_error_msg);

    /* Cleanup and return */
    sqlite3_finalize(stmt);
    sqlite3_free(sqlite_error_msg);
    sqlite3_close(db_connection);
    return NUMERUS_OK;
}
