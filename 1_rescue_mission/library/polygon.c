#include "polygon.h"
#include <stdio.h>
#include <stdlib.h>

double polygon_area(list_t *polygon) {
  double area = 0;
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *vec0 = list_get(polygon, i);
    vector_t *vec1 = list_get(polygon, (i + 1) % list_size(polygon));
    area += (vec0->x) * (vec1->y) - (vec0->y) * (vec1->x);
  }
  return area / 2;
}

vector_t polygon_centroid(list_t *polygon) {
  double x = 0;
  double y = 0;
  double area = polygon_area(polygon);
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *vec0 = list_get(polygon, i);
    vector_t *vec1 = list_get(polygon, (i + 1) % list_size(polygon));
    x += (vec0->x + vec1->x) * ((vec0->x) * (vec1->y) - (vec0->y) * (vec1->x));
    y += (vec0->y + vec1->y) * ((vec0->x) * (vec1->y) - (vec0->y) * (vec1->x));
  }
  vector_t vector = {.x = 1 / (6 * area) * x, .y = 1 / (6 * area) * y};
  return vector;
}

void polygon_translate(list_t *polygon, vector_t translation) {
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *new_position = malloc(sizeof(vector_t));
    new_position->x =
        vec_add(*((vector_t *)list_get(polygon, i)), translation).x;
    new_position->y =
        vec_add(*((vector_t *)list_get(polygon, i)), translation).y;
    vector_t *old_position = list_replace(polygon, i, new_position);
    free(old_position);
  }
}

void polygon_rotate(list_t *polygon, double angle, vector_t point) {
  polygon_translate(polygon, vec_negate(point));
  for (size_t i = 0; i < list_size(polygon); i++) {
    vector_t *new_position = malloc(sizeof(vector_t));
    new_position->x = vec_rotate(*((vector_t *)list_get(polygon, i)), angle).x;
    new_position->y = vec_rotate(*((vector_t *)list_get(polygon, i)), angle).y;
    vector_t *old_position = list_replace(polygon, i, new_position);
    free(old_position);
  }
  polygon_translate(polygon, point);
}
