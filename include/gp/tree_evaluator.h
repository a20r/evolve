#ifndef _GP_TREE_EVALUATOR_H_
#define _GP_TREE_EVALUATOR_H_

#include <dstruct/stack.h>
#include <dstruct/darray.h>


/* FUNCTIONS */
int evaluate_node(struct ast *node, struct stack *s);
struct ast *evaluate_program(struct darray *program);

#endif
