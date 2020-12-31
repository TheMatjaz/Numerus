/**
 * @file
 * Test cases for the fraction functions from numerus_fraction.c
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


static void test_simplify_fraction_invalid(void)
{
    numerus_frac_t fraction;

    atto_eq(numerus_simplify_fraction(NULL), NUMERUS_ERR_NULL_FRACTION);
    fraction.int_part = NUMERUS_EXTENDED_INT_MAX;
    fraction.twelfths = 12;
    atto_eq(numerus_simplify_fraction(&fraction),
            NUMERUS_ERR_VALUE_OUT_OF_RANGE);
    fraction.int_part = NUMERUS_EXTENDED_INT_MAX + 1;
    fraction.twelfths = 0;
    atto_eq(numerus_simplify_fraction(&fraction),
            NUMERUS_ERR_VALUE_OUT_OF_RANGE);
}

static void test_simplify_fraction_valid(void)
{
    numerus_frac_t fraction;

    fraction.int_part = 0;
    fraction.twelfths = 0;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, 0);
    atto_eq(fraction.twelfths, 0);

    fraction.int_part = 1;
    fraction.twelfths = 0;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, 1);
    atto_eq(fraction.twelfths, 0);

    fraction.int_part = 1;
    fraction.twelfths = -10;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, 0);
    atto_eq(fraction.twelfths, 2);

    fraction.int_part = -1;
    fraction.twelfths = -10;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, -1);
    atto_eq(fraction.twelfths, -10);

    fraction.int_part = -1;
    fraction.twelfths = -12;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, -2);
    atto_eq(fraction.twelfths, 0);

    fraction.int_part = 10;
    fraction.twelfths = -15;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, 8);
    atto_eq(fraction.twelfths, 9);

    fraction.int_part = -100;
    fraction.twelfths = -61;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, -105);
    atto_eq(fraction.twelfths, -1);

    fraction.int_part = -100;
    fraction.twelfths = 61;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, -94);
    atto_eq(fraction.twelfths, -11);

    fraction.int_part = NUMERUS_EXTENDED_INT_MIN + 1;
    fraction.twelfths = -23;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, NUMERUS_EXTENDED_INT_MIN);
    atto_eq(fraction.twelfths, -11);

    fraction.int_part = NUMERUS_EXTENDED_INT_MAX - 1;
    fraction.twelfths = 23;
    atto_eq(numerus_simplify_fraction(&fraction), NUMERUS_OK);
    atto_eq(fraction.int_part, NUMERUS_EXTENDED_INT_MAX);
    atto_eq(fraction.twelfths, 11);
}

static void test_fraction_to_double_invalid(void)
{
    numerus_frac_t fraction;
    double real = 1.0;

    atto_eq(numerus_fraction_to_double(NULL, fraction), NUMERUS_ERR_NULL_DOUBLE);
    fraction.int_part = NUMERUS_EXTENDED_INT_MAX;
    fraction.twelfths = 12;
    atto_eq(numerus_fraction_to_double(&real, fraction),
            NUMERUS_ERR_VALUE_OUT_OF_RANGE);
    atto_nan(real);
    real = 1.0;
    fraction.int_part = NUMERUS_EXTENDED_INT_MAX + 1;
    fraction.twelfths = 0;
    atto_eq(numerus_fraction_to_double(&real, fraction),
            NUMERUS_ERR_VALUE_OUT_OF_RANGE);
    atto_nan(real);
}

static void test_fraction_to_double_valid(void)
{
    numerus_frac_t fraction;
    double real = 1.0;

    fraction.int_part = 0;
    fraction.twelfths = 0;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, 0.0, 1e-4);

    fraction.int_part = 1;
    fraction.twelfths = 0;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, 1.0, 1e-4);

    fraction.int_part = 1;
    fraction.twelfths = -10;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, 2.0 / 12, 1e-4);

    fraction.int_part = -1;
    fraction.twelfths = -10;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, -1.0 - 10.0 / 12, 1e-4);

    fraction.int_part = -1;
    fraction.twelfths = -12;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, -2.0, 1e-4);

    fraction.int_part = 10;
    fraction.twelfths = -15;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, 8.0 + 9.0 / 12, 1e-4);

    fraction.int_part = -100;
    fraction.twelfths = -61;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, -105.0 - 1.0 / 12, 1e-4);

    fraction.int_part = -100;
    fraction.twelfths = 61;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, -94 - 11.0 / 12, 1e-4);

    fraction.int_part = NUMERUS_EXTENDED_INT_MIN + 1;
    fraction.twelfths = -23;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, NUMERUS_EXTENDED_INT_MIN - 11.0 / 12, 1e-4);

    fraction.int_part = NUMERUS_EXTENDED_INT_MAX - 1;
    fraction.twelfths = 23;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, NUMERUS_EXTENDED_INT_MAX + 11.0 / 12, 1e-4);

    fraction.int_part = NUMERUS_EXTENDED_INT_MIN;
    fraction.twelfths = -11;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, NUMERUS_EXTENDED_MIN, 1e-4);

    fraction.int_part = NUMERUS_EXTENDED_INT_MAX;
    fraction.twelfths = 11;
    atto_eq(numerus_fraction_to_double(&real, fraction), NUMERUS_OK);
    atto_ddelta(real, NUMERUS_EXTENDED_MAX, 1e-4);
}


static void test_double_to_fraction_invalid(void)
{
    numerus_frac_t fraction;

    atto_eq(numerus_double_to_fraction(NULL, 1.0), NUMERUS_ERR_NULL_FRACTION);
    atto_eq(numerus_double_to_fraction(&fraction, NAN),
            NUMERUS_ERR_NOT_FINITE_DOUBLE);
    atto_eq(numerus_double_to_fraction(&fraction, +INFINITY),
            NUMERUS_ERR_NOT_FINITE_DOUBLE);
    atto_eq(numerus_double_to_fraction(&fraction, -INFINITY),
            NUMERUS_ERR_NOT_FINITE_DOUBLE);
    atto_eq(numerus_double_to_fraction(
            &fraction, NUMERUS_EXTENDED_MIN - 1.0 / 12.0),
            NUMERUS_ERR_VALUE_OUT_OF_RANGE);
    atto_eq(numerus_double_to_fraction(
            &fraction, NUMERUS_EXTENDED_MAX + 1.0 / 12.0),
            NUMERUS_ERR_VALUE_OUT_OF_RANGE);
}

static void test_double_to_fraction_valid(void)
{
    numerus_frac_t fraction;

    atto_eq(numerus_double_to_fraction(&fraction, 0.0), NUMERUS_OK);
    atto_eq(fraction.int_part, 0);
    atto_eq(fraction.twelfths, 0);

    atto_eq(numerus_double_to_fraction(&fraction, 1.005), NUMERUS_OK);
    atto_eq(fraction.int_part, 1);
    atto_eq(fraction.twelfths, 0);

    atto_eq(numerus_double_to_fraction(&fraction, 2.1 / 12), NUMERUS_OK);
    atto_eq(fraction.int_part, 0);
    atto_eq(fraction.twelfths, 2);

    atto_eq(numerus_double_to_fraction(&fraction, -1.0 - 9.8 / 12),
            NUMERUS_OK);
    atto_eq(fraction.int_part, -1);
    atto_eq(fraction.twelfths, -10);

    atto_eq(numerus_double_to_fraction(&fraction, -2.0), NUMERUS_OK);
    atto_eq(fraction.int_part, -2);
    atto_eq(fraction.twelfths, 0);

    atto_eq(numerus_double_to_fraction(&fraction, 8.0 + 9.2 / 12), NUMERUS_OK);
    atto_eq(fraction.int_part, 8);
    atto_eq(fraction.twelfths, 9);

    atto_eq(numerus_double_to_fraction(&fraction, -105.0 - 1.1 / 12),
            NUMERUS_OK);
    atto_eq(fraction.int_part, -105);
    atto_eq(fraction.twelfths, -1);

    atto_eq(numerus_double_to_fraction(&fraction, -94 + 10.7566 / 12),
            NUMERUS_OK);
    atto_eq(fraction.int_part, -93);
    atto_eq(fraction.twelfths, -1);

    atto_eq(numerus_double_to_fraction(&fraction, NUMERUS_EXTENDED_MIN),
            NUMERUS_OK);
    atto_eq(fraction.int_part, NUMERUS_EXTENDED_INT_MIN);
    atto_eq(fraction.twelfths, -11);

    atto_eq(numerus_double_to_fraction(&fraction, NUMERUS_EXTENDED_MAX),
            NUMERUS_OK);
    atto_eq(fraction.int_part, NUMERUS_EXTENDED_INT_MAX);
    atto_eq(fraction.twelfths, 11);
}

void test_fraction(void)
{
    test_simplify_fraction_invalid();
    test_simplify_fraction_valid();
    test_fraction_to_double_invalid();
    test_fraction_to_double_valid();
    test_double_to_fraction_invalid();
    test_double_to_fraction_valid();
}
