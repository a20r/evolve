#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "munit.h"
#include "tree.h"
#include "random.h"
#include "population.h"
#include "regression.h"

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
    struct function *functions[9] = {
        function_new_func(ADD, 2),
        function_new_func(SUB, 2),
        function_new_func(MUL, 2),
        function_new_func(DIV, 2),
        function_new_func(POW, 2),

        function_new_func(LOG, 2),
        function_new_func(EXP, 1),
        function_new_func(SIN, 1),
        function_new_func(COS, 1)
    };
    fs = function_set_new(functions, 10);

    /* terminal set */
    int one = 1;
    float two = 2.0;
    const char *three = "three";

    struct terminal *terminals[3] = {
        terminal_new_constant(INTEGER, &one),
        terminal_new_constant(FLOAT, &two),
        terminal_new_constant(STRING, &three)
    };
    ts = terminal_set_new(terminals, 3);

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
