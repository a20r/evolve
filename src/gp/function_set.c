#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <dstruct/ast.h>
#include <dbg/dbg.h>

#include "gp/function_set.h"


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
        struct ast *result;

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
        struct ast *result;

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
