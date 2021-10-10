/*---------------------------------------------------------------------------------------
 * SOURCE FILE:	    helper
 * 
 * PROGRAM:		    helper
 * 
 * FUNCTIONS:		
 *                  
 * 
 * DATE:			October 10, 2021
 * 
 * REVISIONS:		NA
 * 
 * DESIGNERS:       Nicole Jingco
 * 
 * PROGRAMMERS:		Nicole Jingco
 * 
 * Notes:
 * This file contains helper functions unrelated to stegoing or encryption
 * ---------------------------------------------------------------------------------------*/

#include "helper.h"

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
 * FUNCTION:       err_msg
 *
 * DATE:           October  08, 2021
 *
 * REVISIONS:      NA
 * 
 * DESIGNER:       Nicole Jingco
 *
 * PROGRAMMER:     Nicole Jingco
 *
 * INTERFACE:      char *msg - error message string
 *
 * RETURNS:        NA
 *
 * NOTES:
 * Prints the error message
 * -----------------------------------------------------------------------*/
void err_msg(char *msg)
{
    fprintf(stderr, "%s\n", msg);
}
