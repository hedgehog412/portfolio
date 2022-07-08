#include "list.h"
#include "assert.h"
#include <stddef.h>
#include <stdlib.h>

struct list {
  void **lst;
  size_t capacity;
  size_t size;
  free_func_t freer;
};

void shift_left(list_t *list, size_t index);
void shift_right(list_t *list, size_t index);
void list_resize(list_t *list);

list_t *list_init(size_t initial_size, free_func_t freer) {
  list_t *init_list = malloc(sizeof(list_t));
  assert(init_list != NULL);
  init_list->lst = malloc(sizeof(void *) * (initial_size));
  assert(init_list->lst != NULL);
  init_list->capacity = initial_size;
  init_list->size = 0;
  init_list->freer = freer;
  return init_list;
}

void list_free(list_t *list) {
  if (list->freer != NULL) {
    for (size_t i = 0; i < list->size; i++) {
      (*list->freer)(list->lst[i]);
    }
  }
  free(list->lst);
  free(list);
}

size_t list_size(list_t *list) { return list->size; }

void *list_get(list_t *list, size_t index) {
  assert(index >= 0);
  assert(index < list->size);
  return list->lst[index];
}

void *list_remove(list_t *list, size_t index) {
  void *deleted = list_get(list, index);
  shift_left(list, index + 1);
  list->size--;
  return deleted;
}

void list_add(list_t *list, void *value) {
  if (list->size >= list->capacity) {
    list_resize(list);
  }
  assert(list_size(list) < list->capacity);
  list->lst[list_size(list)] = value;
  list->size++;
}

void *list_replace(list_t *list, size_t index, void *value) {
  void *result = list_get(list, index);
  list->lst[index] = value;
  return result;
}

void list_insert(list_t *list, size_t index, void *value) {
  if (list->size >= list->capacity) {
    list_resize(list);
  }
  assert(list_size(list) < list->capacity);
  shift_right(list, index);
  list->lst[index] = value;
}

/**
 * Shifts down the elements of a list starting at a given index, the first
 * one inclusive and the second one exclusive.
 * @param list list to be shifted
 * @param index first index to be moved(inclusive)
 */
void shift_left(list_t *list, size_t index) {
  assert(index >= 0);
  for (size_t i = index; i < list->size; i++) {
    list->lst[i - 1] = list->lst[i];
  }
}

/**
 * Shifts up the elements of a list starting at a given index, the first
 * one inclusive and the second one exclusive.
 * @param list list to be shifted
 * @param index first index to be moved (inclusive)
 */
void shift_right(list_t *list, size_t index) {
  assert(index > 0);
  for (size_t i = index; i < list->size; i++) {
    list->lst[i + 1] = list->lst[i];
  }
}


/**
 * Resizes a list multiplying its capacity by 2.
 * @param list list to be resized
 */
void list_resize(list_t *list) {
  void **temp_list = malloc(sizeof(void *) * ((list->capacity + 1) * 2));
  for (size_t i = 0; i < list->size; i++) {
    temp_list[i] = list->lst[i];
  }
  free(list->lst);
  list->lst = temp_list;
  list->capacity = (list->capacity + 1) * 2;
}
