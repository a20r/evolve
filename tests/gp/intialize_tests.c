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

#define TEST_CONFIG_FILE "tests/test_files/init_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct population *p;
struct gp_tree *gp;


static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
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
        print_gp_tree(gp->tree);

        mu_assert(gp != NULL, "Failed to intialize gp tree!");
        mu_assert(gp->size == 7, "Failed to intialize gp tree!");
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
        struct gp_tree *tree;

        p = population_initialize(init_tree_full, gp_config);

        for (i = 0; i < p->max_population; i++) {
                tree = darray_get(p->individuals, i);
                printf("- TREE[%d]", i);
                printf(" [size: %d]", tree->size);
                printf("[depth: %d]\n", tree->depth);
                print_gp_tree(tree->tree);
                printf("\n\n");
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
