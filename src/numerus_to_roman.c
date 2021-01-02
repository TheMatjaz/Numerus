/**
 * @file
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


const char NUMERUS_ZERO_ROMAN[NUMERUS_ZERO_ROMAN_LEN_WITH_TERM] = "NULLA";

numerus_err_t
numerus_roman_from_int(char* numeral, int_fast32_t value)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    if (value < NUMERUS_BASIC_MIN || value > NUMERUS_BASIC_MAX)
    {
        *numeral = '\0';
        return NUMERUS_ERR_VALUE_OUT_OF_RANGE;
    }
    if (value == 0)
    {
        strcpy(numeral, NUMERUS_ZERO_ROMAN);
        return NUMERUS_OK;
    }
    if (value < 0)
    {
        *(numeral++) = '-';
        value = -value;
    }
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
    return NUMERUS_OK;
}

numerus_err_t
numerus_roman_from_double(char* numeral, double value)
{
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err = numerus_fraction_from_double(&fraction, value);
    if (err != NUMERUS_OK)
    {
        *numeral = '\0';
        return err;
    }
    return numerus_roman_from_fraction(numeral, fraction);
}

numerus_err_t
numerus_roman_from_fraction(
        char* numeral,
        numerus_frac_t fraction)
{
    if (numeral == NULL) { return NUMERUS_ERR_NULL_NUMERAL; }
    numerus_err_t err = numerus_fraction_simplify(&fraction);
    if (err != NUMERUS_OK)
    {
        *numeral = '\0';
        return err;
    }
    if (fraction.int_part == 0 && fraction.twelfths == 0)
    {
        strcpy(numeral, NUMERUS_ZERO_ROMAN);
        return NUMERUS_OK;
    }
    if (fraction.int_part < 0)
    {
        *(numeral++) = '-';
        fraction.int_part = -fraction.int_part;
    }
    if (fraction.int_part > NUMERUS_BASIC_MAX)
    {
        // Vinculum part
        *(numeral++) = '_';
        const int32_t vinculum_value = fraction.int_part / 1000;
        err = numerus_roman_from_int(numeral, vinculum_value);
        if (err != NUMERUS_OK)
        {
            *numeral = '\0';
            return err;
        }
        *(numeral++) = '_';
        fraction.int_part -= vinculum_value;
    }
    err = numerus_roman_from_int(numeral, fraction.int_part);
    if (err != NUMERUS_OK)
    {
        *numeral = '\0';
        return err;
    }
    if (fraction.twelfths >= 6)
    {
        *(numeral++) = 'S';
        fraction.twelfths -= 6;
    }
    if (fraction.twelfths >= 1)
    {
        *(numeral++) = '.';
        fraction.twelfths -= 1;
        if (fraction.twelfths >= 1)
        {
            *(numeral++) = '.';
            fraction.twelfths -= 1;
            if (fraction.twelfths >= 1)
            {
                *(numeral++) = '.';
                fraction.twelfths -= 1;
                if (fraction.twelfths >= 1)
                {
                    *(numeral++) = '.';
                    fraction.twelfths -= 1;
                    if (fraction.twelfths >= 1)
                    {
                        *(numeral++) = '.';
                        fraction.twelfths -= 1;
                    }
                }
            }
        }
    }
    *numeral = '\0';
    return NUMERUS_OK;
}
