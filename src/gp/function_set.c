#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <dbg/dbg.h>
#include <al/utils.h>
#include <dstruct/ast.h>
#include <dstruct/ast_cmp.h>

#include "config/config.h"
#include "gp/function_set.h"
#include "gp/tree_validator.h"


void *function_add(struct ast *x, struct ast *y)
{
        struct ast *result;
        int *integer;
        float *real;

        if (x->tag == REAL && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.real + y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == REAL && y->tag == INTEGER) {
                real = malloc(sizeof(float));
                *real = x->type.real + y->type.integer;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.integer + y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == INTEGER) {
                integer = malloc(sizeof(int));
                *integer = x->type.integer + y->type.integer;
                result = ast_make_exp(INTEGER, integer);
                free(integer);
        } else {
                result = NULL;
        }

        return result;
}

void *function_sub(struct ast *x, struct ast *y)
{
        struct ast *result;
        int *integer;
        float *real;

        if (x->tag == REAL && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.real - y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == REAL && y->tag == INTEGER) {
                real = malloc(sizeof(float));
                *real = x->type.real - y->type.integer;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.integer - y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == INTEGER) {
                integer = malloc(sizeof(int));
                *integer = x->type.integer - y->type.integer;
                result = ast_make_exp(INTEGER, integer);
                free(integer);
        } else {
                result = NULL;
        }

        return result;
}

void *function_mul(struct ast *x, struct ast *y)
{
        struct ast *result;
        int *integer;
        float *real;

        if (x->tag == REAL && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.real * y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == REAL && y->tag == INTEGER) {
                real = malloc(sizeof(float));
                *real = x->type.real * y->type.integer;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.integer * y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == INTEGER) {
                integer = malloc(sizeof(int));
                *integer = x->type.integer * y->type.integer;
                result = ast_make_exp(INTEGER, integer);
                free(integer);
        } else {
                result = NULL;
        }

        return result;
}

void *function_div(struct ast *x, struct ast *y)
{
        struct ast *result;
        int *integer;
        float *real;

        if (x->tag == REAL && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.real / y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == REAL && y->tag == INTEGER) {
                real = malloc(sizeof(float));
                *real = x->type.real / y->type.integer;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.integer / y->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER && y->tag == INTEGER) {
                integer = malloc(sizeof(int));
                *integer = x->type.integer / y->type.integer;
                result = ast_make_exp(INTEGER, integer);
                free(integer);
        } else {
                result = NULL;
        }

        return result;
}

void *function_cos(struct ast *x)
{
        struct ast *result;
        int *integer;
        float *real;

        if (x->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER) {
                integer = malloc(sizeof(int));
                *integer = x->type.integer;
                result = ast_make_exp(INTEGER, integer);
                free(integer);
        } else {
                result = NULL;
        }

        return result;
}

void *function_sin(struct ast *x)
{
        struct ast *result;
        int *integer;
        float *real;

        if (x->tag == REAL) {
                real = malloc(sizeof(float));
                *real = x->type.real;
                result = ast_make_exp(REAL, real);
                free(real);
        } else if (x->tag == INTEGER) {
                integer = malloc(sizeof(int));
                *integer = x->type.integer;
                result = ast_make_exp(INTEGER, integer);
                free(integer);
        } else {
                result = NULL;
        }

        return result;
}

struct ast *execute_binary_function(struct ast *node, struct ast *x, struct ast *y)
{
        char *op;
        struct ast *result = NULL;

        op = node->type.unary->op_name;
        if (strcmp(op, "ADD") == 0) {
                result = function_add(x, y);
                check(result != NULL, "Failed to execute COS function!");
        } else if (strcmp(op, "SUB") == 0) {
                result = function_sub(x, y);
                check(result != NULL, "Failed to execute SIN function!");
        } else if (strcmp(op, "MUL") == 0) {
                result = function_mul(x, y);
                check(result != NULL, "Failed to execute SIN function!");
        } else if (strcmp(op, "DIV") == 0) {
                result = function_div(x, y);
                check(result != NULL, "Failed to execute SIN function!");
        }

        return result;
error:
        return NULL;
}

struct ast *execute_unary_function(struct ast *node, struct ast *value)
{
        char *op;
        struct ast *result = NULL;

        op = node->type.unary->op_name;
        if (strcmp(op, "COS")) {
                result = function_cos(value);
                check(result != NULL, "Failed to execute COS function!");
        } else if (strcmp(op, "SIN")) {
                result = function_sin(value);
                check(result != NULL, "Failed to execute SIN function!");
        }

        return result;
error:
        return NULL;
}


int node_is_function(struct ast *node)
{
        int i;
        enum ast_tag tags[2] = {
                BINARY_OP,
                UNARY_OP
        };

        for (i = 0; i < 2; i++) {
                if (node->tag == tags[i]) {
                        return 1;
                }
        }

        return 0;
}

int function_nodes_equal(struct ast *node_1, struct ast *node_2)
{
        int res;
        char *op_name_1;
        char *op_name_2;

        if (node_1 == node_2) {
                return 1;
        } else if (node_1->tag == node_2->tag) {
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

struct ast *get_new_function_node(
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

                /* terminate conditions */
                equals = function_nodes_equal(new_node, node);
                if (equals == 0 && new_node->tag == tag) {
                        break;
                }

                i++;
        }

        return new_node;
}
