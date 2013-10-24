#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "evolve_config.h"

#define TEST_CONFIG_FILE "../tests/test_files/good_config.json"

/* GLOBAL VARS */
struct evolve_config *config;

int test_parse_config()
{
        struct selection_config *selection;
        struct crossover_config *crossover;
        struct mutation_config *mutation;

        float crossover_prob = 0.8;
        float mutation_prob = 0.1;

        /* parse config file */
        config = parse_config(TEST_CONFIG_FILE);
        mu_assert(config != NULL, "Failed to parse config!");

        selection = config->selection;
        crossover = config->crossover;
        mutation = config->mutation;

        /* mode */
        printf("SELECT: %s\n", selection->select);
        mu_assert(
                strcmp(selection->select, "default") == 0,
                "Failed to get select!"
        );

        /* select */
        mu_assert(
                strcmp(selection->select, "default") == 0,
                "Failed to get select!"
        );

        /* crossover */
        mu_assert(
                float_cmp(crossover->probability, &crossover_prob) == 0,
                "Failed to get prob!"
        );
        mu_assert(
                strcmp(crossover->pivot_index, "default") == 0,
                "Failed to get pivot_index!"
        );

        /* mutation */
        mu_assert(
                float_cmp(mutation->probability, &mutation_prob) == 0,
                "Failed to get prob!"
        );
        mu_assert(
                strcmp(mutation->mutation_point, "default") == 0,
                "Failed to get mutation_point!"
        );

        /* clean up */
        config_destroy(config);

        return 0;
}

void test_suite()
{
        mu_run_test(test_parse_config);
}

mu_run_test_suite(test_suite);
