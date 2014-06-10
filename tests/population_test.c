#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "gp/tree.h"
#include "utils.h"
#include "random.h"
#include "population.h"
#include "gp/regression.h"

/* GLOBAL VARS */
static struct population *p = NULL;
static struct function_set *fs = NULL;
static struct terminal_set *ts = NULL;


/* TEST PROTOTYPES */
int free_wrapper(void *value);
void setup_population(void);
void teardown_population(void);
int test_population_new_and_destroy(void);
int test_population_best(void);
void test_suite(void);


void setup_population()
{
    int i;
    int size = 10;
    struct tree *t;

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
    int *one = malloc_int(1);
    float *two = malloc_float(2.0);
    char *three = malloc_string("three");

    struct terminal *terminals[3] = {
        terminal_new_constant(INTEGER, &one),
        terminal_new_constant(FLOAT, &two),
        terminal_new_constant(STRING, &three)
    };
    ts = terminal_set_new(terminals, 3);

    /* create trees */
    p = tree_population(size, FULL, fs, ts, 5);
    for (i = 0; i < size; i++) {
        t = p->individuals[i];
        t->score = malloc_float(randf(0, 100));
    }

    free(one);
    free(two);
    free(three);
}

void teardown_population()
{
    population_destroy(p, tree_destroy);
    terminal_set_destroy(ts);
    function_set_destroy(fs);
}

int test_population_new_and_destroy(void)
{
    int i;
    int size = 10;
    p = population_new(size, sizeof(struct tree));

    mu_check(p->generation == 0);
    mu_check(p->size == size);
    mu_check(p->size == size);
    mu_check(p->individuals != NULL);
    for (i = 0; i < size; i++) {
        mu_check(p->individuals[i] == NULL);
    }

    population_destroy(p, free);
    return 0;
}

int test_population_best(void)
{
    int i;
    struct tree *t = NULL;

    setup_population();

    /* list trees in population */
    for (i = 0; i < p->size; i++) {
        t = p->individuals[i];
        mu_print("tree[%d]: %f\n", i, *t->score);
    }
    mu_print("----------\n");

    /* find lowest scoring trees */
    t = (struct tree *) population_best(p, tree_desc_cmp);
    mu_print("lowest score: %f\n", *(float *) t->score);

    mu_print("----------\n");

    /* find highest scoring trees */
    t = (struct tree *) population_best(p, tree_asc_cmp);
    mu_print("highest score: %f\n", *(float *) t->score);

    teardown_population();

    return 0;
}


void test_suite(void)
{
    srand((unsigned int) time(NULL));

    mu_add_test(test_population_new_and_destroy);
    mu_add_test(test_population_best);
}

mu_run_tests(test_suite)
