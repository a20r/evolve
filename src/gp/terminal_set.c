#include <stdlib.h>
#include <string.h>

#include <dbg/dbg.h>
#include <al/utils.h>
#include <dstruct/ast_cmp.h>

#include "gp/initialize.h"
#include "gp/terminal_set.h"
#include "gp/tree_validator.h"
#include "gp/tree_parser.h"


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

int node_is_input(struct ast *node, struct evolve_config *config)
{
        int i;
        struct darray *input_set;
        struct ast *input_node;

        input_set = config->general.gp_tree->input_set;
        for (i = 0; i < input_set->end; i++) {
                input_node = darray_get(input_set, i);
                if (ast_nodes_equal(input_node, node)) {
                        return 1;
                }
        }

        return 0;
}

int terminal_nodes_equal(struct ast *node_1, struct ast *node_2)
{
        int res;

        if (node_1 == node_2) {
                return 1;
        } else if (node_1->tag == node_2->tag) {
                if (node_1->tag == INTEGER) {
                        res = ast_node_integer_equals(node_1, node_2);
                } else if (node_1->tag == REAL) {
                        res = ast_node_real_equals(node_1, node_2);
                } else if (node_1->tag == STRING) {
                        res = ast_node_string_equals(node_1, node_2);
                } else if (node_1->tag == CHARACTER) {
                        res = ast_node_character_equals(node_1, node_2);
                } else if (node_1->tag == BOOL) {
                        res = ast_node_boolean_equals(node_1, node_2);
                } else {
                        log_err("Unrecognised node tag!");
                        res = -1;
                }
        } else {
                res = 0;
        }

        return res;
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

struct ast *get_new_input_node(
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
                index = randnum_i(0, config->input_set->end);
                new_node = darray_get(config->input_set, index);

                /* terminate conditions */
                equals = terminal_nodes_equal(new_node, node);
                if (equals == 0 && new_node->tag == tag) {
                        break;
                }

                i++;
        }

        return new_node;
}
