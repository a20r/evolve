#include <stdio.h>
#include <math.h>

#include <dbg/dbg.h>
#include <al/comparator.h>

#include "gp/initialize.h"
#include "gp/function_set.h"
#include "gp/terminal_set.h"
#include "gp/tree_evaluator.h"
#include "gp/tree_parser.h"


int evaluate_node(struct ast *node, struct stack *s)
{
        struct ast *value;
        struct ast *x;
        struct ast *y;
        struct ast *result = NULL;

        if (node->tag == INTEGER || node->tag == REAL) {
                stack_push(s, node);

        } else if (node->tag == STRING) {
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

static int find_header_index(
        struct ast *node,
        struct gp_tree_config *config,
        int header_type
)
{
        int i = 0;
        int res = 0;

        if (header_type == INPUT_HEADER) {
                for (i = 0; i < config->input_cols; i++) {
                        res = strcmp(node->type.string, config->input_header[i]);
                        if (res == 0) {
                                return i;
                        }
                }
        } else if (header_type == RESPONSE_HEADER) {
                for (i = 0; i < config->response_cols; i++) {
                        res = strcmp(node->type.string, config->response_header[i]);
                        if (res == 0) {
                                return i;
                        }
                }
        }

        return -1;
}

struct ast *mod_input_with_value(
        struct ast *input_node,
        struct gp_tree_config *config,
        int row
)
{
        struct ast *input_copy = NULL;
        int col = 0;
        float value = 0.0;

        /* get column number via header index */
        input_copy = ast_copy_node(input_node);
        col = find_header_index(input_copy, config, INPUT_HEADER);
        if (col == -1) {
                log_err("Failed to find input node from configuration!");
                return NULL;
        }

        /* get value for input node */
        value = config->input_data[row][col];

        /* replace input node with value */
        if (input_copy->tag == STRING) {
                free(input_copy->type.string);
        }
        input_copy->tag = REAL;
        input_copy->type.real = value;

        return input_copy;
}

float evaluate_program(struct gp_tree *tree, struct evolve_config *config)
{
        int i;
        int j;
        int res;
        float sum_sq_err = 0.0;
        float value = 0.0;
        struct stack *s;
        struct ast *node;
        struct ast *node_copy;
        struct darray *program;
        struct gp_tree_config *gp;

        /* setup */
        program = tree->program;
        gp = config->general.gp_tree;

        /* loop through each row of data */
        for (i = 0; i < gp->input_rows; i++) {

                /* evaluate program in reverse polish notation */
                s = stack_create(0);

                for (j = 0; j < program->end; j++) {
                        node = darray_get(program, j);

                        if (node_is_input(node, config)) {
                                node_copy = mod_input_with_value(node, gp, i);
                        } else {
                                node_copy = ast_copy_node(node);
                        }

                        res = evaluate_node(node_copy, s);
                        check(res == 0, "Failed to evaluate node!");
                }

                /* obtain evaulation result */
                node = stack_pop(s);
                check(node != NULL, "Failed to evaluate program!");
                check(s->count == 0, "Failed to evaluate program!");

                value = 0.0;
                value = node->type.real;
                sum_sq_err += pow((value - gp->response_data[i][0]), 2);

                /* clean up */
                stack_destroy(s);
                free(node);
        }

        return sum_sq_err;
error:
        return -1.0;
}

int evaluate_programs(struct population *p, struct evolve_config *config)
{
        int i = 0;
        int res = 0;
        int goal_reached = 0;
        float *score = NULL;
        float epsilon = 0.001;
        struct gp_tree *tree = NULL;

        for (i = 0; i < p->max_population; i++) {
                /* obtain and evaluate individual from population */
                tree = darray_get(p->individuals, i);
                score = malloc(sizeof(float));
                *score = evaluate_program(tree, config);
                darray_set(p->scores, i, score);

                /* check goal is reached with the current individual */
                if (float_epsilon_cmp(score, &p->goal, epsilon) == 0) {
                        p->solution = tree;
                        goal_reached = 1;
                }

                /* update score and total_score */
                release_mem(darray_remove(p->scores, i), free);
                darray_set(p->scores, i, score);
                p->total_score += *score;
        }

        return res;
}

