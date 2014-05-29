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

int test_node_new_and_destroy(void);
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
    int functions[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};
    fs = function_set_new(functions, arities, 10);
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
    f = function_new(0, 2);

    mu_check(f->function == 0);
    mu_check(f->arity == 2);

    function_destroy(f);
    return 0;
}


/* TERMINAL SET TESTS */
void setup_terminal_set(void)
{
    int value_types[3] = {INT, FLOAT, STRING};
    int one = 1;
    float two = 2.0;
    char three[5] = "three";
    void *values[3] = {&one, &two, three};
    ts = terminal_set_new(value_types, values, 3);
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

        /* if (term->value_type == INT) { */
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
    t = terminal_new(INT, &one);
    mu_check(t->value_type == INT);
    mu_check(*(int *) t->value == 1);
    terminal_destroy(t);

    /* FLOAT */
    float two = 2.0;
    t = terminal_new(FLOAT, &two);
    mu_check(t->value_type == FLOAT);
    mu_check(*(float *) t->value == 2.0);
    terminal_destroy(t);

    /* STRING */
    char three[5] = "three";
    t = terminal_new(STRING, three);
    mu_check(t->value_type == STRING);
    mu_check(strcmp(three, t->value) == 0);
    terminal_destroy(t);

    return 0;
}


/* NODE TESTS */
int test_node_new_and_destroy(void)
{
    node = node_new(TERM_NODE);
    mu_check(node->type == 0);
    node_destroy(node);
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

        /* printf("node->type: %d\n", node->type); */
        /* printf("node->value_type: %d\n", node->value_type); */
        /* if (node->value_type == INT) { */
        /*     printf("node->value: %d\n", *(int *) node->value); */
        /* } else if (node->value_type == FLOAT) { */
        /*     printf("node->value: %f\n", *(float *) node->value); */
        /* } else if (node->value_type == STRING) { */
        /*     printf("node->value: %s\n", (char *) node->value); */
        /* } */
        /* printf("\n"); */


        mu_check(node->type == TERM_NODE);
        mu_check(node->value_type >= 0 && node->value_type < 3);
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

    /* node */
    mu_add_test(test_node_new_and_destroy);
    mu_add_test(test_node_random_func);
    mu_add_test(test_node_random_term);

    /* tree */
    mu_add_test(test_tree_new_and_destroy);
    mu_add_test(test_tree_build);
    mu_add_test(test_tree_asc_cmp);
    mu_add_test(test_tree_desc_cmp);
}

mu_run_tests(test_suite)
