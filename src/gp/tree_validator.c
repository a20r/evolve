#include <stdio.h>
#include <string.h>

#include <al/comparator.h>
#include <dstruct/darray.h>
#include <dstruct/linked_list.h>

#include "gp/initialize.h"
#include "gp/terminal_set.h"
#include "gp/function_set.h"
#include "gp/tree_validator.h"

static struct darray *create_input_check_list(struct darray *input_set)
{
        int i;
        int len;
        struct darray *input_check_list;
        struct ast *node;

        len = input_set->end + 1;
        input_check_list = darray_create(sizeof(struct ast), len);
        for (i = 0; i < len; i++) {
                node = darray_get(input_set, i);
                darray_set(input_check_list, i, ast_copy_node(node));
        }

        return input_check_list;
}

static inline int node_integer_equals(struct ast *node_1, struct ast *node_2)
{
        int res;
        res = int_cmp(&node_1->type.integer, &node_2->type.integer);
        return (res == 0 ? 1 : 0);
}

static inline int node_real_equals(struct ast *node_1, struct ast *node_2)
{
        int res;
        res = float_cmp(&node_1->type.real, &node_2->type.real);
        return (res == 0 ? 1 : 0);
}

static inline int node_string_equals(struct ast *node_1, struct ast *node_2)
{
        int res;
        res = strcmp(node_1->type.string, node_2->type.string);
        return (res == 0 ? 1 : 0);
}

static inline int node_character_equals(struct ast *node_1, struct ast *node_2)
{
        int res;
        res = int_cmp(&node_1->type.character, &node_2->type.character);
        return (res == 0 ? 1 : 0);
}

static inline int node_boolean_equals(struct ast *node_1, struct ast *node_2)
{
        int res;
        res = int_cmp(&node_1->type.boolean, &node_2->type.boolean);
        return (res == 0 ? 1 : 0);
}

int terminal_nodes_equal(struct ast *node_1, struct ast *node_2)
{
        int res;

        if (node_1->tag == node_2->tag) {
                if (node_1->tag == INTEGER) {
                        res = node_integer_equals(node_1, node_2);
                } else if (node_1->tag == REAL) {
                        res = node_real_equals(node_1, node_2);
                } else if (node_1->tag == STRING) {
                        res = node_string_equals(node_1, node_2);
                } else if (node_1->tag == CHARACTER) {
                        res = node_character_equals(node_1, node_2);
                } else if (node_1->tag == BOOL) {
                        res = node_boolean_equals(node_1, node_2);
                } else {
                        log_err("Unrecognised node tag!");
                        res = -1;
                }
        } else {
                res = 0;
        }

        return res;
}

static int node_unary_op_equals(struct ast *node_1, struct ast *node_2)
{
        int res = 0;
        struct ast *value_1;
        struct ast *value_2;
        char *op_name_1;
        char *op_name_2;

        value_1 = node_1->type.unary->value;
        value_2 = node_2->type.unary->value;

        /* check left */
        if (value_1->tag == value_2->tag) {
                if (node_is_terminal(value_1)) {
                        res = terminal_nodes_equal(value_1, value_2);
                        silent_check(res == 1);

                } else if (value_1->tag == UNARY_OP) {
                        op_name_1 = value_1->type.unary->op_name;
                        op_name_2 = value_2->type.unary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);

                } else if (value_1->tag == BINARY_OP) {
                        op_name_1 = value_1->type.binary->op_name;
                        op_name_2 = value_2->type.binary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);

                }
        }

        return 1;
error:
        return 0;
}

static int node_binary_op_equals(struct ast *node_1, struct ast *node_2)
{
        int res = 0;
        struct ast *left_1;
        struct ast *left_2;
        struct ast *right_1;
        struct ast *right_2;
        char *op_name_1;
        char *op_name_2;

        left_1 = node_1->type.binary->left;
        right_1 = node_1->type.binary->right;

        left_2 = node_2->type.binary->left;
        right_2 = node_2->type.binary->right;

        /* check left */
        if (left_1->tag == left_2->tag) {
                if (node_is_terminal(left_1)) {
                        res = terminal_nodes_equal(left_1, left_2);
                        silent_check(res == 1);

                } else if (left_1->tag == UNARY_OP) {
                        op_name_1 = left_1->type.unary->op_name;
                        op_name_2 = left_2->type.unary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);

                } else if (left_1->tag == BINARY_OP) {
                        op_name_1 = left_1->type.binary->op_name;
                        op_name_2 = left_2->type.binary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);


                }
        }

        /* check right */
        if (right_1->tag == right_2->tag) {
                if(node_is_terminal(right_1)) {
                        res = terminal_nodes_equal(right_1, right_2);
                        silent_check(res == 1);

                } else if (right_1->tag == UNARY_OP) {
                        op_name_1 = right_1->type.unary->op_name;
                        op_name_2 = right_2->type.unary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);

                } else if (right_1->tag == BINARY_OP) {
                        op_name_1 = right_1->type.binary->op_name;
                        op_name_2 = right_2->type.binary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);

                }
        }

        return 1;
error:
        return 0;
}

int function_nodes_equal(struct ast *node_1, struct ast *node_2)
{
        int res;
        char *op_name_1;
        char *op_name_2;

        printf("node_1: %p, node_2: %p\n", node_1, node_2);
        if (node_1->tag == node_2->tag) {
                if (node_1->tag == UNARY_OP) {
                        op_name_1 = node_1->type.unary->op_name;
                        op_name_2 = node_2->type.unary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);
                } else if (node_1->tag == BINARY_OP) {
                        op_name_1 = node_1->type.binary->op_name;
                        op_name_2 = node_2->type.binary->op_name;
                        res = strcmp(op_name_1, op_name_2);
                        silent_check(res == 0);
                } else {
                        log_err("Unrecognised node tag!");
                        return -1;
                }
        } else {
                log_err("Unrecognised node tag!");
                return -1;
        }

        return 1;
error:
        return 0;
}

int validate_tree(struct gp_tree *gp, struct darray *input_set)
{
        int i;
        int j;
        int res;
        int check_list_len;
        int term_nodes_len;
        struct darray *input_check_list;
        struct ast *node_1;
        struct ast *node_2;

        /* setup */
        input_check_list = create_input_check_list(input_set);
        check_list_len = input_check_list->end;
        term_nodes_len = gp->terminal_nodes->end + 1;

        /* loop through and tick off input check list */
        for (i = check_list_len; i >= 0 ; i--) {
                node_1 = darray_get(input_check_list, i);

                /* loop through terminal nodes and compare */
                for (j = 0; j < term_nodes_len; j++) {
                        node_2 = darray_get(gp->terminal_nodes, i);
                        res = terminal_nodes_equal(node_1, node_2);

                        /* found an input node in terminal nodes */
                        if (res == 1) {
                                darray_pop(input_check_list);
                                ast_destroy(node_1);
                                break;
                        }
                }
        }

        /* check if input check list still has items */
        node_1 = darray_get(input_check_list, 0);
        if (input_check_list->end == 0 || node_1 == NULL) {
                res = 1;
        } else {
                res = 0;
        }

        /* clean up */
        for (i = 0; i <= input_check_list->end; i++) {
                node_1 = darray_get(input_check_list, i);
                release_mem(node_1, ast_destroy);
        }
        darray_destroy(input_check_list);

        return res;
}
