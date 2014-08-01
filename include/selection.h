#ifndef _SELECTION_H_
#define _SELECTION_H_

#define TOURNAMENT_SELECTION 0

/* ERRORS */
#define ETSIZE "c->selection->tournament_size not set!"

#include "config.h"

/* FUNCTIONS */
struct population *tournament_selection(
    struct population *p,
    struct config *c
);

#endif
