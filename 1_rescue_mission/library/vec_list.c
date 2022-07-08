#include "vec_list.h"
#include <assert.h>
#include <stdlib.h>

typedef struct vec_list {
  size_t size;
  size_t capacity;
  vector_t **vectors;
} vec_list_t;

vec_list_t *vec_list_init(size_t initial_size) {
  vec_list_t *vec_pointer = malloc((sizeof(vec_list_t)));
  assert(vec_pointer != NULL);
  vec_list_t vec_list;
  vector_t **arr = malloc((sizeof(vector_t *) * initial_size));
  assert(arr != NULL);
  vec_list.size = 0;
  vec_list.capacity = initial_size;
  vec_list.vectors = arr;
  vec_pointer[0] = vec_list;
  return vec_pointer;
}

void vec_list_free(vec_list_t *list) {
  for (size_t i = 0; i < list[0].size; i++) {
    free(list[0].vectors[i]);
  }
  free(list->vectors);
  free(list);
}

size_t vec_list_size(vec_list_t *list) { return list[0].size; }

vector_t *vec_list_get(vec_list_t *list, size_t index) {
  assert(0 <= index);
  assert(index < list[0].size);
  return list[0].vectors[index];
}

void vec_list_add(vec_list_t *list, vector_t *value) {
  assert(value != NULL);
  assert(list[0].size < list[0].capacity);
  list[0].vectors[list[0].size] = value;
  list[0].size++;
}

vector_t *vec_list_remove(vec_list_t *list) {
  assert(list[0].size != 0);
  vector_t *deleted = vec_list_get(list, list[0].size - 1);
  list[0].size--;
  return deleted;
}
