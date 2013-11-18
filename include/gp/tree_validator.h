#ifndef _GP_TREE_VALIDATOR_H_
#define _GP_TREE_VALIDATOR_H_

#include <dstruct/ast.h>
#include <dstruct/darray.h>

#include "gp/initialize.h"


/* FUNCTIONS */
int tree_validate(struct gp_tree *gp, struct darray *input_set);
int tree_add_input_nodes(struct gp_tree *gp, struct gp_tree_config *config);

#endif
