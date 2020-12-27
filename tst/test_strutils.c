/**
 * @file
 * Test cases for the string-utilities functions from numerus_strutils.c
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"

static void test_is_zero(void)
{
    atto_eq(numerus_is_zero(NULL), false);
    atto_eq(numerus_is_zero(""), false);
    atto_eq(numerus_is_zero("0"), false);
    atto_eq(numerus_is_zero("hello"), false);
    atto_eq(numerus_is_zero("NULL"), false);
    atto_eq(numerus_is_zero("NULLAA"), false);
    atto_eq(numerus_is_zero("nullaA"), false);
    atto_eq(numerus_is_zero("NULLA"), true);
    atto_eq(numerus_is_zero("nulla"), true);
    atto_eq(numerus_is_zero("nuLLA"), true);
    atto_eq(numerus_is_zero("-NULLA"), true);
    atto_eq(numerus_is_zero("-nulla"), true);
    atto_eq(numerus_is_zero("-nuLLA"), true);
}

static void test_sign(void)
{
    atto_eq(numerus_sign(NULL), 0);
    atto_eq(numerus_sign(""), 0);
    atto_eq(numerus_sign("NULLA"), 0);
    atto_eq(numerus_sign("nulla"), 0);
    atto_eq(numerus_sign("-NULLA"), 0);
    atto_eq(numerus_sign("-nulla"), 0);
    atto_eq(numerus_sign("HELLO"), 1);
    atto_eq(numerus_sign("-HELLO"), -1);
}

void test_strutils(void) {
    test_is_zero();
    test_sign();
}
