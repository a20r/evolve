#include <stdio.h>

#include <dbg/dbg.h>
#include <dstruct/ast.h>
#include <dstruct/linked_list.h>


static int dot_graph_list_node(struct ast*node)
{
        if (node->tag == INTEGER) {
                printf("\"%d", node->type.integer);
                printf("%p\"", &node->type.integer);
                printf("[label=\"%d\"];\n", node->type.integer);
        } else if (node->tag == REAL) {
                printf("\"%f", node->type.real);
                printf("%p\"", &node->type.real);
                printf("[label=\"%f\"];\n", node->type.real);
        } else if (node->tag == STRING) {
                printf("\"%s", node->type.string);
                printf("%p\"", &node->type.string);
                printf("[label=\"%s\"];\n", node->type.string);
        } else if (node->tag == CHARACTER) {
                printf("\"%c", node->type.character);
                printf("%p\"", &node->type.character);
                printf("[label=\"%c\"];\n", node->type.character);
        } else if (node->tag == BOOL) {
                printf("\"%i", node->type.boolean);
                printf("%p\"", &node->type.boolean);
                printf("[label=\"%i\"];\n", node->type.boolean);
        } else if (node->tag == UNARY_OP) {
                printf("\"%s", node->type.unary->op_name);
                printf("%p\"", &node->type.unary->op_name);
                printf("[label=\"%s\"];\n", node->type.unary->op_name);
        } else if (node->tag == BINARY_OP || node->tag == START) {
                printf("\"%s", node->type.binary->op_name);
                printf("%p\"", &node->type.binary->op_name);
                printf("[label=\"%s\"];\n", node->type.binary->op_name);
        } else {
                log_err("OPPS! Unknown node type to list!");
                return -1;
        }

        return 0;
}

static int dot_graph_print_node(struct ast *node)
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
                printf("%p", &node->type.binary->op_name);
        } else {
                log_err("OPPS! Unknown node type to print!");
                return -1;
        }

        return 0;
}

static int dot_graph_print_relation(
        struct ast *from,
        struct ast *to,
        struct linked_list *node_list
)
{
        /* print */
        dot_graph_print_node(from);
        printf(" -> ");
        dot_graph_print_node(to);
        printf(";\n");

        /* push node to list */
        linked_list_push(node_list, to);

        return 0;
}

static int print_tree_structure(struct ast *node, struct linked_list *node_list)
{
        int res = 0;

        if (node->tag == BINARY_OP || node->tag == START) {
                res = dot_graph_print_relation(node, node->type.binary->left, node_list);
                silent_check(res == 0);

                res = print_tree_structure(node->type.binary->left, node_list);
                check(res == 0, "Failed to parse the left binary op!");

                res = dot_graph_print_relation(node, node->type.binary->right, node_list);
                silent_check(res == 0);

                res = print_tree_structure(node->type.binary->right, node_list);
                check(res == 0, "Failed to parse the right binary op!");
        }

        return res;
error:
        return res;
}

int print_gp_tree_structure(struct ast *node)
{
        int i = 0;
        struct linked_list *node_list;
        struct ast *n;
        int node_count;

        /* setup */
        node_list = linked_list_create();
        linked_list_push(node_list, node);

        /* print nodes and relationships */
        print_tree_structure(node, node_list);
        printf("\n");

        /* print node list */
        node_count = node_list->count;
        for (i = 0; i < node_count; i++) {
                n = linked_list_pop(node_list);
                dot_graph_list_node(n);
        }

        /* clean up */
        linked_list_destroy(node_list);

        return 0;
}
