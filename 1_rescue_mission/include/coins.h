#ifndef __COINS_H__
#define __COINS_H__

#include "body.h"

// zapper characteristics - without circular heads

#define MASS_C 1
#define RADIUS_1 20
#define RADIUS_2 13
#define CIRCLE_VERTICES_C 30
#define SPAWN_X_POSITION_C 2060
#define COIN_POINTS 3

// generates a coin
body_t *coin_generate(vector_t MAX, vector_t MIN, double speed_c);

body_t *coin_power_generate(vector_t MAX, vector_t MIN, double speed_c);

#endif // #ifndef __MISSILES_H__