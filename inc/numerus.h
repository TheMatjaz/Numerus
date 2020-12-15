/**
 * @file
 * @brief
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#ifndef NUMERUS_H_
#define NUMERUS_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * Version of the Numerus library using semantic versioning.
 *
 * <https://semver.org/spec/v2.0.0.html>
 */
#define NUMERUS_VERSION "v3.0.0"

#define NUMERUS_BASIC_MAX 3999
#define NUMERUS_BASIC_MIN -3999
#define NUMERUS_EXTENDED_MAX (3999999 + 11.5 / 12.0)
#define NUMERUS_EXTENDED_MIN (-3999999 - 11.5 / 12.0)
#define  NUMERUS_BASIC_MAX_LEN 17U
#define  NUMERUS_EXTENDED_MAX_LEN 37U
#define  NUMERUS_EXTENDED_OVERLINED_MAX_LEN 53U
#define  NUMERUS_ZERO_ROMAN_LEN 6U

typedef enum
{
    NUMERUS_OK = 0,
    NUMERUS_ERR_NULL_NUMERAL,
    NUMERUS_ERR_VALUE_OUT_OF_RANGE,
} numerus_err_t;

extern const char* const NUMERUS_ZERO_ROMAN;

numerus_err_t
numerus_to_roman(char* numeral, int_fast16_t value);

#ifdef __cplusplus
}
#endif

#endif  /* NUMERUS_H_ */