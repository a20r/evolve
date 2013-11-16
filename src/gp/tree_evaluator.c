#include <stdio.h>

#include <dbg/dbg.h>

#include "gp/initialize.h"
#include "gp/function_set.h"
#include "gp/terminal_set.h"
#include "gp/tree_evaluator.h"


int evaluate_node(struct ast *node, struct stack *s)
{
        struct ast *value;
        struct ast *x;
        struct ast *y;
        struct ast *result = NULL;

        if (node->tag == INTEGER
                || node->tag == REAL
                || node->tag == STRING
                || node->tag == CHARACTER
                || node->tag == BOOL
        ) {
                stack_push(s, node);

        } else if (node->tag == UNARY_OP) {
                value = stack_pop(s);

                result = execute_unary_function(node, value);
                stack_push(s, result);

                ast_destroy(value);
                ast_destroy(node);

        } else if (node->tag == BINARY_OP) {
                y = stack_pop(s);
                x = stack_pop(s);

                result = execute_binary_function(node, x, y);
                stack_push(s, result);

                ast_destroy(x);
                ast_destroy(y);
                ast_destroy(node);

        } else {
                log_err("Error! Unknown node->tag!");
                goto error;
        }

        return 0;
error:
        return -1;
}

struct ast *evaluate_program(struct darray *program)
{
        int i;
        int res;
        struct stack *s;
        struct ast *node;
        struct ast *node_copy;

        /* setup */
        s = stack_create(0);

        /* loop through program in reverse polish notation */
        for (i = 0; i < program->end; i++) {
                node = darray_get(program, i);
                node_copy = ast_copy_node(node);
                res = evaluate_node(node_copy, s);
                check(res == 0, "Failed to evaluate node!");
        }

        /* obtain evaulation result */
        node = stack_pop(s);
        check(node != NULL, "Failed to evaluate program!");
        check(s->count == 0, "Failed to evaluate program!");

        /* clean up */
        stack_destroy(s);

        return node;
error:
        return NULL;
}
