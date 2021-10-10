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
#include <MagickWand/MagickWand.h>

/*--------------------------------------------------------------------------
 * FUNCTION:       is_supported
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *wand - wand of an image
 *
 * RETURNS:        bool -   true: supported, 
 *                          false: not supported
 *
 * NOTES:
 * returns true if image is supported, and false if image type is 
 * not supported
 * -----------------------------------------------------------------------*/
bool is_supported(MagickWand *wand)
{
    char *type = MagickGetImageFormat(wand);
    char *name = MagickGetImageFilename(wand);

    if (strcmp(BMP, type) == 0)
    {
        return true;
    }
    fprintf(stdout, "Image: %s  \nType: %s is NOT Supported\n\n", name, type);
    return false;
}

/*--------------------------------------------------------------------------
 * FUNCTION:       is_cover_larger
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *cover - wand of an cover image
 *                 MagickWand *secret - wand of an secret image
 *
 * RETURNS:        bool -   true: cover image is large enough, 
 *                          false: cover image is too small
 *
 * NOTES:
 * Returns true if cover letter is large enough to contain the secret
 * image
 * -----------------------------------------------------------------------*/
bool is_cover_larger(MagickWand *cover, MagickWand *secret)
{
    int coverLen = get_img_size(cover);
    int secretLen = get_img_size(secret);

    if (coverLen >= (secretLen * 8))
    {
        return true;
    }
    fprintf(stdout, "Cover Image NOT Large Enough\n\n");
    return false;
}

/*--------------------------------------------------------------------------
 * FUNCTION:       get_img_size
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *wand - wand of an image
 *
 * RETURNS:        int - size of image
 *
 * NOTES:
 * returns the size of the image
 * -----------------------------------------------------------------------*/
int get_img_size(MagickWand *wand)
{
    MagickSizeType wandLen = 0;

    // Cover image size
    MagickGetImageLength(wand, &wandLen);
    return wandLen;
}

/*--------------------------------------------------------------------------
 * FUNCTION:       save_img
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *wand - wand of an image
 *
 * RETURNS:        bool -   true: saved, 
 *                          false: failed
 *
 * NOTES:
 * returns true if image is successfuly saved
 * -----------------------------------------------------------------------*/
bool save_img(MagickWand *wand)
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

char *stuff_secret(MagickWand *cover, MagickWand *secret)
{
    // create streams of the wands
    char *coverStream = img_to_stream(cover);
    char *secretStream = img_to_stream(secret);
    // int coverLen = get_img_size(cover);
    // int secretLen = get_img_size(secret);

    // int c = 0;
    for (int s = 0; s < 1; s++)
    {
        fprintf(stdout, "\nsbin: %d ", secretStream[s]);

        // // // loop through each secret char binary
        // for (int i = 0; i < 8; i++)
        // {
        //     fprintf(stdout, "a%d", sbin[i]);
        //     //     byte cbin[8] = coverStream[c].getByte(); // to binary of each cover char
        //     //     cbin[7] = sbin[i];                       // change last cover bit to secrets bit

        //     //     c++;
        // }
    }

    return coverStream;
}

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
 * INTERFACE:      
 *
 * RETURNS:        
 *
 * NOTES:
 * 
 * -----------------------------------------------------------------------*/
void stego(MagickWand *coverWand, MagickWand *secretWand)
{
    PixelIterator *cover;
    PixelWand **cPixels;
    size_t cWidth;
    cover = NewPixelIterator(coverWand);
    // char *secretStream = img_to_stream(secretWand);

    long rgb[3];
    char *rgbString;
    if ((cover == (PixelIterator *)NULL))
        ThrowWandException(coverWand);

    fprintf(stdout, "Stegoing Image...\n");

    for (int i = 0; i < (ssize_t)MagickGetImageHeight(coverWand); i++)
    {
        cPixels = PixelGetNextIteratorRow(cover, &cWidth);

        if ((cPixels == (PixelWand **)NULL))
            break;

        for (int j = 0; j < (ssize_t)cWidth; j++)
        {
            rgbString = PixelGetColorAsString(cPixels[j]);
            parse_colour_string(rgb, rgbString);

            for (int k = 0; k < 3; k++)
            {
                // stuff secret in pixel
                rgb[k] = 0x01;
            }
            parse_color_int(rgb, rgbString);
            PixelSetColor(cPixels[j], rgbString);
        }
        PixelSyncIterator(cover);
    }

    if (!save_img(coverWand))
    {
        exit(1);
    }
}

/*--------------------------------------------------------------------------
 * FUNCTION:       
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      
 *
 * RETURNS:        
 *
 * NOTES:
 * 
 * -----------------------------------------------------------------------*/
void unstego(MagickWand *cover, MagickWand *secret)
{
}

/*--------------------------------------------------------------------------
 * FUNCTION:       img_to_stream
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *wand - wand of an image
 *
 * RETURNS:        char * of image stream
 *
 * NOTES:
 * returns  image stream
 * -----------------------------------------------------------------------*/
char *img_to_stream(MagickWand *wand)
{
    size_t size = (size_t)get_img_size(wand);
    return (char *)MagickGetImagesBlob(wand, &size);
}

FILE *open_file(char *filename)
{
    FILE *fp = fopen(filename, "wb+");
    return fp;
}

int write_file(FILE *file, char *buffer, int size)
{
    size_t result = -1;
    result = fwrite(buffer, 1, size, file);
    return result;
}

void parse_colour_string(long *clr, char *colorStr)
{
    // While there are more characters to process...
    while (*colorStr)
    {
        if (isdigit(*colorStr) && isdigit(*(colorStr + 1)))
        {
            // Found a number
            *clr = strtol(colorStr, &colorStr, 10); // Read number
            clr++;
        }
        else
        {
            colorStr++;
        }
    }
}

void parse_color_int(long *clr, char *colorStr)
{
    sprintf(colorStr, "srgb(%lu,%lu,%lu)", clr[0], clr[1], clr[2]);
}