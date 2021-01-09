/**
 * @file
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


static void test_roman_to_int_invalid(void)
{
    int32_t value = 0;
    numerus_err_t err;

    err = numerus_roman_to_int(NULL, "I");
    atto_eq(err, NUMERUS_ERR_NULL_INT);

    err = numerus_roman_to_int(&value, NULL);
    atto_eq(err, NUMERUS_ERR_NULL_NUMERAL);

    err = numerus_roman_to_int(&value, "");
    atto_eq(err, NUMERUS_ERR_PARSING_EMPTY_NUMERAL);
    err = numerus_roman_to_int(&value, "-");
    atto_eq(err, NUMERUS_ERR_PARSING_EMPTY_NUMERAL);

    err = numerus_roman_to_int(
            &value,
            "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"
            "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    atto_eq(err, NUMERUS_ERR_PARSING_INVALID_SYNTAX);

    err = numerus_roman_to_int(&value, "IS");
    atto_eq(err, NUMERUS_ERR_PARSING_UNEXPECTED_TWELFTHS);

    err = numerus_roman_to_int(&value, "I.");
    atto_eq(err, NUMERUS_ERR_PARSING_UNEXPECTED_TWELFTHS);

    err = numerus_roman_to_int(&value, "_I_M");
    atto_eq(err, NUMERUS_ERR_PARSING_M_AFTER_VINCULUM);

    err = numerus_roman_to_int(&value, "_CI");
    atto_eq(err, NUMERUS_ERR_PARSING_NON_TERMINATED_VINCULUM);

    // Empty vinculum
    err = numerus_roman_to_int(&value, "__");
    atto_eq(err, NUMERUS_ERR_PARSING_EMPTY_VINCULUM);
    // Empty vinculum
    err = numerus_roman_to_int(&value, "__I");
    atto_eq(err, NUMERUS_ERR_PARSING_EMPTY_VINCULUM);
    // Wrong order
    err = numerus_roman_to_int(&value, "IC");
    atto_eq(err, NUMERUS_ERR_PARSING_INVALID_SYNTAX);
    // Too many repetitions
    err = numerus_roman_to_int(&value, "MMMM");
    atto_eq(err, NUMERUS_ERR_PARSING_INVALID_SYNTAX);
    // Too many repetitions
    err = numerus_roman_to_int(&value, "IVIV");
    atto_eq(err, NUMERUS_ERR_PARSING_INVALID_SYNTAX);
    // Impossible character
    err = numerus_roman_to_int(&value, "G");
    atto_eq(err, NUMERUS_ERR_PARSING_INVALID_SYNTAX);
    // Impossible character
    err = numerus_roman_to_int(&value, "--I");
    atto_eq(err, NUMERUS_ERR_PARSING_INVALID_SYNTAX);
    // Value character before vinculum
    err = numerus_roman_to_int(&value, "I_I_");
    atto_eq(err, NUMERUS_ERR_PARSING_INVALID_SYNTAX);
}

/**
 * @internal
 * Compares the value parsed from a roman numeral with the expected one.
 * making the code of the tests_roman_to_*() test cases easier to read.
 */
#define TO_ROMAN_TO_INT_EQ(x, str) \
    do { \
        err = numerus_roman_to_int(&value, (str)); \
        atto_eq(err, NUMERUS_OK); \
        atto_eq((x), value); \
    } while(0)

static void test_roman_to_int_valid_first_hundred_positives(void)
{
    int32_t value = 0;
    numerus_err_t err;

    TO_ROMAN_TO_INT_EQ(0, "NULLA");
    TO_ROMAN_TO_INT_EQ(1, "I");
    TO_ROMAN_TO_INT_EQ(2, "II");
    TO_ROMAN_TO_INT_EQ(3, "III");
    TO_ROMAN_TO_INT_EQ(4, "IV");
    TO_ROMAN_TO_INT_EQ(5, "V");
    TO_ROMAN_TO_INT_EQ(6, "VI");
    TO_ROMAN_TO_INT_EQ(7, "VII");
    TO_ROMAN_TO_INT_EQ(8, "VIII");
    TO_ROMAN_TO_INT_EQ(9, "IX");
    TO_ROMAN_TO_INT_EQ(10, "X");
    TO_ROMAN_TO_INT_EQ(11, "XI");
    TO_ROMAN_TO_INT_EQ(12, "XII");
    TO_ROMAN_TO_INT_EQ(13, "XIII");
    TO_ROMAN_TO_INT_EQ(14, "XIV");
    TO_ROMAN_TO_INT_EQ(15, "XV");
    TO_ROMAN_TO_INT_EQ(16, "XVI");
    TO_ROMAN_TO_INT_EQ(17, "XVII");
    TO_ROMAN_TO_INT_EQ(18, "XVIII");
    TO_ROMAN_TO_INT_EQ(19, "XIX");
    TO_ROMAN_TO_INT_EQ(20, "XX");
    TO_ROMAN_TO_INT_EQ(21, "XXI");
    TO_ROMAN_TO_INT_EQ(22, "XXII");
    TO_ROMAN_TO_INT_EQ(23, "XXIII");
    TO_ROMAN_TO_INT_EQ(24, "XXIV");
    TO_ROMAN_TO_INT_EQ(25, "XXV");
    TO_ROMAN_TO_INT_EQ(26, "XXVI");
    TO_ROMAN_TO_INT_EQ(27, "XXVII");
    TO_ROMAN_TO_INT_EQ(28, "XXVIII");
    TO_ROMAN_TO_INT_EQ(29, "XXIX");
    TO_ROMAN_TO_INT_EQ(30, "XXX");
    TO_ROMAN_TO_INT_EQ(31, "XXXI");
    TO_ROMAN_TO_INT_EQ(32, "XXXII");
    TO_ROMAN_TO_INT_EQ(33, "XXXIII");
    TO_ROMAN_TO_INT_EQ(34, "XXXIV");
    TO_ROMAN_TO_INT_EQ(35, "XXXV");
    TO_ROMAN_TO_INT_EQ(36, "XXXVI");
    TO_ROMAN_TO_INT_EQ(37, "XXXVII");
    TO_ROMAN_TO_INT_EQ(38, "XXXVIII");
    TO_ROMAN_TO_INT_EQ(39, "XXXIX");
    TO_ROMAN_TO_INT_EQ(40, "XL");
    TO_ROMAN_TO_INT_EQ(41, "XLI");
    TO_ROMAN_TO_INT_EQ(42, "XLII");
    TO_ROMAN_TO_INT_EQ(43, "XLIII");
    TO_ROMAN_TO_INT_EQ(44, "XLIV");
    TO_ROMAN_TO_INT_EQ(45, "XLV");
    TO_ROMAN_TO_INT_EQ(46, "XLVI");
    TO_ROMAN_TO_INT_EQ(47, "XLVII");
    TO_ROMAN_TO_INT_EQ(48, "XLVIII");
    TO_ROMAN_TO_INT_EQ(49, "XLIX");
    TO_ROMAN_TO_INT_EQ(50, "L");
    TO_ROMAN_TO_INT_EQ(51, "LI");
    TO_ROMAN_TO_INT_EQ(52, "LII");
    TO_ROMAN_TO_INT_EQ(53, "LIII");
    TO_ROMAN_TO_INT_EQ(54, "LIV");
    TO_ROMAN_TO_INT_EQ(55, "LV");
    TO_ROMAN_TO_INT_EQ(56, "LVI");
    TO_ROMAN_TO_INT_EQ(57, "LVII");
    TO_ROMAN_TO_INT_EQ(58, "LVIII");
    TO_ROMAN_TO_INT_EQ(59, "LIX");
    TO_ROMAN_TO_INT_EQ(60, "LX");
    TO_ROMAN_TO_INT_EQ(61, "LXI");
    TO_ROMAN_TO_INT_EQ(62, "LXII");
    TO_ROMAN_TO_INT_EQ(63, "LXIII");
    TO_ROMAN_TO_INT_EQ(64, "LXIV");
    TO_ROMAN_TO_INT_EQ(65, "LXV");
    TO_ROMAN_TO_INT_EQ(66, "LXVI");
    TO_ROMAN_TO_INT_EQ(67, "LXVII");
    TO_ROMAN_TO_INT_EQ(68, "LXVIII");
    TO_ROMAN_TO_INT_EQ(69, "LXIX");
    TO_ROMAN_TO_INT_EQ(70, "LXX");
    TO_ROMAN_TO_INT_EQ(71, "LXXI");
    TO_ROMAN_TO_INT_EQ(72, "LXXII");
    TO_ROMAN_TO_INT_EQ(73, "LXXIII");
    TO_ROMAN_TO_INT_EQ(74, "LXXIV");
    TO_ROMAN_TO_INT_EQ(75, "LXXV");
    TO_ROMAN_TO_INT_EQ(76, "LXXVI");
    TO_ROMAN_TO_INT_EQ(77, "LXXVII");
    TO_ROMAN_TO_INT_EQ(78, "LXXVIII");
    TO_ROMAN_TO_INT_EQ(79, "LXXIX");
    TO_ROMAN_TO_INT_EQ(80, "LXXX");
    TO_ROMAN_TO_INT_EQ(81, "LXXXI");
    TO_ROMAN_TO_INT_EQ(82, "LXXXII");
    TO_ROMAN_TO_INT_EQ(83, "LXXXIII");
    TO_ROMAN_TO_INT_EQ(84, "LXXXIV");
    TO_ROMAN_TO_INT_EQ(85, "LXXXV");
    TO_ROMAN_TO_INT_EQ(86, "LXXXVI");
    TO_ROMAN_TO_INT_EQ(87, "LXXXVII");
    TO_ROMAN_TO_INT_EQ(88, "LXXXVIII");
    TO_ROMAN_TO_INT_EQ(89, "LXXXIX");
    TO_ROMAN_TO_INT_EQ(90, "XC");
    TO_ROMAN_TO_INT_EQ(91, "XCI");
    TO_ROMAN_TO_INT_EQ(92, "XCII");
    TO_ROMAN_TO_INT_EQ(93, "XCIII");
    TO_ROMAN_TO_INT_EQ(94, "XCIV");
    TO_ROMAN_TO_INT_EQ(95, "XCV");
    TO_ROMAN_TO_INT_EQ(96, "XCVI");
    TO_ROMAN_TO_INT_EQ(97, "XCVII");
    TO_ROMAN_TO_INT_EQ(98, "XCVIII");
    TO_ROMAN_TO_INT_EQ(99, "XCIX");
    TO_ROMAN_TO_INT_EQ(100, "C");
}

static void test_roman_to_int_valid_first_hundred_negatives(void)
{
    int32_t value = 0;
    numerus_err_t err;

    TO_ROMAN_TO_INT_EQ(-0, "-NULLA");
    TO_ROMAN_TO_INT_EQ(-1, "-I");
    TO_ROMAN_TO_INT_EQ(-2, "-II");
    TO_ROMAN_TO_INT_EQ(-3, "-III");
    TO_ROMAN_TO_INT_EQ(-4, "-IV");
    TO_ROMAN_TO_INT_EQ(-5, "-V");
    TO_ROMAN_TO_INT_EQ(-6, "-VI");
    TO_ROMAN_TO_INT_EQ(-7, "-VII");
    TO_ROMAN_TO_INT_EQ(-8, "-VIII");
    TO_ROMAN_TO_INT_EQ(-9, "-IX");
    TO_ROMAN_TO_INT_EQ(-10, "-X");
    TO_ROMAN_TO_INT_EQ(-11, "-XI");
    TO_ROMAN_TO_INT_EQ(-12, "-XII");
    TO_ROMAN_TO_INT_EQ(-13, "-XIII");
    TO_ROMAN_TO_INT_EQ(-14, "-XIV");
    TO_ROMAN_TO_INT_EQ(-15, "-XV");
    TO_ROMAN_TO_INT_EQ(-16, "-XVI");
    TO_ROMAN_TO_INT_EQ(-17, "-XVII");
    TO_ROMAN_TO_INT_EQ(-18, "-XVIII");
    TO_ROMAN_TO_INT_EQ(-19, "-XIX");
    TO_ROMAN_TO_INT_EQ(-20, "-XX");
    TO_ROMAN_TO_INT_EQ(-21, "-XXI");
    TO_ROMAN_TO_INT_EQ(-22, "-XXII");
    TO_ROMAN_TO_INT_EQ(-23, "-XXIII");
    TO_ROMAN_TO_INT_EQ(-24, "-XXIV");
    TO_ROMAN_TO_INT_EQ(-25, "-XXV");
    TO_ROMAN_TO_INT_EQ(-26, "-XXVI");
    TO_ROMAN_TO_INT_EQ(-27, "-XXVII");
    TO_ROMAN_TO_INT_EQ(-28, "-XXVIII");
    TO_ROMAN_TO_INT_EQ(-29, "-XXIX");
    TO_ROMAN_TO_INT_EQ(-30, "-XXX");
    TO_ROMAN_TO_INT_EQ(-31, "-XXXI");
    TO_ROMAN_TO_INT_EQ(-32, "-XXXII");
    TO_ROMAN_TO_INT_EQ(-33, "-XXXIII");
    TO_ROMAN_TO_INT_EQ(-34, "-XXXIV");
    TO_ROMAN_TO_INT_EQ(-35, "-XXXV");
    TO_ROMAN_TO_INT_EQ(-36, "-XXXVI");
    TO_ROMAN_TO_INT_EQ(-37, "-XXXVII");
    TO_ROMAN_TO_INT_EQ(-38, "-XXXVIII");
    TO_ROMAN_TO_INT_EQ(-39, "-XXXIX");
    TO_ROMAN_TO_INT_EQ(-40, "-XL");
    TO_ROMAN_TO_INT_EQ(-41, "-XLI");
    TO_ROMAN_TO_INT_EQ(-42, "-XLII");
    TO_ROMAN_TO_INT_EQ(-43, "-XLIII");
    TO_ROMAN_TO_INT_EQ(-44, "-XLIV");
    TO_ROMAN_TO_INT_EQ(-45, "-XLV");
    TO_ROMAN_TO_INT_EQ(-46, "-XLVI");
    TO_ROMAN_TO_INT_EQ(-47, "-XLVII");
    TO_ROMAN_TO_INT_EQ(-48, "-XLVIII");
    TO_ROMAN_TO_INT_EQ(-49, "-XLIX");
    TO_ROMAN_TO_INT_EQ(-50, "-L");
    TO_ROMAN_TO_INT_EQ(-51, "-LI");
    TO_ROMAN_TO_INT_EQ(-52, "-LII");
    TO_ROMAN_TO_INT_EQ(-53, "-LIII");
    TO_ROMAN_TO_INT_EQ(-54, "-LIV");
    TO_ROMAN_TO_INT_EQ(-55, "-LV");
    TO_ROMAN_TO_INT_EQ(-56, "-LVI");
    TO_ROMAN_TO_INT_EQ(-57, "-LVII");
    TO_ROMAN_TO_INT_EQ(-58, "-LVIII");
    TO_ROMAN_TO_INT_EQ(-59, "-LIX");
    TO_ROMAN_TO_INT_EQ(-60, "-LX");
    TO_ROMAN_TO_INT_EQ(-61, "-LXI");
    TO_ROMAN_TO_INT_EQ(-62, "-LXII");
    TO_ROMAN_TO_INT_EQ(-63, "-LXIII");
    TO_ROMAN_TO_INT_EQ(-64, "-LXIV");
    TO_ROMAN_TO_INT_EQ(-65, "-LXV");
    TO_ROMAN_TO_INT_EQ(-66, "-LXVI");
    TO_ROMAN_TO_INT_EQ(-67, "-LXVII");
    TO_ROMAN_TO_INT_EQ(-68, "-LXVIII");
    TO_ROMAN_TO_INT_EQ(-69, "-LXIX");
    TO_ROMAN_TO_INT_EQ(-70, "-LXX");
    TO_ROMAN_TO_INT_EQ(-71, "-LXXI");
    TO_ROMAN_TO_INT_EQ(-72, "-LXXII");
    TO_ROMAN_TO_INT_EQ(-73, "-LXXIII");
    TO_ROMAN_TO_INT_EQ(-74, "-LXXIV");
    TO_ROMAN_TO_INT_EQ(-75, "-LXXV");
    TO_ROMAN_TO_INT_EQ(-76, "-LXXVI");
    TO_ROMAN_TO_INT_EQ(-77, "-LXXVII");
    TO_ROMAN_TO_INT_EQ(-78, "-LXXVIII");
    TO_ROMAN_TO_INT_EQ(-79, "-LXXIX");
    TO_ROMAN_TO_INT_EQ(-80, "-LXXX");
    TO_ROMAN_TO_INT_EQ(-81, "-LXXXI");
    TO_ROMAN_TO_INT_EQ(-82, "-LXXXII");
    TO_ROMAN_TO_INT_EQ(-83, "-LXXXIII");
    TO_ROMAN_TO_INT_EQ(-84, "-LXXXIV");
    TO_ROMAN_TO_INT_EQ(-85, "-LXXXV");
    TO_ROMAN_TO_INT_EQ(-86, "-LXXXVI");
    TO_ROMAN_TO_INT_EQ(-87, "-LXXXVII");
    TO_ROMAN_TO_INT_EQ(-88, "-LXXXVIII");
    TO_ROMAN_TO_INT_EQ(-89, "-LXXXIX");
    TO_ROMAN_TO_INT_EQ(-90, "-XC");
    TO_ROMAN_TO_INT_EQ(-91, "-XCI");
    TO_ROMAN_TO_INT_EQ(-92, "-XCII");
    TO_ROMAN_TO_INT_EQ(-93, "-XCIII");
    TO_ROMAN_TO_INT_EQ(-94, "-XCIV");
    TO_ROMAN_TO_INT_EQ(-95, "-XCV");
    TO_ROMAN_TO_INT_EQ(-96, "-XCVI");
    TO_ROMAN_TO_INT_EQ(-97, "-XCVII");
    TO_ROMAN_TO_INT_EQ(-98, "-XCVIII");
    TO_ROMAN_TO_INT_EQ(-99, "-XCIX");
    TO_ROMAN_TO_INT_EQ(-100, "-C");
}

static void test_roman_to_int_valid_some_extended(void)
{
    int32_t value = 0;
    numerus_err_t err;

    TO_ROMAN_TO_INT_EQ(3000, "MMM");
    TO_ROMAN_TO_INT_EQ(3999, "MMMCMXCIX");
    TO_ROMAN_TO_INT_EQ(4000, "_IV_");
    TO_ROMAN_TO_INT_EQ(4001, "_IV_I");
    TO_ROMAN_TO_INT_EQ(4002, "_IV_II");
    TO_ROMAN_TO_INT_EQ(5000, "_V_");
    TO_ROMAN_TO_INT_EQ(5555, "_V_DLV");
    TO_ROMAN_TO_INT_EQ(3900001, "_MMMCM_I");

    TO_ROMAN_TO_INT_EQ(-3000, "-MMM");
    TO_ROMAN_TO_INT_EQ(-3999, "-MMMCMXCIX");
    TO_ROMAN_TO_INT_EQ(-4000, "-_IV_");
    TO_ROMAN_TO_INT_EQ(-4001, "-_IV_I");
    TO_ROMAN_TO_INT_EQ(-4002, "-_IV_II");
    TO_ROMAN_TO_INT_EQ(-5000, "-_V_");
    TO_ROMAN_TO_INT_EQ(-5555, "-_V_DLV");
    TO_ROMAN_TO_INT_EQ(-3900001, "-_MMMCM_I");
}

static void test_roman_to_int_valid_extremes(void)
{
    int32_t value = 0;
    numerus_err_t err;

    TO_ROMAN_TO_INT_EQ(NUMERUS_MIN_INT_CLASSIC, "-MMMCMXCIX");
    TO_ROMAN_TO_INT_EQ(NUMERUS_MAX_INT_CLASSIC, "MMMCMXCIX");

    // Longest integer numeral (without twelfths part)
    TO_ROMAN_TO_INT_EQ(-3888888, "-_MMMDCCCLXXXVIII_DCCCLXXXVIII");
    TO_ROMAN_TO_INT_EQ(3888888, "_MMMDCCCLXXXVIII_DCCCLXXXVIII");

    TO_ROMAN_TO_INT_EQ(NUMERUS_MIN_INT, "-_MMMCMXCIX_CMXCIX");
    TO_ROMAN_TO_INT_EQ(NUMERUS_MAX_INT, "_MMMCMXCIX_CMXCIX");
}

static void test_roman_to_int_valid_all_classic(void)
{
    char roman[NUMERUS_MAX_LEN_CLASSIC_WITH_TERM];
    int32_t value = 0;
    numerus_err_t err;

    for (int32_t i = NUMERUS_MIN_INT_CLASSIC;
         i <= NUMERUS_MIN_INT_CLASSIC;
         i++)
    {
        err = numerus_roman_from_int(roman, i);
        atto_eq(err, NUMERUS_OK);
        TO_ROMAN_TO_INT_EQ(i, roman);
    }
}

void test_roman_to_int(void)
{
    test_roman_to_int_invalid();
    test_roman_to_int_valid_first_hundred_positives();
    test_roman_to_int_valid_first_hundred_negatives();
    test_roman_to_int_valid_some_extended();
    test_roman_to_int_valid_extremes();
    test_roman_to_int_valid_all_classic();
}
