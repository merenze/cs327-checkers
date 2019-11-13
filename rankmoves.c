/*
 * rankmoves.c
 *
 * Matthew Renze
 */
#include <stdio.h>
#include <string.h>
#include "movelist.h"
#include "gameutil.h"

FILE* rankmoves_logfile;
int e(int);

int main(int argc, char** argv) {
	rankmoves_logfile = fopen("rankmoves.log", "w");
	FILE* infile = NULL;
	char* d_string;
	int D = 0;
	int verbose = 0;
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
				int len = strlen(argv[++i]);
				fprintf(rankmoves_logfile, "Getting D... ");
				for (int j = 0; j < len; j++) {
					if (argv[i][j] < '0' || argv[i][j] > '9')
						continue;
					D += (argv[i][j] - '0') * e(len - j - 1);
				}
				fprintf(rankmoves_logfile, "%d\n", D);
			} else if (argv[i][1] == 'v' && i < argc - 1) {
				fprintf(rankmoves_logfile, "Switch -v found, verbose set to 1\n");
				fflush(rankmoves_logfile);
				verbose = 1;
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
	for (Node* cursor = moves; cursor; cursor = cursor->next)
		printf("%s for %s: score %d\n", cursor->move, is_black_turn() ? "black" : "red", get_score_for_move(board, cursor->move, is_black_turn(), D)); 

	return 0;
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
