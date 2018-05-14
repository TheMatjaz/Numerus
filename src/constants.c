/**
 * @file constants.c
 * @brief
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"
#include "internal.h"

const int16_t NUMERUS_MAX_BASIC_VALUE = 3999;
const int16_t NUMERUS_MIN_BASIC_VALUE = -NUMERUS_MAX_BASIC_VALUE;

const int32_t NUMERUS_MAX_EXTENDED_VALUE_INT_PART = 3999999;
const int32_t NUMERUS_MIN_EXTENDED_VALUE_INT_PART =
        -NUMERUS_MAX_EXTENDED_VALUE_INT_PART;

// TODO evaluate using 11.5/12.0 because it would be de facto the maximum
// representable number as the conversion function would round it down to
// 11.0/12.0 before converting it.
const double NUMERUS_MAX_EXTENDED_VALUE =
        NUMERUS_MAX_EXTENDED_VALUE_INT_PART + 11.0/12.0;
const double NUMERUS_MIN_EXTENDED_VALUE = -NUMERUS_MAX_EXTENDED_VALUE;


const uint8_t NUMERUS_MAX_BASIC_LENGTH = 17U;
const uint8_t NUMERUS_MAX_EXTENDED_LENGTH = 37U;
const char* const NUMERUS_ZERO_NUMERAL = "NULLA";


/**
 * Dictionary of a priori known roman chars, their values and repetitions used
 * by conversion functions.
 *
 * The last value is a terminator, to be used by conversion functions to
 * understand that the array has been parsed.
 */
const dictionary_char_t DICTIONARY[NUMERUS_DICTIONARY_SIZE] = {
        {1000, 'M', '\0', 3}, // index: 0
        {900,  'C', 'M',  1}, // index: 1
        {500,  'D', '\0', 1}, // index: 2
        {400,  'C', 'D',  1}, // index: 3
        {100,  'C', '\0', 3}, // index: 4
        {90,   'X', 'C',  1}, // index: 5
        {50,   'L', '\0', 1}, // index: 6
        {40,   'X', 'L',  1}, // index: 7
        {10,   'X', '\0', 3}, // index: 8
        {9,    'I', 'X',  1}, // index: 9
        {5,    'V', '\0', 1}, // index: 10
        {4,    'I', 'V',  1}, // index: 11
        {1,    'I', '\0', 3}, // index: 12
        {6,    'S', '\0', 1}, // index: 13
        {1,    '.', '\0', 5}, // index: 14
};
