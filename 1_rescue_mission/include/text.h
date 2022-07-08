#ifndef __TEXT_H__
#define __TEXT_H__

#include "color.h"
#include "list.h"
#include "vector.h"
#include "polygon.h"
#include <stdbool.h>

typedef struct text text_t;


/**
 * Initializes a piece of text
 * @param string string to display
 * @param position position to display
 * @param text_color color to display
 * @param text_size size to display
 */
text_t *text_init(char *string, vector_t position, rgb_color_t text_color, double text_size);

/**
 * Gets string from text object
 * @param txt text from which to get string
 */
char * txt_get_string(text_t *txt);

/**
 * Gets position from text object
 * @param txt text from which to get position
 */
vector_t txt_get_position(text_t *txt);

/**
 * Gets color from text object
 * @param txt text from which to get color
 */
rgb_color_t txt_get_color(text_t *txt);

/**
 * Gets size from text object
 * @param txt text from which to get size
 */
double txt_get_size(text_t *txt);

/**
 * frees a text object
 */
void txt_free(text_t *txt);

/**
 * Sets the text of a text object
 * @param txt text object
 * @param str text 
 */
void txt_set_txt(text_t *txt, char *str);

#endif // #ifndef __TEXT_H__