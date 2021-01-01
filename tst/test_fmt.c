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

static void test_fmt_fraction_invalid(void)
{
    char formatted[NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM];
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    err = numerus_fmt_fraction(NULL, fraction);
    atto_eq(err, NUMERUS_ERR_NULL_FORMATTED);

    fraction.int_part = 50000000;
    fraction.twelfths = 0;
    err = numerus_fmt_fraction(formatted, fraction);
    atto_eq(err, NUMERUS_ERR_VALUE_OUT_OF_RANGE);
}

/**
 * @internal
 * Compares the formatting of the given fraction with the expected one,
 * making the code of the test_fmt_fraction_*() test cases easier to read.
 */
#define FRAC_FMT_EQ(ints, tw, expected) \
     do { \
        fraction.int_part = (ints); \
        fraction.twelfths = (tw); \
        err = numerus_fmt_fraction(formatted, fraction); \
        atto_eq(err, NUMERUS_OK); \
        atto_streq(formatted, (expected), \
            NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM); \
    } while(0)


static void test_fmt_fraction_valid_twelfths_only(void)
{
    char formatted[NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM];
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    FRAC_FMT_EQ(0, 1, "1/12");
    FRAC_FMT_EQ(0, 2, "1/6");
    FRAC_FMT_EQ(0, 3, "1/4");
    FRAC_FMT_EQ(0, 4, "1/3");
    FRAC_FMT_EQ(0, 5, "5/12");
    FRAC_FMT_EQ(0, 6, "1/2");
    FRAC_FMT_EQ(0, 7, "7/12");
    FRAC_FMT_EQ(0, 8, "2/3");
    FRAC_FMT_EQ(0, 9, "3/4");
    FRAC_FMT_EQ(0, 10, "5/6");
    FRAC_FMT_EQ(0, 11, "11/12");
    FRAC_FMT_EQ(0, -1, "-1/12");
    FRAC_FMT_EQ(0, -2, "-1/6");
    FRAC_FMT_EQ(0, -3, "-1/4");
    FRAC_FMT_EQ(0, -4, "-1/3");
    FRAC_FMT_EQ(0, -5, "-5/12");
    FRAC_FMT_EQ(0, -6, "-1/2");
    FRAC_FMT_EQ(0, -7, "-7/12");
    FRAC_FMT_EQ(0, -8, "-2/3");
    FRAC_FMT_EQ(0, -9, "-3/4");
    FRAC_FMT_EQ(0, -10, "-5/6");
    FRAC_FMT_EQ(0, -11, "-11/12");
}

static void test_fmt_fraction_valid_int_part_only(void)
{
    char formatted[NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM];
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    FRAC_FMT_EQ(0, 0, "0");
    FRAC_FMT_EQ(1, 0, "1");
    FRAC_FMT_EQ(20, 0, "20");
    FRAC_FMT_EQ(-1, 0, "-1");
    FRAC_FMT_EQ(-20000, 0, "-20000");
}

static void test_fmt_fraction_valid_both_parts(void)
{
    char formatted[NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM];
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    FRAC_FMT_EQ(1, 2, "1, 1/6");
    FRAC_FMT_EQ(-10, -3, "-10, -1/4");
    FRAC_FMT_EQ(-20, 1, "-19, -11/12");
    FRAC_FMT_EQ(-1, 3, "-3/4");
    FRAC_FMT_EQ(20000, 30, "20002, 1/2");
    FRAC_FMT_EQ(-20000, -30, "-20002, -1/2");
    FRAC_FMT_EQ(20000, -30, "19997, 1/2");
    FRAC_FMT_EQ(-20000, 30, "-19997, -1/2");
}


static void test_fmt_fraction_alloc_invalid(void)
{
    char* formatted = NULL;
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    err = numerus_fmt_fraction_alloc(NULL, fraction);
    atto_eq(err, NUMERUS_ERR_NULL_FORMATTED);

    fraction.int_part = 50000000;
    fraction.twelfths = 0;
    err = numerus_fmt_fraction_alloc(&formatted, fraction);
    atto_eq(err, NUMERUS_ERR_VALUE_OUT_OF_RANGE);
}


/**
 * @internal
 * Compares the formatting of the given fraction with the expected one,
 * making the code of the test_fmt_fraction_*() test cases easier to read.
 * This one also frees the content at the end.
 */
#define FRAC_FMT_ALLOC_EQ(ints, tw, expected) \
    do { \
        fraction.int_part = (ints); \
        fraction.twelfths = (tw); \
        err = numerus_fmt_fraction_alloc(&formatted, fraction); \
        atto_eq(err, NUMERUS_OK); \
        atto_streq(formatted, (expected), \
            NUMERUS_FORMATTED_FRACTION_MAX_LEN_WITH_TERM); \
        free(formatted); \
    } while(0)


static void test_fmt_fraction_alloc_valid_twelfths_only(void)
{
    char* formatted = NULL;
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    FRAC_FMT_ALLOC_EQ(0, 1, "1/12");
    FRAC_FMT_ALLOC_EQ(0, 2, "1/6");
    FRAC_FMT_ALLOC_EQ(0, 3, "1/4");
    FRAC_FMT_ALLOC_EQ(0, 4, "1/3");
    FRAC_FMT_ALLOC_EQ(0, 5, "5/12");
    FRAC_FMT_ALLOC_EQ(0, 6, "1/2");
    FRAC_FMT_ALLOC_EQ(0, 7, "7/12");
    FRAC_FMT_ALLOC_EQ(0, 8, "2/3");
    FRAC_FMT_ALLOC_EQ(0, 9, "3/4");
    FRAC_FMT_ALLOC_EQ(0, 10, "5/6");
    FRAC_FMT_ALLOC_EQ(0, 11, "11/12");
    FRAC_FMT_ALLOC_EQ(0, -1, "-1/12");
    FRAC_FMT_ALLOC_EQ(0, -2, "-1/6");
    FRAC_FMT_ALLOC_EQ(0, -3, "-1/4");
    FRAC_FMT_ALLOC_EQ(0, -4, "-1/3");
    FRAC_FMT_ALLOC_EQ(0, -5, "-5/12");
    FRAC_FMT_ALLOC_EQ(0, -6, "-1/2");
    FRAC_FMT_ALLOC_EQ(0, -7, "-7/12");
    FRAC_FMT_ALLOC_EQ(0, -8, "-2/3");
    FRAC_FMT_ALLOC_EQ(0, -9, "-3/4");
    FRAC_FMT_ALLOC_EQ(0, -10, "-5/6");
    FRAC_FMT_ALLOC_EQ(0, -11, "-11/12");
}

static void test_fmt_fraction_alloc_valid_int_part_only(void)
{
    char* formatted = NULL;
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    FRAC_FMT_ALLOC_EQ(0, 0, "0");
    FRAC_FMT_ALLOC_EQ(1, 0, "1");
    FRAC_FMT_ALLOC_EQ(20, 0, "20");
    FRAC_FMT_ALLOC_EQ(-1, 0, "-1");
    FRAC_FMT_ALLOC_EQ(-20000, 0, "-20000");
}

static void test_fmt_fraction_alloc_valid_both_parts(void)
{
    char* formatted = NULL;
    numerus_frac_t fraction = {0, 0};
    numerus_err_t err;

    FRAC_FMT_ALLOC_EQ(1, 2, "1, 1/6");
    FRAC_FMT_ALLOC_EQ(-10, -3, "-10, -1/4");
    FRAC_FMT_ALLOC_EQ(-20, 1, "-19, -11/12");
    FRAC_FMT_ALLOC_EQ(-1, 3, "-3/4");
    FRAC_FMT_ALLOC_EQ(20000, 30, "20002, 1/2");
    FRAC_FMT_ALLOC_EQ(-20000, -30, "-20002, -1/2");
    FRAC_FMT_ALLOC_EQ(20000, -30, "19997, 1/2");
    FRAC_FMT_ALLOC_EQ(-20000, 30, "-19997, -1/2");
}


void test_fmt(void)
{
    test_fmt_overlined_invalid();
    test_fmt_overlined_valid();
    test_fmt_overlined_alloc_invalid();
    test_fmt_overlined_alloc_valid();
    test_fmt_fraction_invalid();
    test_fmt_fraction_valid_int_part_only();
    test_fmt_fraction_valid_twelfths_only();
    test_fmt_fraction_valid_both_parts();
    test_fmt_fraction_alloc_invalid();
    test_fmt_fraction_alloc_valid_int_part_only();
    test_fmt_fraction_alloc_valid_twelfths_only();
    test_fmt_fraction_alloc_valid_both_parts();
}
