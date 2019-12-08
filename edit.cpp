/**
 * edit.cpp
 *
 * Part 4
 * Matthew Renze
 */
#include <iostream>
#include <fstream>
#include <string.h>
#include <termbox.h>
//#include "gameutil.h"

using namespace std;

/**
 * Coordinates used for splitting the box into two screens
 */
#define FILE_START_X 0		// Inclusive
#define FILE_START_Y 5 		// Inclusive
#define FILE_END_Y 83		// Exclusive
#define BOARD_START_X 40	// Inclusive
#define BOARD_START_Y 0		// Inclusive
#define BOARD_END_X 66		// Exclusive
#define BOARD_END_Y 25		// Exclusive
/**
 * Text justification
 */
#define LEFT -1
#define RIGHT 1

struct Cursor {
	int line;
	int justify;
};

void tb_write(int, int, const char*, int);
void draw_board_background();
struct tb_cell* get_cell(int, int);

FILE* file;

int main(int argc, char** argv) {
	if (argc <= 1) {
		cout << "Usage: edit <file>\n";
		return 0;
	}
	const char* filename = argv[1];
	//file = fopen(filename, "r");
	//load_config(file);
	tb_init();

	// File editor header
	tb_write(0, LEFT, filename, TB_WHITE);
	tb_write(1, LEFT, "Arrow keys: move cursor", TB_WHITE);
	tb_write(1, RIGHT, "<ESC>: Quit", TB_WHITE);
	tb_write(2, LEFT, "========================================", TB_WHITE);
	tb_write(4, LEFT, "Red:", TB_WHITE);
	tb_write(4, RIGHT, "Black:", TB_WHITE);
	
	draw_board_background();
	

	struct tb_event* event = (tb_event*) malloc(sizeof(tb_event));
	// Main loop
	while(true) {
		tb_poll_event(event);

		// Quit
		if (event->type == TB_EVENT_KEY && event->key == TB_KEY_ESC) {
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
					int red_square = (x % 2 == 0 && y % 2 == 1) || (x % 2 == 1 && y % 2 == 0);
					tb_change_cell(cx, cy, ' ', TB_DEFAULT, red_square ? TB_RED : TB_BLACK);
				}
			}
		}
	}
	tb_present();
}
