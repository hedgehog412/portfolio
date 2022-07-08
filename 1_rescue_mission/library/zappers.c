#include "zappers.h"
#include "assert.h"
#include "math.h"


#include <stdlib.h>

//random length
double rand_zapper_length();
// spawn position of zapper in window
vector_t zapper_position(vector_t MAX, vector_t MIN, size_t length);
// adds the vertices of the semi-circle to a list corresponding to the vertices of a zapper
void add_semi_circles (list_t *list, vector_t displacement);
void add_counter_semi_circles (list_t *list, vector_t displacement);
void rotate_zapper(body_t *body);


// generates a zapper based on a list of vertices and a position
body_t *zapper_generate(vector_t MAX, vector_t MIN, double speed_z) {
    info_t *info = malloc(sizeof(info_t));
    assert(info != NULL);
    info->type = ZAPPER;

    double length = MIN_LENGTH_Z;
    for (size_t i = 0; i < rand_zapper_length(); i++) {
        length += 10;
    }
    list_t *zapper_vertices = list_init(CIRCLE_VERTICES_Z*2+4, (free_func_t)free);
    vector_t *L_D = malloc(sizeof(vector_t));
    assert(L_D != NULL);
    L_D->x = 0;
    L_D->y = 0;
    list_add(zapper_vertices, L_D);
    vector_t *L_U = malloc(sizeof(vector_t));
    assert(L_U != NULL);
    L_U->x = 0;
    L_U->y = length;
    list_add(zapper_vertices, L_U);

    add_semi_circles(zapper_vertices, *L_U);

    vector_t *R_U = malloc(sizeof(vector_t));
    assert(R_U != NULL);
    R_U->x = WIDTH_Z;
    R_U->y = length;
    list_add(zapper_vertices, R_U);
    vector_t *R_D = malloc(sizeof(vector_t));
    assert(R_D != NULL);
    R_D->x = WIDTH_Z;
    R_D->y = 0;
    list_add(zapper_vertices, R_D);

    add_counter_semi_circles(zapper_vertices, VEC_ZERO);


    body_t *zapper = body_init_with_info(zapper_vertices, MASS_Z, color_init(255, 69, 0),
                    info, (free_func_t)free);
    body_set_velocity(zapper, (vector_t){-speed_z, 0});
    body_set_centroid(zapper, zapper_position(MAX, MIN, length));
    return zapper;
}

// random length
double rand_zapper_length(){
    double r = (rand() % 30) + 1;
    return r;
}

// adds the vertices of the semi-circle to a list corresponding to the vertices of a zapper
void add_semi_circles (list_t *list, vector_t displacement) {
    vector_t initial_vector = {-(WIDTH_Z/2), 0};
    for (size_t i = 0; i <= CIRCLE_VERTICES_Z; i++) {
        vector_t *add_vertex = malloc(sizeof(vector_t));
        assert(add_vertex != NULL);
        add_vertex->x = vec_rotate(initial_vector, i * (-M_PI / CIRCLE_VERTICES_Z)).x;
        add_vertex->y = vec_rotate(initial_vector, i * (-M_PI / CIRCLE_VERTICES_Z)).y;

        vector_t *add_vertex_moved = malloc(sizeof(vector_t));
        assert(add_vertex_moved != NULL);
        add_vertex_moved->x = vec_add((vector_t){WIDTH_Z/2, 0}, vec_add(*add_vertex, displacement)).x;
        add_vertex_moved->y = vec_add((vector_t){WIDTH_Z/2, 0}, vec_add(*add_vertex, displacement)).y;
        list_add(list, add_vertex_moved);
        free(add_vertex);
    }
}

void add_counter_semi_circles (list_t *list, vector_t displacement) {
    vector_t initial_vector = {-(WIDTH_Z/2), 0};
    for (size_t i = 0; i <= CIRCLE_VERTICES_Z; i++) {
        vector_t *add_vertex = malloc(sizeof(vector_t));
        assert(add_vertex != NULL);
        add_vertex->x = vec_rotate(initial_vector, i * (M_PI / CIRCLE_VERTICES_Z)).x;
        add_vertex->y = vec_rotate(initial_vector, i * (M_PI / CIRCLE_VERTICES_Z)).y;

        vector_t *add_vertex_moved = malloc(sizeof(vector_t));
        assert(add_vertex_moved != NULL);
        add_vertex_moved->x = vec_add((vector_t){WIDTH_Z/2, 0}, vec_add(*add_vertex, displacement)).x;
        add_vertex_moved->y = vec_add((vector_t){WIDTH_Z/2, 0}, vec_add(*add_vertex, displacement)).y;
        list_add(list, add_vertex_moved);
        free(add_vertex);
    }
}


// spawn position of zapper in window
vector_t zapper_position(vector_t MAX, vector_t MIN, size_t length) {
    double max = MAX.y - (length/2 + WIDTH_Z/2);
    double min = MIN.y + length/2 + WIDTH_Z/2;
    double r = (rand() % (int)(max + 1 - min)) + min;
    return (vector_t){SPAWN_X_POSITION_Z, r};
}

void rotate_zapper(body_t *body) {
  double r = rand() % 5;
  if (r == 4) {
    body_set_rotation(body, 0.7);
  }
  if (r == 3) {
    body_set_rotation(body, 0);
  }
  if (r == 2) {
    body_set_rotation(body, -0.7);
  }
  if (r == 1) {
    body_set_rotation(body, 1.5708);
  }
}
