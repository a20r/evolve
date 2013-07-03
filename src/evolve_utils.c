#include "evolve_utils.h"


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

