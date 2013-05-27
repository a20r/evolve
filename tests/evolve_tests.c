#include <time.h>
#include <math.h>
#include <string.h>

#include <munit/munit.h>

#include "evolve.h"

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

char *test_randnum_i()
{
        int i = 0;
        int curr_num = 0;  /* current number */
        int last_num = 0;  /* last number */

        for (i = 0; i < 10; i++) {
                curr_num = randnum_i(0, 100);

                debug("Run: %d", i);
                debug("last_num: [%d]", last_num);
                debug("curr_num: [%d]", curr_num);
                debug("");

                mu_assert(curr_num <= 100, "curr_num should not be over 100!");
                mu_assert(curr_num >= 0, "curr_num should be over 0!");
                mu_assert(curr_num != last_num, "curr_num == last_num!");
                last_num = curr_num;
        }

        return NULL;
}

char *test_randstr()
{
        int i = 0;
        char *curr_str = "";  /* current string */
        char *last_str = "";  /* last string */

        for (i = 0; i < 10; i++) {
                curr_str = randstr(10);

                debug("Run: %d", i);
                debug("last_str: [%s]", last_str);
                debug("curr_str: [%s]", curr_str);
                debug("");

                mu_assert(strlen(curr_str) == 10, "strlen should not be 10!");
                mu_assert(
                        strcmp(last_str, curr_str) != 0,
                        "curr_str != last_str failed"
                );

                last_str = curr_str;
        }

        return NULL;
}

char *test_init_population()
{
        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                5,  /* max_pop */
                1 /* max_gen */
        );

        /* chromosomes */
        mu_assert(p->chromosomes != NULL, "chromosomes should not be NULL!");
        mu_assert(p->chromosome_scores != NULL, "scores should not be NULL!");

        /* fitness details */
        mu_assert(p->parameters == 12, "parameters should be 12!");
        mu_assert(p->goal == 0.0, "goal should be 0.0!");

        /* evolution details */
        mu_assert(p->curr_population == 0, "current population should be 0!");
        mu_assert(p->curr_generation == 0, "current generation should be 0!");
        mu_assert(p->max_population == 5, "max population should be 5!");
        mu_assert(p->max_generation == 1, "max generation should be 1!");

        return NULL;
}

char *test_clean_population()
{
        clean_population(&p);

        mu_assert(p == NULL, "struct population should be NULL!");

        return NULL;
}

char *test_gen_init_chromosomes()
{
        int i = 0;
        char *curr_chromosome;
        char *last_chromosome;

        p = init_population(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                5,  /* max_pop */
                1 /* max_gen */
        );
        gen_init_chromosomes(&p, randstr);

        for (i = 0; i < p->max_population; i++) {
                curr_chromosome = darray_get(p->chromosomes, i);

                debug("Run: %d", i);
                debug("last_chromosome: [%s]", last_chromosome);
                debug("curr_chromosome: [%s]", curr_chromosome);
                debug("");

                mu_assert(
                        curr_chromosome != last_chromosome,
                        "curr_num == last_num!"
                );
                mu_assert(p->curr_population == 5,"population != 5!");

                last_chromosome = curr_chromosome;
        }

        return NULL;
}

char *test_evaluate_chromosomes()
{
        int i = 0;
        char *curr_chromosome;
        char *last_chromosome;
        float curr_score = 0.0;
        float last_score = 0.0;

        evaluate_chromosomes(fitness_function, &p);

        for (i = 0; i < p->max_population; i++) {
                curr_chromosome = (char *) darray_get(p->chromosomes, i);
                curr_score = *(float *) darray_get(p->chromosome_scores, i);

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
                mu_assert(p->curr_population == 5, "population should be 5!");

                debug("chromosome [%s]", curr_chromosome);
                debug("chromosome score: %f", curr_score);
                debug("");

                last_chromosome = curr_chromosome;
                last_score = curr_score;
        }

        return NULL;
}

char *test_run_evolution()
{

        return NULL;
}

void test_suite()
{
        mu_run_test(test_randnum_i);
        mu_run_test(test_randstr);
        mu_run_test(test_init_population);
        mu_run_test(test_clean_population);
        mu_run_test(test_gen_init_chromosomes);
        mu_run_test(test_evaluate_chromosomes);
        /* mu_run_test(test_run_evolution); */
}

int main()
{
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        test_suite();
        mu_report();
        return 0;
}
