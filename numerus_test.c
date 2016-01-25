#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "numerus.h"

/**
 * Converts all 95999977 possible values of roman numerals to their roman form
 * and back to their value.
 *
 * The test is about 2-3 minutes long on a single core 2.4 GHz Intel Core i5
 * and verifies two things:
 *
 * - the roman numeral with a specified value should be syntactically correct
 * - the conversions should be bijective, which means that the value converted
 *   to roman numeral and back to value should still be the same value.
 *
 * @returns the computation time on stdout and returns 0 on success or outputs
 * any error on stderr and returns 1.
 */
int numtest_convert_all_romans() {
    double i;
    double value;
    char *roman;
    int errcode;
    double decimal;
    clock_t start_clock = clock();
    for (i = NUMERUS_MIN_LONG_VALUE; i <= NUMERUS_MAX_LONG_VALUE; i++) {
        for (decimal = 0.0; decimal < 1.0; decimal += 1.0/12.0) {
            roman = numerus_double_to_roman(i);
            errcode = numerus_roman_to_value(roman, &value);
            if (errcode != NUMERUS_OK) {
                fprintf(stderr, "%15.15f: %s: %s\n", i,
                        numerus_explain_error(errcode), roman);
                return 1;
            }
            if ((float) i != (float) value) {
                fprintf(stderr, "Error at converting %15.15f -> %s -> %15.15f",
                        i, roman, value);
                return 1;
            }
            free(roman);
        }
    }
    clock_t end_clock = clock();
    double seconds_taken = (end_clock - start_clock) / (double) CLOCKS_PER_SEC;
    printf("Time to convert all 95999977 roman numerals both ways: %f\n",
           seconds_taken);
    printf("It's %f bidirectional conversions per second.\n",
           95999977.0/seconds_taken);
    return 0;
}

/**
 * Perform a single test to verify the reaction of the roman to value
 * conversion when the syntax is correct.
 */
void _num_test_for_error(char *roman, int error_code) {
    double value = 0.0;
    int code = NUMERUS_OK;
    code = numerus_roman_to_value(roman, &value);
    if (code == error_code) {
        fprintf(stderr, "Test passed: %s raises error \"%s\"\n", roman, numerus_explain_error(code));
    } else {
        fprintf(stderr, "Test FAILED: %s raises \"%s\" instead of \"%s\"\n", roman, numerus_explain_error(code), numerus_explain_error(error_code));
    }
}

/**
 * Performs a series of tests to verify the reaction of the roman to value
 * conversion when the syntax is correct.
 *
 * @see _num_test_for_error(char *roman, int error_code)
 */
void numtest_roman_syntax_errors() {
    _num_test_for_error("-_MCM_XX_I", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);
    _num_test_for_error("-_MCM__I", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);
    _num_test_for_error("-_MCM_LI_", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);

    _num_test_for_error("-MMCM-LI", NUMERUS_ERROR_ILLEGAL_MINUS);
    _num_test_for_error("--_MCM_LI", NUMERUS_ERROR_ILLEGAL_MINUS);

    _num_test_for_error("MMMCMLCI", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MMMCMLIIIX", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MMMCMLIII.S", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);

    _num_test_for_error("-XVIFI", NUMERUS_ERROR_ILLEGAL_CHARACTER);
    _num_test_for_error("-XVI.FI", NUMERUS_ERROR_ILLEGAL_CHARACTER);
    _num_test_for_error("-XVI,.", NUMERUS_ERROR_ILLEGAL_CHARACTER);

    _num_test_for_error("MMMM", NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS);
    _num_test_for_error("MMCCCC", NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS);
    _num_test_for_error("MMDD", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MMCMCM", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MMCMD", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MMDCD", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MMCMCD", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MMSS", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
    _num_test_for_error("MM......", NUMERUS_ERROR_TOO_MANY_REPEATED_CHARS);

    _num_test_for_error("-_MCMLI", NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE);
    _num_test_for_error("_MCMLI", NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE);

    _num_test_for_error("_MCMS_LI", NUMERUS_ERROR_DECIMALS_IN_LONG_PART);
    _num_test_for_error("_MCM.._LI", NUMERUS_ERROR_DECIMALS_IN_LONG_PART);
    _num_test_for_error("_MCMs.._LI", NUMERUS_ERROR_DECIMALS_IN_LONG_PART);

    _num_test_for_error("_MCM_MLI", NUMERUS_ERROR_M_IN_SHORT_PART);

    _num_test_for_error("CCX_II", NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG);
    _num_test_for_error("-CCX_II", NUMERUS_ERROR_UNDERSCORE_IN_NON_LONG);

    _num_test_for_error("IVI", NUMERUS_ERROR_ILLEGAL_CHAR_SEQUENCE);
}