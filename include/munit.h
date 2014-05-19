#ifndef _MUNIT_H_
#define _MUNIT_H_
#include <stdio.h>

/* GLOBAL VARS */
int tests;
int passed;
int failed;

/* MACROS */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/* MUNIT MACROS */
#define mu_assert(test, message) \
    do { \
        if (!(test)) { \
            printf( \
                "%sERROR!%s [%s:%d] %s\n", \
                KRED, \
                KNRM, \
                __func__, \
                __LINE__, \
                message \
            ); \
            return -1; \
        }\
    } while (0)

#define mu_check(test) \
    do { \
        if (!(test)) { \
            printf( \
                "%sERROR!%s [%s:%d] %s %sFAILED!%s\n", \
                KRED, \
                KNRM, \
                __func__, \
                __LINE__, \
                #test, \
                KRED, \
                KNRM \
            ); \
            return -1; \
        }\
    } while (0)

#define mu_run_test(test) \
    do { \
        tests++; \
        printf( \
            "%s-> %s %s\n", \
            KBLU, \
            #test, \
            KNRM \
        ); \
        fflush(stdout); \
        if (test() == -1) { \
            printf("%sTEST FAILED!%s\n\n", \
                KRED, \
                KNRM \
            ); \
            failed++; \
        } else { \
            printf("%sTEST PASSED!%s\n\n", \
                KGRN, \
                KNRM \
            ); \
            passed++; \
        } \
    } while (0)

#define mu_report() \
    do { \
        printf(KBLU); \
        printf("%d tests, ", tests); \
        printf("%d passed, ", passed); \
        printf("%d failed\n", tests - passed); \
        printf("\n"); \
        printf(KNRM); \
        if (failed != 0) return -1; \
        else return 0; \
    } while (0)
#define mu_run_test_suite(TEST_SUITE) \
    int main() \
    { \
            TEST_SUITE(); \
            mu_report(); \
    }

#endif
