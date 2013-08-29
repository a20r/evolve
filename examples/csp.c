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

#define NUM_LBOUND 49
#define NUM_UBOUND 52
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))


/* GLOBAL VARS */
/* int k = 5;  #<{(| number of elements |)}># */
/* int len = 100;  #<{(| length of each element |)}># */
/* char *solutions[5]; */
/* char *largest; */
/* char *smallest; */
/* float min_hd; */
/* volatile sig_atomic_t stop_signal = 0; */
/*  */
/*  */
/* static void signal_handler(int signnum) */
/* { */
/*         printf("SIGNAL!!\n"); */
/*         stop_signal = 1; */
/* } */
/*  */
/* static char *randnumstr(int length) */
/* { */
/*         int i = 0; */
/*         int num_index = 0; */
/*         char num[10]; */
/*         char *r_num = malloc(sizeof(char *) * length); */
/*  */
/*         check_mem(r_num); */
/*         memset(r_num, '\0', sizeof(char *) * length); */
/*  */
/*         #<{(| create num array |)}># */
/*         for (i = NUM_LBOUND; i <= NUM_UBOUND; i++) { */
/*                 num[num_index] = i; */
/*                 num_index++; */
/*         } */
/*  */
/*         #<{(| generate random ASCII char |)}># */
/*         for (i = 0; i < length; i++) { */
/*                 r_num[i] = num[randnum_i(0, num_index - 1)]; */
/*         } */
/*         r_num[length] = '\0';  #<{(| add null terminator for safety |)}># */
/*  */
/*         return r_num; */
/* error: */
/*         if (r_num) free(r_num); */
/*         return NULL; */
/* } */
/*  */
/* static void mutate_numstr(char **str) */
/* { */
/*         int index = randnum_i(0, strlen(*str) - 1); */
/*         char num[4] = { '1', '2', '3', '4' }; */
/*         char new_num; */
/*  */
/*         #<{(| if new_num is same as previous, mutate again |)}># */
/*         while((new_num = num[randnum_i(0, 3)]) == (*str)[index]) {} */
/*  */
/*         (*str)[index] = new_num; */
/* } */
/*  */
/* static float min_edit_dist(char *s1, char *s2) */
/* { */
/*         unsigned int s1_len = strlen(s1); */
/*         unsigned int s2_len = strlen(s2); */
/*         unsigned int x; */
/*         unsigned int y; */
/*         unsigned int last_diag; */
/*         unsigned int old_diag; */
/*         unsigned int column[s1_len + 1]; */
/*  */
/*         for (y = 1; y <= s1_len; y++) { */
/*                 column[y] = y; */
/*         } */
/*  */
/*         for (x = 1; x <= s2_len; x++) { */
/*                 column[0] = x; */
/*                 for (y = 1, last_diag = x - 1; y <= s1_len; y++) { */
/*                         old_diag = column[y]; */
/*                         column[y] = MIN3( */
/*                                 column[y] + 1, */
/*                                 column[y - 1] + 1, */
/*                                 last_diag + (s1[y - 1] == s2[x - 1] ? 0 : 1) */
/*                         ); */
/*                         last_diag = old_diag; */
/*                 } */
/*         } */
/*  */
/*         return (column[s1_len]); */
/* } */
/*  */
/* static void insertion_sort_strings( */
/*         char *arr[], */
/*         int end, */
/*         int elements */
/* ) */
/* { */
/*         int j = 0; */
/*         char *value; */
/*  */
/*         for (j = 1; j <= elements - 1; j++) { */
/*                 int i = j - 1; */
/*                 value = arr[j]; */
/*  */
/*                 while (i >= 0 && str_cmp((void *) arr[i], (void *) value) > 0) { */
/*                         arr[i + 1] = arr[i]; */
/*                         i--; */
/*                 } */
/*  */
/*                 arr[i + 1] = value; */
/*         } */
/* } */
/*  */
/*  */
/* static void insertion_sort_ptr( */
/*         float arr[], */
/*         int end, */
/*         int elements */
/* ) */
/* { */
/*         int j = 0; */
/*         float value; */
/*  */
/*         for (j = 1; j <= elements - 1; j++) { */
/*                 int i = j - 1; */
/*                 value = arr[j]; */
/*  */
/*                 while (i >= 0 && arr[i] > value) { */
/*                         arr[i + 1] = arr[i]; */
/*                         i--; */
/*                 } */
/*  */
/*                 arr[i + 1] = value; */
/*         } */
/* } */
/*  */
/* static void setup_solutions(int k, int len) */
/* { */
/*         int i; */
/*         int j; */
/*         float min_hd; */
/*         float scores[15]; */
/*  */
/*         #<{(| initialize closest string solutions |)}># */
/*         for (i = 0; i < k; i++) { */
/*                 solutions[i] = randnumstr(len); */
/*                 printf("s[%d]: %s\n", i, solutions[i]); */
/*         } */
/*  */
/*         int index = 0; */
/*         for (i = 0; i < k; i++) { */
/*                 for (j = i + 1; j < k; j++) { */
/*                         scores[index] = min_edit_dist(solutions[i], solutions[j]); */
/*                         index++; */
/*                 } */
/*         } */
/*  */
/*         #<{(| find theoretical minimal hamming diameter |)}># */
/*         #<{(| insertion_sort_strings(solutions, k - 1, k); |)}># */
/*         #<{(| largest = solutions[k - 1]; |)}># */
/*         #<{(| smallest = solutions[0]; |)}># */
/*         #<{(| min_hd = min_edit_dist(largest, smallest) / 2; |)}># */
/*  */
/*         #<{(| printf("Largest string: %s\n", largest); |)}># */
/*         #<{(| printf("Smallest string: %s\n", smallest); |)}># */
/*         #<{(| printf("Theoretical Minimum Hamming Distance: %.2f\n", min_hd); |)}># */
/* } */
/*  */
/* static void teardown_solutions() */
/* { */
/*         int i = 0; */
/*  */
/*         for (i = 0; i < k; i++) { */
/*                 free(solutions[i]); */
/*         } */
/* } */
/*  */
/* static float fitness_function(char *chromosome) */
/* { */
/*         float score_1; */
/*         float score_2; */
/*         float max_score = 52 * len + 1; */
/*  */
/*         #<{(| evaulate closest string |)}># */
/*         score_1 = min_edit_dist(chromosome, largest); */
/*         score_2 = min_edit_dist(chromosome, smallest); */
/*  */
/*         return max_score - (score_1 - score_2); */
/* } */
/*  */
/* static void print_evolve_results(struct population *p) */
/* { */
/*         if (p->solution != NULL) { */
/*                 printf("SUCCESS! FOUND SOLUTION!\n"); */
/*                 printf("Solution: %s\n", p->solution); */
/*                 printf("Score: %.2f\n", fitness_function(p->solution)); */
/*                 printf("Goal: %.2f\n", p->goal); */
/*                 printf("Took %d Generations\n", p->curr_generation); */
/*         } else { */
/*                 printf("Failed to find solution . . . \n"); */
/*         } */
/* } */
/*  */
/* static void print_top_chromosomes(struct evolve_monitor *m, int top) */
/* { */
/*         int i = 0; */
/*  */
/*         #<{(| sort results |)}># */
/*         printf("SORTING RESULTS!\n"); */
/*         sort_generation_stats(m, float_cmp_desc); */
/*  */
/*         #<{(| print top 5 chromosomes |)}># */
/*         if (m->generations->end >= 5) { */
/*                 printf("\nTOP %d CHROMOSOMES:\n", top); */
/*                 for (i = 0; i < top; i++) { */
/*                         printf( */
/*                                 "chromosome: %s\n", */
/*                                 (char *) darray_get(m->best_chromosomes, i) */
/*                         ); */
/*                         printf( */
/*                                 "score: %.2f\n", */
/*                                 *(float *) darray_get(m->best_scores, i) */
/*                         ); */
/*                         printf( */
/*                                 "generation: %d\n", */
/*                                 *(int *) darray_get(m->generations, i) */
/*                         ); */
/*                         printf( */
/*                                 "convergence rate: %.2f\n", */
/*                                 *(float *) darray_get(m->convergence_rates, i) */
/*                         ); */
/*                         printf( */
/*                                 "goal distance: %.2f\n\n", */
/*                                 *(float *) darray_get(m->goal_distances, i) */
/*                         ); */
/*                 } */
/*         } */
/* } */

int main(int argc, char *argv[])
{
        /* setup_solutions(k, len); */
        /* int max_pop = 10; */
        /* int max_gen = 10; */
        /* float p_c = (argv[1] == NULL) ? 0.8 : atof(argv[1]); */
        /* float p_m = (argv[1] == NULL) ? 0.1 : atof(argv[2]); */

        /* #<{(| seed random - VERY IMPORTANT! |)}># */
        /* srand(time(NULL)); */

        /* #<{(| setup |)}># */
        /* setup_solutions(k, len); */
        /* sleep(2); */
        /* printf("Running!\n"); */
        /* signal(SIGINT, signal_handler); */


        /* #<{(| initialize evolution |)}># */
        /* struct population *p = init_population( */
        /*         len,  #<{(| param |)}># */
        /*         #<{(| 52 * len,  #<{(| goal |)}># |)}># */
        /*         0, */
        /*         max_pop,  #<{(| max_pop |)}># */
        /*         max_gen  #<{(| max_gen |)}># */
        /* ); */
        /* struct evolve_monitor *m = init_evolve_monitor( */
        /*         p->chromosomes->element_size,  #<{(| chromosome size |)}># */
        /*         max_gen, */
        /*         "cloest_string_problem.dat" */
        /* ); */

        /* #<{(| run evolution |)}># */
        /* printf("RUNNING GA!\n"); */
        /* printf("Crossover Probability [%.2f]!\n", p_c); */
        /* printf("Mutation Probability [%.2f]!\n", p_m); */
        /* gen_init_chromosomes(&p, randnumstr); */
        /* run_evolution( */
        /*         &p, */
        /*         fitness_function, */

        /*         #<{(| selection |)}># */
        /*         roulette_wheel_selection, */
        /*         DEFAULT_SELECT, */

        /*         #<{(| crossover |)}># */
        /*         one_pt_crossover, */
        /*         p_c, #<{(| crossover probability |)}># */
        /*         DEFAULT_PIVOT, */

        /*         #<{(| mutation |)}># */
        /*         mutate_numstr, */
        /*         p_m, #<{(| mutation probability |)}># */

        /*         m, */
        /*         1, */
        /*         &stop_signal */
        /* ); */

        /* #<{(| print results |)}># */
        /* print_evolve_results(p); */
        /* print_top_chromosomes(m, 5); */

        /* #<{(| clean up |)}># */
        /* destroy_evolve_monitor(&m); */
        /* destroy_population(&p); */
        /* teardown_solutions(); */

        return 0;
}
