/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


inline bool numerus_is_zero(const char* numeral)
{
    // Lightweight case-INsensitive strncmp for a fixed 5-char string
    if (numeral == NULL) { return false; }
    if (numeral[0] == '-') { numeral++; }
    return numeral != NULL
           && toupper(numeral[0]) == NUMERUS_ZERO_ROMAN[0]
           && toupper(numeral[1]) == NUMERUS_ZERO_ROMAN[1]
           && toupper(numeral[2]) == NUMERUS_ZERO_ROMAN[2]
           && toupper(numeral[3]) == NUMERUS_ZERO_ROMAN[3]
           && toupper(numeral[4]) == NUMERUS_ZERO_ROMAN[4]
           && numeral[5] == '\0';
}

inline int_fast8_t numerus_sign(const char* const numeral)
{
    if (numeral == NULL || numerus_is_zero(numeral)) { return 0; }
    if (numeral[0] == '-') { return -1; }
    const int first = toupper(numeral[0]);
    if (first == '_'
        || first == 'M'
        || first == 'D'
        || first == 'C'
        || first == 'L'
        || first == 'X'
        || first == 'V'
        || first == 'I'
        || first == 'S'
        || first == '.') { return +1; }
    return 0;
}
