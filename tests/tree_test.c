#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "munit.h"
#include "tree.h"

/* GLOBAL VARS */
static struct function_set *fs = NULL;
static struct function *f = NULL;
static struct terminal_set *ts = NULL;
static struct terminal *t = NULL;
static struct tree *tree = NULL;
static struct node *node = NULL;


/* TESTS */
int test_function_set_new_and_destroy(void);
int test_function_new_and_destroy(void);

int test_terminal_set_new_and_destroy(void);
int test_terminal_new_and_destroy(void);

int test_node_new_and_destroy(void);
int test_node_random_func(void);
int test_node_random_term(void);

int test_tree_new_and_destroy(void);
int test_tree_full_method(void);

void test_suite(void);


/* FUNCTION SET TESTS */
int test_function_set_new_and_destroy(void)
{
    int i;
    int functions[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};
    fs = function_set_new(functions, arities, 10);

    mu_check(fs != NULL);

    for (i = 0; i < 10; i++) {
        /* printf("function number: %d\t", fs->functions[i]->function); */
        /* printf("arity: %d\n", fs->functions[i]->arity); */
    }

    function_set_destroy(fs);
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
int test_terminal_set_new_and_destroy(void)
{
    int i;
    int value_types[3] = {INT, FLOAT, STRING};
    int one = 1;
    float two = 2.0;
    char three[5] = "three";
    void *values[3] = {&one, &two, three};
    ts = terminal_set_new(value_types, values, 3);

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

    terminal_set_destroy(ts);
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
    int functions[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};

    fs = function_set_new(functions, arities, 10);
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

    function_set_destroy(fs);
    return 0;
}

int test_node_random_term(void)
{
    int i;
    int value_types[3] = {INT, FLOAT, STRING};
    int one = 1;
    float two = 2.0;
    char three[5] = "three";
    void *values[3] = {&one, &two, three};
    ts = terminal_set_new(value_types, values, 3);

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

    terminal_set_destroy(ts);
    return 0;
}


/* TREE TESTS */
int test_tree_new_and_destroy(void)
{
    tree = tree_new();

    mu_check(tree->depth == 0);
    mu_check(tree->size == 0);

    tree_destroy(tree);
    return 0;
}

int test_tree_full_method(void)
{
    int functions[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};
    int value_types[3] = {INT, FLOAT, STRING};
    int one = 1;
    float two = 2.0;
    char three[5] = "three";
    void *values[3] = {&one, &two, three};

    /* function and terminal set */
    ts = terminal_set_new(value_types, values, 3);
    fs = function_set_new(functions, arities, 10);

    /* tree */
    tree = tree_new();
    tree->root = node_random_func(fs);

    /* generate tree using full method */
    tree_full_method(tree, tree->root, fs, ts, 2);
    tree_traverse(tree->root, node_print);
    printf("\n");

    /* clean up */
    tree_clear_destroy(tree);
    function_set_destroy(fs);
    terminal_set_destroy(ts);
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
    mu_add_test(test_tree_full_method);
}

mu_run_tests(test_suite)
