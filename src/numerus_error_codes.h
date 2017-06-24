/**
 * @file numerus_error_codes.h
 * @brief Numerus error codes with their description.
 * @copyright Copyright © 2015-2017, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 *
 * This header contains all macros for the Numerus error codes and
 * is a part of the numerus.h header file.
 */


/**
 * Everything went all right.
 *
 * Usually this means that the processed numeral contained no syntax errors.
 * This is the opposite of any NUMERUS_ERROR_*.
 */
#ifndef NUMERUS_ERROR_CODES_H
#define NUMERUS_ERROR_CODES_H

#define NUMERUS_OK 1


/**
 * An unknown or unspecified error happened.
 *
 * The error did not match any foreseen case.
 */
#define NUMERUS_ERROR_GENERIC 100


/**
 * The value to be converted to roman numeral is out of conversion range.
 *
 * The value should be within [NUMERUS_MAX_VALUE, NUMERUS_MIN_VALUE].
 */
#define NUMERUS_ERROR_VALUE_OUT_OF_RANGE 101


/**
 * The roman numeral contains a character that is not part of the syntax of
 * roman numerals.
 *
 * The only allowed characters are: "MDCLXVIS_.-" both down- and upcase and
 * the NUMERUS_NULLA string, which is a special case.
 */
#define NUMERUS_ERROR_ILLEGAL_CHARACTER 102


/**
 * The roman numeral is too long to be syntactically correct.
 *
 * The max length of a roman numeral is NUMERUS_MAX_LENGTH.
 */
#define NUMERUS_ERROR_TOO_LONG_NUMERAL 103


/**
 * The roman numeral contains too many consecutive repetitions of a repeatable
 * character.
 *
 * Characters that are allowed to be repeated multiple times are "MCXI.". They
 * have 3 maximum consecutive repetitions each, the dot "." has 5.
 */
#define NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS 104


/**
 * The roman numeral contains mispositioned characters.
 *
 * This is the most generic error for a syntax failure of the roman numeral.
 * It usually means that a roman character is repeated non-consecutively
 * (like "MCIM") or that a non-repeatable roman character has been repeated
 * (like "DD").
 */
#define NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE 105


/**
 * The roman numeral contains one underscore but not the second one.
 *
 * A roman numeral may contain 0 or 2 underscores, the latter being a long
 * numeral. In this case it contains only 1 underscore.
 */
#define NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE 106


/**
 * The long roman numeral contains one underscore after the second one.
 *
 * A roman numeral may contain 0 or 2 underscores, the latter being a long
 * numeral. In this case it contains 3 (or more) underscores, thus being a long
 * numeral with an extra underscore.
 */
#define NUMERUS_ERROR_UNDERSCORE_AFTER_LONG_PART 107


/**
 * The non-long roman numeral contains one underscore.
 *
 * A roman numeral may contain 0 or 2 underscores, the latter being a long
 * numeral. In this case it contains 1 underscore (or more) which is not placed
 * at the first position of the numeral (eventually after the minus), making
 * the numeral a non-long with an unwanted underscore.
 */
#define NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG 108


/**
 * The long roman numeral contains decimal characters "Ss." in the long part.
 *
 * A roman numeral may contain only characters with integer value in the long
 * part (between the underscores). Decimal characters are allowed only in the
 * non-long part, at the end of the numeral.
 */
#define NUMERUS_ERROR_DECIMALS_IN_LONG_PART 109


/**
 * The roman numeral contains a misplaced minus '-' or more than one.
 *
 * A roman numeral may contain only one minus which, if existing, must be at the
 * first position of the numeral.
 */
#define NUMERUS_ERROR_ILLEGAL_MINUS 110


/**
 * The long roman numeral contains an 'M' character after the long part.
 *
 * A roman numeral may contain an 'M' only between the underscores, if it's
 * long, because in it values bigger than 1000 ('M') are represented between
 * underscores, making an 'M' character after the underscore illogical.
 */
#define NUMERUS_ERROR_M_IN_SHORT_PART 111


/**
 * Heap memory allocation failure.
 *
 * The malloc() function failed to allocate memory within one of the Numerus
 * functions. This is an error depending on the OS and its current status.
 * Please retry.
 */
#define NUMERUS_ERROR_MALLOC_FAIL 112


/**
 * The pointer to the roman numeral string is NULL.
 *
 * Make it point to an actual allocated string.
 */
#define NUMERUS_ERROR_NULL_ROMAN 113


/**
 * The roman numeral string is empty or filled with whitespace.
 *
 * Insert some non-whitespace characters in the string.
 */
#define NUMERUS_ERROR_EMPTY_ROMAN 114


/**
 * The roman numeral string contains whitespace characters, even at the end.
 *
 * Remove the whitespace characters from inside the string and trim it.
 */
#define NUMERUS_ERROR_WHITESPACE_CHARACTER 115

#endif // NUMERUS_ERROR_CODES_H

