#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "munit.h"
#include "tree.h"
#include "random.h"

/* GLOBAL VARS */
static struct function_set *fs = NULL;
static struct function *f = NULL;
static struct terminal_set *ts = NULL;
static struct terminal *t = NULL;
static struct value_range *int_range = NULL;
static struct value_range *float_range = NULL;
static struct value_range *double_range = NULL;
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
    int ftypes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int funcs[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};
    fs = function_set_new(ftypes, funcs, arities, 10);
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
    return 0;
}


/* TERMINAL SET TESTS */
void setup_terminal_set(void)
{
    int types[5] = {CONSTANT, CONSTANT, CONSTANT, INPUT, RANDOM_CONSTANT};
    int value_types[5] = {INTEGER, FLOAT, STRING, STRING, FLOAT};

    int one = 1;
    float two = 2.0;
    const char *three = "three";
    const char *input = "x";
    float_range = value_range_float_new(0.0, 10.0, 2);

    void *values[5] = {&one, &two, (void *) three, (void *) input, NULL};
    void *value_ranges[5] = {NULL, NULL, NULL, NULL, float_range};
    ts = terminal_set_new(types, value_types, values, value_ranges, 3);
}

void teardown_terminal_set(void)
{
    terminal_set_destroy(ts);
    value_range_destroy(float_range);
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
    /* INTEGEREGER */
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

    return 0;
}

int test_terminal_resolve_random(void)
{
    int i;
    int *int_ptr = NULL;
    float *float_ptr = NULL;
    double *double_ptr = NULL;

    /* resolve random int */
    int_range = value_range_int_new(0, 10);
    for (i = 0; i < 1000; i++) {
        int_ptr = (int *) terminal_resolve_random(INTEGER, int_range);
        mu_check(*int_ptr >= 0.0 && *int_ptr <= 10.0);
        free(int_ptr);
    }

    /* resolve random float */
    float_range = value_range_float_new(0.0, 10.0, 2);
    for (i = 0; i < 1000; i++) {
        float_ptr = (float *) terminal_resolve_random(FLOAT, float_range);
        mu_check(*float_ptr >= 0.0 && *float_ptr <= 10.0);
        free(float_ptr);
    }

    /* resolve random double */
    double_range = value_range_float_new(0.0, 10.0, 2);
    for (i = 0; i < 1000; i++) {
        double_ptr = (double *) terminal_resolve_random(DOUBLE, double_range);
        mu_check(*double_ptr >= 0.0 && *double_ptr <= 10.0);
        free(double_ptr);
    }

    /* clean up */
    value_range_destroy(int_range);
    value_range_destroy(float_range);
    value_range_destroy(double_range);
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
    mu_add_test(test_node_random_func);
    mu_add_test(test_node_random_term);

    /* tree */
    mu_add_test(test_tree_new_and_destroy);
    mu_add_test(test_tree_build);
    mu_add_test(test_tree_asc_cmp);
    mu_add_test(test_tree_desc_cmp);
}

mu_run_tests(test_suite)
