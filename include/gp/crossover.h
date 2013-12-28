#ifndef _GP_CROSSOVER_H_
#define _GP_CROSSOVER_H_

#include <dstruct/ast.h>

#include "gp/initialize.h"


/* FUNCTIONS */
int gp_crossover_random_index(
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        struct gp_tree_config *config
);
struct ast *gp_get_linked_node(
        struct ast *node,
        struct darray *program,
        int index
);
void gp_crossover_replace_func_node_value(
        int branch,
        struct ast *func_node,
        struct ast *val_node
);
void gp_crossover_switch_nodes(
        struct ast *node_1,
        struct ast *node_2,
        struct gp_tree *tree_1,
        struct gp_tree *tree_2,
        int index
);
int gp_one_point_crossover(
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
