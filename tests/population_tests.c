#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/darray.h>

#include "test_utils.h"

#include "population.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"

#define TEST_OUTPUT "evolve_utils_tests.out"

/* GLOBAL VAR */
struct population *p;
struct evolve_monitor *m;
int max_pop = 10;

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
}

static void print_setup(int max_pop, int max_gen)
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

int test_init_population()
{
        setup(max_pop, 1);

        /* chromosomes */
        mu_assert(p->chromosomes != NULL, "Chromosomes should not be NULL!");
        mu_assert(p->scores != NULL, "Scores should not be NULL!");
        mu_assert(p->total_score == 0.0, "Sum of scores should be 0.0!");

        /* fitness details */
        mu_assert(p->parameters == 12, "Parameters should be 12!");
        mu_assert(p->goal == 0.0, "Goal should be 0.0!");

        /* population details */
        mu_assert(p->population == 0, "Current population should be 0!");
        mu_assert(p->generation == 0, "Current generation should be 0!");
        mu_assert(p->max_population == max_pop, "Invalid max population!");
        mu_assert(p->max_generation == 1, "Max generation should be 1!");
        mu_assert(p->solution == NULL, "Solution should be NULL!");

        teardown();

        return 0;
}

int test_extend_max_generation()
{
        setup(max_pop, 1);

        /* extend max generations */
        extend_max_generation(p, 100);

        /* assert extend */
        mu_assert(p->max_generation == 101, "Failed to extend generations!");

        teardown();

        return 0;
}

int test_gen_init_chromosomes()
{
        int i = 0;
        char *chromosome = '\0';
        char *last_chromosome = '\0';

        setup(max_pop, 1);
        gen_init_chromosomes(&p, randstr);

        /* assert tests */
        for (i = 0; i < p->max_population; i++) {
                chromosome = darray_get(p->chromosomes, i);

                mu_assert(
                                chromosome != last_chromosome,
                                "num == last_num!"
                         );
                mu_assert(p->population == max_pop,"Invalid population!");

                last_chromosome = chromosome;
        }

        teardown();

        return 0;
}

int test_evaluate_chromosomes()
{
        int i = 0;
        int goal_reached = 0;
        float score = 0.0;
        float last_score = 0.0;
        char *chromosome_solution = "hello world!";
        char *solution;

        /* setup */
        setup(max_pop, 1);
        gen_init_chromosomes(&p, randstr);

        /* replace last chromosome to match the solution */
        solution = malloc(sizeof(char *) * p->parameters);
        memcpy(solution, chromosome_solution, sizeof(char *) * p->parameters);
        free(darray_get(p->chromosomes, 4));  /* MUST FREE BEFORE REPLACING! */
        darray_set(p->chromosomes, 4, solution);

        /* evaluate */
        goal_reached = evaluate_chromosomes(fitness_function, &p);
        mu_assert(goal_reached == 1, "Goal should have been reached!");

        /* loop through and check scores */
        for (i = 0; i < p->max_population; i++) {
                score = *((float *) darray_get(p->scores, i));

                mu_assert(
                                score != last_score,
                                "score == last_score!"
                         );
                mu_assert(
                                p->population == max_pop,
                                "Invalid population!"
                         );

                last_score = score;
        }
        mu_assert(p->total_score != 0.0, "Sum of scores should not be 0!");

        /* teardown */
        teardown();

        return 0;
}

int test_normalize_fitness_values()
{
        int i = 0;
        float sum = 0;
        float unity = 1;
        int cmp_res = 0;

        /* setup */
        setup(10, 1);
        gen_init_chromosomes(&p, randstr);
        evaluate_chromosomes(fitness_function, &p);

        printf("Before Normalization\n");
        print_population(p);

        normalize_fitness_values(&p);

        printf("After Normalization\n");
        print_population(p);

        /* assert tests */
        for (i = 0; i < p->max_population; i++) {
                sum += *(float *) darray_get(p->scores, i);
        }

        sum = round(sum);
        cmp_res = float_cmp(&sum, &unity);
        mu_assert(cmp_res == 0, "Sum of normalized values should be 1!");

        /* teardown */
        teardown();

        return 0;
}

int test_print_chromosome()
{
        int max_pop = 10;
        int max_gen = 5;

        redirect_stdout();
        print_setup(max_pop, max_gen);

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
        print_setup(max_pop, max_gen);

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
        print_setup(max_pop, max_gen);

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

        /* setup */
        setup(max_pop, max_gen);
        gen_init_chromosomes(&p, randstr);
        evaluate_chromosomes(fitness_function, &p);

        printf("Before Population Sort\n");
        print_chromosomes(p);

        /* keep pivot value for reference */
        int pivot_index = 2;
        float pivot_value = *(float *) darray_get(
                p->scores,
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
        float value = *(float *) darray_get(p->scores, res);

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

int test_populate()
{
        /* setup */
        setup(max_pop, 1);
        gen_init_chromosomes(&p, randstr);
        evaluate_chromosomes(fitness_function, &p);
        roulette_wheel_selection(&p, NULL);

        float *crossover_prob = calloc(1, sizeof(float));
        float *mutate_prob = calloc(1, sizeof(float));
        *crossover_prob = 0.9;
        *mutate_prob = 0.2;

        populate(&p,
                one_pt_crossover,
                crossover_prob,
                DEFAULT_PIVOT,
                mutate_str,
                mutate_prob
        );

        /* assert tests */
        mu_assert(p->chromosomes->end == max_pop - 1, "Invalid end value!");
        mu_assert(p->population == max_pop, "Invalid current population!");

        /* teardown */
        teardown();

        return 0;
}

int test_destroy_population()
{
        destroy_population(&p);
        mu_assert(p == NULL, "Struct population should be NULL!");

        return 0;
}

void test_suite()
{
        /* tests population functions */
        mu_run_test(test_init_population);
        mu_run_test(test_extend_max_generation);
        mu_run_test(test_gen_init_chromosomes);
        mu_run_test(test_evaluate_chromosomes);
        mu_run_test(test_normalize_fitness_values);
        mu_run_test(test_destroy_population);

        /* print functions */
        mu_run_test(test_print_chromosome);
        mu_run_test(test_print_chromosomes);
        mu_run_test(test_print_population);

        /* sort functions */
        mu_run_test(test_insertion_sort_population);
        mu_run_test(test_partition_population);
        mu_run_test(test_quick_sort_population);
        mu_run_test(test_sort_population);

        /* tests populate */
        mu_run_test(test_populate);

        testsuite_cleanup();
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
