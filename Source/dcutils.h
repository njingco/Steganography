#ifndef DCUTILS_H
#define DCUTILS_H

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "dcimage.h"
#include "helper.h"

bool stego(MagickWand *cover, MagickWand *secret);
bool unstego(MagickWand *cover);

#endif
