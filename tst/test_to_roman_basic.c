/**
 * @file
 * Test cases for the value to **basic** roman numeral conversion functions
 * from numerus_to_roman.c and numerus_to_roman_alloc.c
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


#define CANARY '@'
#define ATTO_CANARY_IS_INTACT() atto_eq(roman[NUMERUS_BASIC_MAX_LEN_WITH_TERM], CANARY)
#define EXPECTED_ROMANS_ELEMENTS 101

static const char* EXPECTED_ROMANS[EXPECTED_ROMANS_ELEMENTS] =
        {
                "NULLA",
                "I",
                "II",
                "III",
                "IV",
                "V",
                "VI",
                "VII",
                "VIII",
                "IX",
                "X",
                "XI",
                "XII",
                "XIII",
                "XIV",
                "XV",
                "XVI",
                "XVII",
                "XVIII",
                "XIX",
                "XX",
                "XXI",
                "XXII",
                "XXIII",
                "XXIV",
                "XXV",
                "XXVI",
                "XXVII",
                "XXVIII",
                "XXIX",
                "XXX",
                "XXXI",
                "XXXII",
                "XXXIII",
                "XXXIV",
                "XXXV",
                "XXXVI",
                "XXXVII",
                "XXXVIII",
                "XXXIX",
                "XL",
                "XLI",
                "XLII",
                "XLIII",
                "XLIV",
                "XLV",
                "XLVI",
                "XLVII",
                "XLVIII",
                "XLIX",
                "L",
                "LI",
                "LII",
                "LIII",
                "LIV",
                "LV",
                "LVI",
                "LVII",
                "LVIII",
                "LIX",
                "LX",
                "LXI",
                "LXII",
                "LXIII",
                "LXIV",
                "LXV",
                "LXVI",
                "LXVII",
                "LXVIII",
                "LXIX",
                "LXX",
                "LXXI",
                "LXXII",
                "LXXIII",
                "LXXIV",
                "LXXV",
                "LXXVI",
                "LXXVII",
                "LXXVIII",
                "LXXIX",
                "LXXX",
                "LXXXI",
                "LXXXII",
                "LXXXIII",
                "LXXXIV",
                "LXXXV",
                "LXXXVI",
                "LXXXVII",
                "LXXXVIII",
                "LXXXIX",
                "XC",
                "XCI",
                "XCII",
                "XCIII",
                "XCIV",
                "XCV",
                "XCVI",
                "XCVII",
                "XCVIII",
                "XCIX",
                "C",
        };


static void test_to_roman_basic_invalid(void)
{
    char roman[NUMERUS_BASIC_MAX_LEN_WITH_TERM] = {'A'};
    numerus_err_t err;

    roman[0] = 'A';
    err = numerus_to_roman(NULL, 0);
    atto_eq(NUMERUS_ERR_NULL_NUMERAL, err);
    atto_streq(roman, "A", NUMERUS_BASIC_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_to_roman(roman, NUMERUS_BASIC_MAX + 1);
    atto_eq(NUMERUS_ERR_VALUE_OUT_OF_RANGE, err);
    atto_streq(roman, "", NUMERUS_BASIC_MAX_LEN_WITH_TERM);

    roman[0] = 'A';
    err = numerus_to_roman(roman, NUMERUS_BASIC_MIN - 1);
    atto_eq(NUMERUS_ERR_VALUE_OUT_OF_RANGE, err);
    atto_streq(roman, "", NUMERUS_BASIC_MAX_LEN_WITH_TERM);
}

static void test_to_roman_basic_valid_few_basic(void)
{
    char roman[NUMERUS_BASIC_MAX_LEN_WITH_TERM];
    numerus_err_t err;

    err = numerus_to_roman(roman, 0);
    atto_eq(err, NUMERUS_OK);
    atto_streq("NULLA", roman, NUMERUS_ZERO_ROMAN_LEN_WITH_TERM-1);
    err = numerus_to_roman(roman, 1);
    atto_eq(err, NUMERUS_OK);
    atto_streq("I", roman, 1);
    err = numerus_to_roman(roman, 2);
    atto_eq(err, NUMERUS_OK);
    atto_streq("II", roman, 2);
    err = numerus_to_roman(roman, 4);
    atto_eq(err, NUMERUS_OK);
    atto_streq("IV", roman, 2);
    err = numerus_to_roman(roman, 9);
    atto_eq(err, NUMERUS_OK);
    atto_streq("IX", roman, 2);
    err = numerus_to_roman(roman, -9);
    atto_eq(err, NUMERUS_OK);
    atto_streq("-IX", roman, 2);
    err = numerus_to_roman(roman, NUMERUS_BASIC_MIN);
    atto_eq(err, NUMERUS_OK);
    atto_streq("-MMMCMXCIX", roman, 10);
    err = numerus_to_roman(roman, NUMERUS_BASIC_MAX);
    atto_eq(err, NUMERUS_OK);
    atto_streq("MMMCMXCIX", roman, 9);
}

static void test_to_roman_basic_valid_positives(void)
{
    char roman[NUMERUS_BASIC_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_BASIC_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;
    int_fast16_t i;

    for (i = 0; i < EXPECTED_ROMANS_ELEMENTS; i++)
    {
        err = numerus_to_roman(roman, i);
        atto_eq(err, NUMERUS_OK);
        atto_streq(EXPECTED_ROMANS[i], roman, NUMERUS_BASIC_MAX_LEN_WITH_TERM-1);
        ATTO_CANARY_IS_INTACT();
    }
}

static void test_to_roman_basic_valid_negatives(void)
{
    char roman[NUMERUS_BASIC_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_BASIC_MAX_LEN_WITH_TERM] = CANARY;
    numerus_err_t err;
    int_fast16_t i;

    for (i = 1; i < EXPECTED_ROMANS_ELEMENTS; i++)
    {
        err = numerus_to_roman(roman, -i);
        atto_eq(err, NUMERUS_OK);
        atto_eq('-', roman[0]);
        atto_streq(EXPECTED_ROMANS[i], &roman[1], NUMERUS_BASIC_MAX_LEN_WITH_TERM-1);
        ATTO_CANARY_IS_INTACT();
    }
}

static void test_to_roman_basic_valid_alloc(void)
{
    char* roman = NULL;
    numerus_err_t err;
    int_fast16_t i;

    for (i = 0; i < EXPECTED_ROMANS_ELEMENTS; i++)
    {
        err = numerus_to_roman_alloc(&roman, i);
        atto_eq(err, NUMERUS_OK);
        atto_neq(roman, NULL);
        atto_streq(EXPECTED_ROMANS[i], roman, NUMERUS_BASIC_MAX_LEN_WITH_TERM-1);
        free(roman);
        roman = NULL;
    }
    for (i = 1; i < EXPECTED_ROMANS_ELEMENTS; i++)
    {
        err = numerus_to_roman_alloc(&roman, -i);
        atto_eq(err, NUMERUS_OK);
        atto_neq(roman, NULL);
        atto_eq('-', roman[0]);
        atto_streq(EXPECTED_ROMANS[i], &roman[1], NUMERUS_BASIC_MAX_LEN_WITH_TERM-1);
        free(roman);
        roman = NULL;
    }
}


void test_to_roman_basic(void)
{
    test_to_roman_basic_invalid();
    test_to_roman_basic_valid_few_basic();
    test_to_roman_basic_valid_positives();
    test_to_roman_basic_valid_negatives();
    test_to_roman_basic_valid_alloc();
}
