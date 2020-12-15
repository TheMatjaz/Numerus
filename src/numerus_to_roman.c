/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"
#include "numerus_dictionary.h"


numerus_err_t
numerus_to_roman(char* numeral, int_fast16_t value)
{
    if (value < NUMERUS_BASIC_MIN || value > NUMERUS_BASIC_MAX)
    {
        return NUMERUS_ERR_VALUE_OUT_OF_RANGE;
    }
    if (value == 0)
    {
        strcpy(numeral, NUMERUS_ZERO_ROMAN);
        return NUMERUS_OK;
    };
    if (value < 0)
    {
        *(numeral++) = '-';
        value = -value;
    }
    uint_fast8_t dict_index = DICTIONARY_INDEX_FOR_M;
    while (value > 0)
    {
        while (value >= DICTIONARY[dict_index].value)
        {
            *(numeral++) = DICTIONARY[dict_index].character_1;
            if (DICTIONARY[dict_index].character_2
                != DICTIONARY_UNUSED_CHAR)
            {
                *(numeral++) = DICTIONARY[dict_index].character_2;
            }
            value -= DICTIONARY[dict_index].value;
        }
        dict_index++;
    }
    *numeral = '\0';
    return NUMERUS_OK;
}
