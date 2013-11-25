#ifndef _EVOLVE_H_
#define _EVOLVE_H_

#include <signal.h>
#include "config/config.h"
#include "population.h"
#include "evolve_monitor.h"


/* FUNCTIONS */
int run_evolution(
        struct population **p,
        float (*eval_func)(void *),
        void (*selection_func)(struct population **, int *),
        int *select,
        int (*crossover_func)(void **, void **, int),
        float *crossover_prob,
        int pivot_index,
        void (*mutation_func)(char **),
        float *mutate_prob,
        struct evolve_monitor *m,
        int wait_signal,
        volatile sig_atomic_t *stop_signal
);
int evolve(
        struct population *p,
        float (*eval_func)(void *, struct evolve_config *),
        int (*eval_pop_func)(
                struct population *,
                struct evolve_config *
        ),
        void (*selection_func)(struct population **, int *),
        int (*crossover_func)(void *, void *, struct evolve_config *),
        int (*mutation_func)(void *, struct evolve_config *),
        struct evolve_config *config,
        struct evolve_monitor *m,
        int wait_signal,
        volatile sig_atomic_t *stop_signal
);
#endif
