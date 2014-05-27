#include "munit.h"
#include "tree.h"

int test_function_set_new(void)
{
    int functions[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arities[10] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1};
    struct function_set *fs = function_set_new(functions, arities, 10);

    mu_check(fs != NULL);

    return 0;
}

void test_suite(void)
{
    mu_add_test(test_function_set_new);
}

mu_run_tests(test_suite);
