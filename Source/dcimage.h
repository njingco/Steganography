#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <MagickWand/MagickWand.h>
#include <string.h>

#define BMP "BMP3"

bool isSupported(MagickWand *wand);
bool saveImg(MagickWand *wand, char *filename);
bool isCoverSizeLarger(MagickWand *cover, MagickWand *secret);
void stego(MagickWand *cover, MagickWand *secret);