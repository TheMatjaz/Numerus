/**
 * @file
 * API to the collection of tests of the components of Numerus.
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NUMERUS_TEST_H_
#define NUMERUS_TEST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "atto.h"
#include "numerus.h"
#include <time.h>


void test_constants(void);
void test_strutils(void);
void test_fraction(void);
void test_fmt(void);
void test_roman_from_int(void);
void test_roman_from_double(void);
void test_roman_from_fraction(void);
void test_roman_to_int(void);
void test_roman_to_double(void);
void test_roman_to_fraction(void);
void test_bijectivity(void);
void test_benchmark(void);

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_TEST_H_ */
