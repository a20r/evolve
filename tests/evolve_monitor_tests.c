#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "evolve.h"
#include "evolve_utils.h"
#include "evolve_monitor.h"
#include "selection.h"
#include "utils.h"

/* GLOBAL VAR */
struct evolve_monitor *m;


static float fitness_function(char *chromosome)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += fabsf(roundf(target[i] - chromosome[i]));
        }

        return total;
}

int test_init_evolve_monitor()
{
        int len = strlen("hello world!");
        size_t chromo_sz = sizeof(char) * len;
        long max_gen = 10;

        m = init_evolve_monitor(chromo_sz, max_gen);

        mu_assert(
                m->best_chromosomes->max == max_gen,
                "Darray size of best_chromosomes is invalid!"
        );
        mu_assert(
                m->best_scores->max == max_gen,
                "Darray size of best_scores is invalid!"
        );
        mu_assert(
                m->generations->max == max_gen,
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
        int chromo_sz = strlen("hello world!") * sizeof(char);
        struct population *p = init_population(
                chromo_sz,  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                5 /* max_gen */
        );
        m = init_evolve_monitor(chromo_sz, 5);
        gen_init_chromosomes(&p, randstr);
        evaluate_chromosomes(fitness_function, &p);

        /* record generation stats */
        record_generation_stats(p, m);

        /* asserts */
        /* debug("best chromo: %s", (char *) darray_get(m->best_chromosomes, 0)); */
        /* mu_assert( */
        /*         strcmp(darray_get(m->best_chromosomes, 0), "EQxvj(ADnvQ/") == 0, */
        /*         "Failed to record best chromosome!" */
        /* ); */

        float best_score = *(float *) darray_get(m->best_scores, 0);
        mu_assert(best_score != 0, "Failed to record best score!");

        mu_assert(m->best_chromosomes->end == 0, "Chromosomes array is not 0!");
        mu_assert(m->best_scores->end == 0, "Scores array is not 0!");
        mu_assert(m->generations->end == 0, "Generations array is not 0!");

        mu_assert(m->best_chromosomes->max == 5, "Chromosomes array is not 5!");
        mu_assert(m->best_scores->max == 5, "Scores array is not 5!");
        mu_assert(m->generations->max == 5, "Generations array is not 5!");

        /* clean up */
        destroy_population(&p);
        destroy_evolve_monitor(&m);
        return 0;
}

int test_sort_generation_stats()
{
        int i = 0;
        float curr_score = 0;
        float prev_score = 0;
        int max_gen = 2;

        /* run an evolution to fill the monitor struct */
        int chromo_sz = strlen("hello world!");
        struct population *p = init_population(
                chromo_sz,  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                max_gen /* max_gen */
        );
        m = init_evolve_monitor(chromo_sz, max_gen);
        gen_init_chromosomes(&p, randstr);
        run_evolution(
                &p,
                fitness_function,
                0.8,
                0.2,
                m
        );

        /* #<{(| sort population |)}># */
        /* debug("Before Stats Sort"); */
        /* print_generation_stats(m); */

        /* sort_generation_stats(m, float_cmp); */
        /* prev_score = *(float *) darray_get(m->best_scores, 0); */

        /* debug("After Stats Sort"); */
        /* print_generation_stats(m); */

        /* #<{(| assert tests |)}># */
        /* for (i = 1; i < m->generations->end; i++) { */
        /*         curr_score = *(float *) darray_get(m->best_scores, i); */
        /*         mu_assert(curr_score <= prev_score, "Failed to sort scores!"); */
        /*         prev_score = curr_score; */
        /* } */

        destroy_population(&p);
        destroy_evolve_monitor(&m);
        return 0;
}

void test_suite()
{
        mu_run_test(test_init_evolve_monitor);
        mu_run_test(test_destroy_evolve_monitor);
        mu_run_test(test_record_generation_stats);
        mu_run_test(test_sort_generation_stats);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(10);  /* DO NOT CHANGE SEED - asserts are based on this seed */
        test_suite();
        mu_report();
}