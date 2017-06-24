/**
 * @file numerus.h
 * @brief Numerus roman numerals library header
 * @copyright Copyright © 2015-2017, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under 
 * the BSD 3-clause license.
 *
 * This header allows access to all public functionality of Numerus.
 */

#ifndef NUMERUS_H
#define NUMERUS_H

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
char *numerus_double_to_roman(double double_value, int *errcode);
char *numerus_int_to_roman(long int_value, int *errcode);
char *numerus_int_with_twelfth_to_roman(long int_part, short twelfths,
                                        int *errcode);


/* Conversion function from roman numeral to value */
double numerus_roman_to_double(char *roman, int *errcode);
long numerus_roman_to_int(char *roman, int *errcode);
long numerus_roman_to_int_part_and_twelfths(char *roman, short *twelfths,
                                            int *errcode);


/* Functions to manage twelfths */
double numerus_parts_to_double(long int_part, short twelfths);
long numerus_double_to_parts(double value, short *twelfths);
void numerus_shorten_and_same_sign_to_parts(long *int_part, short *twelfths);


/* Numeral analysis functions */
short numerus_is_zero(char *roman, int *errcode);
short numerus_is_long_numeral(char *roman, int *errcode);
short numerus_is_float_numeral(char *roman, int *errcode);
short numerus_sign(char *roman, int *errcode);
short numerus_count_roman_chars(char *roman, int *errcode);
short numerus_compare_value(char *roman_bigger, char *roman_smaller,
                            int *errcode);


/* Output formatting functions */
char *numerus_overline_long_numerals(char *roman, int *errcode);
char *numerus_create_pretty_value_as_double(double double_value);
char *numerus_create_pretty_value_as_parts(long int_part, short twelfths);
const char *numerus_explain_error(int error_code);


/* Command line interface */
int numerus_cli(int argc, char **args);

#endif	// NUMERUS_H

