#include <stdlib.h>
#include <string.h>

#include <dstruct/ast.h>
#include <dbg/dbg.h>

#include "gp/terminal_set.h"

int node_is_terminal(struct ast *node)
{
        int i;
        enum ast_tag tags[5] = {
                INTEGER,
                REAL,
                STRING,
                CHARACTER,
                BOOL
        };

        for (i = 0; i < 5; i++) {
                if (node->tag == tags[i]) {
                        return 1;
                }
        }

        return 0;
}
