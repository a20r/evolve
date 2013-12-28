#include <stdlib.h>

#include <al/utils.h>
#include <dstruct/ast.h>
#include <dstruct/ast_cmp.h>

#include "config/config.h"
#include "gp/crossover.h"
#include "gp/function_set.h"
#include "gp/tree_parser.h"


int gp_crossover_random_index(
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config
)
{
        int index = 0;
        int tree_limit = 0;
        int within_limits = 0;
        int func = 0;
        struct ast *n1;
        struct ast *n2;

        if (tree_1->program->end > tree_2->program->end) {
                tree_limit = tree_2->program->end;
        } else {
                tree_limit = tree_1->program->end;
        }

        /* randomly select crossover point */
        while (1) {
                index = randnum_i(0, tree_limit - 2);

                n1 = darray_get(tree_1->program, index);
                n2 = darray_get(tree_2->program, index);

                within_limits = (tree_2->program->end - 1 > index) ? 1 : 0;
                func = (node_is_function(n1) && node_is_function(n2)) ? 1 : 0;
                if (within_limits && func) {
                        break;
                }
        }

        return index;
}

struct ast *gp_get_linked_node(
        struct ast *node,
        struct darray *program,
        int index
)
{
        int limit = 0;
        int value_of = 0;
        int func_node = 0;
        struct ast *linked_node = NULL;

        index = index + 1;
        limit = program->end;
        while (index != limit) {
                linked_node = darray_get(program, index);

                if (ast_node_is_terminal(node)) {
                        func_node = ast_node_is_function(linked_node);
                        value_of = ast_node_is_value_of(linked_node, node);
                        if (func_node && value_of) {
                                break;
                        }

                } else if (ast_node_is_function(node)){
                        value_of = ast_node_is_value_of(linked_node, node);
                        if (value_of) {
                                break;
                        }

                }

                index++;
        }

        return linked_node;
}

void gp_crossover_replace_func_node_value(
        int branch,
        struct ast *func_node,
        struct ast *val_node
)
{
        switch (branch) {
        case VALUE_BRANCH:
                func_node->type.unary->value = val_node;
                break;
        case LEFT_BRANCH:
                func_node->type.binary->left = val_node;
                break;
        case RIGHT_BRANCH:
                func_node->type.binary->right = val_node;
                break;
        default:
                log_err("Unrecognised branch value [%d]!", branch);
        }
}

void gp_crossover_switch_nodes(
        struct ast *node_1,
        struct ast *node_2,
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        int index
)
{
        struct ast *func_node_1 = NULL;
        struct ast *func_node_2 = NULL;
        int branch_1 = 0;
        int branch_2 = 0;

        /* obtain function nodes attached to crossover root nodes */
        func_node_1 = gp_get_linked_node(node_1, tree_1->program, index);
        func_node_2 = gp_get_linked_node(node_2, tree_2->program, index);
        branch_1 = ast_node_is_value_of(func_node_1, node_1);
        branch_2 = ast_node_is_value_of(func_node_2, node_2);

        /* switch */
        gp_crossover_replace_func_node_value(branch_1, func_node_1, node_2);
        gp_crossover_replace_func_node_value(branch_2, func_node_2, node_1);
}

int gp_one_point_crossover(
        void *tree_1,
        void *tree_2,
        struct evolve_config *config
)
{
        int index = 0;
        struct gp_tree_config *gp = NULL;

        /* setup */
        gp = config->general.gp_tree;

        /* choose random index and crossover */
        index = gp_crossover_random_index(tree_1, tree_2, gp);
        gp_crossover_switch_nodes(
                darray_get(((struct gp_tree *) tree_1)->program, index),
                darray_get(((struct gp_tree *) tree_2)->program, index),
                tree_1,
                tree_2,
                index
        );

        /* update program */
        update_program(tree_1);
        update_program(tree_2);

        /* update terminal nodes */
        update_terminal_nodes(tree_1);
        update_terminal_nodes(tree_2);

        /* update input nodes */
        update_input_nodes(tree_1, config);
        update_input_nodes(tree_2, config);

        return 0;
}

int crossover_trees(
        void *tree_1,
        void *tree_2,
        int (*crossover_func)(void *, void *, struct evolve_config *),
        struct evolve_config *config
)
{
        int res = 0;

        /* crossover or not */
        if (*(config->crossover->probability) > randnum_f(0.0, 1.0)) {
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
