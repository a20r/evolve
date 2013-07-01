#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <dbg/dbg.h>

#include "crossover.h"
#include "evolve.h"
#include "utils.h"


int one_ptr_crossover(void **chromo_1, void **chromo_2, int index)
{
        /* debug("chromo_1: %s", chromo_1); */
        /* debug("strlen chromo_1: %d", strlen(chromo_1)); */
        int len = (int) strlen(*chromo_1) + 1;
        int i = (index == DEFAULT_PIVOT ? round(len / 2) - 1 : index);


        check(
                strlen(*chromo_1) == strlen(*chromo_2),
                "Chromosome lengths are different!"
        );

        /* set child_1 and child_2 */
        char *child_1 = calloc(1, sizeof(char) * (len));
        char *child_2 = calloc(1, sizeof(char) * (len));
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
        free(child_1);
        free(child_2);
        return 0;
error:
        printf("chromo 1: %s\n", (char *) *chromo_1);
        printf("chromo 2: %s\n", (char *) *chromo_2);
        printf("strlen chromo 1: %zu\n", strlen(*chromo_1));
        printf("strlen chromo 2: %zu\n", strlen(*chromo_2));


        char *c_1 = (char *) chromo_1;
        char *c_2 = (char *) chromo_2;

        printf("chromo 1\n");
        for (i = 0; i < strlen(*chromo_1); i++) {
                printf("%d-", c_1[i]);
        }
        printf("\n");

        printf("chromo 2\n");
        for (i = 0; i < strlen(*chromo_1); i++) {
                printf("%d-", c_2[i]);
        }
        printf("\n");


        exit(-1);
        return -1;
}

void crossover(
        void **child_1,
        void **child_2,
        int index,
        int (*crossover_func)(void **chromo_1, void **chromo_2, int index),
        float crossover_prob
)
{
        if (crossover_prob > randnum_f(0.0, 1.0)) {
                crossover_func(&(*child_1), &(*child_2), index);
        }
}
