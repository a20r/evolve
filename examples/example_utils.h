#ifndef _EXAMPLE_UTILS_H_
#define _EXAMPLE_UTILS_H_

#include "population.h"
#include "evolve_monitor.h"

void print_evolve_results(struct population *p, float (*fitness_func)(char *));
void print_top_chromosomes(struct evolve_monitor *m, int top);

#endif
