#ifndef __COLOR_H__
#define __COLOR_H__

/**
 * A color to display on the screen.
 * The color is represented by its red, green, and blue components.
 * Each component must be between 0 (black) and 1 (white).
 */
typedef struct {
  float r;
  float g;
  float b;
} rgb_color_t;

extern const rgb_color_t BLACK;

/**
 * Initialised the color
 * @param r red component of color (0-255)
 * @param g green component of color (0-255)
 * @param b blue component of color (0-255)
 */
rgb_color_t *color_init(float r, float g, float b);

/**
 * Generates a random color.
 */
rgb_color_t rand_color();

/**
 * Gernerates a rainbow ray accross several different object.
 * @param nbr: nbr of objects total.
 * @param i: index of the specific object.
 */
rgb_color_t rainbow(int nbr, int i);

rgb_color_t *color_to_pointer(rgb_color_t color);
#endif // #ifndef __COLOR_H__
