#ifndef point_h
#define point_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    float x;
    float y;
    float z;
} Point3d;

typedef struct {
    Point *points;
    unsigned int length;
} Line;

typedef struct {
    Point3d *points;
    unsigned int length;
} Line3d;

char *fmt_point3d(Point3d p);

void print_point3d(Point3d p);
void print_point(Point p);
Point3d rotateX(Point3d center, Point3d point, float degrees);
Point3d rotateY(Point3d center, Point3d point, float degrees);
Point3d rotateZ(Point3d center, Point3d point, float degrees);


#endif
