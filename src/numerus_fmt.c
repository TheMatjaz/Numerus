/**
 * @file
 * @internal
 * String formatting functions for roman numerals and fractions.
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


/** @internal
 * Format string for an integer part of a #numerus_frac_t */
#define FMTSTR_INTPART "%"PRId32

/** @internal
 * Format string for the twelfths of a #numerus_frac_t */
#define FMTSTR_TWELFTHS "%"PRId32"/%"PRIdFAST8

/** @internal
 * Format string for the separator between the integer part and twelfths of a
 * #numerus_frac_t */
#define FMTSTR_SEPARATOR ", "

/**
 * @internal
 * The maximum length a basic roman numeral string may have, excluding the
 * null terminator `\0`.
 *
 * That is the length of the roman numeral "-DCCCLXXXVIIIS....."
 * with value -888-11/12.
 */
#define NUMERUS_POST_VINCULUM_MAX_LEN (19)

/**
 * @internal
 * Behaves like strncpy() but always forcibly null-terminates the destination.
 *
 * Copies \p src to \p dst until the end of \p src or until \p len
 * characters are copied over. Then, regardless of which of the two cases
 * happened, forcibly terminates the destination string with '\0';
 *
 * @param [out] dst copy of \p src
 * @param [in] src original string to copy
 * @param [in] len max amount of bytes (chars) to copy
 * @returns end of the \p dst (the null terminator)
 */
inline static char* safe_strncpy(char* dst, const char* src, size_t len)
{
    while (*src != '\0' && len)
    {
        *(dst++) = *(src++);
        len--;
    }
    *dst = '\0'; // Forcibly terminate destination
    return dst;
}

numerus_err_t numerus_fmt_overlined(
        char* formatted,
        const char* numeral,
        const bool use_windows_eol)
{
    // TODO what if the numeral is too long? Do this check everywhere!
    if (formatted == NULL) { return NUMERUS_ERR_NULL_FORMATTED; }
    if (numeral == NULL)
    {
        *formatted = '\0';  // Provide empty string output in case of error
        return NUMERUS_ERR_NULL_NUMERAL;
    }
    const bool is_negative = (*numeral == '-');
    const bool has_vinculum =
            (is_negative && numeral[1] == '_') || (numeral[0] == '_');
    if (has_vinculum)
    {
        // Has vinculum, thus the overlining must be applied.
        if (is_negative)
        {
            // Write space above the minus sign, before the vinculum line
            *(formatted++) = ' ';
            numeral++;  // Skip the minus sign in the original string
        }
        numeral++;  // Skip start of vinculum in the original string
        // Start overlining the vinculum
        size_t vinculum_len = 0;
        while (*numeral != '_')
        {
            if (*numeral == '\0')
            {
                *formatted = '\0';
                return NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM;
            }
            *(formatted++) = '_';  // Overline a character in the vinculum
            numeral++;
            vinculum_len++;
        }
        // End of overlining, go to next line. Append EOL character(s)
        if (use_windows_eol) { *(formatted++) = '\r'; }
        *(formatted++) = '\n';
        // Write the minus before the overlined part
        if (is_negative) { *(formatted++) = '-'; }
        // Copy the part within the vinculum until the second '_'
        formatted = safe_strncpy(
                formatted, numeral - vinculum_len, vinculum_len);
        numeral++; // Skip end of vinculum in the original string
    }
    // Copy the rest of the string after the vinculum or (if no vinculum
    // at all) simply the whole string. The function null terminates properly.
    safe_strncpy(formatted, numeral, NUMERUS_POST_VINCULUM_MAX_LEN);
    return NUMERUS_OK;
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
simplify_fraction_smaller_than_1(
        int32_t* const numerator,
        int_fast8_t* const denominator)
{
    // Store the negative sign to reapply it at the end
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
    // Reapply negative sign, if any
    if (is_negative) { *numerator = -*numerator; }
}

numerus_err_t numerus_fmt_fraction(
        char* formatted, numerus_frac_t fraction)
{
    if (formatted == NULL) { return NUMERUS_ERR_NULL_FORMATTED; }
    numerus_err_t err = numerus_simplify_fraction(&fraction);
    if (err != NUMERUS_OK)
    {
        *formatted = '\0';  // Provide emtpy string output in case of error
        return err;
    }
    if (fraction.twelfths == 0 || fraction.int_part != 0)
    {
        // Write only the integer part, regardless of its value.
        // This covers the case when the fraction is zero and
        // when there is only an integer part.
        formatted += sprintf(formatted, FMTSTR_INTPART, fraction.int_part);
    }
    if (fraction.twelfths != 0)
    {
        if (fraction.int_part != 0)
        {
            // Something was written down for the integer part so far, so
            // add a separator between it and the twelfths.
            formatted += sprintf(formatted, FMTSTR_SEPARATOR);
        }
        // Write down the twelfths part
        int_fast8_t denominator = 12;
        simplify_fraction_smaller_than_1(&fraction.twelfths, &denominator);
        formatted += sprintf(formatted, FMTSTR_TWELFTHS,
                             fraction.twelfths, denominator);
    }
    *formatted = '\0';  // Always null terminate the output.
    return NUMERUS_OK;
}
