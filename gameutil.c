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
#include "movelist.h"

#define LENGTH 100	// Maximum allowed length of a token.

// Some global variables.
int valid = 1;		// True if standard input is valid.
int black_turn;		// True if currently black's turn, false if red's.
int multiple_jumps;	// True if multiple jumps are allowed.
int no_capture;		// True if capture rule not enforced.
int flipped;		// True if board is flipped configuration
int num_moves;		// Number of moves in movelist
Node* movelist;	// List of moves in order
FILE* logfile;		// Useful for debugging

const char ERROR_RULES[] = "Input error (line %d): Invalid token ('%s') in rules.\n";

// Some helper methods.
char rtoc(int);
int is_empty(char**, int, int);
int is_red(char**, int, int);
int is_black(char**, int, int);
void add_move(char*);
int count_occurrences(char);
int board_valid();
int get_rules(FILE*, char*);
int get_board(FILE*, char*);
int move_valid(char*);
int get_moves(FILE*, char*);
void print_board(FILE*, char**);

// The main dish.
int load_config(FILE* infile) {
	logfile = fopen("log", "w");
	// Use this to check for EOF
	int has_input;
	// Some variables and stuff
	char token[LENGTH];
	has_input = next_token(infile, token, LENGTH);
	multiple_jumps = 0;

	if (!has_input) {
		fprintf(stderr, "Invalid input (line %d): No input found.\n", get_line());
		return 0;
	}
	// Load rules
	if (strcmp(token, "RULES")) {
		fprintf(stderr, "Invalid input (line %d): Expected 'RULES' but found '%s'.\n", get_line(), token);
		return 0;
	}
	if (!next_token(infile, token, LENGTH)) {
		fprintf(stderr, "Invalid input (line %d): Expected 'TURN' but found EOF.\n", get_line());
		return 0;
	}
	if (!get_rules(infile, token)) {
		return 0;
	}
	// Load turn
	if (strcmp(token, "TURN")) {
		fprintf(stderr, "Invalid input (line %d): Expected 'TURN' but found '%s'.\n", get_line(), token);
		return 0;
	}
	if (!next_token(infile, token, LENGTH)) {
		fprintf(stderr, "Invalid input (line %d): Expected 'red' or 'black' but found EOF.\n", get_line());
	}
	if (!strcmp(token, "red")) {
		black_turn = 0;
	} else if (!strcmp(token, "black")) {
		black_turn = 1;
	} else {
		fprintf(stderr, "Invalid input (line %d): Expected 'red' or 'black' but found '%s'.\n", get_line(), token);
		return 0;
	}

	// Load board
	if (!next_token(infile, token, LENGTH)) {
		fprintf(stderr, "Invalid input (line %d): Expected 'BOARD' but found EOF.\n", get_line());
		return 0;
	}
	if (strcmp(token, "BOARD")) {
		fprintf(stderr, "Invalid input (line %d): Expected 'BOARD' but found '%s'.\n", get_line(), token);
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
		fprintf(stderr, "Invalid input (line %d): expected 'MOVES' but found '%s'\n", get_line(), token);
		return 0;
	}
	if (!get_moves(infile, token)) {
		return 0;
	}
	fclose(logfile);
	return 1;
}

int get_rules(FILE* infile, char* token) {
	fprintf(logfile, "Call to get_rules()\n");
	while (strcmp(token, "TURN")) {
		// Check for capture.
		if (!next_token(infile, token, LENGTH)) {
			fprintf(stderr, "Invalid input (line %d): Expected rule but found EOF.\n", get_line());
			return 0;
		}
		if (!strcmp(token, "no")) {
			fprintf(logfile, "Token 'no' found. Checking for 'capture'.\n");
			if (!next_token(infile, token, LENGTH)) {
				fprintf(stderr, "Invalid input (line %d): Expected 'capture' but found EOF.\n", get_line());
				return 0;
			}
			if (!strcmp(token, "capture")) {
				fprintf(logfile, "Token 'capture' found. Setting no_capture to 1.\n"); // DEBUG
				no_capture = 1;
			} else {
				fprintf(stderr, ERROR_RULES, get_line(), token);
				return 0;
			}
		} else if (!strcmp(token, "capture")) {
			fprintf(logfile, "Token 'capture' found. Setting no_capture to 0.\n"); // DEBUG
			no_capture = 0;
		} else if (!strcmp(token, "multiple")) {
			fprintf(logfile, "Token 'multiple' found. Checking for 'jumps'.\n"); // DEBUG
			if (!next_token(infile, token, LENGTH)) {
				fprintf(stderr, "Invalid input (line %d): Expected 'capture' but found EOF.\n", get_line());
				return 0;
			}
			if (!strcmp(token, "jumps")) {
				fprintf(logfile, "Token 'jumps' found. Setting multiple_jumps to 1.\n"); // DEBUG
				multiple_jumps = 1;
				if (!next_token(infile, token, LENGTH)) {
					fprintf(stderr, "Invalid input (line %d): Expected rule but found EOF.\n", get_line());
					return 0;
				}
			} else {
			fprintf(logfile, "Token 'jumps' not found after 'multiple'.\n"); // DEBUG
				fprintf(stderr, ERROR_RULES, get_line(), token);
				return 0;
			}
		} else if (!strcmp(token, "single")) {
			fprintf(logfile, "Token 'single' found. Checking for 'jumps'.\n");
			if (!next_token(infile, token, LENGTH)) {
				fprintf(stderr, "Invalid input (line %d): Expected 'jumps' but found EOF.\n", get_line());
			}
			if (!strcmp(token, "jumps")) {
				fprintf(logfile, "Token 'jumps' found. Setting multiple_jumps to 0.");
				if (!next_token(infile, token, LENGTH)) {
					fprintf(stderr, "Invalid input (line %d): Expected rule but found EOF.\n", get_line());
					return 0;
				}
			}
		} else {
			fprintf(stderr, ERROR_RULES, get_line(), token);
			return 0;
		}
	}
	return 1;
}

int get_board(FILE* infile, char* token) {
	if (!next_token(infile, token, LENGTH)) {
		fprintf(stderr, "Invalid input (line %d): EOF reached.\n", get_line());
		return 0;
	}
	int row = 0;
	int col = 0;
	while (row < 8 && token[0] != 0) {
		for (int i = 0; token[i] != 0 && row < 8; i++) {
			if(token[i] == 'r' || token[i] == 'R' || token[i] == 'b' || token[i] == 'B' || token[i] == '.' || token[i] == '\"') {
				board[row][col++] = token[i];
				fprintf(logfile, "Added '%c' to board[%d][%d].\n", token[i], row, col - 1); // DEBUG
				if (col == 8) {
					col = 0;
					row++;
				}
			}
		}
		if (!next_token(infile, token, LENGTH)) {
			fprintf(stderr, "Invalid input (line %d): EOF reached.\n", get_line());
			return 0;
		}
	}
	print_board(logfile); // DEBUGG
	return 1;
}

int get_moves(FILE* infile, char* token) {
	for (num_moves = 0; next_token(infile, token, LENGTH); num_moves++) {
		if (!move_valid(token)) {
			fprintf(stderr, "Invalid input (line %d): Expected move but found '%s'.\n", get_line(), token);
			return 0;
		}
		movelist_add(movelist, token);
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
				fprintf(stderr, "Invalid input (line %d): Invalid board. Space %c%d uninitialized\n", 
						get_line(), j + 97, 8 - i);
				return 0;
			}
			// Make sure red spaces are red, and black spaces are black.
			// If board is not flipped, space is supposed to be red, and space is not red
			if (!flipped && redFlag && board[i][j] != '\"') {
				fprintf(stderr, "Invalid input (line %d): Invalid board.\n\tIn space %c%d: Expected %s space but found %s space.\n", 
						get_line(), j + 97, 8 - i, (redFlag ? "red" : "black"), (redFlag ? "black" : "red"));
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
	fprintf(logfile, "\nCall to do_move(%s)\n", move);
	for (int i = 0; i + 5 < strlen(move); i += 4) {
		// Convert rank and file into board row and column
		int row_c = 7 - move[i + 1] + '1';	// Current row
		int row_t = 7 - move[i + 5] + '1';	// Target row
		int col_c = move[i] - 'a';		// Current column
		int col_t = move[i + 4] - 'a';		// Target column
		
		fprintf(logfile, "\nCurrent row: %c%c [%d][%d]\nTarget row: %c%c [%d][%d]\nInitial board:\n",
				move[i], move[i+1], row_c, col_c, move[i+4], move[i+5], row_t, col_t);
		print_board(logfile);
		// Check for a piece at the current position
		if (!(board[row_c][col_c] == 'r' || board[row_c][col_c] == 'R' || board[row_c][col_c] == 'b' || board[row_c][col_c] == 'B')) {
			fprintf(logfile, "Illegal move: No piece at start space]\n");	// DEBUG
			return 0;
		}
		// Red pawns cannot move down. Black pawns cannot move up
		if (row_t > row_c && board[row_c][col_c] == 'r' || row_t < row_c && board[row_c][col_c] == 'b') {
			fprintf(logfile, "Illegal move: Pawn attempting to move backwards\n");
			return 0;
		}	
		// Check for a free space at the target position
		if (!(board[row_t][col_t] == '.' || board[row_t][col_t] == '"')) {
			fprintf(logfile, "Illegal move: Pawn attempting to move to occupied space\n");	// DEBUG
			return 0;
		}
		// When attempting to jump down
		if (row_t == row_c + 2) {
			fprintf(logfile, "Move is a jump down\n");
			// When attempting to move right
			if (col_t == col_c + 2) {
				char victim = board[row_c + 1][col_c + 1];
				fprintf(logfile, "Attempting to jump space '%c' [%d][%d]\n", victim, row_c + 1, col_c + 1);
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
				board[row_c + 1][col_c + 1] = flipped ? '"' : '.';
			}
			// When attempting to move left
			else if (col_t == col_c - 2) {
				char victim = board[row_c + 1][col_c - 1];
				fprintf(logfile, "Attempting to jump space '%c' [%d][%d]\n", victim, row_c + 1, col_c - 1);
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
				board[row_c + 1][col_c - 1] = flipped ? '"' : '.';
			} else {
				return 0;
			}
		}
		// When attempting to jump up
		else if (row_t == row_c - 2) {
			fprintf(logfile, "Move is a jump up\n");
			// When attempting to move right
			if (col_t == col_c + 2) {
				char victim = board[row_c - 1][col_c + 1];
				fprintf(logfile, "Attempting to jump space '%c' [%d][%d]\n", victim, row_c - 1, col_c + 1);
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
				board[row_c - 1][col_c + 1] = flipped ? '"' : '.';
			}
			// When attempting to move left
			else if (col_t == col_c - 2) {
				char victim = board[row_c - 1][col_c - 1];
				fprintf(logfile, "Attempting to jump space '%c' [%d][%d]\n", victim, row_c - 1, col_c - 1);
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
				board[row_c - 1][col_c - 1] = flipped ? '"' : '.';
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
		board[row_c][col_c] = flipped ? '"' : '.';
		fprintf(logfile, "Move successful.\nNew board:\n");
		print_board(logfile);
		return 1;
	}
}

Node* get_movelist() {
	return movelist;
}

Node* get_possible_moves(char** G, int do_black) {
	fprintf(logfile, "get_possible_moves called for %s\nConfiguration:\n", do_black ? "black" : "red");
	print_board(logfile, G);
	Node* result = NULL;
	
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			fprintf(logfile, "Checking space %c%c... ", ctoc(row), rtoc(col));
			if (do_black && !is_black(board, row, col) || !do_black && !is_red(board, row, col)) {
				fprintf(logfile, "No %s piece; continue.\n", do_black ? "black" : "red");
				continue;
			}
			for (int j = col - 2; j <= col + 2; j++) {
				// Bounds check on col
				if (!(0 <= j && j < 8))
					continue;
				for (int i = row + 2; i >= row - 2; row--) {
					// Bounds check on row
					if (!(0 <= i && i < 8))
						continue;
					// Offset check
					if (row - i != col - j)
						continue;
					fprintf(logfile, "Checking for move to %c%c... ", ctoc(j), ctoc(i));
					// Getting black moves
					if (do_black) {
						if (is_empty(G, i, j)) {
							


				}
			}

	return result;
}


char rtoc(int r) {
	return (r >= 0 && r < 8) ? ('0' + (8 - r)) : '#';	
}

char ctoc(int c) {
	return (c >= 0 && c < 8) ? ('a' + c) : '#';
}

int is_empty(char** G, int row, int col) {
	return G[row][col] == '.' || G[row][col] == '"';
}

int is_red(char** G, int row, int col) {
	return G[row][col] == 'r' || G[row][col] == 'R';
}

int is_black(char** G, int row, int col) {
	return G[row][col] == 'b' || G[row][col] == 'B';
}

// Returns the number of moves from config
int get_num_moves() {
	return num_moves;
}
// For debugging
void print_board(FILE* file, char** G) {
	fprintf(file, "  a b c d e f g h \n");
	for (int row = 0; row < 8; row++) {
		fprintf(file, "%d ", 8 - row);
		/for (int col = 0; col < 8; col++) {
			fprintf(file, "%c ", G[row][col]);
		}
		fprintf(file, "\n");
	}
}
