/*---------------------------------------------------------------------------------------
 * SOURCE FILE:	    dcimage
 * 
 * PROGRAM:		    dcimage
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
 * this module will contain all of the functions for the image processing and  manipulation.
 * ---------------------------------------------------------------------------------------*/

#include "dcimage.h"

bool isSupported(MagickWand *wand)
{
    char *type = MagickGetImageFormat(wand);
    char *name = MagickGetImageFilename(wand);

    if (strcmp(BMP, type) == 0)
    {
        fprintf(stdout, "Image %s Type: %s is Supported\n", name, type);
        return true;
    }
    fprintf(stdout, "Image %s Type: %s is NOT Supported\n", name, type);
    return false;
}

bool isCoverSizeLarger(MagickWand *cover, MagickWand *secret)
{
    MagickSizeType coverLen = 0;
    MagickSizeType secretLen = 0;

    // Cover image size
    MagickGetImageLength(cover, &coverLen);

    // Secret image size
    MagickGetImageLength(secret, &secretLen);

    fprintf(stdout, "CL: %lld \nSL: %lld \n", coverLen, secretLen);

    if (coverLen >= (secretLen * 8))
    {
        fprintf(stdout, "Cover Image Large Enough\n");

        return true;
    }
    fprintf(stdout, "Cover Image NOT Large Enough\n");
    return false;
}

bool saveImg(MagickWand *wand, char *filename)
{
    MagickBooleanType status;
    status = MagickWriteImages(wand, filename, MagickTrue);
    if (status == MagickFalse)
    {
        fprintf(stderr, "Error with Saving Image\n");
        return false;
    }
    fprintf(stdout, "Saved Image\n");
    return true;
}