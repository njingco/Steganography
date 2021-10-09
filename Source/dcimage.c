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

double getImgSize(MagickWand *wand, const *filename)
{
    /* Read the input image */
    MagickReadImage(wand, filename);

    fprintf(stdout, "");
    return 0;
}