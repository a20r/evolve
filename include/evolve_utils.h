#ifndef _EVOLVE_UTILS_H_
#define _EVOLVE_UTILS_H_

/* FUNCTIONS */
void print_chromosome(struct population *p, int index);
void print_chromosomes(struct population *p);
void print_population(struct population *p);
char *obtain_best_chromosome(struct population *p);
#endif
