/**
 * edit.cpp
 *
 * Part 4
 * Matthew Renze
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <termbox.h>

#include "gameutil.h"

using namespace std;

/**
 * Coordinates used for splitting the box into two screens.
 * Start is inclusive, end is exclusive.
 */
#define FILE_START_X 0
#define FILE_START_Y 4
#define BOARD_START_X 40
#define BOARD_START_Y 0
#define BOARD_END_X 65
#define BOARD_END_Y 24
#define VISUAL_CURSOR FILE_START_Y + cursor.line - first_line
/**
 * Text justification
 */
#define LEFT -1
#define RIGHT 1

/**
 * Scroll direction
 */
#define UP -1
#define DOWN 1

/**
 * Highlights a move in the file view
 */
struct Cursor {
	int line;
	int justify;
};

/**
 * Helper functions
 */
void tb_write(int, int, const char*, int);
void draw_board_background();
struct tb_cell* get_cell(int, int);
void map_moves();
int highlight_cell(int, int, int, int);
void highlight_move(int);
void scroll(int);
void write_moves();
void update_line_number();
char** copy_board(char**);
void map_boards();
void draw_board();
void edit();
void clear_move();
void set_mode(int);
/**
 * Cursor
 */
struct Cursor cursor;

/**
 * First line visible in file view
 */
int first_line = 0;

/**
 * Map of moves.
 */
char* move_map[1000][2];

/**
 * Map of boards.
 */
char** board_map[1000][2];

/**
 * Map of move validity
 */
int valid_map[1000][2];

/**
 * Input file.
 */
FILE* infile;

int main(int argc, char** argv) {
	// Check for args
	if (argc <= 1) {
		cout << "Usage: edit <file>\n";
		return 0;
	}
	// Get input file
	const char* filename = argv[1];
	infile = fopen(filename, "r");
	if (!load_config(infile)) {
		return 1;
	}
	fclose(infile);

	map_moves();
	map_boards();

	tb_init();

	// File editor header
	tb_write(0, LEFT, filename, TB_WHITE);
	tb_write(0, RIGHT, "Line:    ", TB_WHITE);
	set_mode(0);
	tb_write(2, LEFT, "========================================", TB_WHITE);
	tb_write(FILE_START_Y - 1, LEFT, !is_black_turn() ? "Red:" : "Black:", TB_WHITE);
	tb_write(FILE_START_Y - 1, RIGHT, !is_black_turn() ? "Black:" : "Red", TB_WHITE);
	// Write moves to file editor
	write_moves();
	// Set initial cursor position
	cursor.line = first_line;
	cursor.justify = LEFT;
	highlight_move(1);
	// Draw the checkerboard pattern in the terminal
	draw_board_background();
	

	struct tb_event* event = (tb_event*) malloc(sizeof(tb_event));
	// Main loop
	while(true) {
		update_line_number();
		draw_board();
		// Wait for command
		tb_poll_event(event);
		int height = tb_height();
		if (event->type == TB_EVENT_RESIZE) {
			write_moves();
			draw_board_background();
			draw_board();
			if (tb_height() < VISUAL_CURSOR)
				cursor.line = tb_height() - 1;
			highlight_move(1);
			continue;
		}

		if (event->type != TB_EVENT_KEY)
			continue;

		// Cursor down
		if (event->key == TB_KEY_ARROW_DOWN) {
			if (cursor.line <= 1000 && move_map[cursor.line + 1][cursor.justify == LEFT ? 0 : 1]) {
				if (VISUAL_CURSOR < tb_height() - 1)
					highlight_move(0);
				else
					scroll(DOWN);
				cursor.line += 1;
				highlight_move(1);
			}
		}
		// Cursor up
		else if (event->key == TB_KEY_ARROW_UP) {
			if (cursor.line > 0) {
				if (cursor.line > first_line)
					highlight_move(0);
				else
					scroll(UP);
				cursor.line -= 1;
				highlight_move(1);
			}
		}
		// Cursor right
		else if (event->key == TB_KEY_ARROW_RIGHT) {
			if (cursor.justify == LEFT) {
				highlight_move(0);
				cursor.justify = RIGHT;
				highlight_move(1);
			} else if (cursor.justify == RIGHT) {
				if (VISUAL_CURSOR < tb_height())
					highlight_move(0);
				else
					scroll(DOWN);
				cursor.line += 1;
				cursor.justify = LEFT;
				highlight_move(1);
			}
		}
		// Cursor left
		else if (event->key == TB_KEY_ARROW_LEFT) {
			if (cursor.justify == RIGHT) {
				highlight_move(0);
				cursor.justify = LEFT;
				highlight_move(1);
			} else if (cursor.justify == LEFT) {
				if (cursor.line > 0) {
					if (cursor.line > first_line)
						highlight_move(0);
					else
						scroll(UP);
					cursor.line -= 1;
					cursor.justify = RIGHT;
					highlight_move(1);
				}
			}
		}
		// Edit
		else if (event->key == TB_KEY_ENTER)
			edit();
		// Quit
		else if (event->key == TB_KEY_ESC) {
			tb_shutdown();
			break;
		}
	}
	free(event);	
	return 0;
}

void tb_write(int line, int justify, const char* string, int color) {
	int start, end;
	if (justify == LEFT) {
		start = 0;
	} else if (justify == RIGHT) {
		start = BOARD_START_X - strlen(string);
	} else {
		return;
	}
	for (int i = 0; i < strlen(string); i++) {
		tb_change_cell(start + i, line, string[i], color, TB_DEFAULT);
	}
	tb_present();
}

struct tb_cell* get_cell(int x, int y) {
	int index = y * tb_width() + x;
	return &(tb_cell_buffer()[index]);
}

void draw_board_background() {
	int square_width = 5;
	int square_height = 3;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			for (int k = 0; k < square_height; k++) {
				for (int l = 0; l < square_width; l++) {
					int cx = BOARD_START_X + x * square_width + l;
					int cy = BOARD_START_Y + y * square_height + k;
					if (cx >= tb_width() || cy >= tb_height())
						continue;
					int red_square = (x % 2 == 0 && y % 2 == 1) || (x % 2 == 1 && y % 2 == 0);
					tb_change_cell(cx, cy, ' ', TB_DEFAULT, red_square ? TB_RED : TB_BLACK);
				}
			}
		}
	}
	tb_present();
}

void map_moves() {
	Node* node = get_movelist();
	for (int i = 0; node; node = node->next) {
		move_map[i][0] = node->move;
		if (node->next) {
			node = node->next;
			move_map[i++][1] = node->move;
		}
	}
}

void map_boards() {
	// Get initial board layout
	char** board = copy_board(get_board());
	int black_turn = is_black_turn();
	int valid = 1;
	for (int i = 0; move_map[i][0]; i++) {
		for (int j = 0; j < 2 && move_map[i][j]; j++) {
			if (valid) {
				board_map[i][j] = copy_board(board);
				valid_map[i][j] = valid =  do_move(board, move_map[i][j], black_turn);
				black_turn = !black_turn;
			} else {
				board_map[i][j] = board;
			}
		}
	}
}

void highlight_move(int on) {
	char ch = 0;
	if (cursor.justify == LEFT)
		for (int i = 0; highlight_cell(i, VISUAL_CURSOR, valid_map[cursor.line][cursor.justify == LEFT ? 1 : 2] ? TB_GREEN : TB_RED, on); i++);
	else if (cursor.justify == RIGHT)
		for (int i = BOARD_START_X - 1; highlight_cell(i, VISUAL_CURSOR, valid_map[cursor.line][cursor.justify == LEFT ? 1 : 2] ? TB_GREEN : TB_RED, on); i--);
	tb_present();
}

int highlight_cell(int cx, int cy, int fg, int on) {
	char ch = get_cell(cx, cy)->ch;
	if (ch == ' ')
		return 0;
	tb_change_cell(cx, cy, ch, on ? TB_BLACK : fg, on ? TB_GREEN : TB_DEFAULT);
	return 1;
}

void scroll(int dir) {
	if (dir == DOWN) {
		first_line += 1;
	} else if (dir == UP) {
		first_line -= 1;
	}
	write_moves();
}

void draw_board() {
	char** board = board_map[cursor.line][cursor.justify == LEFT ? 1 : 2];
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++) {
			int cx = BOARD_START_X + 5 * x + 2;
			int cy = 3 * y + 1;
			char ch;
			if(board[y][x] == '.' || board[y][x] == '"')
				ch = ' ';
			else if (board[y][x] == 'r' || board[y][x] == 'b')
				ch = 'o';
			else if (board[y][x] == 'R' || board[y][x] == 'B')
				ch = '@';
			int red_space = x % 2 == 0 && y % 2 == 1 || x % 2 == 1 && y % 2 == 0;
			tb_change_cell(cx, cy, ch, (board[y][x] == 'r' || board[y][x] == 'R') ? TB_WHITE : TB_BLACK, (red_space) ? TB_RED : TB_BLACK);
		}
	tb_present();
}

void write_moves() {
	for (int i = first_line; move_map[i][0]; i++)
		tb_write(FILE_START_Y + i - first_line, LEFT, move_map[i][0], valid_map[i][0] ? TB_GREEN : TB_RED);
	for (int i = first_line; move_map[i][1]; i++)
		tb_write(FILE_START_Y + i - first_line, RIGHT, move_map[i][1], valid_map[i][1] ? TB_GREEN : TB_RED);
}

void update_line_number() {
	int n = cursor.line + 1;
	char ch1 = (n < 100) ? ' ' : n / 100 + '0';
	char ch2 = (n < 10) ? ' ' : ((n - (100 * (n / 100))) - (n % 10)) / 10 + '0';
	char ch3 = (n % 10) + '0';
	tb_change_cell(BOARD_START_X - 3, 0, ch1, TB_WHITE, TB_DEFAULT);
	tb_change_cell(BOARD_START_X - 2, 0, ch2, TB_WHITE, TB_DEFAULT);
	tb_change_cell(BOARD_START_X - 1, 0, ch3, TB_WHITE, TB_DEFAULT);
	tb_present();
}

char** copy_board(char** board) {
	char** G = (char**) malloc(8 * sizeof(char*));
	for (int i = 0; i < 8; i++)
		G[i] = (char*) malloc(8 * sizeof(char));
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			G[i][j] = board[i][j];
	return G;
}

void edit() {
	set_mode(1);
	string str = "";
	const char* original = move_map[cursor.line][cursor.justify == LEFT ? 0 : 1];
	tb_event* event = (tb_event*) malloc(sizeof(event));
	clear_move();
	// Edit mode
	while (true) {
		tb_write(FILE_START_Y + cursor.line - first_line, cursor.justify, str.c_str(), TB_WHITE);
		tb_poll_event(event);
		if (event->type != TB_EVENT_KEY)
			continue;
		// Cancel
		if (event->key == TB_KEY_ESC) {
			free(event);
			// Erase written move
			for (int i = cursor.justify == LEFT ? 0 : BOARD_START_X - 1; i < str.length(); cursor.justify == LEFT ? i++ : i--)
				tb_change_cell(i, FILE_START_Y + cursor.line - first_line, ' ', TB_WHITE, TB_DEFAULT);
			// Rewrite original move
			tb_write(FILE_START_Y + cursor.line - first_line, cursor.justify, original, TB_WHITE);
			highlight_move(1);
			set_mode(0);
			return;
		}
		// Save
		if (event->key == TB_KEY_ENTER)
			break;
		if (event->key == TB_KEY_BACKSPACE) {
			str = str.substr(0, str.length() - 1);
			clear_move();
		}
		if (event->ch)
			str += (char) event->ch;
	}
	// After edit mode
	free(event);
	if (move_valid((char*) str.c_str()))
		move_map[cursor.line][cursor.justify == LEFT ? 0 : 1] = const_cast<char*>(str.c_str());
	else {
		for (int i = cursor.justify == LEFT ? 0 : BOARD_START_X - 1; i < str.length(); cursor.justify == LEFT ? i++ : i--)
		tb_change_cell(i, FILE_START_Y + cursor.line - first_line, ' ', TB_WHITE, TB_DEFAULT);
	}
	map_boards();
	write_moves();
	highlight_move(1);
	set_mode(0);
}

void clear_move() {
	if (cursor.justify == LEFT)
		for (int i = 0; get_cell(i, cursor.line)->ch != ' '; i++)
			tb_change_cell(i, FILE_START_Y + cursor.line - first_line, ' ', TB_WHITE, TB_DEFAULT);
	else if (cursor.justify == RIGHT)
		for (int i = BOARD_START_X - 1; get_cell(i, cursor.line)->ch != ' '; i--)
			tb_change_cell(i, FILE_START_Y + cursor.line - first_line, ' ', TB_WHITE, TB_DEFAULT);
}

void set_mode(int mode) {
	for (int i = 0; i < BOARD_START_X; i++) {
		tb_change_cell(i, 1, ' ', TB_DEFAULT, TB_DEFAULT);
	}
	if (mode == 0) {
		tb_write(1, LEFT, "Arrow keys: move cursor", TB_WHITE);
		tb_write(1, RIGHT, "<ESC>: Quit", TB_WHITE);
	} else if (mode == 1) {
		tb_write(1, LEFT, "<ENTER>: Save", TB_WHITE);
		tb_write(1, RIGHT, "<ESC>: Cancel", TB_WHITE);
	}
}
