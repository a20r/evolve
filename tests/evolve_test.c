#include <stdio.h>
#include <stdlib.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "utils.h"
#include "evolve.h"
#include "population.h"
#include "selection.h"
#include "gp/tree.h"
#include "gp/crossover.h"
#include "gp/mutation.h"
#include "gp/regression.h"


/* GLOBAL VAR */
static struct population *p;
static struct function_set *fs;
static struct terminal_set *ts;
static struct evolve *e;

/* TESTS */
void setup_population(void);
void teardown_population(void);
int test_evolve_reproduction(void);
void test_suite(void);


void setup_population()
{
    int size = 100;

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

    /* setup evolve */
    e = evolve_new(TOURNAMENT_SELECTION, NONE, NONE);

    /* general config */
    e->copy_func = tree_copy;
    e->free_func = tree_destroy;
    e->cmp = tree_cmp;
    e->function_set = fs;
    e->terminal_set = ts;

    /* selection config */
    e->selection->select_func = tournament_selection;
    e->selection->tournament_size = 5;

    /* crossover config */
    e->crossover->crossover_func = point_crossover;
    e->mutation->mutation_func = point_mutation;

    free(one);
    free(two);
}

void teardown_population()
{
    population_destroy(p, tree_destroy);
    terminal_set_destroy(ts);
    function_set_destroy(fs);
    evolve_destroy(e);
}

int test_evolve_reproduction(void)
{
    setup_population();

    struct population *new_generation = evolve_reproduce(p, e);
    population_destroy(new_generation, tree_destroy);

    teardown_population();
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_evolve_reproduction);
}

mu_run_tests(test_suite)
