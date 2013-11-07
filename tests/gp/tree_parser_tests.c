#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/utils.h>
#include <dstruct/darray.h>
#include <dstruct/queue.h>

#include "config/config.h"
#include "population.h"
#include "gp/initialize.h"
#include "gp/tree_parser.h"

#define TEST_CONFIG_FILE "tests/test_files/init_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
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

static int print_node(struct ast *node)
{
        if (node->tag == INTEGER) {
                printf("NODE[INTEGER]: %d\n", node->type.integer);
        } else if (node->tag == REAL) {
                printf("NODE[REAL]: %f\n", node->type.real);
        } else if (node->tag == STRING) {
                printf("NODE[STRING]: %s\n", node->type.string);
        } else if (node->tag == CHARACTER) {
                printf("NODE[CHARACTER]: %c\n", node->type.character);
        } else if (node->tag == BOOL) {
                printf("NODE[BOOL]: %i\n", node->type.boolean);
        } else if (node->tag == UNARY_OP) {
                printf("NODE[UNARY_OP]: %s\n", node->type.unary->op_name);
        } else if (node->tag == BINARY_OP || node->tag == START) {
                printf("NODE[BINARY_OP]: %s\n", node->type.binary->op_name);
        } else {
                log_err("OPPS! Unknown node type to print!");
                return -1;
        }

        return 0;
}

int test_print_gp_tree()
{
        int res = 0;

        res = print_gp_tree(gp->tree);
        mu_assert(gp != NULL, "Failed to print gp tree!");
        mu_assert(res == 0, "Failed to print gp tree!");

        return 0;
}

int test_parse_gp_tree()
{
        int i = 0;
        int len = 0;
        int status = 0;
        struct queue *q;
        struct ast *node;

        q = parse_gp_tree(gp->tree);
        mu_assert(q != NULL, "Failed to parse gp tree!");

        len = q->count;
        for (i = 0; i < len; i++) {
                node = queue_dequeue(q);
                mu_assert(node != NULL, "Node from q should not be NULL!");

                status = print_node(node);
                mu_assert(status != -1, "Unrecognised node!");

                ast_destroy(node);
        }
        queue_destroy(q);

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        setup();

        mu_run_test(test_print_gp_tree);
        mu_run_test(test_parse_gp_tree);

        teardown();
}

mu_run_test_suite(test_suite);
