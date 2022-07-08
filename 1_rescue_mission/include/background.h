#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "body.h"
#include "scene.h"

#define BACKGR_CIRCLE_VERT 25

body_t* background_generator();
body_t* background_generator_back(vector_t MAX, vector_t MIN);
body_t* background_generator_front(vector_t MAX, vector_t MIN);
body_t *background_generator_mountain(double radius, vector_t origin, vector_t drift_vel);
body_t *background_generator_bush(double radius, vector_t origin, vector_t drift_vel);
void background_init(scene_t *scene, vector_t MAX, vector_t MIN);
void background_wraparound(body_t *body);

#endif // #ifndef __BACKGROUND_H__