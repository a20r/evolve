#include <stdlib.h>

#include <al/utils.h>
#include <dstruct/ast.h>
#include <dstruct/ast_cmp.h>

#include "config/config.h"
#include "gp/initialize.h"
#include "gp/crossover.h"
#include "gp/function_set.h"
#include "gp/tree_parser.h"


static int crossover_random_index(
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config
)
{
        int index = 0;
        int within_limits = 0;
        int func = 0;
        struct ast *n1;
        struct ast *n2;

        /* randomly select crossover point */
        while (1) {
                index = randnum_i(0, tree_1->program->end - 2);

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

static struct ast *get_linked_func_node(
        struct ast *node,
        struct darray *program,
        int index
)
{
        int limit = 0;
        struct ast *linked_node = NULL;

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

static void replace_func_node_value(
        int branch,
        struct ast *func_node,
        struct ast *val_node
)
{
        if (branch == LEFT_BRANCH) {
                func_node->type.binary->left = val_node;
        }  else if (branch == RIGHT_BRANCH) {
                func_node->type.binary->right = val_node;
        } else if (branch == VALUE_BRANCH) {
                func_node->type.unary->value = val_node;
        }
}

static void switch_nodes(
        struct ast *node_1,
        struct ast *node_2,
        struct gp_tree *tree_1,
        struct gp_tree *tree_2
)
{
        struct ast *linked_1 = NULL;
        struct ast *linked_2 = NULL;
        int branch_1 = 0;
        int branch_2 = 0;
        int index = 0;

        /* obtain function nodes attached to crossover root nodes */
        linked_1 = get_linked_func_node(node_1, tree_1->program, index);
        linked_2 = get_linked_func_node(node_2, tree_2->program, index);
        branch_1 = ast_node_is_value_of(linked_1, node_1);
        branch_2 = ast_node_is_value_of(linked_2, node_2);

        /* crossover */
        replace_func_node_value(branch_1, linked_1, node_1);
        replace_func_node_value(branch_2, linked_2, node_2);
}

int one_point_crossover(
        void *tree_1,
        void *tree_2,
        struct evolve_config *config
)
{
        int index = 0;
        struct gp_tree *t_1;
        struct gp_tree *t_2;
        struct gp_tree_config *gp;

        /* setup */
        gp = config->general.gp_tree;
        t_1 = (struct gp_tree *) tree_1;
        t_2 = (struct gp_tree *) tree_2;

        /* choose random index and switch nodes */
        index = crossover_random_index(tree_1, tree_2, gp);
        switch_nodes(
                darray_get(t_1->program, index),
                darray_get(t_2->program, index),
                t_1,
                t_2
        );

        /* update program */
        update_program(t_1);
        update_program(t_2);

        /* update terminal nodes */
        update_terminal_nodes(t_1);
        update_terminal_nodes(t_2);

        /* update input nodes */
        update_input_nodes(t_1, config);
        update_input_nodes(t_2, config);

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
