#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <munit/munit.h>
#include <al/comparator.h>
#include <al/utils.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "evolve_monitor.h"
#include "population.h"
#include "selection.h"
#include "ga/crossover.h"
#include "ga/mutation.h"

#define TARGET_SOLUTION "hello world!"

/* GLOBAL VAR */
volatile sig_atomic_t stop_signal = 0;


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

int test_run_evolution()
{
        struct population *p;
        struct evolve_monitor *m;
        int max_pop = 30;
        int max_gen = 20;
        float *crossover_prob;
        float *mutate_prob;

        p = init_population(
                (int) strlen(TARGET_SOLUTION),  /* param */
                122 * strlen(TARGET_SOLUTION),  /* goal */
                max_pop,  /* max_pop */
                max_gen  /* max_gen */
        );
        m = init_evolve_monitor(
                p->chromosomes->element_size,  /* chromosome size */
                5,
                NULL
        );

        /* init crossover and mutation probability */
        crossover_prob = calloc(1, sizeof(float));
        mutate_prob = calloc(1, sizeof(float));
        *crossover_prob = 0.5;
        *mutate_prob = 0.2;

        gen_init_chromosomes(&p, randstr);
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

                /* monitor */
                m,
                0,
                &stop_signal
        );
        destroy_population(&p);

        mu_assert(p == NULL, "Population should be NULL!");

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        mu_run_test(test_run_evolution);
}

mu_run_test_suite(test_suite);
