#ifndef _GP_INTIALIZE_H_
#define _GP_INTIALIZE_H_

#include <dstruct/ast.h>
#include <dstruct/darray.h>

#include "population.h"


/* STRUCTS */
struct gp_tree
{
    struct ast *tree;
    int depth;
    int size;
    int max_depth;
    int max_size;

    struct hashmap *function_set;
    struct hashmap *terminal_set;
}

/* FUNCTIONS */
void init_tree_full(struct population *p);
void init_tree_grow(struct population *p);
void init_tree_ramped_half_and_half(struct population *p);

#endif
