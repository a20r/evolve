#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "data.h"
#include "config.h"
#include "stack.h"
#include "utils.h"
#include "population.h"
#include "gp/tree/tree.h"
#include "gp/tree/regression.h"

#define TEST_DATA "./tests/data/sine.csv"

/* GLOBAL VAR */
static struct population *p;
static struct function_set *fs;
static struct terminal_set *ts;
static struct config *c;


/* TESTS */
void free_chromosome(struct node **chromosome, int length);
void setup_population(void);
void teardown_population(void);
int test_regression_func_input(void);
int test_regression_traverse(void);
int test_regression_evaluate(void);
int test_regression_evaluate_population(void);
int test_regression_print(void);
void test_suite(void);


void free_chromosome(struct node **chromosome, int length)
{
    int i;
    for (i = 0; i < length; i++) {
        node_destroy(chromosome[i]);
    }
}

void setup_population()
{
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

    /* setup config */
    c = config_new();
    c->population_size = 100;

    /* general config */
    c->get_score = tree_score;
    c->copy_func = tree_copy;
    c->free_func = tree_destroy;
    c->cmp = tree_cmp;

    /* tree config */
    c->data_struct = tree_config_new();
    ((struct tree_config *) c->data_struct)->build_method = FULL;
    ((struct tree_config *) c->data_struct)->max_depth = 3;
    ((struct tree_config *) c->data_struct)->fs = fs;
    ((struct tree_config *) c->data_struct)->ts = ts;
    c->data_struct_free = tree_config_destroy;

    /* create trees */
    p = tree_population(c);

    free(one);
    free(two);
}

void teardown_population()
{
    population_destroy(p, tree_destroy);
    terminal_set_destroy(ts);
    function_set_destroy(fs);
    config_destroy(c);
}

int test_regression_func_input(void)
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
    regression_func_input(stack[0], d, func_input, 0);
    for (i = 0; i < d->rows; i++) {
        res = fltcmp(&func_input[0][i], d->data[data_field_index(d, "x")][i]);
        mu_check(res == 0);
    }

    /* input node - y */
    regression_func_input(stack[1], d, func_input, 1);
    for (i = 0; i < d->rows; i++) {
        res = fltcmp(&func_input[1][i], d->data[data_field_index(d, "y")][i]);
        mu_check(res == 0);
    }

    /* constant node */
    regression_func_input(stack[2], d, func_input, 0);
    for (i = 0; i < d->rows; i++) {
        res = fltcmp(&func_input[0][i], &f);
        mu_check(res == 0);
    }

    /* eval node */
    regression_func_input(stack[3], d, func_input, 0);
    for (i = 0; i < d->rows; i++) {
        f = (float) i;
        res = fltcmp(&func_input[0][i], &f);
        mu_check(res == 0);
    }

    /* clean up */
    node_destroy(stack[0]);
    node_destroy(stack[1]);
    node_destroy(stack[2]);
    node_destroy(stack[3]);
    data_destroy(d);
    free_mem_arr(func_input, 2, free);
    free(stack);

    return 0;
}

int test_regression_traverse(void)
{
    int i;
    int res;
    float f1 = 1.0;
    float f2 = 2.0;
    float f3;
    float *flt_ptr;
    float zero = 0.0;
    float **func_input;
    struct node *result_node;
    struct node **chromosome = malloc(sizeof(struct node *) * 3);
    struct stack *stack = stack_new();
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    /* initialize func_input */
    func_input = malloc(sizeof(float *) * 2);
    for (i = 0; i < 2; i++) {
        func_input[i] = malloc(sizeof(float) * (unsigned long) d->rows);
    }

    /* ADD */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_constant(FLOAT, &f2);
    chromosome[2] = node_new_func(ADD, 2);
    res = regression_traverse(0, 2, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = f1 + f2;
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 3);


    /* SUB */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_constant(FLOAT, &f2);
    chromosome[2] = node_new_func(SUB, 2);
    res = regression_traverse(0, 2, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = f1 - f2;
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 3);


    /* MUL */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_constant(FLOAT, &f2);
    chromosome[2] = node_new_func(MUL, 2);
    res = regression_traverse(0, 2, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = f1 * f2;
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 3);


    /* DIV */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_constant(FLOAT, &f2);
    chromosome[2] = node_new_func(DIV, 2);
    res = regression_traverse(0, 2, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = f1 / f2;
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 3);


    /* DIV - FAIL TEST - DIVIDE BY ZERO */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_constant(FLOAT, &zero);
    chromosome[2] = node_new_func(DIV, 2);
    res = regression_traverse(0, 2, chromosome, stack, func_input, d);
    mu_check(res == -1);
    free_chromosome(chromosome, 3);


    /* POW */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_constant(FLOAT, &f2);
    chromosome[2] = node_new_func(POW, 2);
    res = regression_traverse(0, 2, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = (float) pow(f1, f2);
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 3);


    /* LOG */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_func(LOG, 1);
    res = regression_traverse(0, 1, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = (float) log(f1);
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 2);


    /* EXP */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_func(EXP, 1);
    res = regression_traverse(0, 1, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = (float) exp(f1);
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 2);

    /* RAD */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_func(RAD, 1);
    res = regression_traverse(0, 1, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = (float) f1 * (float) (PI / 180.0);
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 2);


    /* SIN */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_func(SIN, 1);
    res = regression_traverse(0, 1, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = (float) sin(f1);
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 2);


    /* COS */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_func(COS, 1);
    res = regression_traverse(0, 1, chromosome, stack, func_input, d);

    result_node = stack_pop(stack);
    for (i = 0; i < d->rows; i++) {
        f3 = (float) cos(f1);
        flt_ptr = &((float *) result_node->values)[i];
        mu_check(fltcmp(flt_ptr, &f3) == 0);
    }
    node_destroy(result_node);
    free_chromosome(chromosome, 2);


    /* FAIL TEST - UNKNOWN FUNCTION */
    chromosome[0] = node_new_constant(FLOAT, &f1);
    chromosome[1] = node_new_func(99, 1);
    res = regression_traverse(0, 1, chromosome, stack, func_input, d);
    mu_check(res == -2);
    free_chromosome(chromosome, 2);


    /* clean up */
    stack_destroy(stack, free);
    free(chromosome);
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
    struct node **chromosome = malloc(sizeof(struct node *) * 5);
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    /* initialize func_input */
    func_input = malloc(sizeof(float *) * 2);
    for (i = 0; i < 2; i++) {
        func_input[i] = malloc(sizeof(float) * (unsigned long) d->rows);
    }

    /* build chromosome */
    chromosome[0] = node_new_constant(FLOAT, &f);
    chromosome[1] = node_new_input((char *) "x");
    chromosome[2] = node_new_func(MUL, 2);
    chromosome[3] = node_new_func(RAD, 1);
    chromosome[4] = node_new_func(SIN, 1);

    /* build tree */
    t = malloc(sizeof(struct tree));
    t->root = NULL;
    t->size = 5;
    t->depth = -1;
    t->score = NULL;
    t->chromosome = chromosome;

    /* evaluate tree */
    res = regression_evaluate(t, func_input, d, (char *) "y");
    mu_check(res == 0);
    mu_check(fltcmp(t->score,  &solution_score) == 0);
    mu_check(t->hits == 361);

    /* clean up */
    free_chromosome(chromosome, 5);
    tree_destroy(t);
    data_destroy(d);
    free_mem_arr(func_input, 2, free);
    return 0;
}

int test_regression_evaluate_population(void)
{
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    setup_population();

    regression_evaluate_population(p, d);

    /* clean up */
    teardown_population();
    data_destroy(d);
    return 0;
}

int test_regression_print(void)
{
    struct tree *t;

    setup_population();
    t = p->individuals[0];
    regression_print(t);

    teardown_population();
    return 0;
}


void test_suite(void)
{
    mu_add_test(test_regression_func_input);
    mu_add_test(test_regression_traverse);
    mu_add_test(test_regression_evaluate);
    mu_add_test(test_regression_evaluate_population);
    mu_add_test(test_regression_print);
}

mu_run_tests(test_suite)
