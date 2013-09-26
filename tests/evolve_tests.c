#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <munit/munit.h>
#include <al/comparator.h>
#include <al/utils.h>

#include "evolve.h"
#include "population.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"

/* GLOBAL VAR */
struct population *p;
struct evolve_monitor *m;
float *crossover_prob;
float *mutate_prob;
int max_pop = 10;
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
        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                max_pop,  /* max_pop */
                2 /* max_gen */
        );
        gen_init_chromosomes(&p, randstr);

        crossover_prob = calloc(1, sizeof(float));
        *crossover_prob = 0.5;

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

                /* monitor */
                NULL,
                0,
                &stop_signal
        );
        destroy_population(&p);

        mu_assert(p == NULL, "Population should be NULL!");

        return 0;
}

void test_suite()
{
        mu_run_test(test_run_evolution);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
