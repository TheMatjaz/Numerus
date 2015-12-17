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
 * This is the opposite of ROMAN_ERROR.
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
 * interval [NUMERUS_MIN_VALUE, NUMERUS_MAX_VALUE].
 */
#define NUMERUS_ERROR_OUT_OF_RANGE 101;

/**
 * regexec() internal error.
 *
 * Happens while checking the syntax correctness of a roman numeral to be
 * converted to short by numerus_roman_to_short(). Check regerror() and the stderr for
 * more information.
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

extern const short int NUMERUS_MAX_VALUE;
extern const short int NUMERUS_MIN_VALUE;
extern const char *NUMERUS_ZERO;
extern const short int NUMERUS_MAX_LENGTH;
extern const char *NUMERUS_SYNTAX_REGEX_STRING;
extern short int numerus_error_code;

int numerus_is_roman(char *roman);
int numerus_roman_is_zero(char *roman);
char *numerus_short_to_roman(short int arabic);
short numerus_roman_to_short(char *roman);
int numerus_compare_value(char *roman_bigger, char *roman_smaller);
char**numerus_allocate_all_romans(short int include_negatives);
int numerus_export_all_to_sqlite3(char *filename);
