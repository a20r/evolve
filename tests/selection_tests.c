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

#include "ga/initialize.h"
#include "ga/crossover.h"
#include "ga/mutation.h"

#include "gp/initialize.h"
#include "gp/data_loader.h"
#include "gp/crossover.h"
#include "gp/mutation.h"
#include "gp/tree_parser.h"
#include "gp/tree_evaluator.h"

#define TEST_OUTPUT "evolve_utils_tests.out"
#define CONFIG_FILE "examples/sym_regression/config.json"
#define TEST_INPUT_DATA "examples/sym_regression/data/sine_input.dat"
#define TEST_RESPONSE_DATA "examples/sym_regression/data/sine_response.dat"

/* GLOBAL VAR */
struct population *p;


static float fitness_function(void *individual)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(individual); i++) {
                total += fabsf(roundf(target[i] - ((char *) individual)[i]));
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
        ga_population_initialize(p, randstr);
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

int test_roulette_wheel_selection_gp()
{
        int i = 0;
        int individuals = 0;
        struct evolve_config *config = NULL;
        struct evolve_monitor *m = NULL;
        struct population *p = NULL;
        struct gp_tree *tree = NULL;

        /* setup */
        config = load_config(CONFIG_FILE);
        load_data(TEST_INPUT_DATA, config, INPUT_DATA);
        load_data(TEST_RESPONSE_DATA, config, RESPONSE_DATA);
        p = gp_population_initialize(init_tree_full, config->general.gp_tree);
        evaluate_programs(p, config);

        /* select */
        roulette_wheel_selection(&p, NULL);


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
