#ifndef _GP_INTIALIZE_H_
#define _GP_INTIALIZE_H_

#include <dstruct/ast.h>
#include <dstruct/darray.h>

#include "config/config.h"
#include "population.h"


/* STRUCTS */
struct gp_tree
{
        struct ast *root;
        struct darray *program;

        int depth;
        int size;

        struct darray *terminal_nodes;
        struct darray *input_nodes;
};

struct node_set
{
        struct darray *function_set;
        struct darray *terminal_set;
        struct darray *input_set;
};

/* FUNCTIONS */
struct gp_tree *init_tree_full(struct gp_tree_config *config);
void gp_tree_destroy(void *gp);
struct gp_tree *gp_tree_copy(struct gp_tree *src);
struct gp_tree *init_tree_full(struct gp_tree_config *config);
struct population *gp_population_initialize(
        struct gp_tree *(*tree_initializer)(struct gp_tree_config *),
        struct gp_tree_config *config
);

#endif
