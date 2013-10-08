#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <al/utils.h>
#include <dbg/dbg.h>

#include "ga/evolve.h"
#include "ga/crossover.h"


int one_pt_crossover(void **chromo_1, void **chromo_2, int index)
{
        int len = (int) strlen(*chromo_1) + 1;
        int i = (index == DEFAULT_PIVOT ? round(len / 2) : index);
        char *child_1 = calloc(1, sizeof(char) * (len));
        char *child_2 = calloc(1, sizeof(char) * (len));

        check(
                strlen(*chromo_1) == strlen(*chromo_2),
                "Chromosome lengths are different!"
        );

        /* set child_1 and child_2 */
        memcpy(child_1, *chromo_1, sizeof(char) * len);
        memcpy(child_2, *chromo_2, sizeof(char) * len);

        /* pivot at pivot point and swap chromosomes */
        memcpy(child_1, *chromo_1, sizeof(char) * i);
        memcpy(child_2, *chromo_2, sizeof(char) * i);
        memcpy(child_1 + i, *chromo_2 + i, sizeof(char) * (len - i));
        memcpy(child_2 + i, *chromo_1 + i, sizeof(char) * (len - i));

        /* add null terminator for safety */
        memset(child_1 + len - 1, '\0', sizeof(char));
        memset(child_2 + len - 1, '\0', sizeof(char));

        /* replace old chromo with new */
        memcpy(*chromo_1, child_1, sizeof(char) * len);
        memcpy(*chromo_2, child_2, sizeof(char) * len);

        /* clean up */
        if (child_1) free(child_1);
        if (child_2) free(child_2);
        return 0;
error:
        /* clean up */
        if (child_1) free(child_1);
        if (child_2) free(child_2);
        return -1;
}

int two_pt_crossover(void **chromo_1, void **chromo_2, int index)
{
        int result = 0;
        int len = (int) strlen(*chromo_1) + 1;
        int i_1 = index == DEFAULT_PIVOT ? round(len / 3) : index;
        int i_2 = index == DEFAULT_PIVOT ? round(len * 2 / 3) : len * 2 / 3;

        result = one_pt_crossover(&(*chromo_1), &(*chromo_2), i_1);
        result = one_pt_crossover(&(*chromo_1), &(*chromo_2), i_2);

        return result;
}

void crossover(
        void **child_1,
        void **child_2,
        int index,
        int (*crossover_func)(void **chromo_1, void **chromo_2, int index),
        float *crossover_prob
)
{
        if (*crossover_prob > randnum_f(0.0, 1.0)) {
                crossover_func(&(*child_1), &(*child_2), index);
        }
}
