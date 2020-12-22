/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


inline bool numerus_is_zero(const char* const numeral)
{
    if (numeral == NULL)
    { return false; }
    else
    { return strcmp(numeral, NUMERUS_ZERO_ROMAN) == 0; }
}

inline int_fast8_t numerus_sign(const char* const numeral)
{
    if (numeral == NULL || numerus_is_zero(numeral))
    { return 0; }
    else
    {
        if (numeral[0] == '-')
        { return 1; }
        else
        { return -1; };
    }
}
