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
	load_config();
	if (!is_valid()) {
		return 1;
	}
	printf(
			"VALID INPUT\nInitial configuration:\nTurn: %s\nRed: %d kings, %d pawns\nBlack: %d kings, %d pawns\n", 
			(is_black_turn()) ? "black" : "red", 
			num_red_kings(), 
			num_red_pawns(), 
			num_black_kings(), 
			num_black_pawns()
			);
}
