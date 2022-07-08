#include "scene.h"
#include "assert.h"
#include <stdlib.h>
#include "sdl_wrapper.h"
#include "text.h"

typedef struct scene {
  list_t *bodies;
  list_t *force_creators;
  list_t *texts;
} scene_t;

typedef struct force_c {
  force_creator_t force_c;
  void *aux;
  free_func_t freer;
  list_t *bodies;
} force_c;

const size_t INITIAL_CAPACITY = 10;

void force_c_free(force_c *force_creator);

scene_t *scene_init(void) {
  scene_t *scene = malloc(sizeof(scene_t));
  assert(scene != NULL);
  scene->bodies = list_init(INITIAL_CAPACITY, (free_func_t)body_free);
  scene->force_creators =
      list_init(INITIAL_CAPACITY, (free_func_t)force_c_free);
  scene->texts = list_init(INITIAL_CAPACITY, (free_func_t) txt_free);
  return scene;
}

void scene_free(scene_t *scene) {
  list_free(scene->bodies);
  list_free(scene->force_creators);
  list_free(scene->texts);
  free(scene);
}

size_t scene_bodies(scene_t *scene) { return list_size(scene->bodies); }

body_t *scene_get_body(scene_t *scene, size_t index) {
  return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->bodies, body);
}

void scene_insert_body(scene_t *scene, body_t *body, double index) {
  list_insert(scene->bodies, index, body);
}

void scene_remove_body(scene_t *scene, size_t index) {
  body_remove(scene_get_body(scene, index));
}

void scene_tick(scene_t *scene, double dt) {
  for (size_t i = 0; i < list_size(scene->force_creators); i++) {
    force_c *force_creator = list_get(scene->force_creators, i);
    bool removed = false;
    (*force_creator->force_c)(force_creator->aux);
    if (force_creator->bodies != NULL) {
      for (size_t j = 0; j < list_size(force_creator->bodies); j++) {
        if (body_is_removed(list_get(force_creator->bodies, j))) {
          list_remove(scene->force_creators, i);
          removed = true;
          break;
        }
      }
    }
    if (removed) {
      force_c_free(force_creator);
      i = -1;
    }
  }

  for (size_t i = 0; i < list_size(scene->bodies); i++) {
    if (body_is_removed(scene_get_body(scene, i))) {
      body_t *removed = list_remove(scene->bodies, i);
      i--;
      body_free(removed);
    } else {
      body_tick(list_get(scene->bodies, i), dt);
    }
  }
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux,
                             free_func_t freer) {
  scene_add_bodies_force_creator(scene, forcer, aux, NULL, freer);
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer,
                                    void *aux, list_t *bodies,
                                    free_func_t freer) {
  force_c *force_creator = malloc(sizeof(force_c));
  assert(force_creator != NULL);
  force_creator->force_c = forcer;
  force_creator->aux = aux;
  force_creator->freer = freer;
  force_creator->bodies = bodies;
  list_add(scene->force_creators, force_creator);
}

/**
 * Freese force_c structs
 *
 * @param force_creator force_c to be freed
 */
void force_c_free(force_c *force_creator) {
  if (force_creator->freer != NULL) {
    (*force_creator->freer)(force_creator->aux);
  } else if (force_creator->bodies != NULL) {
    list_free(force_creator->bodies);
  }
  free(force_creator);
}

void scene_add_text(scene_t *scene, text_t *txt) {
  list_add(scene->texts, txt);
}

size_t scene_text(scene_t *scene) { return list_size(scene->texts); }

text_t *scene_get_texts(scene_t *scene, size_t index) {
  return list_get(scene->texts, index);
}

void scene_remove_text(scene_t *scene, size_t index) {
  text_t *deleted = list_remove(scene->texts, index);
  txt_free(deleted);
}

void scene_remove_all_text(scene_t *scene){
  list_free(scene->texts);
  scene->texts = list_init(0, ((free_func_t)txt_free));
}

