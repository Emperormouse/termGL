#include <stdio.h>
#include "headers/point_h.h"

int data_file_length(char *path) {
    FILE *file_ptr = fopen(path, "r");

    if (file_ptr == NULL) {
        printf("file can't be opened \n");
    }

    char buf;
    int length = 0;
    while ((buf = fgetc(file_ptr)) != EOF) {
        if (buf == '-') {
            while (fgetc(file_ptr) != '\n');
            continue;
        }
        if (buf == '\n') (length)++;
    }
    fclose(file_ptr);

    return length;
}

void read_data_file(char *path, Point3d pairs[][2]) {
    FILE *file_ptr = fopen(path, "r");

    if (file_ptr == NULL) {
        printf("file can't be opened \n");
    }

    unsigned int current_pair = 0;
    Point3d tmp_pair[2];
    int nums[6] = {0, 0, 0, 0, 0, 0};
    int idx = 0;
    int magnitude = 0;
    char buf;
    while ((buf = fgetc(file_ptr)) != EOF) {
        if (buf == '-') {
            while (fgetc(file_ptr) != '\n');
            continue;
        }
        if (buf == '\n') {
            Point3d p1 = {(float)nums[0], (float)nums[1], (float)nums[2]};
            Point3d p2 = {(float)nums[3], (float)nums[4], (float)nums[5]};
            //printf("X: %.2f, Y: %.2f, Z: %.2f () ", p1.x, p1.y, p1.z);
            //printf(": %.2f, Y: %.2f, Z: %.2f\n", p2.x, p2.y, p2.z);
            pairs[current_pair][0] = p1;
            pairs[current_pair][1] = p2;
//            memcpy(pairs + current_pair*2*sizeof(Point3d), &p1, sizeof(Point3d));
//            memcpy(pairs + (current_pair*2 +1)*sizeof(Point3d), &p2, sizeof(Point3d));

            current_pair++;
            //cleanup
            idx = 0;
            magnitude = 0;
            for (int i=0; i<6; i++) nums[i] = 0;
            continue;
        }

        if (buf == ':' || buf == ' ') {
            idx++;
            magnitude = 0;
        } else {
            if (magnitude > 0) {
                nums[idx] *= 10;
            }
            nums[idx] += (buf - 48);
            magnitude++;
        }
    }
}

