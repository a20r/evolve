#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MU_PRINT
  #define MU_PRINT 0
#endif

#include "munit.h"
#include "utils.h"
#include "config.h"
#include "population.h"
#include "evolve_gp.h"


/* GLOBAL VARS */
static struct terminal_set *ts = NULL;
static struct terminal *t = NULL;


/* TESTS */
void setup_terminal_set(void);
void teardown_terminal_set(void);
int test_terminal_set_new_and_destroy(void);
int test_terminal_new_and_destroy(void);
int test_terminal_resolve_random(void);

void test_suite(void);


/* TERMINAL SET TESTS */
void setup_terminal_set(void)
{
    const char *x = "x";

    float min = 0.0;
    float max = 100.0;

    struct terminal *terminals[3] = {
        terminal_new_constant(INTEGER, &min),
        terminal_new_constant(STRING, (char *) x),
        terminal_new_random_constant(FLOAT, &min, &max, 2)
    };

    ts = terminal_set_new(terminals, 3);
}

void teardown_terminal_set(void)
{
    terminal_set_destroy(ts);
}

int test_terminal_set_new_and_destroy(void)
{
    int i;

    setup_terminal_set();
    mu_check(ts != NULL);

    struct terminal *term;
    for (i = 0; i < 3; i++) {
        term = ts->terminals[i];

        if (term->value_type == INTEGER) {
            mu_print("terminal: %d\n", *(int *) term->value);
        } else if (term->value_type == FLOAT) {
            mu_print("terminal: %f\n", *(float *) term->value);
        } else if (term->value_type == STRING) {
            mu_print("terminal: %s\n", (char *) term->value);
        }
    }

    teardown_terminal_set();
    return 0;
}

int test_terminal_new_and_destroy(void)
{
    /* INTEGER */
    int one = 1;
    t = terminal_new(CONSTANT, INTEGER, &one);
    mu_check(t->type == CONSTANT);
    mu_check(t->value_type == INTEGER);
    mu_check(*(int *) t->value == 1);
    terminal_destroy(t);

    /* FLOAT */
    float two = 2.0;
    t = terminal_new(CONSTANT, FLOAT, &two);
    mu_check(t->type == CONSTANT);
    mu_check(t->value_type == FLOAT);
    mu_check(*(float *) t->value == 2.0);
    terminal_destroy(t);

    /* DOUBLE */
    double three = 3.0;
    t = terminal_new(CONSTANT, DOUBLE, &three);
    mu_check(t->type == CONSTANT);
    mu_check(t->value_type == DOUBLE);
    mu_check(*(double *) t->value == 3.0);
    terminal_destroy(t);

    /* STRING */
    char four[4] = "four";
    t = terminal_new(CONSTANT, STRING, four);
    mu_check(t->type == CONSTANT);
    mu_check(t->value_type == STRING);
    mu_check(strcmp(four, t->value) == 0);
    terminal_destroy(t);

    /* INPUT */
    char *input = (char *) "x";
    t = terminal_new_input(input);
    mu_check(t->type == INPUT);
    mu_check(t->value_type == STRING);
    mu_check(strcmp(t->value, input) == 0);
    terminal_destroy(t);

    /* CONSTANT */
    t = terminal_new_constant(INTEGER, &one);
    mu_check(t->type == CONSTANT);
    mu_check(t->value_type == INTEGER);
    mu_check(*(int *) t->value == 1);
    terminal_destroy(t);

    /* RANDOM CONSTANT */
    float min = 0.0;
    float max = 100.0;
    int precision = 2;
    t = terminal_new_random_constant(FLOAT, &min, &max, precision);
    mu_check(t->type == RANDOM_CONSTANT);
    mu_check(t->value_type == FLOAT);
    mu_check(t->value == NULL);
    mu_check(fltcmp(t->min, &min) == 0);
    mu_check(fltcmp(t->max, &max) == 0);
    mu_check(t->precision == precision);
    terminal_destroy(t);

    return 0;
}

int test_terminal_resolve_random(void)
{
    int i;
    int n_tests = 1;
    int imin = 0;
    int imax = 100;
    float fmin = 0.0;
    float fmax = 100.0;
    double dmin = 0.0;
    double dmax = 100.0;

    struct terminal *terminals[3] = {
        terminal_new_random_constant(INTEGER, &imin, &imax, 2),
        terminal_new_random_constant(FLOAT, &fmin, &fmax, 2),
        terminal_new_random_constant(DOUBLE, &dmin, &dmax, 2)
    };
    ts = terminal_set_new(terminals, 3);

    /* resolve random int */
    int *int_ptr;
    for (i = 0; i < n_tests; i++) {
        int_ptr = (int *) terminal_resolve_random(terminals[0]);
        mu_print("int: %d\n", *(int *) int_ptr);
        mu_check(*int_ptr >= 0 && *int_ptr <= 100);
        free(int_ptr);
    }

    /* resolve random float */
    float *float_ptr;
    for (i = 0; i < n_tests; i++) {
        float_ptr = (float *) terminal_resolve_random(terminals[1]);
        mu_print("float: %f\n", *(float *) float_ptr);
        mu_check(*float_ptr >= 0.0 && *float_ptr <= 100.0);
        free(float_ptr);
    }

    /* resolve random double */
    double *double_ptr;
    for (i = 0; i < n_tests; i++) {
        double_ptr = (double *) terminal_resolve_random(terminals[2]);
        mu_print("double: %f\n", *(double *) double_ptr);
        mu_check(*double_ptr >= 0.0 && *double_ptr <= 100.0);
        free(double_ptr);
    }

    /* clean up */
    terminal_set_destroy(ts);
    return 0;
}

void test_suite(void)
{
    srand((unsigned int) time(NULL));

    /* terminal set */
    mu_add_test(test_terminal_set_new_and_destroy);
    mu_add_test(test_terminal_new_and_destroy);
    mu_add_test(test_terminal_resolve_random);
}

mu_run_tests(test_suite)
