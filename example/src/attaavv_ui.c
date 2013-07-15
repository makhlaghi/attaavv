/*********************************************
 *********************************************
 *********************************************
 **          Mohammad Akhalaghi             **
 **   http://astr.tohoku.ac.jp/~akhlaghi/   **
 **              July 2013                  **
 *********************************************
 *********************************************
 *********************************************/

#include <stdlib.h>
#include "attaavv.h"

int main (void)
{
    /* Definitions: */
    char input_name[]="./data/cat.txt";
    char output_name[]="tempcat.txt";
    int int_cols[]={0,2,-1}, accu_cols[]={4,-1};
    int space[]={5,10,15}, prec[]={6,8};

    /* Declare the structure that will keep the array: */
    struct ArrayInfo intable;

    /* Read in the input data as an array:*/
    readasciitable(&input_name[0], &intable);

    /* Print the table to a file */
    writeasciitable(&output_name[0], &intable, int_cols, accu_cols, space, prec);

    /* Free up the comments and table arrays: */
    free(intable.d); free(intable.c);
    return 0;
}

