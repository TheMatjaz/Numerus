/**
 * @file
 * @internal
 * Benchmark of the performance of Numerus.
 *
 * @copyright Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license BSD 3-clause license.
 */

#include "numerus_test.h"


static const uint32_t TOTAL_ITERATIONS = (NUMERUS_MAX_INT + 1UL) * 12UL * 2UL;

static int64_t now_ns(void)
{
    struct timespec timestamp;
    timespec_get(&timestamp, TIME_UTC);
    return ((int64_t) timestamp.tv_sec) * 1000000000LL + timestamp.tv_nsec;
}

#define ITERATION_TO_ROMAN(tw) \
    do { \
        input_fraction.twelfths = (tw); \
        numerus_roman_from_fraction(roman, input_fraction); \
    } while(0)

#define ITERATION_FULL(tw) \
    do{ \
        input_fraction.twelfths = (tw); \
        err = numerus_roman_from_fraction(roman, input_fraction); \
        atto_eq(err, NUMERUS_OK); \
        err = numerus_roman_to_fraction(&output_fraction, roman); \
        atto_eq(err, NUMERUS_OK); \
    } while(0)


static int64_t test_benchmark_value_to_roman(void)
{
    puts("Starting benchmarking tests value->roman. This takes some time...");
    fflush(stdout);
    numerus_frac_t input_fraction;
    char roman[NUMERUS_MAX_LEN_WITH_TERM];

    const int64_t start_ns = now_ns();
    for (input_fraction.int_part = 0;
         input_fraction.int_part <= NUMERUS_MAX_INT;
         input_fraction.int_part++)
    {
        ITERATION_TO_ROMAN(0);
        ITERATION_TO_ROMAN(1);
        ITERATION_TO_ROMAN(2);
        ITERATION_TO_ROMAN(3);
        ITERATION_TO_ROMAN(4);
        ITERATION_TO_ROMAN(5);
        ITERATION_TO_ROMAN(6);
        ITERATION_TO_ROMAN(7);
        ITERATION_TO_ROMAN(8);
        ITERATION_TO_ROMAN(9);
        ITERATION_TO_ROMAN(10);
        ITERATION_TO_ROMAN(11);
    }
    for (input_fraction.int_part = 0;
         input_fraction.int_part >= NUMERUS_MIN_INT;
         input_fraction.int_part--)
    {
        ITERATION_TO_ROMAN(0);
        ITERATION_TO_ROMAN(-1);
        ITERATION_TO_ROMAN(-2);
        ITERATION_TO_ROMAN(-3);
        ITERATION_TO_ROMAN(-4);
        ITERATION_TO_ROMAN(-5);
        ITERATION_TO_ROMAN(-6);
        ITERATION_TO_ROMAN(-7);
        ITERATION_TO_ROMAN(-8);
        ITERATION_TO_ROMAN(-9);
        ITERATION_TO_ROMAN(-10);
        ITERATION_TO_ROMAN(-11);
    }
    const int64_t end_ns = now_ns();
    const int64_t timedelta_ns = end_ns - start_ns;
    const int64_t timedelta_ns_per_iteration =
            timedelta_ns
            / ((int64_t) TOTAL_ITERATIONS);
    puts("These are SINGLE CORE/PROCESS/THREAD results!");
    printf("Conversions value->roman: %"PRId32"\n",
           TOTAL_ITERATIONS);
    printf("Elapsed time value->roman: %"PRId64" ns = %.3f s\n",
           timedelta_ns, ((double) timedelta_ns) / 1e9);
    printf("Avg. conversion: %"PRId64" ns\n",
           timedelta_ns_per_iteration);
    return timedelta_ns;
}

static void test_benchmark_roman_to_value(const int64_t timedelta_ns_to_roman)
{
    puts("Starting benchmarking tests value->roman->value. "
         "This takes some time...");
    fflush(stdout);
    numerus_frac_t input_fraction;
    numerus_frac_t output_fraction;
    numerus_err_t err;
    char roman[NUMERUS_MAX_LEN_WITH_TERM];

    const int64_t start_ns = now_ns();
    for (input_fraction.int_part = 0;
         input_fraction.int_part <= NUMERUS_MAX_INT;
         input_fraction.int_part++)
    {
        ITERATION_FULL(0);
        ITERATION_FULL(1);
        ITERATION_FULL(2);
        ITERATION_FULL(3);
        ITERATION_FULL(4);
        ITERATION_FULL(5);
        ITERATION_FULL(6);
        ITERATION_FULL(7);
        ITERATION_FULL(8);
        ITERATION_FULL(9);
        ITERATION_FULL(10);
        ITERATION_FULL(11);
    }
    for (input_fraction.int_part = 0;
         input_fraction.int_part >= NUMERUS_MIN_INT;
         input_fraction.int_part--)
    {
        ITERATION_FULL(0);
        ITERATION_FULL(-1);
        ITERATION_FULL(-2);
        ITERATION_FULL(-3);
        ITERATION_FULL(-4);
        ITERATION_FULL(-5);
        ITERATION_FULL(-6);
        ITERATION_FULL(-7);
        ITERATION_FULL(-8);
        ITERATION_FULL(-9);
        ITERATION_FULL(-10);
        ITERATION_FULL(-11);
    }
    const int64_t end_ns = now_ns();
    const int64_t timedelta_ns_full = end_ns - start_ns;
    const int64_t timedelta_ns_from_roman =
            timedelta_ns_full - timedelta_ns_to_roman;
    const int64_t timedelta_ns_per_iteration =
            timedelta_ns_full / ((int64_t) TOTAL_ITERATIONS);
    const int64_t timedelta_ns_per_iteration_from_roman =
            timedelta_ns_from_roman / ((int64_t) TOTAL_ITERATIONS);
    puts("These are SINGLE CORE/PROCESS/THREAD results!");
    printf("Elapsed time total: %"PRId64" ns = %.3f s\n",
           timedelta_ns_full,
           ((double) timedelta_ns_full) / 1e9);
    printf("Conversions value->roman->value: %"PRId32"\n",
           TOTAL_ITERATIONS);
    printf("Avg. full conversion value->roman->value: %"PRId64" ns\n",
           timedelta_ns_per_iteration);
    printf("Elapsed time roman->value: %"PRId64" ns = %.3f s\n",
           timedelta_ns_from_roman,
           ((double) timedelta_ns_from_roman) / 1e9);
    printf("Avg. conversion roman->value: %"PRId64" ns\n",
           timedelta_ns_per_iteration_from_roman);
    printf("Parsing a roman numeral is %.2fx slower than generating one.\n",
           ((double) timedelta_ns_from_roman) / timedelta_ns_to_roman);
}

void test_benchmark(void)
{
    const int64_t timedelta_ns_to_roman = test_benchmark_value_to_roman();
    test_benchmark_roman_to_value(timedelta_ns_to_roman);
}
