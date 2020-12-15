/**
 * @file
 * @brief 
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "test.h"


void test_constants(void)
{
    atto_eq(NMRS_OK, 0);
    atto_eq(NMRS_MIN_BASIC_VALUE, -3999);
    atto_eq(NMRS_MAX_BASIC_VALUE, +3999);
    atto_eq(NMRS_MAX_BASIC_LENGTH, strlen("-MMMDCCCLXXXVIII") + NULL_TERM_LEN);
    atto_eq(NMRS_MIN_EXTENDED_VALUE_INT_PART, -3999999);
    atto_eq(NMRS_MAX_EXTENDED_VALUE_INT_PART, +3999999);
    atto_eq(NMRS_MIN_EXTENDED_VALUE, -3999999.9166666667f);
    atto_eq(NMRS_MAX_EXTENDED_VALUE, +3999999.9166666667f);
    atto_eq(NMRS_MAX_EXTENDED_LENGTH,
            strlen("-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....") + NULL_TERM_LEN);
    atto_eq(NMRS_MAX_EXTENDED_OVERLINED_LENGTH,
            strlen(" _______________\r\n"
                   "-MMMDCCCLXXXVIIIDCCCLXXXVIIIS.....") + NULL_TERM_LEN);
    atto_streq(NMRS_ZERO_NUMERAL, "NULLA", 6);
}
