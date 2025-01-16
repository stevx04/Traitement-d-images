#ifndef LUT_H
#define LUT_H
#include "picture.h"

typedef struct{
    int size;
    unsigned char* val;
} lut;

lut* create_lut(int n);
void free_lut(lut* lut);
picture lut_on_picture(picture p,lut* lut);
lut* inverse_lut();
lut* normaliser_lut(int min_val, int max_val);
lut* set_levels_lut(byte nb_levels);
#endif