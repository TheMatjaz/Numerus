/**
 * @file
 * Test cases for the constants of the Numerus library, either defines
 * or const data types values.
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


void test_constants(void)
{
    atto_streq(NUMERUS_ZERO_ROMAN, "NULLA", 5);
    atto_eq(strlen(NUMERUS_ZERO_ROMAN),
            NUMERUS_ZERO_ROMAN_LEN_WITH_TERM - 1);
    atto_eq(NUMERUS_ZERO_ROMAN_LEN_WITH_TERM, 5 + 1);
    atto_eq(NUMERUS_MAX_INT_CLASSIC, +3999);
    atto_eq(NUMERUS_MIN_INT_CLASSIC, -3999);
    atto_eq(NUMERUS_MAX_INT, +3999999L);
    atto_eq(NUMERUS_MIN_INT, -3999999L);
    atto_ddelta(NUMERUS_MAX_DOUBLE, +3999999.0 + 11.0 / 12.0, 1e-6);
    atto_ddelta(NUMERUS_MIN_DOUBLE, -3999999.0 - 11.0 / 12.0, 1e-6);
    atto_eq(NUMERUS_MAX_LEN_CLASSIC_WITH_TERM, 17);
    atto_eq(NUMERUS_MAX_LEN_WITH_TERM, 37);
    atto_eq(NUMERUS_OVERLINED_MAX_LEN_WITH_TERM, 53);
}
