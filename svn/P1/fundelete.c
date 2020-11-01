#include "fundelete.h"

int isDirectoryEmpty(char *path) {
	int n = 0;
	struct dirent *d;
	DIR *dir = opendir(path);
	if (dir == NULL) //Not a directory or doesn't exist
		return 1;
	while ((d = readdir(dir)) != NULL) {
		if(++n > 2)
			break;
	}
	closedir(dir);
	if (n <= 2) //Directory Empty
		return 1;
	else
		return 0;
}

void rmDir(char opR, char *path){
	struct dirent *d;
	DIR *dir = opendir(path);
	struct stat s;
	char tipo;
	char tmpName[256];
	int j;

	if(isDirectoryEmpty(path)){
		rmdir(path);
	}else{
		if(opR){
			//Bucle para borrar cada archivo
			if (dir == NULL)
				return;
			while ((d = readdir(dir)) != NULL){
				//comprobar que no sea . o ..
				if(!(strcmp(d->d_name, ".")==0 || strcmp(d->d_name, "..")==0)){
					//Comprobar si path corresponde a un directorio
					strcpy(tmpName, path);
					strcat(strcat(tmpName, "/"), d->d_name);
					if( stat(tmpName,&s) == 0 ){
						tipo = LetraTF(s.st_mode);
					}else{
						printf("No esxiste: %s\n", tmpName);
						break;
					}

					if(tipo == '-'){
						unlink(tmpName);
					}else if(tipo == 'd'){
						rmDir(opR, tmpName);
					}else{
						printf("No conozco el tipo");
					}
				}
			}
			closedir(dir);
			rmdir(path);
		}else{
			printf("%s no está vacio.\n", path);
		}
	}
}

int cmdDelete(int argc, char *argv[]){
	char opR=0;
	char names[256][256];
	char *argsList[256]={"list", "."};
	int i, j, numNames=0;
	struct stat s;
	char tipo;
	char path[256];
	char tmpName[256];

    //Comprobación de la opciones
    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-rec") == 0) opR=1;
        else{
            strcpy(names[numNames], argv[i]);
            numNames++;
        }
    }

	if(numNames == 0){
		cmdList(2, argsList);
		return 0;
	}

	//Inicializar path con el directorio actual
	if (getcwd(path, sizeof(path)) == NULL) {
		perror("getcwd() error");
		return 1;
	}

	for(j=0; j<numNames; j++){
		//Comprobar si path corresponde a un directorio
		if( stat(names[j],&s) == 0 ){
			tipo = LetraTF(s.st_mode);
		}else{
			printf("No esxiste: %s\n", names[j]);
			break;
		}

		if(tipo == '-'){
			unlink(names[j]);
		}else if(tipo == 'd'){
			strcpy(tmpName, path);
			strcat(strcat(tmpName, "/"), names[j]);
			rmDir(opR, tmpName);
		}else{
			printf("No conozco el tipo");
		}
		
	}
}
