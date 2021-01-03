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

    roman[0] = 'A';
    err = numerus_roman_from_double(NULL, 0);
    atto_eq(NUMERUS_ERR_NULL_NUMERAL, err);
    atto_streq(roman, "A", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_roman_from_double(NULL, NAN);
    atto_eq(NUMERUS_ERR_NULL_NUMERAL, err);
    atto_streq(roman, "A", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_roman_from_double(roman, NAN);
    atto_eq(NUMERUS_ERR_NOT_FINITE_DOUBLE, err);
    atto_streq(roman, "", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_roman_from_double(roman, +INFINITY);
    atto_eq(NUMERUS_ERR_NOT_FINITE_DOUBLE, err);
    atto_streq(roman, "", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_roman_from_double(roman, -INFINITY);
    atto_eq(NUMERUS_ERR_NOT_FINITE_DOUBLE, err);
    atto_streq(roman, "", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_roman_from_double(roman, NUMERUS_MIN_DOUBLE - 0.5 / 12.0);
    atto_eq(NUMERUS_ERR_VALUE_OUT_OF_RANGE, err);
    atto_streq(roman, "", NUMERUS_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_roman_from_double(roman, NUMERUS_MAX_DOUBLE + 0.5 / 12.0);
    atto_eq(NUMERUS_ERR_VALUE_OUT_OF_RANGE, err);
    atto_streq(roman, "", NUMERUS_MAX_LEN_WITH_TERM);
}


/**
 * @internal
 * Compares the generated roman numeral with the expected one,
 * making the code of the tests_to_roman_valid*() test cases easier to
 * read.
 */
#define TO_ROMAN_FROM_DOUBLE_EQ(x, str) \
    do { \
        err = numerus_roman_from_double(roman, (x)); \
        atto_eq(err, NUMERUS_OK); \
        atto_streq(roman, (str), NUMERUS_MAX_LEN_WITH_TERM-1); \
        atto_eq(roman[NUMERUS_MAX_LEN_WITH_TERM], CANARY); \
    } while(0)


static void test_to_roman_valid_some_positives(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;

    TO_ROMAN_FROM_DOUBLE_EQ(0.0, "NULLA");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 / 12, ".");
    TO_ROMAN_FROM_DOUBLE_EQ(2.0 / 12, "..");
    TO_ROMAN_FROM_DOUBLE_EQ(3.0 / 12, "...");
    TO_ROMAN_FROM_DOUBLE_EQ(4.0 / 12, "....");
    TO_ROMAN_FROM_DOUBLE_EQ(5.0 / 12, ".....");
    TO_ROMAN_FROM_DOUBLE_EQ(6.0 / 12, "S");
    TO_ROMAN_FROM_DOUBLE_EQ(7.0 / 12, "S.");
    TO_ROMAN_FROM_DOUBLE_EQ(8.0 / 12, "S..");
    TO_ROMAN_FROM_DOUBLE_EQ(9.0 / 12, "S...");
    TO_ROMAN_FROM_DOUBLE_EQ(10.0 / 12, "S....");
    TO_ROMAN_FROM_DOUBLE_EQ(11.0 / 12, "S.....");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0, "I");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 1.0 / 12, "I.");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 2.0 / 12, "I..");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 3.0 / 12, "I...");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 4.0 / 12, "I....");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 5.0 / 12, "I.....");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 6.0 / 12, "IS");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 7.0 / 12, "IS.");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 8.0 / 12, "IS..");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 9.0 / 12, "IS...");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 10.0 / 12, "IS....");
    TO_ROMAN_FROM_DOUBLE_EQ(1.0 + 11.0 / 12, "IS.....");
    TO_ROMAN_FROM_DOUBLE_EQ(2.0, "II");
    TO_ROMAN_FROM_DOUBLE_EQ(2.5, "IIS");
    TO_ROMAN_FROM_DOUBLE_EQ(2.58, "IIS.");
    TO_ROMAN_FROM_DOUBLE_EQ(48.0, "XLVIII");
    TO_ROMAN_FROM_DOUBLE_EQ(48.1, "XLVIII.");
    TO_ROMAN_FROM_DOUBLE_EQ(48.33, "XLVIII....");
    TO_ROMAN_FROM_DOUBLE_EQ(49.0, "XLIX");
    TO_ROMAN_FROM_DOUBLE_EQ(49.92, "XLIXS.....");
    TO_ROMAN_FROM_DOUBLE_EQ(3000.5, "MMMS");
    TO_ROMAN_FROM_DOUBLE_EQ(3999.0 + 2.0 / 12, "MMMCMXCIX..");
    TO_ROMAN_FROM_DOUBLE_EQ(4000.0, "_IV_");
    TO_ROMAN_FROM_DOUBLE_EQ(4001.0 + 10.0 / 12, "_IV_IS....");
    TO_ROMAN_FROM_DOUBLE_EQ(4002.0 + 10.0 / 12, "_IV_IIS....");
    TO_ROMAN_FROM_DOUBLE_EQ(5000.0 + 10.0 / 12, "_V_S....");
    TO_ROMAN_FROM_DOUBLE_EQ(5555.0, "_V_DLV");
    TO_ROMAN_FROM_DOUBLE_EQ(5555.0 + 7.0 / 12, "_V_DLVS.");
    TO_ROMAN_FROM_DOUBLE_EQ(3900001.0, "_MMMCM_I");
    TO_ROMAN_FROM_DOUBLE_EQ(3900001.0 + 3.0 / 12, "_MMMCM_I...");
}

static void test_to_roman_valid_some_negatives(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;

    TO_ROMAN_FROM_DOUBLE_EQ(-0.0, "NULLA");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 / 12, "-.");
    TO_ROMAN_FROM_DOUBLE_EQ(-2.0 / 12, "-..");
    TO_ROMAN_FROM_DOUBLE_EQ(-3.0 / 12, "-...");
    TO_ROMAN_FROM_DOUBLE_EQ(-4.0 / 12, "-....");
    TO_ROMAN_FROM_DOUBLE_EQ(-5.0 / 12, "-.....");
    TO_ROMAN_FROM_DOUBLE_EQ(-6.0 / 12, "-S");
    TO_ROMAN_FROM_DOUBLE_EQ(-7.0 / 12, "-S.");
    TO_ROMAN_FROM_DOUBLE_EQ(-8.0 / 12, "-S..");
    TO_ROMAN_FROM_DOUBLE_EQ(-9.0 / 12, "-S...");
    TO_ROMAN_FROM_DOUBLE_EQ(-10.0 / 12, "-S....");
    TO_ROMAN_FROM_DOUBLE_EQ(-11.0 / 12, "-S.....");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0, "-I");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 1.0 / 12, "-I.");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 2.0 / 12, "-I..");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 3.0 / 12, "-I...");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 4.0 / 12, "-I....");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 5.0 / 12, "-I.....");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 6.0 / 12, "-IS");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 7.0 / 12, "-IS.");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 8.0 / 12, "-IS..");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 9.0 / 12, "-IS...");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 10.0 / 12, "-IS....");
    TO_ROMAN_FROM_DOUBLE_EQ(-1.0 - 11.0 / 12, "-IS.....");
    TO_ROMAN_FROM_DOUBLE_EQ(-2.0, "-II");
    TO_ROMAN_FROM_DOUBLE_EQ(-2.5, "-IIS");
    TO_ROMAN_FROM_DOUBLE_EQ(-2.58, "-IIS.");
    TO_ROMAN_FROM_DOUBLE_EQ(-48.0, "-XLVIII");
    TO_ROMAN_FROM_DOUBLE_EQ(-48.1, "-XLVIII.");
    TO_ROMAN_FROM_DOUBLE_EQ(-48.33, "-XLVIII....");
    TO_ROMAN_FROM_DOUBLE_EQ(-49.0, "-XLIX");
    TO_ROMAN_FROM_DOUBLE_EQ(-49.92, "-XLIXS.....");
    TO_ROMAN_FROM_DOUBLE_EQ(-3000.5, "-MMMS");
    TO_ROMAN_FROM_DOUBLE_EQ(-3999.0 - 2.0 / 12, "-MMMCMXCIX..");
    TO_ROMAN_FROM_DOUBLE_EQ(-4000.0, "-_IV_");
    TO_ROMAN_FROM_DOUBLE_EQ(-4001.0 - 10.0 / 12, "-_IV_IS....");
    TO_ROMAN_FROM_DOUBLE_EQ(-4002.0 - 10.0 / 12, "-_IV_IIS....");
    TO_ROMAN_FROM_DOUBLE_EQ(-5000.0 - 10.0 / 12, "-_V_S....");
    TO_ROMAN_FROM_DOUBLE_EQ(-5555.0, "-_V_DLV");
    TO_ROMAN_FROM_DOUBLE_EQ(-5555.0 - 7.0 / 12, "-_V_DLVS.");
    TO_ROMAN_FROM_DOUBLE_EQ(-3900001.0, "-_MMMCM_I");
    TO_ROMAN_FROM_DOUBLE_EQ(-3900001.0 - 3.0 / 12, "-_MMMCM_I...");
}

static void test_to_roman_valid_extremes(void)
{
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;

    TO_ROMAN_FROM_DOUBLE_EQ(NUMERUS_MIN_INT_CLASSIC, "-MMMCMXCIX");
    TO_ROMAN_FROM_DOUBLE_EQ(NUMERUS_MAX_INT_CLASSIC, "MMMCMXCIX");

    // Longest integer numeral (without twelfths part)
    TO_ROMAN_FROM_DOUBLE_EQ(-3888888, "-_MMMDCCCLXXXVIII_DCCCLXXXVIII");
    TO_ROMAN_FROM_DOUBLE_EQ(3888888, "_MMMDCCCLXXXVIII_DCCCLXXXVIII");

    TO_ROMAN_FROM_DOUBLE_EQ(NUMERUS_MIN_INT, "-_MMMCMXCIX_CMXCIX");
    TO_ROMAN_FROM_DOUBLE_EQ(NUMERUS_MAX_INT, "_MMMCMXCIX_CMXCIX");

    // Longest double numeral
    TO_ROMAN_FROM_DOUBLE_EQ(-3888888.0 - 11.49/12, "-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");
    TO_ROMAN_FROM_DOUBLE_EQ(3888888 + 11.49/12, "_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");

    TO_ROMAN_FROM_DOUBLE_EQ(NUMERUS_MIN_DOUBLE, "-_MMMCMXCIX_CMXCIXS.....");
    TO_ROMAN_FROM_DOUBLE_EQ(NUMERUS_MAX_DOUBLE, "_MMMCMXCIX_CMXCIXS.....");
}

/**
 * @internal
 * Compares the allocated roman numeral with the expected one,
 * making the code of the tests_to_roman_alloc_valid*() test cases
 * easier to read.
 */
#define TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(x, str) \
    do { \
        err = numerus_roman_from_double_alloc(&roman, (x)); \
        atto_eq(err, NUMERUS_OK); \
        atto_streq((str), roman, NUMERUS_MAX_LEN_WITH_TERM-1); \
        free(roman); \
    } while(0)


static void test_to_roman_alloc_valid_some_positives(void)
{
    char* roman = NULL;
    numerus_err_t err;

    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(0.0, "NULLA");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 / 12, ".");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(2.0 / 12, "..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(3.0 / 12, "...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(4.0 / 12, "....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(5.0 / 12, ".....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(6.0 / 12, "S");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(7.0 / 12, "S.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(8.0 / 12, "S..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(9.0 / 12, "S...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(10.0 / 12, "S....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(11.0 / 12, "S.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0, "I");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 1.0 / 12, "I.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 2.0 / 12, "I..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 3.0 / 12, "I...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 4.0 / 12, "I....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 5.0 / 12, "I.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 6.0 / 12, "IS");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 7.0 / 12, "IS.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 8.0 / 12, "IS..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 9.0 / 12, "IS...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 10.0 / 12, "IS....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(1.0 + 11.0 / 12, "IS.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(2.0, "II");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(2.5, "IIS");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(2.58, "IIS.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(48.0, "XLVIII");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(48.1, "XLVIII.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(48.33, "XLVIII....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(49.0, "XLIX");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(49.92, "XLIXS.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(3000.5, "MMMS");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(3999.0 + 2.0 / 12, "MMMCMXCIX..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(4000.0, "_IV_");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(4001.0 + 10.3 / 12, "_IV_IS....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(4002.0 + 10.0 / 12, "_IV_IIS....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(5000.0 + 10.0 / 12, "_V_S....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(5555.0, "_V_DLV");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(5555.0 + 7.1 / 12, "_V_DLVS.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(3900001.0, "_MMMCM_I");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(3900001.0 + 2.899 / 12, "_MMMCM_I...");
}

static void test_to_roman_alloc_valid_some_negatives(void)
{
    char* roman = NULL;
    numerus_err_t err;

    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-0.0, "NULLA");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 / 12, "-.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-2.0 / 12, "-..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-3.0 / 12, "-...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-4.0 / 12, "-....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-5.0 / 12, "-.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-6.0 / 12, "-S");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-7.0 / 12, "-S.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-8.0 / 12, "-S..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-9.0 / 12, "-S...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-10.0 / 12, "-S....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-11.0 / 12, "-S.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0, "-I");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 1.0 / 12, "-I.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 2.0 / 12, "-I..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 3.0 / 12, "-I...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 4.0 / 12, "-I....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 5.0 / 12, "-I.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 6.0 / 12, "-IS");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 7.0 / 12, "-IS.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 8.0 / 12, "-IS..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 9.0 / 12, "-IS...");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 10.0 / 12, "-IS....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-1.0 - 11.0 / 12, "-IS.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-2.0, "-II");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-2.5, "-IIS");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-2.58, "-IIS.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-48.0, "-XLVIII");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-48.1, "-XLVIII.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-48.33, "-XLVIII....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-49.0, "-XLIX");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-49.92, "-XLIXS.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-3000.5, "-MMMS");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-3999.0 - 2.0 / 12, "-MMMCMXCIX..");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-4000.0, "-_IV_");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-4001.0 - 10.0 / 12, "-_IV_IS....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-4002.0 - 10.0 / 12, "-_IV_IIS....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-5000.0 - 10.0 / 12, "-_V_S....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-5555.0, "-_V_DLV");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-5555.0 - 7.0 / 12, "-_V_DLVS.");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-3900001.0, "-_MMMCM_I");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-3900001.0 - 3.0 / 12, "-_MMMCM_I...");
}


static void test_to_roman_alloc_valid_extremes(void)
{
    char* roman = NULL;
    numerus_err_t err;

    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(NUMERUS_MIN_INT_CLASSIC, "-MMMCMXCIX");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(NUMERUS_MAX_INT_CLASSIC, "MMMCMXCIX");

    // Longest integer numeral (without twelfths part)
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-3888888, "-_MMMDCCCLXXXVIII_DCCCLXXXVIII");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(3888888, "_MMMDCCCLXXXVIII_DCCCLXXXVIII");

    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(NUMERUS_MIN_INT, "-_MMMCMXCIX_CMXCIX");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(NUMERUS_MAX_INT, "_MMMCMXCIX_CMXCIX");

    // Longest double numeral
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(-3888888.0 - 11.3/12, "-_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(3888888 + 11.3/12, "_MMMDCCCLXXXVIII_DCCCLXXXVIIIS.....");

    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(NUMERUS_MIN_DOUBLE, "-_MMMCMXCIX_CMXCIXS.....");
    TO_ROMAN_FROM_DOUBLE_ALLOC_EQ(NUMERUS_MAX_DOUBLE, "_MMMCMXCIX_CMXCIXS.....");
}

static void test_to_roman_all_classic(void)
{
    char roman[NUMERUS_MAX_LEN_CLASSIC_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_CLASSIC_WITH_TERM] = CANARY;
    numerus_err_t err;

    for (int_fast32_t i = NUMERUS_MIN_INT_CLASSIC;
         i <= NUMERUS_MIN_INT_CLASSIC;
         i++)
    {
        err = numerus_roman_from_double(roman, i);
        atto_eq(err, NUMERUS_OK);
        atto_le(strlen(roman), NUMERUS_MAX_LEN_CLASSIC_WITH_TERM - 1);
        atto_eq(roman[NUMERUS_MAX_LEN_CLASSIC_WITH_TERM], CANARY);
    }
}

void test_roman_from_double(void)
{
    test_to_roman_invalid();
    test_to_roman_valid_some_positives();
    test_to_roman_valid_some_negatives();
    test_to_roman_valid_extremes();
    test_to_roman_alloc_valid_some_positives();
    test_to_roman_alloc_valid_some_negatives();
    test_to_roman_alloc_valid_extremes();
    test_to_roman_all_classic();
}
