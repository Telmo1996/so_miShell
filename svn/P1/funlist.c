#include "funlist.h"

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

char * ConvierteModo (mode_t m, char *permisos){
	strcpy (permisos,"---------- ");
	
	permisos[0]=LetraTF(m);
	if (m&S_IRUSR) permisos[1]='r';  /*propietario*/
	if (m&S_IWUSR) permisos[2]='w';
	if (m&S_IXUSR) permisos[3]='x';
	if (m&S_IRGRP) permisos[4]='r';   /*grupo*/
	if (m&S_IWGRP) permisos[5]='w';
	if (m&S_IXGRP) permisos[6]='x';
	if (m&S_IROTH) permisos[7]='r';   /*resto*/
	if (m&S_IWOTH) permisos[8]='w';
	if (m&S_IXOTH) permisos[9]='x';
	if (m&S_ISUID) permisos[3]='s';  /*setuid, setgid y stickybit*/
	if (m&S_ISGID) permisos[6]='s';
	if (m&S_ISVTX) permisos[9]='t';

	return permisos;
}

char * ConvierteModo2 (mode_t m){
	static char permisos[12];
	strcpy (permisos,"---------- ");
	
	permisos[0]=LetraTF(m);
	if (m&S_IRUSR) permisos[1]='r';  /*propietario*/
	if (m&S_IWUSR) permisos[2]='w';
	if (m&S_IXUSR) permisos[3]='x';
	if (m&S_IRGRP) permisos[4]='r';   /*grupo*/
	if (m&S_IWGRP) permisos[5]='w';
	if (m&S_IXGRP) permisos[6]='x';
	if (m&S_IROTH) permisos[7]='r';   /*resto*/
	if (m&S_IWOTH) permisos[8]='w';
	if (m&S_IXOTH) permisos[9]='x';
	if (m&S_ISUID) permisos[3]='s';  /*setuid, setgid y stickybit*/
	if (m&S_ISGID) permisos[6]='s';
	if (m&S_ISVTX) permisos[9]='t';
	
	return (permisos);
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

	//Comprobar si path corresponde a un directorio
	if( stat(path,&s) == 0 ){
		tipo = LetraTF(s.st_mode);
	}else{
		printf("NO");
		return;
	}
	if(tipo != 'd'){
		printf("no soy un dir");
		//TODO sacar la imfo del archivo e imprimirla.
		return;
	}

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
				(strcmp("..", direntp->d_name)!=0) &&
				(opR || recu < 1)
			){
				printf("%s%s:\n", tabs, direntp->d_name);
				recu++;
				imprimirDirectorio(currArchivo, opL, opD, opH, opR);
				recu--;
			}else{
				if(opL){
					//datelastmodified inodenumber owner group 
					//mode_drwx_format size (number_of_links) name
					printf("%lld.%.9ld", (long long)s.st_mtim.tv_sec, s.st_mtim.tv_nsec);
					printf("%s%s\t%ld\t%d\t%d\t%s\t%ld\t%ld\t%s\n", tabs, 
						/*s.st_mtim, */"hoy", s.st_ino, s.st_uid, s.st_gid, //TODO add fecha
						ConvierteModo2(s.st_mode), s.st_size, s.st_nlink, direntp->d_name
					);
				}else{
					printf("%s%ld\t%s\n", tabs, s.st_size, direntp->d_name);
				}
			}
		}
	}
}

int cmdList(int argc, char *argv[]){
    char opL=0, opD=0, opH=0, opR=0;
	char path[256];
	char names[256][256];
	int i, j, numNames=0;

	//Comprobación de la opciones
    for(i=1; i<argc; i++){
        if (strcmp(argv[i], "-long") == 0) opL=1;
        else if (strcmp(argv[i], "-dir") == 0) opD=1;
        else if (strcmp(argv[i], "-hid") == 0) opH=1;
        else if (strcmp(argv[i], "-rec") == 0) opR=1;
		else{
			strcpy(names[numNames], argv[i]);
			numNames++;
		}
    }

	if(numNames == 0){
		//Inicializar path con el directorio actual
		if (getcwd(path, sizeof(path)) == NULL) {
			perror("getcwd() error");
			return 1;
		}

		imprimirDirectorio(path, opL, opD, opH, opR);
	}else{
		for(j=0; j<numNames; j++){
			printf("%s:\n", names[j]);
			imprimirDirectorio(names[j], opL, opD, opH, opR);
		}
	}

	return 0;
}
