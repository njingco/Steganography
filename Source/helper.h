#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <MagickWand/MagickWand.h>
#include <arpa/inet.h>

#define FILE_LEN 80
#define BUFFER_SIZE 1028

void err_msg(char *msg);
bool is_even(int num);
FILE *open_file();
int write_file(FILE *file, char *buffer, int size);

#endif
