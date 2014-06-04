#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#include "munit.h"
#include "cmp.h"
#include "data.h"


/* TESTS */
int test_data_new_and_destroy(void);
void test_suite(void);

int test_data_new_and_destroy(void)
{
    int rows = 10;
    int cols = 3;
    const char *fields[3] = {"x", "y", "z"};
    struct data *d = data_new(rows, cols, (void *) fields);

    mu_check(d != NULL);
    mu_check(d->rows == rows);
    mu_check(d->cols == cols);
    mu_check(d->fields != NULL);
    mu_check(d->data != NULL);

    float *data_point = malloc(sizeof(float));
    float *data_point_2 = malloc(sizeof(float));
    *(float *) data_point = 3.0;
    *(float *) data_point_2 = 1.0;

    d->data[0][0] = data_point;
    d->data[0][1] = data_point_2;

    printf("d->data[0][0]: %f\n", *(float *) d->data[0][0]);
    printf("d->data[0][1]: %f\n", *(float *) d->data[0][1]);

    /* assert */
    mu_check(floatcmp(d->data[0][0], data_point) == 0);
    mu_check(floatcmp(d->data[0][1], data_point_2) == 0);

    data_destroy(d);
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_data_new_and_destroy);
}

mu_run_tests(test_suite)
