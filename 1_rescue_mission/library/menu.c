#include "menu.h"
#include "assert.h"
#include "math.h"

#include <stdlib.h>

body_t* start_screen_generator(vector_t MAX, vector_t MIN) {
  info_t *info = malloc(sizeof(info_t));
  assert(info != NULL);
  info->type = BACKGROUND;
  info->control_velocity = VEC_ZERO;
  list_t *shape = rectangle_vertices(1, 1, 0, VEC_ZERO);
  list_t *polygon_colors = list_init(1, (free_func_t)free);
  list_t *polygons = list_init(1, (free_func_t)free);

  list_t* back = rectangle_vertices(MAX.x,MAX.y,0,VEC_ZERO);
  rgb_color_t* back_c = color_init(123,46,46);
  list_add(polygons,back);
  list_add(polygon_colors,back_c);

  list_t* box = rectangle_vertices(MAX.x/2,MAX.y/2,0,VEC_ZERO);
  rgb_color_t* box_c = color_init(224,136,136);
  list_add(polygons,box);
  list_add(polygon_colors,box_c);

 

  body_t *start_screen = body_init_with_polygons(shape, polygons, polygon_colors, INFINITY,
                                       info, (free_func_t)free);
  body_set_centroid(start_screen, vec_multiply(0.5,MAX));
  return start_screen;
}

body_t* end_screen_generator(vector_t MAX, vector_t MIN) {
  info_t *info = malloc(sizeof(info_t));
  assert(info != NULL);
  info->type = BACKGROUND;
  info->control_velocity = VEC_ZERO;
  list_t *shape = rectangle_vertices(1, 1, 0, VEC_ZERO);
  list_t *polygon_colors = list_init(1, (free_func_t)free);
  list_t *polygons = list_init(1, (free_func_t)free);

  list_t* back = rectangle_vertices(MAX.x,MAX.y,0,VEC_ZERO);
  rgb_color_t* back_c = color_init(123,46,46);
  list_add(polygons,back);
  list_add(polygon_colors,back_c);

  list_t* box = rectangle_vertices(MAX.x/2,MAX.y/2,0,VEC_ZERO);
  rgb_color_t* box_c = color_init(224,136,136);
  list_add(polygons,box);
  list_add(polygon_colors,box_c);

  body_t *start_screen = body_init_with_polygons(shape, polygons, polygon_colors, INFINITY,
                                       info, (free_func_t)free);
  body_set_centroid(start_screen, vec_multiply(0.5,MAX));
  return start_screen;
}
