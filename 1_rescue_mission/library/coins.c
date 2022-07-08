#include "coins.h"
#include "assert.h"
#include "math.h"

#include <stdlib.h>

vector_t *vec_init_3(double x_component, double y_component);
vector_t coin_position(vector_t MAX, vector_t MIN);

body_t *coin_generate(vector_t MAX, vector_t MIN, double speed_c) {
    info_t *info = malloc(sizeof(info_t));
    assert(info != NULL);
    info->type = COIN;
    // Shape - hitbox
    list_t *coin_vertices = list_init(CIRCLE_VERTICES_C, (free_func_t)free);
    vector_t *initial = vec_init_3(RADIUS_1, 0);
    list_add(coin_vertices, initial);
    for (size_t i = 1; i <= CIRCLE_VERTICES_C; i++) {
        vector_t *add = vec_init_3(vec_rotate(*initial, i*2*M_PI/CIRCLE_VERTICES_C).x, vec_rotate(*initial, i*2*M_PI/CIRCLE_VERTICES_C).y);
        list_add(coin_vertices, add);
    }
    // Large circle
    list_t *coin_vertices_1 = list_init(CIRCLE_VERTICES_C, (free_func_t)free);
    vector_t *initial_1 = vec_init_3(RADIUS_1, 0);
    list_add(coin_vertices_1, initial_1);
    for (size_t i = 1; i <= CIRCLE_VERTICES_C; i++) {
        vector_t *add = vec_init_3(vec_rotate(*initial_1, i*2*M_PI/CIRCLE_VERTICES_C).x, vec_rotate(*initial_1, i*2*M_PI/CIRCLE_VERTICES_C).y);
        list_add(coin_vertices_1, add);
    }
    // Small circle
    list_t *coin_vertices_2 = list_init(CIRCLE_VERTICES_C, (free_func_t)free);
    vector_t *initial_2 = vec_init_3(RADIUS_2, 0);
    list_add(coin_vertices_2, initial_2);
    for (size_t i = 1; i <= CIRCLE_VERTICES_C; i++) {
        vector_t *add = vec_init_3(vec_rotate(*initial_2, i*2*M_PI/CIRCLE_VERTICES_C).x, vec_rotate(*initial_2, i*2*M_PI/CIRCLE_VERTICES_C).y);
        list_add(coin_vertices_2, add);
    }
    // Shapes
    list_t *shapes = list_init(2, (free_func_t)list_free);
    list_add(shapes, coin_vertices_1);
    list_add(shapes, coin_vertices_2);
    // Colors
    list_t *colors = list_init(2, (free_func_t)free);
    list_add(colors, color_init(139, 128, 0));
    list_add(colors, color_init(255, 215, 0));
    //Final combined shape
    body_t *coin = body_init_with_polygons(coin_vertices, shapes, colors, 1, info, (free_func_t) free);
    body_set_velocity(coin, (vector_t){-speed_c, 0});
    body_set_centroid(coin, coin_position(MAX,MIN));
    return coin;
}

body_t *coin_power_generate(vector_t MAX, vector_t MIN, double speed_c) {
    info_t *info = malloc(sizeof(info_t));
    assert(info != NULL);
    info->type = COINP;
    list_t *shapes = list_init(2, (free_func_t)list_free);
    list_t *colors = list_init(2, (free_func_t)free);

    list_t *outer_circle = circle_verticies(CIRCLE_VERTICES_C, 50, VEC_ZERO);
    list_add(shapes, outer_circle);
    rgb_color_t *color_outer = color_init(0,0,255);
    list_add(colors,color_outer);
    list_t *inner_circle = circle_verticies(CIRCLE_VERTICES_C, 25, VEC_ZERO);
    list_add(shapes,inner_circle);
    rgb_color_t *color_inner = color_init(0,0,125);
    list_add(colors,color_inner);

    body_t *coin = body_init_with_polygons(circle_verticies(CIRCLE_VERTICES_C, 50, VEC_ZERO), shapes, colors, 1, info, (free_func_t) free);
    body_set_velocity(coin, (vector_t){-speed_c, 0});
    body_set_centroid(coin, coin_position(MAX,MIN));
    return coin;

}

vector_t *vec_init_3(double x_component, double y_component) {
    vector_t *vector = malloc(sizeof(vector_t));
    assert(vector != NULL);
    vector->x = x_component;
    vector->y = y_component;
    return vector;
}

// spawn position of missile in window
vector_t coin_position(vector_t MAX, vector_t MIN) {
    double max = MAX.y - 50;
    double min = MIN.y + 50;
    double r = (rand() % (int)(max + 1 - min)) + min;
    return (vector_t){SPAWN_X_POSITION_C, r};
}