#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbg/dbg.h>

#include "gp/data_loader.h"

#define BUFFER_SIZE 256


char *despace(char *s)
{
        char *out = s;
        char *put = s;

        while (*s != '\0') {
                if (*s != ' ') {
                        *put++ = *s;
                }
                s++;
        }
        *put = '\0';

        return out;
}

void trim_newline(char *s)
{
        int len = 0;

        len = strlen(s);
        if (s[len - 1] == '\n') {
                s[len - 1] = '\0';
        }
}

int lines_in_file(char *fp)
{
        FILE *f = NULL;
        int ch = 0;
        int lines = 0;

        f = fopen(fp, "r");
        if (f == NULL) {
                return -1;
        }

        while (EOF != (ch = fgetc(f))) {
                if (ch == '\n') {
                        lines++;
                }
        }
        fclose(f);

        return lines;
}

int number_of_cols(char *header_line, char *delimiters)
{
        int fields = 0;
        char *token = NULL;
        char *header_copy = NULL;

        /* prevent strtok from modifying original string */
        header_copy = malloc(sizeof(char) * strlen(header_line) + 1);
        strcpy(header_copy, header_line);

        /* split string */
        token = strtok(header_copy, delimiters);
        while (token) {
                token = strtok(NULL, delimiters);
                fields++;
        }

        free(header_copy);

        return fields;
}

int parse_header_line(char *line, int fields, char *delimiters, char **header)
{
        char *token = NULL;
        char field[BUFFER_SIZE];
        int i = 0;

        /* printf("parse_header[%d]: %s\n", i, header[i]); */
        token = strtok(line, delimiters);
        while (token) {
                if (i + 1 > fields) {
                        log_err("There are more fields then specified!");
                        return -1;
                }

                sscanf(token, "%s", field);
                token = strtok(NULL, delimiters);

                header[i] = malloc(sizeof(char) * strlen(field) + 1);
                strcpy(header[i], field);
                i++;
        }

        return 0;
}

int parse_data_line(char *line, int fields, char *delimiters, float *data)
{
        char *token = NULL;
        float data_point = 0;
        int i = 0;

        token = strtok(line, delimiters);
        while (token) {
                if (i + 1 > fields) {
                        log_err("There are more fields then specified!");
                        return -1;
                }

                sscanf(token, "%f", &data_point);
                token = strtok(NULL, delimiters);

                data[i] = data_point;
                i++;
        }

        return 0;
}

int load_data(char *fp, struct evolve_config *config, int data_type)
{
        FILE *data_file;
        char *line = NULL;
        size_t len = 0;
        int i = 0;
        int res = 0;
        int cols = 0;
        int rows = 0;
        char *delimiters = ",";
        char **header = NULL;
        float *data_row = NULL;
        float **data = NULL;

        /* open file */
        data_file = fopen(fp, "r");
        if (data_file == NULL) {
                log_err("Failed to open file [%s]!", fp);
                return -1;
        }

        /* parse file */
        res = getline(&line, &len, data_file);
        if (res == -1) {
                log_err("File [%s] is empty!", fp);
        } else {
                cols = number_of_cols(line, delimiters);
                rows = lines_in_file(fp) - 1;

                /* header */
                header = calloc(cols, sizeof(char *));
                trim_newline(line);
                despace(line);
                parse_header_line(line, cols, delimiters, header);

                /* data */
                data = calloc(rows, sizeof(float *));
                i = 0;
                while ((res = getline(&line, &len, data_file)) != -1) {
                        data_row = calloc(cols, sizeof(float));
                        parse_data_line(line, cols, delimiters, data_row);
                        data[i] = data_row;
                        i++;
                }
                free(line);
        }

        /* record into config */
        if (data_type == INPUT_DATA) {
                config->general.gp_tree->input_cols = cols;
                config->general.gp_tree->input_rows = rows;
                config->general.gp_tree->input_header= header;
                config->general.gp_tree->input_data = data;
        } else if (data_type == RESPONSE_DATA) {
                config->general.gp_tree->response_cols = cols;
                config->general.gp_tree->response_rows = rows;
                config->general.gp_tree->response_header= header;
                config->general.gp_tree->response_data = data;
        } else {
                goto error;
        }

        fclose(data_file);

        return 0;
error:
        return -1;
}
