#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "csv.h"
#include "cmp.h"
#include "tree.h"
#include "utils.h"
#include "random.h"
#include "regression.h"
#include "population.h"

#define TEST_DATA "./tests/data/sine.csv"

/* GLOBAL VAR */
static struct population *p;
static struct function_set *fs;
static struct terminal_set *ts;


/* TESTS */
void setup_population(void);
void teardown_population(void);
int test_regression_stack_pop(void);
int test_regression_stack_pop(void);
int test_regression_stack_destroy(void);
int test_regression_traverse(void);
int test_regression_evaluate(void);
int test_regression_evaluate_population(void);
void test_suite(void);


void setup_population()
{
    int size = 1;

    /* function set */
    struct function *functions[10] = {
        function_new_func(ADD, 2),
        function_new_func(SUB, 2),
        function_new_func(MUL, 2),
        function_new_func(DIV, 2),
        function_new_func(POW, 2),

        function_new_func(LOG, 1),
        function_new_func(EXP, 1),
        function_new_func(RAD, 1),
        function_new_func(SIN, 1),
        function_new_func(COS, 1)
    };
    fs = function_set_new(functions, 10);

    /* terminal set */
    float *one= malloc_float(1.0);
    float *two = malloc_float(2.0);

    struct terminal *terminals[2] = {
        terminal_new_constant(FLOAT, one),
        terminal_new_constant(FLOAT, two)
    };
    ts = terminal_set_new(terminals, 2);

    /* create trees */
    p = tree_population(size, FULL, fs, ts, 3);

    free(one);
    free(two);
}

void teardown_population()
{
    population_destroy(p, tree_destroy);
    terminal_set_destroy(ts);
    function_set_destroy(fs);
}

int test_regression_stack_pop(void)
{
    int i;
    int res;
    float f = 100.0;
    float *f_arr;
    float **func_input;
    struct data *d = csv_load_data(TEST_DATA, 1, ",");
    struct node **stack = malloc(sizeof(struct node *) * 4);

    /* malloc func_input */
    func_input = malloc(sizeof(float *) * 2);
    for (i = 0; i < 2; i++) {
        func_input[i] = malloc(sizeof(float) * (unsigned long) d->rows);
    }

    /* initialize float array */
    f_arr = malloc(sizeof(float) * (unsigned long) d->rows);
    for (i = 0; i < d->rows; i++) {
        f_arr[i] = (float) i;
    }

    /* create stack */
    stack[0] = node_new_input((char *) "x");
    stack[1] = node_new_input((char *) "y");
    stack[2] = node_new_constant(FLOAT, &f);
    stack[3] = node_new_eval(FLOAT, (void *) f_arr, d->rows);

    /* input node - x */
    regression_stack_pop(stack[0], d, func_input, 0);
    for (i = 0; i < d->rows; i++) {
        res = fltcmp(&func_input[0][i], d->data[data_field_index(d, "x")][i]);
        mu_check(res == 0);
    }

    /* input node - y */
    regression_stack_pop(stack[1], d, func_input, 1);
    for (i = 0; i < d->rows; i++) {
        res = fltcmp(&func_input[1][i], d->data[data_field_index(d, "y")][i]);
        mu_check(res == 0);
    }

    /* constant node */
    regression_stack_pop(stack[2], d, func_input, 0);
    for (i = 0; i < d->rows; i++) {
        res = fltcmp(&func_input[0][i], &f);
        mu_check(res == 0);
    }

    /* eval node */
    regression_stack_pop(stack[3], d, func_input, 0);
    for (i = 0; i < d->rows; i++) {
        f = (float) i;
        res = fltcmp(&func_input[0][i], &f);
        mu_check(res == 0);
    }

    /* clean up */
    data_destroy(d);
    free_mem_arr(func_input, 2, free);
    free(stack);

    return 0;
}


int test_regression_stack_destroy(void)
{
    int i;
    float *f_arr;
    float f = 1.0;
    struct node **stack = malloc(sizeof(struct node *) * 5);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    /* create float array */
    f_arr = malloc(sizeof(float) * (unsigned long) d->rows);
    for (i = 0; i < d->rows; i++) {
        f_arr[i] = (float) i;
    }

    /* create stack */
    stack[0] = node_new_input((char *) "x");
    stack[1] = node_new_input((char *) "y");
    stack[2] = node_new_constant(FLOAT, &f);
    stack[3] = node_new_eval(FLOAT, f_arr, d->rows);
    stack[4] = node_new_func(ADD, 2);

    regression_stack_destroy(0, 4, stack);
    data_destroy(d);
    return 0;
}

int test_regression_traverse(void)
{
    int i;
    int res;
    float result;
    float f1 = 1.0;
    float f2 = 2.0;
    float f3;
    float zero = 0.0;
    float **func_input;
    struct node **stack = malloc(sizeof(struct node *) * 3);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    /* initialize func_input */
    func_input = malloc(sizeof(float *) * 2);
    for (i = 0; i < 2; i++) {
        func_input[i] = malloc(sizeof(float) * (unsigned long) d->rows);
    }

    /* ADD */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(ADD, 2);
    regression_traverse(0, 2, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 + f2;
        result = ((float *) stack[2]->values)[i];
        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[2]);


    /* SUB */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(SUB, 2);
    regression_traverse(0, 2, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 - f2;
        result = ((float *) stack[2]->values)[i];
        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[2]);


    /* MUL */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(MUL, 2);
    regression_traverse(0, 2, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 * f2;
        result = ((float *) stack[2]->values)[i];
        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[2]);


    /* DIV */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(DIV, 2);
    regression_traverse(0, 2, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = f1 / f2;
        result = ((float *) stack[2]->values)[i];
        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[2]);


    /* DIV - FAIL TEST - DIVIDE BY ZERO */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &zero);
    stack[2] = node_new_func(DIV, 2);
    res = regression_traverse(0, 2, stack, func_input, d);
    mu_check(res == -1);
    stack = malloc(sizeof(struct node *) * 3);


    /* POW */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_constant(FLOAT, &f2);
    stack[2] = node_new_func(POW, 2);
    regression_traverse(0, 2, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) pow(f1, f2);
        result = ((float *) stack[2]->values)[i];

        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[2]);


    /* LOG */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(LOG, 1);
    regression_traverse(0, 1, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) log(f1);
        result = ((float *) stack[1]->values)[i];

        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[1]);


    /* EXP */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(EXP, 1);
    regression_traverse(0, 1, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) exp(f1);
        result = ((float *) stack[1]->values)[i];

        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[1]);

    /* RAD */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(RAD, 1);
    regression_traverse(0, 1, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) f1 * (float) (PI / 180.0);
        result = ((float *) stack[1]->values)[i];

        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[1]);


    /* SIN */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(SIN, 1);
    regression_traverse(0, 1, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) sin(f1);
        result = ((float *) stack[1]->values)[i];

        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[1]);


    /* COS */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(COS, 1);
    regression_traverse(0, 1, stack, func_input, d);

    for (i = 0; i < d->rows; i++) {
        f3 = (float) cos(f1);
        result = ((float *) stack[1]->values)[i];

        mu_check(fltcmp(&result, &f3) == 0);
    }
    node_destroy(stack[1]);


    /* FAIL TEST - UNKNOWN FUNCTION */
    stack[0] = node_new_constant(FLOAT, &f1);
    stack[1] = node_new_func(99, 1);
    res = regression_traverse(0, 1, stack, func_input, d);
    mu_check(res == -2);


    /* clean up */
    data_destroy(d);
    free_mem_arr(func_input, 2, free);
    return 0;
}

int test_regression_evaluate(void)
{
    int i;
    int res;
    float f = 100.0;
    float **func_input;
    float solution_score = 5.0;
    struct tree *t;
    struct node **stack = malloc(sizeof(struct node *) * 5);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    /* initialize func_input */
    func_input = malloc(sizeof(float *) * 2);
    for (i = 0; i < 2; i++) {
        func_input[i] = malloc(sizeof(float) * (unsigned long) d->rows);
    }

    /* build stack */
    stack[0] = node_new_constant(FLOAT, &f);
    stack[1] = node_new_input((char *) "x");
    stack[2] = node_new_func(MUL, 2);
    stack[3] = node_new_func(RAD, 1);
    stack[4] = node_new_func(SIN, 1);

    /* build tree */
    t = malloc(sizeof(struct tree));
    t->root = NULL;
    t->size = 5;
    t->depth = -1;
    t->score = NULL;
    t->chromosome = stack;

    /* evaluate tree */
    res = regression_evaluate(t, func_input, d, (char *) "y");
    mu_check(res == 0);
    mu_check(fltcmp(t->score,  &solution_score) == 0);
    mu_check(t->hits == 361);

    /* clean up */
    for (i = 0; i < 5; i++) {
        node_destroy(stack[i]);
    }
    tree_destroy(t);
    data_destroy(d);
    free_mem_arr(func_input, 2, free);
    return 0;
}

int test_regression_evaluate_population(void)
{
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    setup_population();

    tree_print(p->individuals[0]);
    regression_evaluate_population(p, d);

    /* clean up */
    teardown_population();
    data_destroy(d);
    return 0;
}



void test_suite(void)
{
    mu_add_test(test_regression_stack_pop);
    mu_add_test(test_regression_stack_destroy);
    /* mu_add_test(test_regression_traverse); */
    /* mu_add_test(test_regression_evaluate); */
    /* mu_add_test(test_regression_evaluate_population); */
}

mu_run_tests(test_suite)
