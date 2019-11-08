#include <stdio.h>
#include <stdlib.h>
// #include "gameutil.h"
#include "inpututil.h"
#include "movelist.h"

void print_movelist(Node*);
void test_movelist_swap_1();
void test_movelist_swap();

FILE* logfile;

int main() {
	test_movelist_swap();
	return 0;
}

void test_movelist_swap() {
	Node* movelist = movelist_add(NULL, "1");

	movelist_add(movelist, "2");
	movelist_add(movelist, "3");
	movelist_add(movelist, "4");
	print_movelist(movelist);

	test_movelist_swap_1(movelist);
/*
	Node* n1 = movelist;
	Node* n2 = movelist->next->next;
	swap(n1, n2);
	print_movelist(movelist);
	
	n1 = movelist->next;
	n2 = movelist->next->next->next;
	swap(n1, n2);
	print_movelist(movelist);

	n1 = movelist;
	n2 = movelist->next;
	swap(n1, n2);
	print_movelist(movelist);

	n1 = movelist->next;
	n2 = movelist->next->next;
	swap(n1, n2);
	print_movelist(movelist);

	n1 = movelist->next->next;
	n2 = movelist->next->next->next;
	swap(n1, n2);
	print_movelist(movelist);*/
}

void test_movelist_swap_1(Node* movelist) {
	Node* n1 = movelist;
	Node* n2 = n1->next;
	swap(n1, n2);
	movelist = movelist_get_head(movelist);
	print_movelist(movelist);
}

void print_movelist(Node* movelist) {
	for (Node* cursor = movelist; cursor; cursor = cursor->next) {
		printf("%s ", cursor->move);
	}
	printf("\n");
}
