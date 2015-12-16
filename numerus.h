/**
 * @file
 * @brief Numerus roman numerals library header
 */

/**
 * Maximum value as int a roman numeral may have.
 */
extern const int ROMAN_MAX_VALUE;

/**
 * Minimum value as int a roman numeral may have.
 */
extern const int ROMAN_MIN_VALUE;

/**
 * Roman numeral of value 0 (zero).
 */
extern const char *NULLA;

/**
 * Maximum length of a roman numeral string including the null terminator.
 *
 * The roman numeral `"-MMMDCCCLXXXVIII"` (value: -3888) + `\0` is a string long
 * 16+1 = 17 chars.
 */
extern const int ROMAN_MAX_LENGTH;

/**
 * String containing a to-be-compiled regex matching any syntactically correct
 * roman numeral.
 */
extern const char *ROMAN_SYNTAX_REGEX_STRING;

int is_roman(char *roman);
int roman_is_nulla(char *roman);
char* int_to_roman(int arabic);
short roman_to_short(char *roman, short int *error_code);
int roman_is_bigger(char *roman_bigger, char *roman_smaller);
char** allocate_all_romans(short int include_negatives);
int save_to_sqlite3();
