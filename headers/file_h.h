#ifndef file_h
#define file_h

#include <stdio.h>
#include "point_h.h"

int data_file_length(char *path);
void read_data_file(char *path, Point3d pairs[][2]);

#endif
