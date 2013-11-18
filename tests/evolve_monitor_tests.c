#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>
#include <al/utils.h>
#include <dstruct/darray.h>

#include "test_utils.h"

#include "evolve.h"
#include "population.h"
#include "evolve_monitor.h"
#include "selection.h"
#include "ga/initialize.h"
#include "ga/crossover.h"
#include "ga/mutation.h"

#define TEST_SOLUTION "hello world!"


/* GLOBAL VAR */
struct evolve_monitor *m;
struct population *p;
float *crossover_prob;
float *mutate_prob;
volatile sig_atomic_t stop_signal = 0;

static float fitness_function(void *individual)
{
        char *target = TEST_SOLUTION;
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(individual); i++) {
                total += fabsf(roundf(target[i] - ((char *) individual)[i]));
        }

        return total;
}

static void sort_test_setup(int max_gen, int max_pop)
{
        /* run an evolution to fill the monitor struct */
        size_t chromo_sz = strlen(TEST_SOLUTION) + sizeof(char);
        p = population_create(
                chromo_sz,  /* param */
                122 * strlen(TEST_SOLUTION),  /* goal */
                max_pop,  /* max_pop */
                max_gen /* max_gen */
        );
        m = init_evolve_monitor(chromo_sz, 5, NULL);
        ga_population_initialize(p, randstr);

        crossover_prob = calloc(1, sizeof(float));
        *crossover_prob = 0.8;

        mutate_prob = calloc(1, sizeof(float));
        *mutate_prob = 0.2;

        run_evolution(
                &p,
                fitness_function,

                /* selection */
                roulette_wheel_selection,
                DEFAULT_SELECT,

                /* crossover */
                one_pt_crossover,
                crossover_prob,
                DEFAULT_PIVOT,

                /* mutation */
                mutate_str,
                mutate_prob,

                m,
                0,
                &stop_signal
        );
}

static void sort_test_teardown()
{
        population_destroy(&p, free);
        destroy_evolve_monitor(&m);
        free(crossover_prob);
        free(mutate_prob);
        p = NULL;
        m = NULL;
}

int test_init_evolve_monitor()
{
        int len = strlen(TEST_SOLUTION);
        size_t chromo_sz = sizeof(char) * len;
        long max_gen = 10;

        m = init_evolve_monitor(chromo_sz, max_gen, NULL);

        mu_assert(
                m->best_individuals->max == max_gen,
                "Darray size of best_individuals is invalid!"
        );
        mu_assert(
                m->best_scores->max == max_gen,
                "Darray size of best_scores is invalid!"
        );
        mu_assert(
                m->generations->max == max_gen,
                "Darray size of generations is invalid!"
        );
        mu_assert(
                m->convergence_rates->max == max_gen,
                "Darray size of generations is invalid!"
        );
        mu_assert(
                m->goal_distances->max == max_gen,
                "Darray size of generations is invalid!"
        );

        return 0;
}

int test_destroy_evolve_monitor()
{
        destroy_evolve_monitor(&m);
        mu_assert(m == NULL, "Failed to free evolve_monitor!");
        return 0;
}

int test_record_generation_stats()
{
        int chromo_sz = strlen(TEST_SOLUTION) * sizeof(char);
        struct population *p = population_create(
                chromo_sz,  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                5 /* max_gen */
        );
        m = init_evolve_monitor(chromo_sz, 5, NULL);
        ga_population_initialize(p, randstr);
        evaluate_individuals(fitness_function, &p);

        /* record generation stats */
        record_generation_stats(p, m, float_cmp_asc);

        /* asserts */
        float best_score = *(float *) darray_get(m->best_scores, 0);
        mu_assert(best_score != 0, "Failed to record best score!");

        mu_assert(
                m->best_individuals->end != 0,
                "best_individuals length should not be 0!"
        );
        mu_assert(
                m->best_scores->end != 0,
                "best_scores length should not be 0!"
        );
        mu_assert(
                m->generations->end != 0,
                "generations length should not be 0!"
        );

        mu_assert(m->best_individuals->max == 5, "individuals array is not 5!");
        mu_assert(m->best_scores->max == 5, "Scores array is not 5!");
        mu_assert(m->generations->max == 5, "Generations array is not 5!");

        /* clean up */
        population_destroy(&p, free);
        destroy_evolve_monitor(&m);
        return 0;
}

int test_sort_generation_stats()
{
        /* int res = 0; */
        int max_gen = 1;
        int max_pop = 10;

        sort_test_setup(max_gen, max_pop);

        /* sort population */
        debug("Before Stats Sort");
        print_generation_stats(m);

        /* sort_generation_stats(m, float_cmp_desc); */

        /* debug("After Stats Sort"); */
        /* print_generation_stats(m); */

        /* assert test */
        /* res = assert_sorted_gstats(m, float_cmp_desc); */
        /* mu_assert(res == 0, "Sort Generation Stats Failed!"); */

        sort_test_teardown();

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(10);  /* DO NOT CHANGE SEED - asserts are based on this seed */

        mu_run_test(test_init_evolve_monitor);
        mu_run_test(test_destroy_evolve_monitor);
        mu_run_test(test_record_generation_stats);
        mu_run_test(test_sort_generation_stats);
}

mu_run_test_suite(test_suite);
