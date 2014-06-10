#ifndef __EVOLVE__
#define __EVOLVE__

/* STRUCTS */
struct evolve_stats
{
    int generation;
    int stale_counter;
    int population_size;

    int individuals_evaluated;
    float diversity;
};


/* FUNCTIONS */
struct evolve_stats *evolve_stats_new(void);
void evolve_stats_destroy(void *stats);

#endif
