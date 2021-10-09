#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "dcutils.h"
#include "dcimage.h"

#define KEY_LEN 80
#define FILE_LEN 80

void usage();
void errMsg(char *msg);
void setKey(char *key);
void startStego(char *coverImage, char *secretImage);
void startUnstego(char *coverImage);
void breakWands(MagickWand *cover, MagickWand *secret);
