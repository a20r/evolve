#include <stdio.h>
#include "munit.h"
#include "tree.h"

/* GLOBAL VARS */
struct function_set *fs;


/* TESTS */
int test_function_set_new(void);
int test_function_set_destroy(void);


int test_function_set_new(void)
{
    int i;
    int functions[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};
    fs = function_set_new(functions, arities, 10);

    mu_check(fs != NULL);

    for (i = 0; i < 10; i++) {
        printf("function number: %d\t", fs->functions[i]->function);
        printf("arity: %d\n", fs->functions[i]->arity);
    }

    return 0;
}

int test_function_set_destroy(void)
{
    function_set_destroy(fs);
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_function_set_new);
    mu_add_test(test_function_set_destroy);
}

mu_run_tests(test_suite);
