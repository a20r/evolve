#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "crossover.h"
#include "evolve.h"

/* GLOBAL VAR */
struct chromosome_pair *c_pair;
char *c_1_orig = "Hello Dave!\0";
char *c_2_orig = "Ciao Maria!\0";

void setup()
{
        int c_1_len = strlen(c_1_orig);
        int c_2_len = strlen(c_2_orig );

        c_pair = malloc(sizeof(struct chromosome_pair));

        c_pair->child_1 = calloc(1, sizeof(char *) * c_1_len);
        c_pair->child_2 = calloc(1, sizeof(char *) * c_2_len);

        memcpy(c_pair->child_1, c_1_orig, sizeof(char *) * c_1_len);
        memcpy(c_pair->child_2, c_2_orig , sizeof(char *) * c_2_len);
}

void teardown()
{
        free(c_pair->child_1);
        free(c_pair->child_2);
        free(c_pair);
}

void print_before_pivot(struct chromosome_pair *c_pair)
{
        printf("Before:\n");
        print_chromosome_pair(c_pair);
}

void print_after_pivot(struct chromosome_pair *c_pair)
{
        printf("After:\n");
        print_chromosome_pair(c_pair);
}


int std_crossover_asserts(struct chromosome_pair *c_pair)
{
        char *c_1;
        char *c_2;
        int c_1_len = 0;
        int c_2_len = 0;

        c_1 = c_pair->child_1;
        c_2 = c_pair->child_2;
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
        print_before_pivot(c_pair);
        one_ptr_crossover(&c_pair, pivot);
        mu_assert(
                strcmp(c_pair->child_1, "HelloMaria!") == 0,
                "Failed to crossover child 1!"
        );
        mu_assert(
                strcmp(c_pair->child_2, "Ciao  Dave!") == 0,
                "Failed to crossover child 2!"
        );
        print_after_pivot(c_pair);

        /* slightly different crossover - pivot at index = 3 */
        print_before_pivot(c_pair);
        one_ptr_crossover(&c_pair, 3);
        mu_assert(
                strcmp(c_pair->child_1, "Helo  Dave!") == 0,
                "Failed to crossover!"
        );
        mu_assert(
                strcmp(c_pair->child_2, "CialoMaria!") == 0,
                "Failed to crossover!"
        );
        print_after_pivot(c_pair);

        teardown();
        return 0;
}

void test_suite()
{
        mu_run_test(test_one_ptr_crossover);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
