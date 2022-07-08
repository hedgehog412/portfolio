#include "background.h"
#include "assert.h"
#include "math.h"

#include <stdlib.h>

body_t* background_generator_back(vector_t MAX, vector_t MIN) {
  info_t *info = malloc(sizeof(info_t));
  assert(info != NULL);
  info->type = BACKGROUND;
  info->control_velocity = VEC_ZERO;
  list_t *shape = rectangle_vertices(1, 1, 0, VEC_ZERO); // hitbox for the player
  list_t *polygon_colors = list_init(1, (free_func_t)free);
  list_t *polygons = list_init(1, (free_func_t)free);


  list_t* sky = rectangle_vertices(MAX.x,MAX.y,0,VEC_ZERO);
  rgb_color_t* sky_c = color_init(39,156,197);
  list_add(polygons,sky);
  list_add(polygon_colors,sky_c);

  list_t* cloud1 = circle_verticies(BACKGR_CIRCLE_VERT,50,(vector_t){600,400});
  list_t* cloud2 = circle_verticies(BACKGR_CIRCLE_VERT,50*(2.0/3.0),(vector_t){600-50,400});
  list_t* cloud3 = circle_verticies(BACKGR_CIRCLE_VERT,50,(vector_t){600+50,400});
  list_t* cloud4 = circle_verticies(BACKGR_CIRCLE_VERT,50/2,(vector_t){600-50*(5.0/3.0),400});
  list_t* cloud5 = circle_verticies(BACKGR_CIRCLE_VERT,50/2,(vector_t){600-50*(5.0/3.0),400});
  for (size_t i = 0; i < 5; i++){
    list_add(polygon_colors,color_init(196,204,207));
  }
  list_add(polygons,cloud1);
  list_add(polygons,cloud2);
  list_add(polygons,cloud3);
  list_add(polygons,cloud4);
  list_add(polygons,cloud5);

  body_t *background = body_init_with_polygons(shape, polygons, polygon_colors, INFINITY,
                                       info, (free_func_t)free);
  body_set_centroid(background, vec_multiply(0.5,MAX));
  return background;
}

body_t* background_generator_front(vector_t MAX, vector_t MIN) {
  info_t *info = malloc(sizeof(info_t));
  assert(info != NULL);
  info->type = BACKGROUND;
  info->control_velocity = VEC_ZERO;
  list_t *shape = rectangle_vertices(1, 1, 0, VEC_ZERO); // hitbox for the player
  list_t *polygon_colors = list_init(1, (free_func_t)free);
  list_t *polygons = list_init(1, (free_func_t)free);

  list_t* pavement = rectangle_vertices(MAX.x,200,0,(vector_t){0,-300});
  rgb_color_t* pavement_c = color_init(178,102,42);
  list_add(polygons,pavement);
  list_add(polygon_colors,pavement_c);

  list_t* wall = rectangle_vertices(MAX.x,100,0,(vector_t){0,-450});
  rgb_color_t* wall_c = color_init(103,102,113);
  list_add(polygons,wall);
  list_add(polygon_colors,wall_c);


  body_t *background = body_init_with_polygons(shape, polygons, polygon_colors, 1,
                                       info, (free_func_t)free);
  body_set_centroid(background, vec_multiply(0.5,MAX));
  return background;
}

body_t *background_generator_mountain(double radius, vector_t origin, vector_t drift_vel){
  info_t *info = malloc(sizeof(info_t));
  assert(info != NULL);
  info->type = BACKGROUND;
  info->control_velocity = VEC_ZERO;
  body_t* mountain = body_init_with_info(circle_verticies(BACKGR_CIRCLE_VERT,radius,origin), 1,  color_init(140,89,68), info, (free_func_t)free);
  body_set_velocity(mountain, drift_vel);
  return mountain;
}

body_t *background_generator_bush(double radius, vector_t origin, vector_t drift_vel){
  info_t *info = malloc(sizeof(info_t));
  assert(info != NULL);
  info->type = BACKGROUND;
  info->control_velocity = VEC_ZERO;
  body_t* bush = body_init_with_info(circle_verticies(BACKGR_CIRCLE_VERT,radius,origin), 1,  color_init(106,182,24), info, (free_func_t)free);
  body_set_velocity(bush, drift_vel);
  return bush;
}

void background_init(scene_t *scene, vector_t MAX, vector_t MIN){

  vector_t BUSH_VEL = {-100,0};
  vector_t MOUNTAIN_VEL = {-50,0};
  scene_add_body(scene, background_generator_back(MAX, MIN));
  scene_add_body(scene, background_generator_mountain(200,((vector_t){0,300}),MOUNTAIN_VEL));
  scene_add_body(scene, background_generator_mountain(500,((vector_t){500,300}),MOUNTAIN_VEL));
  scene_add_body(scene, background_generator_mountain(300,((vector_t){1100,300}),MOUNTAIN_VEL));
  scene_add_body(scene, background_generator_mountain(400,((vector_t){1400,300}),MOUNTAIN_VEL));
  scene_add_body(scene, background_generator_mountain(400,((vector_t){2000,300}),MOUNTAIN_VEL));
  scene_add_body(scene, background_generator_mountain(500,((vector_t){2500,300}),MOUNTAIN_VEL));
  scene_add_body(scene, background_generator_bush(200,((vector_t){600,300}),BUSH_VEL));
  scene_add_body(scene, background_generator_bush(100,((vector_t){1200,300}),BUSH_VEL));
  scene_add_body(scene, background_generator_bush(200,((vector_t){1700,300}),BUSH_VEL));
  scene_add_body(scene, background_generator_bush(150,((vector_t){2500,300}),BUSH_VEL));
  scene_add_body(scene, background_generator_front(MAX, MIN));
  
}

void background_wraparound(body_t *body){
  vector_t centroid = body_get_centroid(body);
  if (centroid.x < -500){
    body_set_centroid(body,(vector_t){2500,centroid.y});
  }
}


  