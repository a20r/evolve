#ifndef _GP_CROSSOVER_H_
#define _GP_CROSSOVER_H_

#include <dstruct/ast.h>


/* FUNCTIONS */
int one_point_crossover(
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config
);
int crossover_trees(
        int prob,
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config,
        int (*crossover_func)(
                struct gp_tree *,
                struct gp_tree *,
                struct gp_tree_config *
        )
);

#endif
