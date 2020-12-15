/**
 * @file
 * @brief
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


const int16_t NMRS_MAX_BASIC_VALUE = 3999;
const int16_t NMRS_MIN_BASIC_VALUE = -NMRS_MAX_BASIC_VALUE;

const int32_t NMRS_MAX_EXTENDED_VALUE_INT_PART = 3999999;
const int32_t NMRS_MIN_EXTENDED_VALUE_INT_PART =
        -NMRS_MAX_EXTENDED_VALUE_INT_PART;

const float NMRS_MAX_EXTENDED_VALUE =
        NMRS_MAX_EXTENDED_VALUE_INT_PART + 11.5f / 12.0f;
const float NMRS_MIN_EXTENDED_VALUE = -NMRS_MAX_EXTENDED_VALUE;

const uint8_t NMRS_MAX_BASIC_LENGTH = 17U;
const uint8_t NMRS_MAX_EXTENDED_LENGTH = 37U;
const uint8_t NMRS_MAX_EXTENDED_OVERLINED_LENGTH = 53U;

const char* const NMRS_ZERO_NUMERAL = "NULLA";
const uint8_t ZERO_NUMERAL_SIZE = 6U;


/**
 * Dictionary of a priori known roman chars, their values and repetitions used
 * by conversion functions.
 *
 * The last value is a terminator, to be used by conversion functions to
 * understand that the array has been parsed.
 */
const dictionary_char_t DICTIONARY[NMRS_DICTIONARY_ENTRIES] = {
        {1000, 3, 'M', '\0'}, // index: 0
        {900,  1, 'C', 'M'}, //  index: 1
        {500,  1, 'D', '\0'}, // index: 2
        {400,  1, 'C', 'D'}, //  index: 3
        {100,  3, 'C', '\0'}, // index: 4
        {90,   1, 'X', 'C'}, //  index: 5
        {50,   1, 'L', '\0'}, // index: 6
        {40,   1, 'X', 'L'}, //  index: 7
        {10,   3, 'X', '\0'}, // index: 8
        {9,    1, 'I', 'X'}, //  index: 9
        {5,    1, 'V', '\0'}, // index: 10
        {4,    1, 'I', 'V'}, //  index: 11
        {1,    3, 'I', '\0'}, // index: 12
        {6,    1, 'S', '\0'}, // index: 13
        {1,    5, '.', '\0'}, // index: 14
};
