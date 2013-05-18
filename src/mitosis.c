#include <math.h>
#include <time.h>
#include <dstruct/darray.h>

#include "mitosis.h"


static int randnum(int l_bound, int u_bound)
{
        int divisor = RAND_MAX / (u_bound + 1);
        int retval;

        do {
                retval = rand() / divisor;
        } while (retval < l_bound || retval > u_bound);

        return retval;
}

struct mitosis *init_mitosis(int param, float goal, int max_pop, int max_gen)
{
        struct mitosis *m = malloc(sizeof(struct mitosis));

        /* chromosomes */
        m->chromosomes = darray_create(sizeof(char), max_pop);

        /* mitosis details */
        m->population = 0;
        m->generation = 0;
        m->max_population = max_pop;
        m->max_generation = max_gen;

        /* fitness details */
        m->parameters = param;
        m->goal = goal;

        return m;
}

void init_str_chromosomes(struct mitosis *m, char *(*mutator)(int))
{
        int i = 0;
        char *el;

        srand(time(NULL));

        for (i = 0; i < m->max_population; i++) {
                /* create new darray element and push */
                el = darray_new(m->chromosomes);
                strcpy(el, (*mutator)(m->parameters));
                darray_push(m->chromosomes, el);

                /* increment counter */
                m->population++;
                check(
                        m->population <= m->max_population,
                        "darray population should not exceed max popuation"
                );
        }

error:
        return;
}

char *str_mutator(int param)
{
        int i = 0;
        char *random_str = malloc(sizeof(char) * param);

        for (i = 0; i < param; i++) {
                random_str[i] = randnum(32, 128);
        }

        return random_str;
}
