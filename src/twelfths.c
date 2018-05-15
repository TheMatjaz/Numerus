/**
 * @file twelfths.c
 * @brief Implementation of fractional format handling functions.
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"
#include "math.h"


numerus_status_t numerus_simplify_twelfths(
        int32_t* const integer_part, int8_t* const twelfths)
{
    numerus_status_t status;

    if (integer_part == NULL || twelfths == NULL)
    {
        status = NUMERUS_ERROR_NULL_NUMBER;
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
        if (*integer_part < NUMERUS_MIN_EXTENDED_VALUE_INT_PART
            || *integer_part > NUMERUS_MAX_EXTENDED_VALUE_INT_PART)
        {
            status = NUMERUS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
        }
        else
        {
            status = NUMERUS_OK;
        }
    }
    return status;
}


numerus_status_t numerus_int_parts_to_double(
        const int32_t integer_part, const uint8_t twelfths,
        double* const result)
{
    double value;
    numerus_status_t status;

    value = integer_part + (twelfths / 12.0);
    if (value < NUMERUS_MIN_EXTENDED_VALUE
        || value > NUMERUS_MAX_EXTENDED_VALUE)
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


numerus_status_t numerus_double_to_int_parts(
        double value, int32_t* const integer_part, int8_t* twelfths)
{
    numerus_status_t status;

    if (!isfinite(value))
    {
        status = NUMERUS_ERROR_DOUBLE_VALUE_IS_NOT_FINITE;
    }
    else if (integer_part == NULL || twelfths == NULL)
    {
        status = NUMERUS_ERROR_NULL_NUMBER;
    }
    else if (value < NUMERUS_MIN_EXTENDED_VALUE
             || value > NUMERUS_MAX_EXTENDED_VALUE)
    {
        status = NUMERUS_ERROR_EXTENDED_VALUE_OUT_OF_RANGE;
    }
    else
    {
        *integer_part = (int32_t) value;
        value -= *integer_part;
        value = round(value * 12) / 12; /* Round fractional to nearest 12th */
        value = round(value * 12); /* Get numerator of that 12th */
        *twelfths = (int8_t) value;
        if (*twelfths == 12 || *twelfths == -12)
        {
            /* Rounding of +/- 0.99 gives +/- 12/12, we want 0/12 instead. */
            *integer_part += value / 12;
            *twelfths = 0;
        }
        status = NUMERUS_OK;
    }
    return status;
}
