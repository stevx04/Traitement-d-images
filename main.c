#include "picture.h"
#include "filename.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./my_program <operations> <image files>...\n");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "brighten") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = brighten_picture(a, 1.5);
            write_picture(b, argv[++i]);
        } 
        else if (strcmp(argv[i], "convert_to_color") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = convert_to_color_picture(a);
            write_picture(b, argv[++i]);
        }else if (strcmp(argv[i], "convert_to_gray") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = convert_to_gray_picture(a);
            write_picture(b, argv[++i]);
        }else if (strcmp(argv[i], "melt") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = melt_picture(a, a.h * a.w * a.c);
            write_picture(b, argv[++i]);

        } else if (strcmp(argv[i], "inverse") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = inverse_picture(a);
            write_picture(b, argv[++i]);

        } else if (strcmp(argv[i], "split") == 0) {
            picture a = read_picture(argv[++i]);
            picture* b = split_picture(a);
            write_picture(b[0], argv[++i]);
            write_picture(b[1], argv[++i]);
            write_picture(b[2], argv[++i]);

        } else if (strcmp(argv[i], "normalize_dynamic") == 0) {
            picture a = read_picture(argv[++i]);
            picture* b = split_picture(a);
            picture a1 = normalize_dynamic_picture(b[0]);
            picture a2 = normalize_dynamic_picture(b[1]);
            picture a3 = normalize_dynamic_picture(b[2]);
            picture final = merge_picture(a1, a2, a3);
            write_picture(final, argv[++i]);

        } else if (strcmp(argv[i], "levels") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = set_levels_picture(a, 8);
            write_picture(b, argv[++i]);

        } else if (strcmp(argv[i], "resample_nearest") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = resample_picture_nearest(a, a.w / 1.6, a.h / 1.6);
            write_picture(b, argv[++i]);

        } else if (strcmp(argv[i], "resample_larger") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = resample_picture_nearest(a, a.w * 1.6, a.h * 1.6);
            write_picture(b, argv[++i]);

        } else if (strcmp(argv[i], "multiply") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = read_picture(argv[++i]);
            picture c = mult_picture(a, b);
            write_picture(c, argv[++i]);

        } else if (strcmp(argv[i], "mix") == 0) {
            picture a = read_picture(argv[++i]);
            picture b = read_picture(argv[++i]);
            picture d = read_picture(argv[++i]);
            picture c = mix_picture(a, b, d);
            write_picture(c, argv[++i]);
        }
    }
    return 0;
}