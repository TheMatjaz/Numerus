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

/**
 * Size of the NUMERUS_ZERO_STRING including null terminator.
 */
#define ZERO_NUMERAL_SIZE 6U

/**
 * Size of the dictionary used to convert roman numeral character to values
 * and vice versa.
 */
#define NUMERUS_DICTIONARY_SIZE 15U

/**
 * @internal
 * Struct containing a basic roman char, its integer value and the maximum
 * consecutive repetitions of it that a roman numeral may have.
 *
 * It's used to create the _NUM_DICTIONARY dictionary which in turn is used by
 * conversion functions.
 */
typedef struct
{
    const uint16_t value;
    const char character_1;
    const char character_2;
    const uint8_t max_repetitions;
} dictionary_char_t;


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
    char* const current_numeral_position;
    const dictionary_char_t* current_dictionary_char;
    int32_t int_part;
    uint8_t twelfths;
    uint8_t char_repetitions;
    int8_t numeral_sign;
    bool numeral_is_long;
} numeral_parser_data_t;

// Does not check for NULL pointers
void skip_head_whitespace(const char** p_string);

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_INTERNAL_H_ */
