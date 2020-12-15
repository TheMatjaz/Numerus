/**
 * @file
 *
 * @copyright Copyright © 2015-2020, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


#define CANARY '@'
#define ATTO_CANARY_IS_INTACT() atto_eq(roman[NUMERUS_BASIC_MAX_LEN], CANARY)
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

static void test_to_roman_valid_few_basic(void)
{
    char roman[NUMERUS_BASIC_MAX_LEN];
    numerus_err_t err;

    err = numerus_to_roman(roman, 0);
    atto_eq(err, NUMERUS_OK);
    atto_streq("NULLA", roman, NUMERUS_ZERO_ROMAN_LEN);
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
}

static void test_to_roman_valid_positives(void)
{
    char roman[NUMERUS_BASIC_MAX_LEN + 1];
    roman[NUMERUS_BASIC_MAX_LEN] = CANARY;
    numerus_err_t err;
    int_fast16_t i;

    for (i = 0; i < EXPECTED_ROMANS_ELEMENTS; i++)
    {
        err = numerus_to_roman(roman, i);
        atto_eq(err, NUMERUS_OK);
        atto_streq(EXPECTED_ROMANS[i], roman, strlen(EXPECTED_ROMANS[i]));
        ATTO_CANARY_IS_INTACT();
    }
}

static void test_to_roman_valid_negatives(void)
{
    char roman[NUMERUS_BASIC_MAX_LEN + 1];
    roman[NUMERUS_BASIC_MAX_LEN] = CANARY;
    numerus_err_t err;
    int_fast16_t i;

    for (i = 1; i < EXPECTED_ROMANS_ELEMENTS; i++)
    {
        err = numerus_to_roman(roman, -i);
        atto_eq(err, NUMERUS_OK);
        atto_eq('-', roman[0]);
        atto_streq(EXPECTED_ROMANS[i], &roman[1], strlen(EXPECTED_ROMANS[i]));
        ATTO_CANARY_IS_INTACT();
    }
}

void test_to_roman(void)
{
    test_to_roman_valid_few_basic();
    test_to_roman_valid_positives();
    test_to_roman_valid_negatives();
}
