#ifndef __SELECTION__
#define __SELECTION__

#define TOURNAMENT_SELECTION 0

#include "config.h"

/* FUNCTIONS */
struct population *tournament_selection(
    struct population *p,
    struct selection_config *sc
);

#endif
