/**
 * @file numerus_utils.c
 * @brief Numerus constants and utility functions for roman numerals manipulation.
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
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
#include <stdio.h>    /* To  `fprintf()` to `stderr` */
#include <stdlib.h>   /* For `malloc()` */
#include <string.h>   /* For `strcmp()` */
#include <sqlite3.h>  /* To export all roman numerals to an SQLite3 file */
#include <stdbool.h>  /* To use booleans `true` and `false` */
#include "numerus.h"


/**
 * Verifies if the passed roman numeral is (-)NUMERUS_ZERO, case insensitive.
 *
 * @param *roman string containing a roman numeral to check if it is
 * NUMERUS_ZERO
 * @returns int 1 if the string is (-)NUMERUS_ZERO or 0 if it's not
 */
short numerus_is_zero(char *roman) {
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
short numerus_is_long_numeral(char *roman) {
    if (*roman == '_' || (*roman == '-' && *(roman+1) == '_')) {
        return true;
    } else {
        return false;
    }
}

// Checks for an S or . and returns the index of the first found, else -1
short numerus_is_float_numeral(char *roman) {
    short i = 0;
    while (*roman != '\0') {
        if (i > NUMERUS_MAX_LENGTH) {
            return NUMERUS_ERROR_TOO_LONG_NUMERAL;
        }
        if (*roman == 'S' || *roman == 's' || *roman == '.') {
            return i;
        } else {
            i++;
        }
        roman++;
    }
    return -1;
}

short numerus_sign(char *roman) {
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
int numerus_numeral_length(char *roman, short *length) {
    if (numerus_is_zero(roman)) {
        *length = (short) strlen(NUMERUS_ZERO);
        return NUMERUS_OK;
    }
    short i = 0;
    while (*roman != '\0') {
        if (i > NUMERUS_MAX_LENGTH) {
            *length = -1;
            numerus_error_code = NUMERUS_ERROR_TOO_LONG_NUMERAL;
            return NUMERUS_ERROR_TOO_LONG_NUMERAL;
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
                *length = -1;
                numerus_error_code = NUMERUS_ERROR_ILLEGAL_CHARACTER;
                return NUMERUS_ERROR_ILLEGAL_CHARACTER;
            }
        }
    }
    *length = i;
    return NUMERUS_OK;
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
    double value_bigger;
    int errcode_bigger = numerus_roman_to_double(roman_bigger, &value_bigger);
    double value_smaller;
    int errcode_smaller = numerus_roman_to_double(roman_smaller, &value_smaller);
    if (errcode_bigger != NUMERUS_OK || errcode_smaller != NUMERUS_OK) {
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

static size_t _num_pretty_print_malloc_size(char *roman) {
    size_t alloc_size = 0;
    while (*roman != '\0') {
        if (*roman != '_') {
            alloc_size++;
        }
        roman++;
    }
    alloc_size++; /* For '\n' at end of "____" line */
    // FIXME missing length of part between underscores??
    return alloc_size;
}

/* TODO: USE THE LENGTH MEASURING FUNCTION TO FIND ANY ILLEGAL CHARACTERS? */

char *numerus_pretty_print_long_numerals(char *roman) {
    if (roman == NULL) {
        return NULL;
    }
    if (numerus_is_long_numeral(roman)) {
        char *roman_start = roman;
        char *pretty_roman = malloc(_num_pretty_print_malloc_size(roman));
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

struct _num_error_codes {
    const int code;
    const char *message;
};

struct _num_error_codes _NUM_ERROR_CODES[] = {
        {NUMERUS_ERROR_WRONG_SYNTAX,                "Wrong syntax"},
        {NUMERUS_ERROR_OUT_OF_RANGE,                "Out of range"},
        {NUMERUS_ERROR_REGEXEC,                   "Regex compilation error"},
        {NUMERUS_ERROR_SQLITE,                    "Generic SQLite3 error"},
        {NUMERUS_ERROR_CANNOT_COMPARE,            "Cannot compare two romans because they have other errors"},
        {NUMERUS_ERROR_ILLEGAL_CHARACTER,         "Illegal character"},
        {NUMERUS_ERROR_TOO_LONG_NUMERAL,          "Too long numeral"},
        {NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS,   "Too many repetitions of a >repeatable< character, like MMMM or IIII"},
        {NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE,     "Illegal sequence or order of roman characters"},
        {NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE, "Missing second underscore"},
        {NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART,  "Underscore after second one"},
        {NUMERUS_ERROR_DECIMALS_IN_LONG_PART,     "Decimals between underscores"},
        {NUMERUS_ERROR_ILLEGAL_MINUS,             "Minus sign (-) in illegal position"},
        {NUMERUS_ERROR_M_IN_SHORT_PART,           "`M` symbol after underscores"},
        {NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG,    "Underscore in non long numeral"},
        {NUMERUS_OK,                              "OK"},
        {0,                                       "End reached"}
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
    return "ERROR CODE NOT FOUND";
}
