#ifndef _GP_CROSSOVER_H_
#define _GP_CROSSOVER_H_

#include <dstruct/ast.h>


/* FUNCTIONS */
int one_point_crossover(
        void *tree_1,
        void *tree_2,
        struct evolve_config *config
);
int crossover_trees(
        void *tree_1,
        void *tree_2,
        int (*crossover_func)(void *, void *, struct evolve_config *),
        struct evolve_config *config
);

#endif
