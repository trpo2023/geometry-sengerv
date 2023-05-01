#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libgeometry/parser.h>

int is_input_files_exist(const char* input_path, const char* output_path)
{
    FILE* output_file = fopen(output_path, "w");
    if (!output_file)
        return ERROR_FILE_OUTPUT;

    fclose(output_file);

    FILE* input_file = fopen(input_path, "r");

    if (!input_file)
        return ERROR_FILE_INPUT;

    fclose(input_file);
    return 0;
}

void handle_error(int error_id, char* str)
{
    char* error_msg;
    switch (error_id) {
    case ERROR_FILE_OUTPUT:
        error_msg = "Error: can't create output file";
        break;
    case ERROR_FILE_INPUT:
        error_msg = "Error: can't open input file";
        break;
    case ERROR_PARSER_NAME:
        error_msg = "Error: expected 'circle'";
        break;
    case ERROR_PARSER_LEFT_PARENTHESIS:
        error_msg = "Error: expected '('";
        break;
    case ERROR_PARSER_RIGHT_PARENTHESIS:
        error_msg = "Error: expected ')'";
        break;
    case ERROR_PARSER_COMMA:
        error_msg = "Error: expected ','";
        break;
    case ERROR_PARSER_DOUBLE:
        error_msg = "Error: expected <double>";
        break;
    case ERROR_PARSER_UNEXPECTED_TOKEN:
        error_msg = "Error: unexpected token";
        break;
    default:
        error_msg = "Error: something wrong";
        break;
    }
    fprintf(stderr, "%s\n%s\n", error_msg, str);
}

char* lower_all(char* str)
{
    if (!str)
        return NULL;
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);
        i += 1;
    }
    return str;
}
void print_circles(const char* output_path, Circle* circles, int count)
{
    FILE* file = fopen(output_path, "a");
    for (int i = 0; i < count; i++) {
        char output[MAX_OUTPUT_LENGTH];
        sprintf(output,
                "%d. circle(%f %f, %f)\n perimetr: %f\n area: "
                "%f\n intersects:\n",
                i + 1,
                circles[i].x,
                circles[i].y,
                circles[i].r,
                circles[i].perimetr,
                circles[i].area);

        for (int j = 0; j < count; j++) {
            if (circles[i].intersects[j] == 1) {
                sprintf(output + strlen(output), "\t%d. circle\n", j + 1);
            }
        }
        sprintf(output + strlen(output), "\n");
        fprintf(file, "%s", output);
        printf("%s", output);
    }
    fclose(file);
}

int is_circle(char* str)
{
    if (strncmp(str, "circle", 6))
        return ERROR_PARSER_NAME;
    return 0;
}

int is_double(char* str_start, char** str_end, double* num)
{
    *num = strtod(str_start, str_end);
    if (str_start == *str_end)
        return ERROR_PARSER_DOUBLE;
    return 0;
}

int is_prefix(char* str_start, char* prefix)
{
    size_t length = strlen(prefix);
    length = !length ? 1 : length;
    if (strncmp(str_start, prefix, length))
        return ERROR_PARSER_UNEXPECTED_TOKEN;
    return 0;
}

int is_num_circle(char* str_start, char** str_end, char* ending, double* x)
{
    if (is_double(str_start, str_end, x))
        return ERROR_PARSER_DOUBLE;
    else if (is_prefix(*str_end, ending))
        return ERROR_PARSER_UNEXPECTED_TOKEN;
    *str_end += 1;
    return 0;
}

void calculate_circle(Circle* circle)
{
    circle->perimetr = 2 * M_PI * circle->r;
    circle->area = M_PI * circle->r * circle->r;
}

void get_intersects_circles(Circle* circles, int count)
{
    for (int i = 0; i < count; i++) {
        int* intersects = (int*)calloc(sizeof(int), count);
        for (int j = 0; j < count; j++) {
            if (i == j)
                continue;
            double distance
                    = sqrt(pow(circles[i].x - circles[j].x, 2)
                           + pow(circles[i].y - circles[j].y, 2));
            double sum_radii = circles[i].r + circles[j].r;

            if (distance <= sum_radii) {
                intersects[j] = 1;
            }
        }
        circles[i].intersects = intersects;
    }
}

void free_circles(Circle* circles, int count)
{
    for (int i = 0; i < count; i++) {
        if (circles[i].intersects)
            free(circles[i].intersects);
    }
    free(circles);
}
int parse_circle(char* start, Circle* out_values)
{
    start = lower_all(start);
    char** end = &start;
    int status = 0;
    double x, y, r;

    if (is_circle(start))
        return ERROR_PARSER_NAME;

    if (is_prefix(start + 6, "("))
        return ERROR_PARSER_UNEXPECTED_TOKEN;

    status = is_num_circle(start + 7, end, " ", &x);
    if (status)
        return status;
    status = is_num_circle(*end, end, ",", &y);
    if (status)
        return status;
    status = is_num_circle(*end, end, ")", &r);
    if (status)
        return status;

    if (is_prefix(*end, "\0"))
        return ERROR_PARSER_UNEXPECTED_TOKEN;

    out_values->x = x;
    out_values->y = y;
    out_values->r = r;

    return 0;
}