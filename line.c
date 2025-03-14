#include <math.h>
#include "headers/point_h.h"

Line gen_line(Point p1, Point p2) {
    Point *line;
    unsigned int length = 1;
    Point current_p = p1;

    float diff_x = p2.x - current_p.x;
    float diff_y = p2.y - current_p.y;

    float ratio;
    if (diff_x == 0) {
        ratio = 999999;
    } else {
        ratio = diff_y / diff_x;
    }

    //Loop Twice. The first time figure out the length of the line. 
    //Then allocate enough memory for that length, and fill in the
    //memory with the points.
    for (int i=0; i<2; i++) {
        unsigned int count_x = 0;
        unsigned int count_y = 0;
        unsigned int offset = 0;
        if (i == 1) {
            line = (Point *)malloc(length * sizeof(Point));
            current_p = p1;
        }
        while (roundf(current_p.x) != roundf(p2.x) || roundf(current_p.y) != roundf(p2.y)) {
            //printf("X:%.2f, Y:%.2f, Z:%.2f\n", current_p.x, current_p.y);
            //printf("X:%.2f, Y:%.2f, Z:%.2f\n", p2.x, p2.y);
            if (i == 0) 
                ++length;
            if (i == 1) {
                line[offset].x = current_p.x;
                line[offset].y = current_p.y;
                ++offset;
            }

            if (fabsf(count_x * ratio) < count_y) {
                if (roundf(current_p.x) != roundf(p2.x)) {
                    if (diff_x > 0)
                        ++current_p.x;
                    else
                        --current_p.x;
                }
                ++count_x;
            } else {
                if (roundf(current_p.y) != roundf(p2.y)) {
                    if (diff_y > 0) 
                        ++current_p.y;
                    else 
                        --current_p.y;
                }
                ++count_y;
            }
        }
    }
    line[length-1].x = current_p.x;
    line[length-1].y = current_p.y;

    Line line_struct = {line, length};
    return line_struct;
}

Line3d gen_line3d(Point3d p1, Point3d p2) {
    unsigned int length = 1;
    Point3d current_p = p1;

    float diff_x = p2.x - current_p.x;
    float diff_y = p2.y - current_p.y;
    float diff_z = p2.z - current_p.z;

    float ratioY;
    float ratioZ;
    if (diff_x == 0) {
        ratioY = 9999 * diff_y;
        ratioZ = 9999 * diff_z;
    } else {
        ratioY = diff_y / diff_x;
        ratioZ = diff_z / diff_x;
    }
    float ratioYZ;
    if (diff_z == 0)
        ratioYZ = 99999;
    else
        ratioYZ = diff_y / diff_z;

    unsigned int count_x = 0;
    unsigned int count_y = 0;
    unsigned int count_z = 0;
    unsigned int offset = 0;

    while (roundf(current_p.x) != roundf(p2.x) || roundf(current_p.y) != roundf(p2.y) || roundf(current_p.z) != roundf(p2.z)) {
        ++length;

        if (fabsf(count_x * ratioY) < count_y && fabsf(count_x * ratioZ) < count_z) {
            if (roundf(current_p.x) != roundf(p2.x)) {
                if (diff_x > 0)
                    ++current_p.x;
                else
                    --current_p.x;
            }
            ++count_x;
        } else if (fabsf(count_z * ratioYZ) < count_y) {
            if (roundf(current_p.z) != roundf(p2.z)) {
                if (diff_z > 0)
                    ++current_p.z;
                else
                    --current_p.z;
            }
            ++count_z;

        } else {
            if (roundf(current_p.y) != roundf(p2.y)) {
                if (diff_y > 0) 
                    ++current_p.y;
                else 
                    --current_p.y;
            }
            ++count_y;
        }
    }
    //Loop Twice. The first time figure out the length of the line. 
    //Then allocate enough memory for that length, and fill in the
    //memory with the points.
//    puts("MARK4");
    count_x = 0;
    count_y = 0;
    count_z = 0;
    offset = 0;

//    puts("MARK5");
    //printf("%d\n", length);
    Point3d *line = (Point3d *)malloc(length * sizeof(Point3d));

//    puts("MARK9");
    current_p = p1;

    while (roundf(current_p.x) != roundf(p2.x) || roundf(current_p.y) != roundf(p2.y) || roundf(current_p.z) != roundf(p2.z)) {
        line[offset] = current_p;
//        puts("MARK6");
        ++offset;

        if (fabsf(count_x * ratioY) < count_y && fabsf(count_x * ratioZ) < count_z) {
            if (roundf(current_p.x) != roundf(p2.x)) {
                if (diff_x > 0)
                    ++current_p.x;
                else
                    --current_p.x;
            }
            ++count_x;
        } else if (fabsf(count_z * ratioYZ) < count_y) {
            if (roundf(current_p.z) != roundf(p2.z)) {
                if (diff_z > 0)
                    ++current_p.z;
                else
                    --current_p.z;
            }
            ++count_z;

        } else {
            if (roundf(current_p.y) != roundf(p2.y)) {
                if (diff_y > 0) 
                    ++current_p.y;
                else 
                    --current_p.y;
            }
            ++count_y;
        }
    }
    line[length-1] = current_p;

    Line3d line_struct = {line, length};
    return line_struct;
}

