#include <stdlib.h>

#include "utils.h"
#include "config.h"
#include "population.h"
#include "selection.h"


struct population *tournament_selection(
    struct population *p,
    struct config *config
)
{
    int i;
    int j;
    void *contender = NULL;
    void *best = NULL;
    struct population *parents;

    /* setup new population for parents */
    parents = population_new(p->size, p->individual_size);

    /* select parents */
    for (j = 0; j < p->size; j++) {
        best = p->individuals[randi(0, p->size - 1)];

        for (i = 0; i < config->selection->tournament_size; i++) {
            contender = p->individuals[randi(0, p->size - 1)];

            if (config->cmp(contender, best) == -1) {
                best = contender;
            }
        }

        parents->individuals[j] = config->copy_func(best);
    }

    return parents;
}
