#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "test_utils.h"

#include "evolve.h"
#include "evolve_utils.h"
#include "selection.h"
#include "utils.h"

#define TEST_OUTPUT "evolve_utils_tests.out"

/* GLOBAL VAR */
struct population *p;
int output;
int save_stdout;


static float fitness_function(char *chromosome)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += fabsf(roundf(target[i] - chromosome[i]));
        }

        return total;
}

static void redirect_stdout()
{
        /* open output file */
        output = open(TEST_OUTPUT, O_RDWR|O_CREAT, 0755);
        if (output == -1) {
                printf("Error opening [%s]!\n", TEST_OUTPUT);
                exit(-1);
        }

        /* save stdout */
        save_stdout = dup(1);

        /* redirect stdout to file */
        if (dup2(output, 1) == -1) {
                printf("Failed to redirect stdout!\n");
                exit(-1);
        }
}

static void restore_stdout()
{
        /* flush output */
        fflush(stdout);
        close(output);

        /* restore stdout */
        dup2(save_stdout, 1);
        close(save_stdout);
}

static void setup(int max_pop, int max_gen)
{
        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                max_pop,  /* max_pop */
                max_gen /* max_gen */
        );
        gen_init_chromosomes(&p, randstr);
        evaluate_chromosomes(fitness_function, &p);
}

static void teardown()
{
        destroy_population(&p);
}

static void testsuite_cleanup()
{
        int result = remove(TEST_OUTPUT);

        if (result == 0) {
                printf("Removed file [%s]!\n", TEST_OUTPUT);
        } else {
                printf("ERROR Failed to removed file [%s]!\n", TEST_OUTPUT);
        }
}

static int read_file(char* fp)
{
        int line_counter = 0;
        char line[128];

        FILE *file = fopen(fp, "r");

        while (fgets(line, sizeof(line), file) != NULL) {
                fputs(line, stdout);
                line_counter++;
        }

        fclose(file);

        return line_counter;
}

int test_print_chromosome()
{
        int max_pop = 10;
        int max_gen = 5;

        redirect_stdout();
        setup(max_pop, max_gen);

        print_chromosome(p, 0);

        teardown();
        restore_stdout();

        mu_assert(read_file(TEST_OUTPUT) != 0, "There should be some output!");

        return 0;
}

int test_print_chromosomes()
{
        int max_pop = 10;
        int max_gen = 5;

        redirect_stdout();
        setup(max_pop, max_gen);

        print_chromosomes(p);

        teardown();
        restore_stdout();

        mu_assert(read_file(TEST_OUTPUT) != 0, "There should be some output!");

        return 0;
}

int test_print_population()
{
        int max_pop = 10;
        int max_gen = 5;

        redirect_stdout();
        setup(max_pop, max_gen);

        print_population(p);

        teardown();
        restore_stdout();

        mu_assert(read_file(TEST_OUTPUT) != 0, "There should be some output!");

        return 0;
}

int test_insertion_sort_population()
{
        int res = 0;
        int max_pop = 10;
        int max_gen = 5;

        setup(max_pop, max_gen);

        printf("Before Population Sort\n");
        print_chromosomes(p);

        /* sort population */
        insertion_sort_population(p, 0, p->chromosomes->end, float_cmp_asc);

        printf("After Population Sort\n");
        print_chromosomes(p);

        /* #<{(| assert tests |)}># */
        res = assert_sorted_population(p, float_cmp_asc);
        mu_assert(res == 0, "Failed to sort population!");

        teardown();

        return 0;
}

int test_partition_population()
{
        int res = 0;
        int max_pop = 10;
        int max_gen = 5;

        setup(max_pop, max_gen);

        printf("Before Population Sort\n");
        print_chromosomes(p);

        /* keep pivot value for reference */
        int pivot_index = 2;
        float pivot_value = *(float *) darray_get(
                p->chromosome_scores,
                pivot_index
        );

        /* partition population */
        res = partition_population(
                p,
                pivot_index,
                0,
                p->chromosomes->end,
                float_cmp_asc
        );

        printf("After Population Sort\n");
        print_chromosomes(p);

        /* assert tests */
        insertion_sort_population(p, 0, p->chromosomes->end, float_cmp_asc);
        float value = *(float *) darray_get(p->chromosome_scores, res);

        /* assert test */
        mu_assert(
                float_cmp_asc(&value, &pivot_value) == 0,
                "Failed to partition population!"
        );
        printf("value_1: %f \t value_2: %f", pivot_value, value);

        teardown();

        return 0;
}

int test_quick_sort_population()
{
        int res = 0;
        int max_pop = 100;
        int max_gen = 5;

        setup(max_pop, max_gen);


        printf("Before Population Sort\n");
        print_chromosomes(p);

        /* sort population */
        quick_sort_population(p, 0, p->chromosomes->end, float_cmp_asc);

        printf("After Population Sort\n");
        print_chromosomes(p);

        /* #<{(| assert tests |)}># */
        res = assert_sorted_population(p, float_cmp_asc);
        mu_assert(res == 0, "Failed to sort population!");

        teardown();

        return 0;
}

int test_sort_population()
{
        int res = 0;
        int max_pop = 100;
        int max_gen = 5;

        setup(max_pop, max_gen);

        printf("Before Population Sort\n");
        print_chromosomes(p);

        /* sort population */
        sort_population(p, float_cmp_asc);

        printf("After Population Sort\n");
        print_chromosomes(p);

        /* #<{(| assert tests |)}># */
        res = assert_sorted_population(p, float_cmp_asc);
        mu_assert(res == 0, "Failed to sort population!");

        teardown();

        return 0;
}


void test_suite()
{
        /* print functions */
        mu_run_test(test_print_chromosome);
        mu_run_test(test_print_chromosomes);
        mu_run_test(test_print_population);

        /* sort functions */
        mu_run_test(test_insertion_sort_population);
        mu_run_test(test_partition_population);
        mu_run_test(test_quick_sort_population);
        mu_run_test(test_sort_population);

        testsuite_cleanup();
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
