#ifndef _FUNCTION_SET_H_
#define _FUNCTION_SET_H_

#include <dstruct/ast.h>
#include <dstruct/darray.h>

#include "config/config.h"


/* FUNCTIONS */
struct ast *execute_unary_function(struct ast *node, struct ast *value);
struct ast *execute_binary_function(
        struct ast *node,
        struct ast *x,
        struct ast *y
);
int node_is_function(struct ast *node);
int function_nodes_equal(struct ast *node_1, struct ast *node_2);
struct ast *get_new_function_node(
        struct ast *node,
        enum ast_tag tag,
        struct gp_tree_config *config
);

#endif
