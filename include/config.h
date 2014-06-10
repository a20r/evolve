#ifndef __CONFIG__
#define __CONFIG__

/* STRUCTS */
struct evolve_config
{
    int max_generations;
    int stale_limit;
    float target_score;
};

struct selection_config
{
    int method;

    int tournament_size;
    int (*cmp)(const void *, const void *);
    void *(*copy_func)(void *);
};

/* FUNCTIONS */
struct evolve_config *evolve_config_new(void);
void evolve_config_destroy(void *config);
struct selection_config *selection_config_new(int method);
void selection_config_destroy(void *config);


#endif
