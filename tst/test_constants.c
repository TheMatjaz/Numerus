/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"

void test_constants(void) {
    atto_streq(NUMERUS_ZERO_ROMAN, "NULLA", 5);
    atto_eq(NUMERUS_ZERO_ROMAN_LEN, 5+1);
    atto_eq(NUMERUS_BASIC_MAX, 3999);
    atto_eq(NUMERUS_BASIC_MIN, -3999);
    atto_ddelta(NUMERUS_EXTENDED_MAX, 3999999.9583333335, 1e-6);
    atto_ddelta(NUMERUS_EXTENDED_MIN, -3999999.9583333335, 1e-6);
    atto_eq(NUMERUS_BASIC_MAX_LEN, 17);
    atto_eq(NUMERUS_EXTENDED_MAX_LEN, 37);
    atto_eq(NUMERUS_EXTENDED_OVERLINED_MAX_LEN, 53);
}
