#include <munit/munit.h>

#include "gp/initialize.h"
#include "gp/mutation.h"

#define TEST_CONFIG_FILE "tests/test_files/gp_mutation_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree *gp;


static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp = init_tree_full(config->general.gp_tree);
}

static void teardown()
{
        gp_tree_destroy(gp);
        config_destroy(config);
}

int test_point_mutation()
{
        int i = 0;
        int tests = 1;
        /* struct ast *root_before; */
        /* struct ast *root_after; */
        /* struct darray *prog_before; */
        /* struct darray *prog_after; */

        /* mutate tree n amount of times */
        for (i = 0; i < tests; i++) {
                setup();

                /* root_before = gp->root; */
                /* prog_before = gp->program; */

                mutate_tree(gp, gp_point_mutation, config);

                /* root_after = gp->root; */
                /* prog_after = gp->program; */

                teardown();
        }

        /* printf("Before Mutation!\n"); */
        /* printf("------------------------------\n"); */
        /* print_gp_tree(gp->root); */
        /* printf("\n"); */
        /* print_gp_program(gp->program); */

        /* printf("\nAfter Mutation!\n"); */
        /* printf("------------------------------\n"); */
        /* print_gp_tree(gp->root); */
        /* printf("\n"); */
        /* print_gp_program(gp->program); */

        return 0;
}

void test_suite()
{
        mu_run_test(test_point_mutation);
}

mu_run_test_suite(test_suite);
