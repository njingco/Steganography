#ifndef DCUTILS_H
#define DCUTILS_H

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "dcimage.h"
#include "helper.h"

#define IV "01234567890123412501234560123456"

bool stego(MagickWand *coverWand, MagickWand *secretWand, unsigned char *key, unsigned char *iv);
bool unstego(MagickWand *coverWand, unsigned char *key, unsigned char *iv);
void handleErrors(void);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);

#endif
