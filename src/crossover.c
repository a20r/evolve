#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <dbg/dbg.h>

#include "crossover.h"
#include "evolve.h"


int one_ptr_crossover(struct chromosome_pair **c_pair, int index)
{
        int len = (int) strlen((*c_pair)->child_1) + 1;
        int i = (index == DEFAULT_PIVOT ? round(len / 2) - 1 : index);
        char *c_1 = calloc(1, sizeof(char) * len);
        char *c_2 = calloc(1, sizeof(char) * len);

        /* set c_1 and c_2 */
        memcpy(c_1, (*c_pair)->child_1, sizeof(char) * len);
        memcpy(c_2, (*c_pair)->child_2, sizeof(char) * len);
        check(strlen(c_1) == strlen(c_2), "c_1 and c_2 are not same the len!");

        /* pivot at pivot point and swap chromosomes */
        memcpy((*c_pair)->child_1, c_1, sizeof(char) * i);
        memcpy((*c_pair)->child_2, c_2, sizeof(char) * i);
        memcpy((*c_pair)->child_1 + i, c_2 + i, sizeof(char) * (len - i));
        memcpy((*c_pair)->child_2 + i, c_1 + i, sizeof(char) * (len - i));

        /* add null terminator for safety */
        memcpy((*c_pair)->child_1 + len, "\0", sizeof(char));
        memcpy((*c_pair)->child_2 + len, "\0", sizeof(char));

        /* clean up */
        free(c_1);
        free(c_2);
        return 0;
error:
        free(c_1);
        free(c_2);
        return -1;
}
