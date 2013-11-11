#include <stdio.h>

#include <dbg/dbg.h>
#include <dstruct/ast.h>
#include <al/utils.h>

#include "config/config.h"
#include "gp/initialize.h"
#include "gp/mutation.h"
#include "gp/tree_parser.h"
#include "gp/tree_validator.h"


static struct ast *get_new_terminal_node(
        struct ast *node,
        struct gp_tree_config *config
)
{
        int i = 0;
        int index = 0;
        int limit = 100;
        struct ast *new_node = NULL;


        while (i != limit) {
                index = randnum_i(0, config->terminal_set->end);
                new_node = darray_get(config->terminal_set, index);
                new_node = ast_copy_node(new_node);

                if (terminal_nodes_equal(new_node, node) == 0) {
                        break;
                }

                i++;
        }

        return new_node;
}
/* static int point_mutate_function_node( */
/*         struct ast **node, */
/*         struct gp_tree_config *config */
/* ) */
/* { */
/*         int index; */
/*         struct ast *new_node; */
/*         struct ast *value; */
/*         struct ast *left; */
/*         struct ast *right; */
/*  */
/*  */
/*         if ((*node)->tag == UNARY_OP) { */
/*                 value = (*node)->type.unary->value; */
/*  */
/*                 #<{(| get new function node |)}># */
/*                 while (1) { */
/*                         index = randnum_i(0, config->function_set->end); */
/*                         new_node = darray_get(config->function_set, index); */
/*                         new_node = ast_copy_node(new_node); */
/*  */
/*                         if (new_node->tag == UNARY_OP) { */
/*                                 break; */
/*                         } */
/*                 } */
/*  */
/*                 new_node->type.unary->value = value; */
/*                 ast_destroy((*node)); */
/*                 *node = new_node; */
/*  */
/*         } else if ((*node)->tag == BINARY_OP) { */
/*                 left = (*node)->type.binary->left; */
/*                 right = (*node)->type.binary->right; */
/*  */
/*                 #<{(| get new function node |)}># */
/*                 while (1) { */
/*                         index = randnum_i(0, config->function_set->end); */
/*                         new_node = darray_get(config->function_set, index); */
/*                         new_node = ast_copy_node(new_node); */
/*  */
/*                         if (new_node->tag == BINARY_OP) { */
/*                                 break; */
/*                         } */
/*                 } */
/*  */
/*                 new_node->type.binary->left = left; */
/*                 new_node->type.binary->right = right; */
/*                 ast_destroy((*node)); */
/*                 *node = new_node; */
/*  */
/*         } else { */
/*                 log_err("Error! Unrecognised node tag!"); */
/*                 return -1; */
/*         } */
/*  */
/*         return 0; */
/* } */

static int point_mutate_term_node(
        struct ast *node,
        struct ast *func_node,
        struct gp_tree_config *config
)
{
        int left = 0;
        int right = 0;
        struct ast *new_node;
        struct ast *left_node;
        struct ast *right_node;

        /* get new term node */
        new_node = get_new_terminal_node(node, config);

        printf("\nMutation Terminal Node\n");
        print_node(new_node);

        if (func_node->tag == BINARY_OP) {
                left_node = func_node->type.binary->left;
                right_node = func_node->type.binary->right;

                left = terminal_nodes_equal(node, left_node);
                right = terminal_nodes_equal(node, right_node);

                if (left) {
                        printf("\nLEFT!\n");
                        ast_destroy(func_node->type.binary->left);
                        func_node->type.binary->left = new_node;
                } else if (right) {
                        printf("\nRIGHT!\n");
                        ast_destroy(func_node->type.binary->right);
                        func_node->type.binary->right = new_node;
                }
        }



        return 0;
}

static struct ast *get_linked_func_node(
        struct darray *program,
        int index
)
{
        struct ast *func_node;

        while(1) {
                func_node = darray_get(program, index);
                if (func_node->tag == UNARY_OP
                        || func_node->tag == BINARY_OP
                )
                {
                        break;
                } else if (index == 0) {
                        return NULL;
                }

                index++;
        }

        return func_node;
}

int point_mutation(
        struct gp_tree *tree,
        struct gp_tree_config *config
)
{
        int index = 0;
        int res = 0;
        struct ast *prev_node;
        struct ast *node;
        struct darray *program;

        program = tree->program;
        while (1) {
                index = randnum_i(1, tree->size - 1);
                node = darray_get(program, index);


                /* if (node->tag == UNARY_OP || node->tag == BINARY_OP) { */
                /*         res = point_mutate_function_node(&node, config); */
                /*         break; */
                /* } else if (node->tag == INTEGER */
                if (node->tag == INTEGER
                        || node->tag == REAL
                        || node->tag == STRING
                        || node->tag == CHARACTER
                        || node->tag == BOOL
                )
                {
                        prev_node = get_linked_func_node(program, index);

                        printf("\n");
                        printf("selected: ");
                        print_node(node);
                        printf("\n");
                        printf("function node: ");
                        print_node(prev_node);
                        printf("\n");

                        res = point_mutate_term_node(node, prev_node, config);
                        break;
                /* } else { */
                /*         log_err("Error! Unrecognised node tag!"); */
                /*         return -1; */
                }
        }
        /* darray_destroy(program); */
        /* tree->program = parse_gp_tree(tree->root); */
        check(res == 0, "Failed to perform point mutation!");

        return 0;
error:
        return -1;
}

int mutate_tree(
        int prob,
        struct gp_tree *tree,
        struct gp_tree_config *config,
        int (*mutation_func)(struct gp_tree *, struct gp_tree_config *)
)
{
        int res = 0;

        /* to mutate or not mutate */
        if (prob > randnum_f(0.0, 1.0)) {
                goto mutate;
        } else {
                goto no_mutate;
        }

mutate:
        res = (*mutation_func)(tree, config);
        if (res == -1) {
                log_err("Error! Failed to mutate tree!");
        }

        return res;
no_mutate:
        return -1;
}
