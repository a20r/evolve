#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "evolve.h"
#include "ga/crossover.h"

/* GLOBAL VAR */
struct chromosome_pair *c_pair;
char *c_1_orig = "123456789\0";
char *c_2_orig = "ABCDEFGHI\0";
void *chromo_1;
void *chromo_2;

static void setup()
{
        int c_1_len = strlen(c_1_orig) + 1;  /* + 1 for null terminator */
        int c_2_len = strlen(c_2_orig) + 1;  /* + 1 for null terminator */

        chromo_1 = calloc(1, sizeof(char) * c_1_len);
        chromo_2 =  calloc(1, sizeof(char) * c_2_len);

        memcpy(chromo_1, c_1_orig, sizeof(char) * c_1_len);
        memcpy(chromo_2, c_2_orig , sizeof(char) * c_2_len);
}

static void teardown()
{
        free(chromo_1);
        free(chromo_2);
}

void print_before_pivot()
{
        printf("Before:\n");
        printf("Chromosome 1: %s\n", (char *) chromo_1);
        printf("Chromosome 2: %s\n", (char *) chromo_2);
}

void print_after_pivot()
{
        printf("After:\n");
        printf("Chromosome 1: %s\n", (char *) chromo_1);
        printf("Chromosome 2: %s\n", (char *) chromo_2);
}


int std_crossover_asserts()
{
        char *c_1 = '\0';
        char *c_2 = '\0';
        int c_1_len = 0;
        int c_2_len = 0;

        c_1 = chromo_1;
        c_2 = chromo_2;
        c_1_len = strlen(c_1);
        c_2_len = strlen(c_2);

        /* asserts */
        mu_assert(c_1[c_1_len] == '\0', "c_1 should have null terminator!");
        mu_assert(c_2[c_2_len] == '\0', "c_2 should have null terminator!");
        mu_assert(c_1_len == (int) strlen(c_1_orig), "Invalid c_1 length!");
        mu_assert(c_2_len == (int) strlen(c_2_orig), "Invalid c_2 length!");

        return 0;
}

int test_one_ptr_crossover()
{
        int pivot = DEFAULT_PIVOT;

        setup();

        /* default crossover  */
        print_before_pivot();
        one_pt_crossover(&chromo_1, &chromo_2, pivot);
        mu_assert(
                strcmp(chromo_1, "12345FGHI") == 0,
                "Failed to crossover child 1!"
        );
        mu_assert(
                strcmp(chromo_2, "ABCDE6789") == 0,
                "Failed to crossover child 2!"
        );
        print_after_pivot();

        /* slightly different crossover - pivot at index = 3 */
        print_before_pivot();
        one_pt_crossover(&chromo_1, &chromo_2, 5);
        mu_assert(
                strcmp(chromo_1, "123456789") == 0,
                "Failed to crossover child 1!"
        );
        mu_assert(
                strcmp(chromo_2, "ABCDEFGHI") == 0,
                "Failed to crossover child 2!"
        );
        print_after_pivot();

        teardown();
        return 0;
}

int test_two_ptr_crossover()
{
        int pivot = DEFAULT_PIVOT;

        setup();

        /* default crossover  */
        print_before_pivot();
        two_pt_crossover(&chromo_1, &chromo_2, pivot);
        mu_assert(
                strcmp(chromo_1, "123DEF789") == 0,
                "Failed to crossover child 1!"
        );
        mu_assert(
                strcmp(chromo_2, "ABC456GHI") == 0,
                "Failed to crossover child 2!"
        );
        print_after_pivot();

        /* slightly different crossover - pivot at index = 3 */
        print_before_pivot();
        two_pt_crossover(&chromo_1, &chromo_2, 3);
        mu_assert(
                strcmp(chromo_1, "123456789") == 0,
                "Failed to crossover child 1!"
        );
        mu_assert(
                strcmp(chromo_2, "ABCDEFGHI") == 0,
                "Failed to crossover child 2!"
        );
        print_after_pivot();

        teardown();
        return 0;
}

void test_suite()
{
        mu_run_test(test_one_ptr_crossover);
        mu_run_test(test_two_ptr_crossover);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
