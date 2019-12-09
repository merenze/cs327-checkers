#include <stdio.h>
#include <stdlib.h>
#include "gameutil.h"
#include "inpututil.h"
#include "movelist.h"

void print_movelist(Node*);
void test_movelist_swap_1();
void test_movelist_swap();

FILE* logfile;

int main() {
	load_config(stdin);
	print_movelist(get_movelist());
	return 0;
}

void print_movelist(Node* movelist) {
	for (Node* cursor = movelist; cursor; cursor = cursor->next) {
		printf("%s ", cursor->move);
	}
	printf("\n");
}
