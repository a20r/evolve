#ifndef _GA_INITIALIZE_H_
#define _GA_INITIALIZE_H_

#include "population.h"

/* FUNCTIONS */
void population_initialize(struct population *p, char *(*mutator)(int));

#endif
