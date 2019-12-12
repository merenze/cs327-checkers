/*
 * info.c
 *
 * Part 1 of semester project.
 *
 * Matthew Renze
 */

#include <stdio.h>
#include "gameutil.h"

int main() {
	if (!load_config(stdin)) {
		return 1;
	}
	char** board = get_board();
	printf(
			"VALID INPUT\nInitial configuration:\nTurn: %s\nRed: %d kings, %d pawns\nBlack: %d kings, %d pawns\n", 
			(is_black_turn()) ? "black" : "red", 
			num_red_kings(board), 
			num_red_pawns(board), 
			num_black_kings(board), 
			num_black_pawns(board)
			);
	return 0;
}
