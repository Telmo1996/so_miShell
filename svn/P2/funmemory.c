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
	int tam;

	void * puntero;

    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-allocate")==0) opA=1;
        else if (strcmp(argv[i], "-dealloc")==0) opD=1;
        else if (strcmp(argv[i], "-deletekey")==0) opDlt=1;
        else if (strcmp(argv[i], "-show")==0) opS=1;
        else if (strcmp(argv[i], "-show-vars")==0) opSv=1;
        else if (strcmp(argv[i], "-show-funcs")==0) opSf=1;
        else if (strcmp(argv[i], "-pmap")==0) opP=1;
        else if (strcmp(argv[i], "-dopmap")==0) opDo=1;
        else if (strcmp(argv[i], "-malloc")==0) opMa=1;
        else if (strcmp(argv[i], "-mmap")==0) opMm=1;
        else if (strcmp(argv[i], "-createshared")==0) opCs=1;
        else if (strcmp(argv[i], "-shared")==0) opSh=1;
        else if (strcmp(argv[i], "-all")==0) opAll=1;
    }
	printf("%d%d%d%d%d%d%d%d %d%d%d%d%d\n", opA,opD,opDlt,opS,opSv,opSf,opP,opDo,
		opMa,opMm,opCs,opSh,opAll);
	//Comprobar que las opciones sean validas
	if(opA+opD+opDlt+opS+opSv+opSf+opP+opDo > 1){
		printf("Modos incompatibles\n");
		return 1;
	}
	if(opMa+opMm+opCs+opSh+opAll > 1){
		printf("Opciones incompatibles\n");
		return 1;
	}

	if(opA && opMa){
		if(argc <= 3){
			printf("Imprimiento lista:\n");
			memPrintList(memLista, 'a');
		}else if(argc == 4){
			tam = atoi(argv[3]);
			puntero = (void *)malloc(tam);
			memInsertElement(puntero, tam, 'a', memLista);
			printf("Allocated %d at %p\n", tam, puntero);
		}else{
			printf("Argumentos incorrectos\n");
			return 1;
		}
	}

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
