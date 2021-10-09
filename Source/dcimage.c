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
        fprintf(stdout, "Image: %s \nType: %s is Supported\n\n", name, type);
        return true;
    }
    fprintf(stdout, "Image: %s  \nType: %s is NOT Supported\n\n", name, type);
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

    fprintf(stdout, "\nCL: %lld \nSL: %lld \n\n", coverLen, secretLen);

    if (coverLen >= (secretLen * 8))
    {
        fprintf(stdout, "Cover Image Large Enough\n\n");

        return true;
    }
    fprintf(stdout, "Cover Image NOT Large Enough\n\n");
    return false;
}

bool saveImg(MagickWand *wand)
{
    MagickBooleanType status;

    char newImgName[FILE_LEN];
    fprintf(stdout, "Enter New Image Name:");
    scanf("%s", newImgName);

    status = MagickWriteImages(wand, newImgName, MagickTrue);
    if (status == MagickFalse)
    {
        fprintf(stderr, "Error with Saving Image\n");
        return false;
    }
    fprintf(stdout, "Saved Image\n");
    return true;
}

void stego(MagickWand *cover, MagickWand *secret)
{
}