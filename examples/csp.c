#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include <al/utils.h>
#include <al/comparator.h>
#include <al/sort.h>
#include <dstruct/darray.h>

#include "selection.h"
#include "crossover.h"
#include "mutation.h"
#include "evolve.h"
#include "evolve_monitor.h"

#define MAXN 100
#define NUM_LBOUND 49
#define NUM_UBOUND 52
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))


/* GLOBAL VARS */
int k = 4;  /* number of elements */
int len = 3;  /* length of each element */
char *strings[4];
char *largest;
char *smallest;
float min_hd;
volatile sig_atomic_t stop_signal = 0;


static void signal_handler(int signnum)
{
        printf("SIGNAL %d!!\n", signnum);
        stop_signal = 1;
}

static char *randnumstr(int length)
{
        int i = 0;
        int num_index = 0;
        char num[10];
        char *r_num = malloc(sizeof(char *) * length);

        check_mem(r_num);
        memset(r_num, '\0', sizeof(char *) * length);

        /* create num array */
        for (i = NUM_LBOUND; i <= NUM_UBOUND; i++) {
                num[num_index] = i;
                num_index++;
        }

        /* generate random ASCII char */
        for (i = 0; i < length; i++) {
                r_num[i] = num[randnum_i(0, num_index - 1)];
        }
        r_num[length] = '\0';  /* add null terminator for safety */

        return r_num;
error:
        if (r_num) free(r_num);
        return NULL;
}

static void mutate_numstr(char **str)
{
        int index = randnum_i(0, strlen(*str) - 1);
        char num[4] = { '1', '2', '3', '4' };
        char new_num;

        /* if new_num is same as previous, mutate again */
        while((new_num = num[randnum_i(0, 3)]) == (*str)[index]) {}

        (*str)[index] = new_num;
}

static float hamming_dist(char *s1, char *s2)
{
        int i;
        int len = strlen(s1);
        float dist = 0.0;

        for (i = 0; i < len; i++) {
                if (s1[i] != s2[i]) {
                        dist += 1;
                }
        }

        return dist;
}

static void insertion_sort_scores(
        float *arr,
        char *combinations[],
        int elements
)
{
        int j = 0;
        float value;
        char *combination;

        for (j = 1; j <= elements - 1; j++) {
                int i = j - 1;
                value = arr[j];
                combination = combinations[j];

                while (i >= 0 && arr[i] > value) {
                        arr[i + 1] = arr[i];
                        combinations[i + 1] = combinations[i];
                        i--;
                }

                arr[i + 1] = value;
                combinations[i + 1] = combination;
        }
}

static int binomial_coefficient(n, m)
{
        int i;
        int j;
        long bc[MAXN][MAXN];    /* table of binomial coefficient values */

        for (i = 0; i <= n; i++) {
                bc[i][0] = 1;
        }

        for (j = 0; j <= n; j++) {
                bc[j][j] = 1;
        }

        for (i = 1; i <= n; i++) {
                for (j = 1; j < i; j++) {
                        bc[i][j] = bc[i - 1][j - 1] + bc[i - 1][j];
                }
        }

        return bc[n][m];
}

static void setup_strings(int k, int len)
{
        int i;
        int j;
        int scores_len = binomial_coefficient(k, 2);
        float scores[scores_len];
        char *combinations[scores_len];
        char *cache;

        /* initialize closest string strings */
        for (i = 0; i < k; i++) {
                strings[i] = randnumstr(len);
                printf("s[%d]: %s\n", i, strings[i]);
        }

        /* find min edit dist between all strings */
        int index = 0;
        for (i = 0; i < k; i++) {
                for (j = i + 1; j < k; j++) {
                        /* calculate min edit distance */
                        scores[index] = hamming_dist(strings[i], strings[j]);

                        /* make combination string */
                        cache = calloc(1, (sizeof(char) * len * 2) + 1);
                        strcpy(cache, strings[i]);
                        strcat(cache, strings[j]);

                        /* record combination string */
                        combinations[index] = cache;
                        index++;
                }
        }

        /* sort by score */
        insertion_sort_scores(scores, combinations, scores_len);
        for (i = 0; i < scores_len; i++) {
                printf("scores:[%f] combination:[%s]\n", scores[i], combinations[i]);
        }

        /* find theoretical minimal hamming diameter */
        largest = calloc(1, (sizeof(char) * len * 2) + 1);
        smallest = calloc(1, (sizeof(char) * len * 2) + 1);
        strcpy(largest, combinations[scores_len - 1]);
        strcpy(smallest, combinations[0]);
        min_hd = fabs((scores[0] - scores[scores_len - 1]) / 2);
        printf("Largest combination: %s\n", largest);
        printf("Smallest combination: %s\n", smallest);
        printf("Theoretical Minimum Hamming Distance: %.2f\n", min_hd);

        /* clean up */
        for (i = 0; i < scores_len; i++) {
                free(combinations[i]);
        }
}

static void teardown_strings()
{
        int i;

        for (i = 0; i < k; i++) {
                free(strings[i]);
        }

        free(largest);
        free(smallest);
}

static float fitness_function(char *chromosome)
{
        int i;
        float score = 0.0;
        float max_score = len * k;

        /* evaulate closest string */
        for (i = 0; i < k; i++) {
                score += hamming_dist(chromosome, strings[i]);
        }

        return max_score - score;
}

static void print_evolve_results(struct population *p)
{
        if (p->solution != NULL) {
                printf("SUCCESS! FOUND SOLUTION!\n");
                printf("Solution: %s\n", p->solution);
                printf("Score: %.2f\n", fitness_function(p->solution));
                printf("Goal: %.2f\n", p->goal);
                printf("Took %d Generations\n", p->curr_generation);
        } else {
                printf("Failed to find solution . . . \n");
        }
}

static void print_top_chromosomes(struct evolve_monitor *m, int top)
{
        int i = 0;

        /* sort results */
        printf("SORTING RESULTS!\n");
        sort_generation_stats(m, float_cmp_desc);

        /* print top chromosomes */
        if (m->top >= top) {
                printf("\nTOP %d CHROMOSOMES:\n", top);
                for (i = 0; i < top; i++) {
                        printf(
                                "chromosome: %s\n",
                                (char *) darray_get(m->best_chromosomes, i)
                        );
                        printf(
                                "score: %.2f\n",
                                *(float *) darray_get(m->best_scores, i)
                        );
                        printf(
                                "generation: %d\n",
                                *(int *) darray_get(m->generations, i)
                        );
                        printf(
                                "convergence rate: %.2f\n",
                                *(float *) darray_get(m->convergence_rates, i)
                        );
                        printf(
                                "goal distance: %.2f\n\n",
                                *(float *) darray_get(m->goal_distances, i)
                        );
                }
        }
}

int main(int argc, char *argv[])
{
        int max_pop = 6;
        int max_gen = 10;
        float p_c = (argv[1] == NULL) ? 0.0 : atof(argv[1]);
        float p_m = (argv[1] == NULL) ? 0.8 : atof(argv[2]);

        if (argc != 2) {
                printf("missing arguments, using default settings!\n");
        }

        /* seed random - VERY IMPORTANT! */
        srand(time(NULL));

        /* setup */
        setup_strings(k, len);
        printf("Running!\n");
        signal(SIGINT, signal_handler);


        /* initialize evolution */
        struct population *p = init_population(
                len,  /* param */
                len * k,  /* goal */
                max_pop,  /* max_pop */
                max_gen  /* max_gen */
        );
        struct evolve_monitor *m = init_evolve_monitor(
                p->chromosomes->element_size,  /* chromosome size */
                5,
                "cloest_string_problem.dat"
        );

        /* run evolution */
        printf("RUNNING GA!\n");
        printf("Crossover Probability [%.2f]!\n", p_c);
        printf("Mutation Probability [%.2f]!\n", p_m);
        gen_init_chromosomes(&p, randnumstr);
        run_evolution(
                &p,
                fitness_function,

                /* selection */
                roulette_wheel_selection,
                DEFAULT_SELECT,

                /* crossover */
                one_pt_crossover,
                p_c, /* crossover probability */
                DEFAULT_PIVOT,

                /* mutation */
                mutate_numstr,
                p_m, /* mutation probability */

                m,
                0,
                &stop_signal
        );

        /* print results */
        print_evolve_results(p);
        print_top_chromosomes(m, 5);

        /* clean up */
        destroy_evolve_monitor(&m);
        destroy_population(&p);
        teardown_strings();

        return 0;
}
