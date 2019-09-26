/*
 * inpututil.h
 *
 * Contains utility functions for handling standard input.
 *
 * Matthew Renze
 */


/*
 * Gets the next token from standard input.
 * Tokens are delimited by space, newline, tab, and carriage return.
 * Skips commented lines.
 *
 * char[]	String into which token should be loaded.
 * int		Maximum length of token. Usually equal to size of the passed string.
 */
int next_token(char[], int);

/*
 * Returns 1 when two strings are equal, else returns 0.
 * Strings must have a terminating null character.
 *
 * char[]	String to test.
 * char[]	String to test.
 */
int equals(char[], char[]);
