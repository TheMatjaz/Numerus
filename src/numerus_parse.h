/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NUMERUS_PARSE_H_
#define NUMERUS_PARSE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "numerus.h"
#include "numerus_dictionary.h"

/**
 * @internal
 * Advances the char pointer to the first non-ASCII whitespace in the string.
 */
#define SKIP_LEADING_WHITESPACE(str) { while(isspace(*(str))) { (str)++; } }

#define NUMERUS_POST_VINCULUM_ABSMAX (+999)


/**
 * @internal
 * State of the parsing of numeral into a value.
 */
typedef struct
{
    uint8_t char_repetitions;
    int8_t sign;
    bool numeral_is_extended;
} numerus_parser_t;

void parse_init(numerus_parser_t* parser);

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_PARSE_H_ */
