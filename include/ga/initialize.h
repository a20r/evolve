#ifndef _GA_INITIALIZE_H_
#define _GA_INITIALIZE_H_

#include "population.h"

/* FUNCTIONS */
void initialize_population(struct population *p, char *(*mutator)(int));

#endif
