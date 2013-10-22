#ifndef _GA_SELECTION_H_
#define _GA_SELECTION_H_

#include "evolve.h"

/* MACROS */
#define DEFAULT_SELECT NULL

/* FUNCTIONS */
void roulette_wheel_selection(struct population **p, int *select);
void tournament_selection(struct population **p, int *select);

#endif
