/*
 * inpututil.c
 *
 * Matthew Renze
 */

#include <stdio.h>
#include "inpututil.h"

int iswhitespace(char);

void skipline(char*);

const char whitespace[] = " :\n\t\r";

void next_token(char token[], int size) {
	char ch;	// Char being operated on.

	// Skip comments and leading whitespace.
	while (iswhitespace(ch == getchar()) || ch == '#') {
		if (ch == '#') {
			skipline(&ch);
		} // Else move cursor to next char; done in loop.
	}

	token[0] = ch;	// We already have the first char of our token.
	int i;		// Declared outside loop in order to facilitate adding of terminating null char.
	
	// Main loop to load chars into token.
	for (i = 1; i < size - 1 && !iswhitespace(ch = getchar()) && ch != EOF; i++) {
		// Since comments continue until newline, they always denote the end of a token.
		if (ch == '#') {
			skipline(&ch);
			i++;
			break;
		}
		token[i] = ch;
		
	}
	token[i] = 0;
}

int equals(char s1[], char s2[]) { 
	// Compare string length first
	int l1, l2;
	for (l1 = 0; s1[l1] != 0; l1++);
	for (l2 = 0; s2[l2] != 0; l2++);
	if (l1 != l2) {
		return 0;
	}
	// Now check each arg
	for (int i = 0; s1[i] != 0 && s2[i] != 0; i++) {
		if (s1[i] != s2[i]) {
			return 0;
		}
	}
	return 1;
}

int iswhitespace(char ch) {
	for (int i = 0; whitespace[i] != 0; i++) {
		if (ch == whitespace[i]) {
			return 1;
		}
	}
	return 0;
}

void skipline(char* ch_ptr) {
	while ((*ch_ptr = getchar()) != '\n');
}
