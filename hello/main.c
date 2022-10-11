#include <stdlib.h>
#include <stdio.h>

extern long timeval;

int main(int argc, char **argv)
{
	printf("Hello soft arm9!\n");
	int j = 0;
	printf("timeval: %d\n", timeval);

	for (int i = 0; i < 100000000; i++)
	{
		j++;
	}

	printf("timeval: %d\n", timeval);

	//while (1);

	return 0;
}
