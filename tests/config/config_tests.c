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

static int validate_gp_tree_config(struct gp_tree_config *config)
{
        int res = 0;
        int i = 0;
        int *max_pop_solution;
        int *max_gen_solution;
        int *max_depth_solution;
        int *max_size_solution;
        struct darray *function_set;
        struct darray *terminal_set;
        char *value;
        int array_size;

        /* setup */
        max_pop_solution = calloc(1, sizeof(int));
        max_gen_solution = calloc(1, sizeof(int));
        max_depth_solution = calloc(1, sizeof(int));
        max_size_solution = calloc(1, sizeof(int));
        *max_pop_solution = 1000;
        *max_gen_solution = 1000;
        *max_depth_solution = 10;
        *max_size_solution = 100;

        /* max pop */
        res = int_cmp(config->max_pop, max_pop_solution);
        mu_assert(res == 0, "Invalid max_pop!");

        /* max gen */
        res = int_cmp(config->max_gen, max_gen_solution);
        mu_assert(res == 0, "Invalid max_gen!");

        /* max depth */
        res = int_cmp(config->max_depth, max_depth_solution);
        mu_assert(res == 0, "Invalid max_depth!");

        /* max size */
        res = int_cmp(config->max_size, max_size_solution);
        mu_assert(res == 0, "Invalid max_size!");

        /* function set */
        function_set = config->function_set;
        array_size = function_set->end + 1;
        for (i = 0; i < array_size; i++) {
                value = (char *) darray_get(function_set, i);
                printf("%d: %s\n", i, value);
                mu_assert(value != NULL, "Invalid value!");
        }
        printf("\n");

        /* terminal set */
        terminal_set = config->terminal_set;
        array_size = terminal_set->end + 1;
        for (i = 0; i < array_size; i++) {
                value = (char *) darray_get(terminal_set, i);
                printf("%d: %s\n", i, value);
                mu_assert(value != NULL, "Invalid value!");
        }

        return 0;
}

static int validate_selection_config(struct selection_config *config)
{
        int res = 0;

        res = strcmp(config->select, "default");
        mu_assert(res == 0, "Invalid select!");

        return 0;
}

static int validate_crossover_config(struct crossover_config *config)
{
        int res = 0;
        float crossover_prob = 0.8;

        res = float_cmp(config->probability, &crossover_prob);
        mu_assert(res == 0, "Invalid prob!");

        res = strcmp(config->pivot_index, "default");
        mu_assert(res == 0, "Invalid pivot_index!");

        return 0;
}

static int validate_mutation_config(struct mutation_config *config)
{
        int res = 0;
        float mutation_prob = 0.1;

        res = float_cmp(config->probability, &mutation_prob);
        mu_assert(res == 0, "Invalid prob!");

        res = strcmp(config->mutation_point, "default");
        mu_assert(res == 0, "Invalid mutation_point!");

        return 0;
}

int test_parse_config()
{
        int res = 0;

        /* parse config file */
        config = load_config(TEST_CONFIG_FILE);
        mu_assert(config != NULL, "Failed to parse config!");

        /* validate config */
        res = validate_gp_tree_config(config->general.gp_tree);
        mu_assert(res == 0, "Failed to validate gp_tree_config details!");

        res = validate_selection_config(config->selection);
        mu_assert(res == 0, "Failed to validate selection details!");

        res = validate_crossover_config(config->crossover);
        mu_assert(res == 0, "Failed to validate crossover details!");

        res = validate_mutation_config(config->mutation);
        mu_assert(res == 0, "Failed to validate mutation details!");


        /* clean up */
        config_destroy(config);

        return 0;
}

void test_suite()
{
        mu_run_test(test_parse_config);
}

mu_run_test_suite(test_suite);
