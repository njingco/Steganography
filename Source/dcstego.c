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
 *  ./dcstego -t stego -cm ./img/BMP/MARBLES.BMP -sm ./img/BMP/BLU.BMP 
 *  ./dcstego -t stego -cm ./img/PNG/FOREST.png -sm ./img/PNG/TEAL.png 
 *  ./dcstego -t stego -cm ./img/JPG/FOREST.jpg -sm ./img/JPG/RED.jpg 
 * 
 * Unstego
 *  ./dcstego -t unstego -cm secret.BMP 
 * 
 * TODO:
 * add encryption and decryption
 * fix unstego fize size
 * ---------------------------------------------------------------------------------------*/
#include "dcstego.h"

int main(int argc, char *argv[])
{
    char coverImage[80];
    char secretImage[80];
    int count;
    int stego = 0;

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
            if (strcmp(argv[count + 1], "stego") == 0)
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
        fprintf(stdout, "Staring Stego Process\n\n");
        start_stego(coverImage, secretImage);
    }
    else
    {
        fprintf(stdout, "Staring Unstego Process\n\n");
        start_unstego(coverImage);
    }

    return 0;
}

/*--------------------------------------------------------------------------
 * FUNCTION:       start_stego
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      char *coverImage - file path of cover image
 *                 char *secretImage - file path of secret image
 *
 * RETURNS:        NA
 *
 * NOTES:
 * This function runs the stego process
 * -----------------------------------------------------------------------*/
void start_stego(char *coverImage, char *secretImage)
{
    // char key[KEY_LEN];
    // Get password from user
    // set_key(key);

    // check image type
    MagickWandGenesis();
    MagickWand *cover = NewMagickWand();
    MagickWand *secret = NewMagickWand();

    // Check Cover Image
    if (MagickReadImage(cover, coverImage) == MagickFalse)
    {
        err_msg("Cover Image Wand Error");
        break_wands(cover, secret);
        exit(1);
    }
    // Check Secret Image
    if (MagickReadImage(secret, secretImage) == MagickFalse)
    {
        err_msg("Secret Image Wand Error");
        break_wands(cover, secret);
        exit(1);
    }

    // Check if images file type are supported
    if (!is_supported(cover) || !is_supported(secret))
    {
        break_wands(cover, secret);
        exit(1);
    }

    // encrypt secret image--------------------------------------

    // check encrypter image file size
    if (!is_cover_larger(cover, secret))
    {
        break_wands(cover, secret);
        exit(1);
    }

    // Stego images and save new image
    if (!stego(cover, secret))
    {
        err_msg("Something went wrong with the stego process\n");
        break_wands(cover, secret);
        exit(1);
    }

    // Close Wands
    // break_wands(cover, secret);

    exit(0);
}

/*--------------------------------------------------------------------------
 * FUNCTION:       start_unstego
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      char *coverImage - file path of cover image
 *
 * RETURNS:        NA
 *
 * NOTES:
 * This function runs the unstego process
 * -----------------------------------------------------------------------*/
void start_unstego(char *coverImage)
{
    // check image type
    MagickWandGenesis();
    MagickWand *cover = NewMagickWand();

    // Check Cover Image
    if (MagickReadImage(cover, coverImage) == MagickFalse)
    {
        err_msg("Cover Image Error");
        break_wand(cover);
        exit(1);
    }

    if (!is_supported(cover))
    {
        break_wand(cover);
        exit(1);
    }

    // unstego image and save image
    if (!unstego(cover))
    {
        err_msg("Something went wrong with the unstegoing process\n");
        break_wand(cover);
        exit(1);
    }

    // Close Wands
    // break_wand(cover);

    exit(0);
}

/*--------------------------------------------------------------------------
 * FUNCTION:       set_key
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      char *key - pointer to key for encryption
 *
 * RETURNS:        NA
 *
 * NOTES:
 * This function gets the password key for the encyption process
 * -----------------------------------------------------------------------*/
void set_key(char *key)
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

/*--------------------------------------------------------------------------
 * FUNCTION:       usage
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      NA
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Prints the Usage of the program
 * -----------------------------------------------------------------------*/
void usage()
{
    fprintf(stdout, "Stego Image:\n");
    fprintf(stdout, "-t stego -cm [cover image path] -sm [secret image path]\n\n");

    fprintf(stdout, "Unstego Image: \n");
    fprintf(stdout, "-t unstego -cm [cover image path] \n\n");
}
