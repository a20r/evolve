#include <dstruct/ast.h>
#include <dstruct/linked_list.h>
#include <al/utils.h>

#include "population.h"
#include "config/config.h"
#include "gp/initialize.h"
#include "gp/function_set.h"
#include "gp/terminal_set.h"
#include "gp/tree_parser.h"
#include "gp/tree_validator.h"


static struct gp_tree *gp_tree_create(struct gp_tree_config *config)
{
        int i = 0;
        struct gp_tree *gp;
        struct ast *node;

        /* initialize empty tree */
        gp = calloc(1, sizeof(struct gp_tree));

        i = randnum_i(0, config->function_set->end);
        node = darray_get(config->function_set, i);
        gp->root = ast_copy_node(node);

        /* initialize program */
        gp->program = NULL;

        /* tree size and depth settings */
        gp->size = 1;
        gp->depth = 0;

        /* terminal and input node reference */
        gp->terminal_nodes = darray_create(sizeof(struct ast), 100);
        gp->input_nodes = darray_create(sizeof(struct ast), 100);

        return gp;
}

void gp_tree_destroy(void *gp)
{
        ast_tree_destroy(((struct gp_tree *) gp)->root);
        darray_destroy(((struct gp_tree *) gp)->program);
        darray_destroy(((struct gp_tree *) gp)->terminal_nodes);
        darray_destroy(((struct gp_tree *) gp)->input_nodes);
        free(gp);
}

static struct ast *full_method_gen_node(
        struct gp_tree *tree,
        int depth,
        int max_depth,
        struct node_set *nodes
)
{
        int i;
        struct ast *node;

        if ((depth + 1) == max_depth) {
                /* next node is max depth, make a terminal node */
                i = randnum_i(0, nodes->terminal_set->end);
                node = darray_get(nodes->terminal_set, i);
                node = ast_copy_node(node);

                darray_push(tree->terminal_nodes, node);
                tree->size++;
        } else {
                /* else make function node */
                i = randnum_i(0, nodes->function_set->end);
                node = darray_get(nodes->function_set, i);
                node = ast_copy_node(node);
                tree->size++;
        }

        return node;
}

static int full_method(
        struct gp_tree *tree,
        struct ast *node,
        int depth,
        int max_depth,
        struct node_set *nodes
)
{
        struct ast *n;
        int res = 0;

        /* fill tree */
        if (depth == max_depth) {
                return 0;
        } else if (node->tag == UNARY_OP) {
                /* value */
                n = full_method_gen_node(tree, depth, max_depth, nodes);
                node->type.unary->value = n;

                res = full_method(tree, n, depth + 1, max_depth, nodes);
                check(res == 0, "Failed to intialize tree!");

        } else if (node->tag == BINARY_OP) {
                /* left */
                n = full_method_gen_node(tree, depth, max_depth, nodes);
                node->type.binary->left= n;

                res = full_method(tree, n, depth + 1, max_depth, nodes);
                check(res == 0, "Failed to intialize tree!");

                /* right */
                n = full_method_gen_node(tree, depth, max_depth, nodes);
                node->type.binary->right= n;

                res = full_method(tree, n, depth + 1, max_depth, nodes);
                check(res == 0, "Failed to intialize tree!");

        } else {
                return -1;
        }

        return 0;
error:
        return -1;
}

struct gp_tree *init_tree_full(struct gp_tree_config *config)
{
        struct gp_tree *gp;
        struct node_set *nodes;
        int max_depth;
        int valid_tree = 0;

        /* setup */
        max_depth = *config->max_depth;
        nodes = calloc(1, sizeof(struct node_set));
        nodes->function_set = config->function_set;
        nodes->terminal_set = config->terminal_set;
        nodes->input_set = config->input_set;

        /* initialize gp tree */
        while (valid_tree == 0) {
                gp = gp_tree_create(config);
                full_method(gp, gp->root, gp->depth, max_depth, nodes);
                gp->depth = max_depth;

                if (gp->terminal_nodes->end > gp->input_nodes->end + 1) {
                        valid_tree = 1;
                }
        }

        /* add input nodes */
        tree_add_input_nodes(gp, config);

        /* initialize gp program */
        gp->program = parse_gp_tree(gp->root);

        /* clean up */
        free(nodes);

        return gp;
}

struct population *gp_population_initialize(
        struct gp_tree *(*tree_initializer)(struct gp_tree_config *),
        struct gp_tree_config *config
)
{
        int i = 0;
        struct gp_tree *tree;
        struct population *p;
        float goal = 0.0;

        /* intialize population */
        p = population_create(
                sizeof(struct gp_tree),
                goal,
                *config->max_pop,
                *config->max_gen
        );

        /* fill initial random individual */
        for (i = 0; i < p->max_population; i++) {
                tree = (*tree_initializer)(config);
                darray_set(p->individuals, i, tree);
                p->population++;
        }

        return p;
}
