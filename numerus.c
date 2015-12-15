/**
 * @file
 * @brief Numerus core function for roman numerals conversion.
 *
 * Inspired by:
 * 
 * - http://stackoverflow.com/a/26723344/5292928
 * - http://stackoverflow.com/a/30816418/5292928
 */

#include <regex.h>  /* For `regex_t`, match correct roman numeral syntax*/
#include <stdlib.h> /* For malloc() */

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

/**
 * Compiled regex matching any syntactically correct roman numeral.
 *
 * Obtained by compiling ROMAN_SYNTAX_REGEX_STRING.
 */
static regex_t ROMAN_SYNTAX_REGEX;

/**
 * Status of regexec() matching a string against ROMAN_SYNTAX_REGEX.
 *
 * `0` = the string matches and is a syntactically correct roman numeral, `1` =
 * the string does not match, it is not a roman numeral and cannot be converted
 * or manipulated.
 */
static int roman_regex_matching_status;

/**
 * Buffer where the strings with roman numerals are build an then copied from.
 *
 * This buffer is as long as the longest roman numeral. The usage of this
 * buffer allows one coversion at the time but is more memory efficient since
 * the roman numerals have variable length and can be returned as a string with
 * just the right amount of space allocated. The alternative of not using this
 * buffer would be constuct each roman numeral in an allocated space of
 * ROMAN_MAX_LENGTH characters and return it or at least copy it into a shorter
 * string and free the buffer each time.
 */
static char roman_numeral_build_buffer[ROMAN_MAX_LENGTH];


/**
 * Verifies if the passed string is a correct roman numeral.
 *
 * Matches `*roman` against ROMAN_SYNTAX_REGEX.
 *
 * Compiles the the ROMAN_SYNTAX_REGEX from ROMAN_SYNTAX_REGEX_STRING if necessary on the first run. The regex compilation status is dropped since
 * since ROMAN_SYNTAX_REGEX_STRING is a correct hard coded constant.
 * If you use the flag REG_NOSUB, then regcomp omits from the compiled regular expression the information necessary to record how subexpressions actually match. In this case, you might as well pass 0 for the matchptr and nmatch arguments when you call regexec.
 * REG_ICASE ignores the case
 * REG_EXTENDED uses the extended POSIX standard regular expressions, otherwise it does not work
 * Those are all bit operators and the bit _or_ `|` concatenates them
 * REG_NOSUB does not save subexpressions, only reports the success or failure of compiling the regex
 *
 * @param *roman string containing a roman numeral to check
 * @returns 0 if has correct syntax.
 */
int is_roman(char *roman) {
    if (ROMAN_SYNTAX_REGEX.re_magic == 0) { /* The regex has not been compiled yet */
        /* Compile it */
        regcomp(&ROMAN_SYNTAX_REGEX, ROMAN_SYNTAX_REGEX_STRING, REG_NOSUB|REG_ICASE|REG_EXTENDED);
    }
    int match_result = regexec(&ROMAN_SYNTAX_REGEX, roman, 0, NULL, 0);
    if (match_result == 0) { /* Match */
        return 1;
    } else if (match_result == REG_NOMATCH){ /* No match */
        return 0;
    } else { /* Error */
        char msgbuf[100];
        regerror(match_result, &ROMAN_SYNTAX_REGEX, msgbuf, sizeof(msgbuf));
        return -1;
    }

}
