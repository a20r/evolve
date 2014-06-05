#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "utils.h"
#include "csv.h"
#include "data.h"


int csv_num_cols(FILE *f, const char *token)
{
    int cols = 0;
    char *matched = NULL;
    char line[CSV_LINE_MAX];

    fgets(line, sizeof(line), f);
    matched = strtok(line, token);

    while (matched != NULL) {
        matched = strtok(NULL, token);
        if (matched) {
            cols++;
        }
    }

    if (cols > 1) {
        cols++;
    }
    rewind(f);

    return cols;
}

int csv_num_rows(FILE *f, int header)
{
    int rows = 0;
    char line[CSV_LINE_MAX];

    /* get number of rows */
    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        if (strlen(line) > 1) {
            rows++;
        }
    }
    rewind(f);

    return rows - header;
}

const char **csv_parse_header(FILE *f, int cols, const char *token)
{
    int i = 0;
    const char **fields = malloc(sizeof(char *) * (unsigned long) cols);
    char line[CSV_LINE_MAX];
    char *matched;

    /* get header */
    fgets(line, sizeof(line), f);

    /* tokenize header */
    matched = trim(strtok(line, token));
    if (matched) {
        fields[i] = matched;
        i++;
    } else{
        free(fields);
        return NULL;
    }

    while (matched != NULL) {
        matched = trim(strtok(NULL, token));
        if (matched) {
            fields[i] = matched;
            i++;
        }
    }
    rewind(f);

    return fields;
}

struct data *csv_info(FILE *f, int header, const char *token)
{
    int rows = csv_num_rows(f, header);
    int cols = csv_num_cols(f, token);
    const char **fields = NULL;
    struct data *d;

    /* parse header */
    if (header) {
        fields = csv_parse_header(f, cols, token);
    }

    /* create data */
    d = data_new(rows, cols, fields);

    /* clean up */
    free_mem_arr(fields, cols, free);

    return d;
}

float **csv_parse_data_row(char *line, int cols, const char *token)
{
    int i = 0;
    float **data_row = malloc(sizeof(float *) * (unsigned long) cols);
    char *matched;

    /* parse first token */
    matched = trim(strtok(line, token));
    if (matched) {
        data_row[i] = malloc_float((float) atof(matched));
        i++;
    } else{
        free(data_row);
        return NULL;
    }
    free(matched);

    /* parse subsequent token */
    while (matched != NULL) {
        matched = trim(strtok(NULL, token));
        if (matched) {
            data_row[i] = malloc_float((float) atof(matched));
            i++;
        }
        free(matched);
    }

    return data_row;
}

struct data *csv_load_data(const char *fp, int header, const char *token)
{
    int i;
    int row = 0;
    char line[CSV_LINE_MAX];
    struct data *d;
    float **data_row;
    FILE *f = fopen(fp, "r");

    /* obtain csv info */
    silent_check(f != NULL);
    d = csv_info(f, header, token);

    /* ignore header */
    if (header) {
        fgets(line, sizeof(line), f);
    }

    /* parse data */
    while (fgets(line, sizeof(line), f) && strlen(line) > 1) {
        data_row = csv_parse_data_row(line, d->cols, token);
        for (i = 0; i < d->cols; i++) {
            d->data[i][row] = data_row[i];
        }

        free(data_row);
        row++;
    }

    fclose(f);
    return d;
error:
    fclose(f);
    return NULL;
}
