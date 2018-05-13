/**
 * @file numerus_internal.h
 * @brief Numerus roman numerals library internal header
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-Clause license
  *
 * This header allows access to all public functionality of Numerus and some
 * internal functionality that the library files share within themselves.
 * It's meant to be used just by library files, not for public usage.
 */


#ifndef NUMERUS_INTERNAL_H
#define NUMERUS_INTERNAL_H

#include "../include/numerus.h"

#define SIGN(x) (((x) >= 0)  - ((x) < 0))
#define ABS(x)  (((x) < 0) ? -(x) : (x))

bool _num_is_zero(char *roman);

#endif /* NUMERUS_INTERNAL_H */

