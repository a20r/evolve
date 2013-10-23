#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <al/utils.h>
#include <al/comparator.h>
#include <dstruct/darray.h>

#include "evolve.h"
#include "evolve_monitor.h"
#include "population.h"
#include "selection.h"
#include "ga/initialize.h"
#include "ga/crossover.h"
#include "ga/mutation.h"

#include "example_utils.h"

#define TARGET_SOLUTION "hello world!"


static float fitness_function(char *individual)
{
        char *target = TARGET_SOLUTION;
        float max_score = 122 * strlen(individual);
        float total = 0;
        int i = 0;

        for (i = 0; i <= (int) strlen(individual); i++) {
                total += fabsf(roundf(target[i] - individual[i]));
        }

        return max_score - total;
}

int main(int argc, char *argv[])
{
        volatile sig_atomic_t stop_signal = 0;
        struct population *p;
        struct evolve_monitor *m;
        int max_pop = 100;
        int max_gen = 5000;
        float *p_c = calloc(1, sizeof(float));
        float *p_m = calloc(1, sizeof(float));

        /* setup */
        *p_c = (argv[1] == NULL) ? 0.5 : atof(argv[1]);
        *p_m = (argv[1] == NULL) ? 0.1 : atof(argv[2]);
        if (argc != 2) {
                printf("missing arguments, using default settings!\n");
        }

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* initialize evolution */
        p = init_population(
                (int) strlen(TARGET_SOLUTION),  /* param */
                122 * strlen(TARGET_SOLUTION),  /* goal */
                max_pop,  /* max_pop */
                max_gen  /* max_gen */
        );
        m = init_evolve_monitor(
                p->individuals->element_size,  /* individual size */
                5,
                NULL
        );

        /* run evolution */
        printf("RUNNING GA!\n");
        printf("Crossover Probability [%.4f]!\n", *p_c);
        printf("Mutation Probability [%.4f]!\n", *p_m);
        init_individuals(&p, randstr);
        run_evolution(
                &p,
                fitness_function,

                /* selection */
                tournament_selection,
                DEFAULT_SELECT,

                /* crossover */
                one_pt_crossover,
                p_c, /* crossover probability */
                DEFAULT_PIVOT,

                /* mutation */
                mutate_str,
                p_m , /* mutation probability */

                m,
                0,
                &stop_signal
        );

        /* print results */
        print_evolve_results(p, fitness_function);
        print_top_individuals(m, 5);

        /* clean up */
        destroy_evolve_monitor(&m);
        destroy_population(&p);
        free(p_c);
        free(p_m);

        return 0;
}
