#include <munit/munit.h>
#include <dstruct/ast_cmp.h>

#include "gp/initialize.h"
#include "gp/crossover.h"
#include "gp/tree_parser.h"

#define TEST_CONFIG_FILE "tests/test_files/gp_crossover_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
struct gp_tree_config *gp_config;
struct gp_tree *tree_1;
struct gp_tree *tree_2;


static void setup()
{
        config = load_config(TEST_CONFIG_FILE);
        gp_config = config->general.gp_tree;
        tree_1 = init_tree_full(gp_config);
        tree_2 = init_tree_full(gp_config);
}

static void teardown()
{
        gp_tree_destroy(tree_1);
        gp_tree_destroy(tree_2);
        config_destroy(config);
}

static void print_term_nodes(struct gp_tree *tree)
{
        int i = 0;

        printf("terminal nodes[%d]\n", tree->terminal_nodes->end);
        for (i = 0; i < tree->terminal_nodes->end; i++) {
                print_node(darray_get(tree->terminal_nodes, i));
                printf("\n");
        }
}

int test_point_mutation()
{
        int res = 0;
        struct ast *t_1_before;
        struct ast *t_2_before;
        struct ast *t_1_after;
        struct ast *t_2_after;


        setup();

        printf("Before Crossover!\n");
        printf("------------------------------\n");

        printf("TREE 1\n");
        print_gp_program(tree_1->program);
        print_term_nodes(tree_1);

        printf("\nTREE 2\n");
        print_gp_program(tree_2->program);
        print_term_nodes(tree_2);

        /* crossover */
        t_1_before = ast_copy_node(tree_1->root);
        t_2_before = ast_copy_node(tree_2->root);
        crossover_trees(tree_1, tree_2, one_point_crossover, config);
        t_1_after = ast_copy_node(tree_1->root);
        t_2_after = ast_copy_node(tree_2->root);

        printf("After Crossover!\n");
        printf("------------------------------\n");
        printf("TREE 1\n");
        print_gp_program(tree_1->program);
        print_term_nodes(tree_1);

        printf("\nTREE 2\n");
        print_gp_program(tree_2->program);
        print_term_nodes(tree_2);

        /* asserts */
        res = ast_trees_equal(t_1_before, t_1_after);
        mu_assert(res == 0, "Failed crossover!");
        res = ast_trees_equal(t_2_before, t_2_after);
        mu_assert(res == 0, "Failed crossover!");

        ast_destroy(t_1_before);
        ast_destroy(t_2_before);
        ast_destroy(t_1_after);
        ast_destroy(t_2_after);

        teardown();

        return 0;
}

void test_suite()
{
        mu_run_test(test_point_mutation);
}

mu_run_test_suite(test_suite);
