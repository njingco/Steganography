/*---------------------------------------------------------------------------------------
 * SOURCE FILE:	    dcstego
 * 
 * PROGRAM:		    dcstego
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
 * the main function that will contain the general functionality such as  
 * parsing command line arguments, checking file sizes, file formats, etc. 
 * 
 * Compile:
 * makefile
 * 
 * Usage:
 * Stego
 *  ./dcstego -t stego -cm ./img/MARBLES.BMP -sm ./img/BLU.BMP 
 * 
 * Unstego
 *  ./dcstego -t unstego -cm ./img/MARBLES.BMP 
 * ---------------------------------------------------------------------------------------*/
#include "dcstego.h"

int main(int argc, char *argv[])
{
    char coverImage[80];
    char secretImage[80];
    int count;
    int stego;

    /* Tell them how to use this thing */
    if ((argc < 4) || (argc > 7))
    {
        usage();
        exit(0);
    }

    /* No error checking on the args...next version :) */
    for (count = 0; count < argc; ++count)
    {
        if (strcmp(argv[count], "-t") == 0)
        {
            if (strcmp(argv[count + 1], "stego"))
            {
                stego = 1;
            }
        }
        else if (strcmp(argv[count], "-cm") == 0)
        {
            strncpy(coverImage, argv[count + 1], 79);
        }
        else if (strcmp(argv[count], "-sm") == 0)
        {
            strncpy(secretImage, argv[count + 1], 79);
        }
    }

    /* Check if task is to stego and run process*/
    if (stego)
    {
        startStego(coverImage, secretImage);
    }
    else
    {
        startUnstego(coverImage);
    }

    return 0;
}

void startStego(char *coverImage, char *secretImage)
{
    // char *key;
    // Get password from user
    // setKey(key);

    // check image type
    MagickWandGenesis();
    MagickWand *cover = NewMagickWand();
    MagickWand *secret = NewMagickWand();
    // Check Cover Image
    if (MagickReadImage(cover, coverImage) == MagickFalse)
    {
        errMsg("Cover Image Error");
        exit(1);
    }
    // Check Secret Image
    if (MagickReadImage(secret, secretImage) == MagickFalse)
    {
        errMsg("Secret Image Error");
        exit(1);
    }

    fprintf(stdout, "Cover Image: %s\n", coverImage);
    fprintf(stdout, "Secret Image: %s\n", secretImage);

    // Check if images file type are supported
    if (!isSupported(cover) || !isSupported(secret))
    {
        exit(1);
    }

    // encrypt secret image

    // check encrypter image file size
    if (!isCoverSizeLarger(cover, secret))
    {
        exit(1);
    }
    // Stego images

    // Save Image
    exit(0);
}

void startUnstego(char *coverImage)
{
    // check image type

    // untego image

    // decrypt secret image

    // Get new file name

    // Save Image

    exit(0);
}

void setKey(char *key)
{
    struct termios term;
    int keyMatch = 0;
    char key2[80];

    // Get password
    tcgetattr(fileno(stdin), &term);
    while (!keyMatch)
    {
        // turn off ECHO
        term.c_lflag &= ~ECHO;
        tcsetattr(fileno(stdin), 0, &term);

        fprintf(stdout, "Enter key:");
        scanf("%s", key);
        fprintf(stdout, "\nEnter key again:");
        scanf("%s", key2);
        fprintf(stdout, "\n");

        // Check if key matches
        if (strcmp(key, key2) == 0)
        {
            keyMatch = 1;
        }
        else
        {
            fprintf(stdout, "\nKeys do not match\n");
        }

        // turn on ECHO
        term.c_lflag |= ECHO;
        tcsetattr(fileno(stdin), 0, &term);
    }
}
void usage()
{
    fprintf(stdout, "Stego Image:\n");
    fprintf(stdout, "-t stego -cm [cover image path] -sm [secret image path]\n\n");

    fprintf(stdout, "Unstego Image: \n");
    fprintf(stdout, "-t unstego -cm [cover image path] \n\n");
}

void errMsg(char *msg)
{
    fprintf(stderr, "%s\n", msg);
}