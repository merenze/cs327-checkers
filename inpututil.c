#include <stdio.h>

const char whitespace[] = " :\n\t\r";	// Array of token delimiters.
int line;				// The line number we're on.

int is_whitespace(char);	// Checks if a char is whitespace. Used to delimit tokens.
char _getc(FILE*);		// Wraps getc, and increments line if result = '\n'
void skip_line(FILE*, char*);	// Used to ignore commented lines

int next_token(FILE* infile, char* token, int max) {
	char ch;	// Where we'll be storing result of getchar().
	line = 1;	// Keeping track of the line number
	// Skip leading whitespace and comments.
	while (is_whitespace(ch = _getc(infile)) || ch == '#') {
		// Skip commented lines.
		if (ch == '#') {
			skip_line(infile, &ch);
		}
	}
	// Check for EOF.
	if (ch == EOF) {
		return 0;
	}
	// We've already loaded one character into ch, since that was our signal to stop the previous loop.
	token[0] = ch;
	// Which would make 1 our starting index. Declared outside the loop so it's easier to terminate the string.
	int i = 1;

	// Main loop for loading characters into token. Max - 1 guarentees room for terminating character.
	while (i < max - 1 && !is_whitespace(ch = _getc(infile))) {
		if (ch == EOF) {
			return 0;
		}
		// Since comments continue until \n or EOF, they always signal the end of a token.
		if (ch == '#') {
			skip_line(infile, &ch);
			i++;
			break;
		}
		token[i++] = ch;
	}
	// Terminate string.
	token[i] = 0;
	return 1;
}

int is_whitespace(char ch) {
	for (int i = 0; whitespace[i] != 0; i++) {
		if (ch == whitespace[i]) {
			return 1;
		}
	}
	return 0;
}
 
void skip_line(FILE* infile, char* ch_ptr) {
	while (((*ch_ptr) = _getc(infile)) != '\n');
}

int get_line() {
	return line;
}

char _getc(FILE* infile) {
	char result = getc(infile);
	if (result == '\n') {
		line++;
	}
	return result;
}
