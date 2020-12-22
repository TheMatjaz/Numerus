/**
 * @file
 * Implementation of fractional format handling functions.
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"

/*
numerus_err nmrs_simplify_twelfths(int32_t* integer_part, int8_t* twelfths)
{
    nmrs_err_t status;

    if (integer_part == NULL || twelfths == NULL)
    {
        status = NMRS_ERROR_NULL_NUMBER;
    }
    else
    {
        *integer_part += *twelfths / 12;
        *twelfths %= 12;
        if (*integer_part > 0 && *twelfths < 0)
        {
            *integer_part -= 1;
            *twelfths += 12;
        }
        else if (*integer_part < 0 && *twelfths > 0)
        {
            *integer_part += 1;
            *twelfths -= 12;
        }
        if (*integer_part < NMRS_MIN_EXTENDED_VALUE_INT_PART
            || *integer_part > NMRS_MAX_EXTENDED_VALUE_INT_PART)
        {
            status = NMRS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
        }
        else
        {
            status = NMRS_OK;
        }
    }
    return status;
}


nmrs_err_t numerus_int_parts_to_double(
        const int32_t integer_part, const uint8_t twelfths,
        double* const result)
{
    double value;
    nmrs_err_t status;

    value = integer_part + (twelfths / 12.0);
    if (value < NMRS_MIN_EXTENDED_VALUE
        || value > NMRS_MAX_EXTENDED_VALUE)
    {
        status = NMRS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
    }
    else
    {
        *result = value;
        status = NMRS_OK;
    }
    return status;
}


nmrs_err_t numerus_double_to_int_parts(
        double value, int32_t* const integer_part, int8_t* twelfths)
{
    nmrs_err_t status;

    if (!isfinite(value))
    {
        status = NMRS_ERROR_DOUBLE_VALUE_IS_NOT_FINITE;
    }
    else if (integer_part == NULL || twelfths == NULL)
    {
        status = NMRS_ERROR_NULL_NUMBER;
    }
    else if (value < NMRS_MIN_EXTENDED_VALUE
             || value > NMRS_MAX_EXTENDED_VALUE)
    {
        status = NMRS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
    }
    else
    {
        *integer_part = (int32_t) value;
        value -= *integer_part;
        value = round(value * 12) / 12; // Round fractional to nearest 12th
        value = round(value * 12); // Get numerator of that 12th
        *twelfths = (int8_t) value;
        if (*twelfths == 12 || *twelfths == -12)
        {
            // Rounding of +/- 0.99 gives +/- 12/12, we want 0/12 instead.
            *integer_part += value / 12;
            *twelfths = 0;
        }
        status = NMRS_OK;
    }
    return status;
}
*/
