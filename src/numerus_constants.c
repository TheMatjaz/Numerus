/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"
#include "numerus_dictionary.h"


const int16_t NUMERUS_BASIC_MAX = 3999;
const int16_t NUMERUS_BASIC_MIN = -3999;

const double NUMERUS_EXTENDED_MAX = 3999999.0 + 11.5 / 12.0;
const double NUMERUS_EXTENDED_MIN = -3999999.0 - 11.5 / 12.0;

const uint8_t NUMERUS_BASIC_MAX_LEN = 17U;
const uint8_t NUMERUS_EXTENDED_MAX_LEN = 37U;
const uint8_t NUMERUS_EXTENDED_OVERLINED_MAX_LEN = 53U;

const char* const NUMERUS_ZERO_ROMAN = "NULLA";
const uint8_t NUMERUS_ZERO_ROMAN_LEN = 6U;

const dictionary_entry_t DICTIONARY[] = {
        {1000, 3, 'M', DICTIONARY_UNUSED_CHAR}, // index: 0
        {900,  1, 'C', 'M'}, //  index: 1
        {500,  1, 'D', DICTIONARY_UNUSED_CHAR}, // index: 2
        {400,  1, 'C', 'D'}, //  index: 3
        {100,  3, 'C', DICTIONARY_UNUSED_CHAR}, // index: 4
        {90,   1, 'X', 'C'}, //  index: 5
        {50,   1, 'L', DICTIONARY_UNUSED_CHAR}, // index: 6
        {40,   1, 'X', 'L'}, //  index: 7
        {10,   3, 'X', DICTIONARY_UNUSED_CHAR}, // index: 8
        {9,    1, 'I', 'X'}, //  index: 9
        {5,    1, 'V', DICTIONARY_UNUSED_CHAR}, // index: 10
        {4,    1, 'I', 'V'}, //  index: 11
        {1,    3, 'I', DICTIONARY_UNUSED_CHAR}, // index: 12
        {6,    1, 'S', DICTIONARY_UNUSED_CHAR}, // index: 13
        {1,    5, '.', DICTIONARY_UNUSED_CHAR}, // index: 14
};

