#include <stdio.h>
#include <unistd.h>

int main()
{
	char c;

	printf("I am the target and my pid is: %d\n", getpid());
	read(0, &c, 1);
	puts("The end.");
	return (0);
}
