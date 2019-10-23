/*
 * gameutil.c
 *
 * Matthew Renze
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gameutil.h"
#include "inpututil.h"

#define LENGTH 100	// Maximum allowed length of a token.

// Some global variables.
int valid = 1;		// True if standard input is valid.
int black_turn;		// True if currently black's turn, false if red's.
int multiple_jumps;	// True if multiple jumps are allowed.
int no_capture;		// True if capture rule not enforced.
int flipped;		// True if board is flipped configuration
int num_moves;		// Number of moves in movelist
Node* move_list;	// List of moves in order
// Some error messages.
const char ERROR_RULES[] = "Input error: Invalid token in rules. Allowed tokens are 'capture', 'no capture', and 'multiple jumps'.\n";

// Some helper methods.
void add_move(char*);
int count_occurrences(char);
int board_valid();
int get_rules(FILE*, char*);
int get_board(FILE*, char*);
int move_valid(char*);
int get_moves(FILE*, char*);
void print_board();

// The main dish.
int load_config(FILE* infile) {
	// Use this to check for EOF
	int has_input;
	// Some variables and stuff
	char token[LENGTH];
	has_input = next_token(infile, token, LENGTH);
	multiple_jumps = 0;

	if (!has_input) {
		printf("Invalid input: no input found.");
		return 0;
	}
	// Load rules
	if (strcmp(token, "RULES")) {
		printf("Invalid input: Expected 'RULES' but found '%s'.\n", token);
		return 0;
	}
	if (!next_token(infile, token, LENGTH)) {
		printf("Invalid input: Expected 'TURN' but found EOF.\n");
		return 0;
	}
	if (!get_rules(infile, token)) {
		return 0;
	}
	// Load turn
	if (strcmp(token, "TURN")) {
		printf("Invalid input: Expected 'TURN' but found '%s'.\n", token);
		return 0;
	}
	if (!next_token(infile, token, LENGTH)) {
		printf("Invalid input: Turn error.\n\tExpected 'red' or 'black' but found EOF.");
	}
	if (!strcmp(token, "red")) {
		black_turn = 0;
	} else if (!strcmp(token, "black")) {
		black_turn = 1;
	} else {
		printf("Invalid input: 'TURN' section.\n\tExpected 'red' or 'black' but found '%s'.\n", token);
		return 0;
	}

	// Load board
	if (!next_token(infile, token, LENGTH)) {
		printf("Invalid input: Expected 'BOARD' but found EOF.\n");
		return 0;
	}
	if (strcmp(token, "BOARD")) {
		printf("Invalid input: Expected 'BOARD' but found '%s'.\n", token);
		return 0;
	}
	if (!get_board(infile, token)) {
		return 0;
	}
	if (!board_valid()) {
		return 0;
	}

	// Move cursor to "MOVES"
	while (strcmp(token, "MOVES") && token[0]) {
		next_token(infile, token, LENGTH);
	}
	if (strcmp(token, "MOVES")) {
		printf("Invalid input: expected 'MOVES' but found diddly.\n");
		return 0;
	}
	if (!get_moves(infile, token)) {
		return 0;
	}

	return 1;
}

int get_rules(FILE* infile, char* token) {
	while (strcmp(token, "TURN")) {
		// Check for capture.
		if (!next_token(infile, token, LENGTH)) {
			printf("Invalid input: Rules error.\n\tExpected rule but found EOF.\n");
			return 0;
		}
		if (!strcmp(token, "no")) {
//			printf("Token 'no' found. Checking for 'capture'.\n"); // DEBUG
			if (!next_token(infile, token, LENGTH)) {
				printf("Invalid input: Rules error.\n\tExpected 'capture' but found EOF.\n");
				return 0;
			}
			if (!strcmp(token, "capture")) {
//				printf("Token 'capture' found. Setting no_capture to 1.\n"); // DEBUG
				no_capture = 1;
			} else {
				printf(ERROR_RULES);
				return 0;
			}
		} else if (!strcmp(token, "capture")) {
//			printf("Token 'capture' found. Setting no_capture to 0.\n"); // DEBUG
			no_capture = 0;
		} else if (!strcmp(token, "multiple")) {
//			printf("Token 'multiple' found. Checking for 'jumps'.\n"); // DEBUG
			if (!next_token(infile, token, LENGTH)) {
				printf("Invalid input: Rules error.\n\tExpected 'capture' but found EOF.\n");
				return 0;
			}
			if (!strcmp(token, "jumps")) {
//				printf("Token 'jumps' found. Setting multiple_jumps to 1.\n"); // DEBUG
				multiple_jumps = 1;
				if (!next_token(infile, token, LENGTH)) {
					printf("Invalid input: Rules error.\n\tExpected rule but found EOF.\n");
					return 0;
				}
			} else {
//			printf("Token 'jumps' not found after 'multiple'.\n"); // DEBUG
				printf(ERROR_RULES);
				return 0;
			}
		} else {
			printf(ERROR_RULES);
			return 0;
		}
	}
	return 1;
}

int get_board(FILE* infile, char* token) {
	if (!next_token(infile, token, LENGTH)) {
		printf("Invalid input: Board error. EOF reached.\n");
		return 0;
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
		if (!next_token(infile, token, LENGTH)) {
			printf("Invalid input: Board error. EOF reached.\n");
			return 0;
		}
	}
//	print_board(); // DEBUGG
	return 1;
}

int get_moves(FILE* infile, char* token) {
	for (num_moves = 0; next_token(infile, token, LENGTH); num_moves++) {
		if (!move_valid(token)) {
			printf("Invalid input: 'TOKENS' section.\n\tExpected a move but found '%s'.\n", token);
			return 0;
		}
		add_move(token);
	}
	return 1;
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
	flipped = board[0][0] != '\"';
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

/**
 * This method checks whether the input token for a move is of a valid format.
 * It does NOT check if the move is legal.
 */
int move_valid(char move[]) {
	// String is wrong length
	if (!multiple_jumps && strlen(move) != 6) {
		return 0;
	}
	if ((strlen(move) - 6) % 4 != 0) {
		return 0;
	}
	for (int i = 0; move[i]; i++) {
		// Check for letters in appropriate positions
		if (i % 4 == 0) {
			if (!('a' <= move[i] && move[i] <= 'z')) {
				return 0;
			}
		}
		// Check for numbers in appropriate positions
		else if ((i - 1) % 4 == 0) {
			if (!('1' <= move[i] && move[i] <= '8')) {
				return 0;
			 }
		}
		// Check for '-' (arrow)
		else if ((i - 2) % 4 == 0) {
			if (move[i] != '-') {
				return 0;
			}
		}
		// Check for '>' (arrow)
		else if ((i - 3) % 4 == 0) {
			if (move[i] != '>') {
				return 0;
			}
		}
	}
	return 1;
}

/**
 * Execute a move.
 *
 * Returns:	1 on success (move is legal). 0 on failure (move is illegal).
 */
int do_move(char* move) {
	for (int i = 0; i + 5 < strlen(move); i += 4) {
		// Convert rank and file into board row and column
		int row_c = 8 - move[i + 1];	// Current row
		int row_t = 8 - move[i + 5];	// Target row
		int col_c = move[i] - 'a';		// Current column
		int col_t = move[i + 4] - 'a';	// Target column
		// Check for a piece at the current position
		if (!(board[row_c][col_c] == 'r' || board[row_c][col_c] == 'R' || board[row_c][col_c] == 'b' || board[row_c][col_c] == 'B')) {
			return 0;
		}
		// Red pawns cannot move down. Black pawns cannot move up
		if (row_t > row_c && board[row_c][col_c] == 'r' || row_t < row_c && board[row_c][col_c] == 'b') {
			return 0;
		}	
		// Check for a free space at the target position
		if (!(board[row_t][col_t] == '.' || board[row_t][col_t] == '"')) {
			return 0;
		}
		// When attempting to jump down
		if (row_t == row_c + 2) {
			// When attempting to move right
			if (col_t == col_c + 2) {
				char victim = board[row_c + 1][col_c + 1];
				// If piece is red
				if (board[row_c][col_c] == 'R') {
					if (victim != 'b' && victim != 'B') {
						return 0;
					}
				}
				// If piece is black
				else {
					if (victim != 'r' && victim != 'R') {
						return 0;
					}
				}

			}
			// When attempting to move left
			else if (col_t == col_c - 2) {
				char victim = board[row_c + 1][col_c - 1];
				// If piece is red
				if (board[row_c][col_c] == 'R') {
					if (victim != 'b' && victim != 'B') {
						return 0;
					}
				}
				// If piece is black
				else {
					if (victim != 'r' && victim != 'R') {
						return 0;
					}
				}
			} else {
				return 0;
			}
		}
		// When attempting to jump up
		else if (row_t == row_c - 2) {
			// When attempting to move right
			if (col_t == col_c + 2) {
				char victim = board[row_c - 1][col_c + 1];
				// If piece is black
				if (board[row_c][col_c] == 'B') {
					if (victim != 'r' && victim != 'R') {
						return 0;
					}
				}
				// If piece is red
				else {
					if (victim != 'b' && victim != 'B') {
						return 0;
					}
				}
			}
			// When attempting to move left
			else if (col_t == col_c - 2) {
				char victim = board[row_c - 1][col_c - 1];
				// If piece is black
				if (board[row_c][col_c] == 'B') {
					if (victim != 'r' && victim != 'R') {
						return 0;
					}
				}
				// If piece is red
				else {
					if (victim != 'b' && victim != 'B') {
						return 0;
					}
				}
			} else {
				return 0;
			}
		}
		// Not a jump. Check for diagonal relationship
		else if (!(	row_t == row_c + 1 && (col_t == col_c + 1 || col_t == col_c - 1) || 
				row_t == row_c - 1 && (col_t == col_c + 1 || col_t == col_c - 1) )) {
			return 0;
		}
		// Execute the move
		board[row_t][col_t] = board[row_c][col_c];
		board[row_c][col_c] = flipped ? '.' : '"';
		return 1;
	}
}

void add_move(char* move) {
	if (!move_list) {
		move_list = (Node*) malloc(sizeof(Node));
		strcpy(move_list->move, move);
		move_list->next = NULL;
		return;
	}
	Node* cursor;
	for (cursor = move_list; cursor->next; cursor = cursor->next);
	cursor->next = (Node*) malloc(sizeof(Node));
	strcpy(cursor->next->move, move);
	cursor->next->next = NULL;
}

// Returns the head of the movelist
Node* get_move_list() {
	return move_list;
}

// Returns the number of moves from config
int get_num_moves() {
	return num_moves;
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
