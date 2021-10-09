#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "dcutils.h"
#include "dcimage.h"

void usage();
void errMsg(char *msg);
void setKey(char *key);
void startStego(char *coverImage, char *secretImage);
void startUnstego(char *coverImage);