#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "csv.h"
#include "cmp.h"
#include "utils.h"
#include "regression.h"

#define TEST_DATA "./tests/data/sine.csv"


/* TESTS */
int test_regression_stack_pop(void);
int test_regression_stack_destroy(void);
int test_regression_traverse(void);
int test_regression_evaluate(void);
void test_suite(void);


int test_regression_stack_pop(void)
{
    int i;
    float f = 1.0;
    float **values;
    struct node **stack = malloc(sizeof(struct node *) * 4);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");
    float **f_arr = malloc(sizeof(float *) * (unsigned long) d->rows);

    /* create float array */
    for (i = 0; i < d->rows; i++) {
        f_arr[i] = malloc_float(i);
    }

    /* create stack */
    stack[0] = node_new_input((char *) "x");
    stack[1] = node_new_input((char *) "y");
    stack[2] = node_new_constant(FLOAT, &f);
    stack[3] = node_new_eval(FLOAT, (void **) f_arr, d->rows);

    /* check input node - x */
    values = regression_stack_pop(0, stack, d);
    for (i = 0; i < d->rows; i++) {
        mu_check(fltcmp(values[i], d->data[0][i]) == 0);
    }

    /* check input node - y */
    values = regression_stack_pop(1, stack, d);
    for (i = 0; i < d->rows; i++) {
        mu_check(fltcmp(values[i], d->data[1][i]) == 0);
    }

    /* check constant node */
    values = regression_stack_pop(2, stack, d);
    for (i = 0; i < d->rows; i++) {
        mu_check(fltcmp(values[i], &f) == 0);
    }
    free(values);

    /* check constant node */
    values = regression_stack_pop(3, stack, d);
    for (i = 0; i < d->rows; i++) {
        f = (float) i;
        mu_check(fltcmp(values[i], &f) == 0);
        free(values[i]);
    }
    free(values);

    /* clean up */
    data_destroy(d);
    free(stack);
    return 0;
}

int test_regression_stack_destroy(void)
{
    int i;
    float f = 1.0;
    struct node **stack = malloc(sizeof(struct node *) * 5);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");
    float **f_arr = malloc(sizeof(float *) * (unsigned long) d->rows);

    /* create float array */
    for (i = 0; i < d->rows; i++) {
        f_arr[i] = malloc_float(i);
    }

    /* create stack */
    stack[0] = node_new_input((char *) "x");
    stack[1] = node_new_input((char *) "y");
    stack[2] = node_new_constant(FLOAT, &f);
    stack[3] = node_new_eval(FLOAT, (void **) f_arr, d->rows);
    stack[4] = node_new_func(ADD, 2);

    regression_stack_destroy(0, 4, stack);

    data_destroy(d);
    return 0;
}

int test_regression_traverse(void)
{
    int i;
    float result;
    float f1 = 1.0;
    float f2 = 2.0;
    float f3;
    float zero = 0.0;
    struct node **stack = malloc(sizeof(struct node *) * 3);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");


    /* ADD */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(ADD, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 + f2;
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[2]);


    /* SUB */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(SUB, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 - f2;
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[2]);


    /* MUL */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(MUL, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 * f2;
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[2]);


    /* DIV */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(DIV, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 / f2;
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[2]);


    /* DIV - FAIL TEST - DIVIDE BY ZERO */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &zero);
    stack[2] = node_new_func(DIV, 2);
    mu_check(regression_traverse(0, 2, stack, d) == -1);
    stack = malloc(sizeof(struct node *) * 3);


    /* POW */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(POW, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) pow(f1, f2);
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[2]);


    /* LOG */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(LOG, 1);
    regression_traverse(0, 1, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) log(f1);
        result = *(float *) stack[1]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[1]->values[i]);
    }
    node_destroy(stack[1]);


    /* EXP */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(EXP, 1);
    regression_traverse(0, 1, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) exp(f1);
        result = *(float *) stack[1]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[1]->values[i]);
    }
    node_destroy(stack[1]);

    /* RAD */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(RAD, 1);
    regression_traverse(0, 1, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) f1 * (float) (PI / 180.0);
        result = *(float *) stack[1]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[1]->values[i]);
    }
    node_destroy(stack[1]);


    /* SIN */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(SIN, 1);
    regression_traverse(0, 1, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) sin(f1);
        result = *(float *) stack[1]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[1]->values[i]);
    }
    node_destroy(stack[1]);


    /* COS */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(COS, 1);
    regression_traverse(0, 1, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) cos(f1);
        result = *(float *) stack[1]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[1]->values[i]);
    }
    node_destroy(stack[1]);

    /* FAIL TEST - UNKNOWN FUNCTION */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(99, 1);
    mu_check(regression_traverse(0, 1, stack, d) == -2);


    /* clean up */
    data_destroy(d);
    return 0;
}

int test_regression_evaluate(void)
{
    int res;
    float f = 100.0;
    float solution_score = 5.0;
    struct tree *t;
    struct node **stack = malloc(sizeof(struct node *) * 5);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    /* stack */
    stack[0] = node_new_constant(FLOAT, &f);
    stack[1] = node_new_input((char *) "x");
    stack[2] = node_new_func(MUL, 2);
    stack[3] = node_new_func(RAD, 1);
    stack[4] = node_new_func(SIN, 1);

    /* tree */
    t = malloc(sizeof(struct tree));
    t->root = NULL;
    t->size = 5;
    t->depth = -1;
    t->score = NULL;
    t->chromosome = stack;

    /* evaluation */
    res = regression_evaluate(t, d, (char *) "y");
    mu_check(res == 0);
    mu_check(fltcmp(t->score,  &solution_score) == 0);

    /* clean up */
    tree_destroy(t);
    data_destroy(d);
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_regression_stack_pop);
    mu_add_test(test_regression_stack_destroy);
    mu_add_test(test_regression_traverse);
    mu_add_test(test_regression_evaluate);
}

mu_run_tests(test_suite)
