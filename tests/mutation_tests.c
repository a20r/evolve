#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>

#include <mutation.h>

#define LOOP 100
#define TEST_STR "Hello World"
#define TEST_DNA "ACGCT"
#define TEST_NUM "00000"

int test_mutate_str()
{
        int i = 0;

        for (i = 0; i < LOOP; i++) {
                char *str = malloc(sizeof(char) * (strlen(TEST_STR) + 1));
                strcpy(str, TEST_STR);

                printf("Original string: %s\n", str);
                mutate_str(&str);
                printf("Mutated string:  %s\n\n", str);
                fflush(stdout);

                mu_assert(
                        strcmp(str, TEST_STR) != 0,
                        "Failed to mutate string"
                );
                mu_assert(
                        strlen(str) == strlen(TEST_STR),
                        "Failed to mutate string"
                );

                free(str);
        }

        return 0;
}

int test_mutate_num()
{
        int i = 0;

        for (i = 0; i < LOOP; i++) {
                char *num_str = malloc(sizeof(char) * (strlen(TEST_NUM) + 1));
                strcpy(num_str, TEST_NUM);

                printf("Original NUM: %s\n", num_str);
                mutate_num(&num_str);
                printf("Mutated NUM:  %s\n\n", num_str);
                fflush(stdout);

                mu_assert(
                        strcmp(num_str, TEST_NUM) != 0,
                        "Failed to mutate num"
                );
                mu_assert(
                        strlen(num_str) == strlen(TEST_NUM),
                        "Failed to mutate num"
                );

                free(num_str);
        }

        return 0;
}

int test_mutate_dna()
{
        int i = 0;

        for (i = 0; i < LOOP; i++) {
                char *dna_str = malloc(sizeof(char) * (strlen(TEST_DNA) + 1));
                strcpy(dna_str, TEST_DNA);

                printf("Original DNA: %s\n", dna_str);
                mutate_dna(&dna_str);
                printf("Mutated DNA:  %s\n\n", dna_str);
                fflush(stdout);

                mu_assert(
                        strcmp(dna_str, TEST_DNA) != 0,
                        "Failed to mutate dna"
                );
                mu_assert(
                        strlen(dna_str) == strlen(TEST_DNA),
                        "Failed to mutate dna"
                );

                free(dna_str);
        }

        return 0;
}

void test_suite()
{
        mu_run_test(test_mutate_str);
        mu_run_test(test_mutate_num);
        mu_run_test(test_mutate_dna);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        test_suite();
        mu_report();
}
