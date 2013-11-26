#include <time.h>
#include <math.h>
#include <string.h>

#include <dstruct/darray.h>
#include <munit/munit.h>
#include <al/utils.h>

#include "config/config.h"
#include "evolve.h"
#include "population.h"
#include "selection.h"
#include "gp/initialize.h"
#include "gp/tree_parser.h"
#include "gp/tree_validator.h"

#define TEST_CONFIG_FILE "tests/test_files/gp_initialize_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct population *p;
struct gp_tree *gp;


static void setup()
{
        int i = 0;
        struct ast *node;

        config = load_config(TEST_CONFIG_FILE);

        printf("--- TERMINAL NODES ---\n");
        for (i = 0; i < config->general.gp_tree->function_set->end; i++) {
                node = darray_get(config->general.gp_tree->function_set, i);
                print_node(node);
                printf("\n");
        }

        gp_config = config->general.gp_tree;
}

static void teardown()
{
        gp_tree_destroy(gp);
        config_destroy(config);
}

int test_init_tree_full()
{
        gp = init_tree_full(gp_config);
        print_gp_tree(gp->root);

        mu_assert(gp != NULL, "Failed to intialize gp tree!");
        mu_assert(gp->program != NULL, "Failed to intialize gp tree!");
        mu_assert(gp->size > 2, "Failed to intialize gp tree!");
        mu_assert(gp->depth == 2, "Failed to intialize gp tree!");

        return 0;
}

int test_init_tree_grow()
{
        return 0;
}

int test_init_tree_ramped_half_and_half()
{
        return 0;
}

int test_population_initialize()
{
        int i = 0;
        int res = 0;
        struct gp_tree *tree;

        p = gp_population_initialize(init_tree_full, gp_config);

        for (i = 0; i < p->max_population; i++) {
                tree = darray_get(p->individuals, i);
                res = tree_validate(tree, gp_config->input_set);
                if (res == 0) {
                        printf("- TREE[%d]", i);
                        printf(" [size: %d]", tree->size);
                        printf("[depth: %d]\n", tree->depth);
                        print_gp_tree(tree->root);
                        printf("\n\n");
                }
                print_gp_tree(tree->root);
                printf("\n\n");
                mu_assert(res == 1, "Invalid GP TREE!");
        }
        population_destroy(&p, gp_tree_destroy);

        return 0;
}

void test_suite()
{
        setup();

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        mu_run_test(test_init_tree_full);
        /* mu_run_test(test_init_tree_grow); */
        /* mu_run_test(test_init_tree_ramped_half_and_half); */
        mu_run_test(test_population_initialize);

        teardown();
}

mu_run_test_suite(test_suite);
