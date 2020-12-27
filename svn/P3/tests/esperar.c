#include <stdio.h>
#include <unistd.h>

/*
int main(int argc, char *argv[]){
    int i;
	printf("hola");
    for(i=0;i<10;i++){
        printf("%d",i);
        sleep(1);
    }
}
*/
int main( int argc, char* argv[] )
{
	int i;
	for(i=0; i<10; i++)
	{
		sleep(1);
		printf( "%d\n", i);
	}
}
