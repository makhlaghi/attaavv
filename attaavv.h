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

#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Make the macro definitions:
   MAX_ROW_CHARS specifies the maximum number of
      characters in a row in the ascii data file.
      if number in the data file exceeds this, 
      the program will abort and notify you.
   BUFFER_NUM is the unit of elements that will  
      initially be set for the number of rows in 
      the data, number of comments. If the number
      of either case is more than this the program
      will add to the array. So there is no problem. */
#define MAX_ROW_CHARS 100000
#define BUFFER_NUM 1000
#define COMMENT_SIGN  '#'

/* The structure that is used to store the data: 
The values in the shapes are counted from 1, so 
you can just use the condition i<name.s1 for 
iterations over the data or the comments. */
struct ArrayInfo 
{
    int    nc;  /* Number of comments */
    char  **c;  /* Array of comments */
    int    s0;  /* shape along zeroth axis */
    int    s1;  /* shape along first axis */
    double *d;  /* data array (1D) */
};

/* Read and write an array to disk. */
void readasciitable (const char *, struct ArrayInfo *);
void writeasciitable (const char *, struct ArrayInfo *, 
                      int *, int *, int *, int *);

#endif
