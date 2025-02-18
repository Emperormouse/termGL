#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define width 50
#define height 50

typedef struct {
    float x;
    float y;
} Pos;


typedef struct {
    Pos *points;
    unsigned int length;
} Line;

Pos rotate(Pos center, Pos point, float degrees) {
    float rad = (degrees * M_PI) / 180;

    float x = point.x - center.x;
    float y = point.y - center.y;

    float x_rot = x * cos(rad) + y * sin(rad);
    float y_rot = -x * sin(rad) + y * cos(rad);

    x_rot += center.x;
    y_rot += center.y;

    Pos new_pos = {x_rot, y_rot};
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

Pos* read_data_file(char *path, size_t *length) {
    Pos *pairs;
    FILE *file_ptr = fopen(path, "r");

    if (file_ptr == NULL) {
        printf("file can't be opened \n");
    }

    //FIRST PASS
    char buf;
    *length = 0;
    while ((buf = fgetc(file_ptr)) != EOF) {
        if (buf == '\n') (*length)++;
    }
    fclose(file_ptr);

    pairs = malloc(sizeof(Pos) * 2 * *length);
    //SECOND PASS
    file_ptr = fopen(path, "r");
    unsigned int current_pair = 0;
    Pos tmp_pair[2];
    int nums[4] = {0, 0, 0, 0};
    int idx = 0;
    int magnitude = 0;
    while ((buf = fgetc(file_ptr)) != EOF) {
        if (buf == '\n') {
            Pos p1 = {nums[0], nums[1]};
            Pos p2 = {nums[2], nums[3]};
            memcpy(pairs + current_pair*sizeof(Pos)*2, &p1, sizeof(Pos));
            memcpy(pairs + current_pair*sizeof(Pos)*2 + sizeof(Pos), &p2, sizeof(Pos));

            //cleanup
            current_pair++;
            idx = 0;
            magnitude = 0;
            for (int i=0; i<4; i++) nums[i] = 0;
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
    return pairs;

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
        grid[y][x] = 'X';
    }
}

int main() {
    char grid[height][width];
    for (int r=0; r<height; r++) {
        for (int c=0; c<width; c++) {
            grid[r][c] = ' ';
        }
    }

    size_t num_pairs;
    Pos *pairs = read_data_file("data.txt", &num_pairs);
    Pos center = {20, 20};

    for (int count=0; count<100; count++) {
        for (int r=0; r<height; r++) {
            for (int c=0; c<width; c++) {
                grid[r][c] = ' ';
            }
        }
        for (int i=0; i<num_pairs; i++) {
            Pos p1 = *(pairs + i*2*sizeof(Pos));
            Pos p2 = *(pairs + i*2*sizeof(Pos) + sizeof(Pos));
            Line line = gen_line(p1, p2);
            add_line(grid, line);
            free(line.points);

            p1 = rotate(center, p1, 5);
            p2 = rotate(center, p2, 5);
            memcpy(pairs + i*2*sizeof(Pos), &p1, sizeof(p1));
            memcpy(pairs + i*2*sizeof(Pos) + sizeof(Pos), &p2, sizeof(p2));
        }

        print_grid(grid);
        usleep(100000);
    }
    free(pairs);


    return 0;
}
