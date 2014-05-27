#ifndef __POPULATION__
#define __POPULATION__

/* STRUCTURES */
struct population
{
    int generation;
    int num_individuals;

    void *individuals;
};

/* FUNCTIONS */
struct population *population_new();
void population_sort(struct population *p, float (get_score)(void *));



#endif
