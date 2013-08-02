#ifndef _EVOLVE_H_
#define _EVOLVE_H_

#include "population.h"
#include "evolve_monitor.h"


/* FUNCTIONS */
int run_evolution(
        struct population **p,
        float (eval_func)(char *),
        float crossover_prob,
        float mutate_prob,
        struct evolve_monitor *m
);

#endif
