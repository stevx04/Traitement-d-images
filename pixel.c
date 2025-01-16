#include <stdlib.h>
#include <stdio.h>
#include "pixel.h"
#include "picture.h"

byte lire_pixel(picture *p, int i, int j, constante color) {
    int tmp = ((i*(p->w))+j)*(p->c)+color;
    return (p->data[tmp]);
}

void ecrire_pixel(picture *p, int i, int j,byte val, constante color){
    int tmp = ((i*(p->w))+j)*(p->c)+color;
    p->data[tmp]=val;
}