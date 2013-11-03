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
       /* destroy_population(&p); */
}

static int dot_graph_cb(struct ast *node)
{
        if (node->tag == INTEGER) {
                printf("NODE: %d\n", node->type.integer);
        } else if (node->tag == REAL) {
                printf("NODE: %f\n", node->type.real);
        } else if (node->tag == STRING) {
                printf("NODE: %s\n", node->type.string);
        } else if (node->tag == CHARACTER) {
                printf("NODE: %c\n", node->type.character);
        } else if (node->tag == BOOL) {
                printf("NODE: %d\n", node->type.boolean);
        } else if (node->tag == UNARY_OP) {
                printf("NODE: %s\n", node->type.unary->op_name);
        } else if (node->tag == BINARY_OP || node->tag == START) {
                printf("NODE: %s\n", node->type.binary->op_name);
        }

        return 0;
}

int test_init_tree_full()
{
        gp = init_tree_full(gp_config);
        ast_traverse(gp->tree, dot_graph_cb);

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

        teardown();
}

mu_run_test_suite(test_suite);
