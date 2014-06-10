#include <stdio.h>
#include <stdlib.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"


/* TESTS */
int test_something(void);
void test_suite(void);


int test_something(void)
{

    return 0;
}

void test_suite(void)
{
    mu_add_test(test_something);
}

mu_run_tests(test_suite)
