#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/darray.h>

#include "ga/initialize.h"

/* GLOBAL VAR */
struct population *p;
int max_pop = 10;


static void setup(int max_pop, int max_gen)
{
        p = population_create(
                (int) strlen("hello world!"),  /* chromo_length */
                0.0,  /* goal */
                max_pop,  /* max_pop */
                max_gen /* max_gen */
        );
}

static void teardown()
{
        population_destroy(&p, free);
}

int test_initialize_population()
{
        int i = 0;
        char *individual = '\0';
        char *last_individual = '\0';

        setup(max_pop, 1);
        initialize_population(p, randstr);

        /* assert tests */
        for (i = 0; i < p->max_population; i++) {
                individual = darray_get(p->individuals, i);

                mu_assert(individual != last_individual, "num == last_num!");
                mu_assert(p->population == max_pop,"Invalid population!");

                last_individual = individual;
        }

        teardown();

        return 0;
}

void test_suite()
{
        mu_run_test(test_initialize_population);
}

mu_run_test_suite(test_suite);
