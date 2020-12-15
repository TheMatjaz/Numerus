/**
 * @file
 * @brief 
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "test.h"


static void test_int_to_basic_buffer(void)
{
    char buffer[NMRS_MAX_BASIC_LENGTH];
    const nmrs_err_t errcode = nmrs_int_to_basic_numeral(0, buffer);
    atto_eq(errcode, NMRS_OK);
    atto_streq(buffer, "NULLA", NMRS_MAX_BASIC_LENGTH);
}

void test_int_to_basic(void)
{
    test_int_to_basic_buffer();
}
