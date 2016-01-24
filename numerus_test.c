#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "numerus.h"

/**
 * Converting all values in range [%d, %d] to roman numerals and back to values. Then the values are confronted if same or not.\n", NUMERUS_MIN_LONG_VALUE, NUMERUS_MAX_LONG_VALUE)
 */
void numtest_convert_all_romans() {
    printf("Converting all values in range [%ld, %ld] to roman numerals and back to values. Then the values are confronted if same or not.\n", NUMERUS_MIN_LONG_VALUE, NUMERUS_MAX_LONG_VALUE);
    clock_t cStartClock1 = clock();
    double i;
    double value;
    char *roman;
    int errcode;
    for (i = NUMERUS_MIN_LONG_VALUE; i <= NUMERUS_MAX_LONG_VALUE; i++) {
        roman = numerus_double_to_roman(i);
        errcode = numerus_roman_to_double(roman, &value);
        if (errcode != NUMERUS_OK) {
            fprintf(stderr, "%s: %s\n", numerus_explain_error(errcode), roman);
        }
        if (i != value) {
            fprintf(stderr, "Error at converting %f -> %s -> %f", i, roman,
                    value);
            exit(1);
        }
        free(roman);
    }
    clock_t cEndClock1 = clock();
    clock_t cStartClock2 = clock();
    for (i = NUMERUS_MIN_LONG_VALUE; i <= NUMERUS_MAX_LONG_VALUE; i++) {
        roman = numerus_long_to_roman(i);
        free(roman);
    }
    clock_t cEndClock2 = clock();

    clock_t romtoint =
            (cEndClock1 - cStartClock1) - (cEndClock2 - cStartClock2);
    clock_t inttorom = (cEndClock2 - cStartClock2);
    clock_t bothways = (cEndClock1 - cStartClock1);
    printf("Time to convert int to rom: %f\n",
           inttorom / (double) CLOCKS_PER_SEC);
    printf("Time to convert rom to int: %f\n",
           romtoint / (double) CLOCKS_PER_SEC);
    printf("Time to convert both ways:  %f\n",
           bothways / (double) CLOCKS_PER_SEC);
    printf("int to rom conversion is %f times faster than rom to int on average on all romans\n",
           (double) romtoint / (double) inttorom);

}

void _num_test_for_error(char *roman, int error_code) {
    fprintf(stderr, "Converting %s : ", roman);
    double value = 0;
    int code = NUMERUS_OK;
    code = numerus_roman_to_double(roman, &value);
    if (code == error_code) {
        fprintf(stderr, "Passed: %s ---> %s\n", roman, numerus_explain_error(code));
    } else {
        fprintf(stderr, "FAILED: %s -/-> %s\n", roman, numerus_explain_error(code));
    }
}

void numtest_all() {
    _num_test_for_error("-_MCM_XX_I", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);
    _num_test_for_error("-_MCM__I", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);
    _num_test_for_error("-_MCM_LI_", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);
    // NO CHARS BETWEEN UNDERSCORES

    _num_test_for_error("-MMCM-LI", NUMERUS_ERROR_TOO_MANY_MINUSES);
    _num_test_for_error("--_MCM_LI", NUMERUS_ERROR_TOO_MANY_MINUSES);

    _num_test_for_error("MMMCMLCI", NUMERUS_ERROR_ILLEGAL_CHAR_ORDER);
    _num_test_for_error("MMMCMLIIIX", NUMERUS_ERROR_ILLEGAL_CHAR_ORDER);
    _num_test_for_error("MMMCMLIII.S", NUMERUS_ERROR_ILLEGAL_CHAR_ORDER);

    _num_test_for_error("-XVIFI", NUMERUS_ERROR_ILLEGAL_CHARACTER);
    _num_test_for_error("-XVI.FI", NUMERUS_ERROR_ILLEGAL_CHARACTER);
    _num_test_for_error("-XVI,.", NUMERUS_ERROR_ILLEGAL_CHARACTER);

    _num_test_for_error("MMMM", NUMERUS_ERROR_TOO_MANY_REPETITIONS);
    _num_test_for_error("MMCC", NUMERUS_ERROR_TOO_MANY_REPETITIONS);
    _num_test_for_error("MMCMCM", NUMERUS_ERROR_TOO_MANY_REPETITIONS);
    _num_test_for_error("MMSS", NUMERUS_ERROR_TOO_MANY_REPETITIONS);
    _num_test_for_error("MM......", NUMERUS_ERROR_TOO_MANY_REPETITIONS);

    _num_test_for_error("-_MCMLI", NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE);
    _num_test_for_error("_MCMLI", NUMERUS_ERROR_MISSING_SECOND_UNDERSCORE);

    _num_test_for_error("_MCMS_LI", NUMERUS_ERROR_DECIMALS_IN_LONG_PART);
    _num_test_for_error("_MCM.._LI", NUMERUS_ERROR_DECIMALS_IN_LONG_PART);
    _num_test_for_error("_MCMs.._LI", NUMERUS_ERROR_DECIMALS_IN_LONG_PART);

    _num_test_for_error("_MCM_MLI", NUMERUS_ERROR_M_IN_SHORT_PART);

    _num_test_for_error("CCX_II", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);
    _num_test_for_error("-CCX_II", NUMERUS_ERROR_UNDERSCORE_IN_SHORT_PART);
}