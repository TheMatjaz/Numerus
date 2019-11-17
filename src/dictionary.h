/**
 * @file
 * @brief
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NMRS_DICTIONARY_H_
#define NMRS_DICTIONARY_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * Size of the dictionary used to convert roman numeral character to values
 * and vice versa.
 */
#define NMRS_DICTIONARY_SIZE 15U

#define DICTIONARY_INDEX_FOR_M 0U
#define DICTIONARY_INDEX_FOR_CM 1U
#define DICTIONARY_INDEX_FOR_S 13U

#define MAX_DICTIONARY_VALUE_BITSIZE 10U /* 1000 fits in 10 bits. */
#define MAX_DICTIONARY_REPETITIONS_BITSIZE 3U /* 5 fits in 3 bits. */

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
    const uint16_t value : MAX_DICTIONARY_VALUE_BITSIZE;
    const uint8_t max_repetitions : MAX_DICTIONARY_REPETITIONS_BITSIZE;
    const char character_1;
    const char character_2;
} dictionary_char_t;

extern const dictionary_char_t DICTIONARY[NMRS_DICTIONARY_SIZE];

#ifdef __cplusplus
}
#endif

#endif  /* NMRS_DICTIONARY_H_ */
