/**
 * @file
 * @brief
 * @copyright Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "internal.h"


static bool contains_extended_characters(const char* numeral);


/**
 * @internal
 * Minimalistic extraction of an integer's sign into +1, 0 or -1.
 *
 * @param x an integer, also unsigned.
 */
static int8_t integer_sign(int32_t integer)
{
    return (integer > 0) - (integer < 0);
}

/**
* @internal
* Minimalistic extraction of a numeral's sign into +1 or -1.
*
* Does not skip initial whitespace. Does not check for #NUMERUS_ZERO_NUMERAL.
*/
static int8_t numeral_sign(const char* const numeral)
{
    if (*numeral == '-')
    {
        return -1;
    }
    else
    {
        return +1;
    }
}

void skip_head_whitespace(const char** p_string)
{
    while (isspace(**p_string))
    {
        (*p_string)++;
    }
    // TODO: what happens if there is too much whitespace?
}

/**
 * Verifies if the roman numeral is of value 0 (zero) without security checks.
 *
 * Ignores any leading minus. It's case INsensitive.
 *
 * @param *roman string containing the roman numeral.
 * @returns boolean: true if the numeral is NUMERUS_ZERO, false
 * otherwise.
 */
static bool numeral_is_zero_minus_ignored(const char* numeral)
{
    if (*numeral == '-')
    {
        numeral++;
    }
    return strncasecmp(numeral, NUMERUS_ZERO_NUMERAL, ZERO_NUMERAL_SIZE) == 0;
}


/**
 * Performs a security check and headtrimming of the roman numeral.
 *
 * Modifies the passed values themselves. Verifies if the *roman numeral is
 * NULL, if the *errcode is NULL, skips any heading whitespace of the roman
 * numeral, verifies if the roman numeral is empty. Otherwise stores NUMERUS_OK
 * in the *errcode.
 *
 * It is used as a preliminary security check by other functions
 *
 * @param **roman string containing the roman numeral.
 * @param **errcode int where to store the analysis status: NUMERUS_OK or any
 * other error.
 * @returns void as the result is stored in **errcode.
 */
numerus_status_t prepare_for_analysis(const char** p_numeral)
{
    if (*p_numeral == NULL)
    {
        return NUMERUS_ERROR_NULL_NUMERAL;
    }
    skip_head_whitespace(p_numeral);
    if (STRING_IS_EMPTY(*p_numeral))
    {
        return NUMERUS_ERROR_EMPTY_NUMERAL;
    }
    return NUMERUS_OK;
}


/** TODO doxygen
 * Verifies if the roman numeral is of value 0 (zero).
 *
 * Ignores any leading minus. It's case INsensitive.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the
 * error code is different than NUMERUS_OK, an error occurred during the
 * analysis and the returned value is false. The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns boolean: true if the numeral is NUMERUS_ZERO, false
 * otherwise.
 */
numerus_status_t numerus_is_zero(
        const char* numeral, bool* const p_result)
{
    numerus_status_t status;

    status = prepare_for_analysis(&numeral);
    if (status == NUMERUS_OK)
    {
        *p_result = numeral_is_zero_minus_ignored(numeral);
    }
    else
    {
        *p_result = false;
    }
    return status;
}

// TODO doxygen
numerus_status_t numerus_sign(
        const char* numeral, int8_t* const p_result)
{
    numerus_status_t status;

    status = prepare_for_analysis(&numeral);
    if (status == NUMERUS_OK)
    {
        if (numeral_is_zero_minus_ignored(numeral))
        {
            *p_result = 0;
        }
        else
        {
            *p_result = numeral_sign(numeral);
        }
    }
    else
    {
        *p_result = 0;
    }
    return status;
}

// TODO doxygen
/**
 * Verifies if the passed roman numeral is a long roman numeral (if contains a
 * correct number of underscores).
 *
 * Does **not** perform a syntax check or a value check, just searches for
 * underscores. Zero underscores means it's not a long roman numeral, two means
 * it is. Other numbers of underscores result in an error different than
 * NUMERUS_OK.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the
 * error code is different than NUMERUS_OK, an error occurred during the
 * analysis and the returned value is false. The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns boolean: true if the numeral is long, false
 * otherwise.
 */
numerus_status_t numerus_is_basic_numeral(
        const char* numeral, bool* const p_result)
{
    numerus_status_t status;

    status = prepare_for_analysis(&numeral);
    if (status == NUMERUS_OK)
    {
        *p_result = contains_extended_characters(numeral);
    }
    else
    {
        *p_result = false;
    }
    return status;
}

// TODO doxygen
bool contains_extended_characters(const char* numeral)
{
    while (*numeral != '\0')
    {
        switch (toupper(*numeral))
        {
            case '_':
            case 'S':
            case '.':
            {
                return true;
            }
            default:
            {
                numeral++;
            }
        }
    }
    return false;
}


// TODO doxygen
/**
 * Returns the number of roman characters in the roman numeral.
 *
 * Does **not** perform a syntax check or a value check, but **does** check for
 * illegal characters. The length value does not count the underscores or
 * whitespace.
 *
 * The analysis status is stored in the errcode passed as parameter, which
 * can be NULL to ignore the error, although it's not recommended. If the
 * error code is different than NUMERUS_OK, an error occurred during the
 * analysis and the returned value is negative. The error code may help find the
 * specific error.
 *
 * @param *roman string containing the roman numeral.
 * @param *errcode int8_t where to store the analysis status: NUMERUS_OK or any
 * other error. Can be NULL to ignore the error (NOT recommended).
 * @returns int8_t with the number of roman characters excluding underscores.
 */
numerus_status_t numerus_count_roman_chars(
        const char* numeral, uint8_t* const p_result)
{
    numerus_status_t status;
    uint8_t roman_chars_found;

    status = prepare_for_analysis(&numeral);
    if (status != NUMERUS_OK)
    {
        *p_result = 0;
        return status;
    }
    if (numeral_is_zero_minus_ignored(numeral))
    {
        *p_result = ZERO_NUMERAL_SIZE - 1;
        return status;
    }
    roman_chars_found = 0;
    while (*numeral != '\0')
    {
        switch (toupper(*numeral))
        {
            case '-':
            case 'M':
            case 'D':
            case 'C':
            case 'L':
            case 'X':
            case 'V':
            case 'I':
            case 'S':
            case '.':
            {
                numeral++;
                roman_chars_found++;
                break;
            }
            default:
            {
                if (isspace(*numeral))
                {
                    numeral++;
                }
                else
                {
                    status = NUMERUS_ERROR_ILLEGAL_EXTENDED_CHARACTER;
                    return status;
                }
            }
        }
        if (roman_chars_found > NUMERUS_MAX_EXTENDED_LENGTH)
        {
            status = NUMERUS_ERROR_TOO_LONG_EXTENDED_NUMERAL;
            return status;
        }
    }
    return status;
}
