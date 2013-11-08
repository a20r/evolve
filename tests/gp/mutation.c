#include <time.h>
#include <math.h>
#include <string.h>

#include <dstruct/darray.h>
#include <munit/munit.h>
#include <al/utils.h>

#include "config/config.h"
#include "evolve.h"
#include "gp/initialize.h"

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

int test_one_point_crossover()
{
        int i = 0;
        struct gp_tree *tree;

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        setup();
        mu_run_test(test_one_point_crossover);
        teardown();
}

mu_run_test_suite(test_suite);
