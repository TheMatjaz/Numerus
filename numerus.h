/**
 * @file numerus.h
 * @brief Numerus roman numerals library header
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under 
 * the BSD 3-clause license.
 */

#include "numerus_error_codes.h"


/* Extremes of the value range */
extern const double NUMERUS_MAX_VALUE;
extern const double NUMERUS_MIN_VALUE;
extern const long   NUMERUS_MAX_LONG_NONFLOAT_VALUE;
extern const long   NUMERUS_MIN_LONG_NONFLOAT_VALUE;
extern const double NUMERUS_MAX_NONLONG_FLOAT_VALUE;
extern const double NUMERUS_MIN_NONLONG_FLOAT_VALUE;


/* Special values */
extern const short  NUMERUS_MAX_LENGTH;
extern const char  *NUMERUS_ZERO;


/* Error code global variable */
extern int numerus_error_code;


/* Conversion function from value to roman numeral */
char *numerus_int_to_roman(long int_value, int *errcode);
char *numerus_int_with_twelfth_to_roman(long int_part, short twelfths, int *errcode);
char *numerus_double_to_roman(double double_value, int *errcode);


/* Conversion function from roman numeral to value */
long numerus_roman_to_int_part_and_twelfths(char *roman, short *twelfths,
                                            int *errcode);
long numerus_roman_to_int(char *roman, int *errcode);
double numerus_roman_to_double(char *roman, int *errcode);


/* Functions to manage twelfths */
long numerus_double_to_parts(double value, short *twelfths);
double numerus_parts_to_double(long int_part, short twelfths);


/* Numeral analysis functions */
short numerus_is_zero(char *roman, int *errcode);
short numerus_is_long_numeral(char *roman, int *errcode);
short numerus_is_float_numeral(char *roman, int *errcode);
short numerus_sign(char *roman, int *errcode);
short numerus_numeral_length(char *roman, int *errcode);
short numerus_compare_value(char *roman_bigger, char *roman_smaller, int *errcode);


/* Output formatting functions */
char *numerus_pretty_print_long_numerals(char *roman, int *errcode);
char *numerus_pretty_print_float_value(double double_value, int shorten);
char *numerus_shorten_fraction(short twelfth);
const char *numerus_explain_error(int error_code);


/* Command line interface */
void numerus_repl(int argc, char **args);