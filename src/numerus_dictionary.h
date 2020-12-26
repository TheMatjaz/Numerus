/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NUMERUS_DICTIONARY_H_
#define NUMERUS_DICTIONARY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "numerus.h"

/** Parsing starting point when M is the largest possible roman char. */
#define DICTIONARY_INDEX_FOR_M 0U
/** Parsing starting point when CM is the largest possible roman char. */
#define DICTIONARY_INDEX_FOR_CM 1U
/** Parsing starting point when S is the largest possible roman char. */
#define DICTIONARY_INDEX_FOR_S 13U
#define DICTIONARY_INDEX_FOR_END 15U
/** Indicator that the roman character has only 1 symbol. */
#define DICTIONARY_UNUSED_CHAR '\0'

#define DICTIONARY_GLYPH_MAX_LEN 2U

/**
 * @internal
 * Struct containing a dictionary entry, that is a roman numeral character,
 * its integer value and the maximum consecutive repetitions of it that a
 * roman numeral may have.
 *
 * It's used to create the DICTIONARY which in turn is used by
 * conversion functions.
 *
 * Optimised for size.
 */
typedef struct
{
    uint16_t value: 10;  ///< `1000` fits in 10 bits.
    uint16_t max_repetitions: 6; ///< `5` fits in 3 bits, rounded up to align.
    char characters[DICTIONARY_GLYPH_MAX_LEN]; ///< 1 or 2 characters of the numeral glyph.
}dictionary_glyph_t;

/**
 * Dictionary of all possible roman chars, their values and repetitions used
 * by conversion functions.
 */
extern const dictionary_glyph_t DICTIONARY[];

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_DICTIONARY_H_ */
