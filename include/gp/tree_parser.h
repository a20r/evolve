#ifndef _GP_TREE_PARSER_H_
#define _GP_TREE_PARSER_H_

#include <dstruct/queue.h>

/* FUNCTIONS */
int print_node(struct ast *node);
int print_node_label(struct ast*node);
int print_relation(
        struct ast *from,
        struct ast *to,
        struct darray *node_list
);
int print_tree_structure(struct ast *node, struct darray *node_list);
void print_gp_program(struct darray *program);
int print_gp_tree(struct ast *node);
struct darray *parse_gp_tree(struct ast *node);
void update_program(struct gp_tree *tree);
void update_terminal_nodes(struct gp_tree *tree);
void update_input_nodes(struct gp_tree *tree, struct evolve_config *config);

#endif
