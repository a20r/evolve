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

#include "ga/initialize.h"
#include "ga/crossover.h"
#include "ga/mutation.h"

#include "gp/initialize.h"
#include "gp/data_loader.h"
#include "gp/selection.h"
#include "gp/crossover.h"
#include "gp/mutation.h"
#include "gp/tree_parser.h"
#include "gp/tree_evaluator.h"

#define TEST_OUTPUT "evolve_utils_tests.out"
#define CONFIG_FILE "examples/sym_regression/config.json"
#define TEST_INPUT_DATA "examples/sym_regression/data/sine_input.dat"
#define TEST_RESPONSE_DATA "examples/sym_regression/data/sine_response.dat"

/* GLOBAL VAR */
struct population *p;
struct evolve_monitor *m;
int max_pop = 10;

int output;
int save_stdout;


static float fitness_function(void *individual)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(individual); i++) {
                total += fabsf(roundf(target[i] - ((char *) individual)[i]));
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
        p = population_create(
                (int) strlen("hello world!") + sizeof(char),
                0.0,  /* goal */
                max_pop,  /* max_pop */
                max_gen /* max_gen */
        );
}

static void print_setup(int max_pop, int max_gen)
{
        p = population_create(
                (int) strlen("hello world!") + sizeof(char),
                0.0,  /* goal */
                max_pop,  /* max_pop */
                max_gen /* max_gen */
        );
        ga_population_initialize(p, randstr);
        evaluate_individuals(fitness_function, &p);
}

static void teardown()
{
        population_destroy(&p, free);
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

int test_population_create()
{
        setup(max_pop, 1);

        /* individuals */
        mu_assert(p->individuals != NULL, "individuals should not be NULL!");
        mu_assert(p->scores != NULL, "Scores should not be NULL!");
        mu_assert(p->total_score == 0.0, "Sum of scores should be 0.0!");

        /* fitness details */
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

int test_evaluate_individuals()
{
        int i = 0;
        int goal_reached = 0;
        float score = 0.0;
        float last_score = 0.0;
        char *individual_solution = "hello world!";
        char *solution;

        /* setup */
        setup(max_pop, 1);
        ga_population_initialize(p, randstr);

        /* replace last individual to match the solution */
        solution = malloc(p->individuals->element_size);
        memcpy(
                solution,
                individual_solution,
                p->individuals->element_size
        );
        free(darray_get(p->individuals, 4));  /* MUST FREE BEFORE REPLACING! */
        darray_set(p->individuals, 4, solution);

        /* evaluate */
        goal_reached = evaluate_individuals(fitness_function, &p);
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
        ga_population_initialize(p, randstr);
        evaluate_individuals(fitness_function, &p);

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

int test_print_individual()
{
        int max_pop = 10;
        int max_gen = 5;

        redirect_stdout();
        print_setup(max_pop, max_gen);

        print_individual(p, 0);

        teardown();
        restore_stdout();

        mu_assert(read_file(TEST_OUTPUT) != 0, "There should be some output!");

        return 0;
}

int test_print_individuals()
{
        int max_pop = 10;
        int max_gen = 5;

        redirect_stdout();
        print_setup(max_pop, max_gen);

        print_individuals(p);

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
        print_individuals(p);

        /* sort population */
        insertion_sort_population(p, 0, p->individuals->end, float_cmp_asc);

        printf("After Population Sort\n");
        print_individuals(p);

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
        ga_population_initialize(p, randstr);
        evaluate_individuals(fitness_function, &p);

        printf("Before Population Sort\n");
        print_individuals(p);

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
                p->individuals->end,
                float_cmp_asc
        );

        printf("After Population Sort\n");
        print_individuals(p);

        /* assert tests */
        insertion_sort_population(p, 0, p->individuals->end, float_cmp_asc);
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
        print_individuals(p);

        /* sort population */
        quick_sort_population(p, 0, p->individuals->end, float_cmp_asc);

        printf("After Population Sort\n");
        print_individuals(p);

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
        print_individuals(p);

        /* sort population */
        sort_population(p, float_cmp_asc);

        printf("After Population Sort\n");
        print_individuals(p);

        /* assert tests */
        res = assert_sorted_population(p, float_cmp_asc);
        mu_assert(res == 0, "Failed to sort population!");

        teardown();

        return 0;
}

int test_populate()
{
        /* setup */
        setup(max_pop, 1);
        ga_population_initialize(p, randstr);
        evaluate_individuals(fitness_function, &p);
        roulette_wheel_selection(&p, NULL);

        float *crossover_prob = calloc(1, sizeof(float));
        float *mutate_prob = calloc(1, sizeof(float));
        *crossover_prob = 0.9;
        *mutate_prob = 0.2;

        populate(
                &p,
                one_pt_crossover,
                crossover_prob,
                DEFAULT_PIVOT,
                mutate_str,
                mutate_prob
        );

        /* assert tests */
        mu_assert(p->individuals->end == max_pop - 1, "Invalid end value!");
        mu_assert(p->population == max_pop, "Invalid current population!");

        /* clean up*/
        teardown();
        free(crossover_prob);
        free(mutate_prob);

        return 0;
}

int test_reproduce()
{
        int i = 0;
        int individuals = 0;
        struct evolve_config *config = NULL;
        struct evolve_monitor *m = NULL;
        struct population *p = NULL;
        struct gp_tree *tree = NULL;

        config = load_config(CONFIG_FILE);
        load_data(TEST_INPUT_DATA, config, INPUT_DATA);
        load_data(TEST_RESPONSE_DATA, config, RESPONSE_DATA);
        p = gp_population_initialize(init_tree_full, config->general.gp_tree);
        individuals = p->individuals->end;

        evaluate_programs(p, config);
        printf("evaulate p->population: %d\n", p->population);
        gp_tournament_selection(p, NULL);
        /* printf("select p->population: %d\n", p->population); */
        /* struct gp_tree *tree; */
        for (i = 0; i < p->population; i++) {
                tree = darray_get(p->individuals, i);
                printf("- TREE[%d]", i);
                printf(" [size: %d]", tree->size);
                printf("[depth: %d]\n", tree->depth);
                print_gp_tree(tree->root);
                printf("\n\n");
        }

        /* reproduce( */
        /*         p, */
        /*         gp_one_point_crossover, */
        /*         gp_point_mutation, */
        /*         config */
        /* ); */
        /* mu_assert(p->individuals->end == individuals, "Failed to reproduce!"); */

        /* struct gp_tree *tree; */
        /* for (i = 0; i < p->population; i++) { */
        /*         tree = darray_get(p->individuals, i); */
        /*         printf("- TREE[%d]", i); */
        /*         printf(" [size: %d]", tree->size); */
        /*         printf("[depth: %d]\n", tree->depth); */
        /*         print_gp_tree(tree->root); */
        /*         printf("\n\n"); */
        /* } */

        population_destroy(&p, gp_tree_destroy);
        /* destroy_evolve_monitor_tree(m, gp_tree_destroy); */
        config_destroy(config);

        return 0;
}

int test_population_destroy()
{
        population_destroy(&p, free);
        mu_assert(p == NULL, "Struct population should be NULL!");

        return 0;
}

void test_suite()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* tests population functions */
        mu_run_test(test_population_create);
        mu_run_test(test_extend_max_generation);
        mu_run_test(test_evaluate_individuals);
        mu_run_test(test_normalize_fitness_values);
        mu_run_test(test_population_destroy);

        /* print functions */
        mu_run_test(test_print_individual);
        mu_run_test(test_print_individuals);
        mu_run_test(test_print_population);

        /* sort functions */
        mu_run_test(test_insertion_sort_population);
        mu_run_test(test_partition_population);
        mu_run_test(test_quick_sort_population);
        mu_run_test(test_sort_population);

        /* tests populate */
        mu_run_test(test_populate);
        mu_run_test(test_reproduce);

        testsuite_cleanup();
}

mu_run_test_suite(test_suite);
