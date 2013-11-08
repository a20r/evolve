#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/ast.h>
#include <dstruct/darray.h>
#include <dstruct/stack.h>
#include <dstruct/queue.h>

#include "config/config.h"
#include "population.h"
#include "gp/initialize.h"
#include "gp/tree_parser.h"
#include "gp/tree_evaluator.h"
#include "gp/tree_validator.h"

#define TEST_CONFIG_FILE "tests/test_files/validation_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct gp_tree *gp;
struct queue *program;


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

static void print_program()
{
        int i;
        int len;
        struct ast *node;
        struct queue *program_copy;

        /* setup */
        program_copy = queue_create(0);
        len = program->count;

        /* print program */
        printf("PROGRAM STACK [REVERSE POLISH NOTATION]\n");
        for (i = 0; i < len; i++) {
                node = queue_dequeue(program);
                print_node(node);
                queue_enqueue(program_copy, node);
        }
        queue_destroy(program);

        /* original points to copy */
        program = program_copy;
}

static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;
        gp = init_tree_full(config->general.gp_tree);
        program = parse_gp_tree(gp->tree);
        print_program();
}

static void teardown()
{
        int i;
        struct ast *node;
        int prog_len = program->count;

        for (i = 0; i < prog_len; i++) {
               node = queue_dequeue(program);
               ast_destroy(node);
        }
        queue_destroy(program);

        gp_tree_destroy(gp);
        config_destroy(config);
}

int test_validate_tree()
{
        int i;
        int len;
        int res;

        printf("--- TERMINAL NODES ---\n");
        len = gp->terminal_nodes->end;
        for (i = 0; i < len; i++) {
                print_node(darray_get(gp->terminal_nodes, i));
        }

        printf("--- INPUT SET ---\n");
        len = gp_config->input_set->end + 1;
        for (i = 0; i < len; i++) {
                print_node(darray_get(gp_config->input_set, i));
        }

        res = validate_tree(gp, gp_config->input_set);
        printf("res: %d\n", res);


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
