#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>

#include <mutation.h>

#define LOOP 100
#define TEST_STR "Hello World"
#define TEST_DNA "ACGCT"

int test_mutate_str()
{
        int i = 0;

        for (i = 0; i < LOOP; i++) {
                char *str = malloc(sizeof(char) * (strlen(TEST_STR) + 1));
                strcpy(str, TEST_STR);

                printf("Original string: %s\n", str);
                mutate_str(&str);
                printf("Mutated string: %s\n", str);
                fflush(stdout);
                mu_assert(strcmp(str, TEST_STR) != 0, "Failed to mutate string");

                free(str);
        }

        return 0;
}

int test_mutate_dna()
{
        int i = 0;

        for (i = 0; i < LOOP; i++) {
                char *dna = malloc(sizeof(char) * (strlen(TEST_DNA) + 1));
                strcpy(dna, TEST_DNA);

                printf("Original DNA: %s\n", dna);
                mutate_dna(&dna);
                printf("Mutated DNA: %s\n", dna);
                fflush(stdout);
                mu_assert(strcmp(dna, TEST_DNA) != 0, "Failed to mutate dna");

                free(dna);
        }

        return 0;
}

void test_suite()
{
        mu_run_test(test_mutate_str);
        mu_run_test(test_mutate_dna);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        test_suite();
        mu_report();
}
