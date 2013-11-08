#ifndef _GP_TREE_TRAVERSER_H_
#define _GP_TREE_TRAVERSER_H_

/* FUNCTIONS */
int traverse_gp_tree(
        struct ast *node,
        int (*traverse_cb)(struct ast *node)
);

#endif
