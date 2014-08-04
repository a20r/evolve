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
static struct function_set *fs = NULL;
static struct terminal_set *ts = NULL;
static struct tree *tree = NULL;
static struct node *node = NULL;


/* TESTS */
void setup_function_set(void);
void teardown_function_set(void);
void setup_terminal_set(void);
void teardown_terminal_set(void);

int test_node_new_and_destroy(void);
int test_node_copy(void);
int test_node_deepcopy(void);
int test_node_equals(void);
int test_node_deep_equals(void);
int test_node_random_func(void);
int test_node_random_func_arity(void);
int test_node_random_term(void);

void tree_deep_print(struct tree *target);
void setup_tree_test(void);
void teardown_tree_test(void);
int test_tree_config_new_and_destroy(void);
int test_tree_new_and_destroy(void);
int test_tree_build(void);
int test_tree_generate(void);
int test_tree_population(void);
int test_tree_equals(void);
int test_tree_size(void);
int test_tree_string(void);
int test_tree_update(void);
int test_tree_replace_node(void);
int test_tree_asc_cmp(void);
int test_tree_desc_cmp(void);
int test_tree_cmp(void);

int test_copy_value(void);

void test_suite(void);


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



/* NODE TESTS */
int test_node_new_and_destroy(void)
{
    /* blank terminal node */
    node = node_new(TERM_NODE);
    mu_check(node->type == TERM_NODE);
    mu_check(node->terminal_type == -1);
    mu_check(node->value_type == -1);
    mu_check(node->function_type == -1);
    mu_check(node->function == -1);
    mu_check(node->arity == -1);
    mu_check(node->children == NULL);
    node_destroy(node);

    /* blank function node */
    node = node_new(FUNC_NODE);
    mu_check(node->type == FUNC_NODE);
    mu_check(node->terminal_type == -1);
    mu_check(node->value_type == -1);
    mu_check(node->function_type == -1);
    mu_check(node->function == -1);
    mu_check(node->arity == -1);
    mu_check(node->children == NULL);
    node_destroy(node);

    /* function node */
    node = node_new_func(1, 2);
    mu_check(node->function_type == DEFAULT);
    mu_check(node->function == 1);
    mu_check(node->arity == 2);
    node_destroy(node);

    /* classification node */
    node = node_new_cfunc(1, 2);
    mu_check(node->function_type == CLASSIFICATION);
    mu_check(node->function == 1);
    mu_check(node->arity == 2);
    node_destroy(node);

    /* input node */
    const char *input_name = "x";
    node = node_new_input((char *) input_name);
    mu_check(node->terminal_type == INPUT);
    mu_check(node->value_type == STRING);
    mu_check(strcmp(node->value, input_name) == 0);
    node_destroy(node);

    /* constant node */
    int integer = 10;
    node = node_new_constant(INTEGER, &integer);
    mu_check(node->terminal_type == CONSTANT);
    mu_check(node->value_type == INTEGER);
    mu_check(intcmp(node->value, &integer) == 0);
    node_destroy(node);

    return 0;
}

int test_node_copy(void)
{
    int i;
    int n_tests = 10;
    struct node *copy;

    /* copy term node */
    setup_terminal_set();
    for (i = 0; i < n_tests; i++) {
        node = node_random_term(ts);
        copy = (struct node *) node_copy((void *) node);

        mu_check(copy->type == node->type);
        mu_check(copy->terminal_type == node->terminal_type);
        mu_check(copy->value_type == node->value_type);

        if (node->terminal_type == RANDOM_CONSTANT) {
            mu_check(&(*copy->value) != &(*node->value));
        } else {
            mu_check(&(*copy->value) == &(*node->value));
        }

        node_destroy(node);
        node_destroy(copy);
    }
    teardown_terminal_set();

    /* copy func node */
    setup_function_set();
    for (i = 0; i < n_tests; i++) {
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
    /* function and terminal set */
    setup_function_set();
    setup_terminal_set();

    /* tree */
    tree = tree_new(fs);
    tree_build(FULL, tree, tree->root, fs, ts, 0, 2);

    int integer = 1;
    struct node *good_copy = node_deepcopy(tree->root);
    struct node *bad_copy = node_new_constant(INTEGER, &integer);

    mu_check(node_deep_equals(good_copy, tree->root) == 1);
    mu_check(node_deep_equals(bad_copy, tree->root) == 0);

    /* clean up */
    teardown_function_set();
    teardown_terminal_set();
    tree_destroy(tree);
    node_clear_destroy(good_copy);
    node_clear_destroy(bad_copy);
    return 0;
}

int test_node_equals(void)
{
    int res;
    int integer = 1;
    int integer2 = 2;

    struct node *n1 = node_new_constant(INTEGER, &integer);
    struct node *n2 = node_new_constant(INTEGER, &integer);
    struct node *n3 = node_new_constant(INTEGER, &integer2);

    /* pass test */
    res = node_equals(n1, n2);
    mu_check(res == 1);

    /* fail test */
    res = node_equals(n1, n3);
    mu_check(res == 0);

    /* clean up */
    node_destroy(n1);
    node_destroy(n2);
    node_destroy(n3);
    return 0;
}

int test_node_deep_equals(void)
{
    /* function and terminal set */
    setup_function_set();
    setup_terminal_set();

    tree = tree_new(fs);
    tree_build(FULL, tree, tree->root, fs, ts, 0, 2);

    int integer = 1.0;
    struct node *n = node_new_constant(INTEGER, &integer);

    mu_check(node_deep_equals(tree->root, tree->root) == 1);
    mu_check(node_deep_equals(tree->root, n) == 0);

    /* clean up */
    teardown_function_set();
    teardown_terminal_set();
    tree_destroy(tree);
    node_destroy(n);
    return 0;
}

int test_node_random_func(void)
{
    int i;

    setup_function_set();

    for (i = 0; i < 100; i++) {
        node = node_random_func(fs);

        mu_print("node->type: %d\n", node->type);
        mu_print("node->function: %d\n", node->function);
        mu_print("node->arity: %d\n", node->arity);

        mu_check(node->type == FUNC_NODE);
        mu_check(node->function >= 0 && node->function < 10);
        mu_check(node->arity == 1 || node->arity == 2);
        node_destroy(node);
    }

    teardown_function_set();
    return 0;
}

int test_node_random_func_arity(void)
{
    int i;

    setup_function_set();

    for (i = 0; i < 100; i++) {
        node = node_random_func_arity(fs, 2);

        mu_print("node->type: %d\n", node->type);
        mu_print("node->function: %d\n", node->function);
        mu_print("node->arity: %d\n", node->arity);

        mu_check(node->type == FUNC_NODE);
        mu_check(node->function >= 0 && node->function < 5);
        mu_check(node->arity == 2);
        node_destroy(node);
    }

    teardown_function_set();
    return 0;
}

int test_node_random_term(void)
{
    int i;
    int n_type;

    setup_terminal_set();

    for (i = 0; i < 100; i++) {
        node = node_random_term(ts);
        n_type = node->terminal_type;

        mu_check(n_type == CONSTANT || n_type == RANDOM_CONSTANT);
        mu_check(node->type == TERM_NODE);
        mu_check(node->value_type >= 0 && node->value_type < 4);
        mu_check(node->value != NULL);

        node_destroy(node);
    }

    teardown_terminal_set();
    return 0;
}


/* TREE TESTS */
void tree_deep_print(struct tree *target)
{
    int i;
    struct node *target_node;
    char *node_str;
    char *parent;

    for (i = 0; i < target->size; i++) {
        target_node = target->chromosome[i];
        parent = node_string(target_node->parent);
        node_str = node_string(target_node);

        mu_print("%s ", node_str);
        mu_print("{p: %s nth: %d}\n", parent, target_node->nth_child);

        free(node_str);
        free(parent);
    }
}

void setup_tree_test(void)
{
    /* function and terminal set */
    setup_function_set();
    setup_terminal_set();
}

void teardown_tree_test(void)
{
    teardown_function_set();
    teardown_terminal_set();
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

int test_tree_new_and_destroy(void)
{
    tree = tree_new(fs);

    mu_check(tree->depth == 0);
    mu_check(tree->size == 1);
    mu_check(tree->score == NULL);

    tree_destroy(tree);
    return 0;
}

int test_tree_build(void)
{
    char *tree_str;

    /* tree */
    tree = tree_new(fs);
    tree_build(FULL, tree, tree->root, fs, ts, 0, 2);

    tree_update(tree);
    tree_str = tree_string(tree);
    mu_print("tree: %s\n", tree_str);
    mu_print("tree->depth: %d\n", tree->depth);
    mu_print("tree->size: %d\n", tree->size);

    /* clean up */
    tree_destroy(tree);
    free(tree_str);
    return 0;
}

int test_tree_generate(void)
{
    char *tree_str;

    tree = tree_generate(FULL, fs, ts, 2);
    tree_str = tree_string(tree);
    mu_check(strlen(tree_str) > 0);

    tree_destroy(tree);
    free(tree_str);
    return 0;
}

int test_tree_population(void)
{
    int i;
    char *tree_str;
    struct config *c;
    struct tree_config *tc;
    struct population *p;

    /* config */
    c = config_new();
    c->population_size = 100;

    tc = tree_config_new();
    tc->build_method = FULL;
    tc->max_depth = 2;
    tc->fs = fs;
    tc->ts = ts;

    c->data_struct = tc;
    c->data_struct_free = tree_config_destroy;

    /* create population */
    p = tree_population(c);
    for (i = 0; i < 100; i++) {
        tree = p->individuals[i];
        tree_str = tree_string(tree);
        mu_check(strlen(tree_str) > 0);
        free(tree_str);
    }

    population_destroy(p, tree_destroy);
    config_destroy(c);
    return 0;
}

int test_tree_equals(void)
{
    /* tree */
    struct tree *t1 = tree_new(fs);
    struct tree *t2 = tree_new(fs);
    tree_build(FULL, t1, t1->root, fs, ts, 0, 2);
    tree_build(FULL, t2, t2->root, fs, ts, 0, 2);

    mu_check(tree_equals(t1, t1) == 1);
    mu_check(tree_equals(t1, t2) == 0);

    /* clean up */
    tree_destroy(t1);
    tree_destroy(t2);
    return 0;
}

int test_tree_size(void)
{
    /* tree */
    struct tree *t1 = tree_new(fs);
    struct tree *t2 = tree_new(fs);

    tree_build(FULL, t1, t1->root, fs, ts, 0, 2);
    tree_build(FULL, t2, t2->root, fs, ts, 0, 10);

    mu_check(tree_size(t1->root) == t1->size);
    mu_check(tree_size(t1->root) != t2->size);

    /* clean up */
    tree_destroy(t1);
    tree_destroy(t2);
    return 0;
}

int test_tree_string(void)
{
    char *tree_str;

    /* tree */
    struct tree *t1 = tree_new(fs);
    tree_build(FULL, t1, t1->root, fs, ts, 0, 2);
    tree_update(t1);

    tree_str = tree_string(t1);
    mu_check(tree_str != NULL);
    mu_check(strlen(tree_str) > 0);
    /* mu_print("tree: %s\n", tree_str); */

    /* clean up */
    tree_destroy(t1);
    free(tree_str);
    return 0;
}

int test_tree_update(void)
{
    /* tree */
    tree = tree_new(fs);
    tree_build(FULL, tree, tree->root, fs, ts, 0, 2);

    mu_check(tree->size > 0);
    mu_check(tree->depth > 0);
    mu_check(tree->chromosome == NULL);

    /* tree update */
    tree_update(tree);
    mu_check(tree->size > 0);
    mu_check(tree->depth > 0);
    mu_check(tree->chromosome != NULL);
    tree_deep_print(tree);

    /* clean up */
    tree_destroy(tree);
    return 0;
}

int test_tree_replace_node(void)
{
    /* tree */
    struct tree *t1 = tree_generate(FULL, fs, ts, 5);
    struct tree *t2 = tree_generate(FULL, fs, ts, 5);

    /* tree update */
    /* tree_print(t1); */
    /* tree_print(t2); */

    struct node *old = tree_replace_node(t1->chromosome[3], t2->root);
    tree_update(t1);
    t2->root = NULL;

    /* tree_print(t1); */
    /* tree_print(t2); */

    /* clean up */
    node_clear_destroy(old);
    tree_destroy(t1);
    tree_destroy(t2);
    return 0;
}

int test_tree_asc_cmp(void)
{
    int i;

    /* tree */
    struct tree *t1 = tree_new(fs);
    struct tree *t2 = tree_new(fs);

    /* generate tree using full method */
    tree_build(FULL, t1, t1->root, fs, ts, 0, 2);
    tree_build(FULL, t2, t2->root, fs, ts, 0, 2);
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
    mu_check(tree_asc_cmp(t1, t2) == 1);

    /* t2 is NULL */
    free(t2->score);
    t2->score = NULL;
    t1->score = malloc_float(0.0);
    mu_check(tree_asc_cmp(t1, t2) == -1);


    /* clean up */
    tree_destroy(t1);
    tree_destroy(t2);
    return 0;
}

int test_tree_desc_cmp(void)
{
    int i;

    /* tree */
    struct tree *t1 = tree_new(fs);
    struct tree *t2 = tree_new(fs);

    /* generate tree using full method */
    tree_build(FULL, t1, t1->root, fs, ts, 0, 2);
    tree_build(FULL, t2, t2->root, fs, ts, 0, 2);
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
    mu_check(tree_desc_cmp(t1, t2) == 0);

    /* t1 is NULL */
    free(t1->score);
    t1->score = NULL;
    *(t2->score) = 0.0;
    mu_check(tree_desc_cmp(t1, t2) == -1);

    /* t2 is NULL */
    free(t2->score);
    t2->score = NULL;
    t1->score = malloc_float(0.0);
    mu_check(tree_desc_cmp(t1, t2) == 1);


    /* clean up */
    tree_destroy(t1);
    tree_destroy(t2);
    return 0;
}

int test_tree_cmp(void)
{
    /* tree */
    struct tree *t1 = tree_new(fs);
    struct tree *t2 = tree_new(fs);

    /* generate tree using full method */
    tree_build(FULL, t1, t1->root, fs, ts, 0, 2);
    tree_build(FULL, t2, t2->root, fs, ts, 0, 2);
    t1->score = NULL;
    t2->score = malloc_float(2);

    /* null score checks */
    mu_check(tree_cmp(t1, t2) == 1);
    mu_check(tree_cmp(t2, t1) == -1);
    mu_check(tree_cmp(t1, t1) == 0);

    /* clean up */
    tree_destroy(t1);
    tree_destroy(t2);
    return 0;
}


void test_suite(void)
{
    srand((unsigned int) time(NULL));

    /* node */
    mu_add_test(test_node_new_and_destroy);
    mu_add_test(test_node_copy);
    mu_add_test(test_node_deepcopy);
    mu_add_test(test_node_equals);
    mu_add_test(test_node_deep_equals);
    mu_add_test(test_node_random_func);
    mu_add_test(test_node_random_func_arity);
    mu_add_test(test_node_random_term);

    /* tree */
    setup_tree_test();
    mu_add_test(test_tree_config_new_and_destroy);
    mu_add_test(test_tree_new_and_destroy);
    mu_add_test(test_tree_build);
    mu_add_test(test_tree_generate);
    mu_add_test(test_tree_population);
    mu_add_test(test_tree_equals);
    mu_add_test(test_tree_size);
    mu_add_test(test_tree_string);
    mu_add_test(test_tree_update);
    mu_add_test(test_tree_replace_node);
    mu_add_test(test_tree_asc_cmp);
    mu_add_test(test_tree_desc_cmp);
    mu_add_test(test_tree_cmp);
    teardown_tree_test();
}

mu_run_tests(test_suite)
