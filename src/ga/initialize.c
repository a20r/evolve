#include <dstruct/darray.h>

#include "ga/initialize.h"
#include "population.h"


void initialize_population(struct population *p, char *(*mutator)(int))
{
        int i = 0;
        int chromo_length = p->individuals->element_size;

        /* fill initial random individual */
        for (i = 0; i < p->max_population; i++) {
                darray_set(p->individuals, i, (*mutator)(chromo_length));
                p->population++;
        }
}
