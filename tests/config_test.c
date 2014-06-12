#include <stdio.h>
#include <stdlib.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "utils.h"
#include "config.h"
#include "selection.h"


/* TESTS */
int test_evolve_new_and_destroy(void);
int test_tree_config_new_and_destroy(void);
int test_selection_config_new_and_destroy(void);
int test_crossover_config_new_and_destroy(void);
int test_mutation_config_new_and_destroy(void);
void test_suite(void);

int test_evolve_new_and_destroy(void)
{
    struct config *c = config_new(0, 0, 0);
    float solution = -1.0f;

    mu_check(c->max_generations == -1);
    mu_check(c->population_size == -1);

    mu_check(c->stale_limit == -1);
    mu_check(fltcmp(&c->target_score, &solution) == 0);

    mu_check(c->data_struct == NULL);

    mu_check(c->selection != NULL);
    mu_check(c->crossover != NULL);
    mu_check(c->mutation != NULL);

    config_destroy(c);
    return 0;
}

int test_tree_config_new_and_destroy(void)
{
    struct tree_config *tc = tree_config_new();

    mu_check(tc->build_method == -1);
    mu_check(tc->max_depth == -1);
    mu_check(tc->fs == NULL);
    mu_check(tc->ts == NULL);

    tree_config_destroy(tc);

    return 0;
}

int test_selection_config_new_and_destroy(void)
{
    struct selection_config *sc = selection_config_new(TOURNAMENT_SELECTION);
    selection_config_destroy(sc);
    return 0;
}

int test_crossover_config_new_and_destroy(void)
{
    struct crossover_config *cc = crossover_config_new(0);
    crossover_config_destroy(cc);
    return 0;
}

int test_mutation_config_new_and_destroy(void)
{
    struct mutation_config *cc = mutation_config_new(0);
    mutation_config_destroy(cc);
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_evolve_new_and_destroy);
    mu_add_test(test_tree_config_new_and_destroy);
    mu_add_test(test_selection_config_new_and_destroy);
    mu_add_test(test_crossover_config_new_and_destroy);
    mu_add_test(test_mutation_config_new_and_destroy);
}

mu_run_tests(test_suite)
