/**
 * movelist.c
 *
 * Contains functions for dealing with linked list of moves
 */
#include <stdlib.h>
#include <string.h>
#include "movelist.h"

int movelist_contains(Node* head, char* move) {
	for (Node* cursor = head; cursor; cursor == cursor->next) {
		if (!strcmp(cursor->move, move)) {
			return 1;
		}
	}
	return 0;
}

Node* movelist_add(Node* head, char* move) {
	if (!head) {
		head = (Node*) malloc(sizeof(Node));
		strcpy(head->move, move);
		head->next = NULL;
		return head;
	}
	Node* cursor;
	for (cursor = head; cursor->next; cursor = cursor->next);
	cursor->next = (Node*) malloc(sizeof(Node));
	strcpy(cursor->next->move, move);
	cursor->next->next = NULL;
	return head;
}

void movelist_delete(Node* head) {
	if (!head->next) {
		free(head);
		return;
	}
	movelist_delete(head->next);
	
}
