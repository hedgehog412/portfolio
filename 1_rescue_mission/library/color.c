#include "color.h"
#include "assert.h"
#include <math.h>
#include <stdlib.h>

const int COLOR = 256;
const rgb_color_t BLACK  = {0, 0, 0};

double red(double range, double i);
double green(double range, double i);
double blue(double range, double i);

rgb_color_t *color_init(float r, float g, float b) {
  rgb_color_t *color = malloc(sizeof(rgb_color_t));
  assert(color != NULL);
  color->r = (float)((int)r % COLOR) / COLOR;
  color->g = (float)((int)g % COLOR) / COLOR;
  color->b = (float)((int)b % COLOR) / COLOR;
  return color;
}

rgb_color_t rand_color() {
  rgb_color_t random = {random.r = ((double)(rand() % COLOR) / COLOR),
                        random.g = ((double)(rand() % COLOR) / COLOR),
                        random.b = ((double)(rand() % COLOR) / COLOR)};
  return random;
}

rgb_color_t rainbow(int nbr, int i) {
  float R = (float)red((double)nbr, (double)i);
  float G = (float)green((double)nbr, (double)i);
  float B = (float)blue((double)nbr, (double)i);
  rgb_color_t color;
  color.r = (float)((int)R % COLOR) / COLOR;
  color.g = (float)((int)G % COLOR) / COLOR;
  color.b = (float)((int)B % COLOR) / COLOR;
  return color;
}

double red(double range, double i) {
  return ((double)COLOR / 2) * sin((2 * M_PI / range) * (i + range / 4)) +
         ((double)(COLOR - 1)) / 2;
}

double green(double range, double i) {
  return ((double)COLOR / 2) * sin((2 * M_PI / range) * (i + 7 * range / 8)) +
         ((double)(COLOR - 1)) / 2;
}

double blue(double range, double i) {
  return ((double)COLOR / 2) * sin((2 * M_PI / range) * (i + 5 * range / 8)) +
         ((double)(COLOR - 1)) / 2;
}

rgb_color_t *color_to_pointer(rgb_color_t color){
    rgb_color_t *pointer = malloc(sizeof(rgb_color_t));
    pointer->r = color.r;
    pointer->g = color.g;
    pointer->b = color.b;
    return pointer;
}