#ifndef _GP_TREE_EVALUATOR_H_
#define _GP_TREE_EVALUATOR_H_

#include <dstruct/ast.h>
#include <dstruct/stack.h>
#include <dstruct/darray.h>

#include "config/config.h"

#define INPUT_HEADER 1
#define RESPONSE_HEADER 2


/* FUNCTIONS */
int evaluate_node(struct ast *node, struct stack *s);
float evaluate_program(struct gp_tree *node, struct evolve_config *config);
int evaluate_programs(struct population *p, struct evolve_config *config);

#endif
