#ifndef __CSV__
#define __CSV__

#include <stdio.h>
#include "data.h"

#ifndef CSV_LINE_MAX
#define CSV_LINE_MAX 4096
#endif

/* FUNCTIONS */
int csv_num_cols(FILE *f, const char *token);
int csv_num_rows(FILE *f, int header);
char **csv_parse_header(FILE *f, int cols, const char *token);
struct data *csv_info(FILE *f, int header, const char *token);
float **csv_parse_data_row(char *line, int cols, const char *token);
struct data *csv_load_data(const char *fp, int header, const char *token);

#endif
