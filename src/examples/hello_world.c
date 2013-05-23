#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "evolve.h"

float fitness_function(char *chromosome)
{
        char *target = "hello world!";
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(chromosome); i++) {
                total += pow(roundf(target[i] - chromosome[i]), 2);
        }

        return total;
}

int main()
{
        /* initialize mitosis */
        struct evolve *e = init_evolve(
                (int) strlen("hello world!"),  /* param */
                0.0,  /* goal */
                10,  /* max_pop */
                5000  /* max_gen */
        );

        srand(time(NULL));  /* seed random - VERY IMPORTANT! */
        init_str_chromosomes(&e, rand_str);  /* init random chromosomes */

        int i = 0;
        for (i = 0; i < 10; i++) {
                printf("%d: %s\n", i + 1, darray_get(e->chromosomes, i));
        }

        struct chromosome_pair *c_pair = str_mate(
                "hello world!",
                "world hello!"
        );
        printf("before mutation [child_1]: %s\n", c_pair->child_1);
        printf("before mutation [child_2]: %s\n", c_pair->child_2);
        mutate_gene_str(&c_pair, 0, str_mutator);
        printf("after mutation [child_1]: %s\n", c_pair->child_1);
        printf("after mutation [child_2]: %s\n", c_pair->child_2);


        /* clean up */
        darray_clear_destroy(e->chromosomes);
        free(e);

        free(c_pair->child_1);
        free(c_pair->child_2);
        free(c_pair);



        return 0;
}
