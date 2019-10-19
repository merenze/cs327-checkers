#include <stdio.h>
#include "gameutil.h"

int main(int argc, char** argv) {
	FILE* infile = NULL;
	FILE* efile = NULL;
	FILE* hfile = NULL;
	unsigned max = -1;
	// Parse args
	for (int i = 0; i < argc; i++) {
		// Check for switch
		if (argv[i][0] == '-') {
			// Check for arg following switch
			if (i < argc - 1) {
				switch (argv[i][1]) {
					case 'e':
						efile = fopen(char[++i], "w");
						break;
					case 'h':
						hfile = fopen(char[++i], "w");
						break;
					case 'm':
						max = atoi(char[++i]);
						break;
				}
			}
		}
		// If no switch, arg is an input file
		else {
			infile = fopen(char[i], "r");
		}
	}
	// If no input file argument is given, use standard I/O.
	if (!infile) {
		infile = stdin;
	}
	
}
