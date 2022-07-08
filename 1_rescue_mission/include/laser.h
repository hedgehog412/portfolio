#include "body.h"

#define WIDTH_L 49
#define HEIGHT_L 70
#define GRAY_R 125
#define GRAY_G 132
#define GRAY_B 133
#define NUM_VERTICES_L 100
#define INIT_L 5
#define FINAL_L 30
#define WIDTH_CIRC 5

body_t *laser_generator(vector_t max, vector_t min);
void expand_rec(list_t * rec);
