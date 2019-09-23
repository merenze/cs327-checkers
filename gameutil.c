/*
 * gameutil.c
 *
 * Matthew Renze
 */

#include <stdio.h>
#include "gameutil.h"
#include "inpututil.h"

#define LENGTH 100	// Maximum allowed length of a token.

// Some global variables.
int valid = 1;		// True if standard input is valid.
int loaded = 0;		// Prevents trying to load this function more than once.
int black_turn;		// True if currently black's turn, false if red's.
int multiple_jumps;	// True if multiple jumps are allowed.
int no_capture;		// True if capture rule not enforced.

// Some error messages.
const char ERROR_RULES[] = "Input error: Invalid token in rules. Allowed tokens are \'capture\', \'no capture\', and \'multiple jumps\'.\n";
const char ERROR_TURN[]  = "Input error: Invalid token in turn. Allowed tokens are \'red\' and \'black\'.\n";
const char ERROR_BOARD[] = "Input error: Failed to load board. Are there at least 64 valid spaces listed?\n";

// Some helper methods.
int count_occurrences(char);
int board_valid();

// The main dish.
void load_config() {
	// Don't allow function to run more than once
	if (loaded) {
		return;
	}
	loaded = 1;
	
	// Some variables and stuff
	char token[LENGTH];
	next_token(token, LENGTH);
	multiple_jumps = 0;

	// Everything up til "BOARD"
	while (!equals(token, "") && !equals(token, "BOARD")) {
		if (equals(token, "RULES")) {
			// Check for capture.
			next_token(token, LENGTH);
			if (equals(token, "no")) {
				next_token(token, LENGTH);
				if (equals(token, "capture")) {
					no_capture = 1;
					next_token(token, LENGTH);
				} else {
					valid = 0;
					printf(ERROR_RULES);
					break;
				}
			} else if (equals(token, "capture")) {
				no_capture = 0;
			} else if (equals(token, "multiple")) {
				next_token(token, LENGTH);
				if (equals(token, "jumps")) {
					multiple_jumps = 1;
				}
			} else {
				valid = 0;
				printf(ERROR_RULES);
				break;
			}
		} else if (equals(token, "TURN")) {
			next_token(token, LENGTH);
			if (equals(token, "red")) {
				black_turn = 0;
			} else if (equals(token, "black")) {
				black_turn = 1;
			} else {
				valid = 0;
				printf(ERROR_TURN);
				break;
			}
		}
	}
	if (!equals(token, "BOARD")) {
		printf("Invalid input: expected 'BOARD' but found '%s'.\n", token);
		valid = 0;
	}
	if (!valid) {
		return;
	}
	next_token(token, LENGTH);

	// Time to load the board!
	int row = 0;
	int col = 0;
	printf("LOADING BOARD...\n"); // DEBUG
	while (!equals(token, "MOVES") && !equals(token, "") && row < 8) {
		printf("\nToken: %s\n", token); // DEBUG
		if (equals(token, "r") || equals(token, "R") || equals(token, "b") || equals(token, "B") || equals(token, "\"") || equals(token, ".")) {
			printf("Valid. Adding to board[%d][%d]\n", row, col); // DEBUG
			board[row][col++] = token[0];
			if (col == 8) {
				printf("Invalid.\n"); // DEBUG
				col = 0;
				row++;
			}
		}
		next_token(token, LENGTH);
	}
	valid = board_valid();
	if (!valid) {
		return;
	}
	// Move cursor to "MOVES".
	while (!equals(token, "MOVES")) {
		next_token(token, LENGTH);
	}
	// TODO: Do moves
}

int is_valid() {
	return valid;
}

int is_black_turn() {
	return black_turn;
}

int is_no_capture() {
	return no_capture;
}

int is_multiple_jumps() {
	return multiple_jumps;
}

int num_black_pawns() {
	return count_occurrences('b');
}

int num_black_kings() {
	return count_occurrences('B');
}

int num_red_pawns() {
	return count_occurrences('r');
}

int num_red_kings() {
	return count_occurrences('R');
}

// Count the number of occurrences of a char in board.
int count_occurrences(char ch) {
	int result = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == ch) {
				result++;
			}
		}
	}
	return result;
}

// Ensure the board is valid
int board_valid() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == 0) {
				printf(ERROR_BOARD);
				return 0;
			}
		}
	}
	return 1;
}
