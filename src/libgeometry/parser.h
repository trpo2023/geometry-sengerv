#pragma once
#define MAX_INPUT_LENGTH 100
#define MAX_OUTPUT_LENGTH 400
#define MAX_CIRCLES 10

typedef struct Circle {
    double x, y, r, perimetr, area;
    int* intersects;
} Circle;

enum {
    ERROR_FILE_OUTPUT = 1,
    ERROR_FILE_INPUT,
    ERROR_PARSER_NAME,
    ERROR_PARSER_LEFT_PARENTHESIS,
    ERROR_PARSER_RIGHT_PARENTHESIS,
    ERROR_PARSER_COMMA,
    ERROR_PARSER_DOUBLE,
    ERROR_PARSER_UNEXPECTED_TOKEN,
    ERROR_REALLOC
};
// Функция для проверки входных и выходных файлов
int is_input_files_exist(const char* input_path, const char* output_path);
// Функция для обработки ошибок и вывода сообщения
void handle_error(int error_id, char* str);
// Функция для перевода строки в нижний регистр
char* lower_all(char* str);
// Функции для парсинга круга
int is_circle(char* str);
int is_left_parenthesis(char* str);
int is_double(char* str_start, char** str_end, double* num);
int is_prefix(char* str_start, char* prefix);
int is_num_circle(char* str_start, char** str_end, char* ending, double* x);
// Функция для обработки входной строки
void calculate_circle(Circle* circle);
int parse_circle(char* str, Circle* out_values);
// Функция для вывода круга
void print_circles(const char* output_path, Circle* circles, int count);
void get_intersects_circles(Circle* circles, int count);
void free_circles(Circle* circles, int count);