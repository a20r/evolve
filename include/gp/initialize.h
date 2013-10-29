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
    int *max_depth;
    int *max_size;

    struct darray *function_set;
    struct darray *terminal_set;
};

/* FUNCTIONS */
struct gp_tree *init_tree_full(struct gp_tree_config *config);
// void init_tree_grow(struct population *p);
// void init_tree_ramped_half_and_half(struct population *p);

#endif
