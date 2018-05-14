/**
 * @file internal.h
 * @brief Numerus roman numerals library internal/private definitions
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 *
 * This header allows access to the internal functionality that the library
 * files share within themselves. It's meant to be used just by library files,
 * not for public usage.
 */

#ifndef NUMERUS_INTERNAL_H_
#define NUMERUS_INTERNAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../include/numerus.h"
#include "dictionary.h"

/**
 * @internal
 * Checks if the string is empty i.e. contains no characters i.e. length 0.
 *
 * A whitespace-only string counts as non-empty.
 */
#define STRING_IS_EMPTY(s) (*(s) == '\0')

/**
 * @internal
 * Minimalistic computation of the absolute value of an integer.
 *
 * @param x an integer, also unsigned.
 */
#define ABS(x) (((x) < 0) ? (-(x)) : (x))

/** @internal
 * Size of the NUMERUS_ZERO_STRING including null terminator.
 */
extern const uint8_t ZERO_NUMERAL_SIZE;

#define MAX_INT_PART_BITSIZE 22U /* 3999999 fits in 22 bits. */
#define MAX_TWELFTHS_BITSIZE 4U /* 12 fits in 4 bits. */
/**
 * @internal
 * Structure containing data about the numeral that is being converted to value.
 *
 * Used by roman->value conversions and other functions internal to that
 * conversion itself. Contains info about the currently parsed part of the
 * numeral, the dictionary char that the numeral is confronted with, the value,
 * the sign, if it has underscore and counts the number of consecutive
 * repetitions a single roman char has.
 */
typedef struct
{
    int32_t int_part;
    uint8_t current_numeral_position;
    uint8_t current_dictionary_char_index;
    uint8_t twelfths;
    uint8_t char_repetitions;
    int8_t numeral_sign;
    bool numeral_is_extended;
} numeral_parser_data_t;

numerus_status_t prepare_for_analysis(const char** p_numeral);
numerus_status_t obtain_numeral_buffer(
        char** p_result_numeral, uint8_t size);
bool contains_extended_characters(const char* numeral);

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_INTERNAL_H_ */
