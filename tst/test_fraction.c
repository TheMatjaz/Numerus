/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


static void test_simplify_fraction(void)
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

void test_fraction(void)
{
    test_simplify_fraction();
}
