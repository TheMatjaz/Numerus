/**
 * @file
 * @internal
 * Conversion of all possible values into all possible roman numerals and
 * backwards to check that the conversion functions are bijective in the
 * entire domain and that all provided outputs are smaller than the max
 * size.
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


#define CANARY '@'
#define LOG_FREQUENCY 30000L

static const uint32_t TOTAL_ITERATIONS = (NUMERUS_MAX_INT + 1UL) * 12UL * 2UL;

inline static void log_state(const numerus_frac_t input_fraction)
{
    double perc = input_fraction.int_part * 12.0;
    perc /= TOTAL_ITERATIONS;
    perc *= 100;
    printf("\r> %8"PRId32", %6.2f%%", input_fraction.int_part, perc);
    fflush(stdout);
}

void test_bijectivity(void)
{
    puts("Starting bijectivity tests. This takes some time...");
    numerus_frac_t input_fraction;
    numerus_frac_t output_fraction;
    numerus_err_t err;
    char roman[NUMERUS_MAX_LEN_WITH_TERM + 1];
    roman[NUMERUS_MAX_LEN_WITH_TERM] = CANARY;

    for (input_fraction.int_part = 0;
         input_fraction.int_part <= NUMERUS_MAX_INT;
         input_fraction.int_part++)
    {
        if (input_fraction.int_part % 20000L == 0)
        {
            log_state(input_fraction);
        }
        for (input_fraction.twelfths = 0;
             input_fraction.twelfths <= 11;
             input_fraction.twelfths++)
        {
            // Write something into the roman to see that it changes
            roman[0] = '\0';
            // Fraction -> Roman
            err = numerus_roman_from_fraction(roman, input_fraction);
            if (err != NUMERUS_OK)
            {
                printf("\nFrac->roman fail: "
                       "%"PRId32", %"PRId32"/12, err = %d\n",
                       input_fraction.int_part,
                       input_fraction.twelfths,
                       err);
                atto_fail();
            }
            // Length is < than the max length
            if (strnlen(roman, NUMERUS_MAX_LEN_WITH_TERM) >=
                NUMERUS_MAX_LEN_WITH_TERM)
            {
                printf("\nLength exceeding boundaries: "
                       "%"PRId32", %"PRId32"/12\n",
                       input_fraction.int_part,
                       input_fraction.twelfths);
                atto_fail();
            }
            // Canary after the numeral is untouched
            // i.e. no writing out of bounds
            if (roman[NUMERUS_MAX_LEN_WITH_TERM] != CANARY)
            {
                printf("\nCanary was overwritten: "
                       "%"PRId32", %"PRId32"/12\n",
                       input_fraction.int_part,
                       input_fraction.twelfths);
                atto_fail();
            }
            // Clear the output fraction to see that it changes
            output_fraction.int_part = 0xFFFFFFFFL;
            output_fraction.twelfths = 0xFFFFFFFFL;
            // Roman -> fraction
            err = numerus_roman_to_fraction(&output_fraction, roman);
            if (err != NUMERUS_OK)
            {
                printf("\nRoman->Frac fail: "
                       "%"PRId32", %"PRId32"/12 -> %s, err = %d\n",
                       input_fraction.int_part,
                       input_fraction.twelfths,
                       roman,
                       err);
                atto_fail();
            }
            // Bijectivity
            if (input_fraction.int_part != output_fraction.int_part
                || input_fraction.twelfths != output_fraction.twelfths)
            {
                printf("\nBijectivity fail: "
                       "%"PRId32", %"PRId32"/12 "
                       "-> %s "
                       "-> %"PRId32", %"PRId32"/12\n",
                       input_fraction.int_part,
                       input_fraction.twelfths,
                       roman,
                       output_fraction.int_part,
                       output_fraction.twelfths);
                atto_fail();
            }
        }
    }
    puts("\rNon-negatives fully bijective.");
    for (input_fraction.int_part = 0;
         input_fraction.int_part >= NUMERUS_MIN_INT;
         input_fraction.int_part--)
    {
        if (input_fraction.int_part % LOG_FREQUENCY == 0)
        {
            log_state(input_fraction);
        }
        for (input_fraction.twelfths = 0;
             input_fraction.twelfths >= -11;
             input_fraction.twelfths--)
        {
            // Write something into the roman to see that it changes
            roman[0] = '\0';
            // Fraction -> Roman
            err = numerus_roman_from_fraction(roman, input_fraction);
            if (err != NUMERUS_OK)
            {
                printf("\nFrac->roman fail: "
                       "%"PRId32", %"PRId32"/12, err = %d\n",
                       input_fraction.int_part,
                       input_fraction.twelfths,
                       err);
                atto_fail();
            }
            // Length is < than the max length
            if (strnlen(roman, NUMERUS_MAX_LEN_WITH_TERM) >=
                NUMERUS_MAX_LEN_WITH_TERM)
            {
                printf("\nLength exceeding boundaries: "
                       "%"PRId32", %"PRId32"/12\n",
                       input_fraction.int_part,
                       input_fraction.twelfths);
                atto_fail();
            }
            // Canary after the numeral is untouched
            // i.e. no writing out of bounds
            if (roman[NUMERUS_MAX_LEN_WITH_TERM] != CANARY)
            {
                printf("\nCanary was overwritten: "
                       "%"PRId32", %"PRId32"/12\n",
                       input_fraction.int_part,
                       input_fraction.twelfths);
                atto_fail();
            }
            // Clear the output fraction to see that it changes
            output_fraction.int_part = 0xFFFFFFFFL;
            output_fraction.twelfths = 0xFFFFFFFFL;
            // Roman -> fraction
            err = numerus_roman_to_fraction(&output_fraction, roman);
            if (err != NUMERUS_OK)
            {
                printf("\nRoman->Frac fail: "
                       "%"PRId32", %"PRId32"/12 -> %s, err = %d\n",
                       input_fraction.int_part,
                       input_fraction.twelfths,
                       roman,
                       err);
                atto_fail();
            }
            // Bijectivity
            if (input_fraction.int_part != output_fraction.int_part
                || input_fraction.twelfths != output_fraction.twelfths)
            {
                printf("\nBijectivity fail: "
                       "%"PRId32", %"PRId32"/12 "
                       "-> %s "
                       "-> %"PRId32", %"PRId32"/12\n",
                       input_fraction.int_part,
                       input_fraction.twelfths,
                       roman,
                       output_fraction.int_part,
                       output_fraction.twelfths);
                atto_fail();
            }
        }
    }
    puts("\rNegatives fully bijective.");
}
