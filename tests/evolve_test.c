#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef MU_PRINT
  #define MU_PRINT 1
#endif

#define TEST_DATA "./tests/data/sine.csv"

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
void setup(int random_score);
void teardown(void);
int test_stats_new_and_destroy(void);
int test_stats_update(void);
int test_evolve_terminate(void);
int test_evolve_reproduce(void);
int test_evolve_gp(void);
void test_suite(void);


void setup(int random_score)
{
    int i;
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
    float *min= malloc_float(1.0);
    float *max = malloc_float(100.0);

    struct terminal *terminals[2] = {
        terminal_new_input((char *) "x"),
        terminal_new_random_constant(FLOAT, min, max, 0)
    };
    ts = terminal_set_new(terminals, 2);

    /* setup config */
    c = config_new();

    /* general config */
    c->max_generations = 100;
    c->population_size = 100;

    c->population_generator = tree_population;
    c->evaluate_population = regression_evaluate_population;

    /* tree config */
    struct tree_config *tc = tree_config_new();
    c->data_struct = tc;
    tc->build_method = RAMPED_HALF_AND_HALF;
    tc->max_depth = 3;
    tc->fs = fs;
    tc->ts = ts;
    c->data_struct_free = tree_config_destroy;

    /* selection config */
    struct selection_config *sc = selection_config_new(TOURNAMENT_SELECTION);
    sc->select_func = tournament_selection;
    sc->tournament_size = 100;
    c->selection = sc;
    c->selection_free = selection_config_destroy;

    /* genetic operator config */
    struct crossover_config *cc = crossover_config_new(POINT_CROSSOVER);
    c->crossover = cc;
    cc->crossover_func = point_crossover;
    cc->probability = 0.8f;
    c->crossover_free = crossover_config_destroy;

    struct mutation_config *mc = mutation_config_new(POINT_MUTATION);
    c->mutation = mc;
    mc->mutation_func = subtree_mutation;
    mc->probability = 0.4f;
    c->mutation_free = mutation_config_destroy;

    /* misc config */
    c->print_func = regression_print;
    c->get_score = tree_score;
    c->copy_func = tree_copy;
    c->free_func = tree_destroy;
    c->cmp = tree_cmp;

    /* create trees */
    p = c->population_generator(c);
    if (random_score) {
        for (i = 0; i < p->size; i++) {
            t = ((struct tree *) p->individuals[i]);
            t->score = malloc_float(randf(0, 100));
        }
    }

    free(min);
    free(max);
}

void teardown(void)
{
    function_set_destroy(fs);
    terminal_set_destroy(ts);
    config_destroy(c);
}

int test_stats_new_and_destroy(void)
{
    struct stats *s = stats_new();
    float zero = 0.0f;

    setup(0);

    mu_check(s->generation == -1);
    mu_check(s->stale_counter == 0);
    mu_check(s->population_size == 0);
    mu_check(s->best == NULL);

    mu_check(s->individuals_evaluated == 0);
    mu_check(fltcmp(&s->diversity, &zero) == 0);

    stats_destroy(s, c);
    population_destroy(p, tree_destroy);
    teardown();
    return 0;
}

int test_stats_update(void)
{
    struct stats *s;
    struct tree *best;
    float *best_score;

    /* setup */
    setup(1);

    s = stats_new();
    best = population_best(p, c->get_score, c->cmp);
    best_score = tree_score(best);

    /* stats update */
    stats_update(p, c, s);
    mu_check(s->generation == 0);
    mu_check(s->stale_counter == 0);
    mu_check(s->population_size == p->size);
    mu_check(tree_cmp(s->best, best) == 0);

    stats_update(p, c, s);
    mu_check(s->generation == 1);
    mu_check(s->stale_counter == 1);
    mu_check(s->population_size == p->size);
    mu_check(tree_cmp(s->best, best) == 0);

    *best->score -= 1.0;
    stats_update(p, c, s);
    mu_check(s->generation == 2);
    mu_check(s->population_size == p->size);
    mu_check(s->stale_counter == 0);
    mu_check(tree_cmp(s->best, best) == 0);

    /* destroy */
    stats_destroy(s, c);
    population_destroy(p, tree_destroy);
    teardown();
    return 0;
}

int test_evolve_terminate(void)
{
    struct stats *s = stats_new();
    setup(1);

    /* max generation not reached */
    c->max_generations = 100;
    s->generation = 0;
    mu_check(evolve_terminate(s, c) == 0);

    /* max generation reached */
    c->max_generations = 100;
    s->generation = 100;
    mu_check(evolve_terminate(s, c) == 1);

    /* stale limit not reached */
    c->max_generations = 100;
    s->generation = 0;
    c->stale_limit = 100;
    s->stale_counter = 10;
    mu_check(evolve_terminate(s, c) == 0);

    /* stale limit reached */
    c->stale_limit = 100;
    s->stale_counter = 100;
    mu_check(evolve_terminate(s, c) == 1);

    /* target score not reached */
    c->stale_limit = 100;
    s->stale_counter = 10;
    c->target_score = 100;
    s->best_score = 10;
    mu_check(evolve_terminate(s, c) == 0);

    /* target score reached */
    c->target_score = 100;
    s->best_score = 100;
    mu_check(evolve_terminate(s, c) == 1);

    stats_destroy(s, c);
    population_destroy(p, tree_destroy);
    teardown();
    return 0;
}

int test_evolve_reproduce(void)
{
    setup(1);

    /* reproduce */
    struct population *new_generation = evolve_reproduce(p, c);
    population_destroy(new_generation, tree_destroy);

    teardown();
    return 0;
}

int test_evolve_gp(void)
{
    struct data *d = csv_load_data(TEST_DATA, 1, ",");

    setup(0);

    /* evolve */
    evolve_gp(c, d);

    /* clean up */
    teardown();
    population_destroy(p, tree_destroy);
    data_destroy(d);

    return 0;
}

void test_suite(void)
{
    srand((unsigned int) time(NULL));

    mu_add_test(test_stats_new_and_destroy);
    mu_add_test(test_stats_update);
    mu_add_test(test_evolve_terminate);
    mu_add_test(test_evolve_reproduce);
    mu_add_test(test_evolve_gp);
}

mu_run_tests(test_suite)
