/**
 * @file
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


#define CANARY '@'

static void test_to_roman_invalid(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM] = {'A'};
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    roman[0] = 'A';
    err = numerus_roman_from_fraction(NULL, fraction);
    atto_eq(NUMERUS_ERR_NULL_NUMERAL, err);
    atto_streq(roman, "A", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    fraction.int_part = NUMERUS_MAX_INT + 1;
    err = numerus_roman_from_fraction(roman, fraction);
    atto_eq(NUMERUS_ERR_VALUE_OUT_OF_RANGE, err);
    atto_streq(roman, "", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    fraction.int_part = NUMERUS_MIN_INT;
    fraction.twelfths = -12;
    err = numerus_roman_from_fraction(roman, fraction);
    atto_eq(NUMERUS_ERR_VALUE_OUT_OF_RANGE, err);
    atto_streq(roman, "", NUMERUS_MAX_LEN_WITH_TERM);
}


/**
 * @internal
 * Compares the generated roman numeral with the expected one,
 * making the code of the tests_to_roman_valid*() test cases easier to
 * read.
 */
#define TO_ROMAN_FROM_FRACTION_EQ(i, tw, str) \
    do { \
        fraction.int_part = (i); \
        fraction.twelfths = (tw); \
        err = numerus_roman_from_fraction(roman, fraction); \
        atto_eq(err, NUMERUS_OK); \
        atto_streq(roman, (str), NUMERUS_MAX_LEN_WITH_TERM-1); \
        atto_eq(roman[NUMERUS_MAX_LEN_WITH_TERM], CANARY); \
    } while(0)


static void test_to_roman_valid_some_positives(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_EQ(0, 0, "NULLA");
    TO_ROMAN_FROM_FRACTION_EQ(0, 1, ".");
    TO_ROMAN_FROM_FRACTION_EQ(0, 2, "..");
    TO_ROMAN_FROM_FRACTION_EQ(0, 3, "...");
    TO_ROMAN_FROM_FRACTION_EQ(0, 4, "....");
    TO_ROMAN_FROM_FRACTION_EQ(0, 5, ".....");
    TO_ROMAN_FROM_FRACTION_EQ(0, 6, "S");
    TO_ROMAN_FROM_FRACTION_EQ(0, 7, "S.");
    TO_ROMAN_FROM_FRACTION_EQ(0, 8, "S..");
    TO_ROMAN_FROM_FRACTION_EQ(0, 9, "S...");
    TO_ROMAN_FROM_FRACTION_EQ(0, 10, "S....");
    TO_ROMAN_FROM_FRACTION_EQ(0, 11, "S.....");
    TO_ROMAN_FROM_FRACTION_EQ(1, 0, "I");
    TO_ROMAN_FROM_FRACTION_EQ(1, 1, "I.");
    TO_ROMAN_FROM_FRACTION_EQ(1, 2, "I..");
    TO_ROMAN_FROM_FRACTION_EQ(1, 3, "I...");
    TO_ROMAN_FROM_FRACTION_EQ(1, 4, "I....");
    TO_ROMAN_FROM_FRACTION_EQ(1, 5, "I.....");
    TO_ROMAN_FROM_FRACTION_EQ(1, 6, "IS");
    TO_ROMAN_FROM_FRACTION_EQ(1, 7, "IS.");
    TO_ROMAN_FROM_FRACTION_EQ(1, 8, "IS..");
    TO_ROMAN_FROM_FRACTION_EQ(1, 9, "IS...");
    TO_ROMAN_FROM_FRACTION_EQ(1, 10, "IS....");
    TO_ROMAN_FROM_FRACTION_EQ(1, 11, "IS.....");
    TO_ROMAN_FROM_FRACTION_EQ(2, 0, "II");
    TO_ROMAN_FROM_FRACTION_EQ(2, 6, "IIS");
    TO_ROMAN_FROM_FRACTION_EQ(2, 7, "IIS.");
    TO_ROMAN_FROM_FRACTION_EQ(48, 0, "XLVIII");
    TO_ROMAN_FROM_FRACTION_EQ(48, 1, "XLVIII.");
    TO_ROMAN_FROM_FRACTION_EQ(48, 4, "XLVIII....");
    TO_ROMAN_FROM_FRACTION_EQ(49, 0, "XLIX");
    TO_ROMAN_FROM_FRACTION_EQ(49, 11, "XLIXS.....");
    TO_ROMAN_FROM_FRACTION_EQ(3000, 6, "MMMS");
    TO_ROMAN_FROM_FRACTION_EQ(3999, 2, "MMMCMXCIX..");
    TO_ROMAN_FROM_FRACTION_EQ(4000, 0, "_IV_");
    TO_ROMAN_FROM_FRACTION_EQ(4001, 10, "_IV_IS....");
    TO_ROMAN_FROM_FRACTION_EQ(4002, 10, "_IV_IIS....");
    TO_ROMAN_FROM_FRACTION_EQ(5000, 10, "_V_S....");
    TO_ROMAN_FROM_FRACTION_EQ(5555, 0, "_V_DLV");
    TO_ROMAN_FROM_FRACTION_EQ(5555, 7, "_V_DLVS.");
    TO_ROMAN_FROM_FRACTION_EQ(3900001, 0, "_MMMCM_I");
    TO_ROMAN_FROM_FRACTION_EQ(3900001, 3, "_MMMCM_I...");
}

static void test_to_roman_valid_some_negatives(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_EQ(0, -0, "NULLA");
    TO_ROMAN_FROM_FRACTION_EQ(0, -1, "-.");
    TO_ROMAN_FROM_FRACTION_EQ(0, -2, "-..");
    TO_ROMAN_FROM_FRACTION_EQ(0, -3, "-...");
    TO_ROMAN_FROM_FRACTION_EQ(0, -4, "-....");
    TO_ROMAN_FROM_FRACTION_EQ(0, -5, "-.....");
    TO_ROMAN_FROM_FRACTION_EQ(0, -6, "-S");
    TO_ROMAN_FROM_FRACTION_EQ(0, -7, "-S.");
    TO_ROMAN_FROM_FRACTION_EQ(0, -8, "-S..");
    TO_ROMAN_FROM_FRACTION_EQ(0, -9, "-S...");
    TO_ROMAN_FROM_FRACTION_EQ(0, -10, "-S....");
    TO_ROMAN_FROM_FRACTION_EQ(0, -11, "-S.....");
    TO_ROMAN_FROM_FRACTION_EQ(-1, 0, "-I");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -1, "-I.");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -2, "-I..");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -3, "-I...");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -4, "-I....");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -5, "-I.....");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -6, "-IS");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -7, "-IS.");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -8, "-IS..");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -9, "-IS...");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -10, "-IS....");
    TO_ROMAN_FROM_FRACTION_EQ(-1, -11, "-IS.....");
    TO_ROMAN_FROM_FRACTION_EQ(-2, 0, "-II");
    TO_ROMAN_FROM_FRACTION_EQ(-2, -6, "-IIS");
    TO_ROMAN_FROM_FRACTION_EQ(-2, -7, "-IIS.");
    TO_ROMAN_FROM_FRACTION_EQ(-48, 0, "-XLVIII");
    TO_ROMAN_FROM_FRACTION_EQ(-48, -1, "-XLVIII.");
    TO_ROMAN_FROM_FRACTION_EQ(-48, -4, "-XLVIII....");
    TO_ROMAN_FROM_FRACTION_EQ(-49, 0, "-XLIX");
    TO_ROMAN_FROM_FRACTION_EQ(-49, -11, "-XLIXS.....");
    TO_ROMAN_FROM_FRACTION_EQ(-3000, -6, "-MMMS");
    TO_ROMAN_FROM_FRACTION_EQ(-3999, -2, "-MMMCMXCIX..");
    TO_ROMAN_FROM_FRACTION_EQ(-4000, 0, "-_IV_");
    TO_ROMAN_FROM_FRACTION_EQ(-4001, -10, "-_IV_IS....");
    TO_ROMAN_FROM_FRACTION_EQ(-4002, -10, "-_IV_IIS....");
    TO_ROMAN_FROM_FRACTION_EQ(-5000, -10, "-_V_S....");
    TO_ROMAN_FROM_FRACTION_EQ(-5555, 0, "-_V_DLV");
    TO_ROMAN_FROM_FRACTION_EQ(-5555, -7, "-_V_DLVS.");
    TO_ROMAN_FROM_FRACTION_EQ(-3900001, 0, "-_MMMCM_I");
    TO_ROMAN_FROM_FRACTION_EQ(-3900001, -3, "-_MMMCM_I...");
}

static void test_to_roman_valid_extremes(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_EQ(NUMERUS_MIN_INT_CLASSIC, 0, "-MMMCMXCIX");
    TO_ROMAN_FROM_FRACTION_EQ(NUMERUS_MAX_INT_CLASSIC, 0, "MMMCMXCIX");

    // Longest integer numeral (without twelfths part)
    TO_ROMAN_FROM_FRACTION_EQ(-3888888, 0, "-_MMMDCCCLXXXVIII_DCCCLXXXVIII");
    TO_ROMAN_FROM_FRACTION_EQ(3888888, 0, "_MMMDCCCLXXXVIII_DCCCLXXXVIII");

    TO_ROMAN_FROM_FRACTION_EQ(NUMERUS_MIN_INT, 0, "-_MMMCMXCIX_CMXCIX");
    TO_ROMAN_FROM_FRACTION_EQ(NUMERUS_MAX_INT, 0, "_MMMCMXCIX_CMXCIX");

    // Longest double numeral
    TO_ROMAN_FROM_FRACTION_EQ(-3888888, -11,
                              "-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");
    TO_ROMAN_FROM_FRACTION_EQ(3888888, 11,
                              "_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");
}

static void test_to_roman_valid_fraction_simplification(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_EQ(0, 13, "I."); // == (1, 1)
    TO_ROMAN_FROM_FRACTION_EQ(-1, 12, "NULLA"); // == (0, 0)
    TO_ROMAN_FROM_FRACTION_EQ(-1, 2, "-S...."); // == (0, -10)
    TO_ROMAN_FROM_FRACTION_EQ(10, -59, "V."); // == (5, 1)
}

/**
 * @internal
 * Compares the allocated roman numeral with the expected one,
 * making the code of the tests_to_roman_alloc_valid*() test cases
 * easier to read.
 */
#define TO_ROMAN_FROM_FRACTION_ALLOC_EQ(i, tw, str) \
    do { \
        fraction.int_part = (i); \
        fraction.twelfths = (tw); \
        err = numerus_roman_from_fraction_alloc(&roman, fraction); \
        atto_eq(err, NUMERUS_OK); \
        atto_streq((str), roman, NUMERUS_MAX_LEN_WITH_TERM-1); \
        free(roman); \
    } while(0)


static void test_to_roman_alloc_valid_some_positives(void)
{
    char* roman = NULL;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 0, "NULLA");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 1, ".");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 2, "..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 3, "...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 4, "....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 5, ".....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 6, "S");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 7, "S.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 8, "S..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 9, "S...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 10, "S....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 11, "S.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 0, "I");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 1, "I.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 2, "I..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 3, "I...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 4, "I....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 5, "I.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 6, "IS");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 7, "IS.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 8, "IS..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 9, "IS...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 10, "IS....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(1, 11, "IS.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(2, 0, "II");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(2, 6, "IIS");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(2, 7, "IIS.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(48, 0, "XLVIII");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(48, 1, "XLVIII.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(48, 4, "XLVIII....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(49, 0, "XLIX");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(49, 11, "XLIXS.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(3000, 6, "MMMS");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(3999, 2, "MMMCMXCIX..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(4000, 0, "_IV_");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(4001, 10, "_IV_IS....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(4002, 10, "_IV_IIS....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(5000, 10, "_V_S....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(5555, 0, "_V_DLV");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(5555, 7, "_V_DLVS.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(3900001, 0, "_MMMCM_I");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(3900001, 3, "_MMMCM_I...");
}

static void test_to_roman_alloc_valid_some_negatives(void)
{
    char* roman = NULL;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-0, -0, "NULLA");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -1, "-.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -2, "-..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -3, "-...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -4, "-....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -5, "-.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -6, "-S");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -7, "-S.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -8, "-S..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -9, "-S...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -10, "-S....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, -11, "-S.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, 0, "-I");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -1, "-I.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -2, "-I..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -3, "-I...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -4, "-I....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -5, "-I.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -6, "-IS");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -7, "-IS.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -8, "-IS..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -9, "-IS...");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -10, "-IS....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, -11, "-IS.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-2, 0, "-II");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-2, -6, "-IIS");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-2, -7, "-IIS.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-48, 0, "-XLVIII");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-48, -1, "-XLVIII.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-48, -4, "-XLVIII....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-49, 0, "-XLIX");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-49, -11, "-XLIXS.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-3000, -6, "-MMMS");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-3999, -2, "-MMMCMXCIX..");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-4000, 0, "-_IV_");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-4001, -10, "-_IV_IS....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-4002, -10, "-_IV_IIS....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-5000, -10, "-_V_S....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-5555, 0, "-_V_DLV");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-5555, -7, "-_V_DLVS.");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-3900001, 0, "-_MMMCM_I");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-3900001, -3, "-_MMMCM_I...");
}


static void test_to_roman_alloc_valid_extremes(void)
{
    char* roman = NULL;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(NUMERUS_MIN_INT_CLASSIC, 0, "-MMMCMXCIX");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(NUMERUS_MAX_INT_CLASSIC, 0, "MMMCMXCIX");

    // Longest integer numeral (without twelfths part)
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-3888888, 0,
                                    "-_MMMDCCCLXXXVIII_DCCCLXXXVIII");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(3888888,
                                    0,
                                    "_MMMDCCCLXXXVIII_DCCCLXXXVIII");

    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(NUMERUS_MIN_INT, 0, "-_MMMCMXCIX_CMXCIX");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(NUMERUS_MAX_INT, 0, "_MMMCMXCIX_CMXCIX");

    // Longest double numeral
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-3888888, -11,
                                    "-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(3888888,
                                    11,
                                    "_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");
}


static void test_to_roman_alloc_valid_fraction_simplification(void)
{
    char* roman = NULL;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(0, 13, "I."); // == (1, 1)
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, 12, "NULLA"); // == (0, 0)
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(-1, 2, "-S...."); // == (0, -10)
    TO_ROMAN_FROM_FRACTION_ALLOC_EQ(10, -59, "V."); // == (5, 1)
}

static void test_to_roman_all_classic(void)
{
    char roman[NUMERUS_MAX_LEN_CLASSIC_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_CLASSIC_WITH_TERM] = CANARY;
    numerus_err_t err;
    numerus_frac_t fraction = {0, 0};

    for (int_fast32_t i = NUMERUS_MIN_INT_CLASSIC;
         i <= NUMERUS_MIN_INT_CLASSIC;
         i++)
    {
        fraction.int_part = i;
        err = numerus_roman_from_fraction(roman, fraction);
        atto_eq(err, NUMERUS_OK);
        atto_le(strlen(roman), NUMERUS_MAX_LEN_CLASSIC_WITH_TERM - 1);
        atto_eq(roman[NUMERUS_MAX_LEN_CLASSIC_WITH_TERM], CANARY);
    }
}

void test_roman_from_fraction(void)
{
    test_to_roman_invalid();
    test_to_roman_valid_some_positives();
    test_to_roman_valid_some_negatives();
    test_to_roman_valid_extremes();
    test_to_roman_valid_fraction_simplification();
    test_to_roman_alloc_valid_some_positives();
    test_to_roman_alloc_valid_some_negatives();
    test_to_roman_alloc_valid_extremes();
    test_to_roman_alloc_valid_fraction_simplification();
    test_to_roman_all_classic();
}
