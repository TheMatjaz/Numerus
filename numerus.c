/**
 * @file
 * @brief Numerus core function for roman numerals conversion.
 *
 * Inspired by:
 * 
 * - http://stackoverflow.com/a/26723344/5292928
 * - http://stackoverflow.com/a/30816418/5292928
 */

#include "numerus.h"
#include <regex.h>  /* To match correct roman numeral syntax */
#include <stdlib.h> /* For malloc() */

/**
 * Compiled regex matching any syntactiacally correct roman numeral.
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
 * Compiles the the ROMAN_SYNTAX_REGEX from ROMAN_SYNTAX_REGEX_STRING.
 *
 * Returns the compilation status of the regex: 0 if compiled correctly, which
 * should happen since ROMAN_SYNTAX_REGEX_STRING is a correct constant.
 */
int init() {
    return regcomp(&ROMAN_SYNTAX_REGEX, ROMAN_SYNTAX_REGEX_STRING, REG_NOSUB);
}
