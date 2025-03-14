#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "headers/point_h.h"


char *fmt_point3d(Point3d p) {
    char *fmt = (char *)malloc(32); 
    sprintf(fmt, "X:%.2f, Y:%.2f, Z:%.2f", p.x, p.y, p.z);

    return fmt;
}

void print_point3d(Point3d p) {
    printf("X:%.2f, Y:%.2f, Z:%.2f\n", p.x, p.y, p.z);
}
void print_point(Point p) {
    printf("X:%.2f, Y:%.2f\n", p.x, p.y);
}

//Z axis
Point3d rotateX(Point3d center, Point3d point, float degrees) {
    float rad = (degrees * M_PI) / 180;

    float z = point.z - center.z;
    float y = point.y - center.y;

    float z_rot = z * cos(rad) + y * sin(rad);
    float y_rot = -z * sin(rad) + y * cos(rad);

    z_rot += center.z;
    y_rot += center.y;

    Point3d new_pos = {point.x, y_rot, z_rot};
    return  new_pos;
}

//Y axis
Point3d rotateY(Point3d center, Point3d point, float degrees) {
    float rad = (degrees * M_PI) / 180;

    float x = point.x - center.x;
    float z = point.z - center.z;

    float x_rot = x * cos(rad) + z * sin(rad);
    float z_rot = -x * sin(rad) + z * cos(rad);

    x_rot += center.x;
    z_rot += center.z;

    Point3d new_pos = {x_rot, point.y, z_rot};
    return  new_pos;
}

//X axis
Point3d rotateZ(Point3d center, Point3d point, float degrees) {
    float rad = (degrees * M_PI) / 180;

    float x = point.x - center.x;
    float y = point.y - center.y;

    float x_rot = x * cos(rad) + y * sin(rad);
    float y_rot = -x * sin(rad) + y * cos(rad);

    x_rot += center.x;
    y_rot += center.y;

    Point3d new_pos = {x_rot, y_rot, point.z};
    return  new_pos;
}

