#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "selection.h"
#include "evolve.h"
#include "utils.h"

/* GLOBAL VAR */
struct population *p;

static void setup()
{
        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                1 /* max_gen */
        );
        gen_init_chromosomes(&p, randstr);
}

static void teardown()
{
        destroy_population(&p);
}

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

int test_roulette_selection()
{
        setup();

        evaluate_chromosomes(fitness_function, &p);
        p = roulette_wheel_selection(&p, NULL);

        mu_assert(p->curr_population == 5, "Current population should be 5!");
        mu_assert(p->max_population == 10, "Maximum population should be 10!");

        return 0;
}

void test_suite()
{
        mu_run_test(test_roulette_selection);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
