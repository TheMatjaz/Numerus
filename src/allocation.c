/**
 * @file
 * @brief
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


numerus_status_t obtain_numeral_buffer(
        char** const p_result_numeral, const uint8_t size)
{
    numerus_status_t status;

    status = NUMERUS_OK;
    if (p_result_numeral == NULL)
    {
        status = NUMERUS_ERROR_NULL_NUMERAL;
    }
    else if (*p_result_numeral == NULL)
    {
        *p_result_numeral = numerus_malloc(size);
        if (*p_result_numeral == NULL)
        {
            status = NUMERUS_ERROR_ALLOC_FAIL;
        }
    }
    else
    {
        /* *p_result_numeral it's the location of the buffer. Nothing to do. */
    }
    return status;
}
