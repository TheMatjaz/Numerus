/**
 * @file numerus_internal.h
 * @brief Numerus roman numerals library internal header
 * @copyright Copyright © 2015-2017, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 *
 * This header allows access to all public functionality of Numerus and some
 * internal functionality that the library files share within themselves.
 * It's meant to be used just by library files, not for public usage.
 */


#ifndef NUMERUS_INTERNAL_H
#define NUMERUS_INTERNAL_H

#include "numerus.h"
#include "numerus_test.h"


short _num_is_zero(char *roman);
#define SIGN(x)    (((x) >= 0)  - ((x) < 0))
#define ABS(x)     (((x) < 0) ? -(x) : (x))

#endif // NUMERUS_INTERNAL_H

