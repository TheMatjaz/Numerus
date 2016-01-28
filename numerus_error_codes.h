
/**
 * Everything went all right.
 *
 * Usually this means that the processed numeral contained no syntax errors.
 * This is the opposite of any NUMERUS_ERROR_*.
 */
#define NUMERUS_OK 1

/**
 * Value to be converted to roman is out of conversion range.
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
#define NUMERUS_ERROR_ILLEGAL_CHARACTER 105

/**
 * 
 */
#define NUMERUS_ERROR_TOO_LONG_NUMERAL 106
#define NUMERUS_IS_FLOAT 107
#define NUMERUS_IS_LONG 108
#define NUMERUS_IS_NOT_FLOAT 109
#define NUMERUS_IS_NOT_LONG 110
#define NUMERUS_ERROR_NOT_ROMAN (long)(NUMERUS_MAX_VALUE + 1)
#define NUMERUS_IS_ZERO 111
#define NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS 112
#define NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE 113
#define NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE 114
#define NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART 115
#define NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG 120
#define NUMERUS_ERROR_DECIMALS_IN_LONG_PART 116
#define NUMERUS_ERROR_ILLEGAL_MINUS 117
#define NUMERUS_ERROR_M_IN_SHORT_PART 118
#define NUMERUS_ERROR_GENERIC 119
#define NUMERUS_ERROR_MALLOC_FAIL 120
#define NUMERUS_ERROR_NULL_ROMAN 121
#define NUMERUS_ERROR_EMPTY_ROMAN 122