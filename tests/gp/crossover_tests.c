#include <munit/munit.h>

#include "gp/initialize.h"
#include "gp/crossover.h"
#include "gp/tree_parser.h"

#define TEST_CONFIG_FILE "tests/test_files/mutation_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct gp_tree *tree;


static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;
        tree = init_tree_full(gp_config);
}

static void teardown()
{
        gp_tree_destroy(tree);
        config_destroy(config);
}

int test_point_mutation()
{
        int i = 0;
        int tests = 100;

        setup();

        printf("Before Crossover!\n");
        printf("------------------------------\n");
        print_gp_tree(tree->root);
        printf("\n");
        print_gp_program(tree->program);

        crossover_trees(1, tree, tree, gp_config, one_point_crossover);

        printf("After Crossover!\n");
        printf("------------------------------\n");
        print_gp_tree(tree->root);
        printf("\n");
        print_gp_program(tree->program);

        teardown();



        return 0;
}

void test_suite()
{
        mu_run_test(test_point_mutation);
}

mu_run_test_suite(test_suite);
