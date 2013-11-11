#include <stdlib.h>
#include <math.h>

#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/darray.h>

#include "evolve_monitor.h"


struct evolve_monitor *init_evolve_monitor(
        size_t individual_sz,
        long top,
        char *log_fp
)
{
        struct evolve_monitor *m = malloc(sizeof(struct evolve_monitor));

        m->top = top;
        m->best_individuals = darray_create(individual_sz, top);
        m->best_scores = darray_create(sizeof(float), top);
        m->generations = darray_create(sizeof(long), top);
        m->convergence_rates = darray_create(sizeof(float), top);
        m->goal_distances = darray_create(sizeof(float), top);

        if (log_fp != NULL) {
                m->log_stats = 1;
                m->log_fp = fopen(log_fp, "w");
                if (m->log_fp == NULL) {
                        printf("Failed to open [%s] for logging!\n", log_fp);
                        exit(-1);
                }
        } else {
                m->log_stats = 0;
                m->log_fp = NULL;
        }

        return m;
}

void destroy_evolve_monitor(struct evolve_monitor **m)
{
        darray_clear_destroy((*m)->best_individuals);
        darray_clear_destroy((*m)->best_scores);
        darray_clear_destroy((*m)->generations);
        darray_clear_destroy((*m)->convergence_rates);
        darray_clear_destroy((*m)->goal_distances);
        if ((*m)->log_fp != NULL) fclose((*m)->log_fp);
        free(*m);

        *m = NULL;
}

static void log_generation_stats(
        FILE *log_fp,
        int generation,
        char *individual,
        float score,
        float convergence_rate,
        float goal_distance
)
{
        fprintf(log_fp, "%s\n", individual);
        fprintf(log_fp, "%f\n", score);
        fprintf(log_fp, "%d\n", generation);
        fprintf(log_fp, "%f\n", convergence_rate);
        fprintf(log_fp, "%f\n\n", goal_distance);
}

static int fill_leader_board(
        struct population *p,
        struct evolve_monitor *m
)
{
        int i = 0;
        char *individual;
        float *score;
        float *generation;
        float *convergence_rate;
        float *goal_distance;

        size_t individual_sz = p->individuals->element_size;
        size_t score_sz = p->scores->element_size;

        check(
                p->generation == 0,
                "Error! fill_leader_board() should only be in generation 0!"
        );

        /* find the best initial individual and fill up the leader board */
        for (i = 0; i < m->top; i++) {
                /* allocate memory */
                individual = calloc(1, individual_sz + sizeof(char));
                score = darray_new(p->scores);
                generation = calloc(1, sizeof(float));
                convergence_rate = calloc(1, sizeof(float));
                goal_distance = calloc(1, sizeof(float));

                memcpy(individual, darray_get(p->individuals, i), individual_sz);
                memcpy(score, darray_get(p->scores, i), score_sz);
                *generation = 0;
                *convergence_rate = fabs(p->goal - *score);
                *goal_distance = fabs(p->goal - *score);

                darray_set(m->best_individuals, i, individual);
                darray_set(m->best_scores, i, score);
                darray_set(m->generations, i, generation);
                darray_set(m->convergence_rates, i, convergence_rate);
                darray_set(m->goal_distances, i, goal_distance);
        }

        return 0;
error:
        return -1;
}

void sort_generation_stats(
        struct evolve_monitor *m,
        int (*cmp)(const void *, const void *)
)
{
        int j = 0;

        void *score;
        void *individual;
        void *gen;
        void *conv_rate;
        void *goal_dist;

        /* below implements an insertion sort - sort by ASCENDING ORDER */
        for (j = 1; j <= m->top - 1; j++) {
                int i = j - 1;

                /* obtain pointers pointing to individual and score */
                individual = darray_get(m->best_individuals, j);
                score = darray_get(m->best_scores, j);
                gen = darray_get(m->generations, j);
                conv_rate = darray_get(m->convergence_rates, j);
                goal_dist = darray_get(m->goal_distances, j);

                while (
                        i >= 0 &&
                        cmp(
                                darray_get(m->best_scores, i),
                                score
                        ) > 0
                ) {
                        /* individual */
                        darray_set(
                                m->best_individuals,
                                i + 1,
                                darray_get(m->best_individuals, i)
                        );

                        /* score */
                        darray_set(
                                m->best_scores,
                                i + 1,
                                darray_get(m->best_scores, i)
                        );

                        /* generation */
                        darray_set(
                                m->generations,
                                i + 1,
                                darray_get(m->generations, i)
                        );

                        /* convergence rate */
                        darray_set(
                                m->convergence_rates,
                                i + 1,
                                darray_get(m->convergence_rates, i)
                        );

                        /* goal distance */
                        darray_set(
                                m->goal_distances,
                                i + 1,
                                darray_get(m->goal_distances, i)
                        );

                        i--;
                }

                darray_set(m->best_individuals, i + 1, individual);
                darray_set(m->best_scores, i + 1, score);
                darray_set(m->generations, i + 1, gen);
                darray_set(m->convergence_rates, i + 1, conv_rate);
                darray_set(m->goal_distances, i + 1, goal_dist);
        }
}

static int find_best_individual(
        struct population *p,
        int (*cmp)(const void *, const void *)
)
{
        int i = 0;
        size_t score_sz = p->scores->element_size;
        float *score = darray_new(p->scores);
        float *best_score = darray_new(p->scores);
        int best_score_index = 0;

        /* instanciate inital best individual */
        memcpy(score, darray_get(p->scores, 0), score_sz);

        /* find the best individual */
        for (i = 1; i < p->population; i++) {
                memcpy(score, darray_get(p->scores, i), score_sz);

                /* set value 1 and value 2 for comparison */
                float *val_1 = calloc(1, sizeof(float));
                float *val_2 = calloc(1, sizeof(float));
                *val_1 = *score - p->goal;
                *val_2 = *best_score - p->goal;

                /* compare */
                if (cmp(val_1, val_2) < 1) {
                        best_score_index = i;
                        memcpy(best_score, score, score_sz);
                }

                free(val_1);
                free(val_2);
        }

        /* cleanup */
        free(best_score);
        free(score);

        return best_score_index;
}

static int update_leaderboard(
        struct evolve_monitor *m,
        void *best_individual,
        void *best_score,
        void *gen,
        void *conv_rate,
        void *goal_dist,
        int (*cmp)(const void *, const void *)
)
{
        int i = 0;
        int leaderboard_updated = 0;

        for (i = 0; i < m->top; i++) {
                if (cmp(darray_get(m->best_scores, i), best_score) > 0) {
                        darray_update(m->best_individuals, i, best_individual);
                        darray_update(m->best_scores, i, best_score);
                        darray_update(m->generations, i, gen);
                        darray_update(m->convergence_rates, i, conv_rate);
                        darray_update(m->goal_distances, i, goal_dist);
                        leaderboard_updated = 1;
                        break;
                }
        }

        return leaderboard_updated;
}

void record_generation_stats(
        struct population *p,
        struct evolve_monitor *m,
        int (*cmp)(const void *, const void *)
)
{
        /* find the best individual */
        int best_index = find_best_individual(p, cmp);
        size_t individual_sz = p->individuals->element_size;
        size_t score_sz = p->scores->element_size;

        /* leaderboard variables */
        char *best_individual = calloc(1, individual_sz + sizeof(char));
        float *best_score = darray_new(p->scores);
        int *gen = darray_new(m->generations);
        float *conv_rate = darray_new(m->convergence_rates);
        float *goal_dist = darray_new(m->goal_distances);

        memcpy(best_individual, darray_get(p->individuals, best_index), individual_sz);
        memcpy(best_score, darray_get(p->scores, best_index), score_sz);

        /* calculate convergence rate and goal distance */
        *conv_rate = m->convergence_rate;
        *gen = p->generation;
        if (p->generation != 0) {
                *conv_rate = fabs(m->score - *best_score);
        } else {
                fill_leader_board(p, m);
                sort_generation_stats(m, cmp);
                *conv_rate = 0;
        }
        *goal_dist = fabs(p->goal - *best_score);

        /* log generation stats to file */
        if (m->log_stats) {
                log_generation_stats(
                        m->log_fp,
                        p->generation,
                        best_individual,
                        *best_score,
                        *conv_rate,
                        *goal_dist
                );
        }

        /* over-write prev gen details with current */
        m->generation = p->generation;
        m->individual = best_individual;
        m->score = *best_score;
        m->convergence_rate = *conv_rate;
        m->goal_distance = *goal_dist;

        /* printf("\nGEN: %d\n", *gen); */
        /* printf("BEST CHROMO: %s\n", best_individual); */
        /* printf("BEST SCORE: %f\n", *(float *) best_score); */

        int leaderboard_updated = update_leaderboard(
                m,
                best_individual,
                best_score,
                gen,
                conv_rate,
                goal_dist,
                cmp
        );

        /* cleanup */
        if (leaderboard_updated == 0) {
                free(best_individual);
                free(best_score);
                free(gen);
                free(conv_rate);
                free(goal_dist);
        }
}

void print_generation_stats(struct evolve_monitor *m)
{
        int i = 0;
        int elements = m->generations->end;
        struct darray *c = m->best_individuals;
        struct darray *s = m->best_scores;
        struct darray *g = m->generations;
        struct darray *conv_rates = m->convergence_rates;
        struct darray *goal_dists = m->goal_distances;

        for (i = 0; i < elements; i++) {
                printf("GENERATION: %d\n", *(int *) darray_get(g, i));
                printf("individual: [%s]\n", (char *) darray_get(c, i));
                printf("score: %.2f\n", *(float *) darray_get(s, i));
                printf(
                        "convergence rate: %.2f\n",
                        *(float *) darray_get(conv_rates, i)
                );
                printf(
                        "goal distance: %.2f\n\n",
                        *(float *) darray_get(goal_dists, i)
                );
        }
}
