#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>
#include <al/comparator.h>

#include "evolve.h"
#include "utils.h"

/* GLOBAL VAR */
struct population *p;


float fitness_function(char *chromosome)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += fabsf(roundf(target[i] - chromosome[i]));
        }

        return total;
}


int test_init_population()
{
        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                5,  /* max_pop */
                1 /* max_gen */
        );

        /* chromosomes */
        mu_assert(p->chromosomes != NULL, "Chromosomes should not be NULL!");
        mu_assert(p->chromosome_scores != NULL, "Scores should not be NULL!");
        mu_assert(p->total_score == 0.0, "Sum of scores should be 0.0!");

        /* fitness details */
        mu_assert(p->parameters == 12, "Parameters should be 12!");
        mu_assert(p->goal == 0.0, "Goal should be 0.0!");

        /* population details */
        mu_assert(p->curr_population == 0, "Current population should be 0!");
        mu_assert(p->curr_generation == 0, "Current generation should be 0!");
        mu_assert(p->max_population == 5, "Max population should be 5!");
        mu_assert(p->max_generation == 1, "Max generation should be 1!");
        mu_assert(p->solution == NULL, "Solution should be NULL!");

        return 0;
}

int test_gen_init_chromosomes()
{
        int i = 0;
        char *curr_chromosome = '\0';
        char *last_chromosome = '\0';

        gen_init_chromosomes(&p, randstr);

        for (i = 0; i < p->max_population; i++) {
                curr_chromosome = darray_get(p->chromosomes, i);

                mu_assert(
                        curr_chromosome != last_chromosome,
                        "curr_num == last_num!"
                );
                mu_assert(p->curr_population == 5,"population != 5!");

                last_chromosome = curr_chromosome;
        }

        return 0;
}

int test_evaluate_chromosomes()
{
        int i = 0;
        char *curr_chromosome = '\0';
        char *last_chromosome = '\0';
        float curr_score = 0.0;
        float last_score = 0.0;

        /* set the last chromosome to match the solution, then evaluate */
        darray_set(p->chromosomes, 4, "hello world!");
        int goal_reached = evaluate_chromosomes(fitness_function, &p);
        mu_assert(goal_reached == 1, "goal should have been reached!");

        /* loop through chromosomes and their scores to check the are ok */
        for (i = 0; i < p->max_population; i++) {
                curr_chromosome = (char *) darray_get(p->chromosomes, i);
                curr_score = *((float *) darray_get(p->chromosome_scores, i));

                /* chromosome */
                mu_assert(
                        curr_chromosome != last_chromosome,
                        "curr_num == last_num!"
                );

                /* score */
                mu_assert(
                        curr_score != last_score,
                        "curr_score shoud not equal last_score!"
                );

                /* other */
                mu_assert(p->curr_population == 5, "Population should be 5!");

                debug("chromosome [%s]", curr_chromosome);
                debug("chromosome score: %f", curr_score);
                debug("");

                last_chromosome = curr_chromosome;
                last_score = curr_score;
        }
        mu_assert(p->total_score != 0.0, "Sum of scores should not be 0!");

        return 0;
}

int test_normalize_fitness_values()
{
        int i = 0;
        float sum = 0;

        normalize_fitness_values(&p);

        for (i = 0; i < p->max_population; i++) {
                sum += *(float *) darray_get(p->chromosome_scores, i);
        }

        /* debug("sum: %f", sum); */
        /* mu_assert(sum == 1.0, "Sum of normalized values should be 1!"); */

        return 0;
}

int test_sort_population()
{
        int i = 0;
        float curr_score = 0;
        float prev_score = 0;

        debug("Before Population Sort");
        print_chromosomes(p);

        /* sort population */
        sort_population(&p, float_cmp);
        prev_score = *(float *) darray_get(p->chromosome_scores, 0);

        debug("After Population Sort");
        print_chromosomes(p);

        /* assert tests */
        for (i = 1; i < p->max_population; i++) {
                curr_score = *(float *) darray_get(p->chromosome_scores, i);
                mu_assert(curr_score <= prev_score, "Failed to sort scores!");
                prev_score = curr_score;
        }

        return 0;
}

int test_destroy_population()
{
        destroy_population(&p);
        mu_assert(p == NULL, "Struct population should be NULL!");

        return 0;
}

int test_run_evolution()
{
        struct population *pop = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                5,  /* max_pop */
                1 /* max_gen */
        );

        gen_init_chromosomes(&pop, randstr);
        run_evolution(&pop, fitness_function);

        return 0;
}

void test_suite()
{
        mu_run_test(test_init_population);
        mu_run_test(test_gen_init_chromosomes);
        mu_run_test(test_evaluate_chromosomes);
        mu_run_test(test_normalize_fitness_values);
        mu_run_test(test_sort_population);
        mu_run_test(test_destroy_population);
        mu_run_test(test_run_evolution);
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));
        test_suite();
        mu_report();
}
