/**
 * @file
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"

#if NUMERUS_HAS_MALLOC

numerus_err_t
numerus_roman_from_int_alloc(char** numeral, int_fast32_t value)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    *numeral = NULL;  // Provide unusable output in case of error
    char buffer[NUMERUS_BASIC_MAX_LEN_WITH_TERM];
    numerus_err_t err = numerus_roman_from_int(buffer, value);
    if (err != NUMERUS_OK) { return err; }
    *numeral = strdup(buffer);
    if (*numeral == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}

numerus_err_t
numerus_roman_from_fraction_alloc(
        char** numeral, numerus_frac_t fraction)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    *numeral = NULL;  // Provide unusable output in case of error
    char buffer[NUMERUS_EXTENDED_MAX_LEN_WITH_TERM];
    numerus_err_t err = numerus_roman_from_fraction(buffer, fraction);
    if (err != NUMERUS_OK) { return err; }
    *numeral = strdup(buffer);
    if (*numeral == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}

numerus_err_t
numerus_roman_from_double_alloc(
        char** const numeral,
        const double value)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    *numeral = NULL;  // Provide unusable output in case of error
    char buffer[NUMERUS_EXTENDED_MAX_LEN_WITH_TERM];
    numerus_err_t err = numerus_roman_from_double(buffer, value);
    if (err != NUMERUS_OK) { return err; }
    *numeral = strdup(buffer);
    if (*numeral == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}

#endif  /* NUMERUS_HAS_MALLOC */
