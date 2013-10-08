#include <time.h>
#include <math.h>
#include <string.h>

#include <dstruct/darray.h>
#include <munit/munit.h>
#include <al/utils.h>

#include "ga/evolve.h"
#include "ga/population.h"
#include "ga/selection.h"

/* GLOBAL VAR */
struct population *p;


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

static void setup()
{
        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                1 /* max_gen */
        );
        gen_init_chromosomes(&p, randstr);
        evaluate_chromosomes(fitness_function, &p);
}

static void teardown()
{
        destroy_population(&p);
}

int test_roulette_wheel_selection()
{
        setup();

        print_population(p);
        roulette_wheel_selection(&p, NULL);
        print_population(p);

        mu_assert(p->chromosomes->end == 5, "End should be 5!");
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

        mu_assert(p->chromosomes->end == 5, "End should be 5!");
        mu_assert(p->scores->end == 5, "End should be 5!");
        mu_assert(p->population == 6, "Current population should be 6!");
        mu_assert(p->max_population == 10, "Max population should be 10!");

        teardown();
        return 0;
}

void test_suite()
{
        mu_run_test(test_roulette_wheel_selection);
        mu_run_test(test_tournament_selection);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
