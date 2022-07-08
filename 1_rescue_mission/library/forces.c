#include "forces.h"
#include "collision.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

const double GRAVITY_EXCLUSION_LIMIT = 20;

typedef struct f_aux {
  double value;
  list_t *bodies;
  collision_handler_t handler;
  void *h_aux;
  free_func_t h_aux_freer;
  bool applied;
} force_aux_t;

void n_gravity(force_aux_t *g_aux);
void spring(force_aux_t *s_aux);
void drag(force_aux_t *drag_aux);
void destructive_collision(force_aux_t *collision_aux);
void aux_freer(force_aux_t *aux);
void ch_remove_both_bodies(body_t *body1, body_t *body2, vector_t axis,
                           void *aux);
void ch_destructive_bounce(body_t *body1, body_t *body2, vector_t axis,
                           void *aux);
void ch_impulse(body_t *body1, body_t *body2, vector_t axis, void *aux);
void collision(force_aux_t *collision_aux);
void ch_remove_one_body(body_t *body1, body_t *body2, vector_t axis, void *aux);
void one_sided_gravity(force_aux_t *g_aux);

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1,
                              body_t *body2) {
  force_aux_t *g_aux = malloc(sizeof(force_aux_t));
  assert(g_aux != NULL);
  g_aux->value = G;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  g_aux->bodies = bodies;
  g_aux->h_aux = NULL;
  g_aux->h_aux_freer = NULL;
  g_aux->handler = NULL;
  scene_add_bodies_force_creator(scene, (force_creator_t)n_gravity, g_aux,
                                 bodies, (free_func_t)aux_freer);
}

void create_onesided_gravity(scene_t *scene, double G, body_t *body1,
                              body_t *body2) {
  force_aux_t *g_aux = malloc(sizeof(force_aux_t));
  assert(g_aux != NULL);
  g_aux->value = G;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  g_aux->bodies = bodies;
  g_aux->h_aux = NULL;
  g_aux->h_aux_freer = NULL;
  g_aux->handler = NULL;
  scene_add_bodies_force_creator(scene, (force_creator_t)one_sided_gravity, g_aux,
                                 bodies, (free_func_t)aux_freer);
}

// force creator for newtonian gravity force
void n_gravity(force_aux_t *g_aux) {
  double G = g_aux->value;
  body_t *body1 = list_get(g_aux->bodies, 0);
  body_t *body2 = list_get(g_aux->bodies, 1);
  vector_t gravity;
  if (get_distance(body_get_centroid(body1), body_get_centroid(body2)) >
      GRAVITY_EXCLUSION_LIMIT) {
    double r = get_distance(body_get_centroid(body1), body_get_centroid(body2));
    gravity = vec_multiply(
        G * (body_get_mass(body1) * body_get_mass(body2)) / pow(r, 3),
        vec_subtract(body_get_centroid(body1), body_get_centroid(body2)));
    body_add_force(body1, vec_negate(gravity));
    body_add_force(body2, gravity);
  }
}

// force creator for onesided gravity force
void one_sided_gravity(force_aux_t *g_aux) {
  // double G = g_aux->value;
  double magnet_distance = 500;
  body_t *body1 = list_get(g_aux->bodies, 0);
  body_t *body2 = list_get(g_aux->bodies, 1);
  vector_t c1 = body_get_centroid(body1);
  vector_t c2 = body_get_centroid(body2);
  if (get_distance(c1,c2)<magnet_distance){
    body_set_velocity(body2, vec_multiply(get_distance(VEC_ZERO, body_get_velocity(body2)),vec_multiply(1.0/get_distance(c1,c2),vec_subtract(c1,c2))));
  }
}
void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
  force_aux_t *s_aux = malloc(sizeof(force_aux_t));
  assert(s_aux != NULL);
  s_aux->value = k;
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  s_aux->bodies = bodies;
  s_aux->h_aux = NULL;
  s_aux->h_aux_freer = NULL;
  s_aux->handler = NULL;
  scene_add_bodies_force_creator(scene, (force_creator_t)spring, s_aux, bodies,
                                 (free_func_t)aux_freer);
}

// force creator for spring force
void spring(force_aux_t *s_aux) {
  double k = s_aux->value;
  body_t *body1 = list_get(s_aux->bodies, 0);
  body_t *body2 = list_get(s_aux->bodies, 1);
  vector_t s;
  s = vec_multiply(
      k, vec_subtract(body_get_centroid(body1), body_get_centroid(body2)));
  body_add_force(body1, vec_negate(s));
  body_add_force(body2, s);
}

void create_drag(scene_t *scene, double gamma, body_t *body) {
  force_aux_t *drag_aux = malloc(sizeof(force_aux_t));
  assert(drag_aux != NULL);
  drag_aux->value = gamma;
  list_t *bodies = list_init(1, NULL);
  list_add(bodies, body);
  drag_aux->bodies = bodies;
  drag_aux->h_aux = NULL;
  drag_aux->h_aux_freer = NULL;
  drag_aux->handler = NULL;
  scene_add_bodies_force_creator(scene, (force_creator_t)drag, drag_aux, bodies,
                                 (free_func_t)aux_freer);
}

// force creator for drag force
void drag(force_aux_t *drag_aux) {
  double gamma = drag_aux->value;
  body_t *body = list_get(drag_aux->bodies, 0);
  vector_t d;
  d = vec_multiply(gamma, body_get_velocity(body));
  body_add_force(body, vec_negate(d));
}

void create_destructive_collision(scene_t *scene, body_t *body1,
                                  body_t *body2) {
  create_collision(scene, body1, body2,
                   (collision_handler_t)ch_remove_both_bodies, NULL, NULL);
}

void create_physics_collision(scene_t *scene, double elasticity, body_t *body1,
                              body_t *body2) {
  double *el = malloc(sizeof(double));
  assert(el != NULL);
  *el = elasticity;
  create_collision(scene, body1, body2, (collision_handler_t)ch_impulse, el,
                   (free_func_t)free);
}

void create_semi_destructive_collision(scene_t *scene, double elasticity,
                                       body_t *body1, body_t *body2) {
  double *el = malloc(sizeof(double));
  assert(el != NULL);
  *el = elasticity;
  create_collision(scene, body1, body2,
                   (collision_handler_t)ch_remove_one_body, el,
                   (free_func_t)free);
}

void create_collision(scene_t *scene, body_t *body1, body_t *body2,
                      collision_handler_t handler, void *h_aux,
                      free_func_t freer) {
  force_aux_t *collision_aux = malloc(sizeof(force_aux_t));
  assert(collision_aux != NULL);
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  collision_aux->bodies = bodies;
  collision_aux->h_aux = h_aux;
  collision_aux->h_aux_freer = freer;
  collision_aux->handler = handler;
  collision_aux->applied = false;
  scene_add_bodies_force_creator(scene, (force_creator_t)collision,
                                 collision_aux, bodies, (free_func_t)aux_freer);
}

// general force creator for a collision
void collision(force_aux_t *collision_aux) {
  body_t *body1 = list_get(collision_aux->bodies, 0);
  body_t *body2 = list_get(collision_aux->bodies, 1);
  list_t *shape1 = body_get_shape(body1);
  list_t *shape2 = body_get_shape(body2);
  collision_info_t collision_info = find_collision(shape1, shape2);
  if (collision_info.collided && !collision_aux->applied) {
    (*collision_aux->handler)(body1, body2, collision_info.axis,
                              collision_aux->h_aux);
    collision_aux->applied = true;
  } else if (!collision_info.collided) {
    collision_aux->applied = false;
  }
  list_free(shape1);
  list_free(shape2);
}

// function that frees the collision aux
void aux_freer(force_aux_t *aux) {
  list_free(aux->bodies);
  if (aux->h_aux_freer != NULL && aux->h_aux != NULL) {
    (*(aux->h_aux_freer))(aux->h_aux);
  }
  free(aux);
}

// collision handeler for a two-body destructive collision
void ch_remove_both_bodies(body_t *body1, body_t *body2, vector_t axis,
                           void *aux) {
  body_remove(body1);
  body_remove(body2);
}

void ch_remove_one_body(body_t *body1, body_t *body2, vector_t axis, void *aux){
    body_remove(body2);
}

// collision handeler for a single-body destructive collision
void ch_destructive_bounce(body_t *body1, body_t *body2, vector_t axis,
                           void *aux) {
  ch_impulse(body1, body2, axis, aux);
  body_remove(body2);
}

// collision handeler for a two-body non-destructive collision
void ch_impulse(body_t *body1, body_t *body2, vector_t axis, void *aux) {
  double m1 = body_get_mass(body1);
  double m2 = body_get_mass(body2);
  double u1 = vec_dot(body_get_velocity(body1), axis);
  double u2 = vec_dot(body_get_velocity(body2), axis);
  if (m1 == INFINITY) {
    double abs_impulse = m2 * (1 + (*((double *)aux))) * (u2 - u1);
    body_add_impulse(body2, vec_multiply(-abs_impulse, axis));
  } else if (m2 == INFINITY) {
    double abs_impulse = m1 * (1 + (*((double *)aux))) * (u2 - u1);
    body_add_impulse(body1, vec_multiply(abs_impulse, axis));
  } else {
    double abs_impulse =
        (m1 * m2 / (m1 + m2)) * (1 + (*((double *)aux))) * (u2 - u1);
    body_add_impulse(body1, vec_multiply(abs_impulse, axis));
    body_add_impulse(body2, vec_multiply(-abs_impulse, axis));
  }
}