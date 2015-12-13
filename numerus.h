/**
 * @file
 * @brief Numerus roman numerals library header
 */

#include <regex.h>  /* For `regex_t` */

/**
 * Maximum value as int a roman numeral may have.
 */
const int ROMAN_MAX_VALUE = 3999;

/**
 * Minimum value as int a roman numeral may have.
 */
const int ROMAN_MIN_VALUE = -3999;

/**
 * Roman numeral of value 0 (zero).
 */
const char *NULLA = "NULLA";

/**
 * Maximum length of a roman numeral string including the null terminator.
 *
 * The roman numeral `"-MMMDCCCLXXXVIII"` (value: -3888) + `\0` is a string long
 * 16+1 = 17 chars.
 */
const int ROMAN_MAX_LENGTH = 17;

/**
 * String containing a to-be-compiled regex matching any syntactiacally correct
 * roman numeral.
 */
const char *ROMAN_SYNTAX_REGEX_STRING = "^-?((M{0,3})(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))$";

