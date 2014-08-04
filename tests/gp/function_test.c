#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MU_PRINT
  #define MU_PRINT 0
#endif

#include "munit.h"
#include "utils.h"
#include "evolve_gp.h"

/* GLOBAL VARS */
static struct function_set *fs = NULL;
static struct function *f = NULL;


/* TESTS */
void setup_function_set(void);
void teardown_function_set(void);
int test_function_set_new_and_destroy(void);
int test_function_new_and_destroy(void);

void test_suite(void);


/* FUNCTION SET TESTS */
void setup_function_set(void)
{
    struct function *functions[9] = {
        function_new_func(ADD, 2),
        function_new_func(SUB, 2),
        function_new_func(MUL, 2),
        function_new_func(DIV, 2),
        function_new_func(POW, 2),

        function_new_func(LOG, 1),
        function_new_func(EXP, 1),
        function_new_func(SIN, 1),
        function_new_func(COS, 1)
    };
    int n = sizeof(functions) / sizeof(struct function *);

    fs = function_set_new(functions, n);
}

void teardown_function_set(void)
{
    function_set_destroy(fs);
}

int test_function_set_new_and_destroy(void)
{
    int i;

    setup_function_set();
    mu_check(fs != NULL);

    for (i = 0; i < 9; i++) {
        mu_print("function number: %d\t", fs->functions[i]->function);
        mu_print("arity: %d\n", fs->functions[i]->arity);
    }

    teardown_function_set();
    return 0;
}

int test_function_new_and_destroy(void)
{
    f = function_new(DEFAULT, 0, 2);
    mu_check(f->type == DEFAULT);
    mu_check(f->function == 0);
    mu_check(f->arity == 2);
    function_destroy(f);

    /* default function */
    f = function_new_func(0, 2);
    mu_check(f->type == DEFAULT);
    mu_check(f->function == 0);
    mu_check(f->arity == 2);
    function_destroy(f);

    /* classification function */
    f = function_new_cfunc(0, 2);
    mu_check(f->type == CLASSIFICATION);
    mu_check(f->function == 0);
    mu_check(f->arity == 2);
    function_destroy(f);

    return 0;
}

void test_suite(void)
{
    srand((unsigned int) time(NULL));

    /* function set */
    mu_add_test(test_function_set_new_and_destroy);
    mu_add_test(test_function_new_and_destroy);
}

mu_run_tests(test_suite)
