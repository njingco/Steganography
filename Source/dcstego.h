#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <openssl/rsa.h>

// #include "dcutils.h"
#include "dcimage.h"

void usage();
void errMsg(char *msg);
void setKey(char *key);
void startStego(char *coverImage, char *secretImage, char *key);
void startUnstego(char *coverImage, char *key);