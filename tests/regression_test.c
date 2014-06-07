#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "csv.h"
#include "cmp.h"
#include "regression.h"

#define TEST_DATA "./tests/data/sine.csv"


/* TESTS */
int test_regression_pop_stack(void);
int test_regression_traverse(void);
void test_suite(void);


int test_regression_pop_stack(void)
{
    int i;
    float f = 1.0;
    float **values;
    struct node **stack = malloc(sizeof(struct node *) * 3);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    stack[0] = node_new_input((char *) "x");
    stack[1] = node_new_input((char *) "y");
    stack[2] = node_new_constant(FLOAT, &f);

    /* check input node - x */
    values = regression_pop_stack(0, stack, d);
    for (i = 0; i < d->rows; i++) {
        mu_check(fltcmp(values[i], d->data[0][i]) == 0);
    }

    /* check input node - y */
    values = regression_pop_stack(1, stack, d);
    for (i = 0; i < d->rows; i++) {
        mu_check(fltcmp(values[i], d->data[1][i]) == 0);
    }

    /* check constant node */
    values = regression_pop_stack(2, stack, d);
    for (i = 0; i < d->rows; i++) {
        mu_check(fltcmp(values[i], &f) == 0);
    }

    /* clean up */
    data_destroy(d);
    free(stack);
    free(values);
    return 0;
}

int test_regression_traverse(void)
{
    int i;
    float result;
    float f1 = 1.0;
    float f2 = 2.0;
    float f3;
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
    node_destroy(stack[0]);
    node_destroy(stack[1]);
    node_destroy(stack[2]);


    /* SUB */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(SUB, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f2 - f1;
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[0]);
    node_destroy(stack[1]);
    node_destroy(stack[2]);


    /* MUL */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(MUL, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f2 * f1;
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[0]);
    node_destroy(stack[1]);
    node_destroy(stack[2]);


    /* DIV */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(MUL, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f2 / f1;
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[0]);
    node_destroy(stack[1]);
    node_destroy(stack[2]);


    /* POW */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(POW, 2);
    regression_traverse(0, 2, stack, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) pow(f2, f1);
        result = *(float *) stack[2]->values[i];

        mu_check(fltcmp(&result, &f3) == 0);
        free(stack[2]->values[i]);
    }
    node_destroy(stack[0]);
    node_destroy(stack[1]);
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
    node_destroy(stack[0]);
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
    node_destroy(stack[0]);
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
    node_destroy(stack[0]);
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
    node_destroy(stack[0]);
    node_destroy(stack[1]);


    /* clean up */
    data_destroy(d);
    free(stack);
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_regression_pop_stack);
    mu_add_test(test_regression_traverse);
}

mu_run_tests(test_suite)
