#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pixel.h"
#include "picture.h"
#include "filename.h"
#include "lut.h"
#include <math.h>
picture read_picture(char* filename){

	FILE* f = fopen(filename, "r");
    picture img={NULL,0,0,0};
    if(f==NULL){
        fprintf(stderr,"erreur fopen %s\n",filename);
        return img;
    }
	
    
	char buffer[128];
    
    fgets(buffer, 128, f);
    
    int c;
	if(strcmp(buffer, "P5\n")==0){
        c=1;
    }
    else if(strcmp(buffer, "P6\n")==0){
        c=3;
    }
    else{
		fprintf(stderr, "Ce n'est pas un fichier PGM ou PPM \n");
        fclose(f);
        return img;
	}
    
    do{
        fgets(buffer, 128, f);
    }while (buffer[0]=='#');

    int w, h;
	if (sscanf(buffer, "%d %d", &w, &h) != 2 || w <= 0 || h <= 0) {
        fprintf(stderr, "dimensions invalides\n");
        fclose(f);
    return img;
    }

    fgets(buffer, 128, f);
    size_t data_size = w*h*c;
    img.data = malloc(data_size);
    
    fread(img.data, 1, data_size, f);
	img.w = w;
	img.h = h;
    img.c = c;
    
    fclose(f); 
    return img;
    }

void read_picture_info(picture p) {
    if (p.data == NULL) {
        printf("L'image est vide.\n");
        return;
    }
    printf("Dimensions de l'image: %d x %d\n", p.w, p.h);
    printf("Nombre de canaux: %d\n", p.c);
    printf("Taille: %u octets\n", p.w * p.h * p.c);
}

int write_picture(picture p,char* filename){
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
    fprintf(stderr, "Erreur lors de l'ouverture du fichier de sortie: %s\n", filename);
    
    return -1;
    }
    if (p.data==NULL){
        fprintf(stderr,"aucune image à ecrire\n");
    return -1;
    }

    char* ext= ext_from_path(filename);
    if ((p.c==1 && strcmp(ext,"pgm")!=0) ||(p.c==3 && strcmp(ext,"ppm")!=0)){
        fprintf(stderr,"%s incompatible avec l'image \n",ext);

    free(ext);
    return -2;
    }

    if (p.c == 1) {
        fprintf(f, "P5\n");
    } else if (p.c == 3) {
        fprintf(f, "P6\n");
    }
    
    fprintf(f, "%d %d\n", p.w, p.h);
    fprintf(f, "255\n");
    size_t data_size = p.w * p.h * p.c;
    fwrite(p.data, 1, data_size, f);
    fclose(f);
    free(ext);
    printf("Image écrite");
    return 0;
}

picture create_picture(unsigned int width, unsigned int height, unsigned int channels) {
    picture img;
    img.w = width;
    img.h = height;
    img.c = channels;
    img.data = malloc(width*height*channels*sizeof(unsigned char));
    if (img.data == NULL) {
        fprintf(stderr, "Erreur dans l'allocation.\n");
        img.w = img.h = img.c = 0;
    } 
    return img;
}

void clean_picture(picture *p) {
    if (p->data) {
        free(p->data);
        p->data = NULL;
    }
    p->w = 0;
    p->h = 0;
    p->c = 0;
}

picture copy_picture(picture p) {
    picture copy = create_picture(p.w, p.h, p.c);
    if (copy.data!=NULL && p.data!=NULL) {
      for (int i = 0; i < (p.w)*(p.h)*(p.c); ++i) {
            copy.data[i] = p.data[i];
        }  
    }
    return copy;
}

int is_empty_picture(picture p) {
    return(p.data == NULL || p.w == 0 || p.h == 0 || p.c == 0);
}

int is_gray_picture(picture p) {
    return (p.c == 1);
}

int is_color_picture(picture p) {
    return (p.c == 3);
}

void info_picture(picture p) {
    printf("(%u x %u x %u)\n", p.w, p.h, p.c);
}
picture convert_to_color_picture(picture p){
    if (is_color_picture(p)) {
        return copy_picture(p);
    }
    picture color = create_picture(p.w, p.h, 3);
    if (color.data!=NULL && p.data!=NULL) {
        for (int i = 0; i < p.w * p.h; i++) {
            color.data[3*i] = p.data[i];
            color.data[3*i + 1] = p.data[i];
            color.data[3*i + 2] = p.data[i];
        
        /*if (i < 3) {
            printf("Pixel %d : R = %u, G = %u, B = %u\n",i, p.data[i], color.data[3*i],color.data[3*i + 1], color.data[3*i + 2]);
        }*/
    }
            
}
return color;
}

picture convert_to_gray_picture(picture p){
    if (is_gray_picture(p)) {
        return copy_picture(p);
    }

    picture gray = create_picture(p.w, p.h, 1);
    if (gray.data && p.data) {
        for (int i = 0; i < p.w * p.h; i++) {
            unsigned char r = p.data[3 * i];
            unsigned char g = p.data[3 * i + 1];
            unsigned char b = p.data[3 * i + 2];
            gray.data[i] = (unsigned char)(0.299*r + 0.587*g + 0.114*b);
        }
    }
    return gray;
}

picture* split_picture(picture p) {
    if (is_empty_picture(p)) {
        fprintf(stderr, "L'image est vide.\n");
        return NULL;
    }
    
    //Si p est une image en niveaux de gris on renverra un tableau ne contenant qu’un seul élément
    if (is_gray_picture(p)) {
        picture* tab = malloc(sizeof(picture));
        tab[0] = copy_picture(p);
        return tab;
    }

    picture* elements = malloc(3 * sizeof(picture));

    for (int i = 0; i < 3; i++) {
        elements[i] = create_picture(p.w, p.h, 1);
    }

    for (int i = 0; i < p.w * p.h; i++) {
        elements[0].data[i] = p.data[3*i]; //R
        elements[1].data[i] = p.data[3*i + 1];//V
        elements[2].data[i] = p.data[3*i + 2];//B
    }

    return elements;
}

picture merge_picture(picture red, picture green, picture blue){
    if (is_empty_picture(red) || is_empty_picture(green) || is_empty_picture(blue) || red.w != green.w || red.w != blue.w ||
        red.h != green.h || red.h != blue.h || !is_gray_picture(red) || !is_gray_picture(green) || !is_gray_picture(blue)) {
        fprintf(stderr, "Les images ne sont pas compatible pour merge, verifier leurs tailles et/ou extension\n");
        return create_picture(0, 0, 0);
    }
    picture color = create_picture(red.w, red.h, 3);
    
    for (int i = 0; i < red.w * red.h; i++) {
        color.data[3*i] = red.data[i];//R
        color.data[3*i + 1] = green.data[i];//V
        color.data[3*i + 2] = blue.data[i];//B
    }
    return color;
}

picture brighten_picture(picture p, double factor){
    if (is_empty_picture(p) || factor <= 0) {
        fprintf(stderr, "L'image est vide ou le facteur<=0.\n");
        return create_picture(0, 0, 0);
    }
    picture image_eclaircie = copy_picture(p);
    for (int i = 0;i < p.w * p.h * p.c;i++) {
        int tmp = (int)(p.data[i] * factor);
        if (tmp>255){
             image_eclaircie.data[i] = 255;
        }
        else {
            image_eclaircie.data[i] = (byte)tmp;
        }
    }
    return image_eclaircie;
}

picture melt_picture(picture p, int number){

    if (is_empty_picture(p) || p.h < 2 || number <= 0) {
        fprintf(stderr, "L'image est invalide ou le number est incorrect.\n");
    return copy_picture(p);
    }
  
    picture fondu = copy_picture(p);
    for ( int k = 0; k < number ; k++) {
    int i = rand()%(p.h-1) + 1;//hauteur aléatoire
    int j = rand()%p.w;// largeur aléatoire
    if ( i < p.h - 1){
        for (int c=0; c<p.c; c++){
            if (fondu.data[((i-1)* p.w +j)*p.c+c] < fondu.data[(i* p.w + j)*p.c+c]){
                fondu.data[((i-1)* p.w +j)*p.c+c] = fondu.data[(i* p.w + j)*p.c+c];
                printf("Modification : Pixel [%d, %d, %d] -> [%d, %d, %d]\n", i, j, c, i + 1, j, c);
            }
        }
    }
    }
    return fondu;
}

picture inverse_picture(picture p){
    lut* lut=inverse_lut();
    picture tmp = lut_on_picture(p,lut);
    free_lut(lut);
    return tmp;
    
}

picture normalize_dynamic_picture(picture p){
    //recherche de la valeur min et max des composantes pixels de l'images
    int min_val=256;
    int max_val=0;
    for(int i=0;i<p.w*p.h*p.c;i++){
        if (p.data[i]<min_val){
            min_val=p.data[i];
        }
        else if (p.data[i]>max_val){
            max_val=p.data[i];
        }
    }
    printf("%d %d",min_val,max_val);
    lut* lut_normalise= normaliser_lut(min_val,max_val);
    for (int i = 0; i < 256; i++) {
    printf("lut[%d] = %d\n", i, lut_normalise->val[i]);
}
    picture result = lut_on_picture(p,lut_normalise);
    free_lut(lut_normalise);
    return result;
}

picture set_levels_picture(picture p, byte nb_levels){
    if (is_empty_picture(p) ||nb_levels < 2) {
        fprintf(stderr, "L'image est vide ou le nb_levels < 2\n");
        return create_picture(0, 0, 0);
    }
    lut* a= set_levels_lut(nb_levels);
    picture result = lut_on_picture(p,a);
    free_lut(a);
    return result;
}

picture diff_picture(picture p1, picture p2){
     if (is_empty_picture(p1) || is_empty_picture(p2) || p1.w != p2.w || p1.h != p2.h || p1.c != p2.c){
        fprintf(stderr, "Images incompatibles\n");
        return create_picture(0, 0, 0);
     }
    
    picture result = create_picture(p1.w, p1.h, p1.c);
    for (int i = 0; i < p1.w*p1.h*p1.c;i++){
        result.data[i] = abs(p1.data[i]-p2.data[i]);
    }
    return result;
}

picture mult_picture(picture p1, picture p2){
    if (is_empty_picture(p1) || is_empty_picture(p2) || p1.w != p2.w || p1.h != p2.h || p1.c != p2.c){
        fprintf(stderr, "Images incompatibles\n");
        return create_picture(0, 0, 0);
    }
    picture result = create_picture(p1.w, p1.h, p1.c);
    
    for (int i = 0; i < p1.w*p1.h*p1.c;i++){
        if(((p1.data[i] * p2.data[i]) / 255)>255){
            result.data[i]=255;
        }
        else{
        result.data[i] = (p1.data[i] * p2.data[i]) / 255;
    }
    }
    return result;
}
picture mix_picture(picture p1, picture p2, picture p3){
    if (is_empty_picture(p1) || is_empty_picture(p2) || is_empty_picture(p2) || p1.w != p2.w || p1.h != p2.h || p1.c != p2.c || p1.w != p3.w || p1.h != p3.h){
        fprintf(stderr, "Images incompatibles\n");
    return create_picture(0, 0, 0);
    }
    picture result = create_picture(p1.w, p1.h, p1.c);

    for (int i = 0; i < p1.w * p1.h; i++) {
        for (int c = 0; c < p1.c; c++) {
            double alpha = p3.data[i*p3.c + c]/255;
            result.data[i*p1.c + c] =(1-alpha)*p1.data[i*p1.c + c]+alpha*p2.data[i*p2.c + c];
        }
    }
return result;
}

picture resample_picture_nearest(picture image, unsigned int width, unsigned int height){
    if (is_empty_picture(image)) {
        fprintf(stderr, "L'image est vide\n");
        return create_picture(0, 0, 0);
    }
    picture result = create_picture(width, height, image.c);
    printf("%d %d %d",image.w, image.h,image.c);
    float i_scale_factor = (float)(image.w) / (float)(width);
    float j_scale_factor = (float)(image.h) / (float)(height);

    for (unsigned int j = 0; j < height; j++) {
        for (unsigned int i = 0; i < width; i++) {
            unsigned int src_i = (unsigned int)(i * i_scale_factor);
            unsigned int src_j = (unsigned int)(j * j_scale_factor);
            for (int c = 0; c < image.c; c++) {
                result.data[(j *width + i) * image.c + c] = image.data[(src_j*image.w + src_i) * image.c + c];
            }
        }
    }
    return result;
}
