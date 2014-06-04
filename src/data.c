#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"


struct data *data_new(int rows, int cols, char **fields)
{
    int i;
    int j;
    struct data *d = malloc(sizeof(struct data));

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
    int i;
    int j;

    /* free field names */
    if (d->fields) {
        for (i = 0; i < d->cols; i++) {
            free(d->fields[i]);
        }
        free(d->fields);
    }

    /* free data */
    if (d->data) {
        for (i = 0; i < d->cols; i++) {  /* free columns */
            for (j = 0; j < d->rows; j++) {  /* free rows */
                free(d->data[i][j]);
            }
            free(d->data[i]);
        }
        free(d->data);
    }

    free(d);
    return 0;
}
