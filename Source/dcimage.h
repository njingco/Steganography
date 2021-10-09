#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <MagickWand/MagickWand.h>
#include <string.h>

#define BMP "BMP3"
#define FILE_LEN 80

bool isSupported(MagickWand *wand);
bool saveImg(MagickWand *wand);
bool isCoverSizeLarger(MagickWand *cover, MagickWand *secret);
void stego(MagickWand *cover, MagickWand *secret);