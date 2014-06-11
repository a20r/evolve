#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "config.h"
#include "gp/tree.h"
#include "gp/mutation.h"
#include "gp/regression.h"


/* GLOBAL VARS */
static struct function_set *fs = NULL;
static struct terminal_set *ts = NULL;
static struct tree *t = NULL;


/* TESTS */
void setup(void);
void teardown(void);
int test_mutate_node(void);
int test_mutate_new_node(void);
int test_point_mutation(void);

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
}

void teardown()
{
    function_set_destroy(fs);
    terminal_set_destroy(ts);
}

int test_mutate_node(void)
{
    int i;
    int res = 0;
    int arity;
    int nth_child;
    struct node *parent;
    char *before;
    char *after;
    struct node *term = NULL;
    struct node *func = NULL;

    /* mutate terminals */
    for (i = 0; i < 100; i++) {
        term = node_random_term(ts);
        parent = term->parent;
        nth_child = term->nth_child;

        /* before */
        before = node_string(term);
        mu_print("BEFORE: %s\n", before);

        res = mutate_node(term, fs, ts);

        /* after */
        after = node_string(term);
        mu_print("AFTER: %s\n\n", after);

        mu_check(res == 0);
        mu_check(term->parent == parent);
        mu_check(term->nth_child == nth_child);

        free(before);
        free(after);
        node_destroy(term);
    }

    /* mutate functions */
    for (i = 0; i < 100; i++) {
        func = node_random_func(fs);
        parent = func->parent;
        nth_child = func->nth_child;
        arity = func->arity;

        /* before */
        before = node_string(func);
        mu_print("BEFORE: %s\n", before);

        res = mutate_node(func, fs, ts);

        /* after */
        after = node_string(func);
        mu_print("AFTER: %s\n\n", after);

        mu_check(res == 0);
        mu_check(func->parent == parent);
        mu_check(func->nth_child == nth_child);
        mu_check(func->arity == arity);

        free(before);
        free(after);
        node_destroy(func);
    }

    return 0;
}

int test_mutate_new_node(void)
{
    int i;
    int res = 0;

    int nth_child;
    struct node *parent;

    int function;
    int arity;

    char *before;
    char *after;
    struct node *n = node_random_term(ts);


    for (i = 0; i < 100; i++) {
        parent = n->parent;
        nth_child = n->nth_child;
        function = n->function;
        arity = n->arity;

        /* before */
        before = node_string(n);
        mu_print("BEFORE: %s\n", before);

        res = mutate_new_node(n, fs, ts);

        /* after */
        after = node_string(n);
        mu_print("AFTER: %s\n\n", after);

        mu_check(res == 0);
        if (n->type == TERM_NODE) {
            mu_check(n->parent == parent);
            mu_check(n->nth_child == nth_child);
        } else {
            mu_check(n->function != function);
            mu_check(n->arity == arity);
        }
        mu_check(strcmp(before, after) != 0);

        free(before);
        free(after);
    }

    node_destroy(n);
    return 0;
}

int test_point_mutation(void)
{
    int i;
    char *before;
    char *after;

    struct config *config = config_new(NONE, NONE, NONE);
    config->terminal_set = ts;
    config->function_set = fs;

    for (i = 0; i < 100; i++) {
        t = tree_generate(FULL, fs, ts, 2);

        mu_print("BEFORE:\n");
        before = tree_string(t);
        mu_print("tree: %s\n", before);
        mu_print("size: %d depth %d\n\n", t->size, t->depth);

        point_mutation(t, config);

        mu_print("AFTER:\n");
        after = tree_string(t);
        mu_print("tree: %s\n", after);
        mu_print("size: %d depth %d\n\n\n", t->size, t->depth);

        mu_check(strcmp(before, after) != 0);

        free(before);
        free(after);
        tree_destroy(t);
    }

    config_destroy(config);
    return 0;
}

void test_suite(void)
{
    setup();
    mu_add_test(test_mutate_node);
    mu_add_test(test_mutate_new_node);
    mu_add_test(test_point_mutation);
    teardown();
}

mu_run_tests(test_suite)
