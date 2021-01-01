/**
 * @file
 * Test cases for the string-formatting functions from numerus_fmt.c
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


static void test_fmt_overlined_invalid(void)
{
    char formatted[NUMERUS_OVERLINED_MAX_LEN_WITH_TERM];
    numerus_err_t err;

    err = numerus_fmt_overlined(NULL, "abc", true);
    atto_eq(err, NUMERUS_ERR_NULL_FORMATTED);

    err = numerus_fmt_overlined(formatted, NULL, true);
    atto_eq(err, NUMERUS_ERR_NULL_NUMERAL);
    atto_streq(formatted, "", NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);

    err = numerus_fmt_overlined(formatted, "_I", true);
    atto_eq(err, NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM);
    atto_streq(formatted, "_", NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
}

static void test_fmt_overlined_valid(void)
{
    char formatted[NUMERUS_OVERLINED_MAX_LEN_WITH_TERM];
    numerus_err_t err;

    // No effect for basic numerals
    err = numerus_fmt_overlined(formatted, "I", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq("I", formatted, NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined(formatted, "-MMMCCCXXXIII", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq("-MMMCCCXXXIII",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);

    // No effect for extended numerals without vinculum
    err = numerus_fmt_overlined(formatted, "S...", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq("S...", formatted, NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined(formatted, "-MMMCCCXXXIII..", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq("-MMMCCCXXXIII..",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);

    // Overlining for extended numerals with vinculum
    err = numerus_fmt_overlined(formatted, "_I_", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq("_\r\nI", formatted, NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined(formatted, "_M_CCCXXXIII", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq("_\nMCCCXXXIII",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined(formatted, "-_MM_CCCXXXIII", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq(" __\r\n-MMCCCXXXIII",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined(formatted, "-_MMM_CCCXXXIII", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq(" ___\n-MMMCCCXXXIII",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
}

static void test_fmt_overlined_alloc_invalid(void)
{
    char* formatted = NULL;
    numerus_err_t err;

    err = numerus_fmt_overlined_alloc(NULL, "abc", true);
    atto_eq(err, NUMERUS_ERR_NULL_FORMATTED);

    err = numerus_fmt_overlined_alloc(&formatted, NULL, true);
    atto_eq(err, NUMERUS_ERR_NULL_NUMERAL);
    atto_eq(formatted, NULL);

    err = numerus_fmt_overlined_alloc(&formatted, "_", true);
    atto_eq(err, NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM);
    atto_eq(formatted, NULL);
}

static void test_fmt_overlined_alloc_valid(void)
{
    char* formatted = NULL;
    numerus_err_t err;

    // No effect for basic numerals
    err = numerus_fmt_overlined_alloc(&formatted, "I", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq("I", formatted, NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined_alloc(&formatted, "-MMMCCCXXXIII", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq("-MMMCCCXXXIII",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    free(formatted);

    // No effect for extended numerals without vinculum
    err = numerus_fmt_overlined_alloc(&formatted, "S...", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq("S...", formatted, NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined_alloc(&formatted, "-MMMCCCXXXIII..", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq("-MMMCCCXXXIII..",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    free(formatted);

    // Overlining for extended numerals with vinculum
    err = numerus_fmt_overlined_alloc(&formatted, "_I_", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq("_\r\nI", formatted, NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    err = numerus_fmt_overlined_alloc(&formatted, "_M_CCCXXXIII...", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq("_\nMCCCXXXIII...",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    free(formatted);
    err = numerus_fmt_overlined_alloc(&formatted, "-_M_CCCXXXIIIS", true);
    atto_eq(err, NUMERUS_OK);
    atto_streq(" _\r\n-MCCCXXXIIIS",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    free(formatted);
    err = numerus_fmt_overlined_alloc(&formatted, "-_MMM_S.", false);
    atto_eq(err, NUMERUS_OK);
    atto_streq(" ___\n-MMMS.",
               formatted,
               NUMERUS_OVERLINED_MAX_LEN_WITH_TERM);
    free(formatted);
}


void test_fmt(void)
{
    test_fmt_overlined_invalid();
    test_fmt_overlined_valid();
    test_fmt_overlined_alloc_invalid();
    test_fmt_overlined_alloc_valid();
}
