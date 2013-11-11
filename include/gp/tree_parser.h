#ifndef _GP_TREE_PARSER_H_
#define _GP_TREE_PARSER_H_

#include <dstruct/queue.h>

/* FUNCTIONS */
int print_gp_tree(struct ast *node);
struct darray *parse_gp_tree(struct ast *node);

#endif
