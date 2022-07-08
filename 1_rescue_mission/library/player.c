#include "player.h"
#include "assert.h"
#include "math.h"


#include <stdlib.h>

list_t *player_shape();

// generated the player, given a hitbox on which the physics acts
body_t *player_generator() {
  info_t *info = malloc(sizeof(info_t));
  assert(info != NULL);
  info->type = PLAYER;
  info->control_velocity = P_REGULAR_VELOCITY;
  // Create the shapes for the player
  list_t *polygons = player_shape();
  list_t *hitbox = rectangle_vertices(LENGTH_P, WIDTH_P, 0, VEC_ZERO); // hitbox for the player
  // Create the list of colors for the player
  list_t *polygon_colors = list_init(list_size(polygons), (free_func_t)free);
  list_add(polygon_colors, color_init(255,0,0)); // cape
  list_add(polygon_colors, color_init(0,0,0)); // arm
  list_add(polygon_colors, color_init(0,0,0)); // leg
  list_add(polygon_colors, color_init(0,0,0)); // body
  list_add(polygon_colors, color_init(246,200,112)); // head
  // Create player and return
  body_t *player = body_init_with_polygons(hitbox, polygons, polygon_colors, P_MASS, info, (free_func_t)free);
  body_set_centroid(player, PLAYER_INIT_POSITION);
  return player;
}

// Returns the list of polygons that make up superman
list_t *player_shape(){
  // necessary constants
  size_t head_verticies = 20;
  double body_length = 100;
  double body_width = body_length/20;
  double head_radius = body_length/5;
  double leg_length = body_length/2;
  double arm_length = body_length/2;
  double arm_position = body_length/4;
  double arm_angle = -M_PI/4;
  double leg_angle = -M_PI/3;
  double cape_length = body_length;
  double cape_angle1 = M_PI/6;
  double cape_angle2 = M_PI/3;
  // creating polygons
  list_t *cape = list_init(4, (free_func_t)free);
  list_add(cape, malloc_vector((vector_t){arm_position+body_width/2, 0}));
  list_add(cape, malloc_vector((vector_t){arm_position+body_width/2-(cape_length*cos(cape_angle2)), (cape_length*sin(cape_angle2))}));
  list_add(cape, malloc_vector((vector_t){arm_position-body_width/2-(cape_length*cos(cape_angle1)), (cape_length*sin(cape_angle1))}));
  list_add(cape, malloc_vector((vector_t){arm_position-body_width/2, 0}));
  list_t *body = rectangle_vertices(body_length, body_width, 0, VEC_ZERO);
  list_t *head = circle_verticies(head_verticies, head_radius, (vector_t){body_length/2, 0});
  list_t *arm = rectangle_vertices(arm_length, body_width, arm_angle, (vector_t){arm_length*cos(arm_angle)/2 + arm_position, +arm_length*sin(arm_angle)/2});
  list_t *leg = rectangle_vertices(leg_length, body_width, leg_angle, (vector_t){-leg_length*cos(leg_angle)/2 - body_length/2, -leg_length*sin(leg_angle)/2});
  // rotating shape
  polygon_rotate(arm, M_PI/4, VEC_ZERO);
  polygon_rotate(leg, M_PI/4, VEC_ZERO);
  polygon_rotate(cape, M_PI/4, VEC_ZERO);
  polygon_rotate(body, M_PI/4, VEC_ZERO);
  polygon_rotate(head, M_PI/4, VEC_ZERO);
  // creating polygon list
  list_t *polygon_list = list_init(4,(free_func_t)list_free);
  list_add(polygon_list,cape);
  list_add(polygon_list,arm);
  list_add(polygon_list,leg);
  list_add(polygon_list,body);
  list_add(polygon_list,head);
  return polygon_list;
}
