/*
 * moveutil.c
 *
 * Matthew Renze
 */
#include <stdio.h>
#include <string.h>
#include "movelist.h"
#include "gameutil.h"

FILE* logfile;
int e(int);

int main(int argc, char** argv) {
	printf("Attempting to open logfile... ");
	logfile = fopen("rankmoves.log", "w");
	printf(logfile ? "Opened\n" : "Failed\n");
	FILE* infile = NULL;
	char* d_string;
	int D = 0;
	int verbose = 0;
	// Parse args
	fprintf(logfile, "Begin parsing args\n");
	fflush(logfile);
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
			fprintf(logfile, "Switch delimiter found\n");
			fflush(logfile);
			if (argv[i][1] == 'd' && i < argc - 1) {
				fprintf(logfile, "Switch -d found\n");
				fflush(logfile);
				int len = strlen(argv[++i]);
				for (int j = 0; j < len; j++) {
					if (argv[i][j] < '0' || argv[i][j] > '9')
						continue;
					fprintf(logfile, "Getting D...");
					fflush(logfile);
					D += (argv[i][j] - '0') * e(len - j - 1);
					fprintf(logfile, "D set to %d\n", D);
					fflush(logfile);
				}
			} else if (argv[i][1] == 'v' && i < argc - 1) {
				fprintf(logfile, "Switch -v found, verbose set to 1\n");
				fflush(logfile);
				verbose = 1;
			} else {
				printf("rankmoves: Unknown switch '%s'\n", argv[i]);
				return 1;
			}
		} else {
			fprintf(logfile, "Attempting to open file '%s'... ", argv[i]);
			fflush(logfile);
			infile = fopen(argv[i], "r");
			fprintf(logfile, (infile) ? "File opened\n" : "Failed\n");
			fflush(logfile);
		}
	}
	if (!infile)
		infile = stdin;
	fprintf(logfile, "Loading config\n");
	fflush(logfile);
	load_config(infile);
	fprintf(logfile, "Config loaded\n");
	fflush(logfile);
	fclose(infile);
	fprintf(logfile, "Getting possible moves for %s\n", is_black_turn() ? "black" : "red");
	fflush(logfile);
	Node* moves = get_possible_moves(board, is_black_turn());
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
