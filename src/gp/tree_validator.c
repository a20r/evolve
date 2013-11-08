#include <stdio.h>
#include <string.h>

#include <al/comparator.h>
#include <dstruct/darray.h>
#include <dstruct/linked_list.h>

#include "gp/initialize.h"
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
        res = float_cmp(&node_1->type.integer, &node_2->type.integer);
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
                print_node(node_1);

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
