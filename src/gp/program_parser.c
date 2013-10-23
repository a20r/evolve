#include <stdlib.h>

#include "gp/program_parser.h"

int program_traverse_cb(struct ast *node)
{

        return 0;
}

int parse_program(struct ast *tree)
{
        ast_traverse(tree, program_traverse_cb);
}
