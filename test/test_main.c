/**
 * @file
 * @brief 
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "test.h"

int main(void)
{
    test_constants();

    return atto_at_least_one_fail;
}
