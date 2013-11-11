#include <time.h>
#include <math.h>
#include <string.h>

#include <dstruct/darray.h>
#include <munit/munit.h>
#include <al/utils.h>

#include "evolve.h"
#include "population.h"
#include "selection.h"
#include "ga/initialize.h"

/* GLOBAL VAR */
struct population *p;


static float fitness_function(char *individual)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(individual); i++) {
                total += fabsf(roundf(target[i] - individual[i]));
        }

        return total;
}

static void setup()
{
        p = population_create(
                (int) strlen("hello world!") + sizeof(char),
                0.0,  /* goal */
                10,  /* max_pop */
                1 /* max_gen */
        );
        initialize_population(p, randstr);
        evaluate_individuals(fitness_function, &p);
}

static void teardown()
{
        population_destroy(&p, free);
}

int test_roulette_wheel_selection()
{
        setup();

        print_population(p);
        roulette_wheel_selection(&p, NULL);
        print_population(p);

        mu_assert(p->individuals->end == 5, "End should be 5!");
        mu_assert(p->scores->end == 5, "End should be 5!");
        mu_assert(p->population == 6, "Current population should be 6!");
        mu_assert(p->max_population == 10, "Max population should be 10!");

        teardown();
        return 0;
}

int test_tournament_selection()
{
        setup();

        print_population(p);
        tournament_selection(&p, NULL);
        print_population(p);

        mu_assert(p->individuals->end == 5, "End should be 5!");
        mu_assert(p->scores->end == 5, "End should be 5!");
        mu_assert(p->population == 6, "Current population should be 6!");
        mu_assert(p->max_population == 10, "Max population should be 10!");

        teardown();
        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        mu_run_test(test_roulette_wheel_selection);
        mu_run_test(test_tournament_selection);
}

mu_run_test_suite(test_suite);
