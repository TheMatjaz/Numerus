/**
 * @file numerus_test.c
 * @brief Numerus test functions to verify the correctness of the library.
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "numerus.h"
#include "numerus_test.h"

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
 * @returns the *really* approximate computation time on stdout and returns 0
 * on success or outputs any error on stderr and returns 1.
 */
int numtest_convert_all_floats_with_parts() {
    long int_part;
    short frac_part;
    long int_part_converted;
    short frac_part_converted;
    char *roman;
    int errcode;
    printf("Starting conversion of all roman numerals with parts\n");
    clock_t start_clock = clock();
    for (int_part = NUMERUS_MIN_LONG_VALUE; int_part <= NUMERUS_MAX_LONG_VALUE; int_part++) {
        for (frac_part = 0; frac_part < 12; frac_part++) {
            roman = numerus_int_with_twelfth_to_roman(int_part, frac_part, &errcode);
            if (errcode != NUMERUS_OK) {
                fprintf(stderr, "Error converting %ld, %d to roman: %s\n",
                        int_part, frac_part, numerus_explain_error(errcode));
                return 1;
            }
            errcode = numerus_roman_to_int_and_frac_part(roman, &int_part_converted, &frac_part_converted);
            if (errcode != NUMERUS_OK) {
                fprintf(stderr, "Error converting %s to value: %s.\n",
                        roman, numerus_explain_error(errcode));
                return 1;
            }
            if (int_part != int_part_converted || frac_part != frac_part_converted) {
                fprintf(stderr, "Conversion mismatch: %ld, %d/12 (%f) -> %s -> %ld, %d/12 (%f)\n",
                        int_part, frac_part, numerus_parts_to_double(int_part, frac_part), roman, int_part_converted, frac_part_converted, numerus_parts_to_double(int_part_converted, frac_part_converted));
                return 1;
            }
            free(roman);
        }
        if (int_part % 100000 == 0) {
            printf("%5.2f%%\n", 100.0 * (int_part + NUMERUS_MAX_LONG_VALUE) / 7999999.0);
        }
    }
    clock_t end_clock = clock();
    double seconds_taken = (end_clock - start_clock) / (double) CLOCKS_PER_SEC;
    printf("Time to convert all 96000001 float roman numerals both ways as parts: %f\n",
           seconds_taken);
    printf("It's %f bidirectional conversions per second.\n",
           96000001.0/seconds_taken);
    return 0;
}

int numtest_convert_all_floats_with_doubles() {
    long int_part;
    short frac_part;
    double to_convert;
    double converted;
    char *roman;
    int errcode;
    printf("Starting conversion of all roman numerals with doubles \n");
    clock_t start_clock = clock();
    for (int_part = NUMERUS_MIN_LONG_VALUE; int_part <= NUMERUS_MAX_LONG_VALUE; int_part++) {
        for (frac_part = 0; frac_part < 12; frac_part++) {
            to_convert = numerus_parts_to_double(int_part, frac_part);
            roman = numerus_double_to_roman(to_convert, &errcode);
            if (errcode != NUMERUS_OK) {
                fprintf(stderr, "Error converting %ld, %d (%f) to roman: %s.\n",
                        int_part, frac_part, to_convert, numerus_explain_error(errcode));
                return 1;
            }
            errcode = numerus_roman_to_double(roman, &converted);
            if (errcode != NUMERUS_OK) {
                fprintf(stderr, "Error converting %s to value: %s.\n",
                        roman, numerus_explain_error(errcode));
                return 1;
            }
            if ((float) to_convert != (float) converted) {
                fprintf(stderr, "Conversion mismatch: %f (%ld, %d/12) -> %s -> %f\n",
                        to_convert, int_part, frac_part, roman, converted);
                return 1;
            }
            free(roman);
        }
        if (int_part % 100000 == 0) {
            printf("%5.2f%%\n", 100.0 * (int_part + NUMERUS_MAX_LONG_VALUE) / 7999999.0);
        }
    }
    clock_t end_clock = clock();
    double seconds_taken = (end_clock - start_clock) / (double) CLOCKS_PER_SEC;
    printf("Time to convert all 96000001 float roman numerals both ways as doubles: %f\n",
           seconds_taken);
    printf("It's %f bidirectional conversions per second.\n",
           96000001.0/seconds_taken);
    return 0;
}

int numtest_convert_all_integers_with_parts() {
    long int_part;
    long converted;
    char *roman;
    int errcode;
    printf("Starting conversion of all integer roman numerals with parts \n");
    clock_t start_clock = clock();
    for (int_part = NUMERUS_MIN_LONG_VALUE; int_part <= NUMERUS_MAX_LONG_VALUE; int_part++) {
        roman = numerus_int_to_roman(int_part, &errcode);
        if (errcode != NUMERUS_OK) {
            fprintf(stderr, "Error converting %ld to roman: %s\n",
                    int_part, numerus_explain_error(errcode));
            return 1;
        }
        errcode = numerus_roman_to_int(roman, &converted);
        if (errcode != NUMERUS_OK) {
            fprintf(stderr, "Error converting %s to value: %s\n",
                    roman, numerus_explain_error(errcode));
            return 1;
        }
        if (int_part != converted) {
            fprintf(stderr, "Conversion mismatch: %ld -> %s -> %ld",
                    int_part, roman, converted);
            return 1;
        }
        free(roman);
        if (int_part % 100000 == 0) {
            printf("%5.2f%%\n", 100.0 * (int_part + NUMERUS_MAX_LONG_VALUE) / 7999999.0);
        }
    }
    clock_t end_clock = clock();
    double seconds_taken = (end_clock - start_clock) / (double) CLOCKS_PER_SEC;
    printf("Time to convert all 7999999 integer roman numerals both ways as parts: %f\n",
           seconds_taken);
    printf("It's %f bidirectional conversions per second.\n",
           7999999.0/seconds_taken);
    return 0;
}

int numtest_convert_all_integers_with_doubles() {
    long int_part;
    double to_convert;
    double converted;
    char *roman;
    int errcode;
    printf("Starting conversion of all integer roman numerals with doubles \n");
    clock_t start_clock = clock();
    for (int_part = NUMERUS_MIN_LONG_VALUE; int_part <= NUMERUS_MAX_LONG_VALUE; int_part++) {
        to_convert = numerus_parts_to_double(int_part, 0);
        roman = numerus_double_to_roman(to_convert, &errcode);
        if (errcode != NUMERUS_OK) {
            fprintf(stderr, "Error converting %f to roman: %s\n",
                    to_convert, numerus_explain_error(errcode));
            return 1;
        }
        errcode = numerus_roman_to_double(roman, &converted);
        if (errcode != NUMERUS_OK) {
            fprintf(stderr, "Error converting %s to value: %s\n",
                    roman, numerus_explain_error(errcode));
            return 1;
        }
        if (int_part != converted) {
            fprintf(stderr, "Conversion mismatch: %f (%ld) -> %s -> %f",
                    to_convert, int_part, roman, converted);
            return 1;
        }
        free(roman);
        if (int_part % 100000 == 0) {
            printf("%5.2f%%\n", 100.0 * (int_part + NUMERUS_MAX_LONG_VALUE) / 7999999.0);
        }
    }
    clock_t end_clock = clock();
    double seconds_taken = (end_clock - start_clock) / (double) CLOCKS_PER_SEC;
    printf("Time to convert all 7999999 integer roman numerals both ways as doubles: %f\n",
           seconds_taken);
    printf("It's %f bidirectional conversions per second.\n",
           7999999.0/seconds_taken);
    return 0;
}


/**
 * Perform a single test to verify the reaction of the roman to value
 * conversion when the syntax is correct.
 */
static void _num_test_for_error(char *roman, int error_code) {
    double value = 0.0;
    int code = NUMERUS_OK;
    code = numerus_roman_to_double(roman, &value);
    if (code == error_code) {
        fprintf(stderr, "Test passed: %s raises error \"%s\"\n",
                roman, numerus_explain_error(code));
    } else {
        fprintf(stderr, "Test FAILED: %s raises \"%s\" instead of \"%s\"\n",
                roman, numerus_explain_error(code),
                numerus_explain_error(error_code));
    }
}


/**
 * Performs a series of tests to verify the reaction of the roman to value
 * conversion when the syntax is correct.
 *
 * Outputs the result to to stderr.
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