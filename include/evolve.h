#ifndef _EVOLVE_H_
#define _EVOLVE_H_

#include "population.h"
#include "evolve_monitor.h"


/* FUNCTIONS */
int run_evolution(
        struct population **p,
        float (eval_func)(char *),
        int (*crossover_func)(void **chromo_1, void **chromo_2, int index),
        float crossover_prob,
        int pivot_index,
        void (*mutate_func)(char **),
        float mutate_prob,
        struct evolve_monitor *m
);

#endif
