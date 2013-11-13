#include <dbg/dbg.h>
#include <dstruct/ast.h>

#include "gp/terminal_set.h"
#include "gp/tree_traverser.h"


int traverse_gp_tree(
        struct ast *node,
        int (*traverse_cb)(struct ast *node)
)
{
        int res = 0;

        if (node_is_terminal(node)) {
                /* terminal */
                res = traverse_cb(node);
                silent_check(res == 0);

        } else if (node->tag == UNARY_OP) {
                res = traverse_cb(node);
                silent_check(res == 0);

                /* value */
                traverse_gp_tree(node->type.unary->value, traverse_cb);

        } else if (node->tag == BINARY_OP) {
                res = traverse_cb(node);
                silent_check(res == 0);

                /* left */
                traverse_gp_tree(node->type.binary->left, traverse_cb);

                /* right */
                traverse_gp_tree(node->type.binary->right, traverse_cb);

        } else {
                log_err("Unknown node tag!");
                log_err("Failed to traverse tree node!");
                return -1;
        }

        return 0;
error:
        return 0;
}
