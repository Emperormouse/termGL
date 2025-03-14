#include <stdio.h>
#include "headers/constants.h"
#include "headers/line_h.h"

char border[width+2];

void print_grid(char grid[height][width]) {
    if (border[0] != '=')
        for (int i=0; i<width+2; i++) {
            border[i] = '=';
        }

    char fmt_str[12];
    sprintf(fmt_str, "|%%.%ds|\n", width);

    printf("\n\n\n\n\n\n\n\n\n%s\n", border);
    for (int r=0; r<height; r++) {
        printf(fmt_str, grid[r]);
    }
    printf("%s\n", border);

//    char out[width*height + height +1];
//    char fmt_str[10];
//    sprintf(fmt_str, "%%.%ds\n", width);
//    for (int r=0; r<height; r++) {
//        sprintf(out + r*(width+1), fmt_str, grid[r]);
//    }
//    printf("%s", out);
}

void add_line(char grid[height][width], Line line) {
    for (int i=0; i<line.length; i++) {
        unsigned int x = line.points[i].x;
        unsigned int y = line.points[i].y;
        if (y > 0 && y < height && x > 0 && x < width)
            grid[y][x] = 'X';
    }
}

