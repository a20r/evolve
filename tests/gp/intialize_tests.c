#include <time.h>
#include <math.h>
#include <string.h>

#include <dstruct/darray.h>
#include <munit/munit.h>
#include <al/utils.h>

#include "evolve.h"
#include "population.h"
#include "selection.h"

/* GLOBAL VAR */

int test_init_tree_full()
{
        return 0;
}

int test_init_tree_grow()
{
        return 0;
}

int test_init_tree_ramped_half_and_half()
{
        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        mu_run_test(test_init_tree_full);
        mu_run_test(test_init_tree_grow);
        mu_run_test(test_init_tree_ramped_half_and_half);
}

mu_run_test_suite(test_suite);
