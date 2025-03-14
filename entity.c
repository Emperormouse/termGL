#include "headers/grid_h.h"
#include "headers/point_h.h"
#include "headers/file_h.h"
#include "headers/projection_h.h"
#include "headers/line_h.h"
#include "headers/entity_h.h"

Entity gen_entity(char *data_file_path) {
    int num_lines = data_file_length(data_file_path);
    Point3d (*pairs)[2] = (Point3d(*)[2])malloc(num_lines * sizeof(Point3d[2]));
    read_data_file(data_file_path, pairs);
    for (int i=0; i<num_lines; i++) {
        pairs[i][0].x *= zoom;
        pairs[i][0].y *= zoom;
        pairs[i][0].z *= zoom;
        pairs[i][1].x *= zoom;
        pairs[i][1].y *= zoom;
        pairs[i][1].z *= zoom;
    }

    //The first point represents the center of the entity
    Point3d (*only_pos_pairs)[2] = (Point3d(*)[2])malloc((num_lines-1) * sizeof(Point3d[2]));
    for (int i=0; i<num_lines-1; i++) {
        only_pos_pairs[i][0] = pairs[i+1][0];
        only_pos_pairs[i][1] = pairs[i+1][1];
    }
    Entity e = {num_lines-1, pairs[0][0], only_pos_pairs};
    return e;
}

void render_entity(Entity entity, Camera camera, char grid[height][width]) {
    Point3d p1;
    Point3d p2;
    Point p1_2d;
    Point p2_2d;
    Line3d line;
    for (int i=0; i<entity.num_lines; i++) {
        p1 = rotateY(camera.pos, entity.pairs[i][0], -camera.rot.x);
        p2 = rotateY(camera.pos, entity.pairs[i][1], -camera.rot.x);

        p1 = rotateX(camera.pos, p1, -camera.rot.y);
        p2 = rotateX(camera.pos, p2, -camera.rot.y);

        p1 = rotateZ(camera.pos, p1, -camera.rot.z);
        p2 = rotateZ(camera.pos, p2, -camera.rot.z);

        line = gen_line3d(p1, p2);
        Point3d in_range_ends[2] = {{9999, 9999, 9999}, {9999, 9999, 9999}}; //buffer

        if (points_in_range(line, in_range_ends, camera.pos)) {
            p1_2d = project(in_range_ends[0], camera.pos); 
            p2_2d = project(in_range_ends[1], camera.pos); 

            Line line2d = gen_line(p1_2d, p2_2d);
            add_line(grid, line2d);
        }
        free(line.points); 
    }

}

void rotate_entity(Point3d center, Entity entity, Point3d rot) {
    for (int i=0; i<entity.num_lines; i++) {
        if (rot.y != 0) {
            entity.pairs[i][0] = rotateX(center, entity.pairs[i][0], rot.y);
            entity.pairs[i][1] = rotateX(center, entity.pairs[i][0], rot.y);
        }

        if (rot.y != 0) {
            entity.pairs[i][0] = rotateY(center, entity.pairs[i][0], rot.x);
            entity.pairs[i][1] = rotateY(center, entity.pairs[i][0], rot.x);
        }

        if (rot.y != 0) {
            entity.pairs[i][0] = rotateZ(center, entity.pairs[i][0], rot.z);
            entity.pairs[i][1] = rotateZ(center, entity.pairs[i][0], rot.z);
        }
    }
}

void translate_entity(Entity entity, Point3d translation) {
    for (int i=0; i<entity.num_lines; i++) {
        entity.pairs[i][0].x += translation.x;
        entity.pairs[i][0].y += translation.y;
        entity.pairs[i][0].z += translation.z;

        entity.pairs[i][1].x += translation.x;
        entity.pairs[i][1].y += translation.y;
        entity.pairs[i][1].z += translation.z;
    }
}
