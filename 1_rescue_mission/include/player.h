#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "body.h"

// positions and velocites
#define PLAYER_INIT_POSITION ((vector_t){200, 100})
#define P_REGULAR_VELOCITY ((vector_t){0, 1000})
#define P_MASS 20

// object sizes
#define WIDTH_P 60
#define HALF_WIDTH_P (WIDTH_P/2)
#define LENGTH_P 60
#define HALF_LENGTH_P (LENGTH_P/2)

body_t *player_generator();

#endif // #ifndef __PLAYER_H__