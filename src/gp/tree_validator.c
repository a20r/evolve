#include <stdio.h>
#include <string.h>

#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/linked_list.h>
#include <dstruct/ast_cmp.h>

#include "gp/terminal_set.h"
#include "gp/function_set.h"
#include "gp/tree_parser.h"
#include "gp/tree_validator.h"


static struct darray *create_input_check_list(struct darray *input_set)
{
        int i;
        int len;
        struct darray *input_check_list;
        struct ast *node;

        len = input_set->end + 1;
        input_check_list = darray_create(sizeof(struct ast), len);
        for (i = 0; i < len; i++) {
                node = darray_get(input_set, i);
                darray_set(input_check_list, i, ast_copy_node(node));
        }

        return input_check_list;
}

static void destroy_input_check_list(struct darray *check_list)
{
        int i = 0;
        struct ast *node;

        for (i = 0; i <= check_list->end; i++) {
                node = darray_get(check_list, i);
                release_mem(node, ast_destroy);
        }
        darray_destroy(check_list);
}

int validate_tree(struct gp_tree *gp, struct darray *input_set)
{
        int i;
        int j;
        int res;
        int check_list_len;
        int term_nodes_len;
        struct darray *input_check_list;
        struct ast *node_1;
        struct ast *node_2;

        /* setup */
        input_check_list = create_input_check_list(input_set);
        check_list_len = input_check_list->end;
        term_nodes_len = gp->terminal_nodes->end;

        /* loop through terminal nodes and compare */
        for (j = 0; j < term_nodes_len; j++) {
                node_1 = darray_get(gp->terminal_nodes, j);

                printf("[j]: %d ", j);
                print_node(node_1);
                printf("\n");

                /* loop through and tick off input check list */
                for (i = check_list_len; i >= 0 ; i--) {
                        node_2 = darray_get(input_check_list, i);

                        printf("[i]: %d ", i);
                        print_node(node_2);
                        printf("\n");

                        /* compare nodes */
                        res = terminal_nodes_equal(node_1, node_2);
                        if (res == 1) {
                                darray_pop(input_check_list);
                                ast_destroy(node_2);
                                break;
                        }
                }
        }

        /* check if input check list still has items */
        node_1 = darray_get(input_check_list, 0);
        if (input_check_list->end == 0 || node_1 == NULL) {
                res = 1;
        } else {
                res = 0;
        }

        /* clean up */
        destroy_input_check_list(input_check_list);

        return res;
}

static void term_node_to_input_node(struct ast *term_node, struct ast *input_node)
{
        int len = 0;

        /* release memory if node is string */
        if (term_node->tag == STRING) {
                free(term_node->type.string);
        }

        /* modify terminal node to be an input node */
        len = strlen(input_node->type.string);
        term_node->tag = STRING;
        term_node->type.string = malloc(sizeof(char) * len + 1);
        strcpy(term_node->type.string, input_node->type.string);
}

static int nodes_equal(void *node_1, void *node_2)
{
        int res = 0;
        res = ast_nodes_equal((struct ast *) node_1, (struct ast *) node_2);
        return res;
}

int tree_add_input_nodes(struct gp_tree *gp, struct gp_tree_config *config)
{
        int i = 0;
        int res = 0;
        int start = 0;
        int end = 0;
        int inputs = 0;
        int input_index = 0;
        int term_index = 0;
        struct ast *node;
        struct ast *input_node;
        struct darray *visited;

        /* determine how many input nodes to add randomly */
        start = config->input_set->end + 1;
        end = gp->terminal_nodes->end / 2;
        inputs = randnum_i(start, end);

        /* mutate terminal nodes */
        visited = darray_create(sizeof(struct ast), inputs);

        while (inputs > 0) {
                /* get terminal node */
                term_index = randnum_i(0, gp->terminal_nodes->end - 1);
                node = darray_get(gp->terminal_nodes, term_index);

                /* swtich terminal node if not visited */
                if (darray_has(visited, node, nodes_equal) == 0) {
                        darray_push(visited, node);

                        /* get input node */
                        input_node = darray_get(config->input_set, input_index);

                        /* convert terminal node to input node */
                        term_node_to_input_node(node, input_node);
                        darray_push(gp->input_nodes, node);

                        /* increment input_index */
                        input_index++;
                        if (input_index > config->input_set->end) {
                                input_index = 0;
                        }

                        inputs--;
                }
        }
        darray_destroy(visited);

        return res;
}
