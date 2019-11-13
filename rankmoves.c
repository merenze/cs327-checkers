/*
 * rankmoves.c
 *
 * Matthew Renze
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "movelist.h"
#include "gameutil.h"

FILE* rankmoves_logfile;
void brief(Node*, int);
void verbose(char[8][8], Node*, int, int, int);
int e(int);

int main(int argc, char** argv) {
	rankmoves_logfile = fopen("rankmoves.log", "w");
	FILE* infile = NULL;
	char* d_string;
	int D = 0;
	int v = 0;
	// Parse args
	fprintf(rankmoves_logfile, "Begin parsing args\n");
	fflush(rankmoves_logfile);
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
			fprintf(rankmoves_logfile, "Switch delimiter found\n");
			fflush(rankmoves_logfile);
			if (argv[i][1] == 'd' && i < argc - 1) {
				fprintf(rankmoves_logfile, "Switch -d found\n");
				fflush(rankmoves_logfile);
				fprintf(rankmoves_logfile, "Getting D... ");
				D = atoi(argv[++i]);
				fprintf(rankmoves_logfile, "%d\n", D);
			} else if (argv[i][1] == 'v' && i < argc - 1) {
				fprintf(rankmoves_logfile, "Switch -v found, verbose set to 1\n");
				fflush(rankmoves_logfile);
				v = 1;
			} else {
				printf("rankmoves: Unknown switch '%s'\n", argv[i]);
				return 1;
			}
		} else {
			fprintf(rankmoves_logfile, "Attempting to open file '%s'... ", argv[i]);
			fflush(rankmoves_logfile);
			infile = fopen(argv[i], "r");
			fprintf(rankmoves_logfile, (infile) ? "File opened\n" : "Failed\n");
			fflush(rankmoves_logfile);
		}
	}
	if (!infile)
		infile = stdin;
	fprintf(rankmoves_logfile, "Loading config... ");
	fflush(rankmoves_logfile);
	if(!load_config(infile)) {
		fprintf(rankmoves_logfile, "Invalid config.\n");
		return 1;
	}
	fprintf(rankmoves_logfile, "Done\n");
	fflush(rankmoves_logfile);
	fclose(infile);
	fprintf(rankmoves_logfile, "Getting possible moves for %s... ", is_black_turn() ? "black" : "red");
	fflush(rankmoves_logfile);
	Node* moves = get_possible_moves(board, is_black_turn());
	fprintf(rankmoves_logfile, "Done.\n");
	fflush(rankmoves_logfile);
	fprintf(rankmoves_logfile, "Returned list with head at %s\n", moves ? moves->move : "NULL");
	fflush(rankmoves_logfile);
	if (!moves) 
		printf("The current player has no moves available.\n");
	v ? verbose(board, moves, D, is_black_turn(), 0) : brief(moves, D);
	return 0;
}
/**
 * Print brief output
 */
void brief(Node* moves, int D) {
	for (Node* cursor = moves; cursor; cursor = cursor->next)
		printf("%s for %s: %d\n", cursor->move, is_black_turn() ? "black" : "red", get_score_for_move(board, cursor->move, is_black_turn(), D));
}
/**
 * Print verbose output
 */
void verbose(char G[8][8], Node* moves, int D, int do_black, int depth) {
	// Copy board
	char Gp[8][8];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)

	// Get indent
	for (int i = 0; i < depth; i++)
		printf("    ");
	
}

/*
 * Return 10 ^ x, for positive ints x
 */
int e(int x) {
	if (x <= 0)
		return 1;
	int result = 10;
	for (int i = 1; i < x; i++)
		result *= 10;
	return result;
}
