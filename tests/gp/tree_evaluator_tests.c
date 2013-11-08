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

#define TEST_CONFIG_FILE "tests/test_files/test_eval.json"

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

        /* original points to copy */
        program = program_copy;
}

static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;
        gp = init_tree_full(config->general.gp_tree);
        program = parse_gp_tree(gp->tree);
        /* print_program(); */
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

int test_evaluate_node()
{
        struct stack *s;
        struct ast *node;
        struct ast *result;
        float real;
        float solution;
        int status = 0;
        int cmp_res = 0;

        /* setup */
        s = stack_create(0);
        solution = 6.0;

        /* push a real number to stack */
        real = 2.0;
        node = ast_make_exp(REAL, &real);

        status = evaluate_node(node, s);
        mu_assert(status == 0, "Error during node evaluation!");
        mu_assert(s->count == 1, "Wrong stack count!");

        /* push a second real number to stack */
        real = 4.0;
        node = ast_make_exp(REAL, &real);
        status = evaluate_node(node, s);

        mu_assert(status == 0, "Error during node evaluation!");
        mu_assert(s->count == 2, "Wrong stack count!");

        /* test add */
        node = ast_make_binary_exp("ADD", NULL, NULL);
        evaluate_node(node, s);
        mu_assert(s->count == 1, "Wrong stack count!");

        result = stack_pop(s);
        printf("result: %f\n", result->type.real);
        mu_assert(s->count == 0, "Wrong stack count!");

        cmp_res = float_cmp(&result->type.real, &solution);
        mu_assert(cmp_res == 0, "Wrong stack count!");

        /* clean up */
        ast_destroy(node);
        ast_destroy(result);
        stack_destroy(s);

        return 0;
}

int test_evaluate_program()
{
        struct ast *res;

        print_gp_tree(gp->tree);
        res = evaluate_program(program);
        mu_assert(res != NULL, "Failed to evaluate program!");

        if (res->tag == REAL) {
                printf("FINAL RESULT: %f\n", res->type.real);
        } else if (res->tag == INTEGER) {
                printf("FINAL RESULT: %d\n", res->type.integer);
        }

        /* clean up */
        ast_destroy(res);

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        setup();

        /* mu_run_test(test_evaluate_node); */
        mu_run_test(test_evaluate_program);

        teardown();
}

mu_run_test_suite(test_suite);