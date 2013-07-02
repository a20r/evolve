#include <dbg/dbg.h>

#include "evolve.h"

void print_chromosome(struct population *p, int index)
{
        char *chromo = (char *) darray_get(p->chromosomes, index);
        float score = *(float *) darray_get(p->chromosome_scores, index);
        float total_score = (double) p->total_score;

        printf("chromosome [%s]\n", chromo);
        printf("chromosome score: %f\n", score);
        printf("chromosome total_score: %f\n\n", total_score);
}

void print_chromosomes(struct population *p)
{
        int i = 0;
        float score = 0.0;
        char *chromo = '\0';

        for (i = 0; i < p->curr_population; i++) {
                chromo = (char *) darray_get(p->chromosomes, i);
                score = *(float *) darray_get(p->chromosome_scores, i);

                printf("chromosome [%s]\n", chromo);
                printf( "chromosome score: [%f]\n\n", score);
        }
}

void print_population(struct population *p)
{
        printf("population[paramters]: %d\n", p->parameters);
        printf("population[goal]: %f\n", p->goal);
        printf("population[curr_population]: %d\n", p->curr_population);
        printf("population[max_population]: %d\n", p->max_population);
        printf("population[curr_generation]: %d\n", p->curr_generation);
        printf("population[max_generation]: %d\n\n", p->max_generation);

        print_chromosomes(p);
}

char *obtain_best_chromosome(struct population *p)
{
        int i = 0;
        float goal = p->goal;
        float curr_score = 0;
        float best_score = *(float *) darray_get(p->chromosome_scores, 0);
        char *curr_chromo = calloc(1, sizeof(p->chromosomes->element_size));
        char *best_chromo = calloc(1, sizeof(p->chromosomes->element_size));

        memcpy(
                best_chromo,
                darray_get(p->chromosomes, 0),
                p->chromosomes->element_size
        );

        for (i = 1; i < p->curr_population; i++) {
                curr_chromo = (char *) darray_get(p->chromosomes, i);
                curr_score = *(float *) darray_get(p->chromosome_scores, i);

                if ((curr_score - goal) <= (best_score - goal)) {
                        best_score = curr_score;
                        best_chromo = curr_chromo;
                }
        }

        /* printf("chromosome [%s]\n", best_chromo); */
        /* printf("chromosome score: [%f]\n\n", best_score); */

        return best_chromo;
}

