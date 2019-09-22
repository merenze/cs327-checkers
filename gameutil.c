/*
 * gameutil.c
 *
 * Matthew Renze
 */

#include <stdio.h>
#include "gameutil.h"
#include "inpututil.h"

#define LENGTH 100;	// Maximum allowed length of a token.

char[8][8] board;
int valid = 1;
int loaded = 0;
boolean black_turn;

void load_config() {
	if (loaded) {
		return;
	}

	char token[LENGTH];
	next_token(token, LENGTH);
	rules.multiple_jumps = 0;
	
	while (!equals(token, "") && !equals(token, "BOARD")) {
		if (equals(token, "RULES")) {
			// Check for capture.
			next_token(token, LENGTH);
			if (equals(token, "no")) {
				next_token(token, LENGTH);
				if (equals(token, "capture")) {
					no_capture = 1;
					next_token(token, LENGTH);
				} else {
					valid = 0;
					break;
				}
			} else if (equals(token, "capture")) {
				rules.no_capture = 0;
			} else if (equals(token, "multiple")) {
				next_token(token, LENGTH);
				if (equals(token, "jumps")) {
					multiple_jumps = 1;
				}
			} else {
				valid = 0;
				break;
			}
			
		} else if (equals(token, "TURN")) {
			next_token(token, LENGTH);
			if (equals(token, "red")) {
				black_turn = 0;
			} else if (equals(token, "black")) {
				black_turn = 1;
			} else {
				valid = 0;
				break;
			}
		}
	}
	loaded = 1;
	if (!valid) {
		return;
	}
}
