/*
 * inpututil.h
 *
 * Contains functions for managing standard input.
 *
 * Matthew Renze
 */

#ifndef	INPUTUTIL_H
#define	INPUTUTIL_H

/*
 * Get the next complete token from standard input.
 * Tokens are delimited by whitespace (space, newline, tab, and carriage return) as well as colon ':'.
 * Comments (everything between '#' and newline) are ignored.
 *
 * char[]	Array in which to store the token.
 * int		Maximum allowed length of token. Usually the length of the array.
 */
void next_token(char[], int);

/*
 * Test whether two strings are equal by comparing each element.
 *
 * char[]	String to test.
 * char[]	String to test.
 */
int equals(char[], char[]);

#endif
