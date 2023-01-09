#include <stdlib.h>
#include <stdio.h>

extern long timeval;

int main(int argc, char **argv)
{
	printf("Hello soft arm9!\n");

	printf("timeval: %d\n", timeval);

	printf("%d\n", (int)12.3f);
	printf("%6.1f\n", 12.3f);
	//printf("%d\n", (int)12.3f);

	while (1);

	return 0;
}
