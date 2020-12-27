/**
 * @file
 * API to the collection of tests of the components of Numerus.
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
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


void test_constants(void);
void test_strutils(void);
void test_fraction(void);
void test_fmt(void);
void test_to_roman_basic(void);
void test_to_roman_extended(void);
void test_from_roman_basic(void);
void test_from_roman_extended(void);

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_TEST_H_ */
