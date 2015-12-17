/**
 * @file
 * @brief Numerus constants and functions for roman numerals conversion and manipulation.
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
#include "numerus.h"


/**
 * Maximum value as short a roman numeral may have.
 */
const short int ROMAN_MAX_VALUE = 3999;

/**
 * Minimum value as short a roman numeral may have.
 */
const short int ROMAN_MIN_VALUE = -3999;

/**
 * Roman numeral of value 0 (zero).
 */
const char *ROMAN_ZERO = "NULLA";

/**
 * Maximum length of a roman numeral string including the null terminator.
 *
 * The roman numeral `"-MMMDCCCLXXXVIII"` (value: -3888) + `\0` is a string long
 * 16+1 = 17 chars.
 */
const short int ROMAN_MAX_LENGTH = 17;

/**
 * String containing a to-be-compiled regex matching any syntactiacally correct
 * roman numeral.
 */
const char *ROMAN_SYNTAX_REGEX_STRING = "^-?M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$";

/**
 * Compiled regex matching any syntactically correct roman numeral.
 *
 * Obtained by compiling ROMAN_SYNTAX_REGEX_STRING.
 */
static regex_t ROMAN_SYNTAX_REGEX;

/**
 * Buffer where the strings with roman numerals are build an then copied from.
 *
 * This buffer is as long as the longest roman numeral. The usage of this
 * buffer allows one conversion at the time but is more memory efficient since
 * the roman numerals have variable length and can be returned as a string
 * copied from the buffer with just the right amount of space allocated.
 */
static char roman_numeral_build_buffer[ROMAN_MAX_LENGTH];

/**
 * Global error code variable to store any errors during conversions.
 *
 * It may contain any of the ROMAN_ERROR_* error codes or ROMAN_OK.
 */
unsigned short int roman_error_code = ROMAN_OK;


/**
 * Struct containing a pair: basic roman char and its short integer value.
 *
 * It's used to create the ROMAN_CHARS dictionary and used by conversion
 * functions. The "roman chars" as called in this library are strings of 1 or 2
 * chars that have a specific a priori known value.
 */
static struct roman_char_struct {
    const short int value;
    const char chars[3]; /* 1-2 chars + \0 = length 3 */
};

/**
 * Dictionary of basic roman chars and their values used by conversion
 * functions.
 */
static const struct roman_char_struct ROMAN_CHARS[] = {
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
 * Verifies if the passed roman numeral is (-)ROMAN_ZERO, case insensitive.
 *
 * @param *roman string containing a numeral to chech if it is ROMAN_ZERO.
 * @returns int 1 if the string is (-)ROMAN_ZERO or 0 if it's not.
 */
int roman_is_zero(char *roman) {
    if (*roman == '-') {
        roman++;
    }
    if (strcasecmp(roman, ROMAN_ZERO) != 0) {
        return 0;
    } else {
        return 1;
    };
}

/**
 * Copies a string of 1 or 2 characters.
 *
 * Copies the character from the source to the destination. If there is another
 * character after that, that is not the null terminator, copies that as well.
 * Everything is performed without security checks for faster performance. This
 * function is used by int_to_roman() and it's meant to be used just on the
 * roman_char_struct in the dictionary ROMAN_CHARS.
 *
 * @param *source the string of 1-2 characters to copy
 * @param *destination the string, already allocated, to copy the *source into
 * @returns the new position of the destination pointer after the characters have been copied
 */
static char *copy_roman_char_from_dictionary(char *source, char *destination) {
    *destination = *(source++);
    if (*source != '\0') {
        *(++destination) = *source;
    }
    return ++destination;
}

/**
 * Converts a short int to a roman numeral.
 *
 * It allocates a string with the roman numerals long just as required and returns a pointer to it.
 * If the short is outside of [ROMAN_MIN_VALUE, ROMAN_MAX_VALUE], the conversion is impossible.
 *
 * @returns pointer to a string containing the roman numeral, NULL if the short is out of range.
 */
char *int_to_roman(short int arabic) {
    /* Out of range check */
    if (arabic < ROMAN_MIN_VALUE || arabic > ROMAN_MAX_VALUE) {
        roman_error_code = ROMAN_ERROR_OUT_OF_RANGE;
        fprintf(stderr, "Roman conversion error: short int %d out of range [%d, %d]\n",
                arabic, ROMAN_MIN_VALUE, ROMAN_MAX_VALUE);
        return NULL;
    }

    /* Create pointer to the building buffer */
    char *roman_string = &roman_numeral_build_buffer[0];

    /* Save sign or return ROMAN_ZERO for 0 */
    if (arabic < 0) {
        arabic *= -1;
        *(roman_string++) = '-';
    } else if (arabic == 0) {
        return ROMAN_ZERO; /* TODO: Probably should return a copy of it? */
    }

    /* Actual conversion comparing appending chars from ROMAN_CHARS */
    struct roman_char_struct *current_roman_char = &ROMAN_CHARS[0];
    while (arabic > 0) {
        while (arabic >= current_roman_char->value) {
            roman_string = copy_roman_char_from_dictionary(
                    current_roman_char->chars, roman_string);
            arabic -= current_roman_char->value;
        }
        current_roman_char++;
    }
    *roman_string = '\0';

    /* Copy out of the buffer and return it */
    char *returnable_roman_string = malloc(roman_string -
                                           roman_numeral_build_buffer);
    strcpy(returnable_roman_string, roman_numeral_build_buffer);
    return returnable_roman_string;
}

/**
 * Verifies if the passed string is a correct roman numeral.
 *
 * Performs syntax check of the passed roman numeral by checking it against a
 * regex compiled from
 * ROMAN_SYNTAX_REGEX_STRING. It is case insensitive. The compilation is once
 * for all subsequent calls of the function during runtime. The regex
 * compilation status is dropped since ROMAN_SYNTAX_REGEX_STRING is a correct
 * hard coded constant.
 *
 * @param *roman string containing a roman numeral to check
 * @returns int 1 if has correct roman syntax, 0 if it does not and  in case
 * of regex errors.
 */
int is_roman(char *roman) {
    /* Compile the regex if it has not been done yet */
    if (ROMAN_SYNTAX_REGEX.re_magic == 0) {
        /**
         * Flags in regcomp():
         * - REG_NOSUB:    does not save subexpressions (groups), only
         *                 reports the success or failure of compiling the regex
         * - REG_ICASE:    ignores the case, making the regex case insensitive
         * - REG_EXTENDED: uses the extended POSIX standard regular expressions,
         *                 which are required for the regex structure
         */
        regcomp(&ROMAN_SYNTAX_REGEX, ROMAN_SYNTAX_REGEX_STRING,
                REG_NOSUB | REG_ICASE | REG_EXTENDED);
    }
    int match_result = regexec(&ROMAN_SYNTAX_REGEX, roman, 0, NULL, 0);
    if (match_result == 0) { /* Matches regex */
        return 1;
    } else if (match_result == REG_NOMATCH) { /* Does not match regex */
        return 0;
    } else { /* Other errors */
        char msgbuf[100];
        regerror(match_result, &ROMAN_SYNTAX_REGEX, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Roman syntax regex matching internal error.");
        return ROMAN_ERROR_REGEXEC;
    }
}

/**
 * Checks if two strings match in the the next 1 or 2 characters.
 *
 * This functions is used by roman_to_short() and is missing many security check since is internal and is meant to be called on ROMAN_CHARS.
 *
 * @param *to_compare the current position in the string to check against the pattern
 * @param *pattern the position in the string containing the 1 or 2 characters that may be in *to_compare
 * @returns length of the match as short: 0 if they don't match or 1 or 2
 * if they match.
 */
static short int begins_with(char *to_compare, char *pattern) {
    size_t pattern_length = strlen(pattern);
    if (strncmp(to_compare, pattern, pattern_length) == 0) {
        /* compare the first pattern_length characters */
        return (short) pattern_length;
    } else {
        return 0;
    }
}

/**
 * Converts a roman numeral to a short int.
 *
 * If the value cannot be converted, returns a value bigger than ROMAN_MAX_VALUE
 * and a non-zero error code in the second parameter.
 */
short int roman_to_short(char *roman, short int *error_code) {
    /* Exclude nulla numerals */
    if (roman_is_zero(roman)) {
        return 0;
    }

    /* Return an error if the roman is not syntactically correct */
    if (!is_roman(roman)) {
        *error_code = 1;
        return ROMAN_MAX_VALUE + 1;
    }

    /* Save sign */
    short int sign = 1;
    if (*roman == '-') {
        sign = -1;
        roman++;
    }

    /* Actual conversion */
    short int arabic = 0;
    struct roman_char_struct *current_roman_char = &ROMAN_CHARS[0];
    while (*roman != '\0') {
        short matching_chars = begins_with(roman, current_roman_char->chars);
        if (matching_chars > 0) {
            roman += matching_chars;
            arabic += current_roman_char->value;
        } else {
            current_roman_char++;
        }
    }

    *error_code = 0;
    return sign * arabic;
}

/**
 * Use indices as arabic integers. The char* stored at that index, points to the
 * roman numeral of that value.
 * Example usage:
 * char **all_romans = allocate_all_romans(1);
 * all_romans[702] will be int_to_roman(702) = "DCCII" */
char** allocate_all_romans(short int include_negatives) {
    short int num_of_romans = ROMAN_MAX_VALUE;
    if (include_negatives) {
        num_of_romans *= 2;
    }
    num_of_romans += 1; /* For nulla */
    char* all_roman_numerals[num_of_romans]; /* Array of pointers to strings */
    int index = 0;
    short i;
    if (include_negatives) {
        for (i = ROMAN_MIN_VALUE; i < 0; i++) {
            all_roman_numerals[index++] = int_to_roman(i);
        }
    }
    for (i = 0; i <= ROMAN_MAX_VALUE; i++) {
        all_roman_numerals[index++] = int_to_roman(i);
    }
    return &all_roman_numerals[0 + (include_negatives ? ROMAN_MAX_VALUE : 0)];
}
int roman_is_bigger(char *roman_bigger, char *roman_smaller) {
    short error_code;
    short int value_bigger = roman_to_short(roman_bigger, &error_code);
    short int value_smaller = roman_to_short(roman_smaller, &error_code);
    if (value_bigger > value_smaller) {
        return 1;
    } else if (value_bigger == value_smaller) {
        return 0;
    } else {
        return -1;
    }
}

int save_to_sqlite3() {
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open_v2("file:numerus.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); /* Open in RW mode and create if not exists */

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char *query = "DROP TABLE IF EXISTS roman_numerals; "
                  "CREATE TABLE roman_numerals (value INT, numeral TEXT);";
    rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {

        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_free(query);
        sqlite3_close(db);

        return 1;
    }
    //sqlite3_free(query);

    short i;
    for (i = ROMAN_MIN_VALUE; i <= ROMAN_MAX_VALUE; i++) {
        query = sqlite3_mprintf(
                "INSERT INTO roman_numerals VALUES (%d, '%q');", i, int_to_roman(i));

        rc = sqlite3_exec(db, query, 0, 0, &err_msg);
        if (rc != SQLITE_OK ) {

            fprintf(stderr, "SQL error: %s\n", err_msg);

            sqlite3_free(err_msg);
            sqlite3_free(query);
            sqlite3_close(db);

            return 1;
        }
    }

    sqlite3_free(err_msg);
    sqlite3_free(query);
    sqlite3_close(db);
    return 0;
}
