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


static int point_mutate_function_node(
        struct ast *node,
        struct evolve_config *config
)
{
        /* int res = 0; */
        /* int branch = 0; */
        int len = 0;
        struct ast *new_node;
        char *new_op;
        /* struct ast *value; */
        struct gp_tree_config *gp;


        gp = config->general.gp_tree;
        if (node->tag == UNARY_OP) {
                /* new function node */
                new_node = get_new_function_node(node, UNARY_OP, gp);
                new_op = new_node->type.unary->op_name;
                len = strlen(new_op) + 1;  /* + 1 for null terminator */

                /* replace old op_name with new_op */
                free(node->type.unary->op_name);
                node->type.unary->op_name = calloc(1, sizeof(char) * len);
                strcpy(node->type.unary->op_name, new_op);


        } else if (node->tag == BINARY_OP) {
                /* new function node */
                new_node = get_new_function_node(node, BINARY_OP, gp);
                new_op = new_node->type.binary->op_name;
                len = strlen(new_op) + 1;  /* + 1 for null terminator */

                /* replace old op_name with new_op */
                free(node->type.binary->op_name);
                node->type.binary->op_name = calloc(1, sizeof(char) * len);
                strcpy(node->type.binary->op_name, new_op);

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
        struct evolve_config *config
)
{
        struct ast *new_node;
        struct gp_tree_config *gp;

        /* get new term node */
        gp = config->general.gp_tree;
        new_node = get_new_terminal_node(node, node->tag, gp);

        if (node->tag == INTEGER) {
                node->type.integer = new_node->type.integer;
        } else if (node->tag == REAL) {
                node->type.real = new_node->type.real;
        } else if (node->tag == STRING) {
                free(node->type.string);
                strcpy(node->type.string, new_node->type.string);
        } else if (node->tag == CHARACTER) {
                node->type.character = new_node->type.character;
        } else if (node->tag == BOOL) {
                node->type.boolean = new_node->type.boolean;
        }

        return 0;
}

int gp_point_mutation(
        void *tree,
        struct evolve_config *config
)
{
        int res = 0;
        int index = 0;
        struct ast *node;

        /* get node to mutate */
        index = randnum_i(0, ((struct gp_tree *) tree)->program->end - 2);
        node = darray_get(((struct gp_tree *) tree)->program, index);

        if (node_is_function(node)) {
                res = point_mutate_function_node(node, config);
        } else if (node_is_terminal(node)) {
                res = point_mutate_terminal_node(node, config);
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
        void *tree,
        int (*mutation_func)(void *, struct evolve_config *),
        struct evolve_config *config
)
{
        int res = 0;

        /* mutate or not mutate */
        if (*(config->mutation->probability) > randnum_f(0.0, 1.0)) {
                goto mutate;
        } else {
                goto no_mutate;
        }

mutate:
        res = (*mutation_func)(tree, config);
        if (res == -1) {
                log_err("Error! Failed to mutate tree!");
        }

        return res;

no_mutate:
        return -1;
}
