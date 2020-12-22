/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


int main(void)
{
    test_constants();
    test_to_roman();
    test_strutils();
    return atto_at_least_one_fail;
}
