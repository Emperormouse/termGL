#ifndef grid_h
#define grid_h

#include <stdio.h>
#include "constants.h"
#include "line_h.h"

void print_grid(char grid[height][width]);
void add_line(char grid[height][width], Line line);

#endif
