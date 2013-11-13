#ifndef _GP_MUTATION_H_
#define _GP_MUTATION_H_

/* FUNCTIONS */
int point_mutation(
        struct gp_tree *tree,
        int index,
        struct gp_tree_config *config
);
int hoist_mutation(
        struct gp_tree *tree,
        int index,
        struct gp_tree_config *config
);
int shrink_mutation(
        struct gp_tree *tree,
        int index,
        struct gp_tree_config *config
);
int permutation_mutation(
        struct gp_tree *tree,
        int index,
        struct gp_tree_config *config
);
int mutate_tree(
        int prob,
        struct gp_tree *tree,
        struct gp_tree_config *config,
        int (*mutation_func)(struct gp_tree *, int, struct gp_tree_config *)
);

#endif
