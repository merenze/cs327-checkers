#include <stdio.h>

const char whitespace[] = " :\n\t\r";

int is_whitespace(char);

void skip_line(char*);

int end_reached = 0;

void next_token(char token[], int max) {
	if (end_reached) {
		token[0] = 0;
		return;
	}
	char ch;	// Where we'll be storing result of getchar().

	// Skip leading whitespace and comments.
	while (is_whitespace(ch = getchar()) || ch == '#') {
		// Skip commented lines.
		if (ch == '#') {
			skip_line(&ch);
		}
	}
	// Check for EOF.
	if (ch == EOF) {
		token[0] = 0;
		return;
	}
	// We've already loaded one character into ch, since that was our signal to stop the previous loop.
	token[0] = ch;
	// Which would make 1 our starting index. Declared outside the loop so it's easier to terminate the string.
	int i = 1;

	// Main loop for loading characters into token. Max - 1 guarentees room for terminating character.
	while (i < max - 1 && !is_whitespace(ch = getchar()) && ch != EOF) {
		// Since comments continue until \n or EOF, they always signal the end of a token.
		if (ch == '#') {
			skip_line(&ch);
			i++;
			break;
		}
		token[i++] = ch;
	}
	if (ch == EOF) {
		end_reached = 1;
	}
	// Terminate string.
	token[i] = 0;
}

int is_whitespace(char ch) {
	for (int i = 0; whitespace[i] != 0; i++) {
		if (ch == whitespace[i]) {
			return 1;
		}
	}
	return 0;
}
 
void skip_line(char* ch_ptr) {
	while (((*ch_ptr) = getchar()) != '\n');
}
