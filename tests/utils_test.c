#include <stdlib.h>
#include <string.h>

#include "munit.h"
#include "utils.h"


/* TESTS */
int test_trim_char(void);
int test_trim(void);
int test_copy_value(void);
void test_suite(void);


int test_trim_char(void)
{
    mu_check(trim_char('\n') == 1);
    mu_check(trim_char('\t') == 1);
    mu_check(trim_char('\r') == 1);
    mu_check(trim_char(' ') == 1);
    mu_check(trim_char('A') == 0);

    return 0;
}

int test_trim(void)
{
    char *result;
    const char *test_1 = "\nHello World\n ";
    const char *test_2 = "\n\tWorld\n \t";

    result = trim(test_1);
    mu_check(strcmp(result, "Hello World") == 0);
    free(result);

    result = trim(test_2);
    mu_check(strcmp(result, "World") == 0);
    free(result);

    return 0;
}

int test_copy_value(void)
{
    int *int_ptr;
    float *float_ptr;
    double *double_ptr;
    char *str_ptr;

    int int_value = 0;
    float float_value = 10.0;
    double doule_value = 100.0;
    const char *str_value = "test";

    int_ptr = copy_value(INTEGER, &int_value);
    mu_check(intcmp(int_ptr, &int_value) == 0);

    float_ptr = copy_value(FLOAT, &float_value);
    mu_check(fltcmp(float_ptr, &float_value) == 0);

    double_ptr = copy_value(DOUBLE, &doule_value);
    mu_check(fltcmp(double_ptr, &doule_value) == 0);

    str_ptr = copy_value(STRING, (void *) str_value);
    mu_check(strcmp(str_ptr, str_value) == 0);

    /* clean up */
    free(int_ptr);
    free(float_ptr);
    free(double_ptr);
    free(str_ptr);
    return 0;
}

void test_suite(void)
{
    mu_add_test(test_trim_char);
    mu_add_test(test_trim);
    mu_add_test(test_copy_value);
}

mu_run_tests(test_suite)
