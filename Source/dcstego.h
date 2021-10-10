#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "dcutils.h"
#include "dcimage.h"

#define KEY_LEN 80

void usage();
void err_msg(char *msg);
void set_key(char *key);
void start_stego(char *coverImage, char *secretImage);
void start_unstego(char *coverImage);
void break_wands(MagickWand *cover, MagickWand *secret);
