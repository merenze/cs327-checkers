/**
 * movelist.h
 */
#ifndef MOVELIST_H
#define MOVELIST_H
typedef struct node {
	char move[100];
	struct node* prev;
	struct node* next;
} Node;

Node* movelist_add(Node*, char*);

void movelist_unlink(Node*);

int movelist_contains(Node*, char*);

void movelist_sort(Node*);

void movelist_delete(Node*);

Node* movelist_get_head();

// Public for testing
void swap(Node* n1, Node* n2);

#endif
