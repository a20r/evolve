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
        for (i = 0; i < program->end; i++) {
                node = darray_get(program, i);
                test_print_node(node);
        }
}

static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;
        gp = init_tree_full(config->general.gp_tree);
        program = parse_gp_tree(gp->root);
        print_program();
}

static void teardown()
{
        darray_destroy(program);
        gp_tree_destroy(gp);
        config_destroy(config);
}

int test_terminal_nodes_equal()
{
        int res = 0;
        int integer_1 = 1;
        int integer_2 = 1;
        float real_1 = 1.0;
        float real_2 = 1.0;
        char *str_1 = "Hello World!";
        char *str_2 = "Hello World!";
        char c_1 = 'a';
        char c_2 = 'a';
        int b_1 = 0;
        int b_2 = 0;
        struct ast *node_1;
        struct ast *node_2;

        /* compare integer nodes */
        /* equal test */
        node_1 = ast_make_exp(INTEGER, &integer_1);
        node_2 = ast_make_exp(INTEGER, &integer_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 1, "Failed to compare INTEGER terminal nodes!");
        free(node_1);
        free(node_2);

        /* fail test */
        integer_2 = 2;
        node_1 = ast_make_exp(INTEGER, &integer_1);
        node_2 = ast_make_exp(INTEGER, &integer_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 0, "Failed to compare INTEGER terminal nodes!");
        free(node_1);
        free(node_2);

        /* compare real nodes */
        /* equal test */
        node_1 = ast_make_exp(REAL, &real_1);
        node_2 = ast_make_exp(REAL, &real_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 1, "Failed to compare real terminal nodes!");
        free(node_1);
        free(node_2);

        /* fail test */
        real_2 = 2.0;
        node_1 = ast_make_exp(REAL, &real_1);
        node_2 = ast_make_exp(REAL, &real_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 0, "Failed to compare real terminal nodes!");
        free(node_1);
        free(node_2);

        /* compare string nodes */
        /* equal test */
        node_1 = ast_make_exp(STRING, &str_1);
        node_2 = ast_make_exp(STRING, &str_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 1, "Failed to compare STRING terminal nodes!");
        free(node_1);
        free(node_2);

        /* fail test */
        str_2 = "Hello C!";
        node_1 = ast_make_exp(STRING, &str_1);
        node_2 = ast_make_exp(STRING, &str_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 0, "Failed to compare STRING terminal nodes!");
        free(node_1);
        free(node_2);

        /* compare character nodes */
        /* equal test */
        node_1 = ast_make_exp(CHARACTER, &c_1);
        node_2 = ast_make_exp(CHARACTER, &c_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 1, "Failed to compare CHARACTER terminal nodes!");
        free(node_1);
        free(node_2);

        /* fail test */
        c_2 = 'b';
        node_1 = ast_make_exp(CHARACTER, &c_1);
        node_2 = ast_make_exp(CHARACTER, &c_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 0, "Failed to compare CHARACTER terminal nodes!");
        free(node_1);
        free(node_2);

        /* compare boolean nodes */
        /* equal test */
        node_1 = ast_make_exp(BOOL, &b_1);
        node_2 = ast_make_exp(BOOL, &b_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 1, "Failed to compare BOOL terminal nodes!");
        free(node_1);
        free(node_2);

        /* fail test */
        b_2 = 'b';
        node_1 = ast_make_exp(BOOL, &b_1);
        node_2 = ast_make_exp(BOOL, &b_2);
        res = terminal_nodes_equal(node_1, node_2);
        mu_assert(res == 0, "Failed to compare BOOL terminal nodes!");
        free(node_1);
        free(node_2);

        return 0;
}

int test_validate_tree()
{
        int i;
        int len;
        int res;

        printf("--- TERMINAL NODES ---\n");
        len = gp->terminal_nodes->end;
        for (i = 0; i < len; i++) {
                test_print_node(darray_get(gp->terminal_nodes, i));
        }

        printf("--- INPUT SET ---\n");
        len = gp_config->input_set->end + 1;
        for (i = 0; i < len; i++) {
                test_print_node(darray_get(gp_config->input_set, i));
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
        mu_run_test(test_terminal_nodes_equal);
        mu_run_test(test_validate_tree);
        teardown();
}

mu_run_test_suite(test_suite);
