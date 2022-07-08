#include "missiles.h"
#include "assert.h"
#include "math.h"

#include <stdlib.h>

vector_t *vec_init_2(double x_component, double y_component);
vector_t missile_position(vector_t MAX, vector_t MIN);

body_t *missile_generate(vector_t MAX, vector_t MIN, double speed_m) {
    info_t *info = malloc(sizeof(info_t));
    assert(info != NULL);
    info->type = MISSILE;
    //info->control_velocity = (vector_t) {-200, 0};
    // Shape - hitbox
    list_t *missile_vertices = list_init(7, (free_func_t)free);
    list_add(missile_vertices, vec_init_2(20, 25));
    list_add(missile_vertices, vec_init_2(20, -25));
    list_add(missile_vertices, vec_init_2(0, -15));
    list_add(missile_vertices, vec_init_2(-30, -15));
    list_add(missile_vertices, vec_init_2(-50, 0));
    list_add(missile_vertices, vec_init_2(-30, 15));
    list_add(missile_vertices, vec_init_2(0, 15));
    // Front triangle
    list_t *front_triangle = list_init(3, (free_func_t)free);
    list_add(front_triangle, vec_init_2(-50, 0));
    list_add(front_triangle, vec_init_2(-30, 15));
    list_add(front_triangle, vec_init_2(-30, -15));
    // Rear
    list_t *rear = list_init(4, (free_func_t)free);
    list_add(rear, vec_init_2(20, 25));
    list_add(rear, vec_init_2(20, -25));
    list_add(rear, vec_init_2(0, -15));
    list_add(rear, vec_init_2(0, 15));
    // Rectangle
    list_t *rectangle = list_init(4, (free_func_t)free);
    list_add(rectangle, vec_init_2(-30, 15));
    list_add(rectangle, vec_init_2(-30, -15));
    list_add(rectangle, vec_init_2(0, -15));
    list_add(rectangle, vec_init_2(0, 15));
    // Shapes
    list_t *shapes = list_init(3, (free_func_t)list_free);
    list_add(shapes, front_triangle);
    list_add(shapes, rear);
    list_add(shapes, rectangle);
    // Colors
    list_t *colors = list_init(3, (free_func_t)free);
    list_add(colors, color_init(0, 255, 0));
    list_add(colors, color_init(0, 0, 255));
    list_add(colors, color_init(255, 0, 0));
    //Final combined shape
    body_t *missile = body_init_with_polygons(missile_vertices, shapes, colors, 1, info, (free_func_t) free);
    body_set_velocity(missile, (vector_t){-speed_m, 0});
    body_set_centroid(missile, missile_position(MAX,MIN));
    return missile;
}


vector_t *vec_init_2(double x_component, double y_component) {
    vector_t *vector = malloc(sizeof(vector_t));
    assert(vector != NULL);
    vector->x = x_component;
    vector->y = y_component;
    return vector;
}

// spawn position of missile in window
vector_t missile_position(vector_t MAX, vector_t MIN) {
    double max = MAX.y - (HEIGHT/2);
    double min = MIN.y + (HEIGHT/2);
    double r = (rand() % (int)(max + 1 - min)) + min;
    return (vector_t){SPAWN_X_POSITION, r};
}