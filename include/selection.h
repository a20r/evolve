#ifndef _SELECTION_H_
#define _SELECTION_H_

#define TOURNAMENT_SELECTION 0

/* ERRORS */
#define ETSIZE "c->selection->tournament_size not set!"

#include "config.h"

/* STRUCTS */
struct selection_config
{
    int method;
    struct population *(*select_func)(
        struct population *,
        struct config *
    );

    int tournament_size;
};

/* FUNCTIONS */
struct selection_config *selection_config_new(int method);
void selection_config_destroy(void *config);

struct population *tournament_selection(
    struct population *p,
    struct config *c
);

#endif
