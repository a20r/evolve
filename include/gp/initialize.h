#ifndef _GP_INTIALIZE_H_
#define _GP_INTIALIZE_H_

#include <dstruct/ast.h>
#include <dstruct/darray.h>

#include "config/config.h"
#include "population.h"


/* STRUCTS */
struct gp_tree
{
    struct ast *tree;
    int depth;
    int size;
};

/* FUNCTIONS */
struct gp_tree *init_tree_full(struct gp_tree_config *config);
void gp_tree_destroy(struct gp_tree *gp);
// void init_tree_grow(struct population *p);
// void init_tree_ramped_half_and_half(struct population *p);

#endif
