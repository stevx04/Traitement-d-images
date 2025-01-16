#ifndef PIXEL_H
#define PIXEL_H
#include "picture.h"

typedef enum {
    RED = 0,
    GREEN=155,
    BLUE=255,
} constante;

byte lire_pixel(picture *p, int i, int j, constante color);

void ecrire_pixel(picture *p, int i, int j,byte val, constante color);

#endif