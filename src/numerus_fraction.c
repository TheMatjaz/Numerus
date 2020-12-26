/**
 * @file
 * Implementation of fractional format handling functions.
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus.h"


static const int8_t TWELVE = 12;

#define OUT_OF_EXTENDED_INT_RANGE(x) \
    ((x) < NUMERUS_EXTENDED_INT_MIN || (x) > NUMERUS_EXTENDED_INT_MAX)
#define OUT_OF_EXTENDED_RANGE(x) \
    ((x) < NUMERUS_EXTENDED_MIN || (x) > NUMERUS_EXTENDED_MAX)

numerus_err_t
numerus_simplify_fraction(numerus_frac_t* const fraction)
{
    if (fraction == NULL) { return NUMERUS_ERR_NULL_FRACTION; }
    // Step 1: reduce the twelfts to a value in [-11, +11] converting anything
    // with more than +12/12 or less than -12/12 to integer parts.
    // Keep all the signs as they are.
    // Example: {-5, +14/12} => {-4, +2/12}
    fraction->int_part += fraction->twelfths / TWELVE;  // Integer division
    fraction->twelfths %= TWELVE;
    // Step 2: enforce same sign for int and fractional part.
    // Example: {-4, +2/12} => {-3, -10/12}
    if (fraction->int_part > 0 && fraction->twelfths < 0)
    {
        fraction->int_part -= 1;
        fraction->twelfths += TWELVE;
    }
    else if (fraction->int_part < 0 && fraction->twelfths > 0)
    {
        fraction->int_part += 1;
        fraction->twelfths -= TWELVE;
    }
    // Step 3: check if out of bounds after simplification.
    // The fractional part is already guaranteed to be in [-11, +11].
    if (OUT_OF_EXTENDED_INT_RANGE(fraction->int_part))
    {
        return NUMERUS_ERR_VALUE_OUT_OF_RANGE;
    }
    return NUMERUS_OK;
}


numerus_err_t numerus_fraction_to_double(
        double* const result,
        const numerus_frac_t fraction)
{
    if (result == NULL) { return NUMERUS_ERR_NULL_DOUBLE; }
    *result = NAN;  // Unusable result in case of errors after this line
    double local_result = fraction.twelfths;
    local_result /= TWELVE;  // True division
    local_result += fraction.int_part;
    if (OUT_OF_EXTENDED_RANGE(local_result))
    {
        return NUMERUS_ERR_VALUE_OUT_OF_RANGE;
    }
    *result = local_result;
    return NUMERUS_OK;
}

double numerus_fraction_to_double_noerr(const numerus_frac_t fraction)
{
    double result = NAN;
    numerus_fraction_to_double(&result, fraction);
    return result;
}

numerus_err_t numerus_double_to_fraction(
        numerus_frac_t* const fraction,
        const double real)
{
    //Todo accept 0.5/12.0 above the limit as its rounded down
    if (fraction == NULL) { return NUMERUS_ERR_NULL_FRACTION; }
    if (!isfinite(real)) { return NUMERUS_ERR_NOT_FINITE_DOUBLE; }
    if (OUT_OF_EXTENDED_RANGE(real))
    {
        return NUMERUS_ERR_VALUE_OUT_OF_RANGE;
    }
    fraction->int_part = (int32_t) real;
    fraction->twelfths = (int32_t) round((real - fraction->int_part) * TWELVE);
    return numerus_simplify_fraction(fraction);
}
