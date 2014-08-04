#ifndef _CARTESIAN_H_
#define _CARTESIAN_H_


/* ERROR MSGS */
#define CARTESIAN_ADDR_INVALID "Error invalid address!"
#define CARTESIAN_ADDR_IS_INPUT "Error address is input!"


/* STRUCTS */
struct cartesian_config
{
    /* graph details */
    int rows;
    int columns;
    int levels_back;
    int max_arity;

    int num_inputs;
    int num_outputs;

    /* data details */
    int **functions;
    int functions_length;
    int *terminals;
};

struct cartesian
{
    float *score;

    /* graph details */
    int rows;
    int columns;

    int levels_back;
    int max_arity;

    int num_inputs;
    int num_outputs;

    /* graph elements */
    int *graph;
    int *inputs;
    int *outputs;

    int **address_grid;
};


/* FUNCTIONS */
struct cartesian_config *cartesian_config_new(void);
void cartesian_config_destroy(void *cc);

struct cartesian *cartesian_new(struct cartesian_config *cc);
void cartesian_destroy(void *c);
void cartesian_print(struct cartesian *c);
char *cartesian_string(struct cartesian *c);
int **cartesian_address_grid(struct cartesian *c);
struct cartesian *cartesian_generate(struct cartesian_config *cc);

#endif
