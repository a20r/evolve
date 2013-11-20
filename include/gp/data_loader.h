#ifndef _GP_DATA_LOADER_H_
#define _GP_DATA_LOADER_H_

#include "config/config.h"

#define INPUT_DATA 1
#define RESPONSE_DATA 2


/* FUNCTIONS */
char *despace(char *s);
void trim_newline(char *s);
int lines_in_file(char *fp);
int number_of_cols(char *header_line, char *delimiters);
int parse_header_line(char *line, int fields, char *delimiters, char **headers);
int parse_data_line(char *line, int fields, char *delimiters, float *data);
int load_data(char *fp, struct evolve_config *config, int data_type);

#endif
