#include <math.h>
#include <time.h>

#include <dstruct/darray.h>
#include <dbg/dbg.h>

#include "evolve.h"
#include "crossover.h"
#include "mutate.h"


int randnum_i(int l_bound, int u_bound)
{
        int divisor = RAND_MAX / (u_bound + 1);
        int retval;

        do {
                retval = rand() / divisor;
        } while (retval < l_bound || retval > u_bound);

        return retval;
}

char *randstr(int length)
{
        int i = 0;
        int el_size = sizeof(char) * (length + 1);
        char *randstr = malloc(el_size);

        check_mem(randstr);
        memset(randstr, '\0', el_size);

        /* generate random ASCII char */
        for (i = 0; i < length; i++) {
                randstr[i] = randnum_i(32, 128);
        }

        return randstr;
error:
        return NULL;
}

struct population *init_population(
        int param,
        float goal,
        int max_pop,
        int max_gen)
{
        struct population *p = malloc(sizeof(struct population));

        log_info("initializing struct population");

        /* chromosomes */
        p->chromosomes = darray_create(sizeof(char) * (param + 1), max_pop);
        p->chromosome_scores = darray_create(sizeof(float), max_pop);

        /* population details */
        p->curr_population = 0;
        p->curr_generation = 0;
        p->max_population = max_pop;
        p->max_generation = max_gen;

        /* fitness details */
        p->parameters = param;
        p->goal = goal;

        debug("population[paramters]: %d", p->parameters);
        debug("population[goal]: %f", p->goal);
        debug("population[max_population]: %d", p->max_population);
        debug("population[max_generation]: %d", p->max_generation);

        return p;
}

void clean_population(struct population **p)
{
        log_info("cleaning struct population");
        darray_clear_destroy((*p)->chromosomes);
        darray_clear_destroy((*p)->chromosome_scores);
        free((*p));
        (*p) = NULL;
}

void gen_init_chromosomes(struct population **p, char *(*mutator)(int))
{
        int i = 0;
        int param = (*p)->parameters;
        int el_size = sizeof(char) * (param + 1);  /* +1 for terminal '\0' */
        char *chromosome;
        char *pl;

        /* fill initial random chromosome */
        for (i = 0; i < (*p)->max_population; i++) {
                chromosome = (*mutator)(param);
                darray_set((*p)->chromosomes, i, chromosome);

                (*p)->curr_population++;
        }

error:
        return;
}

void evaluate_chromosomes(float (eval_func)(char *), struct population **p)
{
        int i = 0;
        float *score;
        float score_2;
        char *chromosome;

        log_info("evaluating chromosomes");
        for (i = 0; i < (*p)->max_population; i++) {
                chromosome = darray_get((*p)->chromosomes, i);
                score = malloc(sizeof(float));
                *score = (eval_func)(chromosome);
                darray_set((*p)->chromosome_scores, i, score);

                debug("chromosome [%s]", chromosome);
                debug("chromosome score: %f", *score);
                debug("chromosome score address: %p", score);
                debug("");
        }

}

/* void chromosome_selection(struct population **p, float (eval_func)(char *)) */
/* { */
/*         int i = 0; */
/*         int str_len = (*p)->parameters; */
/*         int curr_gen = (*p)->curr_generation; */
/*  */
/*         #<{(| chromosome vars |)}># */
/*         struct chromosome_pair *c_pair = malloc(sizeof(struct chromosome_pair)); */
/*         char *chromosome; */
/*         char *c_1 = calloc((str_len + 1), sizeof(char));  #<{(| chromosome 1 |)}># */
/*         char *c_2 = calloc((str_len + 1), sizeof(char));  #<{(| chromosome 2 |)}># */
/*         float c_1_score = 0.0; */
/*         float c_2_score = 0.0; */
/*  */
/*         #<{(| allocate memory for chromosome pair |)}># */
/*         c_pair->child_1 = calloc((str_len + 1), sizeof(char)); */
/*         c_pair->child_2 = calloc((str_len + 1), sizeof(char)); */
/*  */
/*         for (i = 0; i < (*p)->max_population; i++) { */
/*                 chromosome = darray_get((*p)->chromosomes, i); */
/*                 score = (eval_func)(chromosome); */
/*  */
/*                 debug("\nchromosome: %s", chromosome); */
/*                 debug("score : %f", score); */
/*  */
/*                 if (curr_gen == 0 && i == 0) { */
/*                         c_1_score = score; */
/*                         strcpy(c_1, chromosome); */
/*                 } else if (curr_gen ==0 && i == 1) { */
/*                         c_2_score = score; */
/*                         strcpy(c_2, chromosome); */
/*                 } else if (score < c_1_score) { */
/*                         debug("chromosome score is better than c_1"); */
/*                         c_2_score = c_1_score; */
/*                         c_1_score = score; */
/*                         strcpy(c_2, c_1); */
/*                         strcpy(c_1, chromosome); */
/*                 } else if (score < c_2_score) { */
/*                         debug("chromosome score is better than c_2"); */
/*                         c_2_score = score; */
/*                         strcpy(c_2, chromosome); */
/*                 } */
/*  */
/*                 debug("score: %f", score); */
/*                 debug("c_1: %s", c_1); */
/*                 debug("c_2: %s", c_2); */
/*                 debug("c_1 score: %f", c_1_score); */
/*                 debug("c_2 score: %f", c_2_score); */
/*  */
/*                 memset(c_1, '\0', sizeof(char) * (str_len + 1)); */
/*                 memset(c_2, '\0', sizeof(char) * (str_len + 1)); */
/*         } */
/*  */
/*         #<{(| clean up |)}># */
/*         free(c_1); */
/*         free(c_2); */
/*  */
/*         free(c_pair->child_1); */
/*         free(c_pair->child_2); */
/*         free(c_pair); */
/* } */

void run_evolution(struct population **p, float (eval_func)(char *))
{
        /* evolution vars */
        int max_gen = (*p)->max_generation;
        int goal_achieved = 0;

        log_info("running evolution");
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* init random chromosomes */
        gen_init_chromosomes(&(*p), randstr);

        /* population until max_gen reached or goal achieved  */
        while ((*p)->curr_generation < max_gen && goal_achieved == 0)
        {
                debug("GENERATION: %d", (*p)->curr_generation);
                evaluate_chromosomes(eval_func, &(*p));


                (*p)->curr_generation++;
        }

        /* clean up */
        clean_population(&(*p));
        log_info("evolution completed!");
}
