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
 * his module will contain all of the functions for the image processing and manipulation
 * ---------------------------------------------------------------------------------------*/
#include "dcimage.h"

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
    char *supported[] = {"BMP", "BMP3", "PNG", "JPEG"};
    char *type = MagickGetImageFormat(wand);
    char *name = MagickGetImageFilename(wand);

    int length = (sizeof(supported) / sizeof(char *));

    for (int i = 0; i < length; i++)
    {
        if (strcmp(supported[i], type) == 0)
        {
            return true;
        }
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

/*--------------------------------------------------------------------------
 * FUNCTION:       parse_colour_string
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      long *clr - pointer to array of long to store rgb value
 *                 char *colorStr - string of rgb 
 *
 * RETURNS:        
 *
 * NOTES:
 * This function turns the rgb string to an array of longs representing
 * r, g, b
 * -----------------------------------------------------------------------*/
void parse_colour_string(long *clr, char *colorStr)
{
    // While there are more characters to process...
    while (*colorStr)
    {
        if (isdigit(*colorStr))
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

/*--------------------------------------------------------------------------
 * FUNCTION:       parse_color_int
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      long *clr - array of long of r,g,b 
 *                 char *colorStr - pointer to char array to store rgb string
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Takes an array of long representing rgb and creates a string in the
 * format "srgb(0,0,0)"
 * -----------------------------------------------------------------------*/
void parse_color_int(long *clr, char *colorStr)
{
    sprintf(colorStr, "srgb(%lu,%lu,%lu)", clr[0], clr[1], clr[2]);
}

/*--------------------------------------------------------------------------
 * FUNCTION:       breakWands
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *cover, MagickWand *secret - wands of images
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Closes the MagickWands
 * -----------------------------------------------------------------------*/
void break_wands(MagickWand *cover, MagickWand *secret)
{
    cover = DestroyMagickWand(cover);
    secret = DestroyMagickWand(secret);
    MagickWandTerminus();
}

/*--------------------------------------------------------------------------
 * FUNCTION:       breakWand
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      MagickWand *cover
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Closes the MagickWand
 * -----------------------------------------------------------------------*/
void break_wand(MagickWand *cover)
{
    cover = DestroyMagickWand(cover);
    MagickWandTerminus();
}