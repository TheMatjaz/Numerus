/**
 * @file numerus_test.h
 * @brief Numerus roman numerals library test file header
 * @copyright Copyright © 2015-2017, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 *
 * This header allows access to all test functions to verify the correctness
 * of the library. It's meant to be used just by library files, not for public
 * usage.
 */

#ifndef NUMERUS_TEST_H
#define NUMERUS_TEST_H

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

