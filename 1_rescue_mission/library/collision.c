#include "collision.h"
#include "assert.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES

void find_proj_axs(list_t *shape, list_t *axs);
float find_max(list_t *shape, vector_t *proj_ax);
float find_min(list_t *shape, vector_t *proj_ax);
float find_proj(vector_t point, vector_t ax);
vector_t find_normal(vector_t axis);

const size_t IN_SIZE = 10;

collision_info_t find_collision(list_t *shape1, list_t *shape2) {
  collision_info_t collision_info;
  collision_info.axis = VEC_ZERO;
  list_t *proj_axs = list_init(IN_SIZE, free);
  find_proj_axs(shape1, proj_axs);
  find_proj_axs(shape2, proj_axs);
  double overlap = INFINITY;
  vector_t collision_ax;
  for (int i = 0; i < list_size(proj_axs); i++) {
    vector_t *proj_ax = list_get(proj_axs, i);
    double overlap1 = find_max(shape1, proj_ax) - find_min(shape2, proj_ax);
    double overlap2 = find_max(shape2, proj_ax) - find_min(shape1, proj_ax);
    if (overlap1 < 0 || overlap2 < 0) {
      list_free(proj_axs);
      collision_info.collided = false;
      return collision_info;
    }
    if (overlap1 < overlap) {
      overlap = overlap1;
      collision_ax = find_normal(*proj_ax);
    }
    if (overlap2 < overlap) {
      overlap = overlap2;
      collision_ax = find_normal(*proj_ax);
      collision_ax = vec_negate(collision_ax);
    }
  }
  list_free(proj_axs);
  collision_info.collided = true;
  collision_info.axis = collision_ax;
  return collision_info;
}

// creates a list of axis formed by the sides of each shape
void find_proj_axs(list_t *shape, list_t *axs) {
  for (int i = 0; i < list_size(shape); i++) {
    vector_t *point1 = list_get(shape, i);
    vector_t *point2 = list_get(shape, (i + 1) % list_size(shape));
    vector_t *proj_ax = malloc(sizeof(vector_t));
    proj_ax->x = vec_subtract(*point1, *point2).x;
    proj_ax->y = vec_subtract(*point1, *point2).y;
    vector_t rotated = vec_rotate(*proj_ax, M_PI / 2);
    proj_ax->x = (float)rotated.x;
    proj_ax->y = (float)rotated.y;
    list_add(axs, proj_ax);
  }
}

// finds the maximum projected point along given axis
float find_max(list_t *shape, vector_t *proj_ax) {
  float max = find_proj(*((vector_t *)list_get(shape, 0)), *proj_ax);
  for (int i = 1; i < list_size(shape); i++) {
    vector_t *point = list_get(shape, i);
    float length = find_proj(*point, *proj_ax);
    if (length > max) {
      max = length;
    }
  }
  return max;
}

// finds the minimum projected point along given axis
float find_min(list_t *shape, vector_t *proj_ax) {
  float min = find_proj(*((vector_t *)list_get(shape, 0)), *proj_ax);
  for (int i = 1; i < list_size(shape); i++) {
    vector_t *point = list_get(shape, i);
    float length = find_proj(*point, *proj_ax);
    if (length < min) {
      min = length;
    }
  }
  return min;
}

// finds the projection of a point onto an axis
float find_proj(vector_t point, vector_t ax) {
  vector_t proj = vec_multiply((vec_dot(point, ax) / vec_dot(ax, ax)), ax);
  float sign = vec_dot(proj, ax) / sqrt(vec_dot(proj, ax) * vec_dot(proj, ax));
  float length = sqrt(vec_dot(proj, proj)) * sign;
  return length;
}

// finds the normal vector of a given vector
vector_t find_normal(vector_t axis) {
  double magnitude = sqrt(vec_dot(axis, axis));
  vector_t normal = vec_multiply((double)1 / magnitude, axis);
  return normal;
}