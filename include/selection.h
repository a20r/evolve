#ifndef __SELECTION__
#define __SELECTION__

#define TOURNAMENT_SELECTION 0

/* STRUCTS */
struct selection_config
{
    int method;

    int tournament_size;
    int (*cmp)(const void *, const void *);
    void *(*copy_func)(void *);
};

/* FUNCTIONS */
struct selection_config *selection_config_new(int method);
void selection_config_destroy(void *);
struct population *tournament_selection(
    struct population *p,
    struct selection_config *sc
);

#endif
