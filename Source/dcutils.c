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
    int charCount = 0;

    unsigned int number = htonl(get_img_size(secretWand));
    char *size = (char *)malloc(4);
    fprintf(stdout, "\nSecret Size: %d", get_img_size(secretWand));

    memcpy(size, &number, 4);

    char *streamChar = (char *)malloc(1);
    char *tempChar = (char *)malloc(1);

    *streamChar = *size;

    // *streamChar = secretStream[streamIndex];

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
                        *streamChar = secretStream[streamIndex];

                        // Check if at the end of the stream
                        if ((streamIndex++) >= get_img_size(secretWand))
                        {
                            PixelSyncIterator(cover);
                            save_img(coverWand);
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

    char charSize[4];
    unsigned int size = 0;

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
                        fclose(fp);
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