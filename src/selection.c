#include <stdlib.h>

#include "utils.h"
#include "config.h"
#include "population.h"
#include "selection.h"



struct selection_config *selection_config_new(int method)
{
    struct selection_config *sc = malloc(sizeof(struct selection_config));

    sc->method = method;
    sc->tournament_size = -1;

    return sc;
}

void selection_config_destroy(void *config)
{
    if (config) {
        free(config);
    }
}

struct population *tournament_selection(
    struct population *p,
    struct config *c
)
{
    int i;
    int j;
    void *contender = NULL;
    void *best = NULL;
    struct population *selected;
    struct selection_config *sc = c->selection;

    /* pre-check */
    check(c->cmp, ECMP);
    check(c->copy_func, ECOPYFUNC);
    check(c->get_score, EGETSCORE);
    check(sc->tournament_size > 0, ETSIZE);

    /* setup new population */
    selected = population_new(p->size, p->individual_size);

    /* select selected */
    for (j = 0; j < p->size; j++) {
        best = p->individuals[randi(0, p->size - 1)];

        for (i = 0; i < sc->tournament_size; i++) {
            contender = p->individuals[randi(0, p->size - 1)];

            if (c->get_score(contender) == NULL)  {
                i--;
            } else {
                if (c->cmp(contender, best) == -1) {
                    best = contender;
                }
            }
        }

        selected->individuals[j] = c->copy_func(best);
    }

    return selected;
error:
    return NULL;
}
