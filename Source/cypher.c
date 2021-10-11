/**
 * Compile:
 * gcc -Wall -o cypher.o cypher.c -lssl -lcrypto `pkg-config --cflags --libs MagickWand`
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <MagickWand/MagickWand.h>
#include <termios.h>

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int get_img_size(MagickWand *wand)
{
    MagickSizeType wandLen = 0;

    // Cover image size
    MagickGetImageLength(wand, &wandLen);
    return wandLen;
}

int main()
{
    fprintf(stdout, "Start...\n\n");

    FILE *enc = fopen("Encrypted", "w+");
    FILE *dec = fopen("Decrypted", "w+");

    // // MagicWand ---------------------------------------------------------------

    MagickWandGenesis();
    MagickWand *wand = NewMagickWand();
    // // Check Secret Image
    if (MagickReadImage(wand, "./img/BMP/BLU.BMP") == MagickFalse)
    {
        fprintf(stdout, "Wand problem\n");
        wand = DestroyMagickWand(wand);
        MagickWandTerminus();
        exit(1);
    }

    int imgSize = get_img_size(wand);
    size_t size = (size_t)imgSize;
    char *plaintext = (char *)MagickGetImagesBlob(wand, &size);

    // Read File ---------------------------------------------------------------

    // int imgSize = 0;

    // FILE *img = fopen("./img/PNG/SUN.png", "r+");
    // fseek(img, 0L, SEEK_END);
    // imgSize = ftell(img);
    // fseek(img, 0L, SEEK_SET);

    // unsigned char *plaintext = calloc(1, imgSize + 1);
    // if (1 != fread(plaintext, imgSize, 1, img))
    //     fclose(img);

    // fprintf(stdout, "Text Size: %d \nPlainText: %s\n\n", imgSize, plaintext);

    // Init ------------------------------------------------------------------

    unsigned char key[80];
    scanf("Key: %hhu", key);
    unsigned char *iv = (unsigned char *)"01234567890123412501234560123456";

    int decryptedtext_len, ciphertext_len;

    unsigned char ciphertext[imgSize];
    unsigned char decryptedtext[imgSize];

    fprintf(stdout, "Key : %d\n", key);
    // Encrypt ---------------------------------------------------------------

    ciphertext_len = encrypt((unsigned char *)plaintext, imgSize, key, iv, ciphertext);
    fwrite(ciphertext, 1, ciphertext_len, enc);

    fprintf(stdout, "ciphertext_len: %d\n", ciphertext_len);

    // // Decrypt ---------------------------------------------------------------

    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
    fwrite(decryptedtext, 1, decryptedtext_len, dec);

    fprintf(stdout, "decryptedtext_len: %d\n", decryptedtext_len);

    free(plaintext);
    fclose(enc);
    fclose(dec);
    return 0;
}