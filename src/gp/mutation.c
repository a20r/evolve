#include <stdio.h>

#include <dbg/dbg.h>
#include <dstruct/ast.h>
#include <al/utils.h>

#include "config/config.h"
#include "gp/initialize.h"
#include "gp/mutation.h"
#include "gp/terminal_set.h"
#include "gp/function_set.h"
#include "gp/tree_parser.h"
#include "gp/tree_validator.h"

#define VALUE_BRANCH 1
#define LEFT_BRANCH 2
#define RIGHT_BRANCH 3


static void shallow_free_node(struct ast *node)
{
        printf("FREE NODE: ");
        print_node(node);
        printf("\n");

        if (node_is_function(node)) {
                if (node->tag == UNARY_OP) {
                       free(node->type.unary->op_name);
                } else if (node->tag == BINARY_OP) {
                       free(node->type.binary->op_name);
                }
                free(node);

        } else if (node_is_terminal(node)) {
                if (node->tag == STRING) {
                        free(node->type.string);
                }
                free(node);

        }
}

static int node_is_value_of(
        struct ast *node,
        struct ast *value_node
)
{
        struct ast *left;
        struct ast *right;
        struct ast *value;

        if (node->tag == UNARY_OP) {
                value = node->type.unary->value;

                if (node_is_terminal(value_node)) {
                        if (function_nodes_equal(value, value_node)) {
                                return VALUE_BRANCH;
                        }
                } else if (node_is_function(value_node)){
                        if (function_nodes_equal(value, value_node)) {
                                return VALUE_BRANCH;
                        }
                }

        } else if (node->tag == BINARY_OP) {
                left = node->type.binary->left;
                right = node->type.binary->right;

                /* determine which branch node belongs in func_node */
                /* left */
                if (node_is_terminal(left)) {
                        if (terminal_nodes_equal(left, value_node)) {
                                printf("LEFT!\n");
                                return LEFT_BRANCH;
                        }
                } else if (node_is_function(left)) {
                        if (function_nodes_equal(left, value_node)) {
                                printf("LEFT!\n");
                                return LEFT_BRANCH;
                        }
                }

                /* right */
                if (node_is_terminal(right)) {
                        if (terminal_nodes_equal(right, value_node)) {
                                printf("RIGHT!\n");
                                return RIGHT_BRANCH;
                        }
                } else if (node_is_function(right)) {
                        if (function_nodes_equal(right, value_node)) {
                                printf("RIGHT!\n");
                                return RIGHT_BRANCH;
                        }
                }

        }

        return 0;
}

static struct ast *get_new_function_node(
        struct ast *node,
        enum ast_tag tag,
        struct gp_tree_config *config
)
{
        int i = 0;
        int index = 0;
        int limit = 100;
        int equals = 0;
        struct ast *new_node = NULL;

        while (i != limit) {
                /* get new function node */
                index = randnum_i(0, config->function_set->end);
                new_node = darray_get(config->function_set, index);
                new_node = ast_copy_node(new_node);

                /* terminate conditions */
                equals = function_nodes_equal(new_node, node);
                if (equals == 0 && new_node->tag == tag) {
                        break;
                }

                i++;
        }

        return new_node;
}

static struct ast *get_new_terminal_node(
        struct ast *node,
        struct gp_tree_config *config
)
{
        int i = 0;
        int index = 0;
        int limit = 100;
        struct ast *new_node = NULL;

        while (i != limit) {
                /* get new terminal node */
                index = randnum_i(0, config->terminal_set->end);
                new_node = darray_get(config->terminal_set, index);
                new_node = ast_copy_node(new_node);

                /* terminate conditions */
                if (terminal_nodes_equal(new_node, node) == 0) {
                        break;
                }

                i++;
        }

        return new_node;
}

static struct ast *get_linked_func_node(
        struct ast *node,
        struct darray *program,
        int index
)
{
        int res = 0;
        int limit = 0;
        struct ast *linked_node;

        limit = program->end;
        while (index != limit) {
                linked_node = darray_get(program, index);

                if (node_is_terminal(node)) {
                        if (node_is_function(linked_node)) {
                                break;
                        }
                } else if (node_is_function(node)){
                        if (node_is_value_of(linked_node, node)) {
                                break;
                        }
                }

                index++;
        }

        return linked_node;
}

static int replace_node(struct ast *old, struct ast *new, struct ast *func_node)
{
        int branch = 0;

        if (func_node->tag == UNARY_OP) {
                ast_destroy(func_node->type.unary->value);
                func_node->type.unary->value = new;

        } else if (func_node->tag == BINARY_OP) {
                branch = node_is_value_of(func_node, old);

                if (branch == LEFT_BRANCH) {
                        shallow_free_node(func_node->type.binary->left);
                        func_node->type.binary->left = new;
                } else if (branch == RIGHT_BRANCH) {
                        shallow_free_node(func_node->type.binary->right);
                        func_node->type.binary->right = new;
                }
        }

        return 0;
}

static int point_mutate_function_node(
        struct ast *node,
        struct ast *func_node,
        struct gp_tree_config *config
)
{
        /* int res = 0; */
        int branch = 0;
        struct ast *new_node;
        /* struct ast *value; */

        if (node->tag == UNARY_OP) {
                /* new function node */
                new_node = get_new_function_node(node, UNARY_OP, config);
                new_node->type.unary->value= node->type.unary->value;

                /* destroy old node in func_node */
                replace_node(node, new_node, func_node);

        } else if (node->tag == BINARY_OP) {
                /* new function node */
                new_node = get_new_function_node(node, BINARY_OP, config);
                new_node->type.binary->left = node->type.binary->left;
                new_node->type.binary->right= node->type.binary->right;

                /* destroy old node in func_node */
                replace_node(node, new_node, func_node);

        } else {
                log_err("Error! Unrecognised node tag!");
                goto error;
        }

        return 0;
error:
        return -1;
}

static int point_mutate_terminal_node(
        struct ast *node,
        struct ast *func_node,
        struct gp_tree_config *config
)
{
        int branch = 0;
        struct ast *new_node;

        /* get new term node */
        new_node = get_new_terminal_node(node, config);

        /* destroy old node in func_node */
        replace_node(node, new_node, func_node);

        return 0;
}


int point_mutation(
        struct gp_tree *tree,
        int index,
        struct gp_tree_config *config
)
{
        int res = 0;
        struct ast *func_node;
        struct ast *node;

        /* get node to mutate */
        node = darray_get(tree->program, index);

        if (node_is_function(node)) {
                printf("MUTATE FUNCTION NODE!\n");
                func_node = get_linked_func_node(node, tree->program, index);

                printf("\n");
                printf("selected: ");
                print_node(node);
                printf("\n");

                res = point_mutate_function_node(node, func_node, config);

        } else if (node_is_terminal(node)) {
                printf("MUTATE TERMINAL NODE!\n");
                func_node = get_linked_func_node(node, tree->program, index);

                printf("\n");
                printf("selected: ");
                print_node(node);
                printf("\n");

                res = point_mutate_terminal_node(node, func_node, config);

        } else {
                log_err("Error! Unrecognised node tag!");
                return -1;
        }
        check(res == 0, "Failed to perform point mutation!");

        return 0;
error:
        return -1;
}

int mutate_tree(
        int prob,
        struct gp_tree *tree,
        struct gp_tree_config *config,
        int (*mutation_func)(struct gp_tree *, int, struct gp_tree_config *)
)
{
        int res = 0;
        int index = 0;

        /* mutate or not mutate */
        if (prob > randnum_f(0.0, 1.0)) {
                goto mutate;
        } else {
                goto no_mutate;
        }

mutate:
        index = randnum_i(0, tree->program->end - 2);
        res = (*mutation_func)(tree, index, config);
        if (res == -1) {
                log_err("Error! Failed to mutate tree!");
        }

        return res;

no_mutate:
        return -1;
}
