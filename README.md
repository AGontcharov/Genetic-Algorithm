/*
 * ------------------------------------------------------------------
 *  README.md
 *
 *  CIS*6420 - Soft Computing - W17
 *  School of Computer Science
 *  University of Guelph
 *  Author: Alexander Gontcharov
 * ------------------------------------------------------------------
 */

A makefile has been provided with the source file to aid with the build and running.

-----------------------
How to Build and Clean
-----------------------
To build type in the following: 
- make

To clean the build type in the following: 
- make clean

-----------------------
How to Run the Program:
-----------------------
To run the program type in the following on the command line: 
- make run

To run the program on its own the following arguments must be supplied:
- ./runMe [Max generation number] [Population size] [bit string length]

By default:
- Maximun generation number is 1000
- Size of the population is 50
- Bit string length is 16 bits

These values can be adjusted by changing the following values for the macros in the makefile:
- GENNUM (Maximum generation number)
- POPNUM (Size of the population)