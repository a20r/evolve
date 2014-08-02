#ifdef OPENCL

#include "munit.h"
#include "parallel.h"


/* TESTS */
int test_opencl_info(void);
void test_suite(void);

int test_opencl_info(void)
{
    opencl_info();
    return 0;
}


void test_suite(void)
{
    mu_add_test(test_opencl_info);
}

mu_run_tests(test_suite)
#endif
