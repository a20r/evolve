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
#include "gp/tree/tree.h"
#include "gp/tree/regression.h"


/* GLOBAL VAR */
static struct population *p = NULL;
static struct function_set *fs = NULL;
static struct terminal_set *ts = NULL;
static struct config *c = NULL;
static struct tree_config *tc = NULL;

/* TESTS */
void setup_population(void);
void teardown_population(void);
int test_selection_config_new_and_destroy(void);
int test_tournament_selection(void);
void test_suite(void);


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

    /* config */
    c = config_new();

    c->population_size = 10;
    c->get_score = tree_score;
    c->copy_func = tree_copy;
    c->cmp = tree_cmp;

    tc = tree_config_new();
    tc->build_method = FULL;
    tc->max_depth = 2;
    tc->fs = fs;
    tc->ts = ts;

    c->data_struct = tc;
    c->data_struct_free = tree_config_destroy;

    struct selection_config *sc = selection_config_new(TOURNAMENT_SELECTION);
    sc->tournament_size = 5;
    c->selection = sc;
    c->selection_free = selection_config_destroy;

    /* create trees */
    p = tree_population(c);

    /* clean up */
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

int test_selection_config_new_and_destroy(void)
{
    struct selection_config *sc = selection_config_new(TOURNAMENT_SELECTION);
    selection_config_destroy(sc);
    return 0;
}

int test_tournament_selection(void)
{
    int i;
    struct tree *t;
    setup_population();

    /* selection config */

    /* print population */
    for (i = 0; i < p->size; i++) {
        t = p->individuals[i];
        t->score = malloc_float(randf(0, 100.0));
        tree_print(t);
        mu_print("score: %f\n\n", *t->score);
    }

    /* select parents */
    struct population *parents = tournament_selection(p, c);
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
    teardown_population();
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_selection_config_new_and_destroy);
    mu_add_test(test_tournament_selection);
}

mu_run_tests(test_suite)
