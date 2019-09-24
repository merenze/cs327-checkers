/* 
 * stringutil.c
 *
 * Mathew Renze
 */
//#include <stdio.h> // DEBUG
#include "stringutil.h"

int sequals(char s1[], char s2[]) {
//	printf("Call to sequals(\"%s\", \"%s\")\n", s1, s2); // DEBUG
	if (slength(s1) != slength(s2)) {
//		printf("\tResult: 0.\n"); // DEBUG
		return 0;
	}
	for (int i = 0; s1[i] != 0; i++) {
		if (s1[i] != s2[i]) {
//			printf("\tResult: 0.\n."); // DEBUG
			return 0;
		}
	}
//`	printf("Result: 1.\n"); // DEBUG
	return 1;
}

int slength(char s[]) {
	int i;
	for (i = 0; s[i] != 0; i++);
	return i - 1;
}
