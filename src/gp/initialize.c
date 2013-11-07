#include <dstruct/ast.h>
#include <dstruct/linked_list.h>
#include <al/utils.h>

#include "population.h"
#include "config/config.h"
#include "gp/initialize.h"
#include "gp/function_set.h"
#include "gp/terminal_set.h"


static struct gp_tree *gp_tree_create(struct gp_tree_config *config)
{
        int i = 0;
        struct gp_tree *gp;
        struct ast *node;

        /* initialize empty tree */
        gp = calloc(1, sizeof(struct gp_tree));
        i = randnum_i(0, config->function_set->end);
        node = darray_get(config->function_set, i);
        gp->tree = ast_copy_node(node);

        /* tree size and depth settings */
        gp->size = 1;
        gp->depth = 0;

        return gp;
}

void gp_tree_destroy(void *gp)
{
        ast_tree_destroy(((struct gp_tree *) gp)->tree);
        free(gp);
}

static struct ast *full_method_gen_node(
        int depth,
        int max_depth,
        struct darray *function_set,
        struct darray *terminal_set
)
{
        struct ast *node;
        int i;

        if ((depth + 1) == max_depth) {
                /* next node is max depth, make a terminal node */
                i = randnum_i(0, terminal_set->end);
                node = darray_get(terminal_set, i);
        } else {
                /* else make function node */
                i = randnum_i(0, function_set->end);
                node = darray_get(function_set, i);
        }

        return ast_copy_node(node);
}

static int full_method(
        struct gp_tree *tree,
        struct ast *node,
        int depth,
        int max_depth,
        struct darray *f_set,
        struct darray *t_set
)
{
        struct ast *n;
        int res = 0;

        /* fill tree */
        if (depth == max_depth) {
                return 0;
        } else if (node->tag == UNARY_OP) {
                /* value */
                n = full_method_gen_node(depth, max_depth, f_set, t_set);
                node->type.unary->value = n;
                tree->size++;

                res = full_method(tree, n, depth + 1, max_depth, f_set, t_set);
                check(res == 0, "Failed to intialize tree!");

        } else if (node->tag == BINARY_OP) {
                /* left */
                n = full_method_gen_node(depth, max_depth, f_set, t_set);
                node->type.binary->left= n;
                tree->size++;

                res = full_method(tree, n, depth + 1, max_depth, f_set, t_set);
                check(res == 0, "Failed to intialize tree!");

                /* right */
                n = full_method_gen_node(depth, max_depth, f_set, t_set);
                node->type.binary->right= n;
                tree->size++;

                res = full_method(tree, n, depth + 1, max_depth, f_set, t_set);
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
        int max_depth;
        struct darray *f_set;
        struct darray *t_set;

        /* setup */
        max_depth = *config->max_depth;
        f_set = config->function_set;
        t_set = config->terminal_set;

        /* initialize gp tree */
        gp = gp_tree_create(config);
        full_method(gp, gp->tree, gp->depth, max_depth, f_set, t_set);
        gp->depth = max_depth;

        return gp;
}

struct population *population_initialize(
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
