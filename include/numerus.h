/**
 * @file numerus.h
 * @brief Numerus roman numerals library API.
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 *
 * This header allows access to all public functionality of Numerus.
 */

#ifndef NUMERUS_H
#define NUMERUS_H

#include <stdbool.h>  /* To use booleans `true` and `false` */
#include <stdint.h>   /* To use integers with bit-lengths like int8, uint16 */

#include "numerus_error_codes.h"


/* Extremes of the value range */
extern const double NUMERUS_MAX_VALUE;
extern const double NUMERUS_MIN_VALUE;
extern const int32_t NUMERUS_MAX_LONG_NONFLOAT_VALUE;
extern const int32_t NUMERUS_MIN_LONG_NONFLOAT_VALUE;
extern const double NUMERUS_MAX_NONLONG_FLOAT_VALUE;
extern const double NUMERUS_MIN_NONLONG_FLOAT_VALUE;


/* Special values */
extern const uint8_t NUMERUS_MAX_LENGTH;
extern const char *NUMERUS_ZERO;


/* Error code global variable */
extern int8_t numerus_error_code;


/* Conversion function from value to roman numeral */
char *numerus_double_to_roman(double double_value, int8_t *errcode);
char *numerus_int_to_roman(int32_t int_value, int8_t *errcode);
char *numerus_int_with_twelfth_to_roman(int32_t int_part, uint8_t twelfths,
        int8_t *errcode);


/* Conversion function from roman numeral to value */
double numerus_roman_to_double(char *roman, int8_t *errcode);
int32_t numerus_roman_to_int(char *roman, int8_t *errcode);
int32_t numerus_roman_to_int_part_and_twelfths(char *roman, uint8_t *twelfths,
        int8_t *errcode);


/* Functions to manage twelfths */
double numerus_parts_to_double(int32_t int_part, uint8_t twelfths);
int32_t numerus_double_to_parts(double value, uint8_t *twelfths);
void numerus_shorten_and_same_sign_to_parts(int32_t *int_part, uint8_t *twelfths);


/* Numeral analysis functions */
bool numerus_is_zero(char *roman, int8_t *errcode);
bool numerus_is_long_numeral(char *roman, int8_t *errcode);
bool numerus_is_float_numeral(char *roman, int8_t *errcode);
int8_t numerus_sign(char *roman, int8_t *errcode);
int8_t numerus_count_roman_chars(char *roman, int8_t *errcode);
int8_t numerus_compare_value(char *roman_bigger, char *roman_smaller,
        int8_t *errcode);


/* Output formatting functions */
char *numerus_overline_long_numerals(char *roman, int8_t *errcode);
char *numerus_create_pretty_value_as_double(double double_value);
char *numerus_create_pretty_value_as_parts(int32_t int_part, uint8_t twelfths);
const char *numerus_explain_error(int8_t error_code);


/* Command line interface */
int8_t numerus_cli(int argc, char **args);


#endif /* NUMERUS_H */

