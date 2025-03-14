#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "headers/point_h.h"
#include "headers/constants.h"
#include "headers/grid_h.h"
#include "headers/projection_h.h"
#include "headers/entity_h.h"

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
            if (r == 0)
                grid[r][c] = '#';
            else
                grid[r][c] = ' ';
        }
    }

    Entity cube = gen_entity("cube.dat");
    Entity pyramid = gen_entity("pyramid.dat");

    Point3d translation = {200, 0, 100};
    translate_entity(pyramid, translation);
    
    char input;
    Point3d camera_pos = {50*zoom, 50*zoom, 100*zoom};
    Point3d camera_rot = {0, 0, 0};
    Camera camera = {camera_pos, camera_rot};
    while (input != 'Q') {
        input = ' ';
        for (int i=0; i<3; i++)
            read(STDIN_FILENO, &input, 1);
        switch (input) {
            case 'w': camera.pos.z -= 25*sin((90-camera.rot.x)*M_PI/180); 
                      camera.pos.x -= 25*cos((90-camera.rot.x)*M_PI/180); 
                      break;
            case 's': camera.pos.z += 25*sin((90-camera.rot.x)*M_PI/180); 
                      camera.pos.x += 25*cos((90-camera.rot.x)*M_PI/180); 
                      break;
            case 'd': camera.pos.z -= 25*sin(-camera.rot.x*M_PI/180); 
                      camera.pos.x -= 25*cos(-camera.rot.x*M_PI/180); 
                      break;
            case 'a': camera.pos.z += 25*sin(-camera.rot.x*M_PI/180); 
                      camera.pos.x += 25*cos(-camera.rot.x*M_PI/180); 
                      break;

            case 'e': camera.pos.y += 5; break;
            case 'q': camera.pos.y -= 5; break;
            case 'h': camera.rot.x -= 10; break;
            case 'l': camera.rot.x += 10; break;
            case 'j': camera.rot.y -= 10; break;
            case 'k': camera.rot.y += 10; break;
        }
        for (int r=0; r<height; r++) {
            for (int c=0; c<width; c++) {
                grid[r][c] = ' ';
            }
        }

        render_entity(cube, camera, grid);
        render_entity(pyramid, camera, grid);

        //printf("(%.2f, %.2f, %.2f)\n", camera_pos.x, camera_pos.y, camera_pos.z);
        print_grid(grid);
        usleep(100000);
    }
    free(cube.pairs);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}

