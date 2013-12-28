#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <dstruct/ast_cmp.h>

#include "gp/initialize.h"
#include "gp/crossover.h"
#include "gp/function_set.h"
#include "gp/tree_parser.h"
#include "gp/tree_validator.h"

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

/* static void print_before(struct ast *t_1_before, struct ast *t_2_before) */
/* { */
/*         printf("Before Crossover!\n"); */
/*         printf("------------------------------\n"); */
/*  */
/*         printf("TREE 1\n"); */
/*         print_gp_tree(t_1_before); */
/*  */
/*         printf("\nTREE 2\n"); */
/*         print_gp_tree(t_2_before); */
/* } */
/*  */
/* static void print_after(struct ast *t_1_after, struct ast *t_2_after) */
/* { */
/*         printf("\nAfter Crossover!\n"); */
/*         printf("------------------------------\n"); */
/*  */
/*         printf("TREE 1\n"); */
/*         print_gp_tree(t_1_after); */
/*  */
/*         printf("\nTREE 2\n"); */
/*         print_gp_tree(t_2_after); */
/* } */

/* static int program_equals(struct darray *prog_1, struct darray *prog_2) */
/* { */
/*         int i = 0; */
/*         int limit = 0; */
/*         void *el_1 = NULL; */
/*         void *el_2 = NULL; */
/*         int res = 1; */
/*  */
/*         if (prog_1->end > prog_2->end) { */
/*                 limit = prog_2->end; */
/*         } else { */
/*                 limit = prog_1->end; */
/*         } */
/*  */
/*         for (i = 0; i < limit; i++) { */
/*                 el_1 = darray_get(prog_1, i); */
/*                 el_2 = darray_get(prog_2, i); */
/*  */
/*                 if (el_1 != el_2) { */
/*                         res = 0; */
/*                 } */
/*         } */
/*  */
/*         return res; */
/* } */

int test_gp_crossover_random_index()
{
        int i = 0;
        int index = 0;
        int tests = 10;
        struct ast *node_1 = NULL;
        struct ast *node_2 = NULL;

        for (i = 0; i < tests; i++) {
                setup();

                index = gp_crossover_random_index(tree_1, tree_2, gp_config);

                mu_assert(index < tree_2->program->end, "Index too large!");
                mu_assert(index < tree_1->program->end, "Index too large!");
                mu_assert(index > 0, "Index too small!");

                node_1 = darray_get(tree_1->program, index);
                node_2 = darray_get(tree_2->program, index);
                mu_assert(node_is_function(node_1) == 1, "Not a func node!");
                mu_assert(node_is_function(node_2) == 1, "Not a func node!");

                teardown();
        }

        return 0;
}

int test_gp_get_linked_node()
{
        int i = 0;
        int index = 0;
        int tests = 10;
        int linked = 0;
        struct ast *node = NULL;
        struct ast *linked_node = NULL;

        for (i = 0; i < tests; i++) {
                setup();

                index = gp_crossover_random_index(tree_1, tree_2, gp_config);

                node = darray_get(tree_1->program, index);
                linked_node = gp_get_linked_node(node, tree_1->program, index);

                linked = ast_node_is_value_of(linked_node, node);
                mu_assert(linked > 0, "Failed to find linked node!");

                node = darray_get(tree_2->program, index);
                linked_node = gp_get_linked_node(node, tree_2->program, index);

                linked = ast_node_is_value_of(linked_node, node);
                mu_assert(linked > 0, "Failed to find linked node!");

                teardown();
        }

        return 0;
}

int test_gp_crossover_replace_func_node_value()
{
        int i = 0;
        int index = 0;
        int tests = 10;
        int branch = 0;
        int branch_new = 0;
        struct ast *node = NULL;
        struct ast *linked_node = NULL;
        struct ast *new_node = NULL;

        for (i = 0; i < tests; i++) {
                setup();

                index = gp_crossover_random_index(tree_1, tree_2, gp_config);

                node = darray_get(tree_1->program, index);
                linked_node = gp_get_linked_node(node, tree_1->program, index);
                branch = ast_node_is_value_of(linked_node, node);

                new_node = ast_make_exp(INTEGER, &i);
                gp_crossover_replace_func_node_value(branch, linked_node, new_node);
                branch_new = ast_node_is_value_of(linked_node, new_node);

                /* asserts */
                mu_assert(branch_new > 0, "Failed to replace value node");
                mu_assert(branch_new == branch, "Failed to replace value node");

                /* revert back and clean up*/
                gp_crossover_replace_func_node_value(branch, linked_node, node);
                ast_destroy(new_node);

                teardown();
        }

        return 0;
}

int test_gp_crossover_switch_nodes()
{
        int i = 0;
        int index = 0;
        int tests = 10;
        int branch = 0;
        int branch_new = 0;
        struct ast *node = NULL;
        struct ast *linked_node = NULL;
        struct ast *new_node = NULL;

        /* TODO: Need a good way of comparing before and after a crossover */
        /* ---> maybe using a string buffer? */

        for (i = 0; i < tests; i++) {
                setup();

                index = gp_crossover_random_index(tree_1, tree_2, gp_config);
                gp_crossover_switch_nodes(
                        darray_get(tree_1->program, index),
                        darray_get(tree_2->program, index),
                        tree_1,
                        tree_2,
                        index
                );

                /* asserts */
                mu_assert(branch_new > 0, "Failed to replace value node");
                mu_assert(branch_new == branch, "Failed to replace value node");

                /* revert back and clean up*/
                gp_crossover_replace_func_node_value(branch, linked_node, node);
                ast_destroy(new_node);

                teardown();
        }

        return 0;
}

int test_one_point_crossover()
{
        int i = 0;
        int equal_1 = 0;
        int equal_2 = 0;
        float tests = 1000;
        struct ast *t_1_before;
        struct ast *t_2_before;
        struct ast *t_1_after;
        struct ast *t_2_after;
        float failed_crossover = 0;
        float successful_crossover = 0;

        for (i = 0; i < tests; i++) {
                /* printf("test: %d\n", i); */
                setup();

                if (ast_trees_equal(tree_1->root, tree_2->root)) {
                        printf("TREES ARE THE SAME!\n");
                }

                /* crossover */
                t_1_before = ast_copy_node(tree_1->root);
                t_2_before = ast_copy_node(tree_2->root);

                crossover_trees(tree_1, tree_2, gp_one_point_crossover, config);

                t_1_after = ast_copy_node(tree_1->root);
                t_2_after = ast_copy_node(tree_2->root);

                /* asserts */
                equal_1 = ast_trees_equal(t_1_before, t_1_after);
                equal_2 = ast_trees_equal(t_2_before, t_2_after);

                if (equal_1 || equal_2) {
                        /* print_before(t_1_before, t_2_before); */
                        /* print_after(t_1_after, t_2_after); */
                        failed_crossover++;
                } else {
                        successful_crossover++;
                }

                /* clean up */
                ast_destroy(t_1_before);
                ast_destroy(t_2_before);
                ast_destroy(t_1_after);
                ast_destroy(t_2_after);
                teardown();
        }
        printf("successful crossovers: %d\n", (int) successful_crossover);
        printf("failed crossovers: %d\n", (int) failed_crossover);
        printf("success rate: %.1f%%\n",  (successful_crossover / tests) * 100);
        /* mu_assert(failed_crossover < (tests * 0.01), "Failed crossover!"); */

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        mu_run_test(test_gp_crossover_random_index);
        mu_run_test(test_gp_get_linked_node);
        mu_run_test(test_gp_crossover_replace_func_node_value);
        mu_run_test(test_one_point_crossover);
}

mu_run_test_suite(test_suite);
