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
It is written fully in C89. The input table must not contain 
any non-numeric elements in this version of attaavv. This is 
a very simple program, only meant to do the most basic input 
and output of tabular numerical data in a nicely formated way. 

attaavv is not a standalone program, it is made to be placed 
inside a C program in order to read an ascii table and save the
result as a 1D C array (for a 2D array look bellow). Refer to
the example file for a simple example.

The program has lots of comments to explain every step.

----------------------------------------
Output structure
----------------------------------------
The function fills in the ArrayInfo structure definition, declared
in attaavv.h. The components include (in order): 

1. Number of header (comment) lines.
2. Array of pointers to comments.
3. Number of rows (zeroth axis) in the data table.
4. Number of columns (first axis) in the data table.
5. Data array (1D).

The 5th element of the output structure (the data in the table) 
is a 1D array containing all the data which you can use
by indexing the output with [i*n2+j] where n2 is the second
axis size and i and j are the first and second axis indexes 
respectively. To convert to a 2D array read bellow. 


----------------------------------------
Running the example:
----------------------------------------
In order to test run the example, put the two .h and .c files in the
src directory and run ($ is the shell prompt on the command line):

    $ make
    $ ./ui

A text file called tempcat.txt should be made showing the data it 
read from the ascii data in the data folder.

----------------------------------------
Converting output to 2D:
----------------------------------------
In case you want your output to be two dimensional, you can simply
create a pointer to a pointer array on top of the output:

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
writeasciitable receives four input arrays, besides the table structure
and the output name, they are: <b>int_cols</b>, <b>accu_cols</b>, 
<b>space</b>, <b>prec</b>. writeasciitable assumes three kinds of numbers:

1. Integers (%d in printf).
2. Scientfic (%g in printf).
3. Float (%f) for very accurate numbers.

The default number format for writeasciitable is the second. If the 
columns are of the first or third types, you have to specify with
the input arrays explained here. The printf formattings of the
default representation is: %-[minspace].[numsigchar]g (scientific 
notation). [minspace] and [numsigchar] are set by the space[1], 
prec[0] inputs.

<ol>
<li><b>int_cols:</b>This array tells the function which columns are 
            integers. The last element has to be negative in order 
            to signal the end of this array. The minimum space 
            parameter, used in printf, for the specified columns is 
            %-[minspace]d. [minspace] is set by space[0].</li>
<li><b>accu_cols:</b>In some cases, we are dealing with a float that 
            requires very high accuracy compared to the other inputs 
            (last column in the example data). In such cases you can 
            specify which columns have this condition in this array. 
            Like int_cols, the last element has to be -1. The 
            formatting for these columns will be assumed to be like 
            this: %-[minspace].[numdecimals]f, [minspace]
            is determined by space[2] and [numdecimals] by prec[1].</li>
<li><b>space:</b>This array shows how much space should be allocated to 
            each kind of column as explained above.</li>
<li><b>prec:</b>This array shows the level of precision (significant 
            numbers for the general type and number of decimal points
            for the extra precision columns) as explained above.</li>
</ol>

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

12 July 2013
