#include <stdio.h>
#include <stdlib.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "config.h"
#include "selection.h"


/* TESTS */
int test_selection_config_new_and_destroy(void);
void test_suite(void);


int test_selection_config_new_and_destroy(void)
{
    struct selection_config *sc = selection_config_new(TOURNAMENT_SELECTION);
    selection_config_destroy(sc);
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_selection_config_new_and_destroy);
}

mu_run_tests(test_suite)
