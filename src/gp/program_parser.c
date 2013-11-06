#include <stdio.h>
#include <stdlib.h>

#include <dbg/dbg.h>
#include "gp/program_parser.h"


int dot_graph_print(struct ast *from, struct ast *to)
{
        int *integer;
        float *real;
        char *string = "";
        char *character;
        int boolean;

        char *src = "";

        if (from->tag == UNARY_OP) {
                src = from->type.unary->op_name;

                if (to->tag == INTEGER) {
                        integer = &to->type.integer;
                        printf("%s%p", src, &src);
                        printf(" -> ");
                        printf("%d%p\n", *integer, &integer);
                } else if (to->tag == REAL) {
                        real = &to->type.real;
                        printf("%s%p", src, &src);
                        printf(" -> ");
                        printf("%f%p\n", *real, &real);
                } else if (to->tag == STRING) {
                        printf("%s%p", src, &src);
                        printf(" -> ");
                        printf("%s%p\n", string, &string);
                } else if (to->tag == CHARACTER) {
                        character = &to->type.character;
                        printf("%s%p", src, &src);
                        printf(" -> ");
                        printf("%c%p\n", *character, &character);
                } else if (to->tag == BOOL) {
                        boolean = to->type.boolean;
                        printf("%s%p", src, &src);
                        printf(" -> ");
                        printf("%i%p\n", boolean, &boolean);
                }

        } else if (from->tag == BINARY_OP || from->tag == START) {
                src = from->type.binary->op_name;

                if (to->tag == INTEGER) {
                        integer = &to->type.integer;
                        printf("%s%p", src, &from->type.binary->op_name);
                        printf(" -> ");
                        printf("%d%p\n", *integer, &to->type.integer);
                } else if (to->tag == REAL) {
                        real = &to->type.real;
                        printf("%s%p", src, &from->type.binary->op_name);
                        printf(" -> ");
                        printf("%f%p\n", *real, &to->type.real);
                } else if (to->tag == STRING) {
                        string = to->type.string;
                        printf("%s%p", src, &from->type.binary->op_name);
                        printf(" -> ");
                        printf("%s%p\n", string, &to->type.string);
                } else if (to->tag == CHARACTER) {
                        character = &to->type.character;
                        printf("%s%p", src, &from->type.binary->op_name);
                        printf(" -> ");
                        printf("%c%p\n", *character, &to->type.character);
                } else if (to->tag == BOOL) {
                        boolean = to->type.boolean;
                        printf("%s%p", src, &from->type.binary->op_name);
                        printf(" -> ");
                        printf("%i%p\n", boolean, &to->type.boolean);
                } else if (to->tag == UNARY_OP) {
                        string = to->type.unary->op_name;
                        printf("%s%p", src, &from->type.binary->op_name);
                        printf(" -> ");
                        printf("%s%p\n", string, &to->type.unary->op_name);
                } else if (to->tag == BINARY_OP) {
                        string = to->type.binary->op_name;
                        printf("%s%p", src, &from->type.binary->op_name);
                        printf(" -> ");
                        printf("%s%p\n", string, &to->type.binary->op_name);
                }
        }

        return 0;
}

int parse_program(struct ast *node)
{
        int res = 0;

        if (node->tag == UNARY_OP) {
                dot_graph_print(node, node->type.unary->value);
                res = parse_program(node->type.unary->value);
                check(res == 0, "Failed to parse unary op!");

        } else if (node->tag == BINARY_OP || node->tag == START) {
                res = dot_graph_print(node, node->type.binary->left);
                silent_check(res == 0);

                res = parse_program(node->type.binary->left);
                check(res == 0, "Failed to parse the left binary op!");

                res = dot_graph_print(node, node->type.binary->right);
                silent_check(res == 0);

                res = parse_program(node->type.binary->right);
                check(res == 0, "Failed to parse the right binary op!");
        }


        return res;
error:
        return res;
}

static void write_includes(FILE *program_file)
{
        fprintf(program_file, "#include <stdio.h>\n");
}

static void write_main(FILE *program_file)
{
        /* header of main */
        fprintf(program_file, "int main()\n");
        fprintf(program_file, "{\n");

        /* body of main */


        /* footer of main */
        fprintf(program_file, "    return 0\n");
        fprintf(program_file, "}\n");
}

int generate_program(struct ast *node, char *fp)
{
        FILE *program_file = fopen(fp, "w");



        return 0;
}
