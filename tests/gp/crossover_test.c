#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "gp/tree.h"
#include "gp/crossover.h"
#include "gp/regression.h"


/* GLOBAL VARS */
static struct function_set *fs = NULL;
static struct terminal_set *ts = NULL;
static struct tree *t1 = NULL;
static struct tree *t2 = NULL;


/* TESTS */
void setup(void);
void teardown(void);
int test_point_crossover(void);

void test_suite(void);

void setup(void)
{
    /* function set */
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
    fs = function_set_new(functions, 9);

    /* terminal set */
    int one = 1;
    float two = 2.0;
    const char *three = "three";

    float min = 0.0;
    float max = 100.0;

    struct terminal *terminals[4] = {
        terminal_new_constant(INTEGER, &one),
        terminal_new_constant(FLOAT, &two),
        terminal_new_constant(STRING, (void *) three),
        terminal_new_random_constant(FLOAT, &min, &max, 2)
    };
    ts = terminal_set_new(terminals, 4);

    /* generate trees */
    t1 = tree_generate(FULL, fs, ts, 1);
    t2 = tree_generate(FULL, fs, ts, 1);
}

void teardown()
{
    function_set_destroy(fs);
    terminal_set_destroy(ts);
    tree_destroy(t1);
    tree_destroy(t2);
}

int test_point_crossover(void)
{
    int i;
    char *t1_before;
    char *t2_before;
    char *t1_after;
    char *t2_after;

    for (i = 0; i < 1; i++) {
        setup();
        mu_print("BEFORE:\n");
        t1_before = tree_string(t1);
        t2_before = tree_string(t2);
        mu_print("t1 tree: %s\n", t1_before);
        mu_print("t2 tree: %s\n", t2_before);
        mu_print("t1 size: %d depth %d\n", t1->size, t1->depth);
        mu_print("t2 size: %d depth %d\n", t2->size, t2->depth);


        point_crossover(t1, t2);

        mu_print("AFTER:\n");
        t1_after = tree_string(t1);
        t2_after = tree_string(t2);
        mu_print("t1 tree: %s\n", t1_after);
        mu_print("t2 tree: %s\n", t2_after);
        mu_print("t1 size: %d depth %d\n", t1->size, t1->depth);
        mu_print("t2 size: %d depth %d\n", t2->size, t2->depth);
        mu_print("\n");

        teardown();
        free(t1_before);
        free(t2_before);
        free(t1_after);
        free(t2_after);
    }

    setup();
    mu_print("BEFORE:\n");
    tree_print(t1);
    tree_print(t2);
    point_crossover(t1, t2);
    mu_print("AFTER:\n");
    tree_print(t1);
    tree_print(t2);

    mu_print("\n\nBEFORE:\n");
    tree_print(t1);
    tree_print(t2);
    point_crossover(t1, t2);
    mu_print("AFTER:\n");
    tree_print(t1);
    tree_print(t2);
    teardown();

    return 0;
}

void test_suite(void)
{
    mu_add_test(test_point_crossover);
}

mu_run_tests(test_suite)
