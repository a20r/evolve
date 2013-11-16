#ifndef _GP_TREE_VALIDATOR_H_
#define _GP_TREE_VALIDATOR_H_

#include <dstruct/ast.h>
#include <dstruct/darray.h>

#include "gp/initialize.h"


/* FUNCTIONS */
int terminal_nodes_equal(struct ast *node_1, struct ast *node_2);
int function_nodes_equal(struct ast *node_1, struct ast *node_2);
int validate_tree(struct gp_tree *gp, struct darray *input_set);

#endif
