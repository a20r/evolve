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
        p = init_population(
                (int) strlen("hello world!"),  /* chromo_length */
                0.0,  /* goal */
                max_pop,  /* max_pop */
                max_gen /* max_gen */
        );
}

static void teardown()
{
        destroy_population(&p);
}

int test_init_individuals()
{
        int i = 0;
        char *individual = '\0';
        char *last_individual = '\0';

        setup(max_pop, 1);
        init_individuals(&p, randstr);

        /* assert tests */
        for (i = 0; i < p->max_population; i++) {
                individual = darray_get(p->individuals, i);

                mu_assert(
                                individual != last_individual,
                                "num == last_num!"
                         );
                mu_assert(p->population == max_pop,"Invalid population!");

                last_individual = individual;
        }

        teardown();

        return 0;
}

void test_suite()
{
        mu_run_test(test_init_individuals);
}

mu_run_test_suite(test_suite);
