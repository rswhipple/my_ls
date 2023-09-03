# Welcome to My Ls
My recreation of the ls utility in C.
***

## Task
The task is to create a working version of the command line's ls function.

For each operand that names a file of a type other than directory, my_ls displays its name as well as any requested, associated information. 
For each operand that names a file of type directory, my_ls displays the names of files contained within that directory, as well as any requested, associated information.

If no operands are given, the contents of the current directory are displayed. 
If more than one operand is given, non-directory operands are displayed first; directory and non-directory operands are sorted separately and in lexicographical order.

The following options are available:

-a Include directory entries whose names begin with a dot (.).
-t Sort by time modified (most recently modified first) before sorting the operands by lexicographical order.

## Description
This my_ls program uses linked lists to store information about each file of type directory or other.
The program uses the opendir() and lstat() functions to retrieve the necessary information for each file.
The struct architecture of each list node includes variables to hold the name, path, and time of last modification (tv.sec AND tv.nsec). 

I implement 2 different bubble sort functions, sort_list() and sort_list_by_time(), to sort nodes based on either lexicographical or descending time.
I use a basic printf() with case dependendant formatting to output the results to stdout.

## Installation
A Makefile and a header.h file are included with the my_ls.c file containing the functions. 
Within the file directory via the command line, use the command "make my_ls" to compile the program.

## Usage
"./my_ls" + desired flags "-at" and the pathname will run the program.

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>

