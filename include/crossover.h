#ifndef _CROSSOVER_H_
#define _CROSSOVER_H_

#include "evolve.h"

/* MACROS */
#define DEFAULT_PIVOT -1

/* FUNCTIONS */
int one_ptr_crossover(struct chromosome_pair **c_pair, int pivot_index);

#endif
