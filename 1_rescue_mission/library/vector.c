#include "vector.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

const vector_t VEC_ZERO = {.x = 0, .y = 0};

vector_t vec_init(double x, double y);

vector_t vec_add(vector_t v1, vector_t v2) {
  return vec_init(v1.x + v2.x, v1.y + v2.y);
}

vector_t vec_subtract(vector_t v1, vector_t v2) {
  return vec_init(v1.x - v2.x, v1.y - v2.y);
}

vector_t vec_negate(vector_t v1) { return vec_init(-v1.x, -v1.y); }

vector_t vec_multiply(double s, vector_t v1) {
  return vec_init((s * v1.x), (s * v1.y));
}

double vec_dot(vector_t v1, vector_t v2) {
  return (v1.x * v2.x) + (v1.y * v2.y);
}

double vec_cross(vector_t v1, vector_t v2) {
  return (v1.x * v2.y) - (v1.y * v2.x);
}

vector_t vec_rotate(vector_t v1, double angle) {
  return vec_init(v1.x * cos(angle) - v1.y * sin(angle),
                  v1.x * sin(angle) + v1.y * cos(angle));
}

/**
 * Helper function that initialises a vector with x and y value
 * @param x x value of vector
 * @param y y value of vector
 */
vector_t vec_init(double x, double y) {
  vector_t vec = {.x = x, .y = y};
  return vec;
}

/**
 * Helper function that calculates the distance between two vectors
 * @param v1 value of vector 1
 * @param v2 value of vector 2
 */
double get_distance(vector_t v1, vector_t v2) {
  return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

vector_t *malloc_vector(vector_t vector){
  vector_t *malloced = malloc(sizeof(vector_t));
  assert(malloced != NULL);
  malloced->x = vector.x;
  malloced->y = vector.y;
  return malloced;
}