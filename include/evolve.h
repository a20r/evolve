#ifndef _EVOLVE_H_
#define _EVOLVE_H_

#include <signal.h>
#include "population.h"
#include "evolve_monitor.h"


/* STRUCTS */
struct evolution_settings
{
        struct population **p;
        float (*eval_func)(char *);

        void (*selection_func)(struct population **, int *);
        int *select;

        int (*crossover_func)(void **, void **, int);
        float crossover_prob;
        int pivot_index;

        void (*mutation_func)(char **);
        float *mutate_prob;

        struct evolve_monitor *m;

        int wait_signal;
        volatile sig_atomic_t *stop_signal;
};

/* FUNCTIONS */
// int run_evolution(struct evolution_settings *settings);
int run_evolution(
        struct population **p,
        float (*eval_func)(char *),
        void (*selection_func)(struct population **, int *),
        int *select,
        int (*crossover_func)(void **, void **, int),
        float crossover_prob,
        int pivot_index,
        void (*mutation_func)(char **),
        float *mutate_prob,
        struct evolve_monitor *m,
        int wait_signal,
        volatile sig_atomic_t *stop_signal
);
#endif
