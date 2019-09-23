#include <stdio.h>
#include "inpututil.h"

int main() {
	char token[100];
	/* equals() test
	printf("1 : %d\n", equals("foo", "foo"));
	printf("1 : %d\n", equals("bar", "bar"));
	printf("0 : %d\n", equals("foo", "foobar"));
	printf("0 : %d\n", equals("foo", "fop"));
	*/
	
	next_token(token, 100);
	while (!equals(token, "")) {
		printf("%s\n", token);
		next_token(token, 100);
	}
}
