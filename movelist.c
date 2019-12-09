/**
 * movelist.c
 *
 * Contains functions for dealing with linked list of moves
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "movelist.h"


Node* movelist_add(Node* head, char* move) {
	if (!head) {
//		printf("Head null. Creating new node... ");
		head = (Node*) malloc(sizeof(Node));
		strcpy(head->move, move);
		head->next = NULL;
//		printf("Created with head %s\n", head->move);
		return head;
	}
//	printf("Head initialized... ");
	Node* cursor;
	for (cursor = head; cursor->next; cursor = cursor->next);
	cursor->next = (Node*) malloc(sizeof(Node));
	strcpy(cursor->next->move, move);
	cursor->next->next = NULL;

//	printf("Move %s added to list with head %s\n", head->move, cursor->next->move);
	return head;
}

void movelist_delete(Node* head) {
	if (!head->next) {
		free(head);
		return;
	}
	movelist_delete(head->next);
	
}
