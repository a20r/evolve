#ifndef _GA_CROSSOVER_H_
#define _GA_CROSSOVER_H_

#include "evolve.h"

/* MACROS */
#define DEFAULT_PIVOT -1

/* FUNCTIONS */
int one_pt_crossover(void **chromo_1, void **chromo_2, int index);
int two_pt_crossover(void **chromo_1, void **chromo_2, int index);
void crossover(
        void **child_1,
        void **child_2,
        int index,
        int (*crossover_func)(void **chromo_1, void **chromo_2, int index),
        float *crossover_prob
);

#endif
