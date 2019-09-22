/*
 * gameutil.h
 *
 * Contains essential variables and functions for manipulating the game of checkers.
 *
 * Matthew Renze
 */

char[8][8] board;	// Stores current layout of the board.

void load_config();	// Loads rules, turn, board, and moves from standard input.

int valid();		// True if standard input is valid.

int black_turn();	// True if currently black's turn.

int no_capture();	// True if capture rule not enforced.

int multiple_jumps();	// True if multiple jumps allowed.
