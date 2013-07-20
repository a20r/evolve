#include "evolve_monitor.h"


struct evolve_monitor *init_evolve_monitor(size_t chromo_sz, long max_gen)
{
        struct evolve_monitor *m = malloc(sizeof(struct evolve_monitor));

        m->best_chromosomes = darray_create(chromo_sz, max_gen);
        m->best_scores = darray_create(sizeof(float), max_gen);
        m->generations = darray_create(sizeof(long), max_gen);

        return m;
}

void destroy_evolve_monitor(struct evolve_monitor **m)
{
        darray_clear_destroy((*m)->best_chromosomes);
        darray_clear_destroy((*m)->best_scores);
        darray_clear_destroy((*m)->generations);
        free(*m);

        *m = NULL;
}

char *find_best_chromosome(struct population *p)
{
        int i = 0;
        float goal = p->goal;
        char *chromo = '\0';
        float score = 0.0;
        char *best_chromo = '\0';
        float best_score = 0.0;

        /* instanciate inital best chromosome */
        best_chromo = (char *) darray_get(p->chromosomes, 0);
        best_score = *(float *) darray_get(p->chromosome_scores, 0);

        /* find the best chromosome */
        for (i = 1; i < p->curr_population; i++) {
                chromo = (char *) darray_get(p->chromosomes, i);
                score = *(float *) darray_get(p->chromosome_scores, i);

                if ((score - goal) <= (best_score - goal)) {
                        best_chromo = chromo;
                        best_score = score;
                }
        }

        printf("\nscore: %f\n", best_score);
        return best_chromo;
}

void record_generation_stats(struct population *p, struct evolve_monitor **m)
{
        int i = 0;
        float goal = p->goal;
        int generation = p->curr_generation;
        size_t chromo_sz = p->chromosomes->element_size;
        size_t score_sz = p->chromosome_scores->element_size;

        char *chromo = calloc(1, chromo_sz);
        float *score = calloc(1, score_sz);

        char *best_chromo = calloc(1, chromo_sz);
        float *best_score = calloc(1, score_sz);

        /* instanciate inital best chromosome */
        memcpy(best_chromo, darray_get(p->chromosomes, 0), chromo_sz);
        memcpy(best_score, darray_get(p->chromosome_scores, 0), score_sz);

        /* find the best chromosome */
        for (i = 1; i < p->curr_population; i++) {
                memcpy(chromo, darray_get(p->chromosomes, i), chromo_sz);
                memcpy(score, darray_get(p->chromosome_scores, i), score_sz);

                if ((*score - goal) <= (*best_score - goal)) {
                        memcpy(best_chromo, chromo, chromo_sz);
                        memcpy(best_score, score, score_sz);
                }
        }

        /* record generation statistics into evolve_monitor */
        int *gen = darray_new((*m)->generations);
        *gen = generation;

        darray_set((*m)->best_chromosomes, generation, best_chromo);
        darray_set((*m)->best_scores, generation, best_score);
        darray_set((*m)->generations, generation, gen);

        /* cleanup */
        free(chromo);
        free(score);
}

void sort_generation_stats(
        struct evolve_monitor **m,
        int (*cmp)(const void *, const void *)
)
{
        int j;
        int elements = (*m)->generations->end;

        size_t score_sz = (*m)->best_scores->element_size;
        size_t chromo_sz = (*m)->best_chromosomes->element_size;
        size_t gen_sz = (*m)->generations->element_size;

        void *score;
        void *chromo;
        void *gen;

        /* below implements an insertion sort - sort by ASCENDING ORDER */
        for (j = 1; j <= elements; j++) {
                int i = j - 1;

                /* obtain chromosome and score */
                chromo = darray_new((*m)->best_chromosomes);
                score = darray_new((*m)->best_scores);
                gen = darray_new((*m)->generations);

                memcpy(chromo, darray_get((*m)->best_chromosomes, j), chromo_sz);
                memcpy(score, darray_get((*m)->best_scores, j), score_sz);
                memcpy(gen, darray_get((*m)->generations, j), gen_sz);

                /* very important! */
                free(darray_get((*m)->best_chromosomes, j));
                free(darray_get((*m)->best_scores, j));
                free(darray_get((*m)->generations, j));

                while (
                        i >= 0 &&
                        cmp(
                                darray_get((*m)->best_scores, i),
                                score
                        ) > 0
                ) {
                        /* chromosome */
                        darray_set(
                                (*m)->best_chromosomes,
                                (i + 1),
                                darray_get((*m)->best_chromosomes, i)
                        );

                        /* score */
                        darray_set(
                                (*m)->best_scores,
                                (i + 1),
                                darray_get((*m)->best_scores, i)
                        );

                        /* generation */
                        darray_set(
                                (*m)->generations,
                                (i + 1),
                                darray_get((*m)->generations, i)
                        );

                        i--;
                }

                darray_set((*m)->best_chromosomes, (i + 1), chromo);
                darray_set((*m)->best_scores, (i + 1), score);
                darray_set((*m)->generations, (i + 1), gen);
        }
}

void print_generation_stats(struct evolve_monitor *m)
{
        int i = 0;
        int elements = m->generations->end + 1;
        struct darray *c = m->best_chromosomes;
        struct darray *s = m->best_scores;
        struct darray *g = m->generations;

        for (i = 0; i < elements; i++) {
                printf("GENERATION: %d\n", *(int *) darray_get(g, i));
                printf("chromosome: [%s]\n", (char *) darray_get(c, i));
                printf("score: %f\n\n", *(float *) darray_get(s, i));
        }
}
