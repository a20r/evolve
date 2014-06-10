#ifndef __SELECTION__
#define __SELECTION__

#define TOURNAMENT_SELECTION 0


/* FUNCTIONS */
struct population *tournament_selection(
    struct population *p,
    int tournament_size,
    int (*cmp)(const void *, const void *),
    void *(*copy_func)(void *)
);

#endif
