#include "text.h"
#include "assert.h"
#include "math.h"
#include <stdlib.h>

struct text {
  char *string;
  vector_t position;
  rgb_color_t text_color;
  size_t text_size;
};

text_t *text_init(char *string, vector_t position, rgb_color_t text_color, double text_size) {
  text_t *txt = malloc(sizeof(text_t));
  assert(txt != NULL);
  txt->string = string;
  txt->position = position;
  txt->text_color = text_color;
  txt->text_size = text_size;
  return txt;
}

char * txt_get_string(text_t *txt){
    return txt->string;
}

vector_t txt_get_position(text_t *txt){
    return txt->position;
}

rgb_color_t txt_get_color(text_t *txt){
    return txt->text_color;
}

double txt_get_size(text_t *txt){
    return txt->text_size;
}

void txt_free(text_t *txt){
    free(txt);
}

void txt_set_txt(text_t *txt, char *str){
    txt->string = str;
}