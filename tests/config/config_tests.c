#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "config/config.h"

#define TEST_CONFIG_FILE "../tests/test_files/good_config.json"

/* GLOBAL VARS */
struct evolve_config *config;

/* static int validate_gp_tree_config(struct selection_config *config) */
/* { */
/*  */
/*         return 0; */
/* } */

static int validate_selection_config(struct selection_config *config)
{
        int res = 0;

        res = strcmp(config->select, "default");
        mu_assert(res == 0, "Failed to get select!");

        return 0;
}

static int validate_crossover_config(struct crossover_config *config)
{
        int res = 0;
        float crossover_prob = 0.8;

        res = float_cmp(config->probability, &crossover_prob);
        mu_assert(res == 0, "Failed to get prob!");

        res = strcmp(config->pivot_index, "default");
        mu_assert(res == 0, "Failed to get pivot_index!");

        return 0;
}

static int validate_mutation_config(struct mutation_config *config)
{
        int res = 0;
        float mutation_prob = 0.1;

        res = float_cmp(config->probability, &mutation_prob);
        mu_assert(res == 0, "Failed to get prob!");

        res = strcmp(config->mutation_point, "default");
        mu_assert(res == 0, "Failed to get mutation_point!");

        return 0;
}

int test_parse_config()
{
        /* parse config file */
        config = load_config(TEST_CONFIG_FILE);
        mu_assert(config != NULL, "Failed to parse config!");

        validate_selection_config(config->selection);
        validate_crossover_config(config->crossover);
        validate_mutation_config(config->mutation);

        /* clean up */
        config_destroy(config);

        return 0;
}

void test_suite()
{
        mu_run_test(test_parse_config);
}

mu_run_test_suite(test_suite);
