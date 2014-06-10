#include <stdlib.h>

#include "random.h"
#include "population.h"
#include "selection.h"


struct population *tournament_selection(
    struct population *p,
    int select,
    int tournament_size,
    float (*get_score)(void *),
    int (*free_func)(void *)
)
{
    int i;
    int j;
    void *best = malloc(p->individual_size);
    void *contender = NULL;
    struct population *parents = population_new(select, p->individual_size);

    for (j = 0; j < select; j++) {
        /* initialize best */
        best = p->individuals[randi(0, p->size - 1)];

        for (i = 0; i < tournament_size; i++) {
            contender = p->individuals[randi(0, p->size - 1)];

            if (get_score(contender) < get_score(best)) {
                best = contender;
            }
        }

        parents->individuals[j] = best;
    }

    free_func(best);
    free_func(contender);
    return parents;
}
