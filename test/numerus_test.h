/**
 * @file
 * @brief Numerus roman numerals library test file header
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-Clause license
  *
 * This header allows access to all test functions to verify the correctness
 * of the library. It's meant to be used just by library files, not for public
 * usage.
 */

#ifndef NUMERUS_TEST_H
#define NUMERUS_TEST_H

#include "../src/internal.h"

#define PRINT_STATUS_DIVIDER 400000

int numtest_convert_all_floats_with_doubles();
void numtest_roman_syntax_errors();
int numtest_convert_all_integers_with_parts();
int numtest_convert_all_integers_with_doubles();
int numtest_convert_all_floats_with_parts();
void numtest_parts_to_from_double_functions();
void numtest_null_handling_conversions();
void numtest_null_handling_utils();
int numtest_pretty_print_all_numerals();
int numtest_pretty_print_all_values();

#endif // NUMERUS_TEST_H

