#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef MU_PRINT
#define MU_PRINT 1
#endif

#define IRIS_DATA "./tests/data/iris.csv"

#include "munit.h"
#include "data.h"
#include "utils.h"
#include "gp/tree.h"


/* TESTS */
int test_data_new_and_destroy(void);
int test_data_field_index(void);

int test_csv_num_cols(void);
int test_csv_num_rows(void);
int test_csv_parse_header(void);
int test_csv_info(void);
int test_csv_parse_data_row(void);
int test_csv_load_data(void);

void test_suite(void);


int test_csv_num_cols(void)
{
    int cols;
    const char *token = ",";

    FILE *f = fopen(IRIS_DATA, "r");
    cols = csv_num_cols(f, token);
    mu_check(cols == 5);
    mu_print("cols: %d\n", cols);
    fclose(f);

    return 0;
}

int test_csv_num_rows(void)
{
    int rows;

    FILE *f = fopen(IRIS_DATA, "r");
    rows = csv_num_rows(f, 1);
    mu_check(rows == 150);
    mu_print("rows: %d\n", rows);
    fclose(f);

    return 0;
}

int test_csv_parse_header(void)
{
    int i;
    FILE *f = fopen(IRIS_DATA, "r");
    int cols = csv_num_cols(f, ",");
    const char **fields = csv_parse_header(f, cols, ",");

    for (i = 0; i < cols; i++) {
        mu_print("field[%d]: %s\n", i, fields[i]);
        mu_check(fields[i] != NULL);
        mu_check(strlen(fields[i]) > 0);
    }

    /* clean up */
    free_mem_arr(fields, cols, free);
    fclose(f);

    return 0;
}

int test_csv_info(void)
{
    int i;
    FILE *f = fopen(IRIS_DATA, "r");
    const char *token = ",";
    struct data *d = csv_info(f, 1, token);

    mu_check(d->rows == 150);
    mu_check(d->cols == 5);
    for (i = 0; i < d->cols; i++) {
        mu_print("field[%d]: %s\n", i, d->fields[i]);
        mu_check(d->fields[i] != NULL);
        mu_check(strlen(d->fields[i]) > 0);
    }

    data_destroy(d);
    fclose(f);
    return 0;
}

int test_csv_parse_data_row(void)
{
    int i;
    char *line = malloc_string("1.0, 2.0, 3.0, 4.0");
    float **result = csv_parse_data_row(line, 4, ",");

    for (i = 0; i < 4; i++) {
        mu_print("[%d]: %f\n", i, *(float *) result[i]);
    }

    free_mem_arr(result, 4, free);
    free(line);
    return 0;
}

int test_csv_load_data(void)
{
    int i;
    int j;
    struct data *d;

    d = csv_load_data(IRIS_DATA, 1, ",");
    for (i = 0; i < d->rows; i++) {
        for (j = 0; j < d->cols; j++) {
            mu_print("%f ", *(float *) d->data[j][i]);
        }
        mu_print("\n");
    }
    data_destroy(d);

    return 0;
}

int test_data_new_and_destroy(void)
{
    int i;
    int rows = 10;
    int cols = 3;

    const char *fields[3] = {"x", "y", "z"};
    struct data *d = data_new(rows, cols, fields);

    float *data_point = malloc_float(3.0);
    float *data_point_2 = malloc_float(1.0);

    d->data[0][0] = data_point;
    d->data[0][1] = data_point_2;

    mu_check(d != NULL);
    mu_check(d->rows == rows);
    mu_check(d->cols == cols);
    mu_check(d->fields != NULL);
    mu_check(d->data != NULL);

    /* assert */
    mu_print("d->data[0][0]: %f\n", *(float *) d->data[0][0]);
    mu_print("d->data[0][1]: %f\n", *(float *) d->data[0][1]);

    mu_check(fltcmp(d->data[0][0], data_point) == 0);
    mu_check(fltcmp(d->data[0][1], data_point_2) == 0);
    for (i = 0; i < cols; i++) {
        mu_print("field[%d]: %s\n", i, d->fields[i]);
        mu_check(strcmp(fields[i], d->fields[i]) == 0);
    }

    data_destroy(d);
    return 0;
}

int test_data_field_index(void)
{

    return 0;
}

void test_suite(void)
{
    mu_add_test(test_data_new_and_destroy);
    mu_add_test(test_data_field_index);

    mu_add_test(test_csv_num_cols);
    mu_add_test(test_csv_num_rows);
    mu_add_test(test_csv_parse_header);
    mu_add_test(test_csv_info);
    mu_add_test(test_csv_parse_data_row);
    mu_add_test(test_csv_load_data);
}

mu_run_tests(test_suite)
