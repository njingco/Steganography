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
 * 
 * TODO:
 * - fix unstego image size
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
    char *supported[] = {"BMP", "BMP3"};
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
bool stego(MagickWand *coverWand, MagickWand *secretWand)
{
    PixelIterator *cover;
    PixelWand **cPixels;
    size_t cWidth;
    cover = NewPixelIterator(coverWand);
    char *secretStream = img_to_stream(secretWand);

    long rgb[3];
    char *rgbString;

    int streamIndex = 0;
    int binIndex = 7;

    char *streamChar = (char *)malloc(1);
    char *tempChar = (char *)malloc(1);

    *streamChar = secretStream[streamIndex];

    if ((cover == (PixelIterator *)NULL))
        ThrowWandException(coverWand);

    fprintf(stdout, "Stegoing Image...\n");

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
                    binIndex = 7;
                    // Check if at the end of the stream
                    if ((streamIndex++) >= get_img_size(secretWand))
                    {
                        fprintf(stdout, "SW: %d\n", get_img_size(secretWand));
                        PixelSyncIterator(cover);
                        save_img(coverWand);
                        return true;
                    }
                    // Move to next character in stream
                    *streamChar = secretStream[streamIndex];
                }
            }
            PixelSyncIterator(cover);
        }
    }
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
bool unstego(MagickWand *coverWand)
{
    PixelIterator *cover;
    PixelWand **cPixels;
    size_t cWidth;
    cover = NewPixelIterator(coverWand);
    long rgb[3];
    char *rgbString;

    int binIndex = 7;
    int even = 0;
    unsigned char tempChar = 0;
    int charCount = 0;

    FILE *fp = open_file();

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
                even = (!(is_even(rgb[k])));
                tempChar |= (even == 1) << binIndex;

                binIndex--;

                //reset binIndex
                if (binIndex < 0)
                {
                    binIndex = 7;
                    charCount++;

                    // if (charCount >= 86454)
                    // if (charCount <= 200)
                    // {
                    //     fprintf(stdout, "%d ", tempChar);

                    //     // fclose(fp);
                    //     // return true;
                    // }

                    // Write to to file
                    if (fputc(tempChar, fp) == -1)
                    {
                        return false;
                    }

                    tempChar = 0;
                }
            }
        }
    }
    fclose(fp);
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
 * FUNCTION:       open_file
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      NA
 *
 * RETURNS:        FILE * pointer to file stream
 *
 * NOTES:
 * return the created file stream
 * -----------------------------------------------------------------------*/
FILE *open_file()
{
    char filename[FILE_LEN];
    fprintf(stdout, "Enter New Image Name: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "wb+");
    return fp;
}

/*--------------------------------------------------------------------------
 * FUNCTION:       write_file
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      NA
 *
 * RETURNS:        FILE * pointer to file stream
 *                 char *buffer - message to write
 *                 int size - size of message
 *
 * NOTES:
 * Writes buffer to file
 * -----------------------------------------------------------------------*/
int write_file(FILE *file, char *buffer, int size)
{
    size_t result = -1;
    result = fwrite(buffer, 1, size, file);
    return result;
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
 * FUNCTION:       is_even
 *
 * DATE:           October  09, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      int num
 *
 * RETURNS:        boolean - true if even, false if odd
 *
 * NOTES:
 * Returns if integer is even
 * -----------------------------------------------------------------------*/
bool is_even(int num)
{
    if ((num % 2) == 0)
    {
        return true;
    }
    return false;
}