#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define zoom 3
#define width zoom*50
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

typedef struct {
    Pos3d *points;
    unsigned int length;
} Line3d;

Pos project(Pos3d pos3d, Pos3d camera_pos);
Pos3d rotateX(Pos3d center, Pos3d point, float degrees);
Pos3d rotateY(Pos3d center, Pos3d point, float degrees);
Pos3d rotateZ(Pos3d center, Pos3d point, float degrees);
Line gen_line(Pos p1, Pos p2);
Line3d gen_line3d(Pos3d p1, Pos3d p2);
int data_file_length(char *path);
void read_data_file(char *path, Pos3d pairs[][2]);
void print_grid(char grid[height][width]);
void add_line(char grid[height][width], Line line);
int is_in_view(Pos3d pos3d, Pos3d camera);
int points_in_range(Line3d line, Pos3d in_range[2], Pos3d camera);


int main() {
    //Terminal settings to allow instant input
    //without having to press the enter key
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    
    char grid[height][width];
    for (int r=0; r<height; r++) {
        for (int c=0; c<width; c++) {
            grid[r][c] = ' ';
        }
    }

    //Change this to change the shape
    char *data_file = "pyramid.dat";

    int num_pairs = data_file_length(data_file);
    Pos3d pairs3d[num_pairs][2];
    read_data_file(data_file, pairs3d);

    Line3d lines[num_pairs];
    for (int i=0; i<num_pairs; i++) {
        lines[i] = gen_line3d(pairs3d[i][0], pairs3d[i][1]);
    }
    /*for (int i=0; i<num_pairs; i++) {
        for (int x=0; x<lines[i].length; x++) {
            printf("X:%.2f, Y:%.2f, Z:%.2f () ", lines[i].points[x].x, lines[i].points[x].y,lines[i].points[x].z);
        }
        puts("");
    }*/

    
    Pos3d centerX = {-1, 40, 40};
    Pos3d centerY = {40, -1, 40};
    Pos3d centerZ = {40, 40, -1};
    char input;
    Pos3d camera_pos = {50, 50, 300};
    int sign = 1;
    double rotation = 0;
    for (int count=0; count<1000; count++) {
        printf("ROT:%.2f\n", rotation);
        for (int i=0; i<num_pairs; i++) {
            printf("X:%.2f, Y:%.2f, Z:%.2f () ", pairs3d[i][0].x, pairs3d[i][0].y, pairs3d[i][0].z);
            printf("X:%.2f, Y:%.2f, Z:%.2f\n", pairs3d[i][1].x, pairs3d[i][1].y, pairs3d[i][1].z);
        }
        //printf("X: %.2f, Y: %.2f\n", camera_pos.x, camera_pos.y);
        input = ' ';
        read(STDIN_FILENO, &input, 1);
        switch (input) {
            case 'w': camera_pos.z -= 25*sin((90-rotation)*M_PI/180); camera_pos.x -= 25*cos((90-rotation)*M_PI/180); break;
            case 's': camera_pos.z += 25*sin((90-rotation)*M_PI/180); camera_pos.x += 25*cos((90-rotation)*M_PI/180); break;
            case 'd': camera_pos.z -= 25*sin(-rotation*M_PI/180); camera_pos.x -= 25*cos(-rotation*M_PI/180); break;
            case 'a': camera_pos.z += 25*sin(-rotation*M_PI/180); camera_pos.x += 25*cos(-rotation*M_PI/180); break;
            case 'e': camera_pos.y += 5; break;
            case 'q': camera_pos.y -= 5; break;
            case 'h': rotation -= 10; break;
            case 'l': rotation += 10; break;
        }
        for (int r=0; r<height; r++) {
            for (int c=0; c<width; c++) {
                grid[r][c] = ' ';
            }
        }
        for (int i=0; i<num_pairs; i++) {
            Pos3d p3d1 = rotateY(camera_pos, pairs3d[i][0], -rotation);
            Pos3d p3d2 = rotateY(camera_pos, pairs3d[i][1], -rotation);
            lines[i] = gen_line3d(p3d1, p3d2);
            Pos3d in_range_points[2] = {{9999, 9999, 9999}, {9999, 9999, 9999}};
//            puts("MARK1");
            if (points_in_range(lines[i], in_range_points, camera_pos)) {
                printf("X:%.2f, Y:%.2f () ", in_range_points[0].x, in_range_points[0].y);
                printf("X:%.2f, Y:%.2f\n", in_range_points[1].x, in_range_points[1].y);
                //sleep(1);
//                puts("MARK2");
                Pos p1 = project(in_range_points[0], camera_pos);
                Pos p2 = project(in_range_points[1], camera_pos);

                printf("X:%.2f, Y:%.2f () ", p1.x, p1.y);
                printf("X:%.2f, Y:%.2f\n", p2.x, p2.y);
                Line line = gen_line(p1, p2);
                add_line(grid, line);
                free(line.points);
                //printf("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)\n", pairs3d[i][0].x, pairs3d[i][0].y, pairs3d[i][0].z, pairs3d[i][1].x, pairs3d[i][1].y, pairs3d[i][1].z);
            }
        }

        //printf("(%.2f, %.2f, %.2f)\n", camera_pos.x, camera_pos.y, camera_pos.z);
        print_grid(grid);
        usleep(100000);
    }
    for (int i=0; i<num_pairs; i++) {
        free(lines[i].points);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}

Pos project(Pos3d pos3d, Pos3d camera_pos) {
    Pos pos2d = { -1, -1 };
    Pos3d relative = { pos3d.x-camera_pos.x, pos3d.y-camera_pos.y, pos3d.z-camera_pos.z };

    double scale;
    if (pos3d.z >= camera_pos.z)
        scale = 5;
    else
        scale = zoom*100/(relative.z - zoom*100);

    pos2d.x = scale*((relative.x)) + width/2.0;
    pos2d.y = scale*((relative.y)) + height/2.0;
    return pos2d;
}

int is_in_view(Pos3d pos3d, Pos3d camera_pos) {
    Pos3d relative = { pos3d.x-camera_pos.x, pos3d.y-camera_pos.y, pos3d.z-camera_pos.z };
    double tan_ratio = tan(M_PI/6);

    if (relative.z >= 0)
        return 0;
    return 1;
    if (fabsf(relative.x/relative.z) > tan_ratio)
        return 0;
    if (fabsf(relative.y/relative.z) > tan_ratio)
        return 0;
    return 1;
}

int points_in_range(Line3d line, Pos3d in_range[2], Pos3d camera) {
    int is_first = 1;
    for (int i=0; i<line.length; i++) {
        Pos3d p = line.points[i];
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
            line = (Pos *)malloc(length * sizeof(Pos));
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

Line3d gen_line3d(Pos3d p1, Pos3d p2) {
    unsigned int length = 1;
    Pos3d current_p = p1;

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
    Pos3d *line = (Pos3d *)malloc(length * sizeof(Pos3d));

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


//Returns all of the points from the data file
void read_data_file(char *path, Pos3d pairs[][2]) {
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
        unsigned int x = line.points[i].x;
        unsigned int y = line.points[i].y;
        if (y > 0 && y < height && x > 0 && x < width)
            grid[y][x] = 'X';
    }
}
