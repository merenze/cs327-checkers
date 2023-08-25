# Checkers
This was a four-part semester-long class project for COM S 327 Fall 2019 semester at Iowa State University.
The purpose was to provide a platform for learning C.
Note that part 3 is ommitted from the final product.

This project is not under active development.
It was fully functional at time of submission, but there is no guarentee everything still works as expected.

This documentation has been since updated to be more readable, but has not been checked for correctness.
I wrote it as a college sophomore, so take it with a grain of salt.
Most likely, the whole project needs to be redesigned from scratch.

## Part 1
Contains most of the logic for initializing the game from the specifications given via standard input.

Standard input is first accessed by inpututil.c, and is broken into tokens divided by whitespace and colons.
Tokens are given to gameutil.c, which parses them for game rules, turn, and board configuration.

The main function accesses this information and prints the output.

TODO The format for input is not yet described by the documentation.


## Part 2
Introduces change.c. 
When given a valid configuration, it
1. Initializes the gameboard, 
2. Performs the given moves until an illegal move is found, 
3. Stores the final board configuration in a human readable format, and stores the unprocessed moves.

Run with:
`change.c [options] [inputfile]`

TODO ^I haven't checked to be sure, but this does NOT look like an executable.

Options:
`-e efilename`: File where unprocessed moves are stored
`-h hfilename`: File where final board configuration is stored.
`-m max`: Maximum number of moves that should be processed.

## Part 4
Introduces edit.cpp.
With a valid configuration, the gameboard is initializedand the terminal is divided into a move view and a board view.
Scrolling through moves allows the user to see the board after that move is executed.

Run with:
`edit <input file>`
