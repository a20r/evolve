#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <dbg/dbg.h>

#include "evolve.h"

int str_crossover(struct chromosome_pair **c_pair)
{
        int str_len = (int) strlen((*c_pair)->child_1) + 1;
        int pivot = round(str_len / 2) - 1;
        char *c_1 = malloc(sizeof(char) * str_len);
        char *c_2 = malloc(sizeof(char) * str_len);

        /* set c_1 and c_2 */
        strncpy(c_1, (*c_pair)->child_1, str_len);
        strncpy(c_2, (*c_pair)->child_2, str_len);

        check(
                strlen(c_1) == strlen(c_2),
                "chromosome 1 and 2 are not same length"
        );

        /* clear c_pair child 1 and 2 before pivoting */
        memset((*c_pair)->child_1, '\0', strlen(c_1));
        memset((*c_pair)->child_2, '\0', strlen(c_2));

        debug("pivot index: %d", pivot);
        debug("str_len: %d", str_len);
        debug("child_1 before pivot: %s", c_1);
        debug("child_2 before pivot: %s", c_2);

        /* pivot at the middle and swap chromosomes */
        strncpy((*c_pair)->child_1, c_1, pivot);
        strncpy((*c_pair)->child_1 + pivot, c_2 + pivot, str_len - pivot);
        strncpy((*c_pair)->child_2 , c_2, pivot);
        strncpy((*c_pair)->child_2 + pivot, c_1 + pivot, str_len - pivot);

        debug("child_1 after pivot: %s", (*c_pair)->child_1);
        debug("child_2 after pivot: %s", (*c_pair)->child_2);

        /* clean up */
        free(c_1);
        free(c_2);
        return 0;
error:
        free(c_1);
        free(c_2);
        return -1;
}
