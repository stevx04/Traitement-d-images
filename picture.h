#ifndef PICTURE_H
#define PICTURE_H

typedef unsigned char byte;
#define MAX_BYTE 255

typedef struct {
    byte* data;
    int h;
    int w;
    int c;
} picture;

/*
    @requires filename doit pointer vers un chemin de fichier valide
    @assigns /
    @ensures Retourne une structure picture contenant l'image ou une image vide en cas d'erreur.
 */
picture read_picture(char* filename);

/*
    @requires p doit être une structure picture valide
    @assigns /
    @ensures Affiche des informations sur l'image (dimensions, canaux, etc.)
 */
void read_picture_info(picture p);

/*
    @requires p doit être une structure picture valide et filename un chemin accessible en écriture
    @assigns Écrit les données de l'image dans le fichier
    @ensures Retourne 0 en cas de succès ou une valeur non nulle en cas d'erreur
 */
int write_picture(picture p, char* filename);

/*
    @requires width, height et channels doivent être supérieurs à 0
    @assigns initialise des données dans une structure pictures
    @ensures Retourne une image vide avec les dimensions spécifiées
 */
picture create_picture(unsigned int width, unsigned int height, unsigned int channels);

/*
    @requires p doit être une structure picture valide
    @assigns Libère la mémoire associée aux données de l'image
    @ensures /
 */
void clean_picture(picture* p);

/*
    @requires p doit être une structure picture valide
    @assigns /
    @ensures Retourne une copie de l'image
 */
picture copy_picture(picture p);

/*
    @requires p doit être une structure picture valide
    @assigns /
    @ensures Retourne une valeur non nulle si l'image est vide, 0 sinon
 */
int is_empty_picture(picture p);

/*
    @requires p doit être une structure picture valide
    @assigns /
    @ensures Affiche des infos sur l'image
 */
void info_picture(picture p);

/*
    @requires p doit être une structure picture valide
    @assigns Alloue une nouvelle image
    @ensures Retourne une image couleur convertie ou une copie si déjà en couleur
 */
picture convert_to_color_picture(picture p);

/*
    @requires p doit être une structure picture valide
    @assigns Alloue une nouvelle image
    @ensures Retourne une image en niveaux de gris ou une copie si déjà en niveaux de gris
 */
picture convert_to_gray_picture(picture p);

/*
    @requires p doit être une structure picture couleur valide
    @assigns Alloue un tableau de 3 images
    @ensures Retourne un tableau de 3 images en niveaux de gris ou NULL en cas d'erreur
 */
picture* split_picture(picture p);

/*
    @requires red, green et blue doivent être des images de dimensions identiques
    @assigns Alloue une nouvelle image
    @ensures Retourne une image couleur ou une image vide en cas d'erreur
 */
picture merge_picture(picture red, picture green, picture blue);

/*
    @requires p doit être une structure picture valide et factor > 0
    @assigns Alloue une nouvelle image
    @ensures Retourne une image éclaircie
 */
picture brighten_picture(picture p, double factor);

/*
    @requires p doit être une structure picture valide et number > 0
    @assigns Modifie les pixels de l'image existante
    @ensures Retourne une image modifiée
 */
picture melt_picture(picture p, int number);

/*
    @requires p doit être une structure picture valide
    @assigns Alloue une nouvelle image
    @ensures Retourne une image avec des valeurs de pixels inversées
 */
picture inverse_picture(picture p);

/*
    @requires p doit être une structure picture valide
    @assigns Modifie les valeurs des pixels de l'image existante
    @ensures Retourne une image avec des valeurs normalisées entre 0 et 255
 */
picture normalize_dynamic_picture(picture p);

/*
@requires p doit être une structure picture valide et nb_levels > 0
@assigns Alloue une nouvelle image
@ensures Retourne une image avec un nombre réduit de niveaux
 */
picture set_levels_picture(picture p, byte nb_levels);

/*
    @requires p1 et p2 doivent être des images de dimensions identiques
    @assigns Alloue une nouvelle image
    @ensures Retourne une image contenant la différence en abs entre p1 et p2
 */
picture diff_picture(picture p1, picture p2);

/*
    @requires p1 et p2 doivent être des images de dimensions identiques
    @assigns Alloue une nouvelle image
    @ensures Retourne une image contenant le produit des deux images
 */
picture mult_picture(picture p1, picture p2);

/*
    @requires p1, p2, et p3 doivent être des images de dimensions identiques
    @assigns Alloue une nouvelle image
    @ensures Retourne une image contenant la combinaison des trois images
 */
picture mix_picture(picture p1, picture p2, picture p3);

/*
    @requires image doit être une structure picture valide et width>0 et height>0
    @assigns Alloue une nouvelle image
    @ensures Retourne une image redimensionnée avec interpolation des plus proche voisin
 */
picture resample_picture_nearest(picture image, unsigned int width, unsigned int height);

#endif
