#ifndef _GP_SELECTION_H_
#define _GP_SELECTION_H_

#include "evolve.h"

/* MACROS */
#define DEFAULT_SELECT NULL

/* FUNCTIONS */
void gp_roulette_wheel_selection(struct population *p, int *select);
void gp_tournament_selection(struct population *p, int *select);

#endif
