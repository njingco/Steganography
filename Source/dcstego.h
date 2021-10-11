#ifndef DCSTEGO_H
#define DCSTEGO_H

#include "dcutils.h"
#include "dcimage.h"
#include "helper.h"

#define KEY_LEN 32

void start_stego(char *coverImage, char *secretImage);
void start_unstego(char *coverImage);
void set_key(char *key1);
void usage();

#endif
