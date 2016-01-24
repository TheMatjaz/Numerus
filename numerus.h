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
#define NUMERUS_OK 1

/**
 * Numeral is not a correct roman one.
 *
 * Check the syntax of the roman numeral for non-roman characters, too many
 * repetitions or wrong character order.
 */
#define NUMERUS_ERROR_WRONG_SYNTAX 100


/**
 * Integer out of conversion range.
 *
 * numerus_short_to_roman() cannot convert the passed value because it exceed the
 * possible values a roman numeral may represent, which are the integers in the
 * interval [NUMERUS_MIN_LONG_VALUE, NUMERUS_MAX_LONG_VALUE].
 */
#define NUMERUS_ERROR_OUT_OF_RANGE 101

/**
 * regexec() internal error.
 *
 * Happens while checking the syntax correctness of a roman numeral to be
 * converted to short by numerus_roman_to_short(). Check regerror() and the
 * stderr for more information.
 */
#define NUMERUS_ERROR_REGEXEC 102

/**
 * An SQLite error happened.
 *
 * Check the stderr for more information.
 */
#define NUMERUS_ERROR_SQLITE 103

/**
 * numerus_compare_value() comparision failed.
 *
 * It could not compare the two roman numerals because at least one of them
 * cannot be converted to short so it has no value.
 */
#define NUMERUS_ERROR_CANNOT_COMPARE 104
#define NUMERUS_ERROR_ILLEGAL_CHARACTER 105
#define NUMERUS_ERROR_TOO_LONG_NUMERAL 106
#define NUMERUS_IS_FLOAT 107
#define NUMERUS_IS_LONG 108
#define NUMERUS_IS_NOT_FLOAT 109
#define NUMERUS_IS_NOT_LONG 110
#define NUMERUS_ERROR_NOT_ROMAN (long)(NUMERUS_MAX_VALUE + 1)
#define NUMERUS_IS_ZERO 111
#define NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS 112
#define NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE 113
#define NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE 114
#define NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART 115
#define NUMERUS_ERROR_DECIMALS_IN_LONG_PART 116
#define NUMERUS_ERROR_ILLEGAL_MINUS 117
#define NUMERUS_ERROR_M_IN_SHORT_PART 118

/* Constants with particular values and extremes */
extern const double NUMERUS_MAX_VALUE;
extern const long NUMERUS_MAX_LONG_VALUE;
extern const double NUMERUS_MIN_VALUE;
extern const long NUMERUS_MIN_LONG_VALUE;
extern const short int NUMERUS_MAX_LENGTH;
extern const char *NUMERUS_ZERO;
extern const char *NUMERUS_FLOAT_SYNTAX_REGEX_STRING;

/**
 * Error code global variable
 */
extern short int numerus_error_code;

/* Numeral analysis functions */
short numerus_is_zero(char *roman);
short numerus_is_long_numeral(char *roman);
short numerus_is_float_numeral(char *roman);
short numerus_sign(char *roman);
int numerus_numeral_length(char *roman, short *length);
short numerus_is_roman(char *roman);

/* Conversion function from value to roman numeral */
char *numerus_long_to_roman(long arabic);
char *numerus_double_to_roman(double value);

/* Conversion function from roman numeral to value */
int numerus_roman_to_double(char *roman, double *value);

/* Utility functions */
int numerus_compare_value(char *roman_bigger, char *roman_smaller);
int numerus_export_to_sqlite3(char *filename, long min_value, long max_value);
int numerus_export_to_csv(char *filename, long min_value, long max_value,
                          int numerals_first, char *separator, char *newline,
                          char *quotes);
char *numerus_pretty_print_long_numerals(char *roman);
const char *numerus_explain_error(int error_code);
// FIXME: IF I DONT PUT THIS ; HERE, IT GIVES ME A SYNTAX ERROR