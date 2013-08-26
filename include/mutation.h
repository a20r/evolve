#ifndef _MUTATE_H_
#define _MUTATE_H_

#include "evolve.h"

/* FUNCTIONS */
void mutate_str(char **str);
void mutate_dna(char **dna_str);
void mutate(
        void **chromosome,
        float mutate_prob,
        void (mutate_func)(char **)
);

#endif
