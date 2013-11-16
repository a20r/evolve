#ifndef _TERMINAL_SET_H_
#define _TERMINAL_SET_H_

#include <dstruct/ast.h>
#include <dstruct/darray.h>

#include "config/config.h"


/* FUNCTIONS */
int node_is_terminal(struct ast *node);
struct ast *get_new_terminal_node(
        struct ast *node,
        enum ast_tag tag,
        struct gp_tree_config *config
);

#endif
