/*
 * gameutil.h
 *
 * Contains essential variables and functions for manipulating the game of checkers.
 *
 * Matthew Renze
 */
#ifndef GAMEUTIL_H
#define GAMEUTIL_H

#include "movelist.h"
/**
 * Stores the current layout of the board
 */
char board[8][8];		
/**
 * Load rules turn, board and moves from input
 */
int load_config(FILE*);			
/**
 * True if currently black's turn
 */
int is_black_turn();				
/**
 * True if capture rule not enforced
 */
int is_no_capture();
/**
 * True if multiple jumps allowed
 */
int is_multiple_jumps();
/**
 * Count the number of black pawns on the board
 */
int num_black_pawns(char G[8][8]);
/**
 * Count the number of black kings on the board
 */
int num_black_kings(char[8][8]);
/**
 * Count the number of red pawns on the board
 */
int num_red_pawns(char[8][8]);
/**
 * Count the number of red kings on the board
 */
int num_red_kings(char[8][8]);
/**
 * Gets the number of moves from the config
 */
int get_num_moves();	
/**
 * Attempt to execute a move
 */
int do_move(char[8][8], char*);	
/**
 * Get list of possible moves
 */
Node* get_possible_moves(char[8][8], int);	
/**
 * Get score for a given move
 */
int get_score_for_move(char[8][8], char*, int, int);
/**
 * Get list of moves
 */
Node* get_movelist();

#endif
