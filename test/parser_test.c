#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libgeometry/parser.h>

#include <ctest.h>

CTEST(TEST_is_input_files_exist, simple_file)
{
    char* wrong_file = "wrong.tmp";
    char* right_file = "right.tmp";
    char* output_file = "out.tmp";

    FILE* create_file = fopen(right_file, "w");
    ASSERT_EQUAL(0, is_input_files_exist(right_file, output_file));
    fclose(create_file);
    remove(right_file);

    remove(wrong_file);
    ASSERT_EQUAL(
            ERROR_FILE_INPUT, is_input_files_exist(wrong_file, output_file));
    remove(output_file);
}

CTEST(TEST_lower_all, simple_lower)
{
    char bad[] = "1!2FsfT3rFl$km.Zzx";
    char good[] = "1!2fsft3rfl$km.zzx";
    char* result = lower_all(bad);

    ASSERT_EQUAL(0, strcmp(good, result));
}

CTEST(TEST_is_circle, simple_circle)
{
    ASSERT_EQUAL(0, is_circle("circle"));
    ASSERT_EQUAL(ERROR_PARSER_NAME, is_circle("not circle"));
}

CTEST(TEST_is_double, simple_double)
{
    char *start = "25", *end;
    double num = 0;
    ASSERT_EQUAL(0, is_double(start, &end, &num));
    ASSERT_EQUAL(25, num);
}

CTEST(TEST_is_prefix, simple_prefix)
{
    char str[] = "Super text";
    ASSERT_EQUAL(0, is_prefix(str, "Super"));
    ASSERT_EQUAL(0, is_prefix(str + 6, "text"));
    ASSERT_EQUAL(
            ERROR_PARSER_UNEXPECTED_TOKEN, is_prefix(str, "not Super text"));
}

CTEST(TEST_is_num_circle, simple_num_circle)
{
    char str[] = "circle(1 5, 25)";
    char* end;
    double x = 0, y = 0, r = 0;

    ASSERT_EQUAL(0, is_num_circle(str + 7, &end, " ", &x));
    ASSERT_EQUAL(0, strcmp("5, 25)", end));

    ASSERT_EQUAL(0, is_num_circle(end, &end, ",", &y));
    ASSERT_EQUAL(0, strcmp(" 25)", end));

    ASSERT_EQUAL(0, is_num_circle(end, &end, ")", &r));
    ASSERT_EQUAL(0, strcmp("", end));

    ASSERT_EQUAL(1, x);
    ASSERT_EQUAL(5, y);
    ASSERT_EQUAL(25, r);
}

CTEST(TEST_parse_circle, simple_parse_circle)
{
    /*                               ._.            */
    char cool_circle[] = "cIrClE(1        5,     25)";
    char bad_circle[] = "cirlce(1 5, 25)";
    char very_bad_circle[] = "circle(1 5, 25)f";
    Circle values;
    ASSERT_EQUAL(0, parse_circle(cool_circle, &values));
    ASSERT_EQUAL(ERROR_PARSER_NAME, parse_circle(bad_circle, &values));
    ASSERT_EQUAL(
            ERROR_PARSER_UNEXPECTED_TOKEN,
            parse_circle(very_bad_circle, &values));
    ASSERT_EQUAL(1, values.x);
    ASSERT_EQUAL(5, values.y);
    ASSERT_EQUAL(25, values.r);
}

CTEST(TEST_calculate_circle, simple_calculate_circle)
{
    Circle circle = {5, 5, 5, 0, 0, NULL};
    calculate_circle(&circle);
    double area = M_PI * circle.r * circle.r;
    double perimetr = 2 * M_PI * circle.r;
    ASSERT_EQUAL(area, circle.area);
    ASSERT_EQUAL(perimetr, circle.perimetr);
}

CTEST(TEST_get_intersects_circles, simple_get_intersects_circles)
{
    Circle* circles = (Circle*)malloc(sizeof(Circle) * 3);
    circles[0].x = 0;
    circles[0].y = 0;
    circles[0].r = 5;

    circles[1].x = 2;
    circles[1].y = 2;
    circles[1].r = 1;

    circles[2].x = 0;
    circles[2].y = 0;
    circles[2].r = 1;

    get_intersects_circles(circles, 3);
    /*/             1 : 2, 3               /*/
    ASSERT_EQUAL(0, circles[0].intersects[0]);
    ASSERT_EQUAL(1, circles[0].intersects[1]);
    ASSERT_EQUAL(1, circles[0].intersects[2]);
    /*/             2 : 1                  /*/
    ASSERT_EQUAL(1, circles[1].intersects[0]);
    ASSERT_EQUAL(0, circles[1].intersects[1]);
    ASSERT_EQUAL(0, circles[1].intersects[2]);
    /*/             3 : 1                  /*/
    ASSERT_EQUAL(1, circles[2].intersects[0]);
    ASSERT_EQUAL(0, circles[2].intersects[1]);
    ASSERT_EQUAL(0, circles[2].intersects[2]);

    free_circles(circles, 3);
}