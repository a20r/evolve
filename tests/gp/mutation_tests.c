#include <munit/munit.h>
#include <dstruct/ast.h>
#include <dstruct/ast_cmp.h>

#include "gp/initialize.h"
#include "gp/mutation.h"
#include "gp/tree_parser.h"

#define TEST_CONFIG_FILE "tests/test_files/gp_mutation_test.json"

/* GLOBAL VAR */
struct evolve_config *config;
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

static void print_before_mutation(struct ast *root, struct darray *program)
{
        printf("Before Mutation!\n");
        printf("------------------------------\n");
        print_gp_tree(root);
        printf("\n");
        print_gp_program(program);
}

static void print_after_mutation(struct ast *root, struct darray *program)
{
        printf("\nAfter Mutation!\n");
        printf("------------------------------\n");
        print_gp_tree(root);
        printf("\n");
        print_gp_program(program);
}

int test_point_mutation()
{
        int i = 0;
        int res = 0;
        int tests = 100;
        struct ast *root_before;
        struct ast *root_after;
        struct darray *prog_before;
        struct darray *prog_after;

        /* mutate tree n amount of times */
        for (i = 0; i < tests; i++) {
                setup();

                /* mutate */
                root_before = ast_copy_node(gp->root);
                prog_before = darray_copy(gp->program);

                mutate_tree(gp, gp_point_mutation, config);

                root_after = ast_copy_node(gp->root);
                prog_after = darray_copy(gp->program);

                /* asserts */
                res = ast_trees_equal(root_before, root_after);
                if (res != 0) {
                        print_before_mutation(root_before, prog_before);
                        print_after_mutation(root_after, prog_after);
                }
                mu_assert(res == 0, "Failed to mutate tree!");

                /* clean up */
                ast_destroy(root_before);
                ast_destroy(root_after);
                teardown();
        }

        return 0;
}

void test_suite()
{
        mu_run_test(test_point_mutation);
}

mu_run_test_suite(test_suite);
