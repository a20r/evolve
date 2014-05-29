#ifndef __SELECTION__
#define __SELECTION__

#define TOURNAMENT_SELECTION 0


/* FUNCTIONS */
struct population *tournament_selection(
    struct population *p,
    int select,
    int tournament_size,
    float (*get_score)(void *),
    int (*free_individual)(void *)
);

#endif
