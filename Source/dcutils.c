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

/*--------------------------------------------------------------------------
 * FUNCTION:       stego
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *coverWand
 *                 MagickWand *secretWand
 *
 * RETURNS:        true if successfull in stegoing image,
 *                 false if unsuccessful in steoing image
 *
 * NOTES:
 * This function handles the steganography process by filling the last 
 * bit of each carrier byte and saves the image
 * -----------------------------------------------------------------------*/
bool stego(MagickWand *coverWand, MagickWand *secretWand, unsigned char *key, unsigned char *iv)
{
    PixelIterator *cover;
    PixelWand **cPixels;
    size_t cWidth;
    cover = NewPixelIterator(coverWand);

    long rgb[3];
    char *rgbString;

    int streamIndex = 0;
    int binIndex = 7;
    int charCount = 0;

    char *streamChar = (char *)malloc(1);
    char *tempChar = (char *)malloc(1);

    // Ecrypt Secret Data and get Cypher size

    int imgSize = get_img_size(secretWand);
    char *secretStream = img_to_stream(secretWand);

    unsigned char ciphertext[imgSize];
    int ciphertext_len = encrypt((unsigned char *)secretStream, imgSize, key, iv, ciphertext);

    unsigned int number = htonl(ciphertext_len);
    char *size = (char *)malloc(4);
    memcpy(size, &number, 4);

    *streamChar = *size;

    fprintf(stdout, "\nSecret Size: %d", imgSize);
    fprintf(stdout, "\nCypher Size: %d", ciphertext_len);

    // Start Pixel Iterator
    if ((cover == (PixelIterator *)NULL))
        ThrowWandException(coverWand);

    fprintf(stdout, "\nStegoing Image...\n");

    // Get Height
    for (int i = 0; i < (ssize_t)MagickGetImageHeight(coverWand); i++)
    {
        //go thorugh each row
        cPixels = PixelGetNextIteratorRow(cover, &cWidth);

        if ((cPixels == (PixelWand **)NULL))
            break;

        for (int j = 0; j < (ssize_t)cWidth; j++)
        {
            // Get Pixel's RGB
            rgbString = PixelGetColorAsString(cPixels[j]);
            parse_colour_string(rgb, rgbString);

            for (int k = 0; k < 3; k++)
            {
                // Go through each bit in a char
                *tempChar = *streamChar >> binIndex;

                // if bits don't match change rgb value by 1
                if ((!is_even(rgb[k]) && is_even(*tempChar)) || (is_even(rgb[k]) && !is_even(*tempChar)))
                {
                    //change color
                    if (rgb[k] >= 255)
                    {
                        rgb[k] = rgb[k] - 1;
                    }
                    else
                    {
                        rgb[k] = rgb[k] + 1;
                    }
                }

                // put modifie pixel back
                parse_color_int(rgb, rgbString);
                PixelSetColor(cPixels[j], rgbString);

                //reset binIndex
                binIndex--;
                if (binIndex < 0)
                {

                    // Move to next character in stream
                    if (charCount < 4)
                    {
                        *streamChar = *(++size);
                        fprintf(stdout, "\nSize: %02x", *streamChar);
                    }
                    else
                    {
                        *streamChar = ciphertext[streamIndex];

                        // Check if at the end of the stream
                        if ((streamIndex++) >= ciphertext_len)
                        {
                            PixelSyncIterator(cover);
                            save_img(coverWand);

                            free(tempChar);
                            free(streamChar);
                            return true;
                        }
                    }

                    charCount++;
                    binIndex = 7;
                }
            }
            PixelSyncIterator(cover);
        }
    }
    fprintf(stdout, "\n rip");

    free(tempChar);
    free(streamChar);
    return false;
}

/*--------------------------------------------------------------------------
 * FUNCTION:       unstego
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *coverWand
 *
 * RETURNS:        true if successfull in unstegoing image,
 *                 false if unsuccessful in unsteoing image
 *
 * NOTES:
 * This function handles the steganography process by extracting the 
 * last bit in each cover image byte and writing it to the image file
 * -----------------------------------------------------------------------*/
bool unstego(MagickWand *coverWand, unsigned char *key, unsigned char *iv)
{
    PixelIterator *cover;
    PixelWand **cPixels;
    size_t cWidth;
    cover = NewPixelIterator(coverWand);
    long rgb[3];
    char *rgbString;

    char charSize[4];
    unsigned int size = 0;

    int binIndex = 7;
    int even = 0;
    unsigned char tempChar = 0;
    int charCount = 0;

    FILE *fp = fopen("temp", "w+");

    if ((cover == (PixelIterator *)NULL))
    {
        ThrowWandException(coverWand);
        return false;
    }

    fprintf(stdout, "Unstegoing Image...\n");

    // Get Height
    for (int i = 0; i < (ssize_t)MagickGetImageHeight(coverWand); i++)
    {
        //go thorugh each row
        cPixels = PixelGetNextIteratorRow(cover, &cWidth);

        if ((cPixels == (PixelWand **)NULL))
            break;

        for (int j = 0; j < (ssize_t)cWidth; j++)
        {
            // Get Pixel's RGB
            rgbString = PixelGetColorAsString(cPixels[j]);
            parse_colour_string(rgb, rgbString);

            for (int k = 0; k < 3; k++)
            {
                // Get file size form the first 32 Bytes

                even = (!(is_even(rgb[k])));
                tempChar |= (even == 1) << binIndex;

                binIndex--;

                //reset binIndex
                if (binIndex < 0)
                {
                    binIndex = 7;

                    if (charCount < 4)
                    {
                        charSize[charCount] = tempChar;
                        fprintf(stdout, "\nSize: %02x", charSize[charCount]);
                    }
                    else if (charCount == 4)
                    {
                        // convert char array to int size
                        memcpy(&size, &charSize, 4);
                        size = htonl(size);
                        fprintf(stdout, "\nTotal Size: %d", size);
                    }
                    else
                    {
                        if (fputc(tempChar, fp) == -1)
                        {
                            return false;
                        }
                    }
                    charCount++;
                    tempChar = 0;
                    if (charCount >= (size + 5))
                    {
                        fprintf(stdout, "\n\nSize: %d\n", size);
                        unsigned char decryptedtext[size];

                        fclose(fp);
                        fp = fopen("temp", "r");

                        unsigned char *ciphertext = (unsigned char *)malloc(size);
                        fread(ciphertext, size, 1, fp);
                        fclose(fp);

                        FILE *secretFile = fopen("secret", "w+");
                        fwrite(ciphertext, 1, size, secretFile);
                        fclose(secretFile);

                        int decryptedtext_len = decrypt(ciphertext, size, key, iv, decryptedtext);
                        secretFile = open_file();
                        fwrite(decryptedtext, 1, decryptedtext_len, secretFile);

                        fclose(secretFile);
                        fprintf(stdout, "\nDone Making Imagee\n");
                        return true;
                    }
                }
            }
        }
    }
    fclose(fp);
    fprintf(stdout, "\nDone Making Image\n");
    return true;
}

/*--------------------------------------------------------------------------
 * FUNCTION:       handleErrors
 *
 * DATE:           October  10, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       OpenSSL Wiki
 *
 * PROGRAMMER:     OpenSSL Wiki
 *
 * INTERFACE:      NA
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Handles the errors
 * 
 * Source:
 * https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 * -----------------------------------------------------------------------*/
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

/*--------------------------------------------------------------------------
 * FUNCTION:       encrypt
 *
 * DATE:           October  10, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       OpenSSL Wiki
 *
 * PROGRAMMER:     OpenSSL Wiki
 *
 * INTERFACE:      NA
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Encrypts the unsigned char *
 * 
 * Source:
 * https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 * -----------------------------------------------------------------------*/
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext)
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

/*--------------------------------------------------------------------------
 * FUNCTION:       decrypt
 *
 * DATE:           October  10, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       OpenSSL Wiki
 *
 * PROGRAMMER:     OpenSSL Wiki
 *
 * INTERFACE:      NA
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Decrypts the Cipher
 * 
 * Source:
 * https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 * -----------------------------------------------------------------------*/
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
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
