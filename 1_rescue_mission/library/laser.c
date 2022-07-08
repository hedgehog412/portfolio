#include "laser.h"
#include "assert.h"
#include "math.h"
#include "time.h"


#include <stdlib.h>

double rand_position_l(vector_t max, vector_t min);

body_t *laser_generator(vector_t max, vector_t min){
    info_t *info = malloc(sizeof(info_t));
    assert(info != NULL);
    info->type = LASER;
    info->control_velocity = VEC_ZERO;

    list_t *polygons = list_init(2, (free_func_t) free);
    list_t *polygon_colors = list_init(2, (free_func_t) free);
    double laser_y = rand_position_l(max, min);
    vector_t l_grey_position = {min.x + WIDTH_L, laser_y};
    vector_t r_grey_position = {max.x - WIDTH_L, laser_y};
    vector_t l_red_position = {min.x + WIDTH_L * 2, laser_y};
    vector_t r_red_position = {max.x - WIDTH_L*2, laser_y};
    vector_t laser_position = {min.x + WIDTH_L * 2 + HEIGHT_L/2, laser_y + HEIGHT_L/2};

    list_t *l_grey_head = rectangle_vertices(WIDTH_L, HEIGHT_L, 0, (vector_t){WIDTH_L/2, HEIGHT_L/2});
    add_vert_semi_circles(l_grey_head, VEC_ZERO, HEIGHT_L, NUM_VERTICES_L);
    
    list_t *r_grey_head = list_init(10, (free_func_t) free);
    for(size_t i = 0; i < list_size(l_grey_head); i++){
        vector_t *new_v = malloc(sizeof(vector_t));
        *new_v = *(vector_t *)list_get(l_grey_head, i);
        list_add(r_grey_head, new_v);
    }
    polygon_rotate(r_grey_head, M_PI, (vector_t){0, HEIGHT_L/2});

    list_t *l_red_head = list_init(2, (free_func_t) free);
    add_vert_counter_semi_circles(l_red_head, VEC_ZERO, HEIGHT_L, NUM_VERTICES_L);

    list_t *r_red_head = list_init(2, (free_func_t) free);
    add_vert_semi_circles(r_red_head, VEC_ZERO, HEIGHT_L, NUM_VERTICES_L);

    double laser_width = max.x - min.x - 4 * WIDTH_L - HEIGHT_L;
    list_t *laser = rectangle_vertices(laser_width, FINAL_L, 0, (vector_t){laser_width/2, INIT_L/2});

    polygon_translate(laser, laser_position);

    polygon_translate(l_grey_head, l_grey_position);
    list_add(polygons, l_grey_head);
    list_add(polygon_colors, color_init(GRAY_R, GRAY_G, GRAY_B));

    polygon_translate(l_red_head, l_red_position);
    list_add(polygons, l_red_head);
    list_add(polygon_colors, color_init(255, 0, 0));

    polygon_translate(r_grey_head, r_grey_position);
    list_add(polygons, r_grey_head);
    list_add(polygon_colors, color_init(GRAY_R, GRAY_G, GRAY_B));

    polygon_translate(r_red_head, r_red_position);
    list_add(polygons, r_red_head);
    list_add(polygon_colors, color_init(255, 0, 0));

    body_t *body = body_init_polygons_no_translate(laser, polygons, polygon_colors, 5, info, (free_func_t) free);
    return body;
}

void expand_rec(list_t * rec){
    double translation = (FINAL_L - INIT_L)/2;
    for(size_t i = 0; i<=1; i++){
        ((vector_t *)list_get(rec, i))->y = ((vector_t *)list_get(rec, i))->y + translation;
        ((vector_t *)list_get(rec, i + 2))->y = ((vector_t *)list_get(rec, i+2))->y - translation;
    }
}

double rand_position_l(vector_t max, vector_t min){
    srand(time(NULL));
    return ((double)(rand() % (int)(max.y - HEIGHT_L * 2 + 1 - min.y)) + min.y + HEIGHT_L);
}