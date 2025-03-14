#include <math.h>
#include "headers/constants.h"
#include "headers/point_h.h"

Point project(Point3d pos3d, Point3d camera_pos) {
    Point pos2d = { -1, -1 };
    Point3d relative = { pos3d.x-camera_pos.x, pos3d.y-camera_pos.y, pos3d.z-camera_pos.z };

    double scale;
    if (pos3d.z >= camera_pos.z)
        scale = 5;
    else
        scale = 100/(relative.z - 100);

    pos2d.x = scale*((relative.x)) + width/2.0;
    pos2d.y = scale*((relative.y)) + height/2.0;
    return pos2d;
}

int is_in_view(Point3d pos3d, Point3d camera_pos) {
    Point3d relative = { pos3d.x-camera_pos.x, pos3d.y-camera_pos.y, pos3d.z-camera_pos.z };
    double tan_ratio = tan(M_PI/6);

    if (relative.z >= 0)
        return 0;
    return 1;

    /*if (fabsf(relative.x/relative.z) > tan_ratio)
        return 0;
    if (fabsf(relative.y/relative.z) > tan_ratio)
        return 0;
    return 1;*/
}

int points_in_range(Line3d line, Point3d in_range[2], Point3d camera) {
    int is_first = 1;
    for (int i=0; i<line.length; i++) {
        Point3d p = line.points[i];
        if (is_in_view(p, camera)) {
            if (is_first-- == 1) {
                in_range[0].x = p.x;
                in_range[0].y = p.y;
                in_range[0].z = p.z;
            }
            if (i == line.length-1) {
                in_range[1].x = p.x;
                in_range[1].y = p.y;
                in_range[1].z = p.z;
            }
        }
    }
    if (in_range[1].x == 9999) {
        in_range[1] = in_range[0];
    }
    return is_first-1;
}


