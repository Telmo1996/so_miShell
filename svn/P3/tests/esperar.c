#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
	pid_t pid=getpid();
	int i;

	printf("%d\n",pid);
	for(i=0; i<atoi(argv[1]); i++)
	{
		sleep(1);
		printf( "%d\n", i);
	}
}
