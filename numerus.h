/**
 * @file numerus.h
 * @brief Numerus roman numerals library header
 * @copyright Copyright © 2015, Matjaž Guštin <dev@matjaz.it> 
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under 
 * the BSD 3-clause license.
 */


/**
 * Everything went all right.
 *
 * This is the opposite of any NUMERUS_ERROR_*.
 */
#define NUMERUS_OK 1000;

/**
 * Numeral is not a correct roman one.
 *
 * Check the syntax of the roman numeral for non-roman characters, too many
 * repetitions or wrong character order.
 */
#define NUMERUS_ERROR_WRONG_SYNTAX 100;

/**
 * Integer out of conversion range.
 *
 * numerus_short_to_roman() cannot convert the passed value because it exceed the
 * possible values a roman numeral may represent, which are the integers in the
 * interval [NUMERUS_MIN_LONG_VALUE, NUMERUS_MAX_LONG_VALUE].
 */
#define NUMERUS_ERROR_OUT_OF_RANGE 101;

/**
 * regexec() internal error.
 *
 * Happens while checking the syntax correctness of a roman numeral to be
 * converted to short by numerus_roman_to_short(). Check regerror() and the
 * stderr for more information.
 */
#define NUMERUS_ERROR_REGEXEC 102;

/**
 * An SQLite error happened.
 *
 * Check the stderr for more information.
 */
#define NUMERUS_ERROR_SQLITE 103;

/**
 * numerus_compare_value() comparision failed.
 *
 * It could not compare the two roman numerals because at least one of them
 * cannot be converted to short so it has no value.
 */
#define NUMERUS_ERROR_CANNOT_COMPARE 104;

/**
 * Could not allocate enough memory for the array of all roman numerals in
 * numerus_allocate_all_romans().
 */
#define NUMERUS_ERROR_ALLOCATE_ALL 105;

extern const long int NUMERUS_MAX_LONG_VALUE;
extern const long int NUMERUS_MIN_LONG_VALUE;
extern const char *NUMERUS_ZERO;
extern const short int NUMERUS_MAX_LONG_LENGTH;
extern const char *NUMERUS_LONG_SYNTAX_REGEX_STRING;
extern short int numerus_error_code;

int numerus_is_roman(char *roman, int is_short_numeral);
int numerus_is_zero(char *roman);
char *numerus_long_to_roman(long int arabic);
long numerus_roman_to_long(char *roman);
int numerus_compare_value(char *roman_bigger, char *roman_smaller);
int numerus_export_to_sqlite3(char *filename, long min_value, long max_value);
int numerus_export_to_csv(char *filename, long min_value, long max_value,
                          int numerals_first, char *separator, char *newline,
                          char *quotes);
short numerus_numeral_length(char *roman);
char *numerus_pretty_print_long_numerals(char *roman);
