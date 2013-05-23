#include <math.h>
#include <time.h>
#include <dstruct/darray.h>
#include <c-dbg/dbg.h>

#include "evolve.h"


static int randnum_i(int l_bound, int u_bound)
{
        int divisor = RAND_MAX / (u_bound + 1);
        int retval;

        do {
                retval = rand() / divisor;
        } while (retval < l_bound || retval > u_bound);

        return retval;
}

struct evolve *init_evolve(
        int param,
        float goal,
        int max_pop,
        int max_gen)
{
        struct evolve *e = malloc(sizeof(struct evolve));

        /* chromosomes */
        e->chromosomes = darray_create(sizeof(char) * (param + 1), max_pop);

        /* evolve details */
        e->population = 0;
        e->generation = 0;
        e->max_population = max_pop;
        e->max_generation = max_gen;

        /* fitness details */
        e->parameters = param;
        e->goal = goal;

        return e;
}

void init_str_chromosomes(struct evolve **e, char *(*mutator)(int))
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

char *rand_str(int length)
{
        int i = 0;
        int el_size = sizeof(char) * (length + 1);
        char *rand_str = malloc(el_size);
        memset(rand_str, '\0', el_size);

        /* generate random ASCII char */
        for (i = 0; i < length; i++) {
                rand_str[i] = randnum_i(32, 128);
        }

        return rand_str;
}

void evaluate_chromosomes(struct evolve *e)
{
        int i = 0;

        for (i = 0; i < e->max_population; i++) {

        }

}

struct chromosome_pair *str_mate(char *c_1, char *c_2)
{
        struct chromosome_pair *c_pair = malloc(sizeof(struct chromosome_pair));
        int pivot = round((int) strlen(c_1) / 2) - 1;
        int str_len = (int) strlen(c_1) + 1;

        printf("pivot index: %d\n", pivot);
        printf("str_len: %d\n", str_len);

        check(
                strlen(c_1) == strlen(c_2),
                "chromosome 1 and 2 are not same length"
        );

        /* allocate memory for gene pair */
        c_pair->child_1 = calloc((str_len + 1), sizeof(char));
        c_pair->child_2 = calloc((str_len + 1), sizeof(char));

        /* pivot at the middle and swap chromosomes */
        strncpy(c_pair->child_1, c_1, pivot);
        strncpy(c_pair->child_1 + pivot, c_2 + pivot, str_len - pivot);
        strncpy(c_pair->child_2 , c_2, pivot);
        strncpy(c_pair->child_2 + pivot, c_1 + pivot, str_len - pivot);

        return c_pair;
error:
        free(c_pair);
        return NULL;
}

void str_mutator(char **chromosome)
{
        /* decide which char to mutate and by moving char val up or down */
        int str_len = strlen((char *) *chromosome);
        int index = randnum_i(0, str_len);
        int up_or_down = randnum_i(0, 100) <= 50 ? -1 : 1;

        (*chromosome)[index] = (*chromosome)[index] + up_or_down;
}

void mutate_gene_str(
        struct chromosome_pair **c_pair,
        float mutate_chance,
        void (mutator)(char **))
{
        /* to mutate or not mutate */
        if (randnum_i(0, 100) > mutate_chance) {
                goto mutate;
        } else {
                goto no_mutate;
        }

mutate:
        (*mutator)((char **) &(*c_pair)->child_1);
no_mutate:
        return;
}
