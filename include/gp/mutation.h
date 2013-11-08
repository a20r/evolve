#ifndef _GP_MUTATION_H_
#define _GP_MUTATION_H_

/* FUNCTIONS */
void point_mutation(struct ast *node, struct gp_tree_config *config);
void hoist_mutation(struct ast *node, struct gp_tree_config *config);
void shrink_mutation(struct ast *node, struct gp_tree_config *config);
void permutation_mutation(struct ast *node, struct gp_tree_config *config);


#endif
