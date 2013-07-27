attaavv
=======

attaavv - ascii table to array and vice versa in C!

----------------------------------------
About this tool
----------------------------------------
attaavv will read an ascii table of data (separated by space or 
comma) and save the table as a C array of doubles along with 
possible comments and other relevant information into a C 
structure. Components of the structure are explained bellow. 
It is written fully in C89. If the input table contains
any non-numeric elements, the program will either abort and notify
the user or change the value to a defined value. This is 
a very simple program, only meant to do the most basic input 
and output of tabular numerical data in a nicely formatted way. 

attaavv is not a standalone program, it is made to be placed 
inside a C program in order to read an ascii table and save the
result as a 1D C array (for a 2D array look bellow). Refer to
the example file for a simple example.

The program has lots of comments to explain every step.

----------------------------------------
Copyright:
----------------------------------------
Copyright (C) 2013 Mohammad Akhlaghi

Tohoku University Astronomical Institute

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

----------------------------------------
Output structure
----------------------------------------
The function fills in the ArrayInfo structure definition, declared
in attaavv.h. Comments specified by the COMMENT_SIGN macro. 
The components of the structure are:

----  c: Comments string.

---- s0: Number of rows (zeroth axis) in the data table.

---- s1: Number of columns (first axis) in the data table.

----  d: Data array (1D).

---- nr: Number of replaced elements.

----  r: Array showing the positions of the replaced elements.

The "d" element of the output structure (the data in the table) 
is a 1D array containing all the data which you can use
by indexing the output with [i*n2+j] where n2 is the second
axis size and i and j are the first and second axis indexes 
respectively. To convert to a 2D array read bellow. 

----------------------------------------
Non-numeric elements in the table:
----------------------------------------
The output of this program has to be a double (C type) array. 
Therefore if the table has character elements or some elements 
that cannot be read, the user should define what the program should 
do. For this job the CHAR_REPLACEMENT macro is defined in 
attaavv.h. If the value of this macro is 0, the program will 
report the position of the first non-numeric element and abort. 
If it has any non-zero value, the program will replace any 
non-numeric element with the value in that macro.

The number of replaced elements in the table is reported in 
the "nr" member (an integer) of the output structure and the 
positions of those replaced elements is stored in the "r" member 
(1D array or length 2*nr) of the output structure. Each pair of
elements in the "r" array shows the positions of each replaced
member in order of (zeroth axis and first axis), therefore this
1D array has 2*nr length. Again you can look at it like a 2D 
array as shown below.


----------------------------------------
Running the example:
----------------------------------------
In order to test run the example, put the two .h and .c files in the
src directory and run ($ is the shell prompt on the command line):

    $ make
    $ ./attaavv_ui

A text file called tempcat.txt should be made showing the data it 
read from the ascii data in the data folder.

----------------------------------------
Converting output to 2D:
----------------------------------------
In case you want to use your table as a two dimensional array, 
you can simply create a pointer to a pointer array on top of 
the output. As an example, in the ./examples/src/ui.c file 
add these lines of code:

In the declarations:

    int i;
    double **TwoDarray;

After calling readasciitable

    TwoDarray=malloc(intable.s0 * sizeof(double *));
    for (i = 0; i < intable.s0; i++)
        TwoDarray[i] = &intable.d[i*intable.s1];

To test it:

    printf("%f\n", TwoDarray[0][2]);

Free the space in the end

    free(TwoDarray);

----------------------------------------
Formatting input parameters to writeasciitable
----------------------------------------
writeasciitable receives four input arrays, besides the table 
structure and the output name, they are: "int_cols", "accu_cols", 
"space", "prec". writeasciitable assumes three kinds of numbers
for printing. The whole table data is saved as double.

1. Integers (%.0f in printf).
2. Scientfic (%lg in printf).
3. Float (%f) for very accurate numbers.

The default number format for writeasciitable is the second. If the 
columns are of the first or third types, you have to specify with
the input arrays explained here. The printf formattings of the
default representation is: %-[minspace].[numsigchar]g (scientific 
notation). [minspace] and [numsigchar] are set by the space[1], 
prec[0] inputs.

<dl>
<dt>int_cols:</dt><dd>This array tells the function which columns are 
            integers. The last element has to be negative in order 
            to signal the end of this array. The minimum space 
            parameter, used in printf, for the specified columns is 
            %-[minspace].0f. [minspace] is set by space[0].</dd>
<dt>accu_cols:</dt><dd>In some cases, we are dealing with a float that 
            requires very high accuracy compared to the other inputs 
            (last column in the example data). In such cases you can 
            specify which columns have this condition in this array. 
            Like int_cols, the last element has to be -1. The 
            formatting for these columns will be assumed to be like 
            this: %-[minspace].[numdecimals]f, [minspace]
            is determined by space[2] and [numdecimals] by prec[1].</dd>
<dt>space:</dt><dd>This array shows how much space should be allocated to 
            each kind of column as explained above.</dd>
<dt>prec:</dt><dd>This array shows the level of precision (significant 
            numbers for the general type and number of decimal points
            for the extra precision columns) as explained above.</dd>
</dl>

----------------------------------------
Comments and suggestions:
----------------------------------------
If you have any suggestions for improvements or find any problems 
in the program please contact me at 

makhlaghi@gmail.com 

akhlaghi@astr.tohoku.ac.jp

----------------------------------------
Mohammad Akhlaghi

http://astr.tohoku.ac.jp/~akhlaghi/

13 July 2013
