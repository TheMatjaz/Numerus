/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


numerus_err_t
numerus_to_roman_alloc(char** const numeral, const int_fast32_t value)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    *numeral = NULL;
    char buffer[NUMERUS_BASIC_MAX_LEN];
    numerus_err_t err = numerus_to_roman(buffer, value);
    if (err != NUMERUS_OK) { return err; }
    *numeral = strdup(buffer);
    if (*numeral == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}

numerus_err_t
numerus_to_roman_extended_fraction_alloc(
        char** const numeral, const numerus_frac_t fraction)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    char buffer[NUMERUS_EXTENDED_MAX_LEN];
    numerus_err_t err = numerus_to_roman_extended_fraction(buffer, fraction);
    if (err != NUMERUS_OK) { return err; }
    *numeral = strdup(buffer);
    if (*numeral == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}

numerus_err_t
numerus_to_roman_extended_double_alloc(
        char** const numeral,
        const double value)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    char buffer[NUMERUS_EXTENDED_MAX_LEN];
    numerus_err_t err = numerus_to_roman_extended_double(buffer, value);
    if (err != NUMERUS_OK) { return err; }
    *numeral = strdup(buffer);
    if (*numeral == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}
