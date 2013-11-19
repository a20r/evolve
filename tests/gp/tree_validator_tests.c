#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/ast.h>
#include <dstruct/darray.h>
#include <dstruct/stack.h>

#include "config/config.h"
#include "population.h"
#include "gp/initialize.h"
#include "gp/function_set.h"
#include "gp/terminal_set.h"
#include "gp/tree_parser.h"
#include "gp/tree_evaluator.h"
#include "gp/tree_validator.h"

#define TEST_CONFIG_FILE "tests/test_files/validation_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct gp_tree *gp;
struct darray *program;


static int test_print_node(struct ast *node)
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

static void print_program()
{
        int i;
        struct ast *node;

        /* print program */
        printf("PROGRAM STACK [REVERSE POLISH NOTATION]\n");
        for (i = 0; i < gp->program->end; i++) {
                node = darray_get(gp->program, i);
                test_print_node(node);
        }
}

static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;

        gp = init_tree_full(config->general.gp_tree);
        print_program();
}

static void teardown()
{
        darray_destroy(program);
        gp_tree_destroy(gp);
        config_destroy(config);
}

int test_validate_tree()
{
        int i;
        int len;
        int res = 0;

        printf("--- TERMINAL NODES ---\n");
        len = gp->terminal_nodes->end;
        for (i = 0; i < len; i++) {
                test_print_node(darray_get(gp->terminal_nodes, i));
        }

        printf("--- INPUT NODES ---\n");
        len = gp->input_nodes->end;
        for (i = 0; i < len; i++) {
                test_print_node(darray_get(gp->input_nodes, i));
        }

        res = tree_validate(gp, gp_config->input_set);
        mu_assert(res == 1, "Failed to validate tree!");

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        setup();
        mu_run_test(test_validate_tree);
        teardown();
}

mu_run_test_suite(test_suite);
