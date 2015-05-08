Welcome to Pucpeople
===================


Pucpeople is a program created for the project of the course of Algorithms in the PUCP university. It is inspired in the glib library and it follows the K&R coding style with 2 spaces.

> We've created a library called tlib which implements the basic data
> types we need to program the rest of the program. Every function that
> belongs to the tlib library has the prefix "t_", while every data type
> has the prefix "T".

Compiling
-------------
Since Pucpeople uses the library getopt.h, **you will only be able to run this program on Linux**. In order to build this program, you need to download this program and type this line

    make

You don't need to follow these steps at least you want to understand the basis of the source code of tlib. We've created a simple example about how to use it. This example is used for sorting a simple array of numbers. However, using TLib we can sort an array of whichever data type. If you wish to compile the example, just follow these steps:

    cd tlib/samples
    make

And if you want to execute the simple example

    ./example1

How to use Pucpeople?
-------------
You can find the instructions of this program by executing

    ./main --h

###Sorting by year, first name and father last name using merge sort

    ./main -f /path/to/file.txt --priority year first-name father-last-name --merge-sort

###Sorting by city, year and mother last name using insertion sort in descendant order

    ./main -f /path/to/file.txt --priority city year mother-last-name --desc --insertion-sort

###Filter by year (2015), city (La Perla) and father last name (Gutierrez)

    ./main -f /path/to/file.txt --filter --year 2015 --city "La Perla" --father-last-name "Gutierrez"

