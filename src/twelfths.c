/**
 * @file twelfths.c
 * @brief
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"
#include "math.h"



/**
 * Shortens the twelfths by adding the remainder to the int part so that they
 * have the same sign.
 *
 * It's useful when the twelfths are more than 11 (or -11 if negative). The
 * transformation is performed so that the parts will carry the same sign for
 * better readability.
 *
 * Example: `-3, 2` (= -3 + 2/12) becomes `-2, -10` (= -2 -10/12).
 *
 * Modifies the passed parameters themselves. The sign of the twelfth will be
 * the same of the int part (the int part leads) which is necessary also for a
 * correct conversion to of the parts to roman numeral.
 *
 * @param *int_part long integer part of a value.
 * @param *twelfths twelfth of the value.
 * @returns void since modifies the passed values.
 */
numerus_status_t numerus_simplify_twelfths(
        int32_t* const int_part, int8_t* const twelfths)
{
    if (int_part == NULL || twelfths == NULL)
    {
        return NUMERUS_ERROR_NULL_NUMBER;
    }
    *int_part += *twelfths / 12;
    *twelfths %= 12;
    if (*int_part > 0 && *twelfths < 0)
    {
        *int_part -= 1;
        *twelfths += 12;
    }
    else if (*int_part < 0 && *twelfths > 0)
    {
        *int_part += 1;
        *twelfths -= 12;
    }
    else
    {
        /* *int_part and *twelfths share sign
         * or *int_part is zero.
         * Nothing to do. */
    }
    return NUMERUS_OK;
}

/**
 * Converts a value expressed as sum of an integer part and a number of twelfths
 * to a double.
 *
 * @param int_part long integer part of a value to be added to the twelfths
 * and converted to double.
 * @param twelfths short integer as number of twelfths (1/12) to be added to the
 * integer part and converted to double.
 * @returns double value as the sum of the integer part and the twelfths.
 */
numerus_status_t numerus_int_parts_to_double(
        const int32_t int_part, const uint8_t twelfths, double* const result)
{
    numerus_status_t status;
    double value;

    value = int_part + (twelfths / 12.0);
    if (ABS(value) > NUMERUS_MAX_EXTENDED_VALUE)
    {
        status = NUMERUS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
    }
    else
    {
        *result = value;
        status = NUMERUS_OK;
    }
    return status;
}


/**
 * Splits a double value to a pair of its integer part and a number of twelfths.
 *
 * The twelfths are obtained by rounding the double value to the nearest
 * twelfths. The number of twelfths is stored in the passed parameter, while the
 * integer part is returned directly.
 *
 * @param value double to be split into integer part and number of twelfths.
 * @param *twelfths number of twelfths.
 * @returns long as the integer part of the value of the roman numeral.
 */
numerus_status_t numerus_double_to_int_parts(
        double value, int32_t* const int_part, int8_t* twelfths)
{

    numerus_status_t status;

    if (int_part == NULL || twelfths == NULL)
    {
        status = NUMERUS_ERROR_NULL_NUMBER;
    }
    else
    {
        *int_part = (int32_t) value;
        value -= *int_part;
        value = round(value * 12) / 12; /* Round fractional to nearest 12th */
        value = round(value * 12); /* Get numerator of that 12th */
        *twelfths = (int8_t) value;
        status = NUMERUS_OK;
    }
    return status;
}
