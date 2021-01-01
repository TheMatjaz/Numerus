/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


/**
 * @internal
 * Advances the char pointer to the first non-ASCII whitespace in the string.
 */
#define SKIP_LEADING_WHITESPACE(str) { while(isspace(*(str))) { (str)++; } }

/**
 * @internal
 * Maximum value the roman numeral substring can have between the
 * end of the vinculum and the beginning of the fractional section ("S...")
 * (if any).
 *
 * After the vinculum, the 'M' character is not allowed, as it should be
 * represented as an 'I' in the vinculum instead.
 *
 * 999 = "CMXCIX"
 */
#define NUMERUS_POST_VINCULUM_MAX (+999)



const char* scan_basic(int32_t* const value, const char* numeral)
{
    // As Regex: "M{0,3}", case INsensitive
    if (toupper(*numeral) == 'M')
    {
        *value += 1000;
        numeral++;
        if (toupper(*numeral) == 'M')
        {
            *value += 1000;
            numeral++;
            if (toupper(*numeral) == 'M')
            {
                *value += 1000;
                numeral++;
            }
        }
    }
    // As Regex: "(CM)|(CD)|(D?C{0,3})", case INsensitive
    if (toupper(numeral[0]) == 'C' && toupper(numeral[1]) == 'M')
    {
        *value += 900;
        numeral += 2;
    }
    else if (toupper(numeral[0]) == 'C' && toupper(numeral[1]) == 'D')
    {
        *value += 400;
        numeral += 2;
    }
    else
    {
        if (toupper(*numeral) == 'D')
        {
            *value += 500;
            numeral++;
        }
        if (toupper(*numeral) == 'C')
        {
            *value += 100;
            numeral++;
            if (toupper(*numeral) == 'C')
            {
                *value += 100;
                numeral++;
                if (toupper(*numeral) == 'C')
                {
                    *value += 100;
                    numeral++;
                }
            }
        }
    }
    // As Regex: "(XC)|(XL)|(L?X{0,3})", case INsensitive
    if (toupper(numeral[0]) == 'X' && toupper(numeral[1]) == 'C')
    {
        *value += 90;
        numeral += 2;
    }
    else if (toupper(numeral[0]) == 'X' && toupper(numeral[1]) == 'L')
    {
        *value += 40;
        numeral += 2;
    }
    else
    {
        if (toupper(*numeral) == 'L')
        {
            *value += 50;
            numeral++;
        }
        if (toupper(*numeral) == 'X')
        {
            *value += 10;
            numeral++;
            if (toupper(*numeral) == 'X')
            {
                *value += 10;
                numeral++;
                if (toupper(*numeral) == 'X')
                {
                    *value += 10;
                    numeral++;
                }
            }
        }
    }
    // As Regex: "(IX)|(IV)|(V?I{0,3})", case INsensitive
    if (toupper(numeral[0]) == 'I' && toupper(numeral[1]) == 'X')
    {
        *value += 9;
        numeral += 2;
    }
    else if (toupper(numeral[0]) == 'I' && toupper(numeral[1]) == 'V')
    {
        *value += 4;
        numeral += 2;
    }
    else
    {
        if (toupper(*numeral) == 'V')
        {
            *value += 5;
            numeral++;
        }
        if (toupper(*numeral) == 'I')
        {
            *value += 1;
            numeral++;
            if (toupper(*numeral) == 'I')
            {
                *value += 1;
                numeral++;
                if (toupper(*numeral) == 'I')
                {
                    *value += 1;
                    numeral++;
                }
            }
        }
    }
    return numeral;
}


static const char*
scan_twelfts(int32_t* const twelfths, const char* numeral)
{
    // As Regex: "S?\.{0,5}", case INsensitive
    if (toupper(*numeral) == 'S')
    {
        *twelfths += 6U;  // 6/12 = 0.5
        numeral++;
    }
    if (*numeral == '.')
    {
        *twelfths += 1;  // 1/12 = 0.0833333
        numeral++;
        if (*numeral == '.')
        {
            *twelfths += 1;  // 1/12 = 0.0833333
            numeral++;
            if (*numeral == '.')
            {
                *twelfths += 1;  // 1/12 = 0.0833333
                numeral++;
                if (*numeral == '.')
                {
                    *twelfths += 1;  // 1/12 = 0.0833333
                    numeral++;
                    if (*numeral == '.')
                    {
                        *twelfths += 1;  // 1/12 = 0.0833333
                        numeral++;
                    }
                }
            }
        }
    }
    return numeral;
}

numerus_err_t
numerus_roman_to_int(int32_t* const value, const char* numeral)
{
    if (value == NULL) { return NUMERUS_ERR_NULL_INT; }
    *value = 0;
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    SKIP_LEADING_WHITESPACE(numeral);
    if (*numeral == '\0') { return NUMERUS_ERR_PARSING_EMPTY_NUMERAL; }
    if (numerus_is_zero(numeral)) { return NUMERUS_OK; }
    bool is_negative = false;
    if (*numeral == '-')
    {
        is_negative = true;
        numeral++;
    }
    numeral = scan_basic(value, numeral);
    // If at this point the string is over, we parsed successfully everything.
    // Otherwise an unexpected character was found (either unknown or
    // repeated too many times.
    if (*numeral != '\0') { return NUMERUS_ERR_PARSING_INVALID_SYNTAX; }
    if (is_negative) { *value = -(*value); }
    return NUMERUS_OK;
}


numerus_err_t numerus_roman_to_fraction(
        numerus_frac_t* fraction,
        const char* numeral)
{
    if (fraction == NULL) { return NUMERUS_ERR_NULL_FRACTION; }
    fraction->int_part = 0;
    fraction->twelfths = 0;
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    SKIP_LEADING_WHITESPACE(numeral);
    if (*numeral == '\0') { return NUMERUS_ERR_PARSING_EMPTY_NUMERAL; }
    if (numerus_is_zero(numeral)) { return NUMERUS_OK; }
    bool is_negative = false;
    if (*numeral == '-')
    {
        is_negative = true;
        numeral++;
    }
    int32_t int_part = 0;
    if (*numeral == '_')
    {
        numeral++;
        numeral = scan_basic(&int_part, numeral);
        int_part *= 1000;  // The vinculum is a 1000x multiplier
        if (*numeral != '_')
        {
            return NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM;
        }
    }
    numeral = scan_basic(&int_part, numeral);
    if (int_part > NUMERUS_POST_VINCULUM_MAX)
    {
        // After the vinculum, the 'M' char is not allowed.
        // If the value received is larger than 999 = "CMXCIX", then
        // an 'M' was used.
        return NUMERUS_ERR_PARSING_M_AFTER_VINCULUM;
    }
    int32_t twelfths = 0;
    numeral = scan_twelfts(&twelfths, numeral);
    // If at this point the string is over, we parsed successfully everything.
    // Otherwise an unexpected character was found (either unknown or
    // repeated too many times.
    if (*numeral != '\0') { return NUMERUS_ERR_PARSING_INVALID_SYNTAX; }
    if (is_negative)
    {
        int_part = -int_part;
        twelfths = -twelfths;
    }
    fraction->int_part = int_part;
    fraction->twelfths = twelfths;
    return NUMERUS_OK;
}

numerus_err_t
numerus_roman_to_double(double* const real, const char* numeral)
{
    numerus_frac_t fraction = {0, 0};
    numerus_err_t
            err = numerus_roman_to_fraction(&fraction, numeral);
    if (err != NUMERUS_OK) { return err; }
    return numerus_fraction_to_double(real, fraction);
}
