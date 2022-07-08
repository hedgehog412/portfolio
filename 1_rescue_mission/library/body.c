#include "body.h"
#include "assert.h"
#include "math.h"


#include <stdlib.h>

const size_t INITIAL_SIZE = 1;

struct body {
  list_t *shape;
  list_t *polygons;
  list_t *polygon_colors;
  double mass;
  vector_t velocity;
  vector_t forces;
  vector_t impulses;
  void *info;
  free_func_t info_freer;
  bool removed;
};

list_t *malloc_shape(list_t *shape);

body_t *body_init(list_t *shape, double mass, rgb_color_t *color) {
  return body_init_with_info(shape, mass, color, NULL, NULL);
}

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t *color,
                            void *info, free_func_t info_freer) {
  // translate the shape to (0,0) in order for transformation to be backwards compatable
  vector_t centroid = polygon_centroid(shape);
  list_t *polygon = malloc_shape(shape);
  polygon_translate(polygon, vec_negate(centroid));

  // creates the list of polygons (just shape)
  list_t *polygons = list_init(1, (free_func_t)list_free);
  list_add(polygons, polygon);

  // creates the list of polygon colors (just one color)
  list_t *polygon_colors = list_init(1, (free_func_t)free);
  list_add(polygon_colors, color);

  return body_init_with_polygons(shape, polygons, polygon_colors, mass, info, info_freer);
}

body_t *body_init_with_polygons(list_t *shape, list_t *polygons, list_t *polygon_colors, double mass,
                            void *info, free_func_t info_freer){
  body_t *body = body_init_polygons_no_translate(shape, polygons, polygon_colors, mass, info, info_freer);

  // place all the polygons in the correct position
  for (size_t i = 0; i < list_size(body->polygons); i++){
    polygon_translate(list_get(body->polygons, i), body_get_centroid(body));
  }
  
  return body;
}

body_t* body_init_polygons_no_translate(list_t *shape, list_t *polygons, list_t *polygon_colors, double mass,
                            void *info, free_func_t info_freer){
  assert(list_size(polygons)==list_size(polygon_colors));
  assert(mass > 0);
  body_t *body = malloc(sizeof(body_t));
  assert(body != NULL);

  body->polygons = polygons;
  body->polygon_colors = polygon_colors;
  body->shape = shape;
  body->mass = mass;
  body->velocity = VEC_ZERO;
  body->forces = VEC_ZERO;
  body->impulses = VEC_ZERO;
  body->info = info;
  body->info_freer = info_freer;
  body->removed = false;
  return body;
}
    
void body_free(body_t *body) {
  list_free(body->shape);
  list_free(body->polygons);
  list_free(body->polygon_colors);
  if (body->info_freer != NULL && body->info != NULL) {
    (*body->info_freer)(body->info);
  }
  free(body);
}

list_t *body_get_shape(body_t *body) {
  return malloc_shape(body->shape);
}

rgb_color_t body_get_color(body_t *body){
    return *((rgb_color_t *)(list_get(body->polygon_colors, 0)));
}

list_t *body_get_polyons(body_t *body) {
  list_t *result = list_init(list_size(body->polygons), (free_func_t)list_free);
  for (size_t i = 0; i < list_size(body->polygons); i++) {
    list_t *shape = list_get(body->polygons, i);
    list_add(result, malloc_shape(shape)); 
  }
  return result;
}

vector_t body_get_centroid(body_t *body) {
  return polygon_centroid(body->shape);
}

vector_t body_get_velocity(body_t *body) { return body->velocity; }

void *body_get_info(body_t *body) { return body->info; }

double body_get_mass(body_t *body) { return body->mass; }

void body_set_centroid(body_t *body, vector_t x) {
  vector_t centroid = body_get_centroid(body);
  polygon_translate(body->shape, vec_add(x,vec_negate(centroid)));
  for (size_t i = 0; i < list_size(body->polygons); i++){
    polygon_translate(list_get(body->polygons, i), vec_add(x,vec_negate(centroid)));
  }
}

void body_set_velocity(body_t *body, vector_t v) { body->velocity = v; }

void body_set_rotation(body_t *body, double angle) {
  vector_t centroid = body_get_centroid(body);
  polygon_rotate(body->shape, angle, centroid);
  for (size_t i = 0; i < list_size(body->polygons); i++) {
    list_t *shape = list_get(body->polygons, i);
    polygon_rotate(shape, angle, centroid);
  }
}

void body_add_force(body_t *body, vector_t force) {
  body->forces = vec_add(body->forces, force);
}

void body_add_impulse(body_t *body, vector_t impulse) {
  body->impulses = vec_add(body->impulses, impulse);
}

void body_tick(body_t *body, double dt) {
  vector_t delta_velocity =
      vec_multiply(dt / body_get_mass(body), body->forces);
  delta_velocity = vec_add(
      delta_velocity, vec_multiply(1 / body_get_mass(body), body->impulses));
  vector_t new_velocity = vec_add(body_get_velocity(body), delta_velocity);
  vector_t old_position = body_get_centroid(body); 
  vector_t new_position = vec_add(
      old_position,
      vec_multiply(dt / 2, vec_add(body_get_velocity(body), new_velocity)));
  body_set_centroid(body, new_position);
  body_set_velocity(body, new_velocity);
  
  body->forces = VEC_ZERO;
  body->impulses = VEC_ZERO;
}

void body_remove(body_t *body) { body->removed = true; }

bool body_is_removed(body_t *body) { return body->removed; }

void body_set_color(body_t *body, rgb_color_t color) { 
  
  for (size_t i=0; i<list_size(body->polygon_colors); i++){
    rgb_color_t *color_init = list_get(body->polygon_colors, i);
    color_init->r = color.r;
    color_init->g = color.g;
    color_init->b = color.b;
  }

}

rgb_color_t body_get_polygon_color(body_t *body, size_t index){
  assert(index<list_size(body->polygon_colors));
  return *((rgb_color_t *)list_get(body->polygon_colors, index));
}

void body_add_polygon(body_t *body, list_t *polygon, rgb_color_t *color){
    list_add(body->polygons, polygon);
    list_add(body->polygon_colors, color);
}

list_t *malloc_shape(list_t *shape){
  list_t *new_shape = list_init(list_size(shape), (free_func_t)free);
    for (size_t j = 0; j < list_size(shape); j++){
      vector_t *new_vec = malloc_vector(*((vector_t *)list_get(shape, j)));
      list_add(new_shape, new_vec);
    }
  return new_shape;
}

// Returns list of vertices of a rectangle centered at an origin and rotated by an angle
list_t *rectangle_vertices(double length, double width, double angle, vector_t origin) {
  list_t *block_vertices = list_init(4, (free_func_t)free);
  
  vector_t *L_U = malloc(sizeof(vector_t));
  assert(L_U != NULL);
  L_U->x = 0;
  L_U->y = width;
  list_add(block_vertices, L_U);
  vector_t *R_U = malloc(sizeof(vector_t));
  assert(R_U != NULL);
  R_U->x = length;
  R_U->y = width;
  list_add(block_vertices, R_U);
  vector_t *R_D = malloc(sizeof(vector_t));
  assert(R_D != NULL);
  R_D->x = length;
  R_D->y = 0;
  list_add(block_vertices, R_D);
  vector_t *L_D = malloc(sizeof(vector_t));
  assert(L_D != NULL);
  L_D->x = 0;
  L_D->y = 0;
  list_add(block_vertices, L_D);
  polygon_translate(block_vertices, (vector_t){-length/2,-width/2});
  polygon_rotate(block_vertices, angle, VEC_ZERO);
  polygon_translate(block_vertices, origin);
  return block_vertices;
}

// Returns list of vertices for a circle centered at an origin
list_t *circle_verticies(size_t n_points, double radius, vector_t origin) {
  list_t *circle = list_init(n_points, (free_func_t)free);
  vector_t initial_vector = {radius, 0};
  for (size_t i = 0; i < n_points; i++) {
    vector_t *add_vertex = malloc_vector(vec_rotate(initial_vector, i * 2 * M_PI / n_points));
    list_add(circle, add_vertex);
  }
  polygon_translate(circle, origin);
  return circle;
}

void add_vert_semi_circles (list_t *list, vector_t displacement, double height, size_t num_vertices) {
    vector_t initial_vector = {0, -(height/2)};
    for (size_t i = 0; i <= num_vertices; i++) {
        vector_t *add_vertex = malloc(sizeof(vector_t));
        assert(add_vertex != NULL);
        add_vertex->x = vec_rotate(initial_vector, i * (-M_PI / num_vertices)).x;
        add_vertex->y = vec_rotate(initial_vector, i * (-M_PI / num_vertices)).y;

        vector_t *add_vertex_moved = malloc(sizeof(vector_t));
        assert(add_vertex_moved != NULL);
        add_vertex_moved->x = vec_add((vector_t){0, height/2}, vec_add(*add_vertex, displacement)).x;
        add_vertex_moved->y = vec_add((vector_t){0, height/2}, vec_add(*add_vertex, displacement)).y;
        list_add(list, add_vertex_moved);
        free(add_vertex);
    }
}

void add_vert_counter_semi_circles (list_t *list, vector_t displacement, double height, size_t num_vertices) {
    vector_t initial_vector = {0, -(height/2)};
    for (size_t i = 0; i <= num_vertices; i++) {
        vector_t *add_vertex = malloc(sizeof(vector_t));
        assert(add_vertex != NULL);
        add_vertex->x = vec_rotate(initial_vector, i * (M_PI / num_vertices)).x;
        add_vertex->y = vec_rotate(initial_vector, i * (M_PI / num_vertices)).y;

        vector_t *add_vertex_moved = malloc(sizeof(vector_t));
        assert(add_vertex_moved != NULL);
        add_vertex_moved->x = vec_add((vector_t){0, height/2}, vec_add(*add_vertex, displacement)).x;
        add_vertex_moved->y = vec_add((vector_t){0, height/2}, vec_add(*add_vertex, displacement)).y;
        list_add(list, add_vertex_moved);
        free(add_vertex);
    }
}