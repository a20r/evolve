#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "evolve.h"
#include "population.h"
#include "selection.h"
#include "utils.h"

/* GLOBAL VAR */
struct population *p;
struct evolve_monitor *m;
int max_pop = 10;


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
        run_evolution(
                &p,
                fitness_function,
                0.5,
                0.2,
                NULL
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
