#include "list.h"

int recu=0;

char LetraTF (mode_t m){
	switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
		case S_IFSOCK:
			return 's';  /*socket */
		case S_IFLNK:
			return 'l';    /*symbolic link*/
		case S_IFREG:
			return '-';   /* fichero normal*/
		case S_IFBLK:
			return 'b';   /*block device*/
		case S_IFDIR:
			return 'd';   /*directorio */
		case S_IFCHR:
			return 'c';   /*char  device*/
		case S_IFIFO:
			return 'p';   /*pipe*/
		default:
			return '?';	/*desconocido, no deberia aparecer*/
	}
}

void imprimirDirectorio(char path[256], char opL, char opD, char opH, char opR){
	char muestra=0;
    DIR *dirp;
    struct dirent *direntp;
	char tipo;
	char tabs[100] = "";
	char currArchivo[256];
	struct stat s;

	for(int j=0; j<recu; j++)
		strcat(tabs, "|\t");

	//Abrir el directorio
	dirp = opendir(path);
	if(dirp == NULL){
		return;
	}

	while((direntp = readdir(dirp)) != NULL){
		memset(currArchivo, '\0', sizeof(currArchivo));
		strcpy(currArchivo, path);
		strcat(strcat(currArchivo, "/"), direntp->d_name);
		//Leer tipo
		if( stat(currArchivo,&s) == 0 ){
			tipo = LetraTF(s.st_mode);
		}
		
		muestra = 0;
		if((direntp->d_name[0] != '.') || opH) //Archivos ocultos
			muestra=1;
		if(muestra){ //Imprime dependiendo de -long
			if(tipo == 'd' && 
				opD && 
				(strcmp(".", direntp->d_name)!=0) && 
				(strcmp("..", direntp->d_name)!=0)
			){
				printf("%s%s:\n", tabs, direntp->d_name);
				recu++;
				imprimirDirectorio(currArchivo, opL, opD, opH, opR);
				recu--;
				//printf("%s-----------\n", tabs);
			}else{
				if(opL){
					printf("%s%ld\t%ld\t%d\t%c\t%s\n", tabs, 
						direntp->d_ino, direntp->d_off,
						direntp->d_reclen, tipo, direntp->d_name
					);
				}else{
					printf("%s%s\n", tabs, direntp->d_name);
				}
			}
		}
	}
}

int cmdList(int argc, char *argv[]){
    char opL=0, opD=0, opH=0, opR=0;
	char path[256];
	int i;

	//Comprobación de la opciones
    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-long") == 0) opL=1;
        else if (strcmp(argv[i], "-dir") == 0) opD=1;
        else if (strcmp(argv[i], "-hid") == 0) opH=1;
        else if (strcmp(argv[i], "-rec") == 0) opR=1;
    }

	//Inicializar path con el directorio actual
	if (getcwd(path, sizeof(path)) == NULL) {
		perror("getcwd() error");
		return 1;
	}

	//Imprimir la cabecera de la opción -long
	if(opL)
		printf("i-node\toffset\t\t\tlong\ttipo\tnombre\n");

	
	
	imprimirDirectorio(path, opL, opD, opH, opR);

	return 0;
}
