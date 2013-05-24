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

struct evolve *init_evolve(
        int param,
        float goal,
        int max_pop,
        int max_gen)
{
        struct evolve *e = malloc(sizeof(struct evolve));

        log_info("initializing struct evolve");

        /* chromosomes */
        e->chromosomes = darray_create(sizeof(char) * (param + 1), max_pop);

        /* evolve details */
        e->max_population = max_pop;
        e->max_generation = max_gen;

        /* fitness details */
        e->parameters = param;
        e->goal = goal;

        debug("evolve[paramters]: %d", e->parameters);
        debug("evolve[goal]: %d", e->goal);
        debug("evolve[max_population]: %d", e->max_population);
        debug("evolve[max_generation]: %d", e->max_generation);

        return e;
}

void clean_evolve(struct evolve **e)
{
        log_info("cleaning struct evolve");
        darray_clear_destroy((*e)->chromosomes);
        free((*e));
}

void init_chromosomes(struct evolve **e, char *(*mutator)(int))
{
        int i = 0;
        int param = (*e)->parameters;
        int el_size = sizeof(char) * (param + 1);  /* +1 for terminal '\0' */
        char *chromosome;
        char *el;

        /* fill initial random chromosome */
        for (i = 0; i < (*e)->max_population; i++) {
                chromosome = (*mutator)(param);
                darray_set((*e)->chromosomes, i, chromosome);
        }

error:
        return;
}

void run_evolution(struct evolve **e, float (eval_func)(char *))
{

        /* evolution vars */
        int i = 0;
        int cur_gen = 0;
        int max_gen = (*e)->max_generation;
        int goal_achieved = 0;
        int str_len = (*e)->parameters;

        /* chromosome vars */
        struct chromosome_pair *c_pair = malloc(sizeof(struct chromosome_pair));
        char *chromosome;  /* current chromosome under testing */
        char *c_1 = calloc((str_len + 1), sizeof(char));  /* chromosome 1 */
        char *c_2 = calloc((str_len + 1), sizeof(char));  /* chromosome 2 */
        float c_1_score = 0.0;
        float c_2_score = 0.0;
        float score = 0.0;


        log_info("running evolution");
        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* init random chromosomes */
        init_chromosomes(&(*e), randstr);

        /* allocate memory for chromosome pair */
        c_pair->child_1 = calloc((str_len + 1), sizeof(char));
        c_pair->child_2 = calloc((str_len + 1), sizeof(char));

        /* evolve until max_gen reached or goal achieved  */
        while (cur_gen < max_gen && goal_achieved == 0)
        {
                debug("\nGENERATION: %d", cur_gen);
                for (i = 0; i < (*e)->max_population; i++) {
                        chromosome = darray_get((*e)->chromosomes, i);
                        score = (eval_func)(chromosome);

                        debug("\nchromosome: %s", chromosome);
                        debug("score : %f", score);

                        if (cur_gen == 0 && i == 0) {
                                c_1_score = score;
                                strcpy(c_1, chromosome);
                        } else if (cur_gen ==0 && i == 1) {
                                c_2_score = score;
                                strcpy(c_2, chromosome);
                        } else if (score < c_1_score) {
                                debug("chromosome score is better than c_1");
                                c_2_score = c_1_score;
                                c_1_score = score;
                                strcpy(c_2, c_1);
                                strcpy(c_1, chromosome);
                        } else if (score < c_2_score) {
                                debug("chromosome score is better than c_2");
                                c_2_score = score;
                                strcpy(c_2, chromosome);
                        }

                        debug("score: %f", score);
                        debug("c_1: %s", c_1);
                        debug("c_2: %s", c_2);
                        debug("c_1 score: %f", c_1_score);
                        debug("c_2 score: %f", c_2_score);
                }

                strcpy(c_pair->child_1, c_1);
                strcpy(c_pair->child_2, c_2);
                str_crossover(&c_pair);
                mutator(&c_pair, 50, mutate_str);

                memset(c_1, '\0', sizeof(char) * (str_len + 1));
                memset(c_2, '\0', sizeof(char) * (str_len + 1));
                cur_gen++;
        }

        /* clean up */
        free(c_1);
        free(c_2);

        free(c_pair->child_1);
        free(c_pair->child_2);
        free(c_pair);

        log_info("evolution completed!");
}

