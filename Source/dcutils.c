/*---------------------------------------------------------------------------------------
 * SOURCE FILE:	    dcutils
 * 
 * PROGRAM:		    dcutils
 * 
 * FUNCTIONS:		
 *                  
 * 
 * DATE:			October 4, 2021
 * 
 * REVISIONS:		NA
 * 
 * DESIGNERS:       Nicole Jingco
 * 
 * PROGRAMMERS:		Nicole Jingco
 * 
 * Notes:
 * this module will contain the two main functions for hiding and extracting the data.
 * ---------------------------------------------------------------------------------------*/
#include "dcutils.h"

// void encryptImg(FILE *img, char *key, char *iv)
// {
//     OpenSSL_add_all_algorithms();
//     ERR_load_crypto_strings();

//     unsigned char ciphertext[128];
//     unsigned char tag[16];
//     int ciphertext_len = 0;

//     ciphertext_len = encrypt(img, strlen(img), aad, strlen(aad), key, iv, ciphertext, tag);

//     printf("Ciphertext is:\n");
//     BIO_dump_fp(stdout, ciphertext, ciphertext_len);
//     printf("Tag is:\n");
//     BIO_dump_fp(stdout, tag, 14);
// }

// void decryptImg(FILE *img, char *key)