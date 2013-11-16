#include <munit/munit.h>

#include "gp/initialize.h"
#include "gp/crossover.h"
#include "gp/tree_parser.h"

#define TEST_CONFIG_FILE "tests/test_files/mutation_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct gp_tree *tree_1;
struct gp_tree *tree_2;


static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;
        tree_1 = init_tree_full(gp_config);
        tree_2 = init_tree_full(gp_config);
}

static void teardown()
{
        gp_tree_destroy(tree_1);
        gp_tree_destroy(tree_2);
        config_destroy(config);
}

int test_point_mutation()
{
        int i = 0;
        int tests = 100;

        setup();

        printf("Before Crossover!\n");
        printf("------------------------------\n");
        print_gp_program(tree_1->program);
        print_gp_program(tree_2->program);

        crossover_trees(1, tree_1, tree_2, gp_config, one_point_crossover);

        printf("After Crossover!\n");
        printf("------------------------------\n");
        print_gp_program(tree_1->program);
        print_gp_program(tree_2->program);

        teardown();



        return 0;
}

void test_suite()
{
        mu_run_test(test_point_mutation);
}

mu_run_test_suite(test_suite);
