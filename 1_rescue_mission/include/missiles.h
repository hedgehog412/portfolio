#ifndef __MISSILES_H__
#define __MISSILES_H__

#include "body.h"

// zapper characteristics - without circular heads

#define MASS_M 1
#define HEIGHT 50
#define LENGTH 40
#define CIRCLE_VERTICES 20
#define SPAWN_X_POSITION 2040


// generates a missile based on a list of vertices and a position
body_t *missile_generate(vector_t MAX, vector_t MIN, double speed_m);

#endif // #ifndef __MISSILES_H__