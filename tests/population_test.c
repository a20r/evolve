#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "munit.h"
#include "tree.h"
#include "random.h"
#include "population.h"

/* GLOBAL VARS */
static struct population *p = NULL;
static struct function_set *fs = NULL;
static struct terminal_set *ts = NULL;


/* TEST PROTOTYPES */
int free_wrapper(void *value);
void setup_population(void);
void teardown_population(void);
int test_population_new_and_destroy(void);
int test_population_best(void);

void test_suite(void);


/* TESTS */
int free_wrapper(void *value)
{
    free(value);
    return 0;
}

void setup_population()
{
    int i = 0;
    int size = 10;
    p = population_new(size, sizeof(struct tree *));

    /* function set */
    int f_types[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int functions[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};
    int value_types[3] = {INTEGER, FLOAT, STRING};
    void *value_ranges[3] = {NULL, NULL, NULL};

    fs = function_set_new(f_types, functions, arities, 10);

    /* terminal set */
    int t_types[3] = {CONSTANT, CONSTANT, CONSTANT};
    int one = 1;
    float two = 2.0;
    char three[5] = "three";
    void *values[3] = {&one, &two, three};

    ts = terminal_set_new(t_types, value_types, values, value_ranges, 3);

    /* create trees */
    struct tree *t = NULL;
    for (i = 0; i < size; i++) {
        t = tree_generate(FULL, fs, ts, 2);
        t->score = malloc(sizeof(float));
        *(t->score) = randf(0.0, 100);

        printf("tree[%d]: %f\n", i, *(float *) t->score);

        p->individuals[i] = t;
    }
}

void teardown_population()
{
    population_destroy(p, tree_destroy);
    terminal_set_destroy(ts);
    function_set_destroy(fs);
}

int test_population_new_and_destroy(void)
{
    int i;
    int size = 10;
    p = population_new(size, sizeof(struct tree));

    mu_check(p->generation == 0);
    mu_check(p->size == size);
    mu_check(p->size == size);
    mu_check(p->individuals != NULL);
    for (i = 0; i < size; i++) {
        mu_check(p->individuals[i] == NULL);
    }

    population_destroy(p, free_wrapper);
    return 0;
}

int test_population_best(void)
{
    struct tree *t = NULL;

    setup_population();


    /* find lowest and highest scoring trees */
    printf("----------\n");
    t = (struct tree *) population_best(p, tree_desc_cmp);
    printf("lowest score: %f\n", *(float *) t->score);

    t = (struct tree *) population_best(p, tree_asc_cmp);
    printf("highest score: %f\n", *(float *) t->score);

    teardown_population();

    return 0;
}


void test_suite(void)
{
    srand((unsigned int) time(NULL));

    mu_add_test(test_population_new_and_destroy);
    mu_add_test(test_population_best);
}

mu_run_tests(test_suite)
