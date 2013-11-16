#include <stdlib.h>
#include <string.h>

#include <dbg/dbg.h>
#include <al/utils.h>

#include "gp/initialize.h"
#include "gp/terminal_set.h"
#include "gp/tree_validator.h"


int node_is_terminal(struct ast *node)
{
        int i;
        enum ast_tag tags[5] = {
                INTEGER,
                REAL,
                STRING,
                CHARACTER,
                BOOL
        };

        for (i = 0; i < 5; i++) {
                if (node->tag == tags[i]) {
                        return 1;
                }
        }

        return 0;
}

struct ast *get_new_terminal_node(
        struct ast *node,
        enum ast_tag tag,
        struct gp_tree_config *config
)
{
        int i = 0;
        int index = 0;
        int limit = 100;
        int equals = 0;
        struct ast *new_node = NULL;

        while (i != limit) {
                /* get new terminal node */
                index = randnum_i(0, config->terminal_set->end);
                new_node = darray_get(config->terminal_set, index);

                /* terminate conditions */
                equals = terminal_nodes_equal(new_node, node);
                if (equals == 0 && new_node->tag == tag) {
                        break;
                }

                i++;
        }

        return new_node;
}
