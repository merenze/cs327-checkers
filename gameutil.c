/*
 * gameutil.c
 *
 * Matthew Renze
 */

#include <stdio.h>
#include "gameutil.h"
#include "inpututil.h"
#include "stringutil.h"

#define LENGTH 100	// Maximum allowed length of a token.

// Some global variables.
int valid = 1;		// True if standard input is valid.
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
void get_rules(char[]);
void get_board(char[]);
void print_board();

// The main dish.
void load_config() {
	// Some variables and stuff
	char token[LENGTH];
	next_token(token, LENGTH);
	multiple_jumps = 0
		;
	// Load rules
	if (!sequals(token, "RULES")) {
		printf("Invalid input: Expected 'RULES' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	next_token(token, LENGTH);
	get_rules(token);	
	if (!valid) {
		return;
	}

	// Load turn
	if (!sequals(token, "TURN")) {
		printf("Invalid input: Expected 'TURN' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	next_token(token, LENGTH);
	if (sequals(token, "red")) {
		black_turn = 0;
	} else if (sequals(token, "black")) {
		black_turn = 1;
	} else {
		printf("Invalid token: expected 'red' or 'black' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	if (!valid) {
		return;
	}

	// Load board
	next_token(token, LENGTH);
	if (!sequals(token, "BOARD")) {
		printf("Invalid input: Expected 'BOARD' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	get_board(token);
	if (!board_valid()) {
		printf("Invalid input: board invalid. Make sure there are at least 64 of any combination of 'r', 'R', 'b', 'B', '\"', '.'.\n");
		valid = 0;
		return;
	}

	// Move cursor to "MOVES"
	while (!sequals(token, "MOVES") && token[0] != 0) {
		next_token(token, LENGTH);
	}
	if (!sequals(token, "MOVES")) {
		printf("Invalid input: expected 'MOVES' but found diddly.\n");
	}
}

void get_rules(char token[]) {
	while (!sequals(token, "TURN")) {
		// Check for capture.
		next_token(token, LENGTH);
		if (sequals(token, "no")) {
//			printf("Token 'no' found. Checking for 'capture'.\n"); // DEBUG
			next_token(token, LENGTH);
			if (sequals(token, "capture")) {
//				printf("Token 'capture' found. Setting no_capture to 1.\n"); // DEBUG
				no_capture = 1;
			} else {
				valid = 0;
				printf(ERROR_RULES);
				return;
			}
		} else if (sequals(token, "capture")) {
//			printf("Token 'capture' found. Setting no_capture to 0.\n"); // DEBUG
			no_capture = 0;
		} else if (sequals(token, "multiple")) {
//			printf("Token 'multiple' found. Checking for 'jumps'.\n"); // DEBUG
			next_token(token, LENGTH);
			if (sequals(token, "jumps")) {
//				printf("Token 'jumps' found. Setting multiple_jumps to 1.\n"); // DEBUG
				multiple_jumps = 1;
				next_token(token, LENGTH);
			} else {
//			printf("Token 'jumps' not found after 'multiple'.\n"); // DEBUG
				printf(ERROR_RULES);
				valid = 0;
				return;
			}
		} else {
			valid = 0;
			printf(ERROR_RULES);
			break;
		}
	}
}

void get_board(char token[]) {
	next_token(token, LENGTH);
	int row = 0;
	int col = 0;
	while (row < 8 && token[0] != 0) {
		/*
		if (sequals(token, "r") || sequals(token, "R") || sequals(token, "b") || sequals(token, "B") || sequals(token, ".") || sequals(token, "\"")) {
			board[row][col++] = token[0];
//			printf("Added %s to board[%d][%d].\n", token, row, col - 1); // DEBUG
			if (col == 8) {
				col = 0;
				row++;
			}
		}
		next_token(token, LENGTH);
	*/
		for (int i = 0; token[i] != 0; i++) {
			if(token[i] == 'r' || token[i] == 'R' || token[i] == 'b' || token[i] == 'B' || token[i] == '.' || token[i] == '\"') {
				board[row][col++] = token[i];
				if (col == 8) {
					col = 0;
					row++;
				}
			}
		}
		next_token(token, LENGTH);
	}
//	print_board(); // DEBUGG
}

int get_moves(char token[]) {
	next_token(token, 6);
	// TODO
	return 1;
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
				return 0;
			}
		}
	}
	return 1;
}

// Ensure a move is valid
int move_valid(char move[]) {
	return
		move[0] >= 'a' && move[0] <= 'g' &&
		move[1] >= '1' && move[1] <= '8' &&
		move[2] == '-' && move[3] == '>' &&
		move[4] >= 'a' && move[4] <= 'g' &&
		move[5] >= '1' && move[5] <= '8'
		;
}

// For debugging
void print_board() {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			printf("%c ", board[row][col]);
		}
		printf("\n");
	}
}
