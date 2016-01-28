/**
 * @file numerus.h
 * @brief Numerus roman numerals library header
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under 
 * the BSD 3-clause license.
 */


/**
 * Internal representation of a roman numeral value with its twelfths.
 *
 * Contains a long representing the integer part of the value and a short with
 * the decimal part, incating precisely the number of twelfths the value has.
 * This is necessary to avoid rounding errors for floating point representations
 * of the value.
 */
//struct _num_numeral_value {
//    long integer_part;
//    short twelfths;
//};

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
#define NUMERUS_ERROR_VALUE_OUT_OF_RANGE 101

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
#define NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG 120
#define NUMERUS_ERROR_DECIMALS_IN_LONG_PART 116
#define NUMERUS_ERROR_ILLEGAL_MINUS 117
#define NUMERUS_ERROR_M_IN_SHORT_PART 118
#define NUMERUS_ERROR_GENERIC 119
#define NUMERUS_ERROR_MALLOC_FAIL 120
#define NUMERUS_ERROR_NULL_ROMAN 121
#define NUMERUS_ERROR_EMPTY_ROMAN 122

/* Constants with particular values and extremes */
extern const double NUMERUS_MAX_VALUE;
extern const long NUMERUS_MAX_LONG_VALUE;
extern const double NUMERUS_MIN_VALUE;
extern const long NUMERUS_MIN_LONG_VALUE;
extern const double NUMERUS_MAX_SHORT_VALUE;
extern const double NUMERUS_MIN_SHORT_VALUE;
extern const short NUMERUS_MAX_LENGTH;
extern const char *NUMERUS_ZERO;
extern const char *NUMERUS_FLOAT_SYNTAX_REGEX_STRING;

/**
 * Error code global variable
 */
extern int numerus_error_code;

/* Numeral analysis functions */
short numerus_is_zero(char *roman, int *errcode);
short numerus_is_long_numeral(char *roman, int *errcode);
short numerus_is_float_numeral(char *roman, int *errcode);
short numerus_sign(char *roman, int *errcode);
short numerus_numeral_length(char *roman, int *errcode);

/* Conversion function from value to roman numeral */
char *numerus_int_to_roman(long int_value, int *errcode);
char *numerus_int_with_twelfth_to_roman(long int_part, short frac_part, int *errcode);
char *numerus_double_to_roman(double double_value, int *errcode);


/* Conversion function from roman numeral to value */
long numerus_roman_to_int_and_frac_part(char *roman, short *frac_part, int *errcode);
long numerus_roman_to_int(char *roman, int *errcode);
double numerus_roman_to_double(char *roman, int *errcode);


/* Utility functions */
double numerus_round_to_nearest_12th(double value);
long numerus_double_to_parts(double value, short *frac_part);
double numerus_parts_to_double(long int_part, short frac_part);
int numerus_compare_value(char *roman_bigger, char *roman_smaller, int *errcode);


/* Export functions */
int numerus_export_to_sqlite3(char *filename, long min_value, long max_value);
int numerus_export_to_csv(char *filename, long min_value, long max_value,
                          int numerals_first, char *separator, char *newline,
                          char *quotes);


/* Printing functions */
char *numerus_pretty_print_long_numerals(char *roman, int *errcode);
char *numerus_pretty_print_float_value(double double_value, int shorten);
const char *numerus_explain_error(int error_code);
char *numerus_shorten_fraction(short twelfth);


/* Command line interface */
void numerus_repl(int argc, char **args);