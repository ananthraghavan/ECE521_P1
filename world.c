#include <stdio.h>

void hello_world(int argc, char *argv[]) {

	unsigned int i;
	printf("\n\nHello World!\n\n");

	for (i = 0; i < (unsigned int)argc; i++)
	   printf("argument #%d = %s\n", i, argv[i]);

	printf("\n\n");
}
