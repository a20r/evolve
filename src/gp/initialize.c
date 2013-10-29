#include <dstruct/ast.h>
#include <dstruct/stack.h>
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
        struct darray *func_set,
        struct darray *term_set
)
{
        int i = 0;
        char *n_type;
        struct ast *new_node;

        printf("Depth: %d\n", depth);
        if (depth < max_depth) {
                i = randnum_i(0, func_set->end);
                n_type = darray_get(func_set, i);
                new_node = gp_default_function_factory(n_type);
        } else if (depth == max_depth) {
                i = randnum_i(0, term_set->end);
                n_type = darray_get(term_set, i);
                new_node = gp_default_function_factory(n_type);
        }

        return new_node;
}

static void full_method(
        struct gp_tree *tree,
        struct stack *s,
        struct ast *node,
        int depth,
        int max_depth,
        struct darray *func_set,
        struct darray *term_set
)
{
        struct ast *n;

        /* fill tree */
        stack_push(s, gp->tree);
        while ((node = stack_pop(s))) {
                if (node->tag == UNARY_OP) {
                        /* generate node */
                        n = gen_node(depth, max_depth, func_set, term_set);
                        node->type.unary->value = n;

                        /* push to stack */
                        stack_push(s, node->type.unary->value);
                        tree->depth++;

                } else if (node->tag == START || node->tag == BINARY_OP) {
                        /* generate nodes */
                        n = gen_node(depth, max_depth, func_set, term_set);
                        node->type.binary->left= n;

                        n = gen_node(depth, max_depth, func_set, term_set);
                        node->type.binary->right= n;

                        /* push to stack */
                        stack_push(s, node->type.binary->left);
                        stack_push(s, node->type.binary->right);
                        tree->depth++;
                } else {
                        return gp;
                }
        }

        return gp;
}

struct gp_tree *init_tree_full(struct gp_tree_config *config)
{
        struct gp_tree *gp;
        struct stack *s;
        struct ast *node;

        struct darray *func_set = config->function_set;
        struct darray *term_set = config->terminal_set;

        /* initialize gp_tree */
        gp = gp_tree_create(config->max_depth, config->max_size, config);
        s = stack_create(0);


        /* clean up */
        stack_destroy(s);

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
