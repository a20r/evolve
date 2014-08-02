#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gp/cartesian.h"
#include "utils.h"


/* CARTESIAN CONFIG */
struct cartesian_config *cartesian_config_new(void)
{
    struct cartesian_config *cc;

    cc = malloc(sizeof(struct cartesian_config));

    cc->rows = 0;
    cc->columns = 0;
    cc->levels_back = 0;
    cc->max_arity = 0;

    cc->num_inputs = 0;
    cc->num_outputs = 0;

    return cc;
}

void cartesian_config_destroy(void *cc)
{
    free(cc);
}



/* CARTESIAN */
struct cartesian *cartesian_new(struct cartesian_config *cc)
{
    struct cartesian *c = malloc(sizeof(struct cartestian *));

    c->score = NULL;

    /* graph details */
    c->rows = cc->rows;
    c->columns = cc->columns;
    c->levels_back = cc->levels_back;
    c->max_arity = cc->max_arity;

    c->num_inputs = cc->num_inputs;
    c->num_outputs = cc->num_outputs;

    /* graph elements */
    c->graph = NULL;
    c->inputs = NULL;
    c->outputs = NULL;
    c->address_grid = NULL;

    return c;
}

void cartesian_destroy(void *c)
{
    struct cartesian *cart = (struct cartesian *) c;

    free_mem(cart->graph, free);
    free_mem(cart->inputs, free);
    free_mem(cart->outputs, free);

    free(cart);
}

void cartesian_print(struct cartesian *c)
{
    int i;
    int graph_size = ((c->rows * c->columns) * c->max_arity);

    /* print inputs */
    for (i = 0; i < c->num_inputs; i++) {
        printf("[%d]", c->inputs[i]);
    }

    /* print graph */
    for (i = 0; i < graph_size; i += c->max_arity) {
        printf("[");
        printf("%d", c->inputs[i]);
        printf(" %d ", c->inputs[i + 1]);
        printf("%d", c->inputs[i + 2]);
        printf("]");
    }

    /* print outputs */
    for (i = 0; i < c->num_outputs; i++) {
        printf("[%d]", c->outputs[i]);
    }
}

/* char *cartesian_string(struct cartesian *c) */
/* { */
/*     int i; */
/*     char buffer[4096] = ""; */
/*     char *cartesian_string = NULL; */
/*     int graph_size = ((c->rows * c->columns) * c->max_arity); */
/*  */
/*     #<{(| build inputs string |)}># */
/*     for (i = 0; i < c->num_inputs; i++) { */
/*         strcat(buffer, "[%d]", c->inputs[i]); */
/*     } */
/*  */
/*     #<{(| build graph string |)}># */
/*     for (i = 0; i < graph_size; i += c->max_arity) { */
/*         strcat(buffer, "["); */
/*         strcat(buffer, "%d", c->inputs[i]); */
/*         strcat(buffer, " %d ", c->inputs[i + 1]); */
/*         strcat(buffer, "%d", c->inputs[i + 2]); */
/*         strcat(buffer, "]"); */
/*     } */
/*  */
/*     #<{(| build outputs string |)}># */
/*     for (i = 0; i < c->num_outputs; i++) { */
/*         strcat(buffer, "[%d]", c->outputs[i]); */
/*     } */
/*  */
/*     #<{(| copy and return |)}># */
/*     cartesian_string = malloc(sizeof(char) * strlen(buffer) + 1); */
/*     strcpy(cartesian_string, buffer); */
/*     return cartesian_string; */
/* } */

/* int **caretsian_address_grid(struct cartesian *c) */
/* { */
/*     int i; */
/*     int j; */
/*     int addr_index = 0; */
/*     int grid_size; */
/*     float **grid; */
/*  */
/*     grid_size = c->num_inputs + (c->rows * c->columns); */
/*     grid  = malloc(sizeof(int *) * grid_size); */
/*  */
/*     #<{(| inputs |)}># */
/*     grid[0] = malloc(sizeof(int) * c->num_inputs); */
/*     for (i = 0; i < c->num_inputs; i++) { */
/*         grid[0][i] = addr_index; */
/*         addr_index++; */
/*     } */
/*  */
/*     #<{(| graph |)}># */
/*     for (i = 1; i < c->columns; i++) { */
/*         grid[i] = malloc(sizeof(int) * c->rows); */
/*  */
/*         for (j = 0; j < c->rows; j++) { */
/*             grid[i][j] = addr_index; */
/*             addr_index++; */
/*         } */
/*     } */
/*  */
/*     return grid; */
/* } */

/* int cartesian_column_level(int address) */
/* { */
/*     int i; */
/*     int j; */
/*     int grid_size; */
/*     int **addr_grid; */
/*  */
/*     #<{(| pre-check |)}># */
/*     grid_size = c->num_inputs + (c->rows * c->columns); */
/*     check(address < grid_size, CARTESIAN_ADDR_INVALID); */
/*     check(address < c->num_inputs, CARTESIAN_ADDR_IS_INPUT); */
/*  */
/*     #<{(| calculate which column the address is |)}># */
/*     addr_grid = cartesian_address_grid(c); */
/*  */
/*     for (i = 0; i < c->columns; i++) { */
/*         for (j = 0; j < c->rows; j++) { */
/*             if (address == addr_grid[i][j]) { */
/*                 free_mem(addr_grid); */
/*                 return i; */
/*             } */
/*         } */
/*     } */
/*     free_mem(addr_grid); */
/*  */
/*     return -1; */
/* error: */
/*     free_mem(addr_grid); */
/*     return -2; */
/* } */
/*  */
/* int cartesian_random_func_gene(struct cartesian_config *cc) */
/* { */
/*  */
/*  */
/*     return 0; */
/* } */
/*  */
/* int cartesian_random_conn_gene(int from_level, struct cartesian_config *cc) */
/* { */
/*  */
/*     return 0; */
/* } */
/*  */
/* int *cartesian_random_function(int addr, int level, struct cartesian_config *cc) */
/* { */
/*     int i; */
/*     int *function; */
/*  */
/*     #<{(| create random function |)}># */
/*     function = malloc(sizeof(int) * (cc->max_arity + 1)); */
/*     function[0] = cartesian_random_func_gene(cc); */
/*     for (i = 1; i < cc->max_arity; i++) { */
/*         function[i] = cartesian_random_conn_gene(cc); */
/*     } */
/*  */
/*     return function; */
/* } */
/*  */
/* int cartesian_random_output(struct cartesian *c) */
/* { */
/*  */
/*     return 0; */
/* } */


/* struct cartesian *cartesian_generate(struct cartesian_config *cc) */
/* { */
/*     int i; */
/*     int rows = cc->rows; */
/*     int cols = cc->columns; */
/*     int max_arity = cc->max_arity; */
/*     int last_addr; */
/*     struct cartesian *c; */
/*  */
/*     #<{(| setup |)}># */
/*     last_addr = (cc->rows * cc->columns) + cc->num_outputs; */
/*  */
/*     c = cartesian_new(cc); */
/*     c->graph = malloc(sizeof(int) * (unsigned long) rows * cols * max_arity); */
/*     c->inputs = malloc(sizeof(int) * (unsigned long) cc->num_inputs); */
/*     c->outputs = malloc(sizeof(int) * (unsigned long) cc->num_outputs); */
/*  */
/*     #<{(| initialize graph |)}># */
/*     for (i = cc->num_inputs; i < last_addr; i++) { */
/*         if (i <= (last_addr - cc->num_outputs)) { */
/*             #<{(| intialize function |)}># */
/*  */
/*  */
/*         } else { */
/*             #<{(| intialize output |)}># */
/*  */
/*  */
/*  */
/*         } */
/*     } */
/*  */
/*     return c; */
/* } */
