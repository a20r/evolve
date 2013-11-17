#include <stdlib.h>

#include <al/utils.h>
#include <dstruct/ast.h>
#include <dstruct/ast_cmp.h>

#include "config/config.h"
#include "gp/initialize.h"
#include "gp/crossover.h"
#include "gp/tree_parser.h"


static int crossover_random_index(
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config
)
{
        int index = 0;

        /* randomly select crossover point */
        while (1) {
                index = randnum_i(0, tree_1->program->end - 2);
                if (tree_2->program->end -1 > index) {
                        break;
                }
        }

        return index;
}

static struct ast *get_linked_func_node(
        struct ast *node,
        struct darray *program,
        int index
)
{
        int limit = 0;
        struct ast *linked_node;

        limit = program->end;
        while (index != limit) {
                linked_node = darray_get(program, index);

                if (ast_node_is_terminal(node)) {
                        if (ast_node_is_function(linked_node)) {
                                break;
                        }
                } else if (ast_node_is_function(node)){
                        if (ast_node_is_value_of(linked_node, node)) {
                                break;
                        }
                }

                index++;
        }

        return linked_node;
}

int one_point_crossover(
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config
)
{
        int index = 0;
        int branch_1 = 0;
        int branch_2 = 0;
        struct ast *c_1;
        struct ast *c_2;
        struct ast *linked_1;
        struct ast *linked_2;

        /* choose random index and get the crossover root nodes */
        index = crossover_random_index(tree_1, tree_2, config);
        c_1 = darray_get(tree_1->program, index);
        c_2 = darray_get(tree_2->program, index);

        /* obtain function nodes attached to crossover root nodes */
        linked_1 = get_linked_func_node(c_1, tree_1->program, index);
        linked_2 = get_linked_func_node(c_2, tree_2->program, index);
        branch_1 = ast_node_is_value_of(linked_1, c_1);
        branch_2 = ast_node_is_value_of(linked_2, c_2);

        /* crossover */
        if (branch_1 == LEFT_BRANCH) {
                linked_1->type.binary->left = c_2;
        }  else if (branch_1 == RIGHT_BRANCH) {
                linked_1->type.binary->right = c_2;
        } else if (branch_1 == VALUE_BRANCH) {
                linked_1->type.unary->value = c_2;
        }

        if (branch_2 == LEFT_BRANCH) {
                linked_2->type.binary->left = c_1;
        } else if (branch_2 == RIGHT_BRANCH) {
                linked_2->type.binary->right = c_1;
        } else if (branch_2 == VALUE_BRANCH) {
                linked_2->type.unary->value = c_1;
        }

        /* reparse program */
        darray_destroy(tree_1->program);
        darray_destroy(tree_2->program);
        tree_1->program = parse_gp_tree(tree_1->root);
        tree_2->program = parse_gp_tree(tree_2->root);

        return 0;
}

int crossover_trees(
        int prob,
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config,
        int (*crossover_func)(
                struct gp_tree *,
                struct gp_tree *,
                struct gp_tree_config *
        )
)
{
        int res = 0;

        /* crossover or not */
        if (prob > randnum_f(0.0, 1.0)) {
                goto crossover;
        } else {
                goto no_crossover;
        }

crossover:
        res = (*crossover_func)(tree_1, tree_2, config);
        if (res == -1) {
                log_err("Error! Failed to crossover trees!");
        }

        return res;

no_crossover:
        return -1;
}
