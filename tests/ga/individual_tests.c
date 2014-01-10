#include <string.h>
#include <munit/munit.h>

#include "ga/individual.h"


int test_ga_individual_create()
{
        struct ga_individual *individual;
        char *chromo = "1010101010";

        individual = ga_individual_create(chromo);
        mu_check(strcmp(individual->chromosome, chromo) == 0);
        mu_check(individual->score == 0.0);

        return 0;
}

int test_ga_individual_destroy()
{
        struct ga_individual *individual;
        char *chromo = "1010101010";

        individual = ga_individual_create(chromo);
        ga_individual_destroy(individual);
        mu_check(individual == NULL);

        return 0;
}

void test_suite()
{
        mu_run_test(test_ga_individual_create);
        mu_run_test(test_ga_individual_destroy);
}

mu_run_test_suite(test_suite);
