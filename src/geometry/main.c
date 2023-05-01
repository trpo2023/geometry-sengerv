#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libgeometry/parser.h>

int main()
{
    const char* output_path = "output";
    const char* input_path = "input/commands";

    int status = is_input_files_exist(input_path, output_path);
    if (status) {
        handle_error(status, (char*)input_path);
        return status;
    }

    FILE* input_file = fopen(input_path, "r");
    char input[MAX_INPUT_LENGTH];
    Circle* circles = (Circle*)malloc(sizeof(Circle) * MAX_CIRCLES);
    int i = 0;
    for (; fgets(input, MAX_INPUT_LENGTH, input_file); i++) {
        if (i > 0 && i % MAX_CIRCLES == 0) {
            Circle* tmp = (Circle*)realloc(
                    circles, MAX_CIRCLES * 2 * (i / MAX_CIRCLES));
            if (!tmp)
                return ERROR_REALLOC;
            circles = tmp;
        }
        input[strcspn(input, "\n")] = '\0';
        status = parse_circle(input, &circles[i]);
        if (status) {
            handle_error(status, input);
            return status;
        } else {
            calculate_circle(&circles[i]);
        }
    }
    get_intersects_circles(circles, i);
    print_circles(output_path, circles, i);
    fclose(input_file);
    free_circles(circles, i);
    return 0;
}