/*  File:       turtle_B.c
    Author:     Mohamed Elmekki
    Date:       15-02-2017
    Topic:
    Purpose:    To generate Postscript files from user input (as text files).
    Notes:
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define M_PI 3.14159265359
#define WIDTH 595
#define HEIGHT 841

/* TYPE DECLARATIONS ------------------------------------------------------- */

/* Information about pen colour */
typedef struct colour {
    float r, g, b;
} colour_t;

/* Information about turtle; positioning and direction */
typedef struct turtle {
    double x, y, dir;
} turtle_t;

/* Informations about pen; colour, down/up (1/0) state, filled/not filled */
typedef struct pen {
    colour_t colour;
    int down;
    int filled;
} pen_t;

/* PROTOTYPE DECLARATIONS -------------------------------------------------- */

int process_commands(turtle_t *t, pen_t *p);
void turtle_reset(turtle_t *t);
void pen_reset(pen_t *p);
void set_pen_colour(pen_t *p, colour_t c);
void pen_fill(pen_t *p);
void turtle_goto(turtle_t *t, float x, float y);
void turtle_square(turtle_t *t, int size);
void turtle_turn(turtle_t *t, float angle);
void turtle_walk(turtle_t *t, int d);
void turtle_row(turtle_t *t, int count, int size);
void turtle_polygon(turtle_t *t, pen_t *p, int count, int size);
void turtle_grid(turtle_t *t, int rows, int columns, int size);
void turtle_random_walk(turtle_t *t, pen_t *p, int steps);
void turtle_fractal(turtle_t *t, int d, int r);

void set_ps_header(int, int);
void set_ps_display();
void set_ps_stroke();
void set_ps_newpath();

int valid_colour(colour_t c);
int randMove(int min, int max);

/* MAIN -------------------------------------------------------------------- */
int main() {

    srand(time(NULL));

    set_ps_header(WIDTH, HEIGHT);

    turtle_t boo;
    pen_t pen;

    turtle_reset(&boo);
    pen_reset(&pen);

    process_commands(&boo, &pen);

    set_ps_display();

    return 0;
}

/* FUNCTIONS --------------------------------------------------------------- */

/* process_commands -----------------------------------------------------------
    Read from file; execute appropriate command.
---------------------------------------------------------------------------- */
int process_commands(turtle_t *t, pen_t *p) {
    char cmd;
    colour_t c;
    float x, y; /* coordinates */
    float s; /* square size */
    int d; /* walk distance */
    int columns; /* square count */
    int rows; /* rows of squares */
    int f; /* number of faces, polygon*/
    float a; /* turn degrees */
    int r; /* fractal recursions */

    /* TODO -------------------------------------------------------------------
    CHECK FOR CHARACTER VALIDITY (lower cases too)
    PAGE 1 OF PDF
    ------------------------------------------------------------------------ */

    while ((fscanf (stdin, "%c", &cmd)) != EOF) {
        switch (cmd) {

            /* Change pen colour */
            case 'C':
                fscanf(stdin, "%f %f %f\n", &c.r, &c.g, &c.b);
                if (valid_colour(c)) {
                    set_pen_colour(p, c);
                }
                break;

            /* Go to */
            case 'G':
                fscanf(stdin, "%f %f\n", &x, &y);
                turtle_goto(t, x, y);
                break;

            /* Walk forward */
            case 'W':
                fscanf(stdin, "%d\n", &d);
                turtle_walk(t, d);
                set_ps_stroke();
                break;

            /* Turn */
            case 'T':
                fscanf(stdin, "%f\n", &a);
                turtle_turn(t, a);
                break;

            /* Square */
            case 'S':
                fscanf(stdin, "%f\n", &s);
                turtle_square(t, s);
                break;

            /* A row of squares */
            case 'R':
                fscanf(stdin, "%d %f\n", &columns, &s);
                turtle_row(t, columns, s);
                break;

            /* A grid of squares */
            case 'M':
                fscanf(stdin, "%d %d %f\n", &rows, &columns, &s);
                turtle_grid(t, rows, columns, s);
                break;

            /* Random walk */
            case 'Z':
                fscanf(stdin, "%d\n", &d);
                turtle_random_walk(t, p, d);
                break;

            /* Polygon */
            case 'P':
                fscanf(stdin, "%d %d\n", &f, &d);
                turtle_polygon(t, p, f, d);
                set_ps_stroke();
                break;

            /* Fractal */
            case 'X':
                fscanf(stdin, "%d %d\n", &d, &r);
                turtle_fractal(t, d, r);
                set_ps_stroke();
                break;

            /* Fill */
            case 'F':
                pen_fill(p);
                break;

            /* Reset */
            case 'U':
                turtle_reset(t);
                break;

            default:
                break;
        }
    }

    return 0;
}

/* turtle_reset ---------------------------------------------------------------
    Replaces turtle at (0,0); sets direction to 0.0.
---------------------------------------------------------------------------- */
void turtle_reset(turtle_t *t) {
    t->x = t->y = 0.0;
    t->dir = 0;
}

/* pen_reset ------------------------------------------------------------------
    Reset pen colour to white; lift pen; filled to false.
---------------------------------------------------------------------------- */
void pen_reset(pen_t *p) {
    colour_t black = {0.0, 0.0, 0.0};
    set_pen_colour(p, black);
    p->down = 0;
    p->filled = 0;
}

/* set_pen_colour -------------------------------------------------------------
    Specify pen colour.
---------------------------------------------------------------------------- */
void set_pen_colour(pen_t *p, colour_t c) {

    /* Modify pen colour */
    p->colour.r = c.r;
    p->colour.g = c.g;
    p->colour.b = c.b;

    fprintf(stdout, "%.2f %.2f %.2f setrgbcolor\n", c.r, c.g, c.b);

    return;
}

/* pen_fill -------------------------------------------------------------------
    Fill/unfill pen.
---------------------------------------------------------------------------- */
void pen_fill(pen_t *p) {
    if (p->filled == 1) p->filled = 0;
    if (p->filled == 0) p->filled = 1;
}

/* turtle_goto ----------------------------------------------------------------
    Moves turtle to given coordinates.
---------------------------------------------------------------------------- */
void turtle_goto(turtle_t *t, float x, float y) {
    t->x = x;
    t->y = y;

    fprintf(stdout, "%f %f moveto\n", x, y);
}

/* turtle_square --------------------------------------------------------------
    Draws square centered at turtle's current coordinates.
---------------------------------------------------------------------------- */
void turtle_square(turtle_t *t, int size) {
    float x2 = t->x - (size/2);
    float y2 = t->y - (size/2);

    fprintf(stdout, "%f %f %d %d rectfill\n", x2, y2, size, size);
}

/* turtle_turn ----------------------------------------------------------------
    Turns turtle by a given value.
---------------------------------------------------------------------------- */
void turtle_turn(turtle_t *t, float angle) {
    t->dir += angle;
}

/* turtle_walk ----------------------------------------------------------------
    Moves turtle a given number of units in the current direction she/he is
    facing.
---------------------------------------------------------------------------- */
void turtle_walk(turtle_t *t, int d) {
    double x2 = t->x + d * cos(t->dir * M_PI/180);
    double y2 = t->y + d * sin(t->dir * M_PI/180);

    fprintf(stdout, "%f %f lineto\n", x2, y2);

    t->x = x2;
    t->y = y2;
}

/* turtle_row -----------------------------------------------------------------
    Draws a horizontal row of squares.
---------------------------------------------------------------------------- */
void turtle_row(turtle_t *t, int count, int size) {
    int i;

    for (i = 1; i <= count; i++) {
        turtle_square(t, size);
        turtle_goto(t, t->x + (size * 1.5), t->y);
    }

    turtle_goto(t, t->x - (size * 1.5), t->y);
}

/* turtle_polygon -------------------------------------------------------------
    Draws a equilateral polygon with the given number of sides.
---------------------------------------------------------------------------- */
void turtle_polygon(turtle_t *t, pen_t *p, int count, int size) {
    int i;

    if (count > 2) {
        for (i = 0; i < count; i++) {
            turtle_walk(t, size);
            turtle_turn(t, 360/count);
        }

        fprintf(stdout, "closepath\n");

        if (p->filled == 1) {
            fprintf(stdout, "fill\n");
        }
    }
}

/* turtle_grid ----------------------------------------------------------------
    Draws a grid of squares with a given size.
---------------------------------------------------------------------------- */
void turtle_grid(turtle_t *t, int rows, int columns, int size) {
    int i;
    float x1 = t->x;

    for (i = 1; i <= rows; i ++) {
        turtle_row(t, columns, size);

        if (i == rows) break;

        turtle_goto(t, x1, t->y + (size * 1.5));
    }
}

/* turtle_random_walk ---------------------------------------------------------
    Move turtle in random cardinal directions; randomize colours.
---------------------------------------------------------------------------- */
void turtle_random_walk(turtle_t *t, pen_t *p, int steps) {
    int i;
    colour_t c;

    c.r = 0.0;
    c.g = 0.0;
    c.b = 0.0;
    set_pen_colour(p, c);

    for (i = 0; i < steps; i++) {
        int move = randMove(1, 4);
        switch (move) {
            case 1:
                set_ps_newpath();
                turtle_goto(t, t->x, t->y);
                turtle_walk(t, 10);
                set_ps_stroke();
                break;

            case 2:
                turtle_turn(t, 90);
                break;

            case 3:
                turtle_turn(t, -90);
                break;

            case 4:
                c.r = (rand() % 10) * .1;
                c.g = (rand() % 10) * .1;
                c.b = (rand() % 10) * .1;
                set_pen_colour(p, c);
                break;
        }
    }
}

/* turtle_fractal -------------------------------------------------------------
    Read from file; execute appropriate command.
---------------------------------------------------------------------------- */
void turtle_fractal(turtle_t *t, int d, int r) {
    /*
    int d2 = d/2;

    if (r == 0) {

    } else {
        turtle_turn(t, 45);
        turtle_fractal(t, d2, r - 1);
        turtle_turn(t, -90);
        turtle_fractal(t, d2, r - 1);
        turtle_turn(t, 45);
        turtle_fractal(t, d2, r - 1);
    }
    */
}

/* set_ps_header --------------------------------------------------------------
    Print postscript header.
---------------------------------------------------------------------------- */
void set_ps_header(int w, int h) {
    fprintf(stdout, "%%!PSAdobe3.0EPSF3.0\n");
    fprintf(stdout, "%%%%BoundingBox: 0 0 %d %d\n", w, h);
}

/* set_ps_display -------------------------------------------------------------
    Print postscript display command.
---------------------------------------------------------------------------- */
void set_ps_display() {
    fprintf(stdout, "showpage\n");
}

/* set_ps_stroke --------------------------------------------------------------
    Print postscipt stroke command.
---------------------------------------------------------------------------- */
void set_ps_stroke() {
    fprintf(stdout, "stroke\n");
}

/* set_ps_newpath -------------------------------------------------------------
    Print postscipt newpath command.
---------------------------------------------------------------------------- */
void set_ps_newpath() {
    fprintf(stdout, "newpath\n");
}

/* valid_color ----------------------------------------------------------------
    Checks desired colour against HEX colour code limitations.
---------------------------------------------------------------------------- */
int valid_colour(colour_t c) {
    if (c.r >= 0 && c.r <= 1 && c.g >= 0 && c.g <= 1 && c.b >= 0 && c.b <= 1) {
        return 1;
    } else {
        return 0;
    }
}

/* randNo ---------------------------------------------------------------------
    Random number generator.
---------------------------------------------------------------------------- */
int randMove(int min, int max) {
    return min + (rand() % max);
}