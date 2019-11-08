/*
 * gameutil.h
 *
 * Contains essential variables and functions for manipulating the game of checkers.
 *
 * Matthew Renze
 */
#include "movelist.h"

char board[8][8];		// Stores current layout of the board.

int load_config(FILE*);		// Loads rules, turn, board, and moves from standard input.

int is_black_turn();		// True if currently black's turn.

int is_no_capture();		// True if capture rule not enforced.

int is_multiple_jumps();	// True if multiple jumps allowed

int num_black_pawns();		// Counts the number of black pawns.

int num_black_kings();		// Counts the number of black kings.

int num_red_pawns();		// Counts the number of red pawns.

int num_red_kings();		// Counts the number of red kings.

int get_num_moves();		// Gets the number of moves in config file

int do_move(char*);		// Attempt to execute a move

Node* get_movelist();		// Gets the list of moves
