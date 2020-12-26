/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_parse.h"


void parse_init(numerus_parser_t* const parser)
{
    memset(parser, 0U, sizeof(numerus_parser_t));
    /*p_parser_data->integer_part = 0;
    p_parser_data->current_numeral_index = 0;
    p_parser_data->current_dictionary_char_index = DICTIONARY_INDEX_FOR_M;
    p_parser_data->twelfths = 0;
    p_parser_data->char_repetitions = 0;
    p_parser_data->numeral_sign = +1;
    p_parser_data->numeral_is_extended = false;*/
}

const char* parse_sign(numerus_parser_t* const parser, const char* numeral);
