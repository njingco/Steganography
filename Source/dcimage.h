#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <MagickWand/MagickWand.h>
#include <string.h>
#include <ctype.h>

#define FILE_LEN 80
#define BUFFER_SIZE 1028

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
bool save_img(MagickWand *wand);
bool is_cover_larger(MagickWand *cover, MagickWand *secret);
int get_img_size(MagickWand *wand);
bool stego(MagickWand *cover, MagickWand *secret);
bool unstego(MagickWand *cover);

char *img_to_stream(MagickWand *wand);

FILE *open_file();
int write_file(FILE *file, char *buffer, int size);

void parse_colour_string(long *clr, char *colorStr);
void parse_color_int(long *clr, char *colorStr);

bool is_even(int num);
