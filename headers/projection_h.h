#ifndef projection_h
#define projection_h

#include <math.h>
#include "constants.h"
#include "point_h.h"

typedef struct {
    Point3d pos;
    Point3d rot;
} Camera;

Point project(Point3d pos3d, Point3d camera_pos);
int is_in_view(Point3d pos3d, Point3d camera_pos);
int points_in_range(Line3d line, Point3d in_range[2], Point3d camera);

#endif
