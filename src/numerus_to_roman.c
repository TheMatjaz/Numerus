/**
 * @file
 * Conversions of integer, floats and fractional value to roman numeral
 * strings without heap-allocation.
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


/** Multiplier of the value of the roman characters within the vinculum. */
#define VINCULUM_MULTIPLIER 1000

const char NUMERUS_ZERO_ROMAN[NUMERUS_ZERO_ROMAN_LEN_WITH_TERM] = "NULLA";

/**
 * @internal
 * Converts an integer part of a value to a roman numeral.
 *
 * @warning has no security checks, internal usage only.
 *
 * @param [out] numeral where to write the roman numeral string
 * @param [in] value integer part of the value to convert to roman numeral
 * @return position of the null-terminator written in \p numeral
 */
static char* build_int_part(char* numeral, int32_t value)
{
    if (value >= 1000)
    {
        *(numeral++) = 'M';
        value -= 1000;
        if (value >= 1000)
        {
            *(numeral++) = 'M';
            value -= 1000;
            if (value >= 1000)
            {
                *(numeral++) = 'M';
                value -= 1000;
            }
        }
    }
    if (value >= 900)
    {
        *(numeral++) = 'C';
        *(numeral++) = 'M';
        value -= 900;
    }
    if (value >= 500)
    {
        *(numeral++) = 'D';
        value -= 500;
    }
    if (value >= 400)
    {
        *(numeral++) = 'C';
        *(numeral++) = 'D';
        value -= 400;
    }
    if (value >= 100)
    {
        *(numeral++) = 'C';
        value -= 100;
        if (value >= 100)
        {
            *(numeral++) = 'C';
            value -= 100;
            if (value >= 100)
            {
                *(numeral++) = 'C';
                value -= 100;
            }
        }
    }
    if (value >= 90)
    {
        *(numeral++) = 'X';
        *(numeral++) = 'C';
        value -= 90;
    }
    if (value >= 50)
    {
        *(numeral++) = 'L';
        value -= 50;
    }
    if (value >= 40)
    {
        *(numeral++) = 'X';
        *(numeral++) = 'L';
        value -= 40;
    }
    if (value >= 10)
    {
        *(numeral++) = 'X';
        value -= 10;
        if (value >= 10)
        {
            *(numeral++) = 'X';
            value -= 10;
            if (value >= 10)
            {
                *(numeral++) = 'X';
                value -= 10;
            }
        }
    }
    if (value >= 9)
    {
        *(numeral++) = 'I';
        *(numeral++) = 'X';
        value -= 9;
    }
    if (value >= 5)
    {
        *(numeral++) = 'V';
        value -= 5;
    }
    if (value >= 4)
    {
        *(numeral++) = 'I';
        *(numeral++) = 'V';
        value -= 4;
    }
    if (value >= 1)
    {
        *(numeral++) = 'I';
        value -= 1;
        if (value >= 1)
        {
            *(numeral++) = 'I';
            value -= 1;
            if (value >= 1)
            {
                *(numeral++) = 'I';
                value -= 1;
            }
        }
    }
    *numeral = '\0';
    return numeral;
}

/**
 * @internal
 * Converts an fractional part (twelfths) of a value to a roman numeral.
 *
 * @warning has no security checks, internal usage only.
 *
 * @param [out] numeral where to write the roman numeral string
 * @param [in] value twelfths part of the value to convert to roman numeral
 * @return position of the null-terminator written in \p numeral
 */
static char* build_twelfths_part(char* numeral, int32_t twelfths)
{
    if (twelfths >= 6)
    {
        *(numeral++) = 'S';
        twelfths -= 6;
    }
    if (twelfths >= 1)
    {
        *(numeral++) = '.';
        twelfths -= 1;
        if (twelfths >= 1)
        {
            *(numeral++) = '.';
            twelfths -= 1;
            if (twelfths >= 1)
            {
                *(numeral++) = '.';
                twelfths -= 1;
                if (twelfths >= 1)
                {
                    *(numeral++) = '.';
                    twelfths -= 1;
                    if (twelfths >= 1)
                    {
                        *(numeral++) = '.';
                        twelfths -= 1;
                    }
                }
            }
        }
    }
    *numeral = '\0';
    return numeral;
}

numerus_err_t
numerus_roman_from_int(char* numeral, int32_t value)
{
    const numerus_frac_t fraction = {value, 0};
    return numerus_roman_from_fraction(numeral, fraction);
}

numerus_err_t
numerus_roman_from_double(char* numeral, double value)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    *numeral = '\0';  // Provide empty string in case of error
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err = numerus_fraction_from_double(&fraction, value);
    if (err != NUMERUS_OK) { return err; }
    return numerus_roman_from_fraction(numeral, fraction);
}

numerus_err_t
numerus_roman_from_fraction(
        char* numeral,
        numerus_frac_t fraction)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    *numeral = '\0';  // Provide empty string in case of error
    numerus_err_t err = numerus_fraction_simplify(&fraction);
    if (err != NUMERUS_OK) { return err; }
    if (fraction.int_part == 0 && fraction.twelfths == 0)
    {
        strcpy(numeral, NUMERUS_ZERO_ROMAN);
        return NUMERUS_OK;
    }
    if (fraction.int_part < 0 || fraction.twelfths < 0)
    {
        *(numeral++) = '-';
        fraction.int_part = -fraction.int_part;
        fraction.twelfths = -fraction.twelfths;
    }
    if (fraction.int_part > NUMERUS_MAX_INT_CLASSIC)
    {
        *(numeral++) = '_'; // Start the vinculum
        const int32_t vinculum_value = fraction.int_part / VINCULUM_MULTIPLIER;
        numeral = build_int_part(numeral, vinculum_value);
        fraction.int_part -= vinculum_value * VINCULUM_MULTIPLIER;
        *(numeral++) = '_'; // Terminate the vinculum
    }
    // Integer part after the vinculum
    numeral = build_int_part(numeral, fraction.int_part);
    // Twelfths part after the integer part
    build_twelfths_part(numeral, fraction.twelfths);
    return NUMERUS_OK;
}
