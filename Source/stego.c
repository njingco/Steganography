
/*---------------------------------------------------------------------------------------
 * SOURCE FILE:	    stego
 * 
 * PROGRAM:		    stego
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
 * 
 * Compile:
 * makefile
 * 
 * Usage:
 * Stego
 *  ./stego -t stego -cm /img/MARBLES.BMP -sm /img/BLU.BMP 
 * 
 * Unstego
 *  ./stego -t unstego -cm /img/MARBLES.BMP 
 * ---------------------------------------------------------------------------------------*/

#include "stego.h"

int main(int argc, char *argv[])
{
    char coverImage[80];
    char secretImage[80];
    char key[80];
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

    if (stego)
    {
        startStego(coverImage, secretImage, key);
    }
    else
    {
        startUnstego(coverImage, key);
    }

    return 0;
}

void startStego(char *coverImage, char *secretImage, char *key)
{
    setKey(key);
    // check image type

    // encrypt secret image

    // check encrypter image file size

    // Stego images

    // Save Image
}

void startUnstego(char *coverImage, char *key)
{
    // check image type

    // untego image

    // decrypt secret image

    // Get new file name

    // Save Image
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