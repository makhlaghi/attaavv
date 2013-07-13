/*********************************************************************

attaavv - ascii table to array and vice versa.
Library to read ascii tables of any size into 1D C arrays.

Copyright (C) 2013 Mohammad Akhlaghi
Tohoku University Astronomical Institute, Sendai, Japan.
http://astr.tohoku.ac.jp/~akhlaghi/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

**********************************************************************/

#include "attaavv.h"

/********************************************
 * This is here because for some strange    *
 * reason, strdup doesn't work in           *
 * ForComments                              *
 ********************************************/
char *
strdup (const char *s) 
{
    char *d = malloc (strlen (s) + 1);
    if (d != NULL)
        strcpy (d,s);
    return d;
}

/********************************************
 *  Get the comments string, add it to the  *
 *  comments array of pointers.             *
 ********************************************/
void 
ForComments(char ***comments, int *num_comments,
            long int *buff_num_comments, char *str)
{
    char **temp_c_pt;

    /* If the number of comments is zero, then no space 
    has been allocated for the comments. Allocate some space: */
    if (*num_comments==0)
    {
        *comments=malloc(*buff_num_comments*sizeof(char *)); 
        if (*comments==NULL)
        { 
            printf("\n### ERROR: malloc failed to");
            printf("create the comments in \"ForComments\".\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Copy the comment line into the comments array 
    and add to the number of comments.*/
    (*comments)[*num_comments] = strdup(str);
    (*num_comments)++;

    /* If it is getting longer than the actual length, 
    make the comments array longer: */
    if (*num_comments>=*buff_num_comments) 
    {
        *buff_num_comments+=BUFFER_NUM;
        temp_c_pt = realloc(*comments, *buff_num_comments*sizeof(char *));
        if(temp_c_pt != NULL) *comments=temp_c_pt;
        else 
        {
            printf("\n### Error: Comments array could");
            printf("not be reallocated in \"ForComments\".\n\n");
            exit(EXIT_FAILURE);
        }

    }
}

/********************************************
 *  Get the first data string and see how   *
 *  many columns of data there are in it.   * 
 ********************************************/
void 
CountCols(char *str, int *s1, double **table, 
         long int buff_num_rows)
{
    /* The original string is copied, because strtok
    will be used on it again in a later function. */
    char str_cpy[MAX_ROW_CHARS];
    strcpy(str_cpy, str);
    if(strtok(str_cpy," \n,")==NULL) return ;
    (*s1)++;
    while (1)
    {
        if(strtok(NULL," \n,")==NULL ) break;
        (*s1)++;
    }

    /* Assign space to the table:  */
    *table=malloc(buff_num_rows*(*s1)*sizeof(double)); 
    if (*table==NULL)
    { 
        printf("\n### ERROR: malloc failed to create a data table\n");
        exit(EXIT_FAILURE);
    }
}

/********************************************
 *  Knowing the number of columns, this     *
 *  function will read in each row          *
 ********************************************/
void 
AddRow(double **table, int *s0, int *s1,
       long int *buff_num_rows, char *str)
{
    /* Declarations: */ 
    int num_cols=1, z_index;
    char *strdata, *tempstr="";
    char **ExtraString=&tempstr;
    double *temp_d_pt;

    /* Set the index of the zeroth element in the row to be added:*/
    z_index=(*s0)*(*s1);

    /* Check if there is anything in this row: */
    if((strdata=strtok(str," \n,"))==NULL) return ;

    /* Read the first element of the row: */
    (*table)[z_index]=strtod(strdata, ExtraString);
    if (strlen(*ExtraString)>0)
    {
        printf("### Error: There are non-numerica data.\n");
        printf("###        in data row number %d",*s0+1);
        printf("(starting from 1).\n");
        exit(EXIT_FAILURE);
    }

    /* Continue with the rest: */
    while (1)
    {
        if( (strdata=strtok(NULL," \n,"))==NULL ) break;
        (*table)[z_index+num_cols++]=strtod(strdata, ExtraString);
        if (strlen(*ExtraString)>0)
        {
            printf("### Error: There are non-numerica data.\n");
            printf("###        in data row number ");
            printf("%d (starting from 1).\n",*s0+1);
            exit(EXIT_FAILURE);
        }        
        if (num_cols>*s1)
        {
            printf("### Error: Too many data in row");
            printf("%d (starting from 1).\n",*s0+1);
            printf("### --------------should have");
            printf("%d but has %d.\n", *s1, num_cols);
            exit(EXIT_FAILURE);
        }        
    }

    /* Check if the number of rows was not smaller than 
    what was expected. In a normal file the last line 
    will have 1 from the initialization.*/
    if (num_cols<*s1 && num_cols>1)
    {
        printf("### Error: Too few data in data row");
        printf("%d (starting from 1).\n",*s0+1);
        printf("### --------------should have");
        printf("%d but has %d.\n", *s1, num_cols);
        exit(EXIT_FAILURE);
    }        
    
    /* Add to the number of saved rows: */
    if (num_cols==*s1) (*s0)++;

    /* Check to see if the buffer size has not been exceeded.
    If it has, add space to the table array for the next rows. */
    if (*s0>=*buff_num_rows)
    {
        *buff_num_rows+=BUFFER_NUM;
        temp_d_pt=realloc(*table, *buff_num_rows*(*s1)*sizeof(double));
        if(temp_d_pt!=NULL) *table=temp_d_pt;
        else 
        {
            printf("\n### Error: Data array could not be reallocated.\n\n");
            exit(EXIT_FAILURE);
        }
    }
}

/****************************************************
 *  Read an ascii table into an array and also      *
 *  give the number of rows and columns in the      *
 *  array. In order to use this function you        *
 *  have to initialize the comments and table       *
 *  variables,                                      *
 *--------------------------------------------------*
 *-- EXAMPLE (don't include the <-- --> sections):- *
 *------------------------------------------------- *
 * <----------------------------------------------->*  
 * <--          For the preprocessor             -->*
 * <----------------------------------------------->*  
 * #define MAX_ROW_CHARS 100000                     *
 * #define BUFFER_NUM 1000                          *
 * #define COMMENT_SIGN  '#'                        *
 * <----------------------------------------------->*   
 * <-- In the function you want to read an array -->*
 * <-- Make these declarations to keep the info  -->*
 * <----------------------------------------------->*   
 * input_name[]="./data/cat.txt";                   *
 * struct ArrayInfo intable;                        *
 * <----------------------------------------------->*  
 * <--             Call the function,            -->*
 * <--   All the data will be in the set in the  -->*
 * <--             structure elements            -->*
 * <----------------------------------------------->*
 * readasciitable(&input_name[0], &intable);        *
 * <----------------------------------------------->*   
 * <--    In case you want to see the results:   -->*
 * <----------------------------------------------->*   
 * int i,j;
 * for(i=0;i<intable.nc;i++) 
 *     printf("%s", intable.c[i]);
 * for(i=0;i<intable.s0;i++)
 * {
 *     for (j=0;j<intable.s1;j++) 
 *          printf("%-7.2f ", intable.d[i*intable.s1+j]);
 *     printf("\n");
 * }
 * printf("Table shape is: %d, %d\n", intable.s0, intable.s1);
 * printf("Number of comments: %d\n", intable.nc);
 * free(intable.d); free(intable.c);
 ***************************************************/
void 
readasciitable (const char *filename, struct ArrayInfo *intable)
{
    /* Declarations: */
    long int line_counter=0;
    long int buff_num_rows=BUFFER_NUM;
    long int buff_num_comments=BUFFER_NUM;
    char str[MAX_ROW_CHARS], **temp_c_pt;
    double *temp_d_pt;
    FILE *fp=fopen(filename, "r");

    /* Check if the file opening was successful: */
    if (fp==NULL)
    {
        printf("\n### Failed to open input file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Set the array and comment sizes to zero for later steps */
    intable->s0=0;
    intable->s1=0;
    intable->nc=0;
    
    /* Go over the input file line by line and read
    the comments and data into the given arrays. */
    while(!feof(fp))
    {
        /* Read line by line: */
        fgets(str, sizeof(str), fp);
        line_counter++;

        /* Incase the length of the string is comparable
        to the macro defining the string length, abort the
        program and notify the user. 10 is just an arbitrary
        number, if one row is so close, others might exceed.
        it is not worth the risk to continue. */
        if ((long int) strlen(str)>MAX_ROW_CHARS-10)
        {
            printf("### Error: The number of characters in\n");
            printf("    line %ld are very near the buffer\n", line_counter); 
            printf("    limit set by MAX_ROW_CHARS, make it larger.\n\n");
            exit(EXIT_FAILURE);
        }

        /* Incase the line is a comment line: */
        if(str[0]==COMMENT_SIGN) 
            ForComments(&intable->c, &intable->nc, &buff_num_comments, str);

        /* If a line doesn't begin with a COMMENT_SIGN, it is 
        read as data and put into an array of data values. */
        else
        {
            /* If this is the first data row, count how many 
            columns it has to make the correct input format */
            if (intable->s1==0) 
                 CountCols(str, &intable->s1, &intable->d, buff_num_rows);

            /* We now have an initial array to begin with,
            we will fill it up with the rows */
            AddRow(&intable->d, &intable->s0, &intable->s1, 
                   &buff_num_rows, str);
        }
    }
    /* Shrink the comments array to the correct size: */
    if (intable->nc!=0)
    {
        temp_c_pt = realloc(intable->c, intable->nc*sizeof(char *));
        if(temp_c_pt != NULL) intable->c=temp_c_pt;
        else 
        {
            printf("\n### Error: Comments array could not be reallocated.\n\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Shrink the data array to the correct size: */
    temp_d_pt=realloc(intable->d, intable->s0*intable->s1*sizeof(double));
    if(temp_d_pt != NULL) intable->d=temp_d_pt;
    else 
        {
            printf("\n### Error: Data array could not be reallocated.\n\n");
            exit(EXIT_FAILURE);
        }

    /* Close the file and return 0 (meaning success)  */
    fclose(fp);
}

/****************************************************
 ** This function gets the formatting settings     **
 ** of the array as required by writeasciitable    **
 ** and makes an array of formatting conditions    **
 ** that is suitable for printing.                 **
 ****************************************************/
void 
DoFormatting(int numcols, char **fmt, char *fmt_all, int *int_cols, 
             int *accu_cols, int *space, int *prec)
{
    int i,j, found=0;
    char intstr[10], eacustr[10], otherstr[10];

    /* Define the formating for each kind of data: */
    sprintf(intstr, "%%-%d.0f", space[0]);
    sprintf(otherstr, "%%-%d.%dlg", space[1], prec[0]);
    sprintf(eacustr, "%%-%d.%df", space[2], prec[1]);

    /* Initialize the format array: */
    for (i=0;i<numcols;i++)
    {
        fmt[i]=&fmt_all[i*10];

        /* Check if an int should be placed: */
        found=0;
        for(j=0;j<numcols;j++)
        {
            if (int_cols[j]<0) break;
            if (i==int_cols[j]) 
            {
                strcpy(fmt[i], intstr);
                found=1;break;
            }
        }
        if (found==1) continue;

        /* Check if an extra precision should be placed: */
        found=0;
        for(j=0;j<numcols;j++)
        {
            if (accu_cols[j]<0) break;
            if (i==accu_cols[j]) 
            {
                strcpy(fmt[i], eacustr);
                found=1;break;
            }
        }
        if (found==1) continue;
 
        strcpy(fmt[i], otherstr);
    }
}

/********************************************
 *  Write an array to an ascii file         * 
 *  The example bellow assumes your array   *
 *  has come from readasciitable            *
 *  Explanations of the input format arrays:*
 *  int_colss: The columns that are integers*
 *  accu_rows: The columns that require     *
 *             extra accuracy               *
 *  NOTE: For the two arrays above, the     *
 *        last element has to be -1.
 *  space: The minimum field width given to *
 *         the three kinds of numbers, the  *
 *         first element is the space for   *
 *         integers, the second for floats  *
 *         (normal numbers) and the third   *
 *         is for those numbers that require*
 *         more accuracy                    *
 *  prec:  This is only for the last two    *
 *         kinds of numbers, it shows the   *
 *         decimal point precision that will*
 *         be used for their printing in    *
 *         order.                           *
---------------------------------------------
EXAMPLE:
Assuming that the array was read in from
readasciitable, this is how to run this function:
---------------------------------------------
char output_name[]="tempcat.txt";
int int_cols[]={0,4,-1}, accu_cols[]={1,2,-1};
int space[]={5,10,15}, prec[]={3,8};
--------------------------------------------
writeasciitable(&output_name[0], &intable, 
        int_cols, accu_cols, space, prec);

********************************************/
void 
writeasciitable (const char *filename, struct ArrayInfo *intable, 
                 int *int_cols, int *accu_cols, int *space, int *prec)
{

    /* Make an array of strings to hold the 
    formattings for each row. */
    int i,j;
    char **fmt=malloc((*intable).s1*sizeof(char *));
    char *fmt_all=malloc(10*(*intable).s1*sizeof(char));
    
    /* Open the output file: */
    FILE *fp=fopen(filename, "w");
    /* Check if the file opening was successful: */
    if (fp==NULL)
    {
        printf("\n### Failed to open output file:"); 
        printf("%s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    /* Prepare the formatting for each column */
    DoFormatting((*intable).s1, fmt, fmt_all, int_cols, 
            accu_cols, space, prec);

    /* Print the headers to file: */
    for(i=0;i<(*intable).nc;i++)    
        fprintf(fp, "%s", (*intable).c[i]);

    /* Print the data to file: */
    for(i=0;i<(*intable).s0;i++)
    {
        for(j=0;j<(*intable).s1;j++) 
            fprintf(fp, fmt[j], (*intable).d[i*(*intable).s1+j]);
        fprintf(fp, "\n");
    }

    /* Close the file and free all pointers: */
    free(fmt_all);free(fmt);
    fclose(fp);    
}
