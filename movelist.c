/**
 * movelist.c
 *
 * Contains functions for dealing with linked list of moves
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "movelist.h"

FILE* movelist_logfile;

int movelist_contains(Node* head, char* move) {
	movelist_logfile = fopen("movelist.log", "a");
	fprintf(movelist_logfile, "Call to movelist_contains(%s)\n", move);
	fflush(movelist_logfile);
	for (Node* cursor = head; cursor; cursor == cursor->next) {
		if (!strcmp(cursor->move, move)) {
			return 1;
		}
	}
	fclose(movelist_logfile);
	return 0;
}

Node* movelist_add(Node* head, char* move) {
	movelist_logfile = fopen("movelist.log", "a");
	fprintf(movelist_logfile, "Call to movelist_add(%s)\n", move);
	fflush(movelist_logfile);
	if (!head) {
		fprintf(movelist_logfile, "movelist_add: Creating new movelist... ");
		fflush(movelist_logfile);
		head = (Node*) malloc(sizeof(Node));
		strcpy(head->move, move);
		head->next = NULL;
		fprintf(movelist_logfile, "Added %s\n", head->move);
		fclose(movelist_logfile);
		return head;
	}
	fprintf(movelist_logfile, "movelist_add: Adding to existing movelist... ");
	fflush(movelist_logfile);
	Node* cursor;
	for (cursor = head; cursor->next; cursor = cursor->next);
	cursor->next = (Node*) malloc(sizeof(Node));
	strcpy(cursor->next->move, move);
	cursor->next->next = NULL;
	fprintf(movelist_logfile, "Added %s\n", cursor->next->move);
	fclose(movelist_logfile);
	return head;
}

void movelist_delete(Node* head) {
	if (!head->next) {
		free(head);
		return;
	}
	movelist_delete(head->next);
	
}
