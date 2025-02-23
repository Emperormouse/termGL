#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define zoom 3
#define width zoom*30
#define height zoom*30

typedef struct {
    float x;
    float y;
} Pos;

typedef struct {
    float x;
    float y;
    float z;
} Pos3d;


typedef struct {
    Pos *points;
    unsigned int length;
} Line;

Pos project(Pos3d pos3d);

Pos3d rotateX(Pos3d center, Pos3d point, float degrees);
Pos3d rotateY(Pos3d center, Pos3d point, float degrees);
Pos3d rotateZ(Pos3d center, Pos3d point, float degrees);

Line gen_line(Pos p1, Pos p2);

int data_file_length(char *path);
void read_data_file(char *path, int length, Pos3d pairs[length][2]);

void print_grid(char grid[height][width]);

void add_line(char grid[height][width], Line line);


int main() {
    char grid[height][width];
    for (int r=0; r<height; r++) {
        for (int c=0; c<width; c++) {
            grid[r][c] = ' ';
        }
    }

    //Change this to change the shape
    char *data_file = "cube.dat";

    int num_pairs = data_file_length(data_file);
    Pos3d pairs3d[num_pairs][2];
    read_data_file(data_file, num_pairs, pairs3d);

    
    Pos3d centerX = {-1, 40, 40};
    Pos3d centerY = {40, -1, 40};
    Pos3d centerZ = {40, 40, -1};
    for (int count=0; count<1000; count++) {
        for (int r=0; r<height; r++) {
            for (int c=0; c<width; c++) {
                grid[r][c] = ' ';
            }
        }
        for (int i=0; i<num_pairs; i++) {
            Pos p1 = project(pairs3d[i][0]);
            Pos p2 = project(pairs3d[i][1]);

            Line line = gen_line(p1, p2);
            add_line(grid, line);
            free(line.points);

            pairs3d[i][0] = rotateY(centerY, pairs3d[i][0], 5);
            pairs3d[i][1] = rotateY(centerY, pairs3d[i][1], 5);
            pairs3d[i][0] = rotateX(centerX, pairs3d[i][0], 5);
            pairs3d[i][1] = rotateX(centerX, pairs3d[i][1], 5);
            printf("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)\n", pairs3d[i][0].x, pairs3d[i][0].y, pairs3d[i][0].z, pairs3d[i][1].x, pairs3d[i][1].y, pairs3d[i][1].z);
        }

        print_grid(grid);
        usleep(100000);
    }
    //free(pairs3d);


    return 0;
}

Pos project(Pos3d pos3d) {
    Pos pos2d;
    pos2d.x = zoom*100*(pos3d.x/(400-pos3d.z));
    pos2d.y = zoom*100*(pos3d.y/(400-pos3d.z));
    return pos2d;
}

//Z axis
Pos3d rotateX(Pos3d center, Pos3d point, float degrees) {
    float rad = (degrees * M_PI) / 180;

    float z = point.z - center.z;
    float y = point.y - center.y;

    float z_rot = z * cos(rad) + y * sin(rad);
    float y_rot = -z * sin(rad) + y * cos(rad);

    z_rot += center.z;
    y_rot += center.y;

    Pos3d new_pos = {point.x, y_rot, z_rot};
    return  new_pos;
}

//Y axis
Pos3d rotateY(Pos3d center, Pos3d point, float degrees) {
    float rad = (degrees * M_PI) / 180;

    float x = point.x - center.x;
    float z = point.z - center.z;

    float x_rot = x * cos(rad) + z * sin(rad);
    float z_rot = -x * sin(rad) + z * cos(rad);

    x_rot += center.x;
    z_rot += center.z;

    Pos3d new_pos = {x_rot, point.y, z_rot};
    return  new_pos;
}

//X axis
Pos3d rotateZ(Pos3d center, Pos3d point, float degrees) {
    float rad = (degrees * M_PI) / 180;

    float x = point.x - center.x;
    float y = point.y - center.y;

    float x_rot = x * cos(rad) + y * sin(rad);
    float y_rot = -x * sin(rad) + y * cos(rad);

    x_rot += center.x;
    y_rot += center.y;

    Pos3d new_pos = {x_rot, y_rot, point.z};
    return  new_pos;
}


Line gen_line(Pos p1, Pos p2) {
    Pos *line;
    unsigned int length = 1;
    Pos current_p = p1;

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
            line = malloc(length * sizeof(Pos));
            current_p = p1;
        }
        while (roundf(current_p.x) != roundf(p2.x) || roundf(current_p.y) != roundf(p2.y)) {
            if (i == 0) 
                ++length;
            if (i == 1) {
                memcpy(line + sizeof(Pos) * (offset), &current_p, sizeof(current_p));
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
        if (i == 1)
            memcpy(line + sizeof(Pos) * (length-1), &current_p, sizeof(current_p));
    }

    Line line_struct = {line, length};
    return line_struct;
}

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

//Returns all of the points from the data file
void read_data_file(char *path, int length, Pos3d pairs[length][2]) {
    FILE *file_ptr = fopen(path, "r");

    if (file_ptr == NULL) {
        printf("file can't be opened \n");
    }

    unsigned int current_pair = 0;
    Pos3d tmp_pair[2];
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
            Pos3d p1 = {nums[0], nums[1], nums[2]};
            Pos3d p2 = {nums[3], nums[4], nums[5]};
            //printf("X: %.2f, Y: %.2f, Z: %.2f () ", p1.x, p1.y, p1.z);
            //printf(": %.2f, Y: %.2f, Z: %.2f\n", p2.x, p2.y, p2.z);
            pairs[current_pair][0] = p1;
            pairs[current_pair][1] = p2;
//            memcpy(pairs + current_pair*2*sizeof(Pos3d), &p1, sizeof(Pos3d));
//            memcpy(pairs + (current_pair*2 +1)*sizeof(Pos3d), &p2, sizeof(Pos3d));

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

void print_grid(char grid[height][width]) {
    char fmt_str[10];
    sprintf(fmt_str, "%%.%ds\n", width);
    for (int r=0; r<height; r++) {
        printf(fmt_str, grid[r]);
    }
}

void add_line(char grid[height][width], Line line) {
    for (int i=0; i<line.length; i++) {
        unsigned int x = (line.points + i*sizeof(Pos))->x;
        unsigned int y = (line.points + i*sizeof(Pos))->y;
        if (y > 0 && y < height && x > 0 && x < width)
            grid[y][x] = 'X';
    }
}
