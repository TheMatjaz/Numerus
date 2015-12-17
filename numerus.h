/**
 * @file
 * @brief Numerus roman numerals library header
 */


/**
 * Everything went all right. This is the opposite of ROMAN_ERROR.
 */
#define ROMAN_OK 1000;

/**
 * Numeral is not a correct roman one. Check the syntax of the roman numeral for non-roman characters, too many repetitions or wrong character order.
 */
#define ROMAN_ERROR_WRONG_SYNTAX 100;

/**
 * Integer out of conversion range. short_to_roman() cannot convert the passed value because it exceed the possible values a roman numeral may represent, which are the integers in the interval [ROMAN_MIN_VALUE, ROMAN_MAX_VALUE].
 */
#define ROMAN_ERROR_OUT_OF_RANGE 101;

/**
 * regexec() internal error while checking the syntax correctness of a roman numeral to be converted to short by roman_to_short(). Check regerror() and the stderr for more information.
 */
#define ROMAN_ERROR_REGEXEC 102;

/**
 * A SQLite error happened. Check the stderr for more information.
 */
#define ROMAN_ERROR_SQLITE 103;

/**
 * roman_is_bigger() could not compare the two roman numerals because at least one of them cannot be converted to short so it has no value.
 */
#define ROMAN_ERROR_CANNOT_COMPARE 104;

extern const char *ROMAN_SYNTAX_REGEX_STRING;

int is_roman(char *roman);
int roman_is_nulla(char *roman);
char* int_to_roman(int arabic);
short roman_to_short(char *roman);
int roman_is_bigger(char *roman_bigger, char *roman_smaller);
char** allocate_all_romans(short int include_negatives);
int save_to_sqlite3();
