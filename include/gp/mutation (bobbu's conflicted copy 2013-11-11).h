#ifndef _GP_MUTATION_H_
#define _GP_MUTATION_H_

/* FUNCTIONS */
void point_mutation(struct gp_tree *tree, struct gp_tree_config *config);
void hoist_mutation(struct gp_tree *tree, struct gp_tree_config *config);
void shrink_mutation(struct gp_tree *tree, struct gp_tree_config *config);
void permutation_mutation(struct gp_tree *tree, struct gp_tree_config *config);
void mutate_tree(
        int prob,
        struct gp_tree *tree,
        struct gp_tree_config *config,
        void (*mutation_func)(struct gp_tree *, struct gp_tree_config *)
);

#endif
