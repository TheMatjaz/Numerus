/**
 * @file main.c
 * @brief Numerus test main.
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-Clause license
  *
 * This file starts a desire set of tests of the Numerus library.
 */

#include "numerus_test.h"


/**
 * Numerus test main that starts a set of test functions.
 */
int main() {
    numtest_convert_all_integers_with_parts();
    numtest_convert_all_integers_with_doubles();
    return 0;
}
