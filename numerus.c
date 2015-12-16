/**
 * @file
 * @brief Numerus core function for roman numerals conversion.
 *
 * Inspired by:
 * 
 * - http://stackoverflow.com/a/26723344/5292928
 * - http://stackoverflow.com/a/30816418/5292928
 */

#include <stdio.h>  /* To `fprintf()` to `stderr` */
#include <stdlib.h> /* For `malloc()` */
#include <string.h> /* For `strcmp()` in `roman_is_nulla()` */
#include <regex.h>  /* To use regexes to match correct roman numeral syntax */
#include <sqlite3.h> /* To save all roman integers to a sqlite3 file */

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

/*
In memory structure:   >  | 16 bit short | 3 bytes string | (string has '\0')
If 1 byte (smallest accessable memory portion) = 8 bits, then it is:
    >   | 16 bit short | 24 bit string | = 40 bits = 5 bytes to store a roman
To store the dictionary it takes 65 bytes.
To store all the roman numbers as STRUCTS it takes 96000 bytes (96 kB), including string null terminators, negative numbers and 0/NULLA, excluding any separators
*/

/**
 * Struct containing a pair basic roman char and its short integer value.
 *
 * It's used to create the ROMAN_CHARS dictionary and used by conversion
 * functions. The "roman chars" as called in this library are strings of 1 or 2
 * chars that have a specific known value.
 */
static struct roman_char_struct {
    const short int value;
    const char chars[3]; /* 1-2 chars + \0 = length 3 */
};

/**
 * Dictionary of basic roman chars and their values used by conversion
 * functions.
 */
static const struct roman_char_struct ROMAN_CHARS[] = {
        {1000, "M" },
        { 900, "CM"},
        { 500, "D" },
        { 400, "CD"},
        { 100, "C" },
        {  90, "XC"},
        {  50, "L" },
        {  40, "XL"},
        {  10, "X" },
        {   9, "IX"},
        {   5, "V" },
        {   4, "IV"},
        {   1, "I" }
};

/**
 * Returns the length of the roman numeral including the leading '-', excluding the null terminator.
 * If it's too long, then is not a roman numeral and returns -1.
 * It's not using strlen() to be able to stop as soon as the string is longer
 * than the maximum possible roman lenght.
 *
 * TODO: since it's a static function, can be used only inside the library. This means there is no need for it to be secure for external users. You may NOT perform the i < ROMAN_MAX_LENGTH check.
 *
 */
static int roman_numeral_length(char *roman) {
    int i;
    for (i = 0; i < ROMAN_MAX_LENGTH; i++) {
        if (roman[i] == '\0') {
            return i;
        }
    }
    return -1;
}

/**
 * Calls strcmp to check if the roman number is NULLA or -NULLA (case INsensitive).
 *
 * Returns 1 if the string is (-)NULLA or 0 if it's not.
 */
int roman_is_nulla(char *roman) {
    if (*roman == '-') {
        roman++;
    }
    if (strcasecmp(roman, NULLA) != 0) {
        return 0;
    } else {
        return 1;
    };
}

/**
 * Copies a string of 1 or 2 characters.
 *
 * Copies the character from the source to the destination. If there is another
 * character after that, that is not the null terminator, copies that as well.
 * Everything is performed without security checks for faster performance. This
 * function is used by int_to_roman() and  it's meant to be used just on the
 * roman_char_struct in the dictionary ROMAN_CHARS.
 */
static char *copy_roman_char_from_dictionary(char *source, char *destination) {
    *destination = *(source++);
    if (*source != '\0') {
        *(++destination) = *source;
    }
    return ++destination;
}

/**
 * Converts an integer to a roman numeral.
 *
 * Returns NULL if the int is out of range, otherwise returns a pointer to a
 * string containing the roman numeral.
 */
char* int_to_roman(int arabic) {
    /* Out of range check */
    if (arabic < -3999 || arabic > 3999) {
        fprintf(stderr, "Roman conversion error: arabic %d out of range.\n", arabic);
        return NULL;
    }

    /* Create pointer to buffer */
    char *roman_string = &roman_numeral_build_buffer[0];

    /* Save sign or return NULLA for 0 */
    if (arabic < 0) {
        arabic *= -1;
        *(roman_string++) = '-';
    } else if (arabic == 0) {
        return NULLA; /* TODO: Probably should return a copy of it? */
    }

    /* Actual conversion */
    struct roman_char_struct *current_roman_char = &ROMAN_CHARS[0];
    while (arabic > 0) {
        while (arabic >= current_roman_char->value) {
            roman_string = copy_roman_char_from_dictionary(
                    current_roman_char->chars, roman_string);
            arabic -= current_roman_char->value;
        }
        current_roman_char++;
    }

    /* Null terminate builded string in buffer */
    *roman_string = '\0';

    /* Copy out of buffer and return it */
    char *returnable_roman_string = malloc(roman_string - roman_numeral_build_buffer); /* Allocate the size of the builded string */
    strcpy(returnable_roman_string, roman_numeral_build_buffer);
    return returnable_roman_string;
}



/**
 * Confronts if the two given string match in the the next 1 or 2 characters.
 *
 * Returns the length of the match, which may be 0 if they don't match or 1 or 2
 * if they match. This functions is used by roman_to_short().
 */
static short int begins_with(char *to_compare, char *pattern) {
    short pattern_length = strlen(pattern);
    if (strncmp(to_compare, pattern, pattern_length) == 0) { /* run strcmp() on first "pattern_length" characters */
        return pattern_length;
    } else {
        return 0;
    }
}

/**
 * Converts a roman numeral to int.
 *
 * If the value cannot be converted, returns a value bigger than ROMAN_MAX_VALUE
 * and a non-zero error code in the second parameter.
 */
short roman_to_short(char *roman, short int *error_code) {
    /* Exclude nulla numerals */
    if (roman_is_nulla(roman)) {
        return 0;
    }

    /* Return an error if the roman is not syntactically correct */
    if (!is_roman(roman)) {
        *error_code = 1;
        return ROMAN_MAX_VALUE + 1;
    }

    /* Save sign */
    short int sign = 1;
    if (*roman == '-') {
        sign = -1;
        roman++;
    }

    /* Actual conversion */
    short int arabic = 0;
    struct roman_char_struct *current_roman_char = &ROMAN_CHARS[0];
    while (*roman != '\0') {
        short matching_chars = begins_with(roman, current_roman_char->chars);
        if (matching_chars > 0) {
            roman += matching_chars;
            arabic += current_roman_char->value;
        } else {
            current_roman_char++;
        }
    }

    *error_code = 0;
    return sign * arabic;
}

/**
 * Use indices as arabic integers. The char* stored at that index, points to the
 * roman numeral of that value.
 * Example usage:
 * char **all_romans = allocate_all_romans(1);
 * all_romans[702] will be int_to_roman(702) = "DCCII" */
char** allocate_all_romans(short int include_negatives) {
    short int num_of_romans = ROMAN_MAX_VALUE;
    if (include_negatives) {
        num_of_romans *= 2;
    }
    num_of_romans += 1; /* For nulla */
    char* all_roman_numerals[num_of_romans]; /* Array of pointers to strings */
    int index = 0;
    short i;
    if (include_negatives) {
        for (i = ROMAN_MIN_VALUE; i < 0; i++) {
            all_roman_numerals[index++] = int_to_roman(i);
        }
    }
    for (i = 0; i <= ROMAN_MAX_VALUE; i++) {
        all_roman_numerals[index++] = int_to_roman(i);
    }
    return &all_roman_numerals[0 + (include_negatives ? ROMAN_MAX_VALUE : 0)];
}
int save_to_sqlite3() {
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open_v2("file:numerus.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); /* Open in RW mode and create if not exists */

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char *query = "DROP TABLE IF EXISTS roman_numerals; "
                  "CREATE TABLE roman_numerals (value INT, numeral TEXT);";
    rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {

        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_free(query);
        sqlite3_close(db);

        return 1;
    }
    //sqlite3_free(query);

    short i;
    for (i = ROMAN_MIN_VALUE; i <= ROMAN_MAX_VALUE; i++) {
        query = sqlite3_mprintf(
                "INSERT INTO roman_numerals VALUES (%d, '%q');", i, int_to_roman(i));

        rc = sqlite3_exec(db, query, 0, 0, &err_msg);
        if (rc != SQLITE_OK ) {

            fprintf(stderr, "SQL error: %s\n", err_msg);

            sqlite3_free(err_msg);
            sqlite3_free(query);
            sqlite3_close(db);

            return 1;
        }
    }

    sqlite3_free(err_msg);
    sqlite3_free(query);
    sqlite3_close(db);
    return 0;
}
