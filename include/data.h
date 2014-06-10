#ifndef __DATA__
#define __DATA__

#include <stdio.h>

#ifndef CSV_LINE_MAX
#define CSV_LINE_MAX 4096
#endif

struct data {
    int rows;
    int cols;
    char **fields;
    float ***data;
};

/* FUNCTIONS */
struct data *data_new(int rows, int cols, const char **field_names);
int data_destroy(struct data *d);
int data_field_index(struct data *d, const char *field);

int csv_num_cols(FILE *f, const char *token);
int csv_num_rows(FILE *f, int header);
const char **csv_parse_header(FILE *f, int cols, const char *token);
struct data *csv_info(FILE *f, int header, const char *token);
float **csv_parse_data_row(char *line, int cols, const char *token);
struct data *csv_load_data(const char *fp, int header, const char *token);

#endif
