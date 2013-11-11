#include <stdio.h>

#include <dbg/dbg.h>
#include <dstruct/ast.h>
#include <dstruct/darray.h>


static int print_node(struct ast *node)
{
        if (node->tag == INTEGER) {
                printf("\"%d", node->type.integer);
                printf("%p\"", &node->type.integer);
        } else if (node->tag == REAL) {
                printf("\"%f", node->type.real);
                printf("%p\"", &node->type.real);
        } else if (node->tag == STRING) {
                printf("\"%s", node->type.string);
                printf("%p\"", &node->type.string);
        } else if (node->tag == CHARACTER) {
                printf("\"%c", node->type.character);
                printf("%p\"", &node->type.character);
        } else if (node->tag == BOOL) {
                printf("\"%i", node->type.boolean);
                printf("%p\"", &node->type.boolean);
        } else if (node->tag == UNARY_OP) {
                printf("\"%s", node->type.unary->op_name);
                printf("%p\"", &node->type.unary->op_name);
        } else if (node->tag == BINARY_OP || node->tag == START) {
                printf("\"%s", node->type.binary->op_name);
                printf("%p\"", &node->type.binary->op_name);
        } else {
                log_err("OPPS! Unknown node type to print!");
                return -1;
        }

        return 0;
}

static int print_node_label(struct ast*node)
{
        if (node->tag == INTEGER) {
                printf("[label=\"%d\"];\n", node->type.integer);
        } else if (node->tag == REAL) {
                printf("[label=\"%f\"];\n", node->type.real);
        } else if (node->tag == STRING) {
                printf("[label=\"%s\"];\n", node->type.string);
        } else if (node->tag == CHARACTER) {
                printf("[label=\"%c\"];\n", node->type.character);
        } else if (node->tag == BOOL) {
                printf("[label=\"%i\"];\n", node->type.boolean);
        } else if (node->tag == UNARY_OP) {
                printf("[label=\"%s\"];\n", node->type.unary->op_name);
        } else if (node->tag == BINARY_OP || node->tag == START) {
                printf("[label=\"%s\"];\n", node->type.binary->op_name);
        } else {
                log_err("OPPS! Unknown node type to q!");
                return -1;
        }

        return 0;
}

static int print_relation(
        struct ast *from,
        struct ast *to,
        struct darray *node_list
)
{
        /* print */
        print_node(from);
        printf(" -> ");
        print_node(to);
        printf(";\n");

        /* push node to node_list */
        darray_push(node_list, to);

        return 0;
}

static int print_tree_structure(struct ast *node, struct darray *node_list)
{
        int res = 0;

        if (node->tag == BINARY_OP || node->tag == START) {
                res = print_relation(node, node->type.binary->left, node_list);
                silent_check(res == 0);

                res = print_tree_structure(node->type.binary->left, node_list);
                check(res == 0, "Failed to parse the left binary op!");

                res = print_relation(node, node->type.binary->right, node_list);
                silent_check(res == 0);

                res = print_tree_structure(node->type.binary->right, node_list);
                check(res == 0, "Failed to parse the right binary op!");
        }

        return res;
error:
        return res;
}

int print_gp_tree(struct ast *node)
{
        int i = 0;
        struct darray *node_list;
        struct ast *n;

        /* setup */
        node_list = darray_create(sizeof(struct ast), 100);
        darray_push(node_list, node);

        /* print nodes and relationships */
        print_tree_structure(node, node_list);
        printf("\n");

        /* print node_list  */
        for (i = 0; i < node_list->end; i++) {
                n = darray_get(node_list, i);
                print_node(n);
                print_node_label(n);
        }

        /* clean up */
        darray_destroy(node_list);

        return 0;
}

static int post_order_traverse(struct ast *node, struct darray *program)
{
        int status = 0;
        struct ast *node_copy;

        /* POST-ORDER TRAVERSAL */
        if (node->tag == INTEGER
                || node->tag == REAL
                || node->tag == STRING
                || node->tag == CHARACTER
                || node->tag == BOOL
        ) {
                node_copy = ast_copy_node(node);
                darray_push(program, node_copy);

        } else if (node->tag == UNARY_OP) {
                /* value */
                status = post_order_traverse(node->type.unary->value, program);
                check(status == 0, "Failed to traverse unary node!");

                /* root */
                node_copy = ast_copy_node(node);
                darray_push(program, node_copy);

        } else if (node->tag == BINARY_OP || node->tag == START) {
                /* left */
                status = post_order_traverse(node->type.binary->left, program);
                check(status == 0, "Failed to traverse binary->left node!");

                /* right */
                status = post_order_traverse(node->type.binary->right, program);
                check(status == 0, "Failed to traverse binary->right node!");

                /* root */
                node_copy = ast_copy_node(node);
                darray_push(program, node_copy);

        } else {
                log_err("OPPS! Unknown node type to q!");
                goto error;
        }

        return 0;
error:
        return -1;
}

struct darray *parse_gp_tree(struct ast *node)
{
        int status = 0;
        struct darray *program;

        /* setup */
        program = darray_create(sizeof(struct ast), 100);

        /* parse gp tree */
        status = post_order_traverse(node, program);
        check(status == 0, "Failed to traverse gp tree!");
        check(program->end > 0, "Program should be larger than 0!");

        return program;
error:
        return NULL;
}
