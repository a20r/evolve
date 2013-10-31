#include <dstruct/ast.h>
#include <dstruct/linked_list.h>
#include <al/utils.h>

#include "population.h"
#include "config/config.h"
#include "gp/initialize.h"
#include "gp/function_set.h"
#include "gp/terminal_set.h"


static struct gp_tree *gp_tree_create(
        int *max_depth,
        int *max_size,
        struct gp_tree_config *config
)
{
        struct gp_tree *gp;
        int func_set_len = config->function_set->end + 1;
        int term_set_len = config->terminal_set->end + 1;

        /* initialize empty tree */
        gp = calloc(1, sizeof(struct gp_tree));
        gp->tree = ast_tree_create();

        /* tree size and depth settings */
        gp->size = 0;
        gp->depth = 0;
        gp->max_size = max_size;
        gp->max_depth = max_depth;

        /* initialize building block array */
        gp->terminal_set = darray_create(sizeof(struct ast), func_set_len);
        gp->function_set = darray_create(sizeof(struct ast), term_set_len);

        return gp;
}

void gp_tree_destory(struct gp_tree *gp)
{
        ast_tree_destroy(gp->tree);

        free(gp->max_size);
        free(gp->max_depth);

        darray_destroy(gp->function_set);
        darray_destroy(gp->terminal_set);

        free(gp);
}

static struct ast *gen_node(
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
                printf("TERM\n");
        } else {
                /* else make function node */
                i = randnum_i(0, function_set->end);
                node = darray_get(function_set, i);
                printf("NODE: %s\n", node->type.binary->op_name);
        }

        return node;
}

static int full_method(
        struct gp_tree *tree,
        struct ast *node,
        int depth,
        int max_depth
)
{
        struct darray *function_set;
        struct darray *terminal_set;
        struct ast *n;
        int res = 0;

        function_set = tree->function_set;
        terminal_set = tree->terminal_set;

        /* fill tree */
        if (depth == max_depth) {
                return 0;
        } else if (node->tag == UNARY_OP) {
                n = gen_node(depth, max_depth, function_set, terminal_set);
                node->type.unary->value = n;

                res = full_method(tree, n, depth + 1, max_depth);
                check(res == 0, "Failed to intialize tree!");

        } else if (node->tag == START || node->tag == BINARY_OP) {
                /* left */
                n = gen_node(depth, max_depth, function_set, terminal_set);
                node->type.binary->left= n;

                res = full_method(tree, n, depth + 1, max_depth);
                check(res == 0, "Failed to intialize tree!");

                /* right */
                n = gen_node(depth, max_depth, function_set, terminal_set);
                node->type.binary->right= n;

                res = full_method(tree, n, depth + 1, max_depth);
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
        struct darray *function_set = config->function_set;
        struct darray *terminal_set = config->terminal_set;

        /* initialize gp tree */
        gp = gp_tree_create(config->max_depth, config->max_size, config);
        gp->function_set = function_set;
        gp->terminal_set = terminal_set;

        full_method(gp, gp->tree, gp->depth, *gp->max_depth);
        printf("\n\n");

        return gp;
}

struct gp_tree *init_tree_grow(
        struct population *p,
        struct gp_tree_config *config
)
{
        struct gp_tree *gp;

        gp = gp_tree_create(config->max_depth, config->max_size, config);


        return gp;
}

struct gp_tree *init_tree_ramped_half_and_half(
        struct population *p,
        struct gp_tree_config *config
)
{
        struct gp_tree *gp;

        gp = gp_tree_create(config->max_depth, config->max_size, config);


        return gp;
}

/* struct population *population_initialize( */
/*         struct gp_tree *(*tree_initializer)(struct gp_tree_config *), */
/*         struct gp_tree_config *config */
/* ) */
/* { */
/*         int i = 0; */
/*         struct gp_tree *tree; */
/*         struct population *p; */
/*         float goal = 0.0; */
/*  */
/*         p = init_population( */
/*                 sizeof(struct gp_tree), */
/*                 goal, */
/*                 *config->max_pop, */
/*                 *config->max_gen */
/*         ); */
/*  */
/*         #<{(| fill initial random individual |)}># */
/*         for (i = 0; i < p->max_population; i++) { */
/*                 darray_set(p->individuals, i, (*tree_initializer)(config)); */
/*                 p->population++; */
/*         } */
/*  */
/*         return p; */
/* } */
