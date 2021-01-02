/**
 * @file
 * Test suite runner of the Numerus library.
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


int main(void)
{
    test_constants();
    test_strutils();
    test_fraction();
    test_fmt();
    test_roman_from_int();
    test_roman_to_int();
    test_roman_to_double();
    return atto_at_least_one_fail;
}
