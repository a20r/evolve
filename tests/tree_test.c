#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "munit.h"
#include "cmp.h"
#include "tree.h"
#include "random.h"
#include "regression.h"

/* GLOBAL VARS */
static struct function_set *fs = NULL;
static struct function *f = NULL;
static struct terminal_set *ts = NULL;
static struct terminal *t = NULL;
static struct tree *tree = NULL;
static struct node *node = NULL;


/* TESTS */
void setup_function_set(void);
void teardown_function_set(void);
int test_function_set_new_and_destroy(void);
int test_function_new_and_destroy(void);

void setup_terminal_set(void);
void teardown_terminal_set(void);
int test_terminal_set_new_and_destroy(void);
int test_terminal_new_and_destroy(void);
int test_terminal_resolve_random(void);

int test_node_new_and_destroy(void);
int test_node_copy(void);
int test_node_deepcopy(void);
int test_node_random_func(void);
int test_node_random_term(void);

int test_tree_new_and_destroy(void);
int test_tree_build(void);
int test_tree_asc_cmp(void);
int test_tree_desc_cmp(void);

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

    for (i = 0; i < 10; i++) {
        /* printf("function number: %d\t", fs->functions[i]->function); */
        /* printf("arity: %d\n", fs->functions[i]->arity); */
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


/* TERMINAL SET TESTS */
void setup_terminal_set(void)
{
    int one = 1;
    float two = 2.0;
    const char *three = "three";

    struct terminal *terminals[3] = {
        terminal_new_constant(INTEGER, &one),
        terminal_new_constant(FLOAT, &two),
        terminal_new_constant(STRING, (void *) three)
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

        /* if (term->value_type == INTEGER) { */
        /*     printf("terminal: %d\n", *(int *) term->value); */
        /* } else if (term->value_type == FLOAT) { */
        /*     printf("terminal: %f\n", *(float *) term->value); */
        /* } else if (term->value_type == STRING) { */
        /*     printf("terminal: %s\n", (char *) term->value); */
        /* } */
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
    mu_check(float_cmp(t->min, &min) == 0);
    mu_check(float_cmp(t->max, &max) == 0);
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
        /* printf("int: %d\n", *(int *) int_ptr); */
        mu_check(*int_ptr >= 0 && *int_ptr <= 100);
        free(int_ptr);
    }

    /* resolve random float */
    float *float_ptr;
    for (i = 0; i < n_tests; i++) {
        float_ptr = (float *) terminal_resolve_random(terminals[1]);
        /* printf("float: %f\n", *(float *) float_ptr); */
        mu_check(*float_ptr >= 0.0 && *float_ptr <= 100.0);
        free(float_ptr);
    }

    /* resolve random double */
    double *double_ptr;
    for (i = 0; i < n_tests; i++) {
        double_ptr = (double *) terminal_resolve_random(terminals[2]);
        /* printf("double: %f\n", *(double *) double_ptr); */
        mu_check(*double_ptr >= 0.0 && *double_ptr <= 100.0);
        free(double_ptr);
    }

    /* clean up */
    terminal_set_destroy(ts);
    return 0;
}


/* NODE TESTS */
int test_node_new_and_destroy(void)
{
    node = node_new(TERM_NODE);

    mu_check(node->type == 0);

    mu_check(node->terminal_type == -1);
    mu_check(node->value_type == -1);

    mu_check(node->function_type == -1);
    mu_check(node->function == -1);
    mu_check(node->arity == -1);
    mu_check(node->children == NULL);

    node_destroy(node);
    return 0;
}

int test_node_copy(void)
{
    int i;
    struct node *copy;

    /* copy term node */
    setup_terminal_set();
    for (i = 0; i < 100; i++) {
        node = node_random_term(ts);
        copy = (struct node *) node_copy((void *) node);

        mu_check(copy->type == node->type);
        mu_check(copy->value_type == node->value_type);
        /* mu_check(&(*copy->value) != &(*node->value)); */

        node_destroy(node);
        node_destroy(copy);
    }
    teardown_terminal_set();

    /* copy func node */
    setup_function_set();
    for (i = 0; i < 100; i++) {
        node = node_random_func(fs);
        copy = (struct node *) node_copy((void *) node);

        mu_check(copy->type == node->type);
        mu_check(copy->function == node->function);
        mu_check(copy->arity == node->arity);

        node_destroy(node);
        node_destroy(copy);
    }
    teardown_function_set();

    return 0;
}

int test_node_deepcopy(void)
{
    /* int i; */

    /* function and terminal set */
    setup_function_set();
    setup_terminal_set();

    /* tree */
    tree = tree_new(fs);
    tree_build(FULL, tree, tree->root, fs, ts, 2);

    node_deepcopy(tree->root);

    /* clean up */
    teardown_function_set();
    teardown_terminal_set();
    return 0;
}

int test_node_equals(void)
{
    int res;
    int integer = 1;
    int integer2 = 2;

    struct node *n1 = node_new(INTEGER);
    struct node *n2 = node_new(INTEGER);
    struct node *n3 = node_new(INTEGER);

    n1->terminal_type = CONSTANT;
    n2->terminal_type = CONSTANT;
    n3->terminal_type = CONSTANT;

    n1->value_type = INTEGER;
    n2->value_type = INTEGER;
    n3->value_type = INTEGER;

    n1->value = &integer;
    n2->value = &integer;
    n3->value = &integer2;

    res = node_equals(n1, n2);
    mu_check(res == 1);

    res = node_equals(n1, n3);
    mu_check(res == 0);

    /* clean up */
    node_destroy(n1);
    node_destroy(n2);
    node_destroy(n3);
    return 0;
}

int test_node_random_func(void)
{
    int i;

    setup_function_set();

    for (i = 0; i < 100; i++) {
        node = node_random_func(fs);

        /* printf("node->type: %d\n", node->type); */
        /* printf("node->function: %d\n", node->function); */
        /* printf("node->arity: %d\n", node->arity); */

        mu_check(node->type == FUNC_NODE);
        mu_check(node->function >= 0 && node->function < 10);
        mu_check(node->arity == 1 || node->arity == 2);
        node_destroy(node);
    }

    teardown_function_set();
    return 0;
}

int test_node_random_term(void)
{
    int i;

    setup_terminal_set();

    for (i = 0; i < 100; i++) {
        node = node_random_term(ts);

        mu_check(node->terminal_type  == CONSTANT);
        mu_check(node->type == TERM_NODE);
        mu_check(node->value_type >= 0 && node->value_type < 4);
        mu_check(node->value != NULL);

        node_destroy(node);
    }

    teardown_terminal_set();
    return 0;
}


/* TREE TESTS */
int test_tree_new_and_destroy(void)
{
    setup_function_set();
    tree = tree_new(fs);

    mu_check(tree->depth == 0);
    mu_check(tree->size == 1);
    mu_check(tree->score == NULL);

    teardown_function_set();
    tree_destroy(tree);
    return 0;
}

int test_tree_build(void)
{
    /* function and terminal set */
    setup_function_set();
    setup_terminal_set();

    /* tree */
    tree = tree_new(fs);

    /* generate tree using full method */
    tree_build(FULL, tree, tree->root, fs, ts, 2);
    tree_traverse(tree->root, node_print);
    printf("\n");

    printf("tree->depth: %d\n", tree->depth);
    printf("tree->size: %d\n", tree->size);

    /* clean up */
    tree_destroy(tree);
    teardown_function_set();
    teardown_terminal_set();
    return 0;
}

int test_tree_asc_cmp(void)
{
    int i;

    /* function and terminal set */
    setup_function_set();
    setup_terminal_set();

    /* tree */
    struct tree *t1 = tree_new(fs);
    struct tree *t2 = tree_new(fs);

    /* generate tree using full method */
    tree_build(FULL, t1, t1->root, fs, ts, 2);
    tree_build(FULL, t2, t2->root, fs, ts, 2);
    t1->score = malloc(sizeof(float));
    t2->score = malloc(sizeof(float));

    /* assert asc comparator is working */
    for (i = 0; i < 100; i++) {
        *(t1->score) = randf(0.0, 99.0);
        *(t2->score) = randf(100.0, 200.0);
        mu_check(tree_asc_cmp(t1, t2) == -1);
    }

    for (i = 0; i < 100; i++) {
        *(t1->score) = randf(100.0, 200.0);
        *(t2->score) = randf(0.0, 99.0);
        mu_check(tree_asc_cmp(t1, t2) == 1);
    }

    /* assert edge cases */
    /* equal case */
    *(t1->score) = 0.0;
    *(t2->score) = 0.0;
    mu_check(tree_asc_cmp(t1, t2) == 0);

    /* t1 is NULL */
    free(t1->score);
    t1->score = NULL;
    *(t2->score) = 0.0;
    mu_check(tree_asc_cmp(t1, t2) == -1);

    /* t2 is NULL */
    free(t2->score);
    t2->score = NULL;
    t1->score = malloc(sizeof(float));
    *(t1->score) = 0.0;
    mu_check(tree_asc_cmp(t1, t2) == 1);


    /* clean up */
    tree_destroy(t1);
    tree_destroy(t2);
    teardown_function_set();
    teardown_terminal_set();
    return 0;
}

int test_tree_desc_cmp(void)
{
    int i;

    /* function and terminal set */
    setup_function_set();
    setup_terminal_set();

    /* tree */
    struct tree *t1 = tree_new(fs);
    struct tree *t2 = tree_new(fs);

    /* generate tree using full method */
    tree_build(FULL, t1, t1->root, fs, ts, 2);
    tree_build(FULL, t2, t2->root, fs, ts, 2);
    t1->score = malloc(sizeof(float));
    t2->score = malloc(sizeof(float));

    /* assert desc comparator is working */
    for (i = 0; i < 100; i++) {
        *(t1->score) = randf(0.0, 99.0);
        *(t2->score) = randf(100.0, 200.0);
        mu_check(tree_desc_cmp(t1, t2) == 1);
    }

    for (i = 0; i < 100; i++) {
        *(t1->score) = randf(100.0, 200.0);
        *(t2->score) = randf(0.0, 99.0);
        mu_check(tree_desc_cmp(t1, t2) == -1);
    }

    /* assert edge cases */
    /* equal case */
    *(t1->score) = 0.0;
    *(t2->score) = 0.0;
    mu_check(tree_asc_cmp(t1, t2) == 0);

    /* t1 is NULL */
    free(t1->score);
    t1->score = NULL;
    *(t2->score) = 0.0;
    mu_check(tree_desc_cmp(t1, t2) == 1);

    /* t2 is NULL */
    free(t2->score);
    t2->score = NULL;
    t1->score = malloc(sizeof(float));
    *(t1->score) = 0.0;
    mu_check(tree_desc_cmp(t1, t2) == -1);


    /* clean up */
    tree_destroy(t1);
    tree_destroy(t2);
    teardown_function_set();
    teardown_terminal_set();
    return 0;
}


void test_suite(void)
{
    srand((unsigned int) time(NULL));

    /* function set */
    mu_add_test(test_function_set_new_and_destroy);
    mu_add_test(test_function_new_and_destroy);

    /* terminal set */
    mu_add_test(test_terminal_set_new_and_destroy);
    mu_add_test(test_terminal_new_and_destroy);
    mu_add_test(test_terminal_resolve_random);

    /* node */
    mu_add_test(test_node_new_and_destroy);
    mu_add_test(test_node_copy);
    mu_add_test(test_node_equals);
    mu_add_test(test_node_random_func);
    mu_add_test(test_node_random_term);

    /* tree */
    mu_add_test(test_tree_new_and_destroy);
    mu_add_test(test_tree_build);
    mu_add_test(test_tree_asc_cmp);
    mu_add_test(test_tree_desc_cmp);
}

mu_run_tests(test_suite)
