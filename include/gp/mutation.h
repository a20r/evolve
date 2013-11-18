#ifndef _GP_MUTATION_H_
#define _GP_MUTATION_H_

/* FUNCTIONS */
int gp_point_mutation(
        void *tree,
        struct evolve_config *config
);
int gp_hoist_mutation(
        void *tree,
        struct evolve_config *config
);
int gp_shrink_mutation(
        void *tree,
        struct evolve_config *config
);
int gp_permutation_mutation(
        void *tree,
        struct evolve_config *config
);
int mutate_tree(
        void *tree,
        int (*mutation_func)(void *, struct evolve_config *),
        struct evolve_config *config
);

#endif
