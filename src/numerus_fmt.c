/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


#define FMTSTR_INT "%"PRId32
#define FMTSTR_INT_AND_TWELFTH "%"PRId32", %"PRId32"/%"PRIdFAST8
/**
 * The maximum length a basic roman numeral string may have.
 *
 * It includes the null terminator `\0`.
 *
 * That is the length of the roman numeral `-DCCCLXXXVIIIS.....\0`
 * with value -888-11/12.
 */
#define NUMERUS_POST_VINCULUM_MAX_LEN (20)

static numerus_err_t
fmt_overlined(char* formatted, const char* numeral, const bool windows)
{
    if (formatted == NULL) { return NUMERUS_ERR_NULL_FORMATTED; }
    if (numeral == NULL)
    {
        *formatted = '\0';
        return NUMERUS_ERR_NULL_NUMERAL;
    }
    const bool is_negative = (*numeral == '-');
    if (*numeral == '_')
    {
        // Has vinculum.
        if (is_negative)
        {
            // Write space above the minus sign, before the vinculum line
            *(formatted++) = ' ';
            numeral++; // Skip the minus sign in the original
        }
        numeral++; // Skip start of vinculum
        while (*numeral != '_')
        {
            *(formatted++) = '_';
            if (*numeral == '\0')
            {
                *formatted = '\0';
                return NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM;
            }
            numeral++;
        }
        // Add the end of line, both Windows or Unix styles supported.
        if (windows) { *(formatted++) = '\r'; }
        *(formatted++) = '\n';
        // Write the minus before the overlined part
        if (is_negative) { *(formatted++) = '-'; }
        numeral++; // Skip end of vinculum
    }
    // Copy the part after the vinculum
    strncpy(formatted, numeral, NUMERUS_POST_VINCULUM_MAX_LEN);
    return NUMERUS_OK;
}

inline numerus_err_t numerus_fmt_overlined_wineol(
        char* formatted, const char* numeral)
{
    return fmt_overlined(formatted, numeral, true);
}

inline numerus_err_t numerus_fmt_overlined_unixeol(
        char* formatted, const char* numeral)
{
    return fmt_overlined(formatted, numeral, false);
}


/**
 * @internal
 * Converts a fraction of twelfts into a simpler fraction of halves, thirds
 * etc.
 *
 * Examples:
 * - 1/12, 5/12, 7/12, 11/12 stay the same
 * - 2/12 becomes 1/6
 * - -3/12 becomes -1/4
 * - 4/12 becomes 1/3
 * - -6/12 becomes -1/2
 *
 * Assumes the input denominator is 12 and numerator is in [-11, -1]
 * or [1, 11].
 *
 * @param[in, out] numerator simplified numerator
 * @param[out] denominator simplified denominator
 */
inline static void
simplify_fraction(int32_t* const numerator, int_fast8_t* const denominator)
{
    const bool is_negative = *numerator < 0;
    if (is_negative) { *numerator = -*numerator; }
    switch (*numerator)
    {
        case 2:
        {
            // 2/12 = 1/6
            *numerator = 1;
            *denominator = 6;
            break;
        }
        case 3:
        {
            // 3/12 = 1/4
            *numerator = 1;
            *denominator = 4;
            break;
        }
        case 4:
        {
            // 4/12 = 1/3
            *numerator = 1;
            *denominator = 3;
            break;
        }
        case 6:
        {
            // 6/12 = 1/2
            *numerator = 1;
            *denominator = 2;
            break;
        }
        case 8:
        {
            // 8/12 = 2/3
            *numerator = 2;
            *denominator = 3;
            break;
        }
        case 9:
        {
            // 9/12 = 3/4
            *numerator = 3;
            *denominator = 4;
            break;
        }
        case 10:
        {
            // 10/12 = 5/6
            *numerator = 5;
            *denominator = 6;
            break;
        }
        default: break; // Nothing to simplify, stays the same
    }
    if (is_negative) { *numerator = -*numerator; }
}

numerus_err_t numerus_fmt_fraction(
        char* formatted, numerus_frac_t fraction)
{
    if (formatted == NULL) { return NUMERUS_ERR_NULL_FORMATTED; }
    numerus_err_t err = numerus_simplify_fraction(&fraction);
    if (err != NUMERUS_OK)
    {
        *formatted = '\0';
        return err;
    }
    if (fraction.twelfths == 0)
    {
        formatted += sprintf(formatted, FMTSTR_INT, fraction.int_part);
    }
    else
    {
        int_fast8_t denominator = 12;
        simplify_fraction(&fraction.twelfths, &denominator);
        formatted += sprintf(
                formatted, FMTSTR_INT_AND_TWELFTH,
                fraction.int_part, fraction.twelfths, denominator);
    }
    *formatted = '\0';
    return NUMERUS_OK;
}
