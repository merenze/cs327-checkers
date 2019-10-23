#include <stdio.h>
#include <stdlib.h>
#include "gameutil.h"

FILE* efile;
FILE* hfile;

void create_exchange(Node*);
void create_human_readable();

int main(int argc, char** argv) {
	FILE* infile = NULL;
	efile = NULL;
	hfile = NULL;
	int max = 100;
	// Parse args
	for (int i = 0; i < argc; i++) {
		// Check for switch
		if (argv[i][0] == '-') {
			// Check for arg following switch
			if (i < argc - 1) {
				switch (argv[i][1]) {
					case 'e':
						efile = fopen(argv[++i], "w");
						break;
					case 'h':
						hfile = fopen(argv[++i], "w");
						break;
					case 'm':
						max = atoi(argv[++i]);
						break;
				}
			}
		}
		// If no switch, arg is an input file
		else {
			infile = fopen(argv[i], "r");
		}
	}
	// If no input file argument is given, use standard I/O.
	if (!infile) {
		infile = stdin;
	}
	int j = 1;
	//TODO: Handle errors
	if (!load_config(infile)) {
		return 1;
	}
	// Do moves
	printf("Input file has %d moves\nProcessing %d moves\n", get_num_moves(), max < get_num_moves() ? max : get_num_moves());
	Node* cursor = get_move_list();
	for (int j = 1; cursor && j++ <= max; cursor = cursor->next) {
		if (!do_move(cursor->move)) {
			printf("Move %d is illegal: %s\n", j, cursor->move);	
			break;
		}
	}
	printf("All moves legal\n");
	// Output unprocessed moves
	create_exchange(cursor);
	// Output human readable
	create_human_readable();
}

void create_exchange(Node* cursor) {
	if (!efile) {
		return;
	}
	for (; cursor; cursor = cursor->next) {
		fprintf(efile, "%s\n", cursor->move);
	}
}

void create_human_readable() {
	if (!hfile) {
		return;
	}
	char* border = "  +---+---+---+---+---+---+---+---+ \n";
	fprintf(hfile, "    a   b   c   d   e   f   g   h    \n%s", border);
	for (int row = 0; row < 8; row++) {
		fprintf(hfile, "%d |", 8 - row);
		for(int col = 0; col < 8; col++) {
			fprintf(hfile, " %c |", board[row][col]);
		}
		fprintf(hfile, " %d\n%s", 8 - row, border);
	}
	fprintf(hfile, "   a   b   c   d   e   f   g   h   \n");
}
