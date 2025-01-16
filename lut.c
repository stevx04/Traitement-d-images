#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pixel.h"
#include "picture.h"
#include "filename.h"
#include "lut.h"


lut* create_lut(int n){
    if(n<=0){
        printf("n choisi invalide");
        return NULL;
    }
    lut* tmp =malloc(sizeof(lut));
    tmp->size=n;
    tmp->val= malloc(n*sizeof(tmp->val));
return tmp;
}

void free_lut(lut* lut){
    if(lut!=NULL){
        free(lut->val);
        free(lut);
    }
}

picture lut_on_picture(picture p,lut* lut){
    if(is_empty_picture(p)){
        printf("l'image est vide");
        return p;
    }
    picture tmp = create_picture(p.w,p.h,p.c);
    for (int i = 0; i < p.h; i++) {
        for (int j = 0; j < p.w; j++) {
            for (int c = 0; c < p.c; c++) {
                tmp.data[(i * p.w + j) * p.c + c] = lut->val[p.data[(i * p.w + j) * p.c + c]]; 
            }
        }
    }
    return tmp;
}

lut* inverse_lut(){
    lut* lut=create_lut(256);
    for(int i=0;i<256;i++){
        lut->val[i]=(256-i);
    }
    return lut;
}

lut* normaliser_lut(int min_val, int max_val){
    lut* lut= create_lut(256);
    if (min_val == max_val) {
        for (int i = 0; i < 256; i++) {
            lut->val[i] = (i <= min_val) ? 0 : 255;
        }
    } 
    else{
    for (int i = 0; i < 256; i++) {
        lut->val[i] =((i - min_val) * 255.0 / (max_val - min_val));
    }
    }

    return lut;
}

lut* set_levels_lut(byte nb_levels){
    lut* lut = create_lut(256);
    for (int i = 0; i < 256; i++) {
        lut->val[i] = (i /(256/nb_levels)) * (256 / nb_levels) + (256 / nb_levels) / 2;
    }

    return lut;
}