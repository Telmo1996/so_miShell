#include "funcionesCmd.h"

int cmdAutores(int argc, char *argv[]){
	char opL=0, opN=0;
	int i;
	for(i=1; i<argc; i++){
		if (strcmp(argv[i], "-l")) opL=1;
        else if (strcmp(argv[i], "-n")) opN=1;
    }
    if (argc==1) opL=opN=1;

    if (opL) printf("anna.taboada@udc.es\n");
    if (opL) printf("telmo.fcorujo@udc.es\n");
    if (opN) printf("Anna Taboada Pardiñas\n");
	if (opN) printf("Telmo Fernandez Corujo\n");

	return 0;
}
