/**
 * @file
 * String formatting functions for roman numerals and fractions
 * using heap-allocated strings.
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"
#if NUMERUS_HAS_MALLOC

numerus_err_t numerus_fmt_fraction_alloc(
        char** const formatted, const numerus_frac_t fraction)
{
    if (formatted == NULL) { return NUMERUS_ERR_NULL_FORMATTED; }
    *formatted = NULL;  // Provide unusable output in case of error
    char buffer[NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM];
    const numerus_err_t err = numerus_fmt_fraction(buffer, fraction);
    if (err != NUMERUS_OK) { return err; }
    *formatted = strdup(buffer);
    if (*formatted == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}

numerus_err_t numerus_fmt_overlined_alloc(
        char** const formatted,
        const char* const numeral,
        const bool use_windows_eol)
{
    if (formatted == NULL) { return NUMERUS_ERR_NULL_FORMATTED; }
    *formatted = NULL;  // Provide unusable output in case of error
    char buffer[NUMERUS_OVERLINED_MAX_LEN_WITH_TERM];
    const numerus_err_t err = numerus_fmt_overlined(
            buffer, numeral, use_windows_eol);
    if (err != NUMERUS_OK) { return err; }
    *formatted = strdup(buffer);
    if (*formatted == NULL) { return NUMERUS_ERR_MALLOC_FAILURE; }
    return NUMERUS_OK;
}

#endif  /* NUMERUS_HAS_MALLOC */
