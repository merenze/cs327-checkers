/*
 * gameutil.c
 *
 * Matthew Renze
 */

#include <stdio.h>
#include <string.h>
#include "gameutil.h"
#include "inpututil.h"

#define LENGTH 100	// Maximum allowed length of a token.

// Some global variables.
int valid = 1;		// True if standard input is valid.
int black_turn;		// True if currently black's turn, false if red's.
int multiple_jumps;	// True if multiple jumps are allowed.
int no_capture;		// True if capture rule not enforced.

// Some error messages.
const char ERROR_RULES[] = "Input error: Invalid token in rules. Allowed tokens are \'capture\', \'no capture\', and \'multiple jumps\'.\n";

// Some helper methods.
int count_occurrences(char);
int board_valid();
void get_rules(char[]);
void get_board(char[]);
int move_valid(char[]);
int get_moves(char[]);
void print_board();

// The main dish.
void load_config() {
	// Use this to check for EOF
	int has_input;
	// Some variables and stuff
	char token[LENGTH];
	has_input = next_token(token, LENGTH);
	multiple_jumps = 0;

	if (!has_input) {
		printf("Invalid input: no input found.");
		valid = 0;
		return;
	}
	// Load rules
	if (strcmp(token, "RULES")) {
		printf("Invalid input: Expected 'RULES' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	has_input = next_token(token, LENGTH);
	if (!has_input) {
		printf("Invalid input: Expected 'TURN' but found EOF.\n");
		valid = 0;
		return;
	}
	get_rules(token);
	if (!valid) {
		return;
	}

	// Load turn
	if (strcmp(token, "TURN")) {
		printf("Invalid input: Expected 'TURN' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	has_input = next_token(token, LENGTH);
	if (!has_input) {
		printf("Invalid input: Turn error.\n\tExpected 'red' or 'black' but found EOF.");
	}
	if (!strcmp(token, "red")) {
		black_turn = 0;
	} else if (!strcmp(token, "black")) {
		black_turn = 1;
	} else {
		printf("Invalid input: 'TURN' section.\n\tExpected 'red' or 'black' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	if (!valid) {
		return;
	}

	// Load board
	has_input = next_token(token, LENGTH);
	if (!has_input) {
		printf("Invalid input: Expected 'BOARD' but found EOF.\n");
		valid = 0;
		return;
	}
	if (strcmp(token, "BOARD")) {
		printf("Invalid input: Expected 'BOARD' but found '%s'.\n", token);
		valid = 0;
		return;
	}
	get_board(token);
	if (!board_valid()) {
		valid = 0;
		return;
	}

	// Move cursor to "MOVES"
	while (strcmp(token, "MOVES") && token[0] != 0) {
		next_token(token, LENGTH);
	}
	if (strcmp(token, "MOVES")) {
		printf("Invalid input: expected 'MOVES' but found diddly.\n");
		valid = 0;
		return;
	}
	if (!get_moves(token)) {
		valid = 0;
	}
}

void get_rules(char token[]) {
	while (strcmp(token, "TURN")) {
		// Check for capture.
		if (!next_token(token, LENGTH)) {
			printf("Invalid input: Rules error.\n\tExpected rule but found EOF.\n");
			valid = 0;
			return;
		}
		if (!strcmp(token, "no")) {
//			printf("Token 'no' found. Checking for 'capture'.\n"); // DEBUG
			if (!next_token(token, LENGTH)) {
				printf("Invalid input: Rules error.\n\tExpected 'capture' but found EOF.\n");
				valid = 0;
				return;
			}
			if (!strcmp(token, "capture")) {
//				printf("Token 'capture' found. Setting no_capture to 1.\n"); // DEBUG
				no_capture = 1;
			} else {
				valid = 0;
				printf(ERROR_RULES);
				return;
			}
		} else if (!strcmp(token, "capture")) {
//			printf("Token 'capture' found. Setting no_capture to 0.\n"); // DEBUG
			no_capture = 0;
		} else if (!strcmp(token, "multiple")) {
//			printf("Token 'multiple' found. Checking for 'jumps'.\n"); // DEBUG
			if (!next_token(token, LENGTH)) {
				printf("Invalid input: Rules error.\n\tExpected 'capture' but found EOF.\n");
			}
			if (!strcmp(token, "jumps")) {
//				printf("Token 'jumps' found. Setting multiple_jumps to 1.\n"); // DEBUG
				multiple_jumps = 1;
				if (!next_token(token, LENGTH)) {
					printf("Invalid input: Rules error.\n\tExpected rule but found EOF.\n");
					valid = 0;
					return;
				}
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
	if (!next_token(token, LENGTH)) {
		printf("Invalid input: Board error. EOF reached.\n");
		valid = 0;
		return;
	}
	int row = 0;
	int col = 0;
	while (row < 8 && token[0] != 0) {
		for (int i = 0; token[i] != 0 && row < 8; i++) {
			if(token[i] == 'r' || token[i] == 'R' || token[i] == 'b' || token[i] == 'B' || token[i] == '.' || token[i] == '\"') {
				board[row][col++] = token[i];
//				printf("Added '%c' to board[%d][%d].\n", token[i], row, col - 1); // DEBUG
				if (col == 8) {
					col = 0;
					row++;
				}
			}
		}
		if (!next_token(token, LENGTH)) {
			printf("Invalid input: Board error. EOF reached.\n");
			valid = 0;
			return;
		}
	}
//	print_board(); // DEBUGG
}

int get_moves(char token[]) {
	while (next_token(token, 7)) {
		if (!move_valid(token)) {
			printf("Invalid input: 'TOKENS' section.\n\tExpected a move but found '%s'.\n", token);
			return 0;
		}
	}
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
	int flipped = board[0][0] != '\"';
	int redFlag = flipped;
	for (int i = 0; i < 8; i++) {
		redFlag = !redFlag;
		for (int j = 0; j < 8; j++) {
			// Make sure the space has been initialized.
			if (board[i][j] == 0) {
				printf("Invalid input: Invalid board.\n\tIn space %c%d: Found uninitialized space. Are there at least 64 squares defined?\n", j + 97, 8 - i);
				return 0;
			}
			// Make sure red spaces are red, and black spaces are black.
			// If board is not flipped, space is supposed to be red, and space is not red
			if (!flipped && redFlag && board[i][j] != '\"') {
				printf("Invalid input: Invalid board.\n\tIn space %c%d: Expected %s space but found %s space.\n", j + 97, 8 - i, (redFlag ? "red" : "black"), (redFlag ? "black" : "red"));
				return 0;
			}
			// If board IS flipped, space is supposed to be red, and space is not red.
			if (flipped && redFlag && board[i][j] == '.') {
				return 0;
			}
			redFlag = !redFlag; // && j != 7; // Flag the next square as the opposite color, unless it starts on a new row.
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
