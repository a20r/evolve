#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "utils.h"


struct data *data_new(int rows, int cols, char **fields)
{
    int i;
    int j;
    struct data *d = malloc(sizeof(struct data));

    /* rows and columns */
    d->rows = rows;
    d->cols = cols;

    /* copy field names */
    d->fields = malloc(sizeof(char *) * (unsigned long) cols);
    for (i = 0; i < cols; i++) {
        d->fields[i] = malloc(sizeof(char) * strlen(fields[i]) + 1);
        strcpy(d->fields[i], fields[i]);
    }

    /* malloc 2d array */
    d->data = malloc(sizeof(float *) * (unsigned long) cols);
    for (i = 0; i < cols; i++) {
        d->data[i] = malloc(sizeof(float *) * (unsigned long) rows);

        for (j = 0; j < rows; j++) {
            d->data[i][j] = NULL;
        }
    }

    return d;
}

int data_destroy(struct data *d)
{
    /* free field names */
    free_mem_arr(d->fields, d->cols, free);

    /* free data */
    free_mem_2darr(d->data, d->rows, d->cols, free);
    free(d);
    return 0;
}
