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
static struct config *c;

/* TESTS */
void setup_population(void);
void teardown_population(void);
int test_log_new_and_destroy(void);
int test_evolve_terminate(void);
int test_evolve_reproduction(void);
int test_evolve_gp(void);
void test_suite(void);


void setup_population(void)
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
    c = config_new(TOURNAMENT_SELECTION, NONE, NONE);

    /* general config */
    c->tree = tree_config_new();
    c->tree->fs = fs;
    c->tree->ts = ts;

    c->copy_func = tree_copy;
    c->free_func = tree_destroy;
    c->cmp = tree_cmp;

    /* selection config */
    c->selection->select_func = tournament_selection;
    c->selection->tournament_size = 5;

    /* crossover config */
    c->crossover->crossover_func = point_crossover;
    c->mutation->mutation_func = point_mutation;

    free(one);
    free(two);
}

void teardown_population(void)
{
    population_destroy(p, tree_destroy);
    config_destroy(c);
}

int test_log_new_and_destroy(void)
{
    struct log *l = log_new();
    log_destroy(l);
    return 0;
}

int test_evolve_terminate(void)
{
    struct log *l = log_new();
    c = config_new(NONE, NONE, NONE);

    c->max_generations = 100;
    l->generation = 0;
    mu_check(evolve_terminate(l, c) == 0);

    c->max_generations = 100;
    l->generation = 100;
    mu_check(evolve_terminate(l, c) == 1);

    c->max_generations = 100;
    l->generation = 0;
    c->stale_limit = 100;
    l->stale_counter = 10;
    mu_check(evolve_terminate(l, c) == 0);

    c->stale_limit = 100;
    l->stale_counter = 100;
    mu_check(evolve_terminate(l, c) == 1);

    c->stale_limit = 100;
    l->stale_counter = 10;
    c->target_score = 100;
    l->best_score = 10;
    mu_check(evolve_terminate(l, c) == 0);

    c->target_score = 100;
    l->best_score = 100;
    mu_check(evolve_terminate(l, c) == 1);

    config_destroy(c);
    log_destroy(l);
    return 0;
}

int test_evolve_reproduction(void)
{
    setup_population();

    struct population *new_generation = evolve_reproduce(p, c);
    population_destroy(new_generation, tree_destroy);

    teardown_population();
    return 0;
}

int test_evolve_gp(void)
{


    return 0;
}

void test_suite(void)
{
    mu_add_test(test_evolve_terminate);
    mu_add_test(test_evolve_reproduction);
}

mu_run_tests(test_suite)
