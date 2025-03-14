#ifndef entity_h
#define entity_h

#include "constants.h"
#include "point_h.h"
#include "line_h.h"
#include "projection_h.h"

typedef struct {
    int num_lines;
    Point3d center;
    Point3d (*pairs)[2];
} Entity;

Entity gen_entity(char *data_file_path);
void render_entity(Entity entity, Camera camera, char grid[height][width]);
void rotate_entity(Point3d center, Entity entity, Point3d rot);
void translate_entity(Entity entity, Point3d translation);

#endif
