#include <stdlib.h>

#include "munit.h"
#include "utils.h"
#include "evolve_gp.h"

/* GLOBAL VARS */
static struct cartesian_config *config = NULL;


/* TESTS */
void setup(void);
void teardown(void);
int test_cartesian_config_new_and_destroy(void);
int test_cartesian_new_and_destroy(void);
int test_cartesian_print(void);
int test_cartesian_string(void);
int test_cartesian_address_grid(void);
int test_cartesian_generate(void);
void test_suite(void);


void setup(void)
{
    /* cartesian config */
    config = cartesian_config_new();

    config->rows = 4;
    config->columns = 4;
    config->levels_back = 1;
    config->max_arity = 3;

    config->num_inputs = 4;
    config->num_outputs = 4;
}

void teardown(void)
{
    cartesian_config_destroy(config);
}

int test_cartesian_config_new_and_destroy(void)
{
    struct cartesian_config *cc = cartesian_config_new();

    mu_check(cc->rows == 0);
    mu_check(cc->columns == 0);
    mu_check(cc->levels_back == 0);
    mu_check(cc->max_arity == 0);

    mu_check(cc->num_inputs == 0);
    mu_check(cc->num_outputs == 0);

    cartesian_config_destroy(cc);
    return 0;
}

int test_cartesian_new_and_destroy(void)
{
    struct cartesian_config *cc = cartesian_config_new();
    struct cartesian *c = cartesian_new(cc);

    mu_check(c->score == NULL);

    /* graph details */
    mu_check(c->rows == 0);
    mu_check(c->columns == 0);
    mu_check(c->levels_back == 0);
    mu_check(c->max_arity == 0);

    mu_check(c->num_inputs == 0);
    mu_check(c->num_outputs == 0);

    /* graph elements */
    mu_check(c->graph == NULL);
    mu_check(c->inputs == NULL);
    mu_check(c->outputs == NULL);

    cartesian_destroy(c);
    cartesian_config_destroy(cc);
    return 0;
}

int test_cartesian_print(void)
{

    return 0;
}

int test_cartesian_string(void)
{

    return 0;
}

int test_cartesian_address_grid(void)
{
    struct cartesian *c;
    int i;
    int j;
    int **grid;

    /* setup */
    setup();
    c = cartesian_new(config);
    grid = cartesian_address_grid(c);

    /* print address grid */
    for (i = 0; i < config->columns; i++) {
        printf("column: %d\n", i);
        for (j = 0; j < config->rows; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }

    /* clean up */
    free_mem_arr(grid, c->columns, free);
    cartesian_destroy(c);
    teardown();

    return 0;
}

/* int test_cartesian_random_func_gene(void) */
/* { */
/*  */
/*     return 0; */
/* } */
/*  */
/* int test_cartesian_generate(void) */
/* { */
/*     setup(); */
/*  */
/*     teardown(); */
/*     return 0; */
/* } */

void test_suite(void)
{
    mu_add_test(test_cartesian_config_new_and_destroy);
    mu_add_test(test_cartesian_new_and_destroy);
    mu_add_test(test_cartesian_address_grid);
}


mu_run_tests(test_suite)
