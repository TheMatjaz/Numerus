/**
 * @file
 * @brief
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


nmrs_err_t obtain_numeral_buffer(
        char** const p_result_numeral, const uint8_t size)
{
    nmrs_err_t status;

    status = NMRS_OK;
    if (p_result_numeral == NULL)
    {
        status = NMRS_ERROR_NULL_NUMERAL;
    }
        #ifdef NMRS_HAS_MALLOC
    else if (*p_result_numeral == NULL)
    {
        *p_result_numeral = nmrs_malloc(size);
        if (*p_result_numeral == NULL)
        {
            status = NMRS_ERROR_ALLOC_FAIL;
        }
    }
        #endif
    else
    {
        /* *p_result_numeral it's the location of the buffer. Nothing to do. */
    }
    return status;
}
