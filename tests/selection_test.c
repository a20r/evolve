#include <stdio.h>
#include <stdlib.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "utils.h"
#include "config.h"
#include "selection.h"
#include "population.h"
#include "gp/tree.h"
#include "gp/regression.h"


/* GLOBAL VAR */
static struct population *p;
static struct function_set *fs;
static struct terminal_set *ts;

/* TESTS */
void setup_population(void);
void teardown_population(void);
int test_tournament_selection(void);
void test_suite(void);


void setup_population()
{
    int size = 10;

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

    /* clean up */
    free(one);
    free(two);
}

void teardown_population()
{
    population_destroy(p, tree_destroy);
    terminal_set_destroy(ts);
    function_set_destroy(fs);
}

int test_tournament_selection(void)
{
    int i;
    struct tree *t;
    struct evolve *config;
    setup_population();

    /* selection config */
    config = evolve_new(TOURNAMENT_SELECTION, NONE, NONE);
    config->selection->tournament_size = 5;
    config->copy_func = tree_copy;
    config->cmp = tree_cmp;

    /* print population */
    for (i = 0; i < p->size; i++) {
        t = p->individuals[i];
        t->score = malloc_float(randf(0, 100.0));
        tree_print(t);
        mu_print("score: %f\n\n", *t->score);
    }

    /* select parents */
    struct population *parents = tournament_selection(p, config);
    mu_print("parents->size: %d\n", parents->size);
    mu_print("--------------------------------\n");
    mu_print("SELECTED: \n");

    /* print parents */
    for (i = 0; i < parents->size; i++) {
        t = parents->individuals[i];
        tree_print(t);
        mu_print("score: %f\n\n", *t->score);
    }

    /* clean up */
    population_destroy(parents, tree_destroy);
    evolve_destroy(config);
    teardown_population();
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_tournament_selection);
}

mu_run_tests(test_suite)
