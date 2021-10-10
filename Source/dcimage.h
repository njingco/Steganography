#ifndef DCIMAGE_H
#define DCIMAGE_H

#include "helper.h"
#include "dcutils.h"

#define ThrowWandException(wand)                                                 \
    {                                                                            \
        char                                                                     \
            *description;                                                        \
                                                                                 \
        ExceptionType                                                            \
            severity;                                                            \
                                                                                 \
        description = MagickGetException(wand, &severity);                       \
        (void)fprintf(stderr, "%s %s %lu %s\n", GetMagickModule(), description); \
        description = (char *)MagickRelinquishMemory(description);               \
        exit(-1);                                                                \
    }

bool is_supported(MagickWand *wand);
bool is_cover_larger(MagickWand *cover, MagickWand *secret);
int get_img_size(MagickWand *wand);

bool save_img(MagickWand *wand);
char *img_to_stream(MagickWand *wand);

void parse_colour_string(long *clr, char *colorStr);
void parse_color_int(long *clr, char *colorStr);

void break_wands(MagickWand *cover, MagickWand *secret);
void break_wand(MagickWand *cover);

#endif
