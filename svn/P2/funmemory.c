#include "funmemory.h"

int cmdMemory(int argc, char *argv[]){
	char opA=0;		//allocate
	char opD=0;		//dealloc
	char opDlt=0;	//deletekey
	char opS=0;		//show
	char opSv=0;	//show-vars
	char opSf=0;	//show-funcs
	char opP=0;		//pmap
	char opDo=0;	//dopmap

	char opMa=0;	//malloc
	char opMm=0;	//mmap
	char opCs=0;	//createshared
	char opSh=0;	//shared
	char opAll=0;	//all

	int i;

    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-allocate")) opA=1;
        else if (strcmp(argv[i], "-dealloc")) opD=1;
        else if (strcmp(argv[i], "-deletekey")) opDlt=1;
        else if (strcmp(argv[i], "-show")) opS=1;
        else if (strcmp(argv[i], "-show-vars")) opSv=1;
        else if (strcmp(argv[i], "-show-funcs")) opSf=1;
        else if (strcmp(argv[i], "-pmap")) opP=1;
        else if (strcmp(argv[i], "-dopmap")) opDo=1;
        else if (strcmp(argv[i], "-malloc")) opMa=1;
        else if (strcmp(argv[i], "-mmap")) opMm=1;
        else if (strcmp(argv[i], "-createshared")) opCs=1;
        else if (strcmp(argv[i], "-shared")) opSh=1;
        else if (strcmp(argv[i], "-all")) opAll=1;
    }
	//Comprobar que las opciones sean validas
	if(opA+opD+opDlt+opS+opSv+opSf+opP+opDo > 1){
		printf("Opciones incompatibles\n");
		return 1;
	}
	if(opMa+opMm+opCs+opSh+opAll > 1){
		printf("Opciones incompatibles\n");
		return 1;
	}

	printPwd();	


	return 0;
}


int cmdMemdump(int argc, char *argv[]){
	printf("soy memdump\n");
	return 0;
}
int cmdMemfill(int argc, char *argv[]){
	printf("soy memfill\n");
	return 0;
}
int cmdRecurse(int argc, char *argv[]){
	printf("soy recurse\n");
	return 0;
}
int cmdReadfile(int argc, char *argv[]){
	printf("soy readfile\n");
	return 0;
}
int cmdWritefile(int argc, char *argv[]){
	printf("soy writefile\n");
	return 0;
}
