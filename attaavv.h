/*********************************************************************

attaavv - ascii table to array and vice versa.
Library to read ascii tables of any size into 1D C arrays.

This is the header file for this library.

Copyright (C) 2013 Mohammad Akhlaghi
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

/* Make the macro definitions: */
/* For use in readasciitable:*/
/* BUFFER_NUM_ROWS is the unit of rows that the data 
   array will start with and that will be added to 
   the array if the number of rows is more  */
#define MAX_ROW_CHARS 100000
#define BUFFER_NUM_ROWS 1000
#define MAX_NUM_COMMENTS  100
#define HEADER_SIGN  '#'

/* The structure that is used to store the data: 
The shapes are counted from 1, so you can just
use the condition i<name.s1 for iterations.*/
struct ArrayInfo 
{
    int    nh;  /* Number of comments */
    char  **h;  /* Header */
    int    s0;  /* shape along first (zeroth) axis */
    int    s1;  /* shape along second axis */
    double *d;  /* data array (1D) */
};

/* Read and write an array to disk. */
void readasciitable (const char *, struct ArrayInfo *);
void writeasciitable (const char *, struct ArrayInfo *, 
                      int *, int *, int *, int *);

#endif
