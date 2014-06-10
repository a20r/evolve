#include <stdlib.h>

#include "cmp.h"
#include "config.h"
#include "random.h"
#include "population.h"
#include "selection.h"


struct population *tournament_selection(
    struct population *p,
    struct selection_config *sc
)
{
    int i;
    int j;
    int select = p->size / 2;
    void *contender = NULL;
    void *best = NULL;
    struct population *parents;

    /* setup new population for parents */
    select += (select % 2);
    parents = population_new(select, p->individual_size);

    /* select parents */
    for (j = 0; j < select; j++) {
        best = p->individuals[randi(0, p->size - 1)];

        for (i = 0; i < sc->tournament_size; i++) {
            contender = p->individuals[randi(0, p->size - 1)];

            if (sc->cmp(contender, best) == -1) {
                best = contender;
            }
        }

        parents->individuals[j] = sc->copy_func(best);
    }

    return parents;
}
