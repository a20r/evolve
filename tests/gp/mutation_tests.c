#include <time.h>
#include <math.h>
#include <string.h>

#include <dstruct/darray.h>
#include <munit/munit.h>
#include <al/utils.h>

#include "config/config.h"
#include "evolve.h"
#include "gp/initialize.h"
#include "gp/mutation.h"
#include "gp/tree_parser.h"

#define TEST_CONFIG_FILE "tests/test_files/mutation_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct gp_tree *gp;


static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;
        gp = init_tree_full(gp_config);
}

static void teardown()
{
        gp_tree_destroy(gp);
        config_destroy(config);
}

int test_point_mutation()
{
        printf("Before Mutation!\n");
        printf("------------------------------\n");
        print_gp_tree(gp->root);
        printf("\n");
        print_gp_program(gp->program);
        mutate_tree(1, gp, gp_config, point_mutation);

        printf("\nAfter Mutation!\n");
        printf("------------------------------\n");
        print_gp_tree(gp->root);
        printf("\n");
        /* print_gp_program(gp->program); */

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        setup();
        mu_run_test(test_point_mutation);
        teardown();
}

mu_run_test_suite(test_suite);
